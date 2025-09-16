#include "containermanager.h"
#include "systemutils.h"
#include "privilegedexecutor.h"
#include <QApplication>
#include <QDesktopServices>
#include <QInputDialog>
#include <QFileDialog>
#include <QStandardPaths>
#include <QScrollBar>
#include <QDebug>

// ContainerSearchWorker Implementation
ContainerSearchWorker::ContainerSearchWorker(const QString &containerType, QObject *parent)
    : QThread(parent), m_containerType(containerType), m_stopRequested(false)
{
}

void ContainerSearchWorker::setParameters(const QString &searchTerm, const QString &containerType)
{
    QMutexLocker locker(&m_mutex);
    m_searchTerm = searchTerm;
    m_containerType = containerType;
}

void ContainerSearchWorker::stop()
{
    QMutexLocker locker(&m_mutex);
    m_stopRequested = true;
}

void ContainerSearchWorker::run()
{
    try {
        m_stopRequested = false;
        searchContainersAsync();
    } catch (const std::exception &e) {
        emit errorOccurred(QString("Search error: %1").arg(e.what()));
    }
}

void ContainerSearchWorker::searchContainersAsync()
{
    if (m_containerType == "docker") {
        // Search Docker containers
        QProcess *process = new QProcess();
        connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
            if (m_stopRequested) {
                process->deleteLater();
                return;
            }
            
            if (exitCode == 0) {
                QString output = process->readAllStandardOutput();
                QStringList lines = output.split('\n', Qt::SkipEmptyParts);
                
                for (const QString &line : lines) {
                    if (m_stopRequested) break;
                    
                    QJsonParseError error;
                    QJsonDocument doc = QJsonDocument::fromJson(line.toUtf8(), &error);
                    if (error.error == QJsonParseError::NoError) {
                        QJsonObject container = doc.object();
                        if (m_searchTerm.isEmpty() || 
                            container["Names"].toString().contains(m_searchTerm, Qt::CaseInsensitive) ||
                            container["Image"].toString().contains(m_searchTerm, Qt::CaseInsensitive)) {
                            emit containerFound(container);
                        }
                    }
                }
            }
            
            process->deleteLater();
            
            // Search Docker images
            searchDockerImagesAsync();
        });
        
        process->start("docker", QStringList() << "ps" << "-a" << "--format" << "json");
    } else {
        emit searchFinished();
    }
}

void ContainerSearchWorker::searchDockerImagesAsync()
{
    QProcess *process = new QProcess();
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        if (m_stopRequested) {
            process->deleteLater();
            return;
        }
        
        if (exitCode == 0) {
            QString output = process->readAllStandardOutput();
            QStringList lines = output.split('\n', Qt::SkipEmptyParts);
            
            for (const QString &line : lines) {
                if (m_stopRequested) break;
                
                QJsonParseError error;
                QJsonDocument doc = QJsonDocument::fromJson(line.toUtf8(), &error);
                if (error.error == QJsonParseError::NoError) {
                    QJsonObject image = doc.object();
                    if (m_searchTerm.isEmpty() || 
                        image["Repository"].toString().contains(m_searchTerm, Qt::CaseInsensitive) ||
                        image["Tag"].toString().contains(m_searchTerm, Qt::CaseInsensitive)) {
                        emit imageFound(image);
                    }
                }
            }
        }
        
        process->deleteLater();
        emit searchFinished();
    });
    
    process->start("docker", QStringList() << "images" << "--format" << "json");
}

// ContainerManager Implementation
ContainerManager::ContainerManager(QWidget *parent)
    : QWidget(parent)
    , m_systemUtils(nullptr)
    , m_privilegedExecutor(nullptr)
    , m_searchWorker(nullptr)
    , m_autoRefresh(true)
    , m_refreshInterval(30000) // 30 seconds
    , m_defaultRuntime("docker")
    , m_isSearching(false)
{
    setupUI();
    setupContextMenus();
    
    // Initialize refresh timer
    m_refreshTimer = new QTimer(this);
    m_refreshTimer->setInterval(m_refreshInterval);
    connect(m_refreshTimer, &QTimer::timeout, this, &ContainerManager::onRefreshTimer);
    
    // Check available runtimes
    if (isDockerAvailable()) {
        m_defaultRuntime = "docker";
    } else if (isPodmanAvailable()) {
        m_defaultRuntime = "podman";
    }
    
    // Initial refresh
    QTimer::singleShot(1000, this, &ContainerManager::refreshContainers);
    QTimer::singleShot(1000, this, &ContainerManager::refreshImages);
    QTimer::singleShot(1000, this, &ContainerManager::refreshDistroboxContainers);
}

ContainerManager::~ContainerManager()
{
    if (m_searchWorker) {
        m_searchWorker->stop();
        m_searchWorker->wait(3000);
        delete m_searchWorker;
    }
}

void ContainerManager::setSystemUtils(SystemUtils *utils)
{
    m_systemUtils = utils;
}

void ContainerManager::setPrivilegedExecutor(PrivilegedExecutor *executor)
{
    m_privilegedExecutor = executor;
    if (m_privilegedExecutor) {
        connect(m_privilegedExecutor, &PrivilegedExecutor::taskProgress,
                this, &ContainerManager::onTaskProgress);
        connect(m_privilegedExecutor, &PrivilegedExecutor::taskFinished,
                this, &ContainerManager::onTaskFinished);
        connect(m_privilegedExecutor, &PrivilegedExecutor::taskStarted,
                this, &ContainerManager::onTaskStarted);
        connect(m_privilegedExecutor, &PrivilegedExecutor::taskError,
                this, &ContainerManager::onTaskError);
    }
}

void ContainerManager::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(12, 12, 12, 12);
    m_mainLayout->setSpacing(8);
    
    // Setup toolbar
    setupToolbar();
    
    // Setup search bar
    setupSearchBar();
    
    // Setup tab widget
    m_tabWidget = new QTabWidget(this);
    m_tabWidget->setTabPosition(QTabWidget::North);
    m_tabWidget->setMovable(false);
    m_tabWidget->setDocumentMode(true);
    
    // Setup tabs
    setupContainerTab();
    setupImageTab();
    setupDistroboxTab();
    
    m_mainLayout->addWidget(m_tabWidget);
    
    // Setup progress area
    setupProgressArea();
    
    // Apply theming
    updateTheme();
}

void ContainerManager::setupToolbar()
{
    m_toolbarLayout = new QHBoxLayout();
    m_toolbarLayout->setContentsMargins(0, 0, 0, 0);
    m_toolbarLayout->setSpacing(8);
    
    // Runtime selection
    QLabel *runtimeLabel = new QLabel("Runtime:");
    m_toolbarLayout->addWidget(runtimeLabel);
    
    QComboBox *runtimeCombo = new QComboBox();
    if (isDockerAvailable()) {
        runtimeCombo->addItem("Docker", "docker");
    }
    if (isPodmanAvailable()) {
        runtimeCombo->addItem("Podman", "podman");
    }
    if (isDistroboxAvailable()) {
        runtimeCombo->addItem("Distrobox", "distrobox");
    }
    m_toolbarLayout->addWidget(runtimeCombo);
    
    m_toolbarLayout->addStretch();
    
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
    
    // Refresh interval
    QLabel *intervalLabel = new QLabel("Interval (s):");
    m_toolbarLayout->addWidget(intervalLabel);
    
    QSpinBox *intervalSpinBox = new QSpinBox();
    intervalSpinBox->setRange(5, 300);
    intervalSpinBox->setValue(m_refreshInterval / 1000);
    connect(intervalSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
        m_refreshInterval = value * 1000;
        m_refreshTimer->setInterval(m_refreshInterval);
    });
    m_toolbarLayout->addWidget(intervalSpinBox);
    
    m_mainLayout->addLayout(m_toolbarLayout);
}

void ContainerManager::setupSearchBar()
{
    m_searchLayout = new QHBoxLayout();
    m_searchLayout->setContentsMargins(0, 0, 0, 0);
    m_searchLayout->setSpacing(8);
    
    // Search input
    m_searchEdit = new QLineEdit();
    m_searchEdit->setPlaceholderText("Search containers, images, or names...");
    m_searchEdit->setClearButtonEnabled(true);
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &ContainerManager::searchContainers);
    m_searchLayout->addWidget(m_searchEdit);
    
    // Filter combo
    m_filterComboBox = new QComboBox();
    m_filterComboBox->addItem("All", "all");
    m_filterComboBox->addItem("Running", "running");
    m_filterComboBox->addItem("Stopped", "stopped");
    m_filterComboBox->addItem("Paused", "paused");
    m_filterComboBox->addItem("Images", "images");
    m_searchLayout->addWidget(m_filterComboBox);
    
    // Search button
    m_searchButton = new QPushButton("Search");
    connect(m_searchButton, &QPushButton::clicked, this, &ContainerManager::searchContainers);
    m_searchLayout->addWidget(m_searchButton);
    
    // Clear button
    m_clearSearchButton = new QPushButton("Clear");
    connect(m_clearSearchButton, &QPushButton::clicked, [this]() {
        m_searchEdit->clear();
        refreshContainers();
        refreshImages();
        refreshDistroboxContainers();
    });
    m_searchLayout->addWidget(m_clearSearchButton);
    
    m_mainLayout->addLayout(m_searchLayout);
}

void ContainerManager::setupProgressArea()
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

bool ContainerManager::isDockerAvailable()
{
    QProcess process;
    process.start("docker", QStringList() << "--version");
    return process.waitForFinished(500) && process.exitCode() == 0; // Very short timeout
}

bool ContainerManager::isPodmanAvailable()
{
    QProcess process;
    process.start("podman", QStringList() << "--version");
    return process.waitForFinished(500) && process.exitCode() == 0; // Very short timeout
}

bool ContainerManager::isDistroboxAvailable()
{
    QProcess process;
    process.start("distrobox", QStringList() << "--version");
    return process.waitForFinished(500) && process.exitCode() == 0; // Very short timeout
}

void ContainerManager::updateTheme()
{
    // Apply modern styling
    setStyleSheet(R"(
        QTabWidget::pane {
            border: 1px solid #c0c0c0;
            border-radius: 6px;
            background: white;
        }
        QTabBar::tab {
            background: #f0f0f0;
            border: 1px solid #c0c0c0;
            border-bottom: none;
            border-radius: 6px 6px 0 0;
            min-width: 100px;
            padding: 8px 16px;
            margin-right: 2px;
        }
        QTabBar::tab:selected {
            background: white;
            border-bottom: 1px solid white;
        }
        QTabBar::tab:hover {
            background: #e0e0e0;
        }
        QTableWidget {
            border: 1px solid #c0c0c0;
            border-radius: 4px;
            background: white;
            alternate-background-color: #f8f8f8;
            gridline-color: #e0e0e0;
        }
        QTableWidget::item {
            padding: 4px 8px;
        }
        QTableWidget::item:selected {
            background: #007acc;
            color: white;
        }
        QPushButton {
            background: #f0f0f0;
            border: 1px solid #c0c0c0;
            border-radius: 4px;
            padding: 6px 12px;
            min-width: 80px;
        }
        QPushButton:hover {
            background: #e0e0e0;
        }
        QPushButton:pressed {
            background: #d0d0d0;
        }
        QPushButton:disabled {
            background: #f5f5f5;
            color: #a0a0a0;
        }
    )");
}

void ContainerManager::showProgress(const QString &title, const QString &message)
{
    m_progressBar->setVisible(true);
    m_progressLabel->setVisible(true);
    m_progressLabel->setText(title);
    m_outputTextEdit->setVisible(true);
    m_outputTextEdit->append(message);
    m_statusLabel->setText("Working...");
}

void ContainerManager::hideProgress()
{
    m_progressBar->setVisible(false);
    m_progressLabel->setVisible(false);
    m_outputTextEdit->setVisible(false);
    m_statusLabel->setText("Ready");
}

void ContainerManager::showError(const QString &title, const QString &message)
{
    QMessageBox::critical(this, title, message);
    m_statusLabel->setText("Error: " + message);
}

void ContainerManager::showSuccess(const QString &title, const QString &message)
{
    m_statusLabel->setText("Success: " + message);
    QTimer::singleShot(3000, [this]() {
        m_statusLabel->setText("Ready");
    });
}

void ContainerManager::showInfo(const QString &title, const QString &message)
{
    QMessageBox::information(this, title, message);
}

QString ContainerManager::formatSize(qint64 bytes)
{
    if (bytes < 1024) return QString("%1 B").arg(bytes);
    if (bytes < 1024 * 1024) return QString("%1 KB").arg(bytes / 1024.0, 0, 'f', 1);
    if (bytes < 1024 * 1024 * 1024) return QString("%1 MB").arg(bytes / (1024.0 * 1024.0), 0, 'f', 1);
    return QString("%1 GB").arg(bytes / (1024.0 * 1024.0 * 1024.0), 0, 'f', 1);
}

QString ContainerManager::formatDuration(const QDateTime &started)
{
    qint64 seconds = started.secsTo(QDateTime::currentDateTime());
    if (seconds < 60) return QString("%1s").arg(seconds);
    if (seconds < 3600) return QString("%1m").arg(seconds / 60);
    if (seconds < 86400) return QString("%1h").arg(seconds / 3600);
    return QString("%1d").arg(seconds / 86400);
}

QString ContainerManager::getStatusColor(const QString &status)
{
    if (status == "running") return "#4CAF50";
    if (status == "stopped" || status == "exited") return "#FF5722";
    if (status == "paused") return "#FF9800";
    if (status == "created") return "#2196F3";
    return "#666666";
}

QString ContainerManager::getContainerIcon(const QString &status)
{
    if (status == "running") return "▶";
    if (status == "stopped" || status == "exited") return "⏹";
    if (status == "paused") return "⏸";
    if (status == "created") return "⭘";
    return "?";
}

QString ContainerManager::getImageIcon(const QString &type)
{
    if (type.contains("ubuntu")) return "🐧";
    if (type.contains("alpine")) return "🏔";
    if (type.contains("nginx")) return "🌐";
    if (type.contains("redis")) return "🔴";
    if (type.contains("postgres")) return "🐘";
    if (type.contains("mysql")) return "🐬";
    return "📦";
}

void ContainerManager::onProgressUpdated(const QString &taskId, int progress, const QString &message)
{
    m_progressBar->setValue(progress);
    m_outputTextEdit->append(message);
    
    // Auto-scroll to bottom
    QScrollBar *scrollBar = m_outputTextEdit->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

void ContainerManager::onTaskCompleted(const QString &taskId, bool success, const QString &message)
{
    if (success) {
        showSuccess("Task Complete", message);
        // Refresh after successful operations
        refreshContainers();
        refreshImages();
        refreshDistroboxContainers();
    } else {
        showError("Task Failed", message);
    }
    
    QTimer::singleShot(2000, this, &ContainerManager::hideProgress);
}

void ContainerManager::onTaskFinished(int taskId, int exitCode, const QString &output)
{
    if (exitCode == 0) {
        showSuccess("Task Completed", QString("Task %1 completed successfully").arg(taskId));
    } else {
        showError("Task Failed", QString("Task %1 failed with exit code %2").arg(taskId).arg(exitCode));
    }
    
    if (!output.isEmpty()) {
        m_outputTextEdit->append(output);
    }
    
    // Auto-refresh after certain operations
    if (m_autoRefresh) {
        refreshContainers();
        refreshImages();
        refreshDistroboxContainers();
    }
    
    QTimer::singleShot(2000, this, &ContainerManager::hideProgress);
}

void ContainerManager::onTaskProgress(int taskId, const QString &progress)
{
    m_outputTextEdit->append(progress);
    
    // Auto-scroll to bottom
    QScrollBar *scrollBar = m_outputTextEdit->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

void ContainerManager::onTaskStarted(int taskId, const QString &description)
{
    showProgress("Task Started", QString("Starting: %1").arg(description));
    m_outputTextEdit->append(QString("Started: %1").arg(description));
}

void ContainerManager::onTaskError(int taskId, const QString &error)
{
    showError("Task Error", error);
    m_outputTextEdit->append(QString("Error: %1").arg(error));
    QTimer::singleShot(2000, this, &ContainerManager::hideProgress);
}

void ContainerManager::onRefreshTimer()
{
    if (m_autoRefresh && !m_isSearching) {
        refreshContainers();
        refreshImages();
        refreshDistroboxContainers();
    }
}

void ContainerManager::setupContainerTab()
{
    m_containerTab = new QWidget();
    m_containerLayout = new QVBoxLayout(m_containerTab);
    m_containerLayout->setContentsMargins(12, 12, 12, 12);
    m_containerLayout->setSpacing(8);
    
    // Container table
    m_containerTable = new QTableWidget();
    m_containerTable->setAlternatingRowColors(true);
    m_containerTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_containerTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_containerTable->setSortingEnabled(true);
    m_containerTable->setContextMenuPolicy(Qt::CustomContextMenu);
    
    // Set up columns
    m_containerTable->setColumnCount(7);
    QStringList headers;
    headers << "Container ID" << "Name" << "Image" << "Status" << "Created" << "Ports" << "Size";
    m_containerTable->setHorizontalHeaderLabels(headers);
    
    // Configure column widths
    QHeaderView *header = m_containerTable->horizontalHeader();
    header->setStretchLastSection(true);
    header->resizeSection(CONTAINER_TABLE_ID_COLUMN, 120);
    header->resizeSection(CONTAINER_TABLE_NAME_COLUMN, 150);
    header->resizeSection(CONTAINER_TABLE_IMAGE_COLUMN, 200);
    header->resizeSection(CONTAINER_TABLE_STATUS_COLUMN, 100);
    header->resizeSection(CONTAINER_TABLE_CREATED_COLUMN, 120);
    header->resizeSection(CONTAINER_TABLE_PORTS_COLUMN, 150);
    
    connect(m_containerTable, &QTableWidget::customContextMenuRequested,
            this, &ContainerManager::onContainerTableContextMenu);
    connect(m_containerTable, &QTableWidget::itemSelectionChanged,
            this, &ContainerManager::onContainerSelectionChanged);
    
    m_containerLayout->addWidget(m_containerTable);
    
    // Button layout
    m_containerButtonLayout = new QHBoxLayout();
    m_containerButtonLayout->setSpacing(8);
    
    m_createContainerButton = new QPushButton("Create");
    m_createContainerButton->setIcon(QIcon(":/icons/add.png"));
    connect(m_createContainerButton, &QPushButton::clicked, this, &ContainerManager::showCreateContainerDialog);
    m_containerButtonLayout->addWidget(m_createContainerButton);
    
    m_startContainerButton = new QPushButton("Start");
    m_startContainerButton->setIcon(QIcon(":/icons/play.png"));
    m_startContainerButton->setEnabled(false);
    connect(m_startContainerButton, &QPushButton::clicked, this, &ContainerManager::onContainerActionTriggered);
    m_containerButtonLayout->addWidget(m_startContainerButton);
    
    m_stopContainerButton = new QPushButton("Stop");
    m_stopContainerButton->setIcon(QIcon(":/icons/stop.png"));
    m_stopContainerButton->setEnabled(false);
    connect(m_stopContainerButton, &QPushButton::clicked, this, &ContainerManager::onContainerActionTriggered);
    m_containerButtonLayout->addWidget(m_stopContainerButton);
    
    m_restartContainerButton = new QPushButton("Restart");
    m_restartContainerButton->setIcon(QIcon(":/icons/restart.png"));
    m_restartContainerButton->setEnabled(false);
    connect(m_restartContainerButton, &QPushButton::clicked, this, &ContainerManager::onContainerActionTriggered);
    m_containerButtonLayout->addWidget(m_restartContainerButton);
    
    m_removeContainerButton = new QPushButton("Remove");
    m_removeContainerButton->setIcon(QIcon(":/icons/delete.png"));
    m_removeContainerButton->setEnabled(false);
    connect(m_removeContainerButton, &QPushButton::clicked, this, &ContainerManager::onContainerActionTriggered);
    m_containerButtonLayout->addWidget(m_removeContainerButton);
    
    m_containerButtonLayout->addStretch();
    
    m_logsContainerButton = new QPushButton("Logs");
    m_logsContainerButton->setIcon(QIcon(":/icons/logs.png"));
    m_logsContainerButton->setEnabled(false);
    connect(m_logsContainerButton, &QPushButton::clicked, this, &ContainerManager::showContainerLogs);
    m_containerButtonLayout->addWidget(m_logsContainerButton);
    
    m_inspectContainerButton = new QPushButton("Inspect");
    m_inspectContainerButton->setIcon(QIcon(":/icons/inspect.png"));
    m_inspectContainerButton->setEnabled(false);
    connect(m_inspectContainerButton, &QPushButton::clicked, this, &ContainerManager::showContainerInspect);
    m_containerButtonLayout->addWidget(m_inspectContainerButton);
    
    m_attachContainerButton = new QPushButton("Attach");
    m_attachContainerButton->setIcon(QIcon(":/icons/terminal.png"));
    m_attachContainerButton->setEnabled(false);
    connect(m_attachContainerButton, &QPushButton::clicked, this, &ContainerManager::onContainerActionTriggered);
    m_containerButtonLayout->addWidget(m_attachContainerButton);
    
    m_execContainerButton = new QPushButton("Execute");
    m_execContainerButton->setIcon(QIcon(":/icons/execute.png"));
    m_execContainerButton->setEnabled(false);
    connect(m_execContainerButton, &QPushButton::clicked, this, &ContainerManager::onContainerActionTriggered);
    m_containerButtonLayout->addWidget(m_execContainerButton);
    
    m_refreshContainerButton = new QPushButton("Refresh");
    m_refreshContainerButton->setIcon(QIcon(":/icons/refresh.png"));
    connect(m_refreshContainerButton, &QPushButton::clicked, this, &ContainerManager::refreshContainers);
    m_containerButtonLayout->addWidget(m_refreshContainerButton);
    
    m_containerLayout->addLayout(m_containerButtonLayout);
    
    m_tabWidget->addTab(m_containerTab, "Containers");
}

void ContainerManager::setupImageTab()
{
    m_imageTab = new QWidget();
    m_imageLayout = new QVBoxLayout(m_imageTab);
    m_imageLayout->setContentsMargins(12, 12, 12, 12);
    m_imageLayout->setSpacing(8);
    
    // Image table
    m_imageTable = new QTableWidget();
    m_imageTable->setAlternatingRowColors(true);
    m_imageTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_imageTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_imageTable->setSortingEnabled(true);
    m_imageTable->setContextMenuPolicy(Qt::CustomContextMenu);
    
    // Set up columns
    m_imageTable->setColumnCount(5);
    QStringList headers;
    headers << "Image ID" << "Repository" << "Tag" << "Size" << "Created";
    m_imageTable->setHorizontalHeaderLabels(headers);
    
    // Configure column widths
    QHeaderView *header = m_imageTable->horizontalHeader();
    header->setStretchLastSection(true);
    header->resizeSection(IMAGE_TABLE_ID_COLUMN, 120);
    header->resizeSection(IMAGE_TABLE_REPOSITORY_COLUMN, 200);
    header->resizeSection(IMAGE_TABLE_TAG_COLUMN, 100);
    header->resizeSection(IMAGE_TABLE_SIZE_COLUMN, 100);
    
    connect(m_imageTable, &QTableWidget::customContextMenuRequested,
            this, &ContainerManager::onImageTableContextMenu);
    connect(m_imageTable, &QTableWidget::itemSelectionChanged,
            this, &ContainerManager::onImageSelectionChanged);
    
    m_imageLayout->addWidget(m_imageTable);
    
    // Button layout
    m_imageButtonLayout = new QHBoxLayout();
    m_imageButtonLayout->setSpacing(8);
    
    m_pullImageButton = new QPushButton("Pull");
    m_pullImageButton->setIcon(QIcon(":/icons/download.png"));
    connect(m_pullImageButton, &QPushButton::clicked, this, &ContainerManager::pullImage);
    m_imageButtonLayout->addWidget(m_pullImageButton);
    
    m_buildImageButton = new QPushButton("Build");
    m_buildImageButton->setIcon(QIcon(":/icons/build.png"));
    connect(m_buildImageButton, &QPushButton::clicked, this, &ContainerManager::buildImage);
    m_imageButtonLayout->addWidget(m_buildImageButton);
    
    m_removeImageButton = new QPushButton("Remove");
    m_removeImageButton->setIcon(QIcon(":/icons/delete.png"));
    m_removeImageButton->setEnabled(false);
    connect(m_removeImageButton, &QPushButton::clicked, this, &ContainerManager::onImageActionTriggered);
    m_imageButtonLayout->addWidget(m_removeImageButton);
    
    m_tagImageButton = new QPushButton("Tag");
    m_tagImageButton->setIcon(QIcon(":/icons/tag.png"));
    m_tagImageButton->setEnabled(false);
    connect(m_tagImageButton, &QPushButton::clicked, this, &ContainerManager::tagImage);
    m_imageButtonLayout->addWidget(m_tagImageButton);
    
    m_pushImageButton = new QPushButton("Push");
    m_pushImageButton->setIcon(QIcon(":/icons/upload.png"));
    m_pushImageButton->setEnabled(false);
    connect(m_pushImageButton, &QPushButton::clicked, this, &ContainerManager::pushImage);
    m_imageButtonLayout->addWidget(m_pushImageButton);
    
    m_imageButtonLayout->addStretch();
    
    m_saveImageButton = new QPushButton("Save");
    m_saveImageButton->setIcon(QIcon(":/icons/save.png"));
    m_saveImageButton->setEnabled(false);
    connect(m_saveImageButton, &QPushButton::clicked, this, &ContainerManager::saveImage);
    m_imageButtonLayout->addWidget(m_saveImageButton);
    
    m_loadImageButton = new QPushButton("Load");
    m_loadImageButton->setIcon(QIcon(":/icons/load.png"));
    connect(m_loadImageButton, &QPushButton::clicked, this, &ContainerManager::loadImage);
    m_imageButtonLayout->addWidget(m_loadImageButton);
    
    m_inspectImageButton = new QPushButton("Inspect");
    m_inspectImageButton->setIcon(QIcon(":/icons/inspect.png"));
    m_inspectImageButton->setEnabled(false);
    connect(m_inspectImageButton, &QPushButton::clicked, this, &ContainerManager::showImageInspect);
    m_imageButtonLayout->addWidget(m_inspectImageButton);
    
    m_refreshImageButton = new QPushButton("Refresh");
    m_refreshImageButton->setIcon(QIcon(":/icons/refresh.png"));
    connect(m_refreshImageButton, &QPushButton::clicked, this, &ContainerManager::refreshImages);
    m_imageButtonLayout->addWidget(m_refreshImageButton);
    
    m_imageLayout->addLayout(m_imageButtonLayout);
    
    m_tabWidget->addTab(m_imageTab, "Images");
}

void ContainerManager::setupDistroboxTab()
{
    m_distroboxTab = new QWidget();
    m_distroboxLayout = new QVBoxLayout(m_distroboxTab);
    m_distroboxLayout->setContentsMargins(12, 12, 12, 12);
    m_distroboxLayout->setSpacing(8);
    
    // Distrobox table
    m_distroboxTable = new QTableWidget();
    m_distroboxTable->setAlternatingRowColors(true);
    m_distroboxTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_distroboxTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_distroboxTable->setSortingEnabled(true);
    m_distroboxTable->setContextMenuPolicy(Qt::CustomContextMenu);
    
    // Set up columns
    m_distroboxTable->setColumnCount(4);
    QStringList headers;
    headers << "Name" << "Status" << "Image" << "Path";
    m_distroboxTable->setHorizontalHeaderLabels(headers);
    
    // Configure column widths
    QHeaderView *header = m_distroboxTable->horizontalHeader();
    header->setStretchLastSection(true);
    header->resizeSection(DISTROBOX_TABLE_NAME_COLUMN, 150);
    header->resizeSection(DISTROBOX_TABLE_STATUS_COLUMN, 100);
    header->resizeSection(DISTROBOX_TABLE_IMAGE_COLUMN, 200);
    
    connect(m_distroboxTable, &QTableWidget::customContextMenuRequested,
            this, &ContainerManager::onDistroboxTableContextMenu);
    connect(m_distroboxTable, &QTableWidget::itemSelectionChanged,
            this, &ContainerManager::onDistroboxSelectionChanged);
    
    m_distroboxLayout->addWidget(m_distroboxTable);
    
    // Button layout
    m_distroboxButtonLayout = new QHBoxLayout();
    m_distroboxButtonLayout->setSpacing(8);
    
    m_createDistroboxButton = new QPushButton("Create");
    m_createDistroboxButton->setIcon(QIcon(":/icons/add.png"));
    connect(m_createDistroboxButton, &QPushButton::clicked, this, &ContainerManager::showCreateDistroboxDialog);
    m_distroboxButtonLayout->addWidget(m_createDistroboxButton);
    
    m_enterDistroboxButton = new QPushButton("Enter");
    m_enterDistroboxButton->setIcon(QIcon(":/icons/terminal.png"));
    m_enterDistroboxButton->setEnabled(false);
    connect(m_enterDistroboxButton, &QPushButton::clicked, this, &ContainerManager::onDistroboxActionTriggered);
    m_distroboxButtonLayout->addWidget(m_enterDistroboxButton);
    
    m_stopDistroboxButton = new QPushButton("Stop");
    m_stopDistroboxButton->setIcon(QIcon(":/icons/stop.png"));
    m_stopDistroboxButton->setEnabled(false);
    connect(m_stopDistroboxButton, &QPushButton::clicked, this, &ContainerManager::onDistroboxActionTriggered);
    m_distroboxButtonLayout->addWidget(m_stopDistroboxButton);
    
    m_removeDistroboxButton = new QPushButton("Remove");
    m_removeDistroboxButton->setIcon(QIcon(":/icons/delete.png"));
    m_removeDistroboxButton->setEnabled(false);
    connect(m_removeDistroboxButton, &QPushButton::clicked, this, &ContainerManager::onDistroboxActionTriggered);
    m_distroboxButtonLayout->addWidget(m_removeDistroboxButton);
    
    m_distroboxButtonLayout->addStretch();
    
    m_upgradeDistroboxButton = new QPushButton("Upgrade");
    m_upgradeDistroboxButton->setIcon(QIcon(":/icons/upgrade.png"));
    m_upgradeDistroboxButton->setEnabled(false);
    connect(m_upgradeDistroboxButton, &QPushButton::clicked, this, &ContainerManager::onDistroboxActionTriggered);
    m_distroboxButtonLayout->addWidget(m_upgradeDistroboxButton);
    
    m_generateEntryButton = new QPushButton("Generate Entry");
    m_generateEntryButton->setIcon(QIcon(":/icons/generate.png"));
    m_generateEntryButton->setEnabled(false);
    connect(m_generateEntryButton, &QPushButton::clicked, this, &ContainerManager::onDistroboxActionTriggered);
    m_distroboxButtonLayout->addWidget(m_generateEntryButton);
    
    m_refreshDistroboxButton = new QPushButton("Refresh");
    m_refreshDistroboxButton->setIcon(QIcon(":/icons/refresh.png"));
    connect(m_refreshDistroboxButton, &QPushButton::clicked, this, &ContainerManager::refreshDistroboxContainers);
    m_distroboxButtonLayout->addWidget(m_refreshDistroboxButton);
    
    m_distroboxLayout->addLayout(m_distroboxButtonLayout);
    
    m_tabWidget->addTab(m_distroboxTab, "Distrobox");
}

void ContainerManager::setupContextMenus()
{
    // Container context menu
    m_containerContextMenu = new QMenu(this);
    m_containerContextMenu->addAction("Start", [this]() { onContainerActionTriggered(); });
    m_containerContextMenu->addAction("Stop", [this]() { onContainerActionTriggered(); });
    m_containerContextMenu->addAction("Restart", [this]() { onContainerActionTriggered(); });
    m_containerContextMenu->addAction("Pause", [this]() { onContainerActionTriggered(); });
    m_containerContextMenu->addAction("Unpause", [this]() { onContainerActionTriggered(); });
    m_containerContextMenu->addSeparator();
    m_containerContextMenu->addAction("Remove", [this]() { onContainerActionTriggered(); });
    m_containerContextMenu->addAction("Kill", [this]() { onContainerActionTriggered(); });
    m_containerContextMenu->addSeparator();
    m_containerContextMenu->addAction("Logs", this, &ContainerManager::showContainerLogs);
    m_containerContextMenu->addAction("Inspect", this, &ContainerManager::showContainerInspect);
    m_containerContextMenu->addAction("Attach", [this]() { onContainerActionTriggered(); });
    m_containerContextMenu->addAction("Execute", [this]() { onContainerActionTriggered(); });
    m_containerContextMenu->addSeparator();
    m_containerContextMenu->addAction("Export", this, &ContainerManager::exportContainer);
    m_containerContextMenu->addAction("Commit", [this]() { onContainerActionTriggered(); });
    
    // Image context menu
    m_imageContextMenu = new QMenu(this);
    m_imageContextMenu->addAction("Remove", [this]() { onImageActionTriggered(); });
    m_imageContextMenu->addAction("Tag", this, &ContainerManager::tagImage);
    m_imageContextMenu->addAction("Push", this, &ContainerManager::pushImage);
    m_imageContextMenu->addSeparator();
    m_imageContextMenu->addAction("Save", this, &ContainerManager::saveImage);
    m_imageContextMenu->addAction("Inspect", this, &ContainerManager::showImageInspect);
    m_imageContextMenu->addAction("History", [this]() { onImageActionTriggered(); });
    m_imageContextMenu->addSeparator();
    m_imageContextMenu->addAction("Create Container", this, &ContainerManager::showCreateContainerDialog);
    
    // Distrobox context menu
    m_distroboxContextMenu = new QMenu(this);
    m_distroboxContextMenu->addAction("Enter", [this]() { onDistroboxActionTriggered(); });
    m_distroboxContextMenu->addAction("Stop", [this]() { onDistroboxActionTriggered(); });
    m_distroboxContextMenu->addAction("Remove", [this]() { onDistroboxActionTriggered(); });
    m_distroboxContextMenu->addSeparator();
    m_distroboxContextMenu->addAction("Upgrade", [this]() { onDistroboxActionTriggered(); });
    m_distroboxContextMenu->addAction("Generate Entry", [this]() { onDistroboxActionTriggered(); });
}

void ContainerManager::refreshContainers()
{
    if (m_isSearching) return;
    
    m_isSearching = true;
    m_statusLabel->setText("Refreshing containers...");
    
    QStringList args;
    args << "ps" << "-a" << "--format" << "json";
    
    if (m_privilegedExecutor && m_systemUtils) {
        QString taskId = "refresh_containers";
        m_privilegedExecutor->executeCommand("echo", args);
    }
}

void ContainerManager::refreshImages()
{
    if (m_isSearching) return;
    
    m_isSearching = true;
    m_statusLabel->setText("Refreshing images...");
    
    QStringList args;
    args << "images" << "--format" << "json";
    
    if (m_privilegedExecutor && m_systemUtils) {
        QString taskId = "refresh_images";
        m_privilegedExecutor->executeCommand("echo", args);
    }
}

void ContainerManager::refreshDistroboxContainers()
{
    if (!isDistroboxAvailable()) return;
    
    m_statusLabel->setText("Refreshing Distrobox containers...");
    
    QStringList args;
    args << "list";
    
    if (m_privilegedExecutor && m_systemUtils) {
        QString taskId = "refresh_distrobox";
        m_privilegedExecutor->executeCommand("echo", args);
    }
}

void ContainerManager::searchContainers()
{
    if (m_isSearching) return;
    
    QString searchTerm = m_searchEdit->text().trimmed();
    QString filter = m_filterComboBox->currentData().toString();
    
    if (m_searchWorker) {
        m_searchWorker->stop();
        m_searchWorker->wait(1000);
        delete m_searchWorker;
    }
    
    m_searchWorker = new ContainerSearchWorker(m_defaultRuntime, this);
    m_searchWorker->setParameters(searchTerm, m_defaultRuntime);
    
    connect(m_searchWorker, &ContainerSearchWorker::containerFound,
            this, &ContainerManager::onContainerFound);
    connect(m_searchWorker, &ContainerSearchWorker::imageFound,
            this, &ContainerManager::onImageFound);
    connect(m_searchWorker, &ContainerSearchWorker::searchFinished,
            this, &ContainerManager::onSearchFinished);
    connect(m_searchWorker, &ContainerSearchWorker::errorOccurred,
            this, &ContainerManager::onSearchError);
    
    m_isSearching = true;
    m_statusLabel->setText("Searching...");
    m_searchButton->setEnabled(false);
    
    m_searchWorker->start();
}

void ContainerManager::searchImages()
{
    // Images are searched together with containers
    searchContainers();
}

void ContainerManager::onContainerFound(const QJsonObject &containerInfo)
{
    QMutexLocker locker(&m_dataMutex);
    
    // Check if container already exists
    bool found = false;
    for (int i = 0; i < m_containers.size(); ++i) {
        if (m_containers[i]["ID"].toString() == containerInfo["ID"].toString()) {
            m_containers[i] = containerInfo;
            found = true;
            break;
        }
    }
    
    if (!found) {
        m_containers.append(containerInfo);
    }
    
    // Update UI in the main thread
    QMetaObject::invokeMethod(this, &ContainerManager::updateContainerTable, Qt::QueuedConnection);
}

void ContainerManager::onImageFound(const QJsonObject &imageInfo)
{
    QMutexLocker locker(&m_dataMutex);
    
    // Check if image already exists
    bool found = false;
    for (int i = 0; i < m_images.size(); ++i) {
        if (m_images[i]["ID"].toString() == imageInfo["ID"].toString()) {
            m_images[i] = imageInfo;
            found = true;
            break;
        }
    }
    
    if (!found) {
        m_images.append(imageInfo);
    }
    
    // Update UI in the main thread
    QMetaObject::invokeMethod(this, &ContainerManager::updateImageTable, Qt::QueuedConnection);
}

void ContainerManager::onSearchFinished()
{
    m_isSearching = false;
    m_searchButton->setEnabled(true);
    m_statusLabel->setText("Ready");
}

void ContainerManager::onSearchError(const QString &error)
{
    m_isSearching = false;
    m_searchButton->setEnabled(true);
    showError("Search Error", error);
}

void ContainerManager::onContainerActionTriggered()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;
    
    QList<QTableWidgetItem*> selectedItems = m_containerTable->selectedItems();
    if (selectedItems.isEmpty()) return;
    
    QString containerId = m_containerTable->item(selectedItems.first()->row(), CONTAINER_TABLE_ID_COLUMN)->text();
    
    if (button == m_startContainerButton) {
        startContainer(containerId);
    } else if (button == m_stopContainerButton) {
        stopContainer(containerId);
    } else if (button == m_restartContainerButton) {
        restartContainer(containerId);
    } else if (button == m_removeContainerButton) {
        removeContainer(containerId);
    } else if (button == m_attachContainerButton) {
        attachToContainer(containerId);
    } else if (button == m_execContainerButton) {
        bool ok;
        QString command = QInputDialog::getText(this, "Execute Command", 
                                               "Command to execute:", QLineEdit::Normal, 
                                               "/bin/bash", &ok);
        if (ok && !command.isEmpty()) {
            executeInContainer(containerId, command);
        }
    }
}

void ContainerManager::onImageActionTriggered()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;
    
    QList<QTableWidgetItem*> selectedItems = m_imageTable->selectedItems();
    if (selectedItems.isEmpty()) return;
    
    QString imageId = m_imageTable->item(selectedItems.first()->row(), IMAGE_TABLE_ID_COLUMN)->text();
    
    if (button == m_removeImageButton) {
        removeImage(imageId);
    }
}

void ContainerManager::onDistroboxActionTriggered()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;
    
    QList<QTableWidgetItem*> selectedItems = m_distroboxTable->selectedItems();
    if (selectedItems.isEmpty()) return;
    
    QString containerName = m_distroboxTable->item(selectedItems.first()->row(), DISTROBOX_TABLE_NAME_COLUMN)->text();
    
    if (button == m_enterDistroboxButton) {
        enterDistroboxContainer(containerName);
    } else if (button == m_stopDistroboxButton) {
        stopDistroboxContainer(containerName);
    } else if (button == m_removeDistroboxButton) {
        removeDistroboxContainer(containerName);
    } else if (button == m_upgradeDistroboxButton) {
        upgradeDistroboxContainer(containerName);
    } else if (button == m_generateEntryButton) {
        bool ok;
        QString appName = QInputDialog::getText(this, "Generate Entry", 
                                               "Application name:", QLineEdit::Normal, 
                                               "", &ok);
        if (ok && !appName.isEmpty()) {
            generateDistroboxEntry(containerName, appName);
        }
    }
}

void ContainerManager::onContainerTableContextMenu(const QPoint &pos)
{
    if (m_containerTable->itemAt(pos)) {
        m_containerContextMenu->exec(m_containerTable->mapToGlobal(pos));
    }
}

void ContainerManager::onImageTableContextMenu(const QPoint &pos)
{
    if (m_imageTable->itemAt(pos)) {
        m_imageContextMenu->exec(m_imageTable->mapToGlobal(pos));
    }
}

void ContainerManager::onDistroboxTableContextMenu(const QPoint &pos)
{
    if (m_distroboxTable->itemAt(pos)) {
        m_distroboxContextMenu->exec(m_distroboxTable->mapToGlobal(pos));
    }
}

void ContainerManager::onContainerSelectionChanged()
{
    QList<QTableWidgetItem*> selectedItems = m_containerTable->selectedItems();
    bool hasSelection = !selectedItems.isEmpty();
    
    m_startContainerButton->setEnabled(hasSelection);
    m_stopContainerButton->setEnabled(hasSelection);
    m_restartContainerButton->setEnabled(hasSelection);
    m_removeContainerButton->setEnabled(hasSelection);
    m_logsContainerButton->setEnabled(hasSelection);
    m_inspectContainerButton->setEnabled(hasSelection);
    m_attachContainerButton->setEnabled(hasSelection);
    m_execContainerButton->setEnabled(hasSelection);
}

void ContainerManager::onImageSelectionChanged()
{
    QList<QTableWidgetItem*> selectedItems = m_imageTable->selectedItems();
    bool hasSelection = !selectedItems.isEmpty();
    
    m_removeImageButton->setEnabled(hasSelection);
    m_tagImageButton->setEnabled(hasSelection);
    m_pushImageButton->setEnabled(hasSelection);
    m_saveImageButton->setEnabled(hasSelection);
    m_inspectImageButton->setEnabled(hasSelection);
}

void ContainerManager::onDistroboxSelectionChanged()
{
    QList<QTableWidgetItem*> selectedItems = m_distroboxTable->selectedItems();
    bool hasSelection = !selectedItems.isEmpty();
    
    m_enterDistroboxButton->setEnabled(hasSelection);
    m_stopDistroboxButton->setEnabled(hasSelection);
    m_removeDistroboxButton->setEnabled(hasSelection);
    m_upgradeDistroboxButton->setEnabled(hasSelection);
    m_generateEntryButton->setEnabled(hasSelection);
}

void ContainerManager::startContainer(const QString &containerId)
{
    if (m_privilegedExecutor) {
        QString taskId = "start_container_" + containerId;
        QStringList args;
        args << "start" << containerId;
        
        showProgress("Starting Container", "Starting container " + containerId);
        m_privilegedExecutor->executeCommand(m_defaultRuntime, args);
    }
}

void ContainerManager::stopContainer(const QString &containerId)
{
    if (m_privilegedExecutor) {
        QString taskId = "stop_container_" + containerId;
        QStringList args;
        args << "stop" << containerId;
        
        showProgress("Stopping Container", "Stopping container " + containerId);
        m_privilegedExecutor->executeCommand(m_defaultRuntime, args);
    }
}

void ContainerManager::restartContainer(const QString &containerId)
{
    if (m_privilegedExecutor) {
        QString taskId = "restart_container_" + containerId;
        QStringList args;
        args << "restart" << containerId;
        
        showProgress("Restarting Container", "Restarting container " + containerId);
        m_privilegedExecutor->executeCommand(m_defaultRuntime, args);
    }
}

void ContainerManager::removeContainer(const QString &containerId)
{
    int ret = QMessageBox::question(this, "Remove Container", 
                                   "Are you sure you want to remove this container?",
                                   QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        if (m_privilegedExecutor) {
            QString taskId = "remove_container_" + containerId;
            QStringList args;
            args << "rm" << "-f" << containerId;
            
            showProgress("Removing Container", "Removing container " + containerId);
            m_privilegedExecutor->executeCommand(m_defaultRuntime, args);
        }
    }
}

void ContainerManager::removeImage(const QString &imageId)
{
    int ret = QMessageBox::question(this, "Remove Image", 
                                   "Are you sure you want to remove this image?",
                                   QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        if (m_privilegedExecutor) {
            QString taskId = "remove_image_" + imageId;
            QStringList args;
            args << "rmi" << "-f" << imageId;
            
            showProgress("Removing Image", "Removing image " + imageId);
            m_privilegedExecutor->executeCommand(m_defaultRuntime, args);
        }
    }
}

void ContainerManager::pauseContainer(const QString &containerId)
{
    if (m_privilegedExecutor) {
        QString taskId = "pause_container_" + containerId;
        QStringList args;
        args << "pause" << containerId;
        
        showProgress("Pausing Container", "Pausing container " + containerId);
        m_privilegedExecutor->executeCommand(m_defaultRuntime, args);
    }
}

void ContainerManager::unpauseContainer(const QString &containerId)
{
    if (m_privilegedExecutor) {
        QString taskId = "unpause_container_" + containerId;
        QStringList args;
        args << "unpause" << containerId;
        
        showProgress("Unpausing Container", "Unpausing container " + containerId);
        m_privilegedExecutor->executeCommand(m_defaultRuntime, args);
    }
}

void ContainerManager::killContainer(const QString &containerId)
{
    if (m_privilegedExecutor) {
        QString taskId = "kill_container_" + containerId;
        QStringList args;
        args << "kill" << containerId;
        
        showProgress("Killing Container", "Killing container " + containerId);
        m_privilegedExecutor->executeCommand(m_defaultRuntime, args);
    }
}

void ContainerManager::attachToContainer(const QString &containerId)
{
    // Launch terminal to attach to container
    QString command = QString("%1 attach %2").arg(m_defaultRuntime).arg(containerId);
    QStringList args;
    args << "-e" << command;
    
    QProcess::startDetached("x-terminal-emulator", args);
}

void ContainerManager::executeInContainer(const QString &containerId, const QString &command)
{
    // Launch terminal to execute command in container
    QString fullCommand = QString("%1 exec -it %2 %3").arg(m_defaultRuntime).arg(containerId).arg(command);
    QStringList args;
    args << "-e" << fullCommand;
    
    QProcess::startDetached("x-terminal-emulator", args);
}

void ContainerManager::enterDistroboxContainer(const QString &name)
{
    // Launch terminal to enter distrobox
    QString command = QString("distrobox enter %1").arg(name);
    QStringList args;
    args << "-e" << command;
    
    QProcess::startDetached("x-terminal-emulator", args);
}

void ContainerManager::stopDistroboxContainer(const QString &name)
{
    if (m_privilegedExecutor) {
        QString taskId = "stop_distrobox_" + name;
        QStringList args;
        args << "stop" << name;
        
        showProgress("Stopping Distrobox", "Stopping distrobox " + name);
        m_privilegedExecutor->executeCommand("distrobox", args);
    }
}

void ContainerManager::removeDistroboxContainer(const QString &name)
{
    int ret = QMessageBox::question(this, "Remove Distrobox", 
                                   "Are you sure you want to remove this distrobox?",
                                   QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        if (m_privilegedExecutor) {
            QString taskId = "remove_distrobox_" + name;
            QStringList args;
            args << "rm" << name;
            
            showProgress("Removing Distrobox", "Removing distrobox " + name);
            m_privilegedExecutor->executeCommand("distrobox", args);
        }
    }
}

void ContainerManager::upgradeDistroboxContainer(const QString &name)
{
    if (m_privilegedExecutor) {
        QString taskId = "upgrade_distrobox_" + name;
        QStringList args;
        args << "upgrade" << name;
        
        showProgress("Upgrading Distrobox", "Upgrading distrobox " + name);
        m_privilegedExecutor->executeCommand("distrobox", args);
    }
}

void ContainerManager::generateDistroboxEntry(const QString &name, const QString &appName)
{
    if (m_privilegedExecutor) {
        QString taskId = "generate_entry_" + name;
        QStringList args;
        args << "generate-entry" << name << "--name" << appName;
        
        showProgress("Generating Entry", "Generating entry for " + name);
        m_privilegedExecutor->executeCommand("distrobox", args);
    }
}

void ContainerManager::updateContainerTable()
{
    QMutexLocker locker(&m_dataMutex);
    
    m_containerTable->setRowCount(m_containers.size());
    
    for (int i = 0; i < m_containers.size(); ++i) {
        const QJsonObject &container = m_containers[i];
        
        // ID
        QTableWidgetItem *idItem = new QTableWidgetItem(container["ID"].toString().left(12));
        idItem->setToolTip(container["ID"].toString());
        m_containerTable->setItem(i, CONTAINER_TABLE_ID_COLUMN, idItem);
        
        // Name
        QString name = container["Names"].toString();
        if (name.startsWith("/")) name = name.mid(1);
        QTableWidgetItem *nameItem = new QTableWidgetItem(name);
        nameItem->setToolTip(name);
        m_containerTable->setItem(i, CONTAINER_TABLE_NAME_COLUMN, nameItem);
        
        // Image
        QTableWidgetItem *imageItem = new QTableWidgetItem(container["Image"].toString());
        imageItem->setToolTip(container["Image"].toString());
        m_containerTable->setItem(i, CONTAINER_TABLE_IMAGE_COLUMN, imageItem);
        
        // Status
        QString status = container["Status"].toString();
        QTableWidgetItem *statusItem = new QTableWidgetItem(getContainerIcon(status) + " " + status);
        statusItem->setToolTip(status);
        statusItem->setForeground(QBrush(QColor(getStatusColor(status))));
        m_containerTable->setItem(i, CONTAINER_TABLE_STATUS_COLUMN, statusItem);
        
        // Created
        QString created = container["CreatedAt"].toString();
        if (created.isEmpty()) {
            created = formatDuration(QDateTime::fromSecsSinceEpoch(container["Created"].toInt()));
        }
        QTableWidgetItem *createdItem = new QTableWidgetItem(created);
        createdItem->setToolTip(created);
        m_containerTable->setItem(i, CONTAINER_TABLE_CREATED_COLUMN, createdItem);
        
        // Ports
        QString ports = container["Ports"].toString();
        QTableWidgetItem *portsItem = new QTableWidgetItem(ports);
        portsItem->setToolTip(ports);
        m_containerTable->setItem(i, CONTAINER_TABLE_PORTS_COLUMN, portsItem);
        
        // Size
        QString size = container["Size"].toString();
        QTableWidgetItem *sizeItem = new QTableWidgetItem(size);
        sizeItem->setToolTip(size);
        m_containerTable->setItem(i, CONTAINER_TABLE_SIZE_COLUMN, sizeItem);
    }
    
    // Auto-resize columns
    m_containerTable->resizeColumnsToContents();
}

void ContainerManager::updateImageTable()
{
    QMutexLocker locker(&m_dataMutex);
    
    m_imageTable->setRowCount(m_images.size());
    
    for (int i = 0; i < m_images.size(); ++i) {
        const QJsonObject &image = m_images[i];
        
        // ID
        QTableWidgetItem *idItem = new QTableWidgetItem(image["ID"].toString().left(12));
        idItem->setToolTip(image["ID"].toString());
        m_imageTable->setItem(i, IMAGE_TABLE_ID_COLUMN, idItem);
        
        // Repository
        QString repository = image["Repository"].toString();
        QTableWidgetItem *repoItem = new QTableWidgetItem(getImageIcon(repository) + " " + repository);
        repoItem->setToolTip(repository);
        m_imageTable->setItem(i, IMAGE_TABLE_REPOSITORY_COLUMN, repoItem);
        
        // Tag
        QTableWidgetItem *tagItem = new QTableWidgetItem(image["Tag"].toString());
        tagItem->setToolTip(image["Tag"].toString());
        m_imageTable->setItem(i, IMAGE_TABLE_TAG_COLUMN, tagItem);
        
        // Size
        QString size = image["Size"].toString();
        QTableWidgetItem *sizeItem = new QTableWidgetItem(size);
        sizeItem->setToolTip(size);
        m_imageTable->setItem(i, IMAGE_TABLE_SIZE_COLUMN, sizeItem);
        
        // Created
        QString created = image["CreatedAt"].toString();
        if (created.isEmpty()) {
            created = formatDuration(QDateTime::fromSecsSinceEpoch(image["Created"].toInt()));
        }
        QTableWidgetItem *createdItem = new QTableWidgetItem(created);
        createdItem->setToolTip(created);
        m_imageTable->setItem(i, IMAGE_TABLE_CREATED_COLUMN, createdItem);
    }
    
    // Auto-resize columns
    m_imageTable->resizeColumnsToContents();
}

void ContainerManager::updateDistroboxTable()
{
    QMutexLocker locker(&m_dataMutex);
    
    m_distroboxTable->setRowCount(m_distroboxContainers.size());
    
    for (int i = 0; i < m_distroboxContainers.size(); ++i) {
        const QJsonObject &container = m_distroboxContainers[i];
        
        // Name
        QTableWidgetItem *nameItem = new QTableWidgetItem(container["name"].toString());
        nameItem->setToolTip(container["name"].toString());
        m_distroboxTable->setItem(i, DISTROBOX_TABLE_NAME_COLUMN, nameItem);
        
        // Status
        QString status = container["status"].toString();
        QTableWidgetItem *statusItem = new QTableWidgetItem(getContainerIcon(status) + " " + status);
        statusItem->setToolTip(status);
        statusItem->setForeground(QBrush(QColor(getStatusColor(status))));
        m_distroboxTable->setItem(i, DISTROBOX_TABLE_STATUS_COLUMN, statusItem);
        
        // Image
        QTableWidgetItem *imageItem = new QTableWidgetItem(container["image"].toString());
        imageItem->setToolTip(container["image"].toString());
        m_distroboxTable->setItem(i, DISTROBOX_TABLE_IMAGE_COLUMN, imageItem);
        
        // Path
        QTableWidgetItem *pathItem = new QTableWidgetItem(container["path"].toString());
        pathItem->setToolTip(container["path"].toString());
        m_distroboxTable->setItem(i, DISTROBOX_TABLE_PATH_COLUMN, pathItem);
    }
    
    // Auto-resize columns
    m_distroboxTable->resizeColumnsToContents();
}

void ContainerManager::showCreateContainerDialog()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Create Container");
    dialog.setModal(true);
    dialog.resize(500, 400);
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    
    // Image selection
    QHBoxLayout *imageLayout = new QHBoxLayout();
    imageLayout->addWidget(new QLabel("Image:"));
    QComboBox *imageCombo = new QComboBox();
    
    // Populate with available images
    for (const QJsonObject &image : m_images) {
        QString imageStr = image["Repository"].toString() + ":" + image["Tag"].toString();
        imageCombo->addItem(imageStr);
    }
    imageCombo->setEditable(true);
    imageLayout->addWidget(imageCombo);
    layout->addLayout(imageLayout);
    
    // Container name
    QHBoxLayout *nameLayout = new QHBoxLayout();
    nameLayout->addWidget(new QLabel("Name:"));
    QLineEdit *nameEdit = new QLineEdit();
    nameLayout->addWidget(nameEdit);
    layout->addLayout(nameLayout);
    
    // Options
    QGroupBox *optionsGroup = new QGroupBox("Options");
    QVBoxLayout *optionsLayout = new QVBoxLayout(optionsGroup);
    
    QCheckBox *interactiveCheck = new QCheckBox("Interactive (-i)");
    interactiveCheck->setChecked(true);
    optionsLayout->addWidget(interactiveCheck);
    
    QCheckBox *ttyCheck = new QCheckBox("Allocate TTY (-t)");
    ttyCheck->setChecked(true);
    optionsLayout->addWidget(ttyCheck);
    
    QCheckBox *daemonCheck = new QCheckBox("Run as daemon (-d)");
    optionsLayout->addWidget(daemonCheck);
    
    QCheckBox *removeCheck = new QCheckBox("Auto-remove (--rm)");
    optionsLayout->addWidget(removeCheck);
    
    layout->addWidget(optionsGroup);
    
    // Port mapping
    QHBoxLayout *portLayout = new QHBoxLayout();
    portLayout->addWidget(new QLabel("Port mapping:"));
    QLineEdit *portEdit = new QLineEdit();
    portEdit->setPlaceholderText("8080:80");
    portLayout->addWidget(portEdit);
    layout->addLayout(portLayout);
    
    // Volume mapping
    QHBoxLayout *volumeLayout = new QHBoxLayout();
    volumeLayout->addWidget(new QLabel("Volume mapping:"));
    QLineEdit *volumeEdit = new QLineEdit();
    volumeEdit->setPlaceholderText("/host/path:/container/path");
    volumeLayout->addWidget(volumeEdit);
    layout->addLayout(volumeLayout);
    
    // Command
    QHBoxLayout *commandLayout = new QHBoxLayout();
    commandLayout->addWidget(new QLabel("Command:"));
    QLineEdit *commandEdit = new QLineEdit();
    commandEdit->setPlaceholderText("/bin/bash");
    commandLayout->addWidget(commandEdit);
    layout->addLayout(commandLayout);
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *createButton = new QPushButton("Create");
    QPushButton *cancelButton = new QPushButton("Cancel");
    buttonLayout->addStretch();
    buttonLayout->addWidget(createButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);
    
    connect(createButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        QString imageName = imageCombo->currentText();
        QString containerName = nameEdit->text();
        
        QStringList options;
        if (interactiveCheck->isChecked()) options << "-i";
        if (ttyCheck->isChecked()) options << "-t";
        if (daemonCheck->isChecked()) options << "-d";
        if (removeCheck->isChecked()) options << "--rm";
        
        if (!containerName.isEmpty()) {
            options << "--name" << containerName;
        }
        
        QString portMapping = portEdit->text();
        if (!portMapping.isEmpty()) {
            options << "-p" << portMapping;
        }
        
        QString volumeMapping = volumeEdit->text();
        if (!volumeMapping.isEmpty()) {
            options << "-v" << volumeMapping;
        }
        
        QString command = commandEdit->text();
        if (!command.isEmpty()) {
            options << command;
        }
        
        createContainer(imageName, containerName, options);
    }
}

void ContainerManager::showCreateDistroboxDialog()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Create Distrobox Container");
    dialog.setModal(true);
    dialog.resize(400, 300);
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    
    // Container name
    QHBoxLayout *nameLayout = new QHBoxLayout();
    nameLayout->addWidget(new QLabel("Name:"));
    QLineEdit *nameEdit = new QLineEdit();
    nameLayout->addWidget(nameEdit);
    layout->addLayout(nameLayout);
    
    // Image selection
    QHBoxLayout *imageLayout = new QHBoxLayout();
    imageLayout->addWidget(new QLabel("Image:"));
    QComboBox *imageCombo = new QComboBox();
    
    // Add popular distrobox images
    imageCombo->addItem("fedora:latest");
    imageCombo->addItem("ubuntu:latest");
    imageCombo->addItem("debian:latest");
    imageCombo->addItem("archlinux:latest");
    imageCombo->addItem("opensuse/leap:latest");
    imageCombo->addItem("centos:latest");
    imageCombo->setEditable(true);
    imageLayout->addWidget(imageCombo);
    layout->addLayout(imageLayout);
    
    // Options
    QGroupBox *optionsGroup = new QGroupBox("Options");
    QVBoxLayout *optionsLayout = new QVBoxLayout(optionsGroup);
    
    QCheckBox *homeCheck = new QCheckBox("Mount home directory");
    homeCheck->setChecked(true);
    optionsLayout->addWidget(homeCheck);
    
    QCheckBox *rootCheck = new QCheckBox("Run as root");
    optionsLayout->addWidget(rootCheck);
    
    QCheckBox *initCheck = new QCheckBox("Use init system");
    optionsLayout->addWidget(initCheck);
    
    layout->addWidget(optionsGroup);
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *createButton = new QPushButton("Create");
    QPushButton *cancelButton = new QPushButton("Cancel");
    buttonLayout->addStretch();
    buttonLayout->addWidget(createButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);
    
    connect(createButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        QString name = nameEdit->text();
        QString image = imageCombo->currentText();
        
        QStringList options;
        if (homeCheck->isChecked()) options << "--home";
        if (rootCheck->isChecked()) options << "--root";
        if (initCheck->isChecked()) options << "--init";
        
        createDistroboxContainer(name, image, options);
    }
}

void ContainerManager::createContainer(const QString &imageName, const QString &containerName, const QStringList &options)
{
    if (m_privilegedExecutor) {
        QString taskId = "create_container_" + containerName;
        QStringList args;
        args << "run" << options << imageName;
        
        showProgress("Creating Container", "Creating container " + containerName);
        m_privilegedExecutor->executeCommand(m_defaultRuntime, args);
    }
}

void ContainerManager::createDistroboxContainer(const QString &name, const QString &image, const QStringList &options)
{
    if (m_privilegedExecutor) {
        QString taskId = "create_distrobox_" + name;
        QStringList args;
        args << "create" << "--name" << name << "--image" << image << options;
        
        showProgress("Creating Distrobox", "Creating distrobox " + name);
        m_privilegedExecutor->executeCommand("distrobox", args);
    }
}

void ContainerManager::showContainerLogs()
{
    QList<QTableWidgetItem*> selectedItems = m_containerTable->selectedItems();
    if (selectedItems.isEmpty()) return;
    
    QString containerId = m_containerTable->item(selectedItems.first()->row(), CONTAINER_TABLE_ID_COLUMN)->text();
    QString containerName = m_containerTable->item(selectedItems.first()->row(), CONTAINER_TABLE_NAME_COLUMN)->text();
    
    QDialog dialog(this);
    dialog.setWindowTitle("Container Logs - " + containerName);
    dialog.setModal(false);
    dialog.resize(800, 600);
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    
    QTextEdit *logTextEdit = new QTextEdit();
    logTextEdit->setReadOnly(true);
    logTextEdit->setFont(QFont("monospace"));
    layout->addWidget(logTextEdit);
    
    QPushButton *refreshButton = new QPushButton("Refresh");
    layout->addWidget(refreshButton);
    
    // Load logs
    auto loadLogs = [=]() {
        QProcess process;
        process.start(m_defaultRuntime, QStringList() << "logs" << containerId);
        if (process.waitForFinished(10000)) {
            logTextEdit->setPlainText(process.readAllStandardOutput());
        }
    };
    
    connect(refreshButton, &QPushButton::clicked, loadLogs);
    loadLogs();
    
    dialog.exec();
}

void ContainerManager::showContainerInspect()
{
    QList<QTableWidgetItem*> selectedItems = m_containerTable->selectedItems();
    if (selectedItems.isEmpty()) return;
    
    QString containerId = m_containerTable->item(selectedItems.first()->row(), CONTAINER_TABLE_ID_COLUMN)->text();
    QString containerName = m_containerTable->item(selectedItems.first()->row(), CONTAINER_TABLE_NAME_COLUMN)->text();
    
    QDialog dialog(this);
    dialog.setWindowTitle("Container Inspect - " + containerName);
    dialog.setModal(false);
    dialog.resize(800, 600);
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    
    QTextEdit *inspectTextEdit = new QTextEdit();
    inspectTextEdit->setReadOnly(true);
    inspectTextEdit->setFont(QFont("monospace"));
    layout->addWidget(inspectTextEdit);
    
    // Load inspect data
    QProcess process;
    process.start(m_defaultRuntime, QStringList() << "inspect" << containerId);
    if (process.waitForFinished(10000)) {
        inspectTextEdit->setPlainText(process.readAllStandardOutput());
    }
    
    dialog.exec();
}

void ContainerManager::showImageInspect()
{
    QList<QTableWidgetItem*> selectedItems = m_imageTable->selectedItems();
    if (selectedItems.isEmpty()) return;
    
    QString imageId = m_imageTable->item(selectedItems.first()->row(), IMAGE_TABLE_ID_COLUMN)->text();
    QString imageName = m_imageTable->item(selectedItems.first()->row(), IMAGE_TABLE_REPOSITORY_COLUMN)->text();
    
    QDialog dialog(this);
    dialog.setWindowTitle("Image Inspect - " + imageName);
    dialog.setModal(false);
    dialog.resize(800, 600);
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    
    QTextEdit *inspectTextEdit = new QTextEdit();
    inspectTextEdit->setReadOnly(true);
    inspectTextEdit->setFont(QFont("monospace"));
    layout->addWidget(inspectTextEdit);
    
    // Load inspect data
    QProcess process;
    process.start(m_defaultRuntime, QStringList() << "inspect" << imageId);
    if (process.waitForFinished(10000)) {
        inspectTextEdit->setPlainText(process.readAllStandardOutput());
    }
    
    dialog.exec();
}

void ContainerManager::pullImage()
{
    bool ok;
    QString imageName = QInputDialog::getText(this, "Pull Image", 
                                             "Image name (e.g., ubuntu:latest):", 
                                             QLineEdit::Normal, "", &ok);
    if (ok && !imageName.isEmpty()) {
        pullImageByName(imageName);
    }
}

void ContainerManager::pullImageByName(const QString &imageName, const QString &tag)
{
    if (m_privilegedExecutor) {
        QString taskId = "pull_image_" + imageName;
        QStringList args;
        args << "pull" << (tag.isEmpty() ? imageName : imageName + ":" + tag);
        
        showProgress("Pulling Image", "Pulling image " + imageName);
        m_privilegedExecutor->executeCommand(m_defaultRuntime, args);
    }
}

void ContainerManager::buildImage()
{
    QString dockerfilePath = QFileDialog::getOpenFileName(this, "Select Dockerfile", 
                                                         QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                         "Dockerfile (Dockerfile);;All Files (*)");
    if (!dockerfilePath.isEmpty()) {
        bool ok;
        QString imageName = QInputDialog::getText(this, "Build Image", 
                                                 "Image name:", QLineEdit::Normal, "", &ok);
        if (ok && !imageName.isEmpty()) {
            buildImageFromDockerfile(dockerfilePath, imageName, "latest");
        }
    }
}

void ContainerManager::buildImageFromDockerfile(const QString &dockerfilePath, const QString &imageName, const QString &tag)
{
    if (m_privilegedExecutor) {
        QString taskId = "build_image_" + imageName;
        QStringList args;
        args << "build" << "-f" << dockerfilePath << "-t" << (imageName + ":" + tag) << QFileInfo(dockerfilePath).absolutePath();
        
        showProgress("Building Image", "Building image " + imageName);
        m_privilegedExecutor->executeCommand(m_defaultRuntime, args);
    }
}

void ContainerManager::tagImage()
{
    QList<QTableWidgetItem*> selectedItems = m_imageTable->selectedItems();
    if (selectedItems.isEmpty()) return;
    
    QString imageId = m_imageTable->item(selectedItems.first()->row(), IMAGE_TABLE_ID_COLUMN)->text();
    
    bool ok;
    QString newTag = QInputDialog::getText(this, "Tag Image", 
                                          "New tag (repository:tag):", 
                                          QLineEdit::Normal, "", &ok);
    if (ok && !newTag.isEmpty()) {
        QStringList parts = newTag.split(":");
        QString repository = parts[0];
        QString tag = parts.size() > 1 ? parts[1] : "latest";
        tagImageWithName(imageId, repository, tag);
    }
}

void ContainerManager::tagImageWithName(const QString &imageId, const QString &repository, const QString &tag)
{
    if (m_privilegedExecutor) {
        QString taskId = "tag_image_" + imageId;
        QStringList args;
        args << "tag" << imageId << (repository + ":" + tag);
        
        showProgress("Tagging Image", "Tagging image " + imageId);
        m_privilegedExecutor->executeCommand(m_defaultRuntime, args);
    }
}

void ContainerManager::pushImage()
{
    QList<QTableWidgetItem*> selectedItems = m_imageTable->selectedItems();
    if (selectedItems.isEmpty()) return;
    
    QString repository = m_imageTable->item(selectedItems.first()->row(), IMAGE_TABLE_REPOSITORY_COLUMN)->text();
    QString tag = m_imageTable->item(selectedItems.first()->row(), IMAGE_TABLE_TAG_COLUMN)->text();
    
    pushImageToRegistry(repository, tag);
}

void ContainerManager::pushImageToRegistry(const QString &imageName, const QString &tag)
{
    if (m_privilegedExecutor) {
        QString taskId = "push_image_" + imageName;
        QStringList args;
        args << "push" << (imageName + ":" + tag);
        
        showProgress("Pushing Image", "Pushing image " + imageName);
        m_privilegedExecutor->executeCommand(m_defaultRuntime, args);
    }
}

void ContainerManager::saveImage()
{
    QList<QTableWidgetItem*> selectedItems = m_imageTable->selectedItems();
    if (selectedItems.isEmpty()) return;
    
    QString imageId = m_imageTable->item(selectedItems.first()->row(), IMAGE_TABLE_ID_COLUMN)->text();
    QString imageName = m_imageTable->item(selectedItems.first()->row(), IMAGE_TABLE_REPOSITORY_COLUMN)->text();
    
    QString filePath = QFileDialog::getSaveFileName(this, "Save Image", 
                                                   QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/" + imageName + ".tar",
                                                   "Tar Archives (*.tar);;All Files (*)");
    if (!filePath.isEmpty()) {
        saveImageToFile(imageId, filePath);
    }
}

void ContainerManager::saveImageToFile(const QString &imageId, const QString &filePath)
{
    if (m_privilegedExecutor) {
        QString taskId = "save_image_" + imageId;
        QStringList args;
        args << "save" << "-o" << filePath << imageId;
        
        showProgress("Saving Image", "Saving image " + imageId);
        m_privilegedExecutor->executeCommand(m_defaultRuntime, args);
    }
}

void ContainerManager::loadImage()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Load Image", 
                                                   QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                   "Tar Archives (*.tar);;All Files (*)");
    if (!filePath.isEmpty()) {
        loadImageFromFile(filePath);
    }
}

void ContainerManager::loadImageFromFile(const QString &filePath)
{
    if (m_privilegedExecutor) {
        QString taskId = "load_image_" + QFileInfo(filePath).baseName();
        QStringList args;
        args << "load" << "-i" << filePath;
        
        showProgress("Loading Image", "Loading image from " + filePath);
        m_privilegedExecutor->executeCommand(m_defaultRuntime, args);
    }
}

void ContainerManager::exportContainer()
{
    QList<QTableWidgetItem*> selectedItems = m_containerTable->selectedItems();
    if (selectedItems.isEmpty()) return;
    
    QString containerId = m_containerTable->item(selectedItems.first()->row(), CONTAINER_TABLE_ID_COLUMN)->text();
    QString containerName = m_containerTable->item(selectedItems.first()->row(), CONTAINER_TABLE_NAME_COLUMN)->text();
    
    QString filePath = QFileDialog::getSaveFileName(this, "Export Container", 
                                                   QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/" + containerName + ".tar",
                                                   "Tar Archives (*.tar);;All Files (*)");
    if (!filePath.isEmpty()) {
        if (m_privilegedExecutor) {
            QString taskId = "export_container_" + containerId;
            QStringList args;
            args << "export" << "-o" << filePath << containerId;
            
            showProgress("Exporting Container", "Exporting container " + containerId);
            m_privilegedExecutor->executeCommand(m_defaultRuntime, args);
        }
    }
}

void ContainerManager::importContainer()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Import Container", 
                                                   QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                   "Tar Archives (*.tar);;All Files (*)");
    if (!filePath.isEmpty()) {
        bool ok;
        QString repository = QInputDialog::getText(this, "Import Container", 
                                                  "Repository name:", QLineEdit::Normal, "", &ok);
        if (ok && !repository.isEmpty()) {
            importImageFromFile(filePath, repository, "latest");
        }
    }
}

void ContainerManager::importImageFromFile(const QString &filePath, const QString &repository, const QString &tag)
{
    if (m_privilegedExecutor) {
        QString taskId = "import_image_" + repository;
        QStringList args;
        args << "import" << filePath << (repository + ":" + tag);
        
        showProgress("Importing Image", "Importing image from " + filePath);
        m_privilegedExecutor->executeCommand(m_defaultRuntime, args);
    }
}

void ContainerManager::pruneContainers()
{
    int ret = QMessageBox::question(this, "Prune Containers", 
                                   "Remove all stopped containers?",
                                   QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        if (m_privilegedExecutor) {
            QString taskId = "prune_containers";
            QStringList args;
            args << "container" << "prune" << "-f";
            
            showProgress("Pruning Containers", "Removing stopped containers");
            m_privilegedExecutor->executeCommand(m_defaultRuntime, args);
        }
    }
}

void ContainerManager::pruneImages()
{
    int ret = QMessageBox::question(this, "Prune Images", 
                                   "Remove all unused images?",
                                   QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        if (m_privilegedExecutor) {
            QString taskId = "prune_images";
            QStringList args;
            args << "image" << "prune" << "-f";
            
            showProgress("Pruning Images", "Removing unused images");
            m_privilegedExecutor->executeCommand(m_defaultRuntime, args);
        }
    }
}

 