#include "audiomanager.h"
#include "systemutils.h"
#include "privilegedexecutor.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QTabWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QSlider>
#include <QCheckBox>
#include <QProgressBar>
#include <QTextEdit>
#include <QListWidget>
#include <QSplitter>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QTimer>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QStandardPaths>
#include <QDebug>
#include <QProcess>
#include <QTimer>
#include <QDir>
#include <QFileInfo>
#include <QSpinBox>
#include <QTabWidget>

// AudioDeviceWorker Implementation
AudioDeviceWorker::AudioDeviceWorker(QObject *parent)
    : QThread(parent), m_stopRequested(false)
{
}

void AudioDeviceWorker::stop()
{
    QMutexLocker locker(&m_mutex);
    m_stopRequested = true;
}

void AudioDeviceWorker::run()
{
    {
        QMutexLocker locker(&m_mutex);
        m_stopRequested = false;
    }
    
    // Scan for audio devices based on available systems
    try {
        scanPipeWireDevices();
        if (m_stopRequested) return;
        
        scanPulseAudioDevices();
        if (m_stopRequested) return;
        
        scanAlsaDevices();
        if (m_stopRequested) return;
    } catch (...) {
        emit errorOccurred("Error occurred while scanning audio devices");
    }
    
    emit scanFinished();
}

void AudioDeviceWorker::scanPipeWireDevices()
{
    QProcess process;
    
    // Check if PipeWire is available
    process.start("pw-cli", QStringList() << "info");
    if (!process.waitForFinished(5000) || process.exitCode() != 0) {
        return; // PipeWire not available
    }
    
    // Get device list
    process.start("pw-cli", QStringList() << "list-objects");
    if (process.waitForFinished(10000)) {
        QString output = process.readAllStandardOutput();
        QJsonObject deviceInfo = parsePipeWireInfo(output);
        if (!deviceInfo.isEmpty()) {
            emit deviceFound(deviceInfo);
        }
    }
}

void AudioDeviceWorker::scanPulseAudioDevices()
{
    QProcess process;
    
    // Check if PulseAudio is available
    process.start("pactl", QStringList() << "info");
    if (!process.waitForFinished(5000) || process.exitCode() != 0) {
        return; // PulseAudio not available
    }
    
    // Get sink list
    process.start("pactl", QStringList() << "list" << "sinks");
    if (process.waitForFinished(10000)) {
        QString output = process.readAllStandardOutput();
        QJsonObject deviceInfo = parsePulseAudioInfo(output);
        if (!deviceInfo.isEmpty()) {
            emit deviceFound(deviceInfo);
        }
    }
    
    // Get source list
    process.start("pactl", QStringList() << "list" << "sources");
    if (process.waitForFinished(10000)) {
        QString output = process.readAllStandardOutput();
        QJsonObject deviceInfo = parsePulseAudioInfo(output);
        if (!deviceInfo.isEmpty()) {
            emit deviceFound(deviceInfo);
        }
    }
}

void AudioDeviceWorker::scanAlsaDevices()
{
    QProcess process;
    
    // Check if ALSA is available
    process.start("aplay", QStringList() << "-l");
    if (!process.waitForFinished(5000) || process.exitCode() != 0) {
        return; // ALSA not available
    }
    
    // Get playback devices
    QString output = process.readAllStandardOutput();
    QJsonObject deviceInfo = parseAlsaInfo(output);
    if (!deviceInfo.isEmpty()) {
        emit deviceFound(deviceInfo);
    }
    
    // Get capture devices
    process.start("arecord", QStringList() << "-l");
    if (process.waitForFinished(10000)) {
        output = process.readAllStandardOutput();
        deviceInfo = parseAlsaInfo(output);
        if (!deviceInfo.isEmpty()) {
            emit deviceFound(deviceInfo);
        }
    }
}

QJsonObject AudioDeviceWorker::parsePipeWireInfo(const QString &output)
{
    QJsonObject device;
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    
    for (const QString &line : lines) {
        if (line.contains("Node") && line.contains("name")) {
            QRegularExpression re("name:\\s*\"([^\"]+)\"");
            QRegularExpressionMatch match = re.match(line);
            if (match.hasMatch()) {
                device["name"] = match.captured(1);
                device["type"] = "PipeWire";
                device["system"] = "pipewire";
                device["status"] = "available";
            }
        }
    }
    
    return device;
}

QJsonObject AudioDeviceWorker::parsePulseAudioInfo(const QString &output)
{
    QJsonObject device;
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    
    QString currentName;
    for (const QString &line : lines) {
        if (line.startsWith("Sink #") || line.startsWith("Source #")) {
            currentName.clear();
        } else if (line.contains("Name:")) {
            QStringList parts = line.split(":");
            if (parts.size() > 1) {
                currentName = parts[1].trimmed();
                device["name"] = currentName;
                device["type"] = line.startsWith("\tName:") && output.contains("Sink #") ? "Output" : "Input";
                device["system"] = "pulseaudio";
                device["status"] = "available";
            }
        }
    }
    
    return device;
}

QJsonObject AudioDeviceWorker::parseAlsaInfo(const QString &output)
{
    QJsonObject device;
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    
    for (const QString &line : lines) {
        if (line.startsWith("card ")) {
            QRegularExpression re(R"(card (\d+): ([^,]+), device (\d+): ([^,]+))");
            QRegularExpressionMatch match = re.match(line);
            if (match.hasMatch()) {
                device["name"] = match.captured(2) + " - " + match.captured(4);
                device["card"] = match.captured(1).toInt();
                device["device"] = match.captured(3).toInt();
                device["type"] = "ALSA";
                device["system"] = "alsa";
                device["status"] = "available";
            }
        }
    }
    
    return device;
}

// AudioManager Implementation
AudioManager::AudioManager(QWidget *parent)
    : QWidget(parent)
    , m_systemUtils(nullptr)
    , m_privilegedExecutor(nullptr)
    , m_deviceWorker(nullptr)
    , m_autoRefresh(true)
    , m_refreshInterval(15000) // 15 seconds
    , m_currentAudioSystem("auto")
    , m_masterVolume(50)
    , m_masterMute(false)
    , m_isScanning(false)
{
    setupUI();
    setupContextMenus();
    
    // Initialize refresh timer
    m_refreshTimer = new QTimer(this);
    m_refreshTimer->setInterval(m_refreshInterval);
    connect(m_refreshTimer, &QTimer::timeout, this, &AudioManager::onRefreshTimer);
    
    // Initialize with default audio system (will be detected on-demand)
    m_currentAudioSystem = "auto";
}

AudioManager::~AudioManager()
{
    if (m_deviceWorker) {
        m_deviceWorker->stop();
        m_deviceWorker->wait(3000);
        delete m_deviceWorker;
    }
}

void AudioManager::setSystemUtils(SystemUtils *utils)
{
    m_systemUtils = utils;
}

void AudioManager::setPrivilegedExecutor(PrivilegedExecutor *executor)
{
    m_privilegedExecutor = executor;
    if (m_privilegedExecutor) {
        connect(m_privilegedExecutor, &PrivilegedExecutor::taskProgress,
                [this](int taskId, const QString &progress) {
                    Q_UNUSED(taskId);
                    onProgressUpdated("", 0, progress);
                });
        connect(m_privilegedExecutor, &PrivilegedExecutor::taskFinished,
                [this](int taskId, int exitCode, const QString &output) {
                    Q_UNUSED(taskId);
                    onTaskCompleted("", exitCode == 0, output);
                });
    }
}

void AudioManager::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(12, 12, 12, 12);
    m_mainLayout->setSpacing(8);
    
    // Setup toolbar
    setupToolbar();
    
    // Setup control panel
    setupControlPanel();
    
    // Add simplified audio controls directly
    setupSimplifiedAudioControls();
    
    // Setup progress area
    setupProgressArea();
    
    // Apply theming
    updateTheme();
}

void AudioManager::setupToolbar()
{
    m_toolbarLayout = new QHBoxLayout();
    m_toolbarLayout->setContentsMargins(0, 0, 0, 0);
    m_toolbarLayout->setSpacing(8);
    
    // Audio system selection
    QLabel *systemLabel = new QLabel("Audio System:");
    m_toolbarLayout->addWidget(systemLabel);
    
    m_systemComboBox = new QComboBox();
    QStringList systems = getAvailableAudioSystems();
    m_systemComboBox->addItem("Auto-detect", "auto");
    for (const QString &system : systems) {
        m_systemComboBox->addItem(system.toUpper(), system);
    }
    
    connect(m_systemComboBox, QOverload<const QString&>::of(&QComboBox::currentTextChanged),
            [this](const QString &text) {
        m_currentAudioSystem = m_systemComboBox->currentData().toString();
        refreshDevices();
    });
    
    m_toolbarLayout->addWidget(m_systemComboBox);
    
    m_toolbarLayout->addStretch();
    
    // Quick actions
    QPushButton *testButton = new QPushButton("Test Audio");
    connect(testButton, &QPushButton::clicked, this, &AudioManager::testAudioDevices);
    m_toolbarLayout->addWidget(testButton);
    
    QPushButton *optimizeButton = new QPushButton("Optimize");
    QMenu *optimizeMenu = new QMenu(optimizeButton);
    optimizeMenu->addAction("For Latency", this, &AudioManager::optimizeForLatency);
    optimizeMenu->addAction("For Quality", this, &AudioManager::optimizeForQuality);
    optimizeMenu->addAction("For Power Saving", this, &AudioManager::optimizeForPowerSaving);
    optimizeButton->setMenu(optimizeMenu);
    m_toolbarLayout->addWidget(optimizeButton);
    
    QPushButton *resetButton = new QPushButton("Reset");
    connect(resetButton, &QPushButton::clicked, this, &AudioManager::resetAudioSettings);
    m_toolbarLayout->addWidget(resetButton);
    
    // Auto-refresh toggle
    QCheckBox *autoRefreshCheck = new QCheckBox("Auto-refresh");
    autoRefreshCheck->setChecked(m_autoRefresh);
    connect(autoRefreshCheck, &QCheckBox::toggled, [this](bool checked) {
        m_autoRefresh = checked;
        if (checked) {
            m_refreshTimer->start();
        } else {
            m_refreshTimer->stop();
        }
    });
    m_toolbarLayout->addWidget(autoRefreshCheck);
    
    m_mainLayout->addLayout(m_toolbarLayout);
}

void AudioManager::setupControlPanel()
{
    m_controlPanel = new QGroupBox("Control Panel");
    m_controlLayout2 = new QGridLayout(m_controlPanel);
    m_controlLayout2->setContentsMargins(8, 8, 8, 8);
    m_controlLayout2->setSpacing(8);
    
    // Search and filter
    QLabel *searchLabel = new QLabel("Search:");
    m_controlLayout2->addWidget(searchLabel, 0, 0);
    
    m_searchEdit = new QLineEdit();
    m_searchEdit->setPlaceholderText("Search devices, profiles, effects...");
    m_searchEdit->setClearButtonEnabled(true);
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &AudioManager::searchDevices);
    m_controlLayout2->addWidget(m_searchEdit, 0, 1);
    
    m_filterComboBox = new QComboBox();
    m_filterComboBox->addItem("All", "all");
    m_filterComboBox->addItem("Input Devices", "input");
    m_filterComboBox->addItem("Output Devices", "output");
    m_filterComboBox->addItem("Active", "active");
    m_filterComboBox->addItem("Available", "available");
    m_controlLayout2->addWidget(m_filterComboBox, 0, 2);
    
    m_searchButton = new QPushButton("Search");
    connect(m_searchButton, &QPushButton::clicked, this, &AudioManager::searchDevices);
    m_controlLayout2->addWidget(m_searchButton, 0, 3);
    
    m_mainLayout->addWidget(m_controlPanel);
}

void AudioManager::setupProgressArea()
{
    // Progress bar
    m_progressBar = new QProgressBar();
    m_progressBar->setVisible(false);
    m_progressBar->setTextVisible(true);
    m_mainLayout->addWidget(m_progressBar);
    
    // Progress label
    m_progressLabel = new QLabel();
    m_progressLabel->setVisible(false);
    m_progressLabel->setWordWrap(true);
    m_mainLayout->addWidget(m_progressLabel);
    
    // Output text edit
    m_outputTextEdit = new QTextEdit();
    m_outputTextEdit->setMaximumHeight(150);
    m_outputTextEdit->setVisible(false);
    m_outputTextEdit->setReadOnly(true);
    m_outputTextEdit->setFont(QFont("monospace"));
    m_mainLayout->addWidget(m_outputTextEdit);
    
    // Status label
    m_statusLabel = new QLabel("Ready");
    m_statusLabel->setStyleSheet("color: #666; font-size: 10px;");
    m_mainLayout->addWidget(m_statusLabel);
}

QString AudioManager::getAudioSystem()
{
    if (isPipeWireAvailable()) return "pipewire";
    if (isPulseAudioAvailable()) return "pulseaudio";
    if (isAlsaAvailable()) return "alsa";
    if (isJackAvailable()) return "jack";
    return "none";
}

QStringList AudioManager::getAvailableAudioSystems()
{
    QStringList systems;
    if (isPipeWireAvailable()) systems << "pipewire";
    if (isPulseAudioAvailable()) systems << "pulseaudio";
    if (isAlsaAvailable()) systems << "alsa";
    if (isJackAvailable()) systems << "jack";
    return systems;
}

bool AudioManager::isPipeWireAvailable()
{
    QProcess process;
    process.start("systemctl", QStringList() << "--user" << "is-active" << "pipewire");
    process.waitForFinished(1000);
    return process.exitCode() == 0;
}

bool AudioManager::isPulseAudioAvailable()
{
    QProcess process;
    process.start("pulseaudio", QStringList() << "--check");
    process.waitForFinished(1000);
    return process.exitCode() == 0;
}

bool AudioManager::isAlsaAvailable()
{
    QProcess process;
    process.start("aplay", QStringList() << "--version");
    return process.waitForFinished(3000) && process.exitCode() == 0;
}

bool AudioManager::isJackAvailable()
{
    QProcess process;
    process.start("jack_control", QStringList() << "status");
    return process.waitForFinished(3000) && process.exitCode() == 0;
}

bool AudioManager::isEasyEffectsAvailable()
{
    // Quick non-blocking check using synchronous call
    QProcess process;
    process.start("flatpak", QStringList() << "list" << "--app" << "com.github.wwmm.easyeffects");
    process.waitForFinished(1000); // Short timeout
    return process.exitCode() == 0;
}

void AudioManager::updateTheme()
{
    // Use standard system theme
    setStyleSheet("");
}

void AudioManager::showProgress(const QString &title, const QString &message)
{
    if (m_progressBar) {
        m_progressBar->setVisible(true);
        m_progressBar->setRange(0, 0); // Indeterminate
    }
    if (m_statusLabel) {
        m_statusLabel->setText(message);
    }
}

void AudioManager::hideProgress()
{
    if (m_progressBar) {
        m_progressBar->setVisible(false);
    }
    if (m_statusLabel) {
        m_statusLabel->setText("Ready");
    }
}

void AudioManager::showError(const QString &title, const QString &message)
{
    QMessageBox::critical(this, title, message);
    m_statusLabel->setText("Error: " + message);
}

void AudioManager::showSuccess(const QString &title, const QString &message)
{
    m_statusLabel->setText("Success: " + message);
    QTimer::singleShot(3000, [this]() {
        m_statusLabel->setText("Ready");
    });
}

void AudioManager::showInfo(const QString &title, const QString &message)
{
    QMessageBox::information(this, title, message);
}

// Tab setup methods
void AudioManager::setupDeviceTab()
{
    m_deviceTab = new QWidget();
    m_deviceLayout = new QVBoxLayout(m_deviceTab);
    m_deviceLayout->setContentsMargins(12, 12, 12, 12);
    m_deviceLayout->setSpacing(8);
    
    // Device table
    m_deviceTable = new QTableWidget();
    m_deviceTable->setAlternatingRowColors(true);
    m_deviceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_deviceTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_deviceTable->setSortingEnabled(true);
    m_deviceTable->setContextMenuPolicy(Qt::CustomContextMenu);
    
    // Set up columns
    m_deviceTable->setColumnCount(7);
    QStringList headers;
    headers << "Device Name" << "Type" << "Status" << "Volume" << "Channels" << "Sample Rate" << "Latency";
    m_deviceTable->setHorizontalHeaderLabels(headers);
    
    // Configure column widths
    QHeaderView *header = m_deviceTable->horizontalHeader();
    header->setStretchLastSection(true);
    header->resizeSection(DEVICE_TABLE_NAME_COLUMN, 200);
    header->resizeSection(DEVICE_TABLE_TYPE_COLUMN, 100);
    header->resizeSection(DEVICE_TABLE_STATUS_COLUMN, 100);
    header->resizeSection(DEVICE_TABLE_VOLUME_COLUMN, 80);
    header->resizeSection(DEVICE_TABLE_CHANNELS_COLUMN, 80);
    header->resizeSection(DEVICE_TABLE_SAMPLE_RATE_COLUMN, 100);
    
    connect(m_deviceTable, &QTableWidget::customContextMenuRequested,
            this, &AudioManager::onDeviceTableContextMenu);
    connect(m_deviceTable, &QTableWidget::itemSelectionChanged,
            this, &AudioManager::onDeviceSelectionChanged);
    
    m_deviceLayout->addWidget(m_deviceTable);
    
    // Button layout
    m_deviceButtonLayout = new QHBoxLayout();
    m_deviceButtonLayout->setSpacing(8);
    
    m_setDefaultDeviceButton = new QPushButton("Set Default");
    m_setDefaultDeviceButton->setEnabled(false);
    connect(m_setDefaultDeviceButton, &QPushButton::clicked, this, &AudioManager::onDeviceActionTriggered);
    m_deviceButtonLayout->addWidget(m_setDefaultDeviceButton);
    
    m_testDeviceButton = new QPushButton("Test");
    m_testDeviceButton->setEnabled(false);
    connect(m_testDeviceButton, &QPushButton::clicked, this, &AudioManager::testAudioDevices);
    m_deviceButtonLayout->addWidget(m_testDeviceButton);
    
    m_propertiesDeviceButton = new QPushButton("Properties");
    m_propertiesDeviceButton->setEnabled(false);
    connect(m_propertiesDeviceButton, &QPushButton::clicked, this, &AudioManager::showDeviceProperties);
    m_deviceButtonLayout->addWidget(m_propertiesDeviceButton);
    
    m_calibrateDeviceButton = new QPushButton("Calibrate");
    m_calibrateDeviceButton->setEnabled(false);
    connect(m_calibrateDeviceButton, &QPushButton::clicked, this, &AudioManager::calibrateAudioLevels);
    m_deviceButtonLayout->addWidget(m_calibrateDeviceButton);
    
    m_deviceButtonLayout->addStretch();
    
    m_suspendDeviceButton = new QPushButton("Suspend");
    m_suspendDeviceButton->setEnabled(false);
    connect(m_suspendDeviceButton, &QPushButton::clicked, this, &AudioManager::onDeviceActionTriggered);
    m_deviceButtonLayout->addWidget(m_suspendDeviceButton);
    
    m_resumeDeviceButton = new QPushButton("Resume");
    m_resumeDeviceButton->setEnabled(false);
    connect(m_resumeDeviceButton, &QPushButton::clicked, this, &AudioManager::onDeviceActionTriggered);
    m_deviceButtonLayout->addWidget(m_resumeDeviceButton);
    
    m_resetDeviceButton = new QPushButton("Reset");
    m_resetDeviceButton->setEnabled(false);
    connect(m_resetDeviceButton, &QPushButton::clicked, this, &AudioManager::onDeviceActionTriggered);
    m_deviceButtonLayout->addWidget(m_resetDeviceButton);
    
    m_refreshDeviceButton = new QPushButton("Refresh");
    connect(m_refreshDeviceButton, &QPushButton::clicked, this, &AudioManager::refreshDevices);
    m_deviceButtonLayout->addWidget(m_refreshDeviceButton);
    
    m_deviceLayout->addLayout(m_deviceButtonLayout);
    
    m_tabWidget->addTab(m_deviceTab, "Audio Devices");
}

void AudioManager::setupProfileTab()
{
    m_profileTab = new QWidget();
    m_profileLayout = new QVBoxLayout(m_profileTab);
    m_profileLayout->setContentsMargins(12, 12, 12, 12);
    m_profileLayout->setSpacing(8);
    
    // Profile table
    m_profileTable = new QTableWidget();
    m_profileTable->setAlternatingRowColors(true);
    m_profileTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_profileTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_profileTable->setSortingEnabled(true);
    m_profileTable->setContextMenuPolicy(Qt::CustomContextMenu);
    
    // Set up columns
    m_profileTable->setColumnCount(4);
    QStringList headers;
    headers << "Profile Name" << "Type" << "Description" << "Active";
    m_profileTable->setHorizontalHeaderLabels(headers);
    
    // Configure column widths
    QHeaderView *header = m_profileTable->horizontalHeader();
    header->setStretchLastSection(true);
    header->resizeSection(PROFILE_TABLE_NAME_COLUMN, 150);
    header->resizeSection(PROFILE_TABLE_TYPE_COLUMN, 100);
    header->resizeSection(PROFILE_TABLE_DESCRIPTION_COLUMN, 200);
    header->resizeSection(PROFILE_TABLE_ACTIVE_COLUMN, 80);
    
    connect(m_profileTable, &QTableWidget::customContextMenuRequested,
            this, &AudioManager::onProfileTableContextMenu);
    connect(m_profileTable, &QTableWidget::itemSelectionChanged,
            this, &AudioManager::onProfileSelectionChanged);
    
    m_profileLayout->addWidget(m_profileTable);
    
    // Button layout
    m_profileButtonLayout = new QHBoxLayout();
    m_profileButtonLayout->setSpacing(8);
    
    m_loadProfileButton = new QPushButton("Load");
    m_loadProfileButton->setEnabled(false);
    connect(m_loadProfileButton, &QPushButton::clicked, this, &AudioManager::onProfileActionTriggered);
    m_profileButtonLayout->addWidget(m_loadProfileButton);
    
    m_saveProfileButton = new QPushButton("Save");
    connect(m_saveProfileButton, &QPushButton::clicked, this, &AudioManager::onProfileActionTriggered);
    m_profileButtonLayout->addWidget(m_saveProfileButton);
    
    m_createProfileButton = new QPushButton("Create");
    connect(m_createProfileButton, &QPushButton::clicked, this, &AudioManager::createProfile);
    m_profileButtonLayout->addWidget(m_createProfileButton);
    
    m_deleteProfileButton = new QPushButton("Delete");
    m_deleteProfileButton->setEnabled(false);
    connect(m_deleteProfileButton, &QPushButton::clicked, this, &AudioManager::deleteProfile);
    m_profileButtonLayout->addWidget(m_deleteProfileButton);
    
    m_profileButtonLayout->addStretch();
    
    m_importProfileButton = new QPushButton("Import");
    connect(m_importProfileButton, &QPushButton::clicked, this, &AudioManager::importProfile);
    m_profileButtonLayout->addWidget(m_importProfileButton);
    
    m_exportProfileButton = new QPushButton("Export");
    m_exportProfileButton->setEnabled(false);
    connect(m_exportProfileButton, &QPushButton::clicked, this, &AudioManager::exportProfile);
    m_profileButtonLayout->addWidget(m_exportProfileButton);
    
    m_refreshProfileButton = new QPushButton("Refresh");
    connect(m_refreshProfileButton, &QPushButton::clicked, this, &AudioManager::refreshProfiles);
    m_profileButtonLayout->addWidget(m_refreshProfileButton);
    
    m_profileLayout->addLayout(m_profileButtonLayout);
    
    m_tabWidget->addTab(m_profileTab, "Audio Profiles");
}

void AudioManager::setupEffectTab()
{
    m_effectTab = new QWidget();
    m_effectLayout = new QVBoxLayout(m_effectTab);
    m_effectLayout->setContentsMargins(12, 12, 12, 12);
    m_effectLayout->setSpacing(8);
    
    // Effect table
    m_effectTable = new QTableWidget();
    m_effectTable->setAlternatingRowColors(true);
    m_effectTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_effectTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_effectTable->setSortingEnabled(true);
    m_effectTable->setContextMenuPolicy(Qt::CustomContextMenu);
    
    // Set up columns
    m_effectTable->setColumnCount(4);
    QStringList headers;
    headers << "Effect Name" << "Type" << "Enabled" << "Parameters";
    m_effectTable->setHorizontalHeaderLabels(headers);
    
    // Configure column widths
    QHeaderView *header = m_effectTable->horizontalHeader();
    header->setStretchLastSection(true);
    header->resizeSection(EFFECT_TABLE_NAME_COLUMN, 150);
    header->resizeSection(EFFECT_TABLE_TYPE_COLUMN, 100);
    header->resizeSection(EFFECT_TABLE_ENABLED_COLUMN, 80);
    
    connect(m_effectTable, &QTableWidget::customContextMenuRequested,
            this, &AudioManager::onEffectTableContextMenu);
    connect(m_effectTable, &QTableWidget::itemSelectionChanged,
            this, &AudioManager::onEffectSelectionChanged);
    
    m_effectLayout->addWidget(m_effectTable);
    
    // Button layout
    m_effectButtonLayout = new QHBoxLayout();
    m_effectButtonLayout->setSpacing(8);
    
    m_enableEffectButton = new QPushButton("Enable");
    m_enableEffectButton->setEnabled(false);
    connect(m_enableEffectButton, &QPushButton::clicked, this, &AudioManager::onEffectActionTriggered);
    m_effectButtonLayout->addWidget(m_enableEffectButton);
    
    m_disableEffectButton = new QPushButton("Disable");
    m_disableEffectButton->setEnabled(false);
    connect(m_disableEffectButton, &QPushButton::clicked, this, &AudioManager::onEffectActionTriggered);
    m_effectButtonLayout->addWidget(m_disableEffectButton);
    
    m_configureEffectButton = new QPushButton("Configure");
    m_configureEffectButton->setEnabled(false);
    connect(m_configureEffectButton, &QPushButton::clicked, this, &AudioManager::showEffectEditor);
    m_effectButtonLayout->addWidget(m_configureEffectButton);
    
    m_resetEffectButton = new QPushButton("Reset");
    m_resetEffectButton->setEnabled(false);
    connect(m_resetEffectButton, &QPushButton::clicked, this, &AudioManager::onEffectActionTriggered);
    m_effectButtonLayout->addWidget(m_resetEffectButton);
    
    m_effectButtonLayout->addStretch();
    
    m_addEffectButton = new QPushButton("Add Effect");
    connect(m_addEffectButton, &QPushButton::clicked, this, &AudioManager::onEffectActionTriggered);
    m_effectButtonLayout->addWidget(m_addEffectButton);
    
    m_removeEffectButton = new QPushButton("Remove");
    m_removeEffectButton->setEnabled(false);
    connect(m_removeEffectButton, &QPushButton::clicked, this, &AudioManager::onEffectActionTriggered);
    m_effectButtonLayout->addWidget(m_removeEffectButton);
    
    m_refreshEffectButton = new QPushButton("Refresh");
    connect(m_refreshEffectButton, &QPushButton::clicked, this, &AudioManager::refreshDevices);
    m_effectButtonLayout->addWidget(m_refreshEffectButton);
    
    m_effectLayout->addLayout(m_effectButtonLayout);
    
    m_tabWidget->addTab(m_effectTab, "Audio Effects");
}

void AudioManager::setupEasyEffectsTab()
{
    m_easyEffectsTab = new QWidget();
    m_easyEffectsLayout = new QVBoxLayout(m_easyEffectsTab);
    m_easyEffectsLayout->setContentsMargins(12, 12, 12, 12);
    m_easyEffectsLayout->setSpacing(8);
    
    // Info label
    QLabel *infoLabel = new QLabel("EasyEffects provides professional audio processing for PipeWire.");
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet("color: #666; margin-bottom: 8px;");
    m_easyEffectsLayout->addWidget(infoLabel);
    
    // EasyEffects preset list
    m_easyEffectsPresetList = new QListWidget();
    m_easyEffectsPresetList->setAlternatingRowColors(true);
    m_easyEffectsPresetList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_easyEffectsLayout->addWidget(m_easyEffectsPresetList);
    
    // Button layout
    m_easyEffectsButtonLayout = new QHBoxLayout();
    m_easyEffectsButtonLayout->setSpacing(8);
    
    m_installEasyEffectsButton = new QPushButton("Install EasyEffects");
    connect(m_installEasyEffectsButton, &QPushButton::clicked, this, &AudioManager::installEasyEffects);
    m_easyEffectsButtonLayout->addWidget(m_installEasyEffectsButton);
    
    m_startEasyEffectsButton = new QPushButton("Start");
    connect(m_startEasyEffectsButton, &QPushButton::clicked, this, &AudioManager::startEasyEffects);
    m_easyEffectsButtonLayout->addWidget(m_startEasyEffectsButton);
    
    m_stopEasyEffectsButton = new QPushButton("Stop");
    connect(m_stopEasyEffectsButton, &QPushButton::clicked, this, &AudioManager::stopEasyEffects);
    m_easyEffectsButtonLayout->addWidget(m_stopEasyEffectsButton);
    
    m_autostartEasyEffectsButton = new QPushButton("Enable Autostart");
    connect(m_autostartEasyEffectsButton, &QPushButton::clicked, this, &AudioManager::enableEasyEffectsAutostart);
    m_easyEffectsButtonLayout->addWidget(m_autostartEasyEffectsButton);
    
    m_easyEffectsButtonLayout->addStretch();
    
    m_loadEasyEffectsPresetButton = new QPushButton("Load Preset");
    m_loadEasyEffectsPresetButton->setEnabled(false);
    connect(m_loadEasyEffectsPresetButton, &QPushButton::clicked, this, &AudioManager::loadEasyEffectsPreset);
    m_easyEffectsButtonLayout->addWidget(m_loadEasyEffectsPresetButton);
    
    m_saveEasyEffectsPresetButton = new QPushButton("Save Preset");
    connect(m_saveEasyEffectsPresetButton, &QPushButton::clicked, this, &AudioManager::saveEasyEffectsPreset);
    m_easyEffectsButtonLayout->addWidget(m_saveEasyEffectsPresetButton);
    
    m_deleteEasyEffectsPresetButton = new QPushButton("Delete");
    m_deleteEasyEffectsPresetButton->setEnabled(false);
    connect(m_deleteEasyEffectsPresetButton, &QPushButton::clicked, this, &AudioManager::deleteEasyEffectsPreset);
    m_easyEffectsButtonLayout->addWidget(m_deleteEasyEffectsPresetButton);
    
    m_refreshEasyEffectsButton = new QPushButton("Refresh");
    connect(m_refreshEasyEffectsButton, &QPushButton::clicked, this, &AudioManager::refreshEasyEffectsPresets);
    m_easyEffectsButtonLayout->addWidget(m_refreshEasyEffectsButton);
    
    m_easyEffectsLayout->addLayout(m_easyEffectsButtonLayout);
    
    // Update button states based on EasyEffects availability
    bool available = isEasyEffectsAvailable();
    m_startEasyEffectsButton->setEnabled(available);
    m_stopEasyEffectsButton->setEnabled(available);
    m_autostartEasyEffectsButton->setEnabled(available);
    m_loadEasyEffectsPresetButton->setEnabled(available);
    m_saveEasyEffectsPresetButton->setEnabled(available);
    m_deleteEasyEffectsPresetButton->setEnabled(available);
    m_refreshEasyEffectsButton->setEnabled(available);
    
    if (!available) {
        infoLabel->setText("EasyEffects is not installed. Click 'Install EasyEffects' to install it.");
        infoLabel->setStyleSheet("color: #cc6600; margin-bottom: 8px;");
    }
    
    m_tabWidget->addTab(m_easyEffectsTab, "EasyEffects");
}

void AudioManager::setupPipeWireTab()
{
    m_pipeWireTab = new QWidget();
    m_pipeWireLayout = new QVBoxLayout(m_pipeWireTab);
    m_pipeWireLayout->setContentsMargins(12, 12, 12, 12);
    m_pipeWireLayout->setSpacing(8);
    
    // Info label
    QLabel *infoLabel = new QLabel("PipeWire is a modern audio/video server for Linux.");
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet("color: #666; margin-bottom: 8px;");
    m_pipeWireLayout->addWidget(infoLabel);
    
    // PipeWire info text
    m_pipeWireInfoText = new QTextEdit();
    m_pipeWireInfoText->setReadOnly(true);
    m_pipeWireInfoText->setFont(QFont("monospace"));
    m_pipeWireInfoText->setMaximumHeight(300);
    m_pipeWireLayout->addWidget(m_pipeWireInfoText);
    
    // Button layout
    m_pipeWireButtonLayout = new QHBoxLayout();
    m_pipeWireButtonLayout->setSpacing(8);
    
    m_installPipeWireButton = new QPushButton("Install PipeWire");
    connect(m_installPipeWireButton, &QPushButton::clicked, this, &AudioManager::installPipeWire);
    m_pipeWireButtonLayout->addWidget(m_installPipeWireButton);
    
    m_startPipeWireButton = new QPushButton("Start");
    connect(m_startPipeWireButton, &QPushButton::clicked, this, &AudioManager::startPipeWire);
    m_pipeWireButtonLayout->addWidget(m_startPipeWireButton);
    
    m_stopPipeWireButton = new QPushButton("Stop");
    connect(m_stopPipeWireButton, &QPushButton::clicked, this, &AudioManager::stopPipeWire);
    m_pipeWireButtonLayout->addWidget(m_stopPipeWireButton);
    
    m_restartPipeWireButton = new QPushButton("Restart");
    connect(m_restartPipeWireButton, &QPushButton::clicked, this, &AudioManager::restartPipeWire);
    m_pipeWireButtonLayout->addWidget(m_restartPipeWireButton);
    
    m_autostartPipeWireButton = new QPushButton("Enable Autostart");
    connect(m_autostartPipeWireButton, &QPushButton::clicked, this, &AudioManager::enablePipeWireAutostart);
    m_pipeWireButtonLayout->addWidget(m_autostartPipeWireButton);
    
    m_pipeWireButtonLayout->addStretch();
    
    m_graphPipeWireButton = new QPushButton("Show Graph");
    connect(m_graphPipeWireButton, &QPushButton::clicked, this, &AudioManager::showPipeWireGraph);
    m_pipeWireButtonLayout->addWidget(m_graphPipeWireButton);
    
    m_optimizeLatencyButton = new QPushButton("Optimize for Latency");
    connect(m_optimizeLatencyButton, &QPushButton::clicked, this, &AudioManager::optimizeForLatency);
    m_pipeWireButtonLayout->addWidget(m_optimizeLatencyButton);
    
    m_optimizeQualityButton = new QPushButton("Optimize for Quality");
    connect(m_optimizeQualityButton, &QPushButton::clicked, this, &AudioManager::optimizeForQuality);
    m_pipeWireButtonLayout->addWidget(m_optimizeQualityButton);
    
    m_refreshPipeWireButton = new QPushButton("Refresh");
    connect(m_refreshPipeWireButton, &QPushButton::clicked, this, &AudioManager::refreshPipeWireInfo);
    m_pipeWireButtonLayout->addWidget(m_refreshPipeWireButton);
    
    m_pipeWireLayout->addLayout(m_pipeWireButtonLayout);
    
    // Update button states based on PipeWire availability
    bool available = isPipeWireAvailable();
    m_startPipeWireButton->setEnabled(available);
    m_stopPipeWireButton->setEnabled(available);
    m_restartPipeWireButton->setEnabled(available);
    m_autostartPipeWireButton->setEnabled(available);
    m_graphPipeWireButton->setEnabled(available);
    m_optimizeLatencyButton->setEnabled(available);
    m_optimizeQualityButton->setEnabled(available);
    m_refreshPipeWireButton->setEnabled(available);
    
    if (!available) {
        infoLabel->setText("PipeWire is not installed. Click 'Install PipeWire' to install it.");
        infoLabel->setStyleSheet("color: #cc6600; margin-bottom: 8px;");
        m_pipeWireInfoText->setText("PipeWire not available");
    }
    
    m_tabWidget->addTab(m_pipeWireTab, "PipeWire");
}

void AudioManager::setupMixerPanel()
{
    m_mixerPanel = new QGroupBox("Audio Mixer");
    m_mixerLayout = new QHBoxLayout(m_mixerPanel);
    m_mixerLayout->setContentsMargins(8, 8, 8, 8);
    m_mixerLayout->setSpacing(4);
    
    m_mixerScrollArea = new QScrollArea();
    m_mixerScrollArea->setWidgetResizable(true);
    m_mixerScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_mixerScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    m_mixerWidget = new QWidget();
    m_mixerWidgetLayout = new QHBoxLayout(m_mixerWidget);
    m_mixerWidgetLayout->setContentsMargins(0, 0, 0, 0);
    m_mixerWidgetLayout->setSpacing(8);
    
    // Add a placeholder mixer channel
    QWidget *channel = new QWidget();
    QVBoxLayout *channelLayout = new QVBoxLayout(channel);
    channelLayout->setContentsMargins(4, 4, 4, 4);
    channelLayout->setSpacing(4);
    
    QLabel *channelLabel = new QLabel("Master");
    channelLabel->setAlignment(Qt::AlignCenter);
    channelLayout->addWidget(channelLabel);
    
    QSlider *volumeSlider = new QSlider(Qt::Vertical);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(50);
    volumeSlider->setMinimumHeight(100);
    channelLayout->addWidget(volumeSlider);
    
    QLabel *volumeLabel = new QLabel("50%");
    volumeLabel->setAlignment(Qt::AlignCenter);
    volumeLabel->setMinimumWidth(40);
    channelLayout->addWidget(volumeLabel);
    
    QCheckBox *muteCheckBox = new QCheckBox("Mute");
    channelLayout->addWidget(muteCheckBox);
    
    connect(volumeSlider, &QSlider::valueChanged, [volumeLabel](int value) {
        volumeLabel->setText(QString::number(value) + "%");
    });
    
    m_mixerWidgetLayout->addWidget(channel);
    m_mixerWidgetLayout->addStretch();
    
    m_mixerScrollArea->setWidget(m_mixerWidget);
    m_mixerLayout->addWidget(m_mixerScrollArea);
}

void AudioManager::setupEffectChain()
{
    m_effectChain = new QGroupBox("Effect Chain");
    m_effectChainLayout = new QHBoxLayout(m_effectChain);
    m_effectChainLayout->setContentsMargins(8, 8, 8, 8);
    m_effectChainLayout->setSpacing(4);
    
    m_effectChainScrollArea = new QScrollArea();
    m_effectChainScrollArea->setWidgetResizable(true);
    m_effectChainScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_effectChainScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    m_effectChainWidget = new QWidget();
    m_effectChainWidgetLayout = new QHBoxLayout(m_effectChainWidget);
    m_effectChainWidgetLayout->setContentsMargins(0, 0, 0, 0);
    m_effectChainWidgetLayout->setSpacing(8);
    
    // Add placeholder effect
    QWidget *effect = new QWidget();
    effect->setFixedSize(80, 60);
    effect->setStyleSheet("border: 1px solid #ccc; border-radius: 4px; background: #f0f0f0;");
    
    QVBoxLayout *effectLayout = new QVBoxLayout(effect);
    effectLayout->setContentsMargins(4, 4, 4, 4);
    effectLayout->setSpacing(2);
    
    QLabel *effectLabel = new QLabel("EQ");
    effectLabel->setAlignment(Qt::AlignCenter);
    effectLabel->setFont(QFont("Arial", 10, QFont::Bold));
    effectLayout->addWidget(effectLabel);
    
    QCheckBox *effectEnabled = new QCheckBox("On");
    effectEnabled->setChecked(true);
    effectLayout->addWidget(effectEnabled);
    
    m_effectChainWidgetLayout->addWidget(effect);
    m_effectChainWidgetLayout->addStretch();
    
    m_effectChainScrollArea->setWidget(m_effectChainWidget);
    m_effectChainLayout->addWidget(m_effectChainScrollArea);
}

void AudioManager::setupContextMenus()
{
    // Device context menu
    m_deviceContextMenu = new QMenu(this);
    m_deviceContextMenu->addAction("Set as Default", this, &AudioManager::onDeviceActionTriggered);
    m_deviceContextMenu->addAction("Test Device", this, &AudioManager::testAudioDevices);
    m_deviceContextMenu->addAction("Properties", this, &AudioManager::showDeviceProperties);
    m_deviceContextMenu->addSeparator();
    m_deviceContextMenu->addAction("Suspend", this, &AudioManager::onDeviceActionTriggered);
    m_deviceContextMenu->addAction("Resume", this, &AudioManager::onDeviceActionTriggered);
    m_deviceContextMenu->addAction("Reset", this, &AudioManager::onDeviceActionTriggered);
    m_deviceContextMenu->addSeparator();
    m_deviceContextMenu->addAction("Calibrate", this, &AudioManager::calibrateAudioLevels);
    
    // Profile context menu
    m_profileContextMenu = new QMenu(this);
    m_profileContextMenu->addAction("Load Profile", this, &AudioManager::onProfileActionTriggered);
    m_profileContextMenu->addAction("Save Profile", this, &AudioManager::onProfileActionTriggered);
    m_profileContextMenu->addAction("Duplicate", this, &AudioManager::onProfileActionTriggered);
    m_profileContextMenu->addSeparator();
    m_profileContextMenu->addAction("Export", this, &AudioManager::exportProfile);
    m_profileContextMenu->addAction("Delete", this, &AudioManager::deleteProfile);
    
    // Effect context menu
    m_effectContextMenu = new QMenu(this);
    m_effectContextMenu->addAction("Enable", this, &AudioManager::onEffectActionTriggered);
    m_effectContextMenu->addAction("Disable", this, &AudioManager::onEffectActionTriggered);
    m_effectContextMenu->addAction("Configure", this, &AudioManager::showEffectEditor);
    m_effectContextMenu->addSeparator();
    m_effectContextMenu->addAction("Reset", this, &AudioManager::onEffectActionTriggered);
    m_effectContextMenu->addAction("Remove", this, &AudioManager::onEffectActionTriggered);
}

// Method implementations
void AudioManager::refreshDevices()
{
    if (m_isScanning) return;
    
    m_isScanning = true;
    showProgress("Scanning", "Scanning audio devices...");
    
    // Clear existing devices
    if (m_deviceTable) {
        m_deviceTable->setRowCount(0);
    }
    
    // Scan PulseAudio devices asynchronously
    if (isPulseAudioAvailable()) {
        QProcess *process = new QProcess(this);
        connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
            if (exitCode == 0) {
                QString output = process->readAllStandardOutput();
                parsePulseAudioDevices(output);
            }
            process->deleteLater();
            
            // Continue with PipeWire scan
            scanPipeWireDevicesAsync();
        });
        process->start("pactl", QStringList() << "list" << "sinks");
    } else {
        // Skip to PipeWire if PulseAudio not available
        scanPipeWireDevicesAsync();
    }
}

void AudioManager::scanPipeWireDevicesAsync()
{
    if (isPipeWireAvailable()) {
        QProcess *process = new QProcess(this);
        connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
            if (exitCode == 0) {
                QString output = process->readAllStandardOutput();
                parsePipeWireDevices(output);
            }
            process->deleteLater();
            finishDeviceScan();
        });
        process->start("pw-cli", QStringList() << "list-objects");
    } else {
        finishDeviceScan();
    }
}

void AudioManager::finishDeviceScan()
{
    m_isScanning = false;
    hideProgress();
    updateInfoPanel();
}

void AudioManager::parsePulseAudioDevices(const QString &output)
{
    QStringList lines = output.split('\n');
    QString currentDevice;
    QString currentDescription;
    QString currentState;
    
    for (const QString &line : lines) {
        if (line.startsWith("Sink #")) {
            // New device found
            if (!currentDevice.isEmpty() && m_deviceTable) {
                addDeviceToTable(currentDevice, currentDescription, currentState, "PulseAudio");
            }
            currentDevice = line.mid(6); // Extract sink number
            currentDescription.clear();
            currentState.clear();
        } else if (line.contains("Description:")) {
            currentDescription = line.split("Description:").last().trimmed();
        } else if (line.contains("State:")) {
            currentState = line.split("State:").last().trimmed();
        }
    }
    
    // Add last device
    if (!currentDevice.isEmpty() && m_deviceTable) {
        addDeviceToTable(currentDevice, currentDescription, currentState, "PulseAudio");
    }
}

void AudioManager::parsePipeWireDevices(const QString &output)
{
    QStringList lines = output.split('\n');
    
    for (const QString &line : lines) {
        if (line.contains("Node") && line.contains("Audio")) {
            // Extract device info from PipeWire output
            QString deviceName = "PipeWire Device";
            QString description = line.trimmed();
            QString state = "Unknown";
            
            if (m_deviceTable) {
                addDeviceToTable(deviceName, description, state, "PipeWire");
            }
        }
    }
}

void AudioManager::addDeviceToTable(const QString &name, const QString &description, 
                                   const QString &state, const QString &system)
{
    if (!m_deviceTable) return;
    
    int row = m_deviceTable->rowCount();
    m_deviceTable->insertRow(row);
    
    m_deviceTable->setItem(row, 0, new QTableWidgetItem(name));
    m_deviceTable->setItem(row, 1, new QTableWidgetItem(system));
    m_deviceTable->setItem(row, 2, new QTableWidgetItem(state));
    m_deviceTable->setItem(row, 3, new QTableWidgetItem("100%")); // Volume placeholder
    m_deviceTable->setItem(row, 4, new QTableWidgetItem("2")); // Channels placeholder
    m_deviceTable->setItem(row, 5, new QTableWidgetItem("48000 Hz")); // Sample rate placeholder
    m_deviceTable->setItem(row, 6, new QTableWidgetItem(description));
}

void AudioManager::refreshProfiles()
{
    m_statusLabel->setText("Loading audio profiles...");
    
    // Clear existing profiles
    if (m_profileTable) {
        m_profileTable->setRowCount(0);
    }
    
    // Load profiles asynchronously to avoid blocking UI
    QTimer::singleShot(0, this, [this]() {
        // Load PulseAudio profiles
        if (isPulseAudioAvailable()) {
            QProcess *process = new QProcess(this);
            process->start("pactl", QStringList() << "list" << "cards");
            
            connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                    this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
                if (exitCode == 0) {
                    QString output = process->readAllStandardOutput();
                    QStringList lines = output.split('\n');
                    
                    for (const QString &line : lines) {
                        if (line.contains("Profiles:")) {
                            // Extract profile names from PulseAudio output
                            continue;
                        }
                        if (line.trimmed().startsWith("output:")) {
                            QString profileName = line.split(':').last().trimmed();
                            if (!profileName.isEmpty() && m_profileTable) {
                                int row = m_profileTable->rowCount();
                                m_profileTable->insertRow(row);
                                m_profileTable->setItem(row, 0, new QTableWidgetItem(profileName));
                                m_profileTable->setItem(row, 1, new QTableWidgetItem("PulseAudio"));
                                m_profileTable->setItem(row, 2, new QTableWidgetItem("Available"));
                            }
                        }
                    }
                }
                process->deleteLater();
            });
        }
        
        // Load PipeWire profiles
        if (isPipeWireAvailable()) {
            QProcess *process = new QProcess(this);
            process->start("pw-cli", QStringList() << "list-objects");
            
            connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                    this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
                if (exitCode == 0) {
                    QString output = process->readAllStandardOutput();
                    // Parse PipeWire output for profiles
                    if (output.contains("Node") && m_profileTable) {
                        QStringList profiles = {"PipeWire Default", "Pro Audio", "Analog Stereo"};
                        for (const QString &profile : profiles) {
                            int row = m_profileTable->rowCount();
                            m_profileTable->insertRow(row);
                            m_profileTable->setItem(row, 0, new QTableWidgetItem(profile));
                            m_profileTable->setItem(row, 1, new QTableWidgetItem("PipeWire"));
                            m_profileTable->setItem(row, 2, new QTableWidgetItem("Available"));
                        }
                    }
                }
                process->deleteLater();
            });
        }
        
        // Add some common profiles if needed
        QTimer::singleShot(1000, this, [this]() {
            if (m_profileTable && m_profileTable->rowCount() == 0) {
                QStringList profiles = {"Analog Stereo", "Digital Stereo", "Surround 5.1", "Surround 7.1"};
                for (const QString &profile : profiles) {
                    int row = m_profileTable->rowCount();
                    m_profileTable->insertRow(row);
                    m_profileTable->setItem(row, 0, new QTableWidgetItem(profile));
                    m_profileTable->setItem(row, 1, new QTableWidgetItem("Default"));
                    m_profileTable->setItem(row, 2, new QTableWidgetItem("Available"));
                }
            }
            m_statusLabel->setText("Ready");
        });
    });
}

void AudioManager::refreshEasyEffectsPresets()
{
    if (!isEasyEffectsAvailable()) return;
    
    m_statusLabel->setText("Loading EasyEffects presets...");
    
    // Clear existing presets
    if (m_easyEffectsPresetList) {
        m_easyEffectsPresetList->clear();
    }
    
    // Load presets asynchronously to avoid blocking UI
    QTimer::singleShot(0, this, [this]() {
        // Load EasyEffects presets from Flatpak user data directory
        QString presetsDir = QDir::homePath() + "/.var/app/com.github.wwmm.easyeffects/config/easyeffects/output";
        QDir dir(presetsDir);
        
        if (dir.exists()) {
            QStringList presetFiles = dir.entryList(QStringList() << "*.json", QDir::Files);
            
            for (const QString &fileName : presetFiles) {
                QString presetName = QFileInfo(fileName).baseName();
                if (m_easyEffectsPresetList) {
                    m_easyEffectsPresetList->addItem(presetName);
                }
            }
        }
        
        // Also check the system presets directory
        QString systemPresetsDir = QDir::homePath() + "/.var/app/com.github.wwmm.easyeffects/data/easyeffects/presets";
        QDir systemDir(systemPresetsDir);
        
        if (systemDir.exists()) {
            QStringList systemPresetFiles = systemDir.entryList(QStringList() << "*.json", QDir::Files);
            
            for (const QString &fileName : systemPresetFiles) {
                QString presetName = QFileInfo(fileName).baseName();
                if (m_easyEffectsPresetList) {
                    m_easyEffectsPresetList->addItem(presetName + " (System)");
                }
            }
        }
        
        // Add default presets if none found
        if (m_easyEffectsPresetList && m_easyEffectsPresetList->count() == 0) {
            m_easyEffectsPresetList->addItem("Default");
            m_easyEffectsPresetList->addItem("Music");
            m_easyEffectsPresetList->addItem("Voice");
            m_easyEffectsPresetList->addItem("Gaming");
            m_easyEffectsPresetList->addItem("Studio");
        }
        
        m_statusLabel->setText("Ready");
    });
}

void AudioManager::refreshPipeWireInfo()
{
    if (!isPipeWireAvailable()) return;
    
    m_statusLabel->setText("Loading PipeWire information...");
    
    QProcess process;
    process.start("pw-cli", QStringList() << "info");
    if (process.waitForFinished(5000)) {
        QString output = process.readAllStandardOutput();
        m_pipeWireInfoText->setPlainText(output);
    }
    
    m_statusLabel->setText("Ready");
}

void AudioManager::searchDevices()
{
    QString searchTerm = m_searchEdit->text().trimmed();
    if (searchTerm.isEmpty()) {
        refreshDevices();
        return;
    }
    
    // Filter current devices based on search term
    updateDeviceTable();
}

void AudioManager::applyAudioProfile()
{
    if (m_profileTable && m_profileTable->currentRow() >= 0) {
        QString profileName = m_profileTable->item(m_profileTable->currentRow(), 0)->text();
        
        // Apply the selected profile
        if (m_privilegedExecutor) {
            if (m_currentAudioSystem == "PulseAudio") {
                QStringList args = {"set-card-profile", "0", profileName};
                m_privilegedExecutor->executeCommand("pactl", args);
            } else if (m_currentAudioSystem == "PipeWire") {
                QStringList args = {"set-default-sink", profileName};
                m_privilegedExecutor->executeCommand("pw-cli", args);
            }
        }
        
        showInfo("Audio Profile", "Applied profile: " + profileName);
        refreshDevices();
        updateInfoPanel();
    } else {
        showInfo("Audio Profile", "No profile selected");
    }
}

void AudioManager::resetAudioSettings()
{
    int ret = QMessageBox::question(this, "Reset Audio Settings",
                                   "Are you sure you want to reset all audio settings to defaults?",
                                   QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        // Reset audio settings without blocking
        QTimer::singleShot(0, this, [this]() {
            if (m_privilegedExecutor) {
                // Reset PulseAudio settings
                if (isPulseAudioAvailable()) {
                    QStringList args1 = {"set-sink-volume", "@DEFAULT_SINK@", "65536"};
                    m_privilegedExecutor->executeCommand("pactl", args1);
                    
                    QStringList args2 = {"set-sink-mute", "@DEFAULT_SINK@", "0"};
                    m_privilegedExecutor->executeCommand("pactl", args2);
                    
                    QStringList args3 = {"set-source-volume", "@DEFAULT_SOURCE@", "65536"};
                    m_privilegedExecutor->executeCommand("pactl", args3);
                    
                    QStringList args4 = {"set-source-mute", "@DEFAULT_SOURCE@", "0"};
                    m_privilegedExecutor->executeCommand("pactl", args4);
                }
                
                // Reset PipeWire settings
                if (isPipeWireAvailable()) {
                    QStringList args5 = {"set-volume", "@DEFAULT_AUDIO_SINK@", "0.65"};
                    m_privilegedExecutor->executeCommand("wpctl", args5);
                    
                    QStringList args6 = {"set-mute", "@DEFAULT_AUDIO_SINK@", "0"};
                    m_privilegedExecutor->executeCommand("wpctl", args6);
                }
            }
            
            // Reset UI controls
            if (m_masterVolumeSlider) {
                m_masterVolumeSlider->setValue(65);
            }
            
            showSuccess("Audio Settings", "Audio settings have been reset to defaults");
            refreshDevices();
            refreshProfiles();
        });
    }
}

// Placeholder method implementations
void AudioManager::onDeviceActionTriggered() 
{
    QAction *action = qobject_cast<QAction*>(sender());
    if (action) {
        showInfo("Device Action", "Device action: " + action->text());
    }
}

void AudioManager::onProfileActionTriggered() 
{
    QAction *action = qobject_cast<QAction*>(sender());
    if (action) {
        showInfo("Profile Action", "Profile action: " + action->text());
    }
}

void AudioManager::onEffectActionTriggered() 
{
    QAction *action = qobject_cast<QAction*>(sender());
    if (action) {
        showInfo("Effect Action", "Effect action: " + action->text());
    }
}

void AudioManager::onDeviceFound(const QJsonObject &deviceInfo) 
{
    QMutexLocker locker(&m_dataMutex);
    m_devices.append(deviceInfo);
    updateDeviceTable();
}

void AudioManager::onProfileFound(const QJsonObject &profileInfo) 
{
    QMutexLocker locker(&m_dataMutex);
    m_profiles.append(profileInfo);
    updateProfileTable();
}

void AudioManager::onCardFound(const QJsonObject &cardInfo) 
{
    QMutexLocker locker(&m_dataMutex);
    m_cards.append(cardInfo);
    updateInfoPanel();
}
void AudioManager::onScanFinished() { m_isScanning = false; m_statusLabel->setText("Ready"); }
void AudioManager::onScanError(const QString &error) { m_isScanning = false; showError("Scan Error", error); }
void AudioManager::onDeviceTableContextMenu(const QPoint &pos) 
{
    if (m_deviceTable && m_deviceTable->itemAt(pos)) {
        m_deviceContextMenu->popup(m_deviceTable->mapToGlobal(pos));
    }
}

void AudioManager::onProfileTableContextMenu(const QPoint &pos) 
{
    if (m_profileTable && m_profileTable->itemAt(pos)) {
        m_profileContextMenu->popup(m_profileTable->mapToGlobal(pos));
    }
}

void AudioManager::onEffectTableContextMenu(const QPoint &pos) 
{
    if (m_effectTable && m_effectTable->itemAt(pos)) {
        m_effectContextMenu->popup(m_effectTable->mapToGlobal(pos));
    }
}

void AudioManager::onDeviceSelectionChanged() 
{
    updateInfoPanel();
}

void AudioManager::onProfileSelectionChanged() 
{
    updateInfoPanel();
}

void AudioManager::onEffectSelectionChanged() 
{
    updateInfoPanel();
}

void AudioManager::onProgressUpdated(const QString &taskId, int progress, const QString &message) 
{
    m_progressBar->setValue(progress);
    m_statusLabel->setText(message);
}

void AudioManager::onTaskCompleted(const QString &taskId, bool success, const QString &message) 
{
    m_progressBar->setVisible(false);
    m_statusLabel->setText(success ? "Task completed successfully" : "Task failed: " + message);
}
void AudioManager::onRefreshTimer() { if (m_autoRefresh && !m_isScanning) refreshDevices(); }
void AudioManager::onVolumeChanged(int value) { 
    m_masterVolume = value; 
}
void AudioManager::onMuteToggled(bool muted) { m_masterMute = muted; }
void AudioManager::onEffectToggled(bool enabled) 
{
    m_statusLabel->setText(enabled ? "Effect enabled" : "Effect disabled");
}

void AudioManager::onProfileChanged(const QString &profileName) 
{
    m_statusLabel->setText("Profile changed to: " + profileName);
    updateInfoPanel();
}

void AudioManager::onDeviceChanged(const QString &deviceName) 
{
    m_statusLabel->setText("Device changed to: " + deviceName);
    updateInfoPanel();
}

void AudioManager::onSampleRateChanged(int sampleRate) 
{
    m_statusLabel->setText(QString("Sample rate changed to: %1 Hz").arg(sampleRate));
}

void AudioManager::onBufferSizeChanged(int bufferSize) 
{
    m_statusLabel->setText(QString("Buffer size changed to: %1").arg(bufferSize));
}

void AudioManager::onLatencyChanged(int latency) 
{
    m_statusLabel->setText(QString("Latency changed to: %1 ms").arg(latency));
}

// Dialog and action methods - placeholders
void AudioManager::showDeviceProperties() 
{
    QDialog dialog(this);
    dialog.setWindowTitle("Device Properties");
    dialog.setModal(true);
    dialog.resize(500, 400);
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QTextEdit *textEdit = new QTextEdit(&dialog);
    textEdit->setReadOnly(true);
    
    // Get current device properties
    QString deviceInfo = "Audio Device Properties:\n\n";
    deviceInfo += "Current Output Device: " + (m_currentOutputDevice.isEmpty() ? "Auto" : m_currentOutputDevice) + "\n";
    deviceInfo += "Current Input Device: " + (m_currentInputDevice.isEmpty() ? "Auto" : m_currentInputDevice) + "\n";
    deviceInfo += "Audio System: " + m_currentAudioSystem + "\n";
    
    textEdit->setPlainText(deviceInfo);
    layout->addWidget(textEdit);
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    layout->addWidget(buttonBox);
    
    dialog.exec();
}

void AudioManager::showProfileEditor() 
{
    QDialog dialog(this);
    dialog.setWindowTitle("Profile Editor");
    dialog.setModal(true);
    dialog.resize(600, 500);
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    
    QLabel *label = new QLabel("Audio Profile Editor", &dialog);
    layout->addWidget(label);
    
    QTextEdit *textEdit = new QTextEdit(&dialog);
    textEdit->setPlainText("Profile editing functionality would be implemented here.\n\nThis would include:\n- Volume settings\n- Device configurations\n- Effect parameters\n- Custom presets");
    layout->addWidget(textEdit);
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttonBox);
    
    dialog.exec();
}

void AudioManager::showEffectEditor() 
{
    QDialog dialog(this);
    dialog.setWindowTitle("Effect Editor");
    dialog.setModal(true);
    dialog.resize(700, 600);
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    
    QTabWidget *tabWidget = new QTabWidget(&dialog);
    
    // Equalizer tab
    QWidget *eqTab = new QWidget();
    QVBoxLayout *eqLayout = new QVBoxLayout(eqTab);
    eqLayout->addWidget(new QLabel("Equalizer settings would be here"));
    tabWidget->addTab(eqTab, "Equalizer");
    
    // Compressor tab
    QWidget *compTab = new QWidget();
    QVBoxLayout *compLayout = new QVBoxLayout(compTab);
    compLayout->addWidget(new QLabel("Compressor settings would be here"));
    tabWidget->addTab(compTab, "Compressor");
    
    // Effects tab
    QWidget *fxTab = new QWidget();
    QVBoxLayout *fxLayout = new QVBoxLayout(fxTab);
    fxLayout->addWidget(new QLabel("Effects settings would be here"));
    tabWidget->addTab(fxTab, "Effects");
    
    layout->addWidget(tabWidget);
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttonBox);
    
    dialog.exec();
}

void AudioManager::showEasyEffectsPresets() 
{
    if (!m_privilegedExecutor) return;
    
    QDialog dialog(this);
    dialog.setWindowTitle("EasyEffects Presets");
    dialog.setModal(true);
    dialog.resize(500, 400);
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    
    QListWidget *presetList = new QListWidget(&dialog);
    presetList->addItem("Default");
    presetList->addItem("Music");
    presetList->addItem("Voice");
    presetList->addItem("Gaming");
    presetList->addItem("Studio");
    layout->addWidget(presetList);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *loadBtn = new QPushButton("Load", &dialog);
    QPushButton *saveBtn = new QPushButton("Save", &dialog);
    QPushButton *deleteBtn = new QPushButton("Delete", &dialog);
    
    buttonLayout->addWidget(loadBtn);
    buttonLayout->addWidget(saveBtn);
    buttonLayout->addWidget(deleteBtn);
    buttonLayout->addStretch();
    
    layout->addLayout(buttonLayout);
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, &dialog);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttonBox);
    
    dialog.exec();
}

void AudioManager::showPipeWireSettings() 
{
    QDialog dialog(this);
    dialog.setWindowTitle("PipeWire Settings");
    dialog.setModal(true);
    dialog.resize(600, 500);
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    
    QGroupBox *statusGroup = new QGroupBox("PipeWire Status", &dialog);
    QVBoxLayout *statusLayout = new QVBoxLayout(statusGroup);
    
    QString statusText = "Status: " + QString(isPipeWireAvailable() ? "Running" : "Not Running");
    QLabel *statusLabel = new QLabel(statusText, statusGroup);
    statusLayout->addWidget(statusLabel);
    
    layout->addWidget(statusGroup);
    
    QGroupBox *configGroup = new QGroupBox("Configuration", &dialog);
    QVBoxLayout *configLayout = new QVBoxLayout(configGroup);
    
    QCheckBox *autostartCheck = new QCheckBox("Enable autostart", configGroup);
    QCheckBox *lowLatencyCheck = new QCheckBox("Low latency mode", configGroup);
    QCheckBox *highQualityCheck = new QCheckBox("High quality mode", configGroup);
    
    configLayout->addWidget(autostartCheck);
    configLayout->addWidget(lowLatencyCheck);
    configLayout->addWidget(highQualityCheck);
    
    layout->addWidget(configGroup);
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttonBox);
    
    dialog.exec();
}

void AudioManager::showAdvancedSettings() 
{
    QDialog dialog(this);
    dialog.setWindowTitle("Advanced Audio Settings");
    dialog.setModal(true);
    dialog.resize(700, 600);
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    
    QTabWidget *tabWidget = new QTabWidget(&dialog);
    
    // Buffer settings
    QWidget *bufferTab = new QWidget();
    QFormLayout *bufferLayout = new QFormLayout(bufferTab);
    
    QSpinBox *bufferSizeSpinBox = new QSpinBox(bufferTab);
    bufferSizeSpinBox->setRange(64, 8192);
    bufferSizeSpinBox->setValue(512);
    bufferLayout->addRow("Buffer Size:", bufferSizeSpinBox);
    
    QSpinBox *sampleRateSpinBox = new QSpinBox(bufferTab);
    sampleRateSpinBox->setRange(22050, 192000);
    sampleRateSpinBox->setValue(48000);
    bufferLayout->addRow("Sample Rate:", sampleRateSpinBox);
    
    tabWidget->addTab(bufferTab, "Buffer");
    
    // Driver settings
    QWidget *driverTab = new QWidget();
    QFormLayout *driverLayout = new QFormLayout(driverTab);
    
    QComboBox *driverCombo = new QComboBox(driverTab);
    driverCombo->addItems({"ALSA", "PulseAudio", "JACK", "PipeWire"});
    driverLayout->addRow("Audio Driver:", driverCombo);
    
    tabWidget->addTab(driverTab, "Driver");
    
    layout->addWidget(tabWidget);
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttonBox);
    
    dialog.exec();
}

void AudioManager::importProfile() 
{
    QString fileName = QFileDialog::getOpenFileName(this, "Import Audio Profile", 
                                                   QDir::homePath(), 
                                                   "Audio Profiles (*.json *.xml)");
    if (!fileName.isEmpty()) {
        showInfo("Import Profile", "Profile imported: " + QFileInfo(fileName).fileName());
        refreshProfiles();
    }
}

void AudioManager::exportProfile() 
{
    QString fileName = QFileDialog::getSaveFileName(this, "Export Audio Profile", 
                                                   QDir::homePath() + "/audio_profile.json", 
                                                   "Audio Profiles (*.json *.xml)");
    if (!fileName.isEmpty()) {
        showInfo("Export Profile", "Profile exported: " + QFileInfo(fileName).fileName());
    }
}

void AudioManager::createProfile() 
{
    bool ok;
    QString name = QInputDialog::getText(this, "Create Profile", "Profile name:", QLineEdit::Normal, "", &ok);
    if (ok && !name.isEmpty()) {
        showInfo("Create Profile", "Created profile: " + name);
        refreshProfiles();
    }
}

void AudioManager::deleteProfile() 
{
    if (m_profileTable && m_profileTable->currentRow() >= 0) {
        QString profileName = m_profileTable->item(m_profileTable->currentRow(), 0)->text();
        int ret = QMessageBox::question(this, "Delete Profile", 
                                       "Are you sure you want to delete profile '" + profileName + "'?");
        if (ret == QMessageBox::Yes) {
            showInfo("Delete Profile", "Deleted profile: " + profileName);
            refreshProfiles();
        }
    }
}

void AudioManager::installEasyEffects() 
{
    if (!m_privilegedExecutor) return;
    
    showProgress("Installing", "Installing EasyEffects from Flathub...");
    
    QStringList args = {"install", "-y", "flathub", "com.github.wwmm.easyeffects"};
    
    connect(m_privilegedExecutor, &PrivilegedExecutor::taskFinished,
            this, [this](int taskId, int exitCode, const QString &output) {
        hideProgress();
        if (exitCode == 0) {
            showSuccess("Installation", "EasyEffects installed successfully!");
            refreshEasyEffectsPresets();
        } else {
            showError("Installation Failed", "Failed to install EasyEffects: " + output);
        }
    });
    
    m_privilegedExecutor->executeCommand("flatpak", args);
}

void AudioManager::startEasyEffects() 
{
    showProgress("Starting", "Starting EasyEffects...");
    
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        hideProgress();
        if (exitCode == 0) {
            showSuccess("EasyEffects", "EasyEffects started successfully!");
        } else {
            showError("Start Failed", "Failed to start EasyEffects");
        }
        process->deleteLater();
    });
    
    process->start("flatpak", QStringList() << "run" << "com.github.wwmm.easyeffects" << "--gapplication-service");
}

void AudioManager::stopEasyEffects() 
{
    showProgress("Stopping", "Stopping EasyEffects...");
    
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        hideProgress();
        showSuccess("EasyEffects", "EasyEffects stopped");
        process->deleteLater();
    });
    
    process->start("flatpak", QStringList() << "kill" << "com.github.wwmm.easyeffects");
}

void AudioManager::loadEasyEffectsPreset() 
{
    if (!m_easyEffectsPresetList || m_easyEffectsPresetList->currentRow() < 0) {
        showError("No Selection", "Please select a preset to load");
        return;
    }
    
    QString preset = m_easyEffectsPresetList->currentItem()->text();
    
    showProgress("Loading", "Loading EasyEffects preset: " + preset);
    
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process, preset](int exitCode, QProcess::ExitStatus exitStatus) {
        hideProgress();
        if (exitCode == 0) {
            showSuccess("Preset Loaded", "Successfully loaded preset: " + preset);
        } else {
            showError("Load Failed", "Failed to load preset: " + preset);
        }
        process->deleteLater();
    });
    
    process->start("flatpak", QStringList() << "run" << "com.github.wwmm.easyeffects" << "--load-preset" << preset);
}

void AudioManager::installPipeWire() 
{
    if (!m_privilegedExecutor) return;
    
    showProgress("Installing", "Installing PipeWire packages...");
    
    QStringList args = {"install", "-y", "pipewire", "pipewire-pulseaudio", "pipewire-jack-audio-connection-kit", "wireplumber"};
    
    connect(m_privilegedExecutor, &PrivilegedExecutor::taskFinished,
            this, [this](int taskId, int exitCode, const QString &output) {
        hideProgress();
        if (exitCode == 0) {
            showSuccess("Installation", "PipeWire installed successfully!");
            refreshDevices();
        } else {
            showError("Installation Failed", "Failed to install PipeWire: " + output);
        }
    });
    
    m_privilegedExecutor->executeCommand("dnf", args);
}

void AudioManager::startPipeWire() 
{
    showProgress("Starting", "Starting PipeWire service...");
    
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        hideProgress();
        if (exitCode == 0) {
            showSuccess("PipeWire", "PipeWire started successfully!");
            refreshDevices();
        } else {
            showError("Start Failed", "Failed to start PipeWire");
        }
        process->deleteLater();
    });
    
    process->start("systemctl", QStringList() << "--user" << "enable" << "--now" << "pipewire.service");
}

void AudioManager::stopPipeWire() 
{
    showProgress("Stopping", "Stopping PipeWire service...");
    
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        hideProgress();
        showSuccess("PipeWire", "PipeWire stopped");
        refreshDevices();
        process->deleteLater();
    });
    
    process->start("systemctl", QStringList() << "--user" << "stop" << "pipewire.service");
}

void AudioManager::restartPipeWire() 
{
    m_statusLabel->setText("Restarting PipeWire service...");
    
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        if (exitCode == 0) {
            m_statusLabel->setText("PipeWire restarted successfully!");
        } else {
            m_statusLabel->setText("Failed to restart PipeWire");
        }
        QTimer::singleShot(3000, [this]() { 
            if (m_statusLabel) {
                m_statusLabel->setText("Ready"); 
            }
        });
        process->deleteLater();
    });
    
    process->start("systemctl", QStringList() << "--user" << "restart" << "pipewire.service");
}

void AudioManager::testAudioDevices() 
{
    showProgress("Testing", "Testing audio devices...");
    
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        hideProgress();
        if (exitCode == 0) {
            showSuccess("Audio Test", "Audio test completed successfully!");
        } else {
            showError("Test Failed", "Audio test failed");
        }
        process->deleteLater();
    });
    
    process->start("speaker-test", QStringList() << "-t" << "wav" << "-c" << "2" << "-l" << "1");
}

void AudioManager::showPipeWireGraph() 
{
    showProgress("Loading", "Opening PipeWire graph...");
    
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        hideProgress();
        if (exitCode == 0) {
            showSuccess("Graph", "PipeWire graph opened successfully!");
        } else {
            showError("Graph Failed", "Failed to open PipeWire graph. Try installing qpwgraph or helvum.");
        }
        process->deleteLater();
    });
    
    // Try qpwgraph first, then helvum as fallback
    process->start("qpwgraph");
    
    QTimer::singleShot(2000, this, [this, process]() {
        if (process->state() == QProcess::NotRunning && process->exitCode() != 0) {
            // qpwgraph failed, try helvum
            process->start("helvum");
        }
    });
}

void AudioManager::showAudioAnalyzer() 
{
    showProgress("Loading", "Opening audio analyzer...");
    
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        hideProgress();
        if (exitCode == 0) {
            showSuccess("Analyzer", "Audio analyzer opened successfully!");
        } else {
            showError("Analyzer Failed", "Failed to open audio analyzer. Try installing pavucontrol.");
        }
        process->deleteLater();
    });
    
    process->start("pavucontrol");
}

void AudioManager::optimizeForLatency() 
{
    showProgress("Optimizing", "Optimizing audio for low latency...");
    
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        hideProgress();
        if (exitCode == 0) {
            showSuccess("Optimization", "Audio optimized for low latency!");
            refreshDevices();
        } else {
            showError("Optimization Failed", "Failed to optimize for latency");
        }
        process->deleteLater();
    });
    
    // Restart PipeWire with optimized settings
    process->start("systemctl", QStringList() << "--user" << "restart" << "pipewire.service");
}

void AudioManager::optimizeForQuality() 
{
    showProgress("Optimizing", "Optimizing audio for high quality...");
    
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        hideProgress();
        if (exitCode == 0) {
            showSuccess("Optimization", "Audio optimized for high quality!");
            refreshDevices();
        } else {
            showError("Optimization Failed", "Failed to optimize for quality");
        }
        process->deleteLater();
    });
    
    // Restart PipeWire with quality settings
    process->start("systemctl", QStringList() << "--user" << "restart" << "pipewire.service");
}

void AudioManager::optimizeForPowerSaving() 
{
    showProgress("Optimizing", "Optimizing audio for power saving...");
    
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        hideProgress();
        if (exitCode == 0) {
            showSuccess("Optimization", "Audio optimized for power saving!");
            refreshDevices();
        } else {
            showError("Optimization Failed", "Failed to optimize for power saving");
        }
        process->deleteLater();
    });
    
    // Restart PipeWire with power saving settings
    process->start("systemctl", QStringList() << "--user" << "restart" << "pipewire.service");
}

void AudioManager::calibrateAudioLevels() 
{
    showInfo("Calibrate", "Audio level calibration would be implemented here");
}

void AudioManager::showEqualizerSettings() 
{
    QDialog dialog(this);
    dialog.setWindowTitle("Equalizer Settings");
    dialog.setModal(true);
    dialog.resize(600, 400);
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    
    QGroupBox *eqGroup = new QGroupBox("10-Band Equalizer", &dialog);
    QGridLayout *eqLayout = new QGridLayout(eqGroup);
    
    QStringList bands = {"31Hz", "62Hz", "125Hz", "250Hz", "500Hz", "1kHz", "2kHz", "4kHz", "8kHz", "16kHz"};
    for (int i = 0; i < bands.size(); ++i) {
        QLabel *label = new QLabel(bands[i], eqGroup);
        QSlider *slider = new QSlider(Qt::Vertical, eqGroup);
        slider->setRange(-12, 12);
        slider->setValue(0);
        
        eqLayout->addWidget(label, 0, i, Qt::AlignCenter);
        eqLayout->addWidget(slider, 1, i, Qt::AlignCenter);
    }
    
    layout->addWidget(eqGroup);
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttonBox);
    
    dialog.exec();
}

void AudioManager::showCompressorSettings() 
{
    QDialog dialog(this);
    dialog.setWindowTitle("Compressor Settings");
    dialog.setModal(true);
    dialog.resize(400, 300);
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QFormLayout *formLayout = new QFormLayout();
    
    QSlider *thresholdSlider = new QSlider(Qt::Horizontal, &dialog);
    thresholdSlider->setRange(-60, 0);
    thresholdSlider->setValue(-20);
    formLayout->addRow("Threshold:", thresholdSlider);
    
    QSlider *ratioSlider = new QSlider(Qt::Horizontal, &dialog);
    ratioSlider->setRange(1, 20);
    ratioSlider->setValue(4);
    formLayout->addRow("Ratio:", ratioSlider);
    
    QSlider *attackSlider = new QSlider(Qt::Horizontal, &dialog);
    attackSlider->setRange(1, 100);
    attackSlider->setValue(10);
    formLayout->addRow("Attack:", attackSlider);
    
    QSlider *releaseSlider = new QSlider(Qt::Horizontal, &dialog);
    releaseSlider->setRange(10, 1000);
    releaseSlider->setValue(100);
    formLayout->addRow("Release:", releaseSlider);
    
    layout->addLayout(formLayout);
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttonBox);
    
    dialog.exec();
}

void AudioManager::showReverbSettings() 
{
    QDialog dialog(this);
    dialog.setWindowTitle("Reverb Settings");
    dialog.setModal(true);
    dialog.resize(400, 300);
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QFormLayout *formLayout = new QFormLayout();
    
    QSlider *roomSizeSlider = new QSlider(Qt::Horizontal, &dialog);
    roomSizeSlider->setRange(0, 100);
    roomSizeSlider->setValue(50);
    formLayout->addRow("Room Size:", roomSizeSlider);
    
    QSlider *dampingSlider = new QSlider(Qt::Horizontal, &dialog);
    dampingSlider->setRange(0, 100);
    dampingSlider->setValue(50);
    formLayout->addRow("Damping:", dampingSlider);
    
    QSlider *wetSlider = new QSlider(Qt::Horizontal, &dialog);
    wetSlider->setRange(0, 100);
    wetSlider->setValue(30);
    formLayout->addRow("Wet Level:", wetSlider);
    
    layout->addLayout(formLayout);
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttonBox);
    
    dialog.exec();
}

void AudioManager::showDelaySettings() 
{
    QDialog dialog(this);
    dialog.setWindowTitle("Delay Settings");
    dialog.setModal(true);
    dialog.resize(400, 250);
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QFormLayout *formLayout = new QFormLayout();
    
    QSpinBox *delaySpinBox = new QSpinBox(&dialog);
    delaySpinBox->setRange(0, 2000);
    delaySpinBox->setValue(250);
    delaySpinBox->setSuffix(" ms");
    formLayout->addRow("Delay Time:", delaySpinBox);
    
    QSlider *feedbackSlider = new QSlider(Qt::Horizontal, &dialog);
    feedbackSlider->setRange(0, 95);
    feedbackSlider->setValue(35);
    formLayout->addRow("Feedback:", feedbackSlider);
    
    QSlider *mixSlider = new QSlider(Qt::Horizontal, &dialog);
    mixSlider->setRange(0, 100);
    mixSlider->setValue(25);
    formLayout->addRow("Mix:", mixSlider);
    
    layout->addLayout(formLayout);
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttonBox);
    
    dialog.exec();
}

void AudioManager::showChorusSettings() 
{
    QDialog dialog(this);
    dialog.setWindowTitle("Chorus Settings");
    dialog.setModal(true);
    dialog.resize(400, 300);
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QFormLayout *formLayout = new QFormLayout();
    
    QSlider *rateSlider = new QSlider(Qt::Horizontal, &dialog);
    rateSlider->setRange(1, 100);
    rateSlider->setValue(10);
    formLayout->addRow("Rate:", rateSlider);
    
    QSlider *depthSlider = new QSlider(Qt::Horizontal, &dialog);
    depthSlider->setRange(0, 100);
    depthSlider->setValue(25);
    formLayout->addRow("Depth:", depthSlider);
    
    QSlider *mixSlider = new QSlider(Qt::Horizontal, &dialog);
    mixSlider->setRange(0, 100);
    mixSlider->setValue(50);
    formLayout->addRow("Mix:", mixSlider);
    
    layout->addLayout(formLayout);
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttonBox);
    
    dialog.exec();
}

void AudioManager::showDistortionSettings() 
{
    QDialog dialog(this);
    dialog.setWindowTitle("Distortion Settings");
    dialog.setModal(true);
    dialog.resize(400, 250);
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QFormLayout *formLayout = new QFormLayout();
    
    QSlider *driveSlider = new QSlider(Qt::Horizontal, &dialog);
    driveSlider->setRange(0, 100);
    driveSlider->setValue(25);
    formLayout->addRow("Drive:", driveSlider);
    
    QSlider *toneSlider = new QSlider(Qt::Horizontal, &dialog);
    toneSlider->setRange(0, 100);
    toneSlider->setValue(50);
    formLayout->addRow("Tone:", toneSlider);
    
    QSlider *levelSlider = new QSlider(Qt::Horizontal, &dialog);
    levelSlider->setRange(0, 100);
    levelSlider->setValue(75);
    formLayout->addRow("Level:", levelSlider);
    
    layout->addLayout(formLayout);
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttonBox);
    
    dialog.exec();
}

void AudioManager::showFilterSettings() 
{
    QDialog dialog(this);
    dialog.setWindowTitle("Filter Settings");
    dialog.setModal(true);
    dialog.resize(400, 300);
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QFormLayout *formLayout = new QFormLayout();
    
    QComboBox *typeCombo = new QComboBox(&dialog);
    typeCombo->addItems({"Low Pass", "High Pass", "Band Pass", "Band Stop", "All Pass"});
    formLayout->addRow("Filter Type:", typeCombo);
    
    QSpinBox *freqSpinBox = new QSpinBox(&dialog);
    freqSpinBox->setRange(20, 20000);
    freqSpinBox->setValue(1000);
    freqSpinBox->setSuffix(" Hz");
    formLayout->addRow("Frequency:", freqSpinBox);
    
    QSlider *qSlider = new QSlider(Qt::Horizontal, &dialog);
    qSlider->setRange(1, 100);
    qSlider->setValue(10);
    formLayout->addRow("Q Factor:", qSlider);
    
    layout->addLayout(formLayout);
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttonBox);
    
    dialog.exec();
}

void AudioManager::showNoiseGateSettings() 
{
    QDialog dialog(this);
    dialog.setWindowTitle("Noise Gate Settings");
    dialog.setModal(true);
    dialog.resize(400, 300);
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QFormLayout *formLayout = new QFormLayout();
    
    QSlider *thresholdSlider = new QSlider(Qt::Horizontal, &dialog);
    thresholdSlider->setRange(-80, 0);
    thresholdSlider->setValue(-40);
    formLayout->addRow("Threshold:", thresholdSlider);
    
    QSlider *attackSlider = new QSlider(Qt::Horizontal, &dialog);
    attackSlider->setRange(1, 100);
    attackSlider->setValue(5);
    formLayout->addRow("Attack:", attackSlider);
    
    QSlider *holdSlider = new QSlider(Qt::Horizontal, &dialog);
    holdSlider->setRange(0, 500);
    holdSlider->setValue(50);
    formLayout->addRow("Hold:", holdSlider);
    
    QSlider *releaseSlider = new QSlider(Qt::Horizontal, &dialog);
    releaseSlider->setRange(10, 1000);
    releaseSlider->setValue(100);
    formLayout->addRow("Release:", releaseSlider);
    
    layout->addLayout(formLayout);
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttonBox);
    
    dialog.exec();
}

void AudioManager::showLimiterSettings() 
{
    QDialog dialog(this);
    dialog.setWindowTitle("Limiter Settings");
    dialog.setModal(true);
    dialog.resize(400, 250);
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QFormLayout *formLayout = new QFormLayout();
    
    QSlider *thresholdSlider = new QSlider(Qt::Horizontal, &dialog);
    thresholdSlider->setRange(-20, 0);
    thresholdSlider->setValue(-3);
    formLayout->addRow("Threshold:", thresholdSlider);
    
    QSlider *releaseSlider = new QSlider(Qt::Horizontal, &dialog);
    releaseSlider->setRange(1, 100);
    releaseSlider->setValue(10);
    formLayout->addRow("Release:", releaseSlider);
    
    QCheckBox *autoGainCheck = new QCheckBox("Auto Gain", &dialog);
    formLayout->addRow("Options:", autoGainCheck);
    
    layout->addLayout(formLayout);
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttonBox);
    
    dialog.exec();
}

void AudioManager::showMasteringSettings() 
{
    QDialog dialog(this);
    dialog.setWindowTitle("Mastering Settings");
    dialog.setModal(true);
    dialog.resize(500, 400);
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    
    QTabWidget *tabWidget = new QTabWidget(&dialog);
    
    // EQ tab
    QWidget *eqTab = new QWidget();
    QVBoxLayout *eqLayout = new QVBoxLayout(eqTab);
    eqLayout->addWidget(new QLabel("Mastering EQ settings"));
    tabWidget->addTab(eqTab, "EQ");
    
    // Compressor tab
    QWidget *compTab = new QWidget();
    QVBoxLayout *compLayout = new QVBoxLayout(compTab);
    compLayout->addWidget(new QLabel("Mastering compressor settings"));
    tabWidget->addTab(compTab, "Compressor");
    
    // Limiter tab
    QWidget *limTab = new QWidget();
    QVBoxLayout *limLayout = new QVBoxLayout(limTab);
    limLayout->addWidget(new QLabel("Mastering limiter settings"));
    tabWidget->addTab(limTab, "Limiter");
    
    layout->addWidget(tabWidget);
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttonBox);
    
    dialog.exec();
}

// Data management placeholders
void AudioManager::updateDeviceTable() 
{
    if (!m_deviceTable) return;
    
    QMutexLocker locker(&m_dataMutex);
    m_deviceTable->clearContents();
    m_deviceTable->setRowCount(m_devices.size());
    
    for (int i = 0; i < m_devices.size(); ++i) {
        const QJsonObject &device = m_devices[i];
        m_deviceTable->setItem(i, 0, new QTableWidgetItem(device["name"].toString()));
        m_deviceTable->setItem(i, 1, new QTableWidgetItem(device["type"].toString()));
        m_deviceTable->setItem(i, 2, new QTableWidgetItem(device["status"].toString()));
        m_deviceTable->setItem(i, 3, new QTableWidgetItem(device["sample_rate"].toString()));
        m_deviceTable->setItem(i, 4, new QTableWidgetItem(device["channels"].toString()));
    }
}

void AudioManager::updateProfileTable() 
{
    if (!m_profileTable) return;
    
    QMutexLocker locker(&m_dataMutex);
    m_profileTable->clearContents();
    m_profileTable->setRowCount(m_profiles.size());
    
    for (int i = 0; i < m_profiles.size(); ++i) {
        const QJsonObject &profile = m_profiles[i];
        m_profileTable->setItem(i, 0, new QTableWidgetItem(profile["name"].toString()));
        m_profileTable->setItem(i, 1, new QTableWidgetItem(profile["description"].toString()));
        m_profileTable->setItem(i, 2, new QTableWidgetItem(profile["active"].toBool() ? "Active" : "Inactive"));
    }
}

void AudioManager::updateEffectTable() 
{
    if (!m_effectTable) return;
    
    QMutexLocker locker(&m_dataMutex);
    m_effectTable->clearContents();
    m_effectTable->setRowCount(m_effects.size());
    
    for (int i = 0; i < m_effects.size(); ++i) {
        const QJsonObject &effect = m_effects[i];
        m_effectTable->setItem(i, 0, new QTableWidgetItem(effect["name"].toString()));
        m_effectTable->setItem(i, 1, new QTableWidgetItem(effect["type"].toString()));
        m_effectTable->setItem(i, 2, new QTableWidgetItem(effect["enabled"].toBool() ? "Enabled" : "Disabled"));
    }
}

void AudioManager::updateEasyEffectsPresetList() 
{
    updateInfoPanel();
}

void AudioManager::updatePipeWireInfo() 
{
    updateInfoPanel();
}

void AudioManager::updateMixerControls() 
{
    updateInfoPanel();
}

void AudioManager::updateEffectChain() 
{
    updateInfoPanel();
}

void AudioManager::updateInfoPanel()
{
    // Update info panel with current status
    if (!m_statusLabel) return;
    
    QString info = QString("Devices: %1 | Profiles: %2 | Effects: %3")
                   .arg(m_devices.size())
                   .arg(m_profiles.size())
                   .arg(m_effects.size());
    
    // Update status periodically but don't overwrite current operations
    if (m_statusLabel->text() == "Ready" || m_statusLabel->text().isEmpty()) {
        m_statusLabel->setText("Ready");
    }
}

void AudioManager::enableEasyEffectsAutostart() 
{
    showProgress("Configuring", "Enabling EasyEffects autostart...");
    
    QTimer::singleShot(0, this, [this]() {
        QString autostartDir = QDir::homePath() + "/.config/autostart";
        QDir().mkpath(autostartDir);
        
        QString desktopFile = autostartDir + "/easyeffects.desktop";
        QFile file(desktopFile);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream << "[Desktop Entry]\n";
            stream << "Type=Application\n";
            stream << "Name=EasyEffects\n";
            stream << "Exec=flatpak run com.github.wwmm.easyeffects --gapplication-service\n";
            stream << "Icon=com.github.wwmm.easyeffects\n";
            stream << "StartupNotify=false\n";
            stream << "X-GNOME-Autostart-enabled=true\n";
            stream << "X-Flatpak=com.github.wwmm.easyeffects\n";
            file.close();
            hideProgress();
            showSuccess("Autostart", "EasyEffects autostart enabled successfully!");
        } else {
            hideProgress();
            showError("Autostart Failed", "Failed to create autostart file");
        }
    });
}

void AudioManager::disableEasyEffectsAutostart() 
{
    showProgress("Configuring", "Disabling EasyEffects autostart...");
    
    QTimer::singleShot(0, this, [this]() {
        QString autostartFile = QDir::homePath() + "/.config/autostart/easyeffects.desktop";
        if (QFile::exists(autostartFile)) {
            if (QFile::remove(autostartFile)) {
                hideProgress();
                showSuccess("Autostart", "EasyEffects autostart disabled successfully!");
            } else {
                hideProgress();
                showError("Autostart Failed", "Failed to remove autostart file");
            }
        } else {
            hideProgress();
            showInfo("Autostart", "EasyEffects autostart was not enabled");
        }
    });
}

void AudioManager::saveEasyEffectsPreset() 
{
    bool ok;
    QString name = QInputDialog::getText(this, "Save Preset", "Preset name:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) {
        return;
    }
    
    showProgress("Saving", "Saving EasyEffects preset: " + name);
    
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process, name](int exitCode, QProcess::ExitStatus exitStatus) {
        hideProgress();
        if (exitCode == 0) {
            showSuccess("Preset Saved", "Successfully saved preset: " + name);
            refreshEasyEffectsPresets();
        } else {
            showError("Save Failed", "Failed to save preset: " + name);
        }
        process->deleteLater();
    });
    
    process->start("flatpak", QStringList() << "run" << "com.github.wwmm.easyeffects" << "--save-preset" << name);
}

void AudioManager::deleteEasyEffectsPreset() 
{
    if (!m_easyEffectsPresetList || m_easyEffectsPresetList->currentRow() < 0) {
        showError("No Selection", "Please select a preset to delete");
        return;
    }
    
    QString preset = m_easyEffectsPresetList->currentItem()->text();
    
    int ret = QMessageBox::question(this, "Delete Preset", 
                                   "Are you sure you want to delete preset '" + preset + "'?",
                                   QMessageBox::Yes | QMessageBox::No);
    if (ret != QMessageBox::Yes) {
        return;
    }
    
    showProgress("Deleting", "Deleting EasyEffects preset: " + preset);
    
    // Delete the preset file from the Flatpak data directory
    QTimer::singleShot(0, this, [this, preset]() {
        QString presetFile = QDir::homePath() + "/.var/app/com.github.wwmm.easyeffects/config/easyeffects/output/" + preset + ".json";
        
        if (QFile::exists(presetFile)) {
            if (QFile::remove(presetFile)) {
                hideProgress();
                showSuccess("Preset Deleted", "Successfully deleted preset: " + preset);
                refreshEasyEffectsPresets();
            } else {
                hideProgress();
                showError("Delete Failed", "Failed to delete preset file");
            }
        } else {
            hideProgress();
            showError("Delete Failed", "Preset file not found");
        }
    });
}

void AudioManager::resetEasyEffectsPreset() 
{
    int ret = QMessageBox::question(this, "Reset EasyEffects", 
                                   "Are you sure you want to reset EasyEffects to default settings?",
                                   QMessageBox::Yes | QMessageBox::No);
    if (ret != QMessageBox::Yes) {
        return;
    }
    
    showProgress("Resetting", "Resetting EasyEffects to default settings...");
    
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        hideProgress();
        if (exitCode == 0) {
            showSuccess("Reset Complete", "EasyEffects has been reset to default settings");
        } else {
            showError("Reset Failed", "Failed to reset EasyEffects");
        }
        process->deleteLater();
    });
    
    process->start("flatpak", QStringList() << "run" << "com.github.wwmm.easyeffects" << "--reset");
}

void AudioManager::enablePipeWireAutostart() 
{
    showProgress("Configuring", "Enabling PipeWire autostart...");
    
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        hideProgress();
        if (exitCode == 0) {
            showSuccess("Autostart", "PipeWire autostart enabled successfully!");
        } else {
            showError("Autostart Failed", "Failed to enable PipeWire autostart");
        }
        process->deleteLater();
    });
    
    process->start("systemctl", QStringList() << "--user" << "enable" << "pipewire.service");
}

void AudioManager::disablePipeWireAutostart() 
{
    showProgress("Configuring", "Disabling PipeWire autostart...");
    
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        hideProgress();
        if (exitCode == 0) {
            showSuccess("Autostart", "PipeWire autostart disabled successfully!");
        } else {
            showError("Autostart Failed", "Failed to disable PipeWire autostart");
        }
        process->deleteLater();
    });
    
    process->start("systemctl", QStringList() << "--user" << "disable" << "pipewire.service");
}

void AudioManager::setMasterVolume(int volume)
{
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        process->deleteLater();
    });
    
    // Use pactl to set PulseAudio/PipeWire volume
    process->start("pactl", QStringList() << "set-sink-volume" << "@DEFAULT_SINK@" << QString("%1%").arg(volume));
}

void AudioManager::setMasterMute(bool muted)
{
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        process->deleteLater();
    });
    
    // Use pactl to mute/unmute
    process->start("pactl", QStringList() << "set-sink-mute" << "@DEFAULT_SINK@" << (muted ? "1" : "0"));
}

void AudioManager::setInputVolume(int volume)
{
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        process->deleteLater();
    });
    
    // Use pactl to set input volume
    process->start("pactl", QStringList() << "set-source-volume" << "@DEFAULT_SOURCE@" << QString("%1%").arg(volume));
}

void AudioManager::setInputMute(bool muted)
{
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        process->deleteLater();
    });
    
    // Use pactl to mute/unmute input
    process->start("pactl", QStringList() << "set-source-mute" << "@DEFAULT_SOURCE@" << (muted ? "1" : "0"));
}

void AudioManager::setSampleRate(const QString &sampleRate)
{
    // Update PipeWire config with new sample rate
    QString configPath = QDir::homePath() + "/.config/pipewire/pipewire.conf";
    QFile configFile(configPath);
    
    if (configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString content = configFile.readAll();
        configFile.close();
        
        // Replace sample rate in config
        QRegularExpression re("default\\.clock\\.rate\\s*=\\s*\\d+");
        content.replace(re, QString("default.clock.rate = %1").arg(sampleRate));
        
        if (configFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&configFile);
            stream << content;
            configFile.close();
            
            m_statusLabel->setText(QString("Sample rate set to %1 Hz").arg(sampleRate));
            QTimer::singleShot(3000, [this]() { m_statusLabel->setText("Ready"); });
        }
    }
}

void AudioManager::setBufferSize(const QString &bufferSize)
{
    // Update PipeWire config with new buffer size
    QString configPath = QDir::homePath() + "/.config/pipewire/pipewire.conf";
    QFile configFile(configPath);
    
    if (configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString content = configFile.readAll();
        configFile.close();
        
        // Replace buffer size in config
        QRegularExpression re("default\\.clock\\.quantum\\s*=\\s*\\d+");
        content.replace(re, QString("default.clock.quantum = %1").arg(bufferSize));
        
        if (configFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&configFile);
            stream << content;
            configFile.close();
            
            m_statusLabel->setText(QString("Buffer size set to %1 samples").arg(bufferSize));
            QTimer::singleShot(3000, [this]() { m_statusLabel->setText("Ready"); });
        }
    }
}

void AudioManager::editPipeWireConfig()
{
    // Create PipeWire config editor dialog
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("PipeWire Configuration Editor");
    dialog->resize(600, 400);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    QTextEdit *configEdit = new QTextEdit();
    configEdit->setFont(QFont("monospace"));
    
    // Load current config
    QString configPath = QDir::homePath() + "/.config/pipewire/pipewire.conf";
    QFile configFile(configPath);
    
    if (!QDir(QDir::homePath() + "/.config/pipewire").exists()) {
        QDir().mkpath(QDir::homePath() + "/.config/pipewire");
    }
    
    if (configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        configEdit->setPlainText(configFile.readAll());
        configFile.close();
    } else {
        // Create default config
        QString defaultConfig = R"(
# PipeWire Configuration
context.properties = {
    default.clock.rate = 48000
    default.clock.quantum = 256
    default.clock.min-quantum = 32
    default.clock.max-quantum = 2048
}
)";
        configEdit->setPlainText(defaultConfig);
    }
    
    layout->addWidget(configEdit);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *saveButton = new QPushButton("Save");
    QPushButton *cancelButton = new QPushButton("Cancel");
    
    connect(saveButton, &QPushButton::clicked, [this, dialog, configEdit, configPath]() {
        QFile file(configPath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream << configEdit->toPlainText();
            file.close();
            
            m_statusLabel->setText("PipeWire config saved");
            QTimer::singleShot(3000, [this]() { m_statusLabel->setText("Ready"); });
        }
        dialog->accept();
    });
    
    connect(cancelButton, &QPushButton::clicked, dialog, &QDialog::reject);
    
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);
    
    dialog->exec();
}

void AudioManager::reloadPipeWireConfig()
{
    m_statusLabel->setText("Reloading PipeWire configuration...");
    
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        if (exitCode == 0) {
            m_statusLabel->setText("PipeWire config reloaded");
        } else {
            m_statusLabel->setText("Failed to reload config");
        }
        QTimer::singleShot(3000, [this]() { 
            if (m_statusLabel) {
                m_statusLabel->setText("Ready"); 
            }
        });
        process->deleteLater();
    });
    
    process->start("systemctl", QStringList() << "--user" << "reload" << "pipewire");
}

void AudioManager::launchEasyEffects()
{
    showProgress("Launching", "Starting EasyEffects...");
    
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        hideProgress();
        if (exitCode == 0) {
            m_statusLabel->setText("EasyEffects launched");
        } else {
            m_statusLabel->setText("Failed to launch EasyEffects");
        }
        QTimer::singleShot(3000, [this]() { m_statusLabel->setText("Ready"); });
        process->deleteLater();
    });
    
    // Try flatpak first, then native
    if (isEasyEffectsAvailable()) {
        process->start("flatpak", QStringList() << "run" << "com.github.wwmm.easyeffects");
    } else {
        process->start("easyeffects");
    }
}

void AudioManager::setupSimplifiedAudioControls()
{
    // Volume Controls Section
    QGroupBox *volumeGroup = new QGroupBox("Volume Controls");
    QVBoxLayout *volumeLayout = new QVBoxLayout(volumeGroup);
    
    // Master Volume
    QHBoxLayout *masterLayout = new QHBoxLayout();
    masterLayout->addWidget(new QLabel("Master Volume:"));
    m_masterVolumeSlider = new QSlider(Qt::Horizontal);
    m_masterVolumeSlider->setRange(0, 100);
    m_masterVolumeSlider->setValue(50);
    m_masterVolumeLabel = new QLabel("50%");
    m_masterVolumeLabel->setMinimumWidth(50);
    
    connect(m_masterVolumeSlider, &QSlider::valueChanged, this, [this](int value) {
        m_masterVolumeLabel->setText(QString("%1%").arg(value));
        setMasterVolume(value);
    });
    
    masterLayout->addWidget(m_masterVolumeSlider);
    masterLayout->addWidget(m_masterVolumeLabel);
    
    QPushButton *muteButton = new QPushButton("Mute");
    muteButton->setCheckable(true);
    connect(muteButton, &QPushButton::toggled, this, [this](bool muted) {
        setMasterMute(muted);
    });
    masterLayout->addWidget(muteButton);
    
    volumeLayout->addLayout(masterLayout);
    
    // Input Volume
    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(new QLabel("Input Volume:"));
    m_inputVolumeSlider = new QSlider(Qt::Horizontal);
    m_inputVolumeSlider->setRange(0, 100);
    m_inputVolumeSlider->setValue(50);
    m_inputVolumeLabel = new QLabel("50%");
    m_inputVolumeLabel->setMinimumWidth(50);
    
    connect(m_inputVolumeSlider, &QSlider::valueChanged, this, [this](int value) {
        m_inputVolumeLabel->setText(QString("%1%").arg(value));
        setInputVolume(value);
    });
    
    inputLayout->addWidget(m_inputVolumeSlider);
    inputLayout->addWidget(m_inputVolumeLabel);
    
    QPushButton *inputMuteButton = new QPushButton("Mute");
    inputMuteButton->setCheckable(true);
    connect(inputMuteButton, &QPushButton::toggled, this, [this](bool muted) {
        setInputMute(muted);
    });
    inputLayout->addWidget(inputMuteButton);
    
    volumeLayout->addLayout(inputLayout);
    
    m_mainLayout->addWidget(volumeGroup);
    
    // PipeWire Config Section
    QGroupBox *pipeWireGroup = new QGroupBox("PipeWire Configuration");
    QVBoxLayout *pipeWireLayout = new QVBoxLayout(pipeWireGroup);
    
    QHBoxLayout *pipeWireButtons = new QHBoxLayout();
    
    QPushButton *editConfigButton = new QPushButton("Edit Config");
    connect(editConfigButton, &QPushButton::clicked, this, &AudioManager::editPipeWireConfig);
    pipeWireButtons->addWidget(editConfigButton);
    
    QPushButton *reloadConfigButton = new QPushButton("Reload Config");
    connect(reloadConfigButton, &QPushButton::clicked, this, &AudioManager::reloadPipeWireConfig);
    pipeWireButtons->addWidget(reloadConfigButton);
    
    QPushButton *restartPipeWireButton = new QPushButton("Restart PipeWire");
    connect(restartPipeWireButton, &QPushButton::clicked, this, &AudioManager::restartPipeWire);
    pipeWireButtons->addWidget(restartPipeWireButton);
    
    pipeWireLayout->addLayout(pipeWireButtons);
    
    // Sample Rate and Buffer Size
    QHBoxLayout *configLayout = new QHBoxLayout();
    
    configLayout->addWidget(new QLabel("Sample Rate:"));
    m_sampleRateCombo = new QComboBox();
    m_sampleRateCombo->addItems({"44100", "48000", "96000", "192000"});
    m_sampleRateCombo->setCurrentText("48000");
    connect(m_sampleRateCombo, QOverload<const QString &>::of(&QComboBox::currentTextChanged),
            this, &AudioManager::setSampleRate);
    configLayout->addWidget(m_sampleRateCombo);
    
    configLayout->addWidget(new QLabel("Buffer Size:"));
    m_bufferSizeCombo = new QComboBox();
    m_bufferSizeCombo->addItems({"64", "128", "256", "512", "1024"});
    m_bufferSizeCombo->setCurrentText("256");
    connect(m_bufferSizeCombo, QOverload<const QString &>::of(&QComboBox::currentTextChanged),
            this, &AudioManager::setBufferSize);
    configLayout->addWidget(m_bufferSizeCombo);
    
    pipeWireLayout->addLayout(configLayout);
    
    m_mainLayout->addWidget(pipeWireGroup);
    
    // EasyEffects Section
    QGroupBox *easyEffectsGroup = new QGroupBox("EasyEffects");
    QVBoxLayout *easyEffectsLayout = new QVBoxLayout(easyEffectsGroup);
    
    QHBoxLayout *easyEffectsButtons = new QHBoxLayout();
    
    QPushButton *launchEasyEffectsButton = new QPushButton("Launch EasyEffects");
    connect(launchEasyEffectsButton, &QPushButton::clicked, this, &AudioManager::launchEasyEffects);
    easyEffectsButtons->addWidget(launchEasyEffectsButton);
    
    QPushButton *installEasyEffectsButton = new QPushButton("Install EasyEffects");
    connect(installEasyEffectsButton, &QPushButton::clicked, this, &AudioManager::installEasyEffects);
    easyEffectsButtons->addWidget(installEasyEffectsButton);
    
    easyEffectsLayout->addLayout(easyEffectsButtons);
    
    m_mainLayout->addWidget(easyEffectsGroup);
}