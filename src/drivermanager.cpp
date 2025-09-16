#include "drivermanager.h"
#include "systemutils.h"
#include "privilegedexecutor.h"
#include <QApplication>
#include <QStyle>
#include <QHeaderView>
#include <QSplitter>
#include <QDebug>

// HardwareScanner Implementation
HardwareScanner::HardwareScanner(QObject *parent)
    : QThread(parent), m_stopRequested(false)
{
}

void HardwareScanner::stop()
{
    QMutexLocker locker(&m_mutex);
    m_stopRequested = true;
}

void HardwareScanner::setScanType(const QString &scanType)
{
    QMutexLocker locker(&m_mutex);
    m_scanType = scanType;
}

void HardwareScanner::run()
{
    m_stopRequested = false;
    
    try {
        if (m_scanType == "hardware" || m_scanType.isEmpty()) {
            scanPCIDevices();
            if (m_stopRequested) return;
            scanUSBDevices();
        }
        
        if (m_scanType == "modules" || m_scanType.isEmpty()) {
            if (m_stopRequested) return;
            scanKernelModules();
            scanLoadedModules();
        }
        
        if (m_scanType == "firmware" || m_scanType.isEmpty()) {
            if (m_stopRequested) return;
            scanAvailableFirmware();
            scanMissingFirmware();
        }
        
        if (m_scanType == "drivers" || m_scanType.isEmpty()) {
            if (m_stopRequested) return;
            scanDrivers();
            scanGPUDrivers();
            scanNetworkDrivers();
            scanAudioDrivers();
        }
        
        emit scanFinished();
    } catch (const std::exception &e) {
        emit errorOccurred(QString("Scan error: %1").arg(e.what()));
    }
}

void HardwareScanner::scanPCIDevices()
{
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        if (m_stopRequested) {
            process->deleteLater();
            return;
        }
        
        if (exitCode != 0) {
            emit errorOccurred("Failed to run lspci");
            process->deleteLater();
            return;
        }
        
        QString output = process->readAllStandardOutput();
        QStringList lines = output.split('\n');
        QJsonObject currentDevice;
        
        for (const QString &line : lines) {
            if (m_stopRequested) {
                process->deleteLater();
                return;
            }
            
            if (!line.startsWith('\t') && !line.isEmpty()) {
                if (!currentDevice.isEmpty()) {
                    emit hardwareFound(currentDevice);
                }
                currentDevice = parseLspciOutput(line);
            } else if (line.startsWith('\t')) {
                // Parse additional device information
                if (line.contains("Kernel driver in use:")) {
                    QString driver = line.split(":").last().trimmed();
                    currentDevice["driver"] = driver;
                    currentDevice["status"] = "loaded";
                } else if (line.contains("Kernel modules:")) {
                    QString modules = line.split(":").last().trimmed();
                    currentDevice["modules"] = modules;
                }
            }
        }
        
        if (!currentDevice.isEmpty()) {
            emit hardwareFound(currentDevice);
        }
        
        process->deleteLater();
    });
    
    process->start("lspci", QStringList() << "-nnv");
}

void HardwareScanner::scanUSBDevices()
{
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        if (m_stopRequested) {
            process->deleteLater();
            return;
        }
        
        if (exitCode != 0) {
            emit errorOccurred("Failed to run lsusb");
            process->deleteLater();
            return;
        }
        
        QString output = process->readAllStandardOutput();
        QStringList lines = output.split('\n');
        
        for (const QString &line : lines) {
            if (m_stopRequested) {
                process->deleteLater();
                return;
            }
            
            if (line.startsWith("Bus ")) {
                QJsonObject device = parseLsusbOutput(line);
                if (!device.isEmpty()) {
                    emit hardwareFound(device);
                }
            }
        }
        
        process->deleteLater();
    });
    
    process->start("lsusb", QStringList() << "-v");
}

void HardwareScanner::scanKernelModules()
{
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        if (m_stopRequested) {
            process->deleteLater();
            return;
        }
        
        if (exitCode != 0) {
            emit errorOccurred("Failed to scan kernel modules");
            process->deleteLater();
            return;
        }
        
        QString output = process->readAllStandardOutput();
        QStringList modules = output.split('\n');
        
        for (const QString &modulePath : modules) {
            if (m_stopRequested) {
                process->deleteLater();
                return;
            }
            if (modulePath.isEmpty()) continue;
            
            QFileInfo fileInfo(modulePath);
            QString moduleName = fileInfo.baseName();
            
            QJsonObject moduleInfo;
            moduleInfo["name"] = moduleName;
            moduleInfo["path"] = modulePath;
            moduleInfo["type"] = "kernel_module";
            moduleInfo["status"] = "available";
            
            emit moduleFound(moduleInfo);
        }
        
        process->deleteLater();
    });
    
    process->start("find", QStringList() << "/lib/modules" << "-name" << "*.ko");
}

void HardwareScanner::scanLoadedModules()
{
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        if (m_stopRequested) {
            process->deleteLater();
            return;
        }
        
        if (exitCode != 0) {
            emit errorOccurred("Failed to run lsmod");
            process->deleteLater();
            return;
        }
        
        QString output = process->readAllStandardOutput();
        QStringList lines = output.split('\n');
        
        // Skip header line
        for (int i = 1; i < lines.size(); ++i) {
            if (m_stopRequested) {
                process->deleteLater();
                return;
            }
            
            const QString &line = lines[i];
            if (line.isEmpty()) continue;
            
            QJsonObject moduleInfo = parseLsmodOutput(line);
            if (!moduleInfo.isEmpty()) {
                emit moduleFound(moduleInfo);
            }
        }
        
        process->deleteLater();
    });
    
    process->start("lsmod");
}

void HardwareScanner::scanAvailableFirmware()
{
    QStringList firmwareDirs = {"/lib/firmware", "/usr/lib/firmware"};
    scanFirmwareDirectory(firmwareDirs, 0);
}

void HardwareScanner::scanFirmwareDirectory(const QStringList &dirs, int currentIndex)
{
    if (currentIndex >= dirs.size() || m_stopRequested) {
        return;
    }
    
    const QString &dir = dirs[currentIndex];
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process, dirs, currentIndex](int exitCode, QProcess::ExitStatus exitStatus) {
        if (m_stopRequested) {
            process->deleteLater();
            return;
        }
        
        if (exitCode == 0) {
            QString output = process->readAllStandardOutput();
            QStringList files = output.split('\n');
            
            for (const QString &file : files) {
                if (m_stopRequested) {
                    process->deleteLater();
                    return;
                }
                if (file.isEmpty()) continue;
                
                QFileInfo fileInfo(file);
                QJsonObject firmwareInfo;
                firmwareInfo["name"] = fileInfo.fileName();
                firmwareInfo["path"] = file;
                firmwareInfo["size"] = fileInfo.size();
                firmwareInfo["status"] = "installed";
                firmwareInfo["type"] = "firmware";
                
                emit firmwareFound(firmwareInfo);
            }
        }
        
        process->deleteLater();
        
        // Continue with next directory
        scanFirmwareDirectory(dirs, currentIndex + 1);
    });
    
    process->start("find", QStringList() << dir << "-type" << "f");
}

void HardwareScanner::scanMissingFirmware()
{
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        if (m_stopRequested) {
            process->deleteLater();
            return;
        }
        
        if (exitCode != 0) {
            emit errorOccurred("Failed to read dmesg");
            process->deleteLater();
            return;
        }
        
        QString output = process->readAllStandardOutput();
        QStringList lines = output.split('\n');
        
        for (const QString &line : lines) {
            if (m_stopRequested) {
                process->deleteLater();
                return;
            }
            
            if (line.contains("firmware") && (line.contains("failed") || line.contains("missing"))) {
                QRegularExpression re("firmware: (\\S+)");
                QRegularExpressionMatch match = re.match(line);
                if (match.hasMatch()) {
                    QString firmwareName = match.captured(1);
                    
                    QJsonObject firmwareInfo;
                    firmwareInfo["name"] = firmwareName;
                    firmwareInfo["status"] = "missing";
                    firmwareInfo["type"] = "firmware";
                    firmwareInfo["error"] = line.trimmed();
                    
                    emit firmwareFound(firmwareInfo);
                }
            }
        }
        
        process->deleteLater();
    });
    
    process->start("dmesg");
}

void HardwareScanner::scanDrivers()
{
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        if (m_stopRequested || exitCode != 0) {
            process->deleteLater();
            return;
        }
        
        QString output = process->readAllStandardOutput();
        QStringList lines = output.split('\n');
        
        for (int i = 1; i < lines.size(); ++i) {
            if (m_stopRequested) {
                process->deleteLater();
                return;
            }
            
            const QString &line = lines[i];
            if (line.isEmpty()) continue;
            
            QJsonObject driverInfo = parseDriverInfo(line);
            if (!driverInfo.isEmpty()) {
                emit driverFound(driverInfo);
            }
        }
        
        process->deleteLater();
    });
    
    process->start("lsmod");
}

void HardwareScanner::scanGPUDrivers()
{
    // Check for NVIDIA drivers
    QProcess nvidia;
    nvidia.start("nvidia-smi", QStringList() << "--query-gpu=name,driver_version" << "--format=csv,noheader,nounits");
    if (nvidia.waitForFinished(3000)) {
        QString output = nvidia.readAllStandardOutput();
        if (!output.isEmpty()) {
            QStringList lines = output.trimmed().split('\n');
            for (const QString &line : lines) {
                QStringList parts = line.split(", ");
                if (parts.size() >= 2) {
                    QJsonObject driverInfo;
                    driverInfo["name"] = "nvidia";
                    driverInfo["version"] = parts[1];
                    driverInfo["device"] = parts[0];
                    driverInfo["type"] = "gpu_driver";
                    driverInfo["status"] = "loaded";
                    emit driverFound(driverInfo);
                }
            }
        }
    }
    
    // Check for AMD drivers
    QProcess amd;
    amd.start("lspci", QStringList() << "-k");
    if (amd.waitForFinished(3000)) {
        QString output = amd.readAllStandardOutput();
        if (output.contains("amdgpu") || output.contains("radeon")) {
            QJsonObject driverInfo;
            driverInfo["name"] = output.contains("amdgpu") ? "amdgpu" : "radeon";
            driverInfo["type"] = "gpu_driver";
            driverInfo["status"] = "loaded";
            emit driverFound(driverInfo);
        }
    }
}

void HardwareScanner::scanNetworkDrivers()
{
    QProcess process;
    process.start("lspci", QStringList() << "-k");
    if (!process.waitForFinished(5000)) return;
    
    QString output = process.readAllStandardOutput();
    QStringList lines = output.split('\n');
    
    for (int i = 0; i < lines.size(); ++i) {
        if (m_stopRequested) return;
        
        if (lines[i].contains("Network") || lines[i].contains("Ethernet") || lines[i].contains("Wireless")) {
            // Look for driver in the next few lines
            for (int j = i + 1; j < qMin(i + 5, lines.size()); ++j) {
                if (lines[j].contains("Kernel driver in use:")) {
                    QString driver = lines[j].split(":").last().trimmed();
                    
                    QJsonObject driverInfo;
                    driverInfo["name"] = driver;
                    driverInfo["type"] = "network_driver";
                    driverInfo["status"] = "loaded";
                    driverInfo["device"] = lines[i].split(": ").last();
                    
                    emit driverFound(driverInfo);
                    break;
                }
            }
        }
    }
}

void HardwareScanner::scanAudioDrivers()
{
    QProcess process;
    process.start("lspci", QStringList() << "-k");
    if (!process.waitForFinished(5000)) return;
    
    QString output = process.readAllStandardOutput();
    QStringList lines = output.split('\n');
    
    for (int i = 0; i < lines.size(); ++i) {
        if (m_stopRequested) return;
        
        if (lines[i].contains("Audio") || lines[i].contains("sound")) {
            // Look for driver in the next few lines
            for (int j = i + 1; j < qMin(i + 5, lines.size()); ++j) {
                if (lines[j].contains("Kernel driver in use:")) {
                    QString driver = lines[j].split(":").last().trimmed();
                    
                    QJsonObject driverInfo;
                    driverInfo["name"] = driver;
                    driverInfo["type"] = "audio_driver";
                    driverInfo["status"] = "loaded";
                    driverInfo["device"] = lines[i].split(": ").last();
                    
                    emit driverFound(driverInfo);
                    break;
                }
            }
        }
    }
}

QJsonObject HardwareScanner::parseLspciOutput(const QString &output)
{
    QJsonObject device;
    QStringList parts = output.split(" ", Qt::SkipEmptyParts);
    
    if (parts.size() < 2) return device;
    
    device["bus_id"] = parts[0];
    device["type"] = "pci";
    
    QString description = output;
    description.remove(QRegularExpression("^[0-9a-f:.]+\\s+"));
    
    // Extract vendor and device IDs
    QRegularExpression vendorRe("\\[([0-9a-f]{4}):([0-9a-f]{4})\\]");
    QRegularExpressionMatch match = vendorRe.match(description);
    if (match.hasMatch()) {
        device["vendor_id"] = match.captured(1);
        device["device_id"] = match.captured(2);
        description.remove(vendorRe);
    }
    
    device["description"] = description.trimmed();
    device["status"] = "detected";
    
    return device;
}

QJsonObject HardwareScanner::parseLsusbOutput(const QString &output)
{
    QJsonObject device;
    QStringList parts = output.split(" ", Qt::SkipEmptyParts);
    
    if (parts.size() < 6) return device;
    
    device["bus"] = parts[1];
    device["device"] = parts[3].chopped(1); // Remove ':'
    device["type"] = "usb";
    
    // Extract vendor and product IDs
    QString idPart = parts[5];
    if (idPart.contains(":")) {
        QStringList ids = idPart.split(":");
        device["vendor_id"] = ids[0];
        device["product_id"] = ids[1];
    }
    
    QString description = output;
    description.remove(QRegularExpression("^Bus \\d+ Device \\d+: ID [0-9a-f:]+\\s+"));
    device["description"] = description.trimmed();
    device["status"] = "detected";
    
    return device;
}

QJsonObject HardwareScanner::parseModinfoOutput(const QString &output)
{
    QJsonObject moduleInfo;
    QStringList lines = output.split('\n');
    
    for (const QString &line : lines) {
        if (line.startsWith("filename:")) {
            moduleInfo["filename"] = line.split(":").last().trimmed();
        } else if (line.startsWith("description:")) {
            moduleInfo["description"] = line.split(":").last().trimmed();
        } else if (line.startsWith("version:")) {
            moduleInfo["version"] = line.split(":").last().trimmed();
        } else if (line.startsWith("license:")) {
            moduleInfo["license"] = line.split(":").last().trimmed();
        } else if (line.startsWith("depends:")) {
            moduleInfo["depends"] = line.split(":").last().trimmed();
        }
    }
    
    return moduleInfo;
}

QJsonObject HardwareScanner::parseLsmodOutput(const QString &output)
{
    QJsonObject moduleInfo;
    QStringList parts = output.split(QRegularExpression("\\s+"));
    
    if (parts.size() >= 3) {
        moduleInfo["name"] = parts[0];
        moduleInfo["size"] = parts[1];
        moduleInfo["used_count"] = parts[2];
        if (parts.size() > 3) {
            moduleInfo["used_by"] = parts[3];
        }
        moduleInfo["status"] = "loaded";
        moduleInfo["type"] = "kernel_module";
    }
    
    return moduleInfo;
}

QJsonObject HardwareScanner::parseFirmwareInfo(const QString &output)
{
    // Implementation for parsing firmware information
    QJsonObject firmwareInfo;
    // Add parsing logic based on specific firmware tools output
    return firmwareInfo;
}

QJsonObject HardwareScanner::parseDriverInfo(const QString &line)
{
    QJsonObject driverInfo;
    QStringList parts = line.split(QRegularExpression("\\s+"));
    
    if (parts.size() >= 3) {
        driverInfo["name"] = parts[0];
        driverInfo["size"] = parts[1];
        driverInfo["used_by"] = parts[2];
        driverInfo["status"] = "loaded";
        driverInfo["type"] = "kernel_driver";
    }
    
    return driverInfo;
}

// DriverManager Implementation
DriverManager::DriverManager(QWidget *parent)
    : QWidget(parent)
    , m_systemUtils(nullptr)
    , m_privilegedExecutor(nullptr)
    , m_mainLayout(nullptr)
    , m_tabWidget(nullptr)
    , m_hardwareScanner(nullptr)
    , m_refreshTimer(new QTimer(this))
    , m_autoRefresh(true)
    , m_refreshInterval(30000) // 30 seconds
    , m_isScanning(false)
{
    setupUI();
    setupContextMenus();
    
    // Initialize hardware scanner
    m_hardwareScanner = new HardwareScanner(this);
    connect(m_hardwareScanner, &HardwareScanner::hardwareFound,
            this, &DriverManager::onHardwareFound);
    connect(m_hardwareScanner, &HardwareScanner::driverFound,
            this, &DriverManager::onDriverFound);
    connect(m_hardwareScanner, &HardwareScanner::moduleFound,
            this, &DriverManager::onModuleFound);
    connect(m_hardwareScanner, &HardwareScanner::firmwareFound,
            this, &DriverManager::onFirmwareFound);
    connect(m_hardwareScanner, &HardwareScanner::scanFinished,
            this, &DriverManager::onScanFinished);
    connect(m_hardwareScanner, &HardwareScanner::errorOccurred,
            this, &DriverManager::onScanError);
    
    // Setup refresh timer (but don't start it automatically)
    connect(m_refreshTimer, &QTimer::timeout, this, &DriverManager::onRefreshTimer);
    m_autoRefresh = false; // Disable auto-refresh to prevent startup prompts
    
    // Initialize system info with defaults (will be updated on-demand)
    m_currentKernel = "Unknown";
    m_currentArchitecture = "Unknown";
    
    updateTheme();
}

DriverManager::~DriverManager()
{
    if (m_hardwareScanner && m_hardwareScanner->isRunning()) {
        m_hardwareScanner->stop();
        m_hardwareScanner->wait(3000);
    }
}

void DriverManager::setSystemUtils(SystemUtils *utils)
{
    m_systemUtils = utils;
}

void DriverManager::setPrivilegedExecutor(PrivilegedExecutor *executor)
{
    m_privilegedExecutor = executor;
    if (m_privilegedExecutor) {
        connect(m_privilegedExecutor, &PrivilegedExecutor::taskProgress,
                this, &DriverManager::onTaskProgress);
        connect(m_privilegedExecutor, &PrivilegedExecutor::taskFinished,
                this, &DriverManager::onTaskFinished);
        connect(m_privilegedExecutor, &PrivilegedExecutor::taskError,
                this, &DriverManager::onTaskError);
        connect(m_privilegedExecutor, &PrivilegedExecutor::taskStarted,
                this, &DriverManager::onTaskStarted);
    }
}

void DriverManager::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    
    // Create toolbar
    setupToolbar();
    
    // Create main content area
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    
    // Create tab widget
    m_tabWidget = new QTabWidget(this);
    m_tabWidget->setTabPosition(QTabWidget::North);
    m_tabWidget->setMovable(true);
    m_tabWidget->setUsesScrollButtons(true);
    
    // Setup tabs
    setupHardwareTab();
    setupDriverTab();
    setupModuleTab();
    setupFirmwareTab();
    setupKernelTab();
    
    // Create info panel
    setupInfoPanel();
    
    // Add to splitter
    splitter->addWidget(m_tabWidget);
    splitter->addWidget(m_infoPanel);
    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 1);
    
    m_mainLayout->addWidget(splitter);
    
    // Create status bar
    QHBoxLayout *statusLayout = new QHBoxLayout();
    m_statusLabel = new QLabel("Ready", this);
    m_progressBar = new QProgressBar(this);
    m_progressBar->setVisible(false);
    m_progressLabel = new QLabel(this);
    m_progressLabel->setVisible(false);
    
    statusLayout->addWidget(m_statusLabel);
    statusLayout->addStretch();
    statusLayout->addWidget(m_progressLabel);
    statusLayout->addWidget(m_progressBar);
    
    m_mainLayout->addLayout(statusLayout);
    
    // Create output area
    m_outputTextEdit = new QTextEdit(this);
    m_outputTextEdit->setMaximumHeight(100);
    m_outputTextEdit->setVisible(false);
    m_mainLayout->addWidget(m_outputTextEdit);
}

void DriverManager::setupToolbar()
{
    m_toolbarLayout = new QHBoxLayout();
    
    // Search functionality
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText("Search hardware, drivers, modules...");
    m_searchEdit->setMaximumWidth(300);
    
    m_searchButton = new QPushButton("Search", this);
    m_clearSearchButton = new QPushButton("Clear", this);
    
    m_filterComboBox = new QComboBox(this);
    m_filterComboBox->addItems({"All", "Hardware", "Drivers", "Modules", "Firmware"});
    
    // Action buttons
    QPushButton *refreshAllButton = new QPushButton("Refresh All", this);
    QPushButton *detectHardwareButton = new QPushButton("Detect Hardware", this);
    QPushButton *installMissingButton = new QPushButton("Install Missing", this);
    QPushButton *updateAllButton = new QPushButton("Update All", this);
    
    // Connect signals
    connect(m_searchButton, &QPushButton::clicked, this, &DriverManager::searchHardware);
    connect(m_clearSearchButton, &QPushButton::clicked, [this]() {
        m_searchEdit->clear();
        searchHardware();
    });
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &DriverManager::searchHardware);
    connect(refreshAllButton, &QPushButton::clicked, [this]() {
        refreshHardware();
        refreshDrivers();
        refreshModules();
        refreshFirmware();
    });
    connect(detectHardwareButton, &QPushButton::clicked, [this]() {
        m_hardwareScanner->setScanType("hardware");
        m_hardwareScanner->start();
    });
    connect(installMissingButton, &QPushButton::clicked, this, &DriverManager::installMissingDrivers);
    connect(updateAllButton, &QPushButton::clicked, this, &DriverManager::updateAllDrivers);
    
    // Add to layout
    m_toolbarLayout->addWidget(new QLabel("Search:"));
    m_toolbarLayout->addWidget(m_searchEdit);
    m_toolbarLayout->addWidget(m_searchButton);
    m_toolbarLayout->addWidget(m_clearSearchButton);
    m_toolbarLayout->addWidget(new QLabel("Filter:"));
    m_toolbarLayout->addWidget(m_filterComboBox);
    m_toolbarLayout->addStretch();
    m_toolbarLayout->addWidget(refreshAllButton);
    m_toolbarLayout->addWidget(detectHardwareButton);
    m_toolbarLayout->addWidget(installMissingButton);
    m_toolbarLayout->addWidget(updateAllButton);
    
    m_mainLayout->addLayout(m_toolbarLayout);
}

void DriverManager::setupHardwareTab()
{
    m_hardwareTab = new QWidget();
    m_hardwareLayout = new QVBoxLayout(m_hardwareTab);
    
    // Create table
    m_hardwareTable = new QTableWidget(0, 6, m_hardwareTab);
    m_hardwareTable->setHorizontalHeaderLabels({
        "Device Name", "Type", "Vendor", "Model", "Driver", "Status"
    });
    
    // Configure table
    m_hardwareTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_hardwareTable->setAlternatingRowColors(true);
    m_hardwareTable->setSortingEnabled(true);
    m_hardwareTable->setContextMenuPolicy(Qt::CustomContextMenu);
    
    // Set column widths
    m_hardwareTable->horizontalHeader()->setStretchLastSection(true);
    m_hardwareTable->horizontalHeader()->resizeSection(0, 200);
    m_hardwareTable->horizontalHeader()->resizeSection(1, 100);
    m_hardwareTable->horizontalHeader()->resizeSection(2, 150);
    m_hardwareTable->horizontalHeader()->resizeSection(3, 150);
    m_hardwareTable->horizontalHeader()->resizeSection(4, 120);
    
    // Connect signals
    connect(m_hardwareTable, &QTableWidget::customContextMenuRequested,
            this, &DriverManager::onHardwareTableContextMenu);
    connect(m_hardwareTable, &QTableWidget::itemSelectionChanged,
            this, &DriverManager::onHardwareSelectionChanged);
    
    m_hardwareLayout->addWidget(m_hardwareTable);
    
    // Create button layout
    m_hardwareButtonLayout = new QHBoxLayout();
    
    m_detectHardwareButton = new QPushButton("Detect Hardware", m_hardwareTab);
    m_refreshHardwareButton = new QPushButton("Refresh", m_hardwareTab);
    m_hardwareDetailsButton = new QPushButton("Details", m_hardwareTab);
    m_installDriverButton = new QPushButton("Install Driver", m_hardwareTab);
    m_findDriverButton = new QPushButton("Find Driver", m_hardwareTab);
    m_deviceManagerButton = new QPushButton("Device Manager", m_hardwareTab);
    m_hardwareReportButton = new QPushButton("Generate Report", m_hardwareTab);
    
    // Connect signals
    connect(m_detectHardwareButton, &QPushButton::clicked, this, &DriverManager::detectHardware);
    connect(m_refreshHardwareButton, &QPushButton::clicked, this, &DriverManager::refreshHardware);
    connect(m_hardwareDetailsButton, &QPushButton::clicked, this, &DriverManager::showHardwareDetails);
    connect(m_installDriverButton, &QPushButton::clicked, this, &DriverManager::installDriver);
    connect(m_findDriverButton, &QPushButton::clicked, this, &DriverManager::showDriverInstaller);
    connect(m_deviceManagerButton, &QPushButton::clicked, this, &DriverManager::showSystemInfo);
    connect(m_hardwareReportButton, &QPushButton::clicked, this, &DriverManager::generateHardwareReport);
    
    m_hardwareButtonLayout->addWidget(m_detectHardwareButton);
    m_hardwareButtonLayout->addWidget(m_refreshHardwareButton);
    m_hardwareButtonLayout->addWidget(m_hardwareDetailsButton);
    m_hardwareButtonLayout->addWidget(m_installDriverButton);
    m_hardwareButtonLayout->addWidget(m_findDriverButton);
    m_hardwareButtonLayout->addWidget(m_deviceManagerButton);
    m_hardwareButtonLayout->addWidget(m_hardwareReportButton);
    m_hardwareButtonLayout->addStretch();
    
    m_hardwareLayout->addLayout(m_hardwareButtonLayout);
    
    m_tabWidget->addTab(m_hardwareTab, "Hardware");
}

void DriverManager::setupDriverTab()
{
    // Create the driver tab widget
    m_driverTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(m_driverTab);
    
    // Firmware Updates Section (fwupd)
    QGroupBox *firmwareGroup = new QGroupBox("Firmware Updates (fwupd)");
    QVBoxLayout *firmwareLayout = new QVBoxLayout(firmwareGroup);
    
    // Status and buttons
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    
    QPushButton *checkUpdatesButton = new QPushButton("Check for Updates");
    connect(checkUpdatesButton, &QPushButton::clicked, this, &DriverManager::checkFirmwareUpdates);
    buttonsLayout->addWidget(checkUpdatesButton);
    
    QPushButton *refreshDevicesButton = new QPushButton("Refresh Devices");
    connect(refreshDevicesButton, &QPushButton::clicked, this, &DriverManager::refreshFirmwareDevices);
    buttonsLayout->addWidget(refreshDevicesButton);
    
    QPushButton *applyUpdatesButton = new QPushButton("Apply Updates");
    connect(applyUpdatesButton, &QPushButton::clicked, this, &DriverManager::applyFirmwareUpdates);
    buttonsLayout->addWidget(applyUpdatesButton);
    
    firmwareLayout->addLayout(buttonsLayout);
    
    // Firmware devices table
    m_firmwareTable = new QTableWidget();
    m_firmwareTable->setColumnCount(4);
    m_firmwareTable->setHorizontalHeaderLabels({"Device", "Current Version", "Available Version", "Status"});
    m_firmwareTable->horizontalHeader()->setStretchLastSection(true);
    m_firmwareTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_firmwareTable->setAlternatingRowColors(true);
    
    firmwareLayout->addWidget(m_firmwareTable);
    
    layout->addWidget(firmwareGroup);
    
    // Status and Progress
    QLabel *statusLabel = new QLabel("Ready");
    statusLabel->setStyleSheet("color: #666; font-size: 10px;");
    layout->addWidget(statusLabel);
    
    QProgressBar *progressBar = new QProgressBar();
    progressBar->setVisible(false);
    layout->addWidget(progressBar);
    
    layout->addStretch();
    
    // Add the tab to the tab widget
    m_tabWidget->addTab(m_driverTab, "Drivers");
    
    // Initial device refresh
    QTimer::singleShot(1000, this, &DriverManager::refreshFirmwareDevices);
}

void DriverManager::setupModuleTab()
{
    m_moduleTab = new QWidget();
    m_moduleLayout = new QVBoxLayout(m_moduleTab);
    
    // Create table
    m_moduleTable = new QTableWidget(0, 5, m_moduleTab);
    m_moduleTable->setHorizontalHeaderLabels({
        "Module Name", "Size", "Used By", "Dependencies", "Status"
    });
    
    // Configure table
    m_moduleTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_moduleTable->setAlternatingRowColors(true);
    m_moduleTable->setSortingEnabled(true);
    m_moduleTable->setContextMenuPolicy(Qt::CustomContextMenu);
    
    // Set column widths
    m_moduleTable->horizontalHeader()->setStretchLastSection(true);
    m_moduleTable->horizontalHeader()->resizeSection(0, 150);
    m_moduleTable->horizontalHeader()->resizeSection(1, 80);
    m_moduleTable->horizontalHeader()->resizeSection(2, 80);
    m_moduleTable->horizontalHeader()->resizeSection(3, 200);
    
    // Connect signals
    connect(m_moduleTable, &QTableWidget::customContextMenuRequested,
            this, &DriverManager::onModuleTableContextMenu);
    connect(m_moduleTable, &QTableWidget::itemSelectionChanged,
            this, &DriverManager::onModuleSelectionChanged);
    
    m_moduleLayout->addWidget(m_moduleTable);
    
    // Create button layout
    m_moduleButtonLayout = new QHBoxLayout();
    
    m_loadModuleButton = new QPushButton("Load", m_moduleTab);
    m_unloadModuleButton = new QPushButton("Unload", m_moduleTab);
    m_reloadModuleButton = new QPushButton("Reload", m_moduleTab);
    m_blacklistModuleButton = new QPushButton("Blacklist", m_moduleTab);
    m_moduleDetailsButton = new QPushButton("Details", m_moduleTab);
    m_moduleManagerButton = new QPushButton("Module Manager", m_moduleTab);
    m_refreshModulesButton = new QPushButton("Refresh", m_moduleTab);
    
    // Connect signals
    connect(m_loadModuleButton, &QPushButton::clicked, this, &DriverManager::loadModule);
    connect(m_unloadModuleButton, &QPushButton::clicked, this, &DriverManager::unloadModule);
    connect(m_reloadModuleButton, &QPushButton::clicked, this, &DriverManager::reloadModule);
    connect(m_blacklistModuleButton, &QPushButton::clicked, this, &DriverManager::blacklistModule);
    connect(m_moduleDetailsButton, &QPushButton::clicked, this, &DriverManager::showModuleDetails);
    connect(m_moduleManagerButton, &QPushButton::clicked, this, &DriverManager::showModuleManager);
    connect(m_refreshModulesButton, &QPushButton::clicked, this, &DriverManager::refreshModules);
    
    m_moduleButtonLayout->addWidget(m_loadModuleButton);
    m_moduleButtonLayout->addWidget(m_unloadModuleButton);
    m_moduleButtonLayout->addWidget(m_reloadModuleButton);
    m_moduleButtonLayout->addWidget(m_blacklistModuleButton);
    m_moduleButtonLayout->addWidget(m_moduleDetailsButton);
    m_moduleButtonLayout->addWidget(m_moduleManagerButton);
    m_moduleButtonLayout->addWidget(m_refreshModulesButton);
    m_moduleButtonLayout->addStretch();
    
    m_moduleLayout->addLayout(m_moduleButtonLayout);
    
    m_tabWidget->addTab(m_moduleTab, "Modules");
}

void DriverManager::setupFirmwareTab()
{
    m_firmwareTab = new QWidget();
    m_firmwareLayout = new QVBoxLayout(m_firmwareTab);
    
    // Create table
    m_firmwareTable = new QTableWidget(0, 5, m_firmwareTab);
    m_firmwareTable->setHorizontalHeaderLabels({
        "Firmware Name", "Version", "Size", "Device", "Status"
    });
    
    // Configure table
    m_firmwareTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_firmwareTable->setAlternatingRowColors(true);
    m_firmwareTable->setSortingEnabled(true);
    m_firmwareTable->setContextMenuPolicy(Qt::CustomContextMenu);
    
    // Set column widths
    m_firmwareTable->horizontalHeader()->setStretchLastSection(true);
    m_firmwareTable->horizontalHeader()->resizeSection(0, 200);
    m_firmwareTable->horizontalHeader()->resizeSection(1, 100);
    m_firmwareTable->horizontalHeader()->resizeSection(2, 80);
    m_firmwareTable->horizontalHeader()->resizeSection(3, 150);
    
    // Connect signals
    connect(m_firmwareTable, &QTableWidget::customContextMenuRequested,
            this, &DriverManager::onFirmwareTableContextMenu);
    connect(m_firmwareTable, &QTableWidget::itemSelectionChanged,
            this, &DriverManager::onFirmwareSelectionChanged);
    
    m_firmwareLayout->addWidget(m_firmwareTable);
    
    // Create button layout
    m_firmwareButtonLayout = new QHBoxLayout();
    
    m_installFirmwareButton = new QPushButton("Install", m_firmwareTab);
    m_removeFirmwareButton = new QPushButton("Remove", m_firmwareTab);
    m_updateFirmwareButton = new QPushButton("Update", m_firmwareTab);
    m_downloadFirmwareButton = new QPushButton("Download", m_firmwareTab);
    m_firmwareDetailsButton = new QPushButton("Details", m_firmwareTab);
    m_scanMissingFirmwareButton = new QPushButton("Scan Missing", m_firmwareTab);
    m_refreshFirmwareButton = new QPushButton("Refresh", m_firmwareTab);
    
    // Connect signals
    connect(m_installFirmwareButton, &QPushButton::clicked, this, &DriverManager::installFirmware);
    connect(m_removeFirmwareButton, &QPushButton::clicked, this, &DriverManager::removeFirmware);
    connect(m_updateFirmwareButton, &QPushButton::clicked, this, &DriverManager::updateFirmware);
    connect(m_downloadFirmwareButton, &QPushButton::clicked, this, &DriverManager::downloadFirmware);
    connect(m_firmwareDetailsButton, &QPushButton::clicked, this, &DriverManager::showFirmwareDetails);
    connect(m_scanMissingFirmwareButton, &QPushButton::clicked, this, &DriverManager::detectMissingFirmware);
    connect(m_refreshFirmwareButton, &QPushButton::clicked, this, &DriverManager::refreshFirmware);
    
    m_firmwareButtonLayout->addWidget(m_installFirmwareButton);
    m_firmwareButtonLayout->addWidget(m_removeFirmwareButton);
    m_firmwareButtonLayout->addWidget(m_updateFirmwareButton);
    m_firmwareButtonLayout->addWidget(m_downloadFirmwareButton);
    m_firmwareButtonLayout->addWidget(m_firmwareDetailsButton);
    m_firmwareButtonLayout->addWidget(m_scanMissingFirmwareButton);
    m_firmwareButtonLayout->addWidget(m_refreshFirmwareButton);
    m_firmwareButtonLayout->addStretch();
    
    m_firmwareLayout->addLayout(m_firmwareButtonLayout);
    
    m_tabWidget->addTab(m_firmwareTab, "Firmware");
}

void DriverManager::setupKernelTab()
{
    m_kernelTab = new QWidget();
    m_kernelLayout = new QVBoxLayout(m_kernelTab);
    
    // Create table
    m_kernelTable = new QTableWidget(0, 5, m_kernelTab);
    m_kernelTable->setHorizontalHeaderLabels({
        "Kernel Version", "Type", "Status", "Size", "Date"
    });
    
    // Configure table
    m_kernelTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_kernelTable->setAlternatingRowColors(true);
    m_kernelTable->setSortingEnabled(true);
    m_kernelTable->setContextMenuPolicy(Qt::CustomContextMenu);
    
    // Set column widths
    m_kernelTable->horizontalHeader()->setStretchLastSection(true);
    m_kernelTable->horizontalHeader()->resizeSection(0, 200);
    m_kernelTable->horizontalHeader()->resizeSection(1, 100);
    m_kernelTable->horizontalHeader()->resizeSection(2, 100);
    m_kernelTable->horizontalHeader()->resizeSection(3, 100);
    
    m_kernelLayout->addWidget(m_kernelTable);
    
    // Create button layout
    m_kernelButtonLayout = new QHBoxLayout();
    
    m_installKernelButton = new QPushButton("Install", m_kernelTab);
    m_removeKernelButton = new QPushButton("Remove", m_kernelTab);
    m_setDefaultKernelButton = new QPushButton("Set Default", m_kernelTab);
    m_rebuildInitramfsButton = new QPushButton("Rebuild Initramfs", m_kernelTab);
    m_kernelHeadersButton = new QPushButton("Install Headers", m_kernelTab);
    m_kernelConfigButton = new QPushButton("Configuration", m_kernelTab);
    m_refreshKernelButton = new QPushButton("Refresh", m_kernelTab);
    
    // Connect signals
    connect(m_installKernelButton, &QPushButton::clicked, this, &DriverManager::installKernel);
    connect(m_removeKernelButton, &QPushButton::clicked, this, &DriverManager::removeKernel);
    connect(m_setDefaultKernelButton, &QPushButton::clicked, this, &DriverManager::setDefaultKernel);
    connect(m_rebuildInitramfsButton, &QPushButton::clicked, this, &DriverManager::rebuildInitramfs);
    connect(m_kernelHeadersButton, &QPushButton::clicked, this, &DriverManager::installKernelHeaders);
    connect(m_kernelConfigButton, &QPushButton::clicked, this, &DriverManager::getKernelConfiguration);
    connect(m_refreshKernelButton, &QPushButton::clicked, [this]() { updateKernelInfo(); });
    
    m_kernelButtonLayout->addWidget(m_installKernelButton);
    m_kernelButtonLayout->addWidget(m_removeKernelButton);
    m_kernelButtonLayout->addWidget(m_setDefaultKernelButton);
    m_kernelButtonLayout->addWidget(m_rebuildInitramfsButton);
    m_kernelButtonLayout->addWidget(m_kernelHeadersButton);
    m_kernelButtonLayout->addWidget(m_kernelConfigButton);
    m_kernelButtonLayout->addWidget(m_refreshKernelButton);
    m_kernelButtonLayout->addStretch();
    
    m_kernelLayout->addLayout(m_kernelButtonLayout);
    
    m_tabWidget->addTab(m_kernelTab, "Kernel");
}

void DriverManager::setupInfoPanel()
{
    m_infoPanel = new QGroupBox("System Information", this);
    m_infoPanelLayout = new QVBoxLayout(m_infoPanel);
    
    m_systemInfoLabel = new QLabel("System: Loading...", m_infoPanel);
    m_kernelInfoLabel = new QLabel("Kernel: Loading...", m_infoPanel);
    m_architectureInfoLabel = new QLabel("Architecture: Loading...", m_infoPanel);
    m_driverCountLabel = new QLabel("Drivers: 0", m_infoPanel);
    m_moduleCountLabel = new QLabel("Modules: 0", m_infoPanel);
    m_firmwareCountLabel = new QLabel("Firmware: 0", m_infoPanel);
    
    m_infoPanelLayout->addWidget(m_systemInfoLabel);
    m_infoPanelLayout->addWidget(m_kernelInfoLabel);
    m_infoPanelLayout->addWidget(m_architectureInfoLabel);
    m_infoPanelLayout->addWidget(m_driverCountLabel);
    m_infoPanelLayout->addWidget(m_moduleCountLabel);
    m_infoPanelLayout->addWidget(m_firmwareCountLabel);
    m_infoPanelLayout->addStretch();
    
    updateInfoPanel();
}

void DriverManager::setupContextMenus()
{
    // Hardware context menu
    m_hardwareContextMenu = new QMenu(this);
    m_hardwareContextMenu->addAction("Show Details", this, &DriverManager::showHardwareDetails);
    m_hardwareContextMenu->addAction("Install Driver", this, &DriverManager::installDriver);
    m_hardwareContextMenu->addAction("Find Driver", this, &DriverManager::showDriverInstaller);
    m_hardwareContextMenu->addSeparator();
    m_hardwareContextMenu->addAction("Enable Device", [this]() { showInfo("Enable Device", "Device enable functionality"); });
    m_hardwareContextMenu->addAction("Disable Device", [this]() { showInfo("Disable Device", "Device disable functionality"); });
    m_hardwareContextMenu->addAction("Reset Device", [this]() { showInfo("Reset Device", "Device reset functionality"); });
    
    // Driver context menu
    m_driverContextMenu = new QMenu(this);
    m_driverContextMenu->addAction("Show Details", this, &DriverManager::showDriverDetails);
    m_driverContextMenu->addAction("Remove Driver", this, &DriverManager::removeDriver);
    m_driverContextMenu->addSeparator();
    m_driverContextMenu->addAction("Enable Driver", this, &DriverManager::enableDriver);
    m_driverContextMenu->addAction("Disable Driver", this, &DriverManager::disableDriver);
    m_driverContextMenu->addAction("Blacklist Driver", this, &DriverManager::blacklistDriver);
    
    // Module context menu
    m_moduleContextMenu = new QMenu(this);
    m_moduleContextMenu->addAction("Show Details", this, &DriverManager::showModuleDetails);
    m_moduleContextMenu->addAction("Load Module", this, &DriverManager::loadModule);
    m_moduleContextMenu->addAction("Unload Module", this, &DriverManager::unloadModule);
    m_moduleContextMenu->addAction("Reload Module", this, &DriverManager::reloadModule);
    m_moduleContextMenu->addSeparator();
    m_moduleContextMenu->addAction("Blacklist Module", this, &DriverManager::blacklistModule);
    m_moduleContextMenu->addAction("Add Parameters", this, &DriverManager::addModuleParameter);
    
    // Firmware context menu
    m_firmwareContextMenu = new QMenu(this);
    m_firmwareContextMenu->addAction("Show Details", this, &DriverManager::showFirmwareDetails);
    m_firmwareContextMenu->addAction("Install Firmware", this, &DriverManager::installFirmware);
    m_firmwareContextMenu->addAction("Remove Firmware", this, &DriverManager::removeFirmware);
    m_firmwareContextMenu->addAction("Update Firmware", this, &DriverManager::updateFirmware);
    m_firmwareContextMenu->addSeparator();
    m_firmwareContextMenu->addAction("Download Firmware", this, &DriverManager::downloadFirmware);
    m_firmwareContextMenu->addAction("Backup Firmware", [this]() { showInfo("Backup Firmware", "Firmware backup functionality"); });
}

// Event handlers and data management methods

void DriverManager::refreshHardware()
{
    m_hardwareScanner->setScanType("hardware");
    if (!m_hardwareScanner->isRunning()) {
        m_isScanning = true;
        m_statusLabel->setText("Scanning hardware...");
        m_hardwareScanner->start();
    }
}

void DriverManager::refreshDrivers()
{
    m_hardwareScanner->setScanType("drivers");
    if (!m_hardwareScanner->isRunning()) {
        m_isScanning = true;
        m_statusLabel->setText("Scanning drivers...");
        m_hardwareScanner->start();
    }
}

void DriverManager::refreshModules()
{
    m_hardwareScanner->setScanType("modules");
    if (!m_hardwareScanner->isRunning()) {
        m_isScanning = true;
        m_statusLabel->setText("Scanning modules...");
        m_hardwareScanner->start();
    }
}

void DriverManager::refreshFirmware()
{
    m_hardwareScanner->setScanType("firmware");
    if (!m_hardwareScanner->isRunning()) {
        m_isScanning = true;
        m_statusLabel->setText("Scanning firmware...");
        m_hardwareScanner->start();
    }
}

void DriverManager::scanMissingDrivers()
{
    if (m_privilegedExecutor) {
        QString taskId = "scan_missing_drivers";
        QStringList commands = {
            "lspci -k | grep -A3 -B1 'Kernel driver'",
            "dmesg | grep -i 'firmware.*failed'",
            "modprobe -D nonexistent 2>&1 | grep -i 'not found' || true"
        };
        
        m_privilegedExecutor->executeCommand("sh", QStringList() << "-c" << commands.join(" && "));
    }
}

void DriverManager::onHardwareFound(const QJsonObject &hardwareInfo)
{
    QMutexLocker locker(&m_dataMutex);
    m_hardware.append(hardwareInfo);
    
    // Update table in main thread
    QMetaObject::invokeMethod(this, "updateHardwareTable", Qt::QueuedConnection);
}

void DriverManager::onDriverFound(const QJsonObject &driverInfo)
{
    QMutexLocker locker(&m_dataMutex);
    m_drivers.append(driverInfo);
    
    // Update table in main thread
    QMetaObject::invokeMethod(this, "updateDriverTable", Qt::QueuedConnection);
}

void DriverManager::onModuleFound(const QJsonObject &moduleInfo)
{
    QMutexLocker locker(&m_dataMutex);
    m_modules.append(moduleInfo);
    
    // Update table in main thread
    QMetaObject::invokeMethod(this, "updateModuleTable", Qt::QueuedConnection);
}

void DriverManager::onFirmwareFound(const QJsonObject &firmwareInfo)
{
    QMutexLocker locker(&m_dataMutex);
    m_firmware.append(firmwareInfo);
    
    // Update table in main thread
    QMetaObject::invokeMethod(this, "updateFirmwareTable", Qt::QueuedConnection);
}

void DriverManager::onScanFinished()
{
    m_isScanning = false;
    m_statusLabel->setText("Ready");
    updateInfoPanel();
}

void DriverManager::onScanError(const QString &error)
{
    m_isScanning = false;
    m_statusLabel->setText("Error: " + error);
    showError("Scan Error", error);
}

void DriverManager::onProgressUpdated(const QString &taskId, int progress, const QString &message)
{
    m_progressBar->setValue(progress);
    m_progressLabel->setText(message);
    m_progressBar->setVisible(true);
    m_progressLabel->setVisible(true);
}

void DriverManager::onTaskCompleted(const QString &taskId, bool success, const QString &message)
{
    m_progressBar->setVisible(false);
    m_progressLabel->setVisible(false);
    
    if (success) {
        m_statusLabel->setText("Task completed successfully");
        if (taskId.contains("install") || taskId.contains("remove") || taskId.contains("update")) {
            // Refresh relevant data
            refreshHardware();
            refreshDrivers();
            refreshModules();
            refreshFirmware();
        }
    } else {
        m_statusLabel->setText("Task failed: " + message);
        showError("Task Failed", message);
    }
}

void DriverManager::onRefreshTimer()
{
    if (m_autoRefresh && !m_isScanning) {
        refreshHardware();
        refreshDrivers();
        refreshModules();
        refreshFirmware();
    }
}

void DriverManager::onTaskProgress(int taskId, const QString &progress)
{
    m_progressLabel->setText(progress);
    m_progressBar->setVisible(true);
    m_progressLabel->setVisible(true);
}

void DriverManager::onTaskFinished(int taskId, int exitCode, const QString &output)
{
    m_progressBar->setVisible(false);
    m_progressLabel->setVisible(false);
    
    if (exitCode == 0) {
        m_statusLabel->setText("Task completed successfully");
        // Refresh relevant data
        refreshHardware();
        refreshDrivers();
        refreshModules();
        refreshFirmware();
    } else {
        m_statusLabel->setText("Task failed");
        showError("Task Failed", output);
    }
}

void DriverManager::onTaskError(int taskId, const QString &error)
{
    m_progressBar->setVisible(false);
    m_progressLabel->setVisible(false);
    m_statusLabel->setText("Task error: " + error);
    showError("Task Error", error);
}

void DriverManager::onTaskStarted(int taskId, const QString &description)
{
    m_statusLabel->setText("Running: " + description);
    m_progressBar->setVisible(true);
    m_progressLabel->setVisible(true);
    m_progressLabel->setText(description);
}

void DriverManager::onHardwareTableContextMenu(const QPoint &pos)
{
    if (m_hardwareTable->itemAt(pos)) {
        m_hardwareContextMenu->popup(m_hardwareTable->mapToGlobal(pos));
    }
}

void DriverManager::onDriverTableContextMenu(const QPoint &pos)
{
    if (m_driverTable->itemAt(pos)) {
        m_driverContextMenu->popup(m_driverTable->mapToGlobal(pos));
    }
}

void DriverManager::onModuleTableContextMenu(const QPoint &pos)
{
    if (m_moduleTable->itemAt(pos)) {
        m_moduleContextMenu->popup(m_moduleTable->mapToGlobal(pos));
    }
}

void DriverManager::onFirmwareTableContextMenu(const QPoint &pos)
{
    if (m_firmwareTable->itemAt(pos)) {
        m_firmwareContextMenu->popup(m_firmwareTable->mapToGlobal(pos));
    }
}

void DriverManager::onHardwareSelectionChanged()
{
    bool hasSelection = m_hardwareTable->selectionModel()->hasSelection();
    m_hardwareDetailsButton->setEnabled(hasSelection);
    m_installDriverButton->setEnabled(hasSelection);
    m_findDriverButton->setEnabled(hasSelection);
}

void DriverManager::onDriverSelectionChanged()
{
    bool hasSelection = m_driverTable->selectionModel()->hasSelection();
    m_driverDetailsButton->setEnabled(hasSelection);
    m_removeDriverButton->setEnabled(hasSelection);
    m_enableDriverButton->setEnabled(hasSelection);
    m_disableDriverButton->setEnabled(hasSelection);
    m_blacklistDriverButton->setEnabled(hasSelection);
}

void DriverManager::onModuleSelectionChanged()
{
    bool hasSelection = m_moduleTable->selectionModel()->hasSelection();
    m_moduleDetailsButton->setEnabled(hasSelection);
    m_loadModuleButton->setEnabled(hasSelection);
    m_unloadModuleButton->setEnabled(hasSelection);
    m_reloadModuleButton->setEnabled(hasSelection);
    m_blacklistModuleButton->setEnabled(hasSelection);
}

void DriverManager::onFirmwareSelectionChanged()
{
    bool hasSelection = m_firmwareTable->selectionModel()->hasSelection();
    m_firmwareDetailsButton->setEnabled(hasSelection);
    m_installFirmwareButton->setEnabled(hasSelection);
    m_removeFirmwareButton->setEnabled(hasSelection);
    m_updateFirmwareButton->setEnabled(hasSelection);
    m_downloadFirmwareButton->setEnabled(hasSelection);
}

void DriverManager::searchHardware()
{
    QString searchTerm = m_searchEdit->text().toLower();
    QString filter = m_filterComboBox->currentText();
    
    // Apply search filter to all tables
    if (filter == "All" || filter == "Hardware") {
        for (int i = 0; i < m_hardwareTable->rowCount(); ++i) {
            bool visible = searchTerm.isEmpty();
            if (!visible) {
                for (int j = 0; j < m_hardwareTable->columnCount(); ++j) {
                    QTableWidgetItem *item = m_hardwareTable->item(i, j);
                    if (item && item->text().toLower().contains(searchTerm)) {
                        visible = true;
                        break;
                    }
                }
            }
            m_hardwareTable->setRowHidden(i, !visible);
        }
    }
    
    if (filter == "All" || filter == "Drivers") {
        for (int i = 0; i < m_driverTable->rowCount(); ++i) {
            bool visible = searchTerm.isEmpty();
            if (!visible) {
                for (int j = 0; j < m_driverTable->columnCount(); ++j) {
                    QTableWidgetItem *item = m_driverTable->item(i, j);
                    if (item && item->text().toLower().contains(searchTerm)) {
                        visible = true;
                        break;
                    }
                }
            }
            m_driverTable->setRowHidden(i, !visible);
        }
    }
    
    if (filter == "All" || filter == "Modules") {
        for (int i = 0; i < m_moduleTable->rowCount(); ++i) {
            bool visible = searchTerm.isEmpty();
            if (!visible) {
                for (int j = 0; j < m_moduleTable->columnCount(); ++j) {
                    QTableWidgetItem *item = m_moduleTable->item(i, j);
                    if (item && item->text().toLower().contains(searchTerm)) {
                        visible = true;
                        break;
                    }
                }
            }
            m_moduleTable->setRowHidden(i, !visible);
        }
    }
    
    if (filter == "All" || filter == "Firmware") {
        for (int i = 0; i < m_firmwareTable->rowCount(); ++i) {
            bool visible = searchTerm.isEmpty();
            if (!visible) {
                for (int j = 0; j < m_firmwareTable->columnCount(); ++j) {
                    QTableWidgetItem *item = m_firmwareTable->item(i, j);
                    if (item && item->text().toLower().contains(searchTerm)) {
                        visible = true;
                        break;
                    }
                }
            }
            m_firmwareTable->setRowHidden(i, !visible);
        }
    }
}

void DriverManager::installMissingDrivers()
{
    if (!m_privilegedExecutor) return;
    
    QString taskId = "install_missing_drivers";
    QStringList commands;
    
    // Install common hardware support packages
    commands << "dnf install -y @hardware-support";
    commands << "dnf install -y kernel-devel kernel-headers";
    commands << "dnf install -y dkms";
    commands << "dnf install -y linux-firmware";
    
    // Install GPU drivers if needed
    commands << "dnf install -y mesa-dri-drivers";
    commands << "dnf install -y xorg-x11-drv-nvidia xorg-x11-drv-nouveau";
    commands << "dnf install -y xorg-x11-drv-amdgpu xorg-x11-drv-ati";
    
    m_privilegedExecutor->executeCommand("sh", QStringList() << "-c" << commands.join(" && "));
}

void DriverManager::updateAllDrivers()
{
    if (!m_privilegedExecutor) return;
    
    QString taskId = "update_all_drivers";
    QStringList commands;
    
    commands << "dnf update -y kernel*";
    commands << "dnf update -y linux-firmware";
    commands << "dnf update -y @hardware-support";
    commands << "dkms autoinstall";
    
    m_privilegedExecutor->executeCommand("sh", QStringList() << "-c" << commands.join(" && "));
}

void DriverManager::rebuildInitramfs()
{
    if (!m_privilegedExecutor) return;
    
    QString taskId = "rebuild_initramfs";
    QString command = QString("dracut --force --regenerate-all");
    
    m_privilegedExecutor->executeCommand("sh", QStringList() << "-c" << command);
}

void DriverManager::installDriver()
{
    int row = m_hardwareTable->currentRow();
    if (row < 0) return;
    
    QTableWidgetItem *nameItem = m_hardwareTable->item(row, 0);
    if (!nameItem) return;
    
    QString deviceName = nameItem->text();
    
    // Try to find appropriate driver package
    QString driverPackage;
    if (deviceName.contains("NVIDIA", Qt::CaseInsensitive)) {
        driverPackage = "akmod-nvidia";
    } else if (deviceName.contains("AMD", Qt::CaseInsensitive) || deviceName.contains("ATI", Qt::CaseInsensitive)) {
        driverPackage = "mesa-dri-drivers";
    } else if (deviceName.contains("Intel", Qt::CaseInsensitive)) {
        driverPackage = "mesa-dri-drivers";
    } else if (deviceName.contains("Wireless", Qt::CaseInsensitive) || deviceName.contains("Wi-Fi", Qt::CaseInsensitive)) {
        driverPackage = "NetworkManager-wifi";
    } else if (deviceName.contains("Bluetooth", Qt::CaseInsensitive)) {
        driverPackage = "bluez";
    } else {
        // Generic hardware support
        driverPackage = "@hardware-support";
    }
    
    if (m_privilegedExecutor) {
        QString taskId = "install_driver_" + deviceName;
        QString command = QString("dnf install -y %1").arg(driverPackage);
        m_privilegedExecutor->executeCommand("sh", QStringList() << "-c" << command);
    }
}

void DriverManager::removeDriver()
{
    int row = m_driverTable->currentRow();
    if (row < 0) return;
    
    QTableWidgetItem *nameItem = m_driverTable->item(row, 0);
    if (!nameItem) return;
    
    QString driverName = nameItem->text();
    
    if (m_privilegedExecutor) {
        QString taskId = "remove_driver_" + driverName;
        QString command = QString("modprobe -r %1").arg(driverName);
        m_privilegedExecutor->executeCommand("sh", QStringList() << "-c" << command);
    }
}

void DriverManager::enableDriver()
{
    int row = m_driverTable->currentRow();
    if (row < 0) return;
    
    QTableWidgetItem *nameItem = m_driverTable->item(row, 0);
    if (!nameItem) return;
    
    QString driverName = nameItem->text();
    
    if (m_privilegedExecutor) {
        QString taskId = "enable_driver_" + driverName;
        QString command = QString("modprobe %1").arg(driverName);
        m_privilegedExecutor->executeCommand("sh", QStringList() << "-c" << command);
    }
}

void DriverManager::disableDriver()
{
    int row = m_driverTable->currentRow();
    if (row < 0) return;
    
    QTableWidgetItem *nameItem = m_driverTable->item(row, 0);
    if (!nameItem) return;
    
    QString driverName = nameItem->text();
    
    if (m_privilegedExecutor) {
        QString taskId = "disable_driver_" + driverName;
        QString command = QString("modprobe -r %1").arg(driverName);
        m_privilegedExecutor->executeCommand("sh", QStringList() << "-c" << command);
    }
}

void DriverManager::blacklistDriver()
{
    int row = m_driverTable->currentRow();
    if (row < 0) return;
    
    QTableWidgetItem *nameItem = m_driverTable->item(row, 0);
    if (!nameItem) return;
    
    QString driverName = nameItem->text();
    
    if (m_privilegedExecutor) {
        QString taskId = "blacklist_driver_" + driverName;
        QString command = QString("echo 'blacklist %1' >> /etc/modprobe.d/blacklist.conf").arg(driverName);
        m_privilegedExecutor->executeCommand("sh", QStringList() << "-c" << command);
    }
}

void DriverManager::loadModule()
{
    int row = m_moduleTable->currentRow();
    if (row < 0) return;
    
    QTableWidgetItem *nameItem = m_moduleTable->item(row, 0);
    if (!nameItem) return;
    
    QString moduleName = nameItem->text();
    
    if (m_privilegedExecutor) {
        QString taskId = "load_module_" + moduleName;
        QString command = QString("modprobe %1").arg(moduleName);
        m_privilegedExecutor->executeCommand("sh", QStringList() << "-c" << command);
    }
}

void DriverManager::unloadModule()
{
    int row = m_moduleTable->currentRow();
    if (row < 0) return;
    
    QTableWidgetItem *nameItem = m_moduleTable->item(row, 0);
    if (!nameItem) return;
    
    QString moduleName = nameItem->text();
    
    if (m_privilegedExecutor) {
        QString taskId = "unload_module_" + moduleName;
        QString command = QString("modprobe -r %1").arg(moduleName);
        m_privilegedExecutor->executeCommand("sh", QStringList() << "-c" << command);
    }
}

void DriverManager::reloadModule()
{
    int row = m_moduleTable->currentRow();
    if (row < 0) return;
    
    QTableWidgetItem *nameItem = m_moduleTable->item(row, 0);
    if (!nameItem) return;
    
    QString moduleName = nameItem->text();
    
    if (m_privilegedExecutor) {
        QString taskId = "reload_module_" + moduleName;
        QString command = QString("modprobe -r %1 && modprobe %1").arg(moduleName);
        m_privilegedExecutor->executeCommand("sh", QStringList() << "-c" << command);
    }
}

void DriverManager::blacklistModule()
{
    int row = m_moduleTable->currentRow();
    if (row < 0) return;
    
    QTableWidgetItem *nameItem = m_moduleTable->item(row, 0);
    if (!nameItem) return;
    
    QString moduleName = nameItem->text();
    
    if (m_privilegedExecutor) {
        QString taskId = "blacklist_module_" + moduleName;
        QString command = QString("echo 'blacklist %1' >> /etc/modprobe.d/blacklist.conf").arg(moduleName);
        m_privilegedExecutor->executeCommand("sh", QStringList() << "-c" << command);
    }
}

void DriverManager::installFirmware()
{
    int row = m_firmwareTable->currentRow();
    if (row < 0) return;
    
    QTableWidgetItem *nameItem = m_firmwareTable->item(row, 0);
    if (!nameItem) return;
    
    QString firmwareName = nameItem->text();
    
    if (m_privilegedExecutor) {
        QString taskId = "install_firmware_" + firmwareName;
        QString command = QString("dnf install -y linux-firmware");
        m_privilegedExecutor->executeCommand("sh", QStringList() << "-c" << command);
    }
}

void DriverManager::removeFirmware()
{
    int row = m_firmwareTable->currentRow();
    if (row < 0) return;
    
    QTableWidgetItem *nameItem = m_firmwareTable->item(row, 0);
    if (!nameItem) return;
    
    QString firmwareName = nameItem->text();
    
    if (m_privilegedExecutor) {
        QString taskId = "remove_firmware_" + firmwareName;
        QString command = QString("rm -f /lib/firmware/%1").arg(firmwareName);
        m_privilegedExecutor->executeCommand("sh", QStringList() << "-c" << command);
    }
}

void DriverManager::updateFirmware()
{
    if (m_privilegedExecutor) {
        QString taskId = "update_firmware";
        QString command = "dnf update -y linux-firmware";
        m_privilegedExecutor->executeCommand("sh", QStringList() << "-c" << command);
    }
}

void DriverManager::downloadFirmware()
{
    if (m_privilegedExecutor) {
        QString taskId = "download_firmware";
        QString command = "dnf install -y linux-firmware";
        m_privilegedExecutor->executeCommand("sh", QStringList() << "-c" << command);
    }
}

void DriverManager::installKernel()
{
    if (m_privilegedExecutor) {
        QString taskId = "install_kernel";
        QString command = "dnf install -y kernel kernel-devel kernel-headers";
        m_privilegedExecutor->executeCommand("sh", QStringList() << "-c" << command);
    }
}

void DriverManager::removeKernel()
{
    int row = m_kernelTable->currentRow();
    if (row < 0) return;
    
    QTableWidgetItem *nameItem = m_kernelTable->item(row, 0);
    if (!nameItem) return;
    
    QString kernelVersion = nameItem->text();
    
    if (m_privilegedExecutor) {
        QString taskId = "remove_kernel_" + kernelVersion;
        QString command = QString("dnf remove -y kernel-%1").arg(kernelVersion);
        m_privilegedExecutor->executeCommand("sh", QStringList() << "-c" << command);
    }
}

void DriverManager::setDefaultKernel()
{
    int row = m_kernelTable->currentRow();
    if (row < 0) return;
    
    QTableWidgetItem *nameItem = m_kernelTable->item(row, 0);
    if (!nameItem) return;
    
    QString kernelVersion = nameItem->text();
    
    if (m_privilegedExecutor) {
        QString taskId = "set_default_kernel_" + kernelVersion;
        QString command = QString("grubby --set-default /boot/vmlinuz-%1").arg(kernelVersion);
        m_privilegedExecutor->executeCommand("sh", QStringList() << "-c" << command);
    }
}

void DriverManager::installKernelHeaders()
{
    if (m_privilegedExecutor) {
        QString taskId = "install_kernel_headers";
        QString command = "dnf install -y kernel-devel kernel-headers";
        m_privilegedExecutor->executeCommand("sh", QStringList() << "-c" << command);
    }
}

void DriverManager::detectHardware()
{
    refreshHardware();
}

void DriverManager::detectMissingFirmware()
{
    m_hardwareScanner->setScanType("firmware");
    if (!m_hardwareScanner->isRunning()) {
        m_isScanning = true;
        m_statusLabel->setText("Scanning for missing firmware...");
        m_hardwareScanner->start();
    }
}

void DriverManager::updateHardwareTable()
{
    QMutexLocker locker(&m_dataMutex);
    
    m_hardwareTable->clearContents();
    m_hardwareTable->setRowCount(m_hardware.size());
    
    for (int i = 0; i < m_hardware.size(); ++i) {
        const QJsonObject &hw = m_hardware[i];
        
        m_hardwareTable->setItem(i, HARDWARE_TABLE_NAME_COLUMN, 
                                 new QTableWidgetItem(hw["description"].toString()));
        m_hardwareTable->setItem(i, HARDWARE_TABLE_TYPE_COLUMN, 
                                 new QTableWidgetItem(hw["type"].toString()));
        m_hardwareTable->setItem(i, HARDWARE_TABLE_VENDOR_COLUMN, 
                                 new QTableWidgetItem(hw["vendor_id"].toString()));
        m_hardwareTable->setItem(i, HARDWARE_TABLE_MODEL_COLUMN, 
                                 new QTableWidgetItem(hw["device_id"].toString()));
        m_hardwareTable->setItem(i, HARDWARE_TABLE_DRIVER_COLUMN, 
                                 new QTableWidgetItem(hw["driver"].toString()));
        m_hardwareTable->setItem(i, HARDWARE_TABLE_STATUS_COLUMN, 
                                 new QTableWidgetItem(hw["status"].toString()));
    }
}

void DriverManager::updateDriverTable()
{
    QMutexLocker locker(&m_dataMutex);
    
    m_driverTable->clearContents();
    m_driverTable->setRowCount(m_drivers.size());
    
    for (int i = 0; i < m_drivers.size(); ++i) {
        const QJsonObject &driver = m_drivers[i];
        
        m_driverTable->setItem(i, DRIVER_TABLE_NAME_COLUMN, 
                               new QTableWidgetItem(driver["name"].toString()));
        m_driverTable->setItem(i, DRIVER_TABLE_VERSION_COLUMN, 
                               new QTableWidgetItem(driver["version"].toString()));
        m_driverTable->setItem(i, DRIVER_TABLE_TYPE_COLUMN, 
                               new QTableWidgetItem(driver["type"].toString()));
        m_driverTable->setItem(i, DRIVER_TABLE_STATUS_COLUMN, 
                               new QTableWidgetItem(driver["status"].toString()));
        m_driverTable->setItem(i, DRIVER_TABLE_DEVICES_COLUMN, 
                               new QTableWidgetItem(driver["device"].toString()));
    }
}

void DriverManager::updateModuleTable()
{
    QMutexLocker locker(&m_dataMutex);
    
    m_moduleTable->clearContents();
    m_moduleTable->setRowCount(m_modules.size());
    
    for (int i = 0; i < m_modules.size(); ++i) {
        const QJsonObject &module = m_modules[i];
        
        m_moduleTable->setItem(i, MODULE_TABLE_NAME_COLUMN, 
                               new QTableWidgetItem(module["name"].toString()));
        m_moduleTable->setItem(i, MODULE_TABLE_SIZE_COLUMN, 
                               new QTableWidgetItem(module["size"].toString()));
        m_moduleTable->setItem(i, MODULE_TABLE_USED_COLUMN, 
                               new QTableWidgetItem(module["used_by"].toString()));
        m_moduleTable->setItem(i, MODULE_TABLE_DEPENDENCIES_COLUMN, 
                               new QTableWidgetItem(module["depends"].toString()));
        m_moduleTable->setItem(i, MODULE_TABLE_STATUS_COLUMN, 
                               new QTableWidgetItem(module["status"].toString()));
    }
}

void DriverManager::updateFirmwareTable()
{
    QMutexLocker locker(&m_dataMutex);
    
    m_firmwareTable->clearContents();
    m_firmwareTable->setRowCount(m_firmware.size());
    
    for (int i = 0; i < m_firmware.size(); ++i) {
        const QJsonObject &fw = m_firmware[i];
        
        m_firmwareTable->setItem(i, FIRMWARE_TABLE_NAME_COLUMN, 
                                 new QTableWidgetItem(fw["name"].toString()));
        m_firmwareTable->setItem(i, FIRMWARE_TABLE_VERSION_COLUMN, 
                                 new QTableWidgetItem(fw["version"].toString()));
        m_firmwareTable->setItem(i, FIRMWARE_TABLE_SIZE_COLUMN, 
                                 new QTableWidgetItem(formatSize(fw["size"].toInt())));
        m_firmwareTable->setItem(i, FIRMWARE_TABLE_DEVICE_COLUMN, 
                                 new QTableWidgetItem(fw["device"].toString()));
        m_firmwareTable->setItem(i, FIRMWARE_TABLE_STATUS_COLUMN, 
                                 new QTableWidgetItem(fw["status"].toString()));
    }
}

void DriverManager::updateKernelInfo()
{
    if (m_privilegedExecutor) {
        QString taskId = "get_kernel_info";
        QString command = "rpm -qa kernel* | sort -V";
        m_privilegedExecutor->executeCommand("sh", QStringList() << "-c" << command);
    }
}

void DriverManager::updateInfoPanel()
{
    if (m_systemUtils) {
        QString systemInfo = QString("%1 %2 %3").arg(SystemUtils::getDistroName()).arg(SystemUtils::getKernelVersion()).arg(SystemUtils::getDesktopEnvironment());
       m_systemInfoLabel->setText(QString("System: %1").arg(systemInfo));
    }
    
    m_kernelInfoLabel->setText(QString("Kernel: %1").arg(m_currentKernel));
    m_architectureInfoLabel->setText(QString("Architecture: %1").arg(m_currentArchitecture));
    
    {
        QMutexLocker locker(&m_dataMutex);
        m_driverCountLabel->setText(QString("Drivers: %1").arg(m_drivers.size()));
        m_moduleCountLabel->setText(QString("Modules: %1").arg(m_modules.size()));
        m_firmwareCountLabel->setText(QString("Firmware: %1").arg(m_firmware.size()));
    }
}

void DriverManager::updateTheme()
{
    // Use standard system theme
    setStyleSheet("");
}

QString DriverManager::getCurrentKernelVersion()
{
    QProcess process;
    process.start("uname", QStringList() << "-r");
    if (process.waitForFinished(3000)) {
        return process.readAllStandardOutput().trimmed();
    }
    return "Unknown";
}

QString DriverManager::getCurrentArchitecture()
{
    QProcess process;
    process.start("uname", QStringList() << "-m");
    if (process.waitForFinished(3000)) {
        return process.readAllStandardOutput().trimmed();
    }
    return "Unknown";
}

QString DriverManager::formatSize(qint64 bytes)
{
    if (bytes < 1024) {
        return QString("%1 B").arg(bytes);
    } else if (bytes < 1024 * 1024) {
        return QString("%1 KB").arg(bytes / 1024);
    } else if (bytes < 1024 * 1024 * 1024) {
        return QString("%1 MB").arg(bytes / (1024 * 1024));
    } else {
        return QString("%1 GB").arg(bytes / (1024 * 1024 * 1024));
    }
}

void DriverManager::showError(const QString &title, const QString &message)
{
    QMessageBox::critical(this, title, message);
}

void DriverManager::showSuccess(const QString &title, const QString &message)
{
    QMessageBox::information(this, title, message);
}

void DriverManager::showInfo(const QString &title, const QString &message)
{
    QMessageBox::information(this, title, message);
}

void DriverManager::showHardwareDetails()
{
    int row = m_hardwareTable->currentRow();
    if (row < 0) return;
    
    QStringList details;
    for (int col = 0; col < m_hardwareTable->columnCount(); ++col) {
        QTableWidgetItem *item = m_hardwareTable->item(row, col);
        if (item) {
            QString header = m_hardwareTable->horizontalHeaderItem(col)->text();
            details << QString("%1: %2").arg(header, item->text());
        }
    }
    
    showInfo("Hardware Details", details.join("\n"));
}

void DriverManager::showDriverDetails()
{
    int row = m_driverTable->currentRow();
    if (row < 0) return;
    
    QStringList details;
    for (int col = 0; col < m_driverTable->columnCount(); ++col) {
        QTableWidgetItem *item = m_driverTable->item(row, col);
        if (item) {
            QString header = m_driverTable->horizontalHeaderItem(col)->text();
            details << QString("%1: %2").arg(header, item->text());
        }
    }
    
    showInfo("Driver Details", details.join("\n"));
}

void DriverManager::showModuleDetails()
{
    int row = m_moduleTable->currentRow();
    if (row < 0) return;
    
    QStringList details;
    for (int col = 0; col < m_moduleTable->columnCount(); ++col) {
        QTableWidgetItem *item = m_moduleTable->item(row, col);
        if (item) {
            QString header = m_moduleTable->horizontalHeaderItem(col)->text();
            details << QString("%1: %2").arg(header, item->text());
        }
    }
    
    showInfo("Module Details", details.join("\n"));
}

void DriverManager::showFirmwareDetails()
{
    int row = m_firmwareTable->currentRow();
    if (row < 0) return;
    
    QStringList details;
    for (int col = 0; col < m_firmwareTable->columnCount(); ++col) {
        QTableWidgetItem *item = m_firmwareTable->item(row, col);
        if (item) {
            QString header = m_firmwareTable->horizontalHeaderItem(col)->text();
            details << QString("%1: %2").arg(header, item->text());
        }
    }
    
    showInfo("Firmware Details", details.join("\n"));
}

void DriverManager::showDriverInstaller()
{
    showInfo("Driver Installer", "Driver installer dialog would open here");
}

void DriverManager::showModuleManager()
{
    showInfo("Module Manager", "Module manager dialog would open here");
}

void DriverManager::showFirmwareUpdater()
{
    showInfo("Firmware Updater", "Firmware updater dialog would open here");
}

void DriverManager::showSystemInfo()
{
    if (m_privilegedExecutor) {
        QString taskId = "show_system_info";
        QString command = "lshw -short";
        m_privilegedExecutor->executeCommand("sh", QStringList() << "-c" << command);
    }
}

void DriverManager::addModuleParameter()
{
    showInfo("Module Parameters", "Module parameter dialog would open here");
}

void DriverManager::generateHardwareReport()
{
    if (m_privilegedExecutor) {
        QString taskId = "generate_hardware_report";
        QString command = "lshw -html > /tmp/hardware_report.html && xdg-open /tmp/hardware_report.html";
        m_privilegedExecutor->executeCommand("sh", QStringList() << "-c" << command);
    }
}

void DriverManager::getKernelConfiguration()
{
    if (m_privilegedExecutor) {
        QString taskId = "get_kernel_config";
        QString command = "cat /boot/config-$(uname -r) | head -50";
        m_privilegedExecutor->executeCommand("sh", QStringList() << "-c" << command);
    }
}

// Stub implementations for methods declared in header but not yet implemented
void DriverManager::onHardwareActionTriggered() 
{
    QAction *action = qobject_cast<QAction*>(sender());
    if (action) {
        m_statusLabel->setText("Hardware action: " + action->text());
    }
}

void DriverManager::onDriverActionTriggered() 
{
    QAction *action = qobject_cast<QAction*>(sender());
    if (action) {
        m_statusLabel->setText("Driver action: " + action->text());
    }
}

void DriverManager::onModuleActionTriggered() 
{
    QAction *action = qobject_cast<QAction*>(sender());
    if (action) {
        m_statusLabel->setText("Module action: " + action->text());
    }
}

void DriverManager::onFirmwareActionTriggered() 
{
    QAction *action = qobject_cast<QAction*>(sender());
    if (action) {
        m_statusLabel->setText("Firmware action: " + action->text());
    }
}

void DriverManager::onKernelChanged(const QString &kernelVersion) 
{
    m_currentKernel = kernelVersion;
    updateInfoPanel();
    m_statusLabel->setText("Kernel changed to: " + kernelVersion);
}

void DriverManager::onArchitectureChanged(const QString &architecture) 
{
    m_currentArchitecture = architecture;
    updateInfoPanel();
    m_statusLabel->setText("Architecture changed to: " + architecture);
}

void DriverManager::showKernelManager() 
{
    showInfo("Kernel Manager", "Kernel management functionality");
}

void DriverManager::showDKMSManager() 
{
    showInfo("DKMS Manager", "DKMS module management functionality");
}

void DriverManager::whitelistDriver() 
{
    showInfo("Whitelist Driver", "Driver whitelist functionality");
}

void DriverManager::whitelistModule() 
{
    showInfo("Whitelist Module", "Module whitelist functionality");
}

void DriverManager::removeModuleParameter() 
{
    showInfo("Remove Module Parameter", "Module parameter removal functionality");
}

void DriverManager::extractFirmware() 
{
    showInfo("Extract Firmware", "Firmware extraction functionality");
}

void DriverManager::installDKMSModule() 
{
    if (m_privilegedExecutor) {
        m_privilegedExecutor->executeCommand("dkms", QStringList() << "status");
    }
}

void DriverManager::removeDKMSModule() 
{
    if (m_privilegedExecutor) {
        m_privilegedExecutor->executeCommand("dkms", QStringList() << "status");
    }
}

void DriverManager::buildDKMSModule() 
{
    if (m_privilegedExecutor) {
        m_privilegedExecutor->executeCommand("dkms", QStringList() << "status");
    }
}

void DriverManager::installBuildEssentials() 
{
    if (m_privilegedExecutor) {
        m_privilegedExecutor->executeCommand("dnf", QStringList() << "install" << "-y" << "gcc" << "make" << "kernel-devel");
    }
}

void DriverManager::checkDriverConflicts() 
{
    showInfo("Check Driver Conflicts", "Driver conflict checking functionality");
}

void DriverManager::resolveDriverConflicts() 
{
    showInfo("Resolve Driver Conflicts", "Driver conflict resolution functionality");
}

void DriverManager::createDriverBackup() 
{
    showInfo("Create Driver Backup", "Driver backup creation functionality");
}

void DriverManager::restoreDriverBackup() 
{
    showInfo("Restore Driver Backup", "Driver backup restoration functionality");
}

void DriverManager::exportDriverList() 
{
    showInfo("Export Driver List", "Driver list export functionality");
}

void DriverManager::importDriverList() 
{
    showInfo("Import Driver List", "Driver list import functionality");
}

void DriverManager::scanForNewHardware() 
{
    refreshHardware();
}

void DriverManager::installProprietaryDrivers() 
{
    if (m_privilegedExecutor) {
        m_privilegedExecutor->executeCommand("dnf", QStringList() << "install" << "-y" << "akmod-nvidia" << "xorg-x11-drv-nvidia");
    }
}

void DriverManager::installOpenSourceDrivers() 
{
    if (m_privilegedExecutor) {
        m_privilegedExecutor->executeCommand("dnf", QStringList() << "install" << "-y" << "mesa-dri-drivers" << "xorg-x11-drv-nouveau");
    }
}

void DriverManager::switchToNvidiaDriver() 
{
    if (m_privilegedExecutor) {
        m_privilegedExecutor->executeCommand("dnf", QStringList() << "install" << "-y" << "akmod-nvidia" << "xorg-x11-drv-nvidia");
    }
}

void DriverManager::switchToNouveauDriver() 
{
    if (m_privilegedExecutor) {
        m_privilegedExecutor->executeCommand("dnf", QStringList() << "install" << "-y" << "xorg-x11-drv-nouveau");
    }
}

void DriverManager::switchToAMDDriver() 
{
    if (m_privilegedExecutor) {
        m_privilegedExecutor->executeCommand("dnf", QStringList() << "install" << "-y" << "xorg-x11-drv-amdgpu");
    }
}

void DriverManager::switchToRadeonDriver() 
{
    if (m_privilegedExecutor) {
        m_privilegedExecutor->executeCommand("dnf", QStringList() << "install" << "-y" << "xorg-x11-drv-ati");
    }
}

void DriverManager::switchToIntelDriver() 
{
    if (m_privilegedExecutor) {
        m_privilegedExecutor->executeCommand("dnf", QStringList() << "install" << "-y" << "xorg-x11-drv-intel");
    }
}

void DriverManager::optimizeDrivers() 
{
    showInfo("Optimize Drivers", "Driver optimization functionality");
}

void DriverManager::updateKernel() 
{
    if (m_privilegedExecutor) {
        m_privilegedExecutor->executeCommand("dnf", QStringList() << "update" << "-y" << "kernel*");
    }
}

void DriverManager::showKernelLogs() 
{
    if (m_privilegedExecutor) {
        m_privilegedExecutor->executeCommand("journalctl", QStringList() << "-k" << "--no-pager" << "-n" << "100");
    }
}

void DriverManager::showDriverLogs() 
{
    if (m_privilegedExecutor) {
        m_privilegedExecutor->executeCommand("journalctl", QStringList() << "--no-pager" << "-n" << "100" << "--grep" << "driver");
    }
}

void DriverManager::showHardwareInfo() 
{
    if (m_privilegedExecutor) {
        m_privilegedExecutor->executeCommand("lshw", QStringList() << "-short");
    }
}

void DriverManager::showDMIdecode() 
{
    if (m_privilegedExecutor) {
        m_privilegedExecutor->executeCommand("dmidecode", QStringList() << "-t" << "system");
    }
}

void DriverManager::showLspci() 
{
    if (m_privilegedExecutor) {
        m_privilegedExecutor->executeCommand("lspci", QStringList() << "-v");
    }
}

void DriverManager::showLsusb() 
{
    if (m_privilegedExecutor) {
        m_privilegedExecutor->executeCommand("lsusb", QStringList() << "-v");
    }
}

void DriverManager::showLsmod() 
{
    if (m_privilegedExecutor) {
        m_privilegedExecutor->executeCommand("lsmod", QStringList());
    }
}

void DriverManager::showLshw() 
{
    if (m_privilegedExecutor) {
        m_privilegedExecutor->executeCommand("lshw", QStringList() << "-short");
    }
}

void DriverManager::showHwinfo() 
{
    if (m_privilegedExecutor) {
        m_privilegedExecutor->executeCommand("hwinfo", QStringList() << "--short");
    }
}

void DriverManager::showInxi() 
{
    if (m_privilegedExecutor) {
        m_privilegedExecutor->executeCommand("inxi", QStringList() << "-Fxz");
    }
}

void DriverManager::checkFirmwareUpdates()
{
    showProgress("Checking", "Checking for firmware updates...");
    
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        hideProgress();
        if (exitCode == 0) {
            QString output = process->readAllStandardOutput();
            parseFirmwareUpdates(output);
            m_statusLabel->setText("Firmware update check completed");
        } else {
            m_statusLabel->setText("Failed to check for updates");
        }
        QTimer::singleShot(3000, [this]() { m_statusLabel->setText("Ready"); });
        process->deleteLater();
    });
    
    process->start("fwupdmgr", QStringList() << "get-updates");
}

void DriverManager::refreshFirmwareDevices()
{
    showProgress("Refreshing", "Refreshing firmware devices...");
    
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        hideProgress();
        if (exitCode == 0) {
            QString output = process->readAllStandardOutput();
            parseFirmwareDevices(output);
            m_statusLabel->setText("Device list refreshed");
        } else {
            m_statusLabel->setText("Failed to refresh devices");
        }
        QTimer::singleShot(3000, [this]() { m_statusLabel->setText("Ready"); });
        process->deleteLater();
    });
    
    process->start("fwupdmgr", QStringList() << "get-devices");
}

void DriverManager::applyFirmwareUpdates()
{
    int ret = QMessageBox::question(this, "Apply Updates", 
                                   "Are you sure you want to apply all available firmware updates?",
                                   QMessageBox::Yes | QMessageBox::No);
    if (ret != QMessageBox::Yes) {
        return;
    }
    
    showProgress("Updating", "Applying firmware updates...");
    
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        hideProgress();
        if (exitCode == 0) {
            m_statusLabel->setText("Firmware updates applied successfully");
            refreshFirmwareDevices(); // Refresh to show new versions
        } else {
            m_statusLabel->setText("Failed to apply firmware updates");
        }
        QTimer::singleShot(3000, [this]() { m_statusLabel->setText("Ready"); });
        process->deleteLater();
    });
    
    process->start("fwupdmgr", QStringList() << "update");
}

void DriverManager::parseFirmwareDevices(const QString &output)
{
    if (!m_firmwareTable) return;
    
    m_firmwareTable->setRowCount(0);
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    
    QString currentDevice;
    QString currentVersion;
    QString deviceName;
    
    for (const QString &line : lines) {
        QString trimmedLine = line.trimmed();
        
        if (trimmedLine.contains("Device ID:")) {
            currentDevice = trimmedLine.split(':').last().trimmed();
        } else if (trimmedLine.contains("Name:")) {
            deviceName = trimmedLine.split(':').last().trimmed();
        } else if (trimmedLine.contains("Version:")) {
            currentVersion = trimmedLine.split(':').last().trimmed();
            
            // Add device to table
            if (!deviceName.isEmpty() && !currentVersion.isEmpty()) {
                int row = m_firmwareTable->rowCount();
                m_firmwareTable->insertRow(row);
                
                m_firmwareTable->setItem(row, 0, new QTableWidgetItem(deviceName));
                m_firmwareTable->setItem(row, 1, new QTableWidgetItem(currentVersion));
                m_firmwareTable->setItem(row, 2, new QTableWidgetItem("Checking..."));
                m_firmwareTable->setItem(row, 3, new QTableWidgetItem("Up to date"));
                
                // Reset for next device
                currentDevice.clear();
                currentVersion.clear();
                deviceName.clear();
            }
        }
    }
}

void DriverManager::parseFirmwareUpdates(const QString &output)
{
    if (!m_firmwareTable) return;
    
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    
    // Simple parsing - look for update information
    for (int i = 0; i < lines.size(); ++i) {
        QString line = lines[i].trimmed();
        
        if (line.contains("Update available") || line.contains("New version")) {
            // Extract device name and version info
            QString deviceInfo = line;
            
            // Find matching row in table and update
            for (int row = 0; row < m_firmwareTable->rowCount(); ++row) {
                QTableWidgetItem *deviceItem = m_firmwareTable->item(row, 0);
                if (deviceItem && deviceInfo.contains(deviceItem->text(), Qt::CaseInsensitive)) {
                    // Extract version if possible
                    QRegularExpression versionRe("(\\d+\\.\\d+[.\\d]*)");
                    QRegularExpressionMatch match = versionRe.match(line);
                    if (match.hasMatch()) {
                        m_firmwareTable->setItem(row, 2, new QTableWidgetItem(match.captured(1)));
                        m_firmwareTable->setItem(row, 3, new QTableWidgetItem("Update available"));
                    }
                    break;
                }
            }
        }
    }
}

void DriverManager::showProgress(const QString &title, const QString &message)
{
    if (m_progressBar) {
        m_progressBar->setVisible(true);
        m_progressBar->setRange(0, 0); // Indeterminate
    }
    if (m_statusLabel) {
        m_statusLabel->setText(message);
    }
}

void DriverManager::hideProgress()
{
    if (m_progressBar) {
        m_progressBar->setVisible(false);
    }
}

 