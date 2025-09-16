#ifndef DRIVERMANAGER_H
#define DRIVERMANAGER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QGroupBox>
#include <QTabWidget>
#include <QProgressBar>
#include <QTimer>
#include <QSplitter>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QSpinBox>
#include <QCheckBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QProcess>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QRegularExpression>
#include <QDesktopServices>
#include <QUrl>
#include <QScrollArea>
#include <QFrame>
#include <QApplication>
#include <QStyle>
#include <QStyleOption>
#include <QGraphicsDropShadowEffect>
#include <QListWidget>
#include <QListWidgetItem>
#include <QButtonGroup>
#include <QRadioButton>

class SystemUtils;
class PrivilegedExecutor;

class HardwareScanner : public QThread
{
    Q_OBJECT
    
public:
    HardwareScanner(QObject *parent = nullptr);
    void stop();
    void setScanType(const QString &scanType);
    
protected:
    void run() override;
    
signals:
    void hardwareFound(const QJsonObject &hardwareInfo);
    void driverFound(const QJsonObject &driverInfo);
    void moduleFound(const QJsonObject &moduleInfo);
    void firmwareFound(const QJsonObject &firmwareInfo);
    void scanFinished();
    void errorOccurred(const QString &error);
    
private:
    QString m_scanType;
    bool m_stopRequested;
    mutable QMutex m_mutex;
    
    void scanPCIDevices();
    void scanUSBDevices();
    void scanKernelModules();
    void scanLoadedModules();
    void scanAvailableFirmware();
    void scanMissingFirmware();
    void scanDrivers();
    void scanGPUDrivers();
    void scanNetworkDrivers();
    void scanAudioDrivers();
    void scanFirmwareDirectory(const QStringList &dirs, int currentIndex);
    QJsonObject parseLspciOutput(const QString &output);
    QJsonObject parseLsusbOutput(const QString &output);
    QJsonObject parseModinfoOutput(const QString &output);
    QJsonObject parseLsmodOutput(const QString &output);
    QJsonObject parseFirmwareInfo(const QString &output);
    QJsonObject parseDriverInfo(const QString &output);
};

class DriverManager : public QWidget
{
    Q_OBJECT

public:
    explicit DriverManager(QWidget *parent = nullptr);
    ~DriverManager();
    
    void setSystemUtils(SystemUtils *utils);
    void setPrivilegedExecutor(PrivilegedExecutor *executor);
    
    // Public interface
    void refreshHardware();
    void refreshDrivers();
    void refreshModules();
    void refreshFirmware();
    void scanMissingDrivers();
    
public slots:
    void onHardwareActionTriggered();
    void onDriverActionTriggered();
    void onModuleActionTriggered();
    void onFirmwareActionTriggered();
    void searchHardware();
    void installMissingDrivers();
    void updateAllDrivers();
    void rebuildInitramfs();
    
private slots:
    void onHardwareFound(const QJsonObject &hardwareInfo);
    void onDriverFound(const QJsonObject &driverInfo);
    void onModuleFound(const QJsonObject &moduleInfo);
    void onFirmwareFound(const QJsonObject &firmwareInfo);
    void onScanFinished();
    void onScanError(const QString &error);
    void onHardwareTableContextMenu(const QPoint &pos);
    void onDriverTableContextMenu(const QPoint &pos);
    void onModuleTableContextMenu(const QPoint &pos);
    void onFirmwareTableContextMenu(const QPoint &pos);
    void onHardwareSelectionChanged();
    void onDriverSelectionChanged();
    void onModuleSelectionChanged();
    void onFirmwareSelectionChanged();
    void onProgressUpdated(const QString &taskId, int progress, const QString &message);
    void onTaskCompleted(const QString &taskId, bool success, const QString &message);
    void onTaskProgress(int taskId, const QString &progress);
    void onTaskFinished(int taskId, int exitCode, const QString &output);
    void onTaskError(int taskId, const QString &error);
    void onTaskStarted(int taskId, const QString &description);
    void onRefreshTimer();
    void onKernelChanged(const QString &kernelVersion);
    void onArchitectureChanged(const QString &architecture);
    void updateHardwareTable();
    void updateDriverTable();
    void updateModuleTable();
    void updateFirmwareTable();
    void showHardwareDetails();
    void showDriverDetails();
    void showModuleDetails();
    void showFirmwareDetails();
    void showDriverInstaller();
    void showModuleManager();
    void showFirmwareUpdater();
    void showKernelManager();
    void showDKMSManager();
    void installDriver();
    void removeDriver();
    void enableDriver();
    void disableDriver();
    void blacklistDriver();
    void whitelistDriver();
    void loadModule();
    void unloadModule();
    void reloadModule();
    void blacklistModule();
    void whitelistModule();
    void addModuleParameter();
    void removeModuleParameter();
    void installFirmware();
    void removeFirmware();
    void updateFirmware();
    void downloadFirmware();
    void extractFirmware();
    void installDKMSModule();
    void removeDKMSModule();
    void buildDKMSModule();
    void installKernelHeaders();
    void installBuildEssentials();
    void checkDriverConflicts();
    void resolveDriverConflicts();
    void createDriverBackup();
    void restoreDriverBackup();
    void generateHardwareReport();
    void exportDriverList();
    void importDriverList();
    void scanForNewHardware();
    void detectMissingFirmware();
    void installProprietaryDrivers();
    void installOpenSourceDrivers();
    void switchToNvidiaDriver();
    void switchToNouveauDriver();
    void switchToAMDDriver();
    void switchToRadeonDriver();
    void switchToIntelDriver();
    void optimizeDrivers();
    void updateKernel();
    void installKernel();
    void removeKernel();
    void setDefaultKernel();
    void showKernelLogs();
    void showDriverLogs();
    void showHardwareInfo();
    void showSystemInfo();
    void showDMIdecode();
    void showLspci();
    void showLsusb();
    void showLsmod();
    void showLshw();
    void showHwinfo();
    void showInxi();
    
    // Simplified firmware update methods
    void checkFirmwareUpdates();
    void refreshFirmwareDevices();
    void applyFirmwareUpdates();
    void parseFirmwareDevices(const QString &output);
    void parseFirmwareUpdates(const QString &output);
    void showProgress(const QString &title, const QString &message);
    void hideProgress();
    
private:
    // UI setup
    void setupUI();
    void setupHardwareTab();
    void setupDriverTab();
    void setupModuleTab();
    void setupFirmwareTab();
    void setupKernelTab();
    void setupToolbar();
    void setupInfoPanel();
    void setupContextMenus();
    void updateTheme();
    
    // Hardware operations
    void detectHardware();
    void identifyDevice(const QString &deviceId);
    void findDriverForDevice(const QString &deviceId);
    void getDeviceInfo(const QString &deviceId);
    void enableDevice(const QString &deviceId);
    void disableDevice(const QString &deviceId);
    void resetDevice(const QString &deviceId);
    void testDevice(const QString &deviceId);
    void configureDevice(const QString &deviceId);
    void updateDeviceDriver(const QString &deviceId, const QString &driverName);
    
    // Driver operations
    void installDriverPackage(const QString &packageName);
    void removeDriverPackage(const QString &packageName);
    void enableDriverPackage(const QString &packageName);
    void disableDriverPackage(const QString &packageName);
    void blacklistDriverPackage(const QString &packageName);
    void whitelistDriverPackage(const QString &packageName);
    void getDriverInfo(const QString &driverName);
    void getDriverDependencies(const QString &driverName);
    void checkDriverCompatibility(const QString &driverName);
    void downloadDriverSource(const QString &driverName);
    void compileDriver(const QString &driverName);
    void installCompiledDriver(const QString &driverName);
    
    // Module operations
    void loadKernelModule(const QString &moduleName);
    void unloadKernelModule(const QString &moduleName);
    void reloadKernelModule(const QString &moduleName);
    void blacklistKernelModule(const QString &moduleName);
    void whitelistKernelModule(const QString &moduleName);
    void addKernelModuleParameter(const QString &moduleName, const QString &parameter, const QString &value);
    void removeKernelModuleParameter(const QString &moduleName, const QString &parameter);
    void getModuleInfo(const QString &moduleName);
    void getModuleDependencies(const QString &moduleName);
    void setModuleOptions(const QString &moduleName, const QStringList &options);
    void createModuleAlias(const QString &moduleName, const QString &alias);
    void removeModuleAlias(const QString &moduleName, const QString &alias);
    
    // Firmware operations
    void installFirmwarePackage(const QString &packageName);
    void removeFirmwarePackage(const QString &packageName);
    void updateFirmwarePackage(const QString &packageName);
    void downloadFirmwarePackage(const QString &packageName);
    void extractFirmwareArchive(const QString &archivePath);
    void installFirmwareFile(const QString &firmwarePath, const QString &targetPath);
    void backupFirmware(const QString &firmwareName);
    void restoreFirmware(const QString &firmwareName);
    void getFirmwareInfo(const QString &firmwareName);
    void scanFirmwareDirectory(const QStringList &dirs, int currentIndex);
    void checkFirmwareLicense(const QString &firmwareName);
    void downloadLinuxFirmware();
    
    // DKMS operations
    void installDKMS();
    void addDKMSModule(const QString &moduleName, const QString &version);
    void removeDKMSModule(const QString &moduleName, const QString &version);
    void buildDKMSModule(const QString &moduleName, const QString &version);
    void installDKMSModule(const QString &moduleName, const QString &version);
    void uninstallDKMSModule(const QString &moduleName, const QString &version);
    void getDKMSStatus();
    void autoinstallDKMSModules();
    void rebuildAllDKMSModules();
    
    // Kernel operations
    void getKernelVersion();
    void getKernelInfo();
    void listAvailableKernels();
    void installKernelPackage(const QString &kernelVersion);
    void removeKernelPackage(const QString &kernelVersion);
    void setDefaultKernelVersion(const QString &kernelVersion);
    void updateGrubConfig();
    void rebuildInitramfsForKernel(const QString &kernelVersion);
    void installKernelHeadersForVersion(const QString &kernelVersion);
    void getKernelConfiguration();
    void buildCustomKernel();
    
    // Data management
    void updateKernelInfo();
    void updateInfoPanel();
    void parseHardwareList(const QString &output);
    void parseDriverList(const QString &output);
    void parseModuleList(const QString &output);
    void parseFirmwareList(const QString &output);
    QJsonObject parseHardwareInfo(const QString &line);
    QJsonObject parseDriverInfo(const QString &line);
    QJsonObject parseModuleInfo(const QString &line);
    QJsonObject parseFirmwareInfo(const QString &line);
    
    // Helper methods
    void showError(const QString &title, const QString &message);
    void showSuccess(const QString &title, const QString &message);
    void showInfo(const QString &title, const QString &message);
    QString getHardwareIcon(const QString &deviceType);
    QString getDriverIcon(const QString &driverType);
    QString getModuleIcon(const QString &moduleType);
    QString getFirmwareIcon(const QString &firmwareType);
    QString getStatusColor(const QString &status);
    QString formatSize(qint64 bytes);
    QString formatVersion(const QString &version);
    bool isDriverLoaded(const QString &driverName);
    bool isModuleLoaded(const QString &moduleName);
    bool isFirmwareInstalled(const QString &firmwareName);
    bool isDKMSAvailable();
    bool isKernelHeadersInstalled();
    bool isBuildEssentialsInstalled();
    QString getCurrentKernelVersion();
    QString getCurrentArchitecture();
    QStringList getLoadedModules();
    QStringList getAvailableModules();
    QStringList getBlacklistedModules();
    QStringList getInstalledFirmware();
    QStringList getMissingFirmware();
    
    // Member variables
    SystemUtils *m_systemUtils;
    PrivilegedExecutor *m_privilegedExecutor;
    
    // UI components
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_toolbarLayout;
    QTabWidget *m_tabWidget;
    
    // Hardware tab
    QWidget *m_hardwareTab;
    QVBoxLayout *m_hardwareLayout;
    QTableWidget *m_hardwareTable;
    QHBoxLayout *m_hardwareButtonLayout;
    QPushButton *m_detectHardwareButton;
    QPushButton *m_refreshHardwareButton;
    QPushButton *m_hardwareDetailsButton;
    QPushButton *m_installDriverButton;
    QPushButton *m_findDriverButton;
    QPushButton *m_deviceManagerButton;
    QPushButton *m_hardwareReportButton;
    
    // Driver tab
    QWidget *m_driverTab;
    QVBoxLayout *m_driverLayout;
    QTableWidget *m_driverTable;
    QHBoxLayout *m_driverButtonLayout;
    QPushButton *m_installDriverButton2;
    QPushButton *m_removeDriverButton;
    QPushButton *m_enableDriverButton;
    QPushButton *m_disableDriverButton;
    QPushButton *m_blacklistDriverButton;
    QPushButton *m_driverDetailsButton;
    QPushButton *m_updateDriversButton;
    QPushButton *m_refreshDriversButton;
    
    // Module tab
    QWidget *m_moduleTab;
    QVBoxLayout *m_moduleLayout;
    QTableWidget *m_moduleTable;
    QHBoxLayout *m_moduleButtonLayout;
    QPushButton *m_loadModuleButton;
    QPushButton *m_unloadModuleButton;
    QPushButton *m_reloadModuleButton;
    QPushButton *m_blacklistModuleButton;
    QPushButton *m_moduleDetailsButton;
    QPushButton *m_moduleManagerButton;
    QPushButton *m_refreshModulesButton;
    
    // Firmware tab
    QWidget *m_firmwareTab;
    QVBoxLayout *m_firmwareLayout;
    QTableWidget *m_firmwareTable;
    QHBoxLayout *m_firmwareButtonLayout;
    QPushButton *m_installFirmwareButton;
    QPushButton *m_removeFirmwareButton;
    QPushButton *m_updateFirmwareButton;
    QPushButton *m_downloadFirmwareButton;
    QPushButton *m_firmwareDetailsButton;
    QPushButton *m_scanMissingFirmwareButton;
    QPushButton *m_refreshFirmwareButton;
    
    // Kernel tab
    QWidget *m_kernelTab;
    QVBoxLayout *m_kernelLayout;
    QTableWidget *m_kernelTable;
    QHBoxLayout *m_kernelButtonLayout;
    QPushButton *m_installKernelButton;
    QPushButton *m_removeKernelButton;
    QPushButton *m_setDefaultKernelButton;
    QPushButton *m_rebuildInitramfsButton;
    QPushButton *m_kernelHeadersButton;
    QPushButton *m_kernelConfigButton;
    QPushButton *m_refreshKernelButton;
    
    // Info panel
    QGroupBox *m_infoPanel;
    QVBoxLayout *m_infoPanelLayout;
    QLabel *m_systemInfoLabel;
    QLabel *m_kernelInfoLabel;
    QLabel *m_architectureInfoLabel;
    QLabel *m_driverCountLabel;
    QLabel *m_moduleCountLabel;
    QLabel *m_firmwareCountLabel;
    
    // Search and filter
    QLineEdit *m_searchEdit;
    QComboBox *m_filterComboBox;
    QPushButton *m_searchButton;
    QPushButton *m_clearSearchButton;
    
    // Progress and status
    QProgressBar *m_progressBar;
    QLabel *m_progressLabel;
    QTextEdit *m_outputTextEdit;
    QLabel *m_statusLabel;
    
    // Context menus
    QMenu *m_hardwareContextMenu;
    QMenu *m_driverContextMenu;
    QMenu *m_moduleContextMenu;
    QMenu *m_firmwareContextMenu;
    QMenu *m_kernelContextMenu;
    
    // Background workers
    HardwareScanner *m_hardwareScanner;
    QTimer *m_refreshTimer;
    
    // Data
    QList<QJsonObject> m_hardware;
    QList<QJsonObject> m_drivers;
    QList<QJsonObject> m_modules;
    QList<QJsonObject> m_firmware;
    QList<QJsonObject> m_kernels;
    
    // Settings
    bool m_autoRefresh;
    int m_refreshInterval;
    QString m_currentKernel;
    QString m_currentArchitecture;
    
    // State
    bool m_isScanning;
    QMutex m_dataMutex;
    
    // Constants
    static const int HARDWARE_TABLE_NAME_COLUMN = 0;
    static const int HARDWARE_TABLE_TYPE_COLUMN = 1;
    static const int HARDWARE_TABLE_VENDOR_COLUMN = 2;
    static const int HARDWARE_TABLE_MODEL_COLUMN = 3;
    static const int HARDWARE_TABLE_DRIVER_COLUMN = 4;
    static const int HARDWARE_TABLE_STATUS_COLUMN = 5;
    
    static const int DRIVER_TABLE_NAME_COLUMN = 0;
    static const int DRIVER_TABLE_VERSION_COLUMN = 1;
    static const int DRIVER_TABLE_TYPE_COLUMN = 2;
    static const int DRIVER_TABLE_STATUS_COLUMN = 3;
    static const int DRIVER_TABLE_DEVICES_COLUMN = 4;
    
    static const int MODULE_TABLE_NAME_COLUMN = 0;
    static const int MODULE_TABLE_SIZE_COLUMN = 1;
    static const int MODULE_TABLE_USED_COLUMN = 2;
    static const int MODULE_TABLE_DEPENDENCIES_COLUMN = 3;
    static const int MODULE_TABLE_STATUS_COLUMN = 4;
    
    static const int FIRMWARE_TABLE_NAME_COLUMN = 0;
    static const int FIRMWARE_TABLE_VERSION_COLUMN = 1;
    static const int FIRMWARE_TABLE_SIZE_COLUMN = 2;
    static const int FIRMWARE_TABLE_DEVICE_COLUMN = 3;
    static const int FIRMWARE_TABLE_STATUS_COLUMN = 4;
};

#endif // DRIVERMANAGER_H 