#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

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
#include <QSlider>
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
#include <QDoubleSpinBox>
#include <QDial>

class SystemUtils;
class PrivilegedExecutor;

class AudioDeviceWorker : public QThread
{
    Q_OBJECT
    
public:
    AudioDeviceWorker(QObject *parent = nullptr);
    void stop();
    
protected:
    void run() override;
    
signals:
    void deviceFound(const QJsonObject &deviceInfo);
    void profileFound(const QJsonObject &profileInfo);
    void cardFound(const QJsonObject &cardInfo);
    void scanFinished();
    void errorOccurred(const QString &error);
    
private:
    bool m_stopRequested;
    mutable QMutex m_mutex;
    
    void scanPipeWireDevices();
    void scanPulseAudioDevices();
    void scanAlsaDevices();
    QJsonObject parsePipeWireInfo(const QString &output);
    QJsonObject parsePulseAudioInfo(const QString &output);
    QJsonObject parseAlsaInfo(const QString &output);
};

class AudioManager : public QWidget
{
    Q_OBJECT

public:
    explicit AudioManager(QWidget *parent = nullptr);
    ~AudioManager();
    
    void setSystemUtils(SystemUtils *utils);
    void setPrivilegedExecutor(PrivilegedExecutor *executor);
    
    // Public interface
    void refreshDevices();
    void refreshProfiles();
    void refreshEasyEffectsPresets();
    void refreshPipeWireInfo();
    
public slots:
    void onDeviceActionTriggered();
    void onProfileActionTriggered();
    void onEffectActionTriggered();
    void searchDevices();
    void applyAudioProfile();
    void resetAudioSettings();
    
private slots:
    void onDeviceFound(const QJsonObject &deviceInfo);
    void onProfileFound(const QJsonObject &profileInfo);
    void onCardFound(const QJsonObject &cardInfo);
    void onScanFinished();
    void onScanError(const QString &error);
    void onDeviceTableContextMenu(const QPoint &pos);
    void onProfileTableContextMenu(const QPoint &pos);
    void onEffectTableContextMenu(const QPoint &pos);
    void onDeviceSelectionChanged();
    void onProfileSelectionChanged();
    void onEffectSelectionChanged();
    void onProgressUpdated(const QString &taskId, int progress, const QString &message);
    void onTaskCompleted(const QString &taskId, bool success, const QString &message);
    void onRefreshTimer();
    void onVolumeChanged(int value);
    void onMuteToggled(bool muted);
    void onEffectToggled(bool enabled);
    void onProfileChanged(const QString &profileName);
    void onDeviceChanged(const QString &deviceName);
    void onSampleRateChanged(int sampleRate);
    void onBufferSizeChanged(int bufferSize);
    void onLatencyChanged(int latency);
    void showDeviceProperties();
    void showProfileEditor();
    void showEffectEditor();
    void showEasyEffectsPresets();
    void showPipeWireSettings();
    void showAdvancedSettings();
    void importProfile();
    void exportProfile();
    void createProfile();
    void deleteProfile();
    void installEasyEffects();
    void startEasyEffects();
    void stopEasyEffects();
    void enableEasyEffectsAutostart();
    void disableEasyEffectsAutostart();
    void loadEasyEffectsPreset();
    void saveEasyEffectsPreset();
    void deleteEasyEffectsPreset();
    void resetEasyEffectsPreset();
    void installPipeWire();
    void startPipeWire();
    void stopPipeWire();
    void restartPipeWire();
    void enablePipeWireAutostart();
    void disablePipeWireAutostart();
    void showPipeWireGraph();
    void optimizeForLatency();
    void optimizeForQuality();
    void optimizeForPowerSaving();
    void testAudioDevices();
    void calibrateAudioLevels();
    void showAudioAnalyzer();
    void showEqualizerSettings();
    void showCompressorSettings();
    void showReverbSettings();
    void showDelaySettings();
    void showChorusSettings();
    void showDistortionSettings();
    void showFilterSettings();
    void showNoiseGateSettings();
    void showLimiterSettings();
    void showMasteringSettings();
    
    // New simplified UI methods
    void setupSimplifiedAudioControls();
    void setMasterVolume(int volume);
    void setMasterMute(bool muted);
    void setInputVolume(int volume);
    void setInputMute(bool muted);
    void setSampleRate(const QString &sampleRate);
    void setBufferSize(const QString &bufferSize);
    void editPipeWireConfig();
    void reloadPipeWireConfig();
    void launchEasyEffects();
    
private:
    // UI setup
    void setupUI();
    void setupDeviceTab();
    void setupProfileTab();
    void setupEffectTab();
    void setupEasyEffectsTab();
    void setupPipeWireTab();
    void setupToolbar();
    void setupControlPanel();
    void setupMixerPanel();
    void setupEffectChain();
    void setupContextMenus();
    void updateTheme();
    void setupProgressArea();
    
    // Audio operations
    void setDeviceVolume(const QString &deviceName, int volume);
    void setDeviceMute(const QString &deviceName, bool muted);
    void setDefaultDevice(const QString &deviceName, const QString &deviceType);
    void connectDevices(const QString &sourceDevice, const QString &sinkDevice);
    void disconnectDevices(const QString &sourceDevice, const QString &sinkDevice);
    void moveStreamToDevice(const QString &streamName, const QString &deviceName);
    void setDeviceProfile(const QString &deviceName, const QString &profileName);
    void setDevicePort(const QString &deviceName, const QString &portName);
    void setDeviceLatency(const QString &deviceName, int latency);
    void setDeviceSampleRate(const QString &deviceName, int sampleRate);
    void setDeviceBufferSize(const QString &deviceName, int bufferSize);
    void setDeviceChannels(const QString &deviceName, int channels);
    void setDeviceFormat(const QString &deviceName, const QString &format);
    void suspendDevice(const QString &deviceName);
    void resumeDevice(const QString &deviceName);
    void resetDevice(const QString &deviceName);
    void calibrateDevice(const QString &deviceName);
    void testDevice(const QString &deviceName);
    
    // Profile operations
    void loadProfile(const QString &profileName);
    void saveProfile(const QString &profileName);
    void deleteProfileByName(const QString &profileName);
    void duplicateProfile(const QString &profileName);
    void renameProfile(const QString &oldName, const QString &newName);
    void exportProfileToFile(const QString &profileName, const QString &filePath);
    void importProfileFromFile(const QString &filePath);
    void createCustomProfile(const QString &name, const QJsonObject &settings);
    void applyOptimizationPreset(const QString &presetName);
    
    // EasyEffects operations
    void loadEasyEffectsPresetByName(const QString &presetName);
    void saveEasyEffectsPresetByName(const QString &presetName);
    void deleteEasyEffectsPresetByName(const QString &presetName);
    void resetEasyEffectsPresetByName(const QString &presetName);
    void duplicateEasyEffectsPreset(const QString &presetName);
    void renameEasyEffectsPreset(const QString &oldName, const QString &newName);
    void exportEasyEffectsPreset(const QString &presetName, const QString &filePath);
    void importEasyEffectsPreset(const QString &filePath);
    void enableEasyEffectsForDevice(const QString &deviceName);
    void disableEasyEffectsForDevice(const QString &deviceName);
    void setEasyEffectsPresetForDevice(const QString &deviceName, const QString &presetName);
    
    // PipeWire operations
    void setPipeWireProperty(const QString &objectName, const QString &property, const QString &value);
    void createPipeWireVirtualDevice(const QString &name, const QString &type, const QJsonObject &properties);
    void destroyPipeWireVirtualDevice(const QString &name);
    void linkPipeWirePorts(const QString &outputPort, const QString &inputPort);
    void unlinkPipeWirePorts(const QString &outputPort, const QString &inputPort);
    void setPipeWireModuleParameter(const QString &moduleName, const QString &parameter, const QString &value);
    void loadPipeWireModule(const QString &moduleName, const QJsonObject &parameters);
    void unloadPipeWireModule(const QString &moduleName);
    void restartPipeWireService();
    void reloadPipeWireConfiguration();
    void dumpPipeWireGraph();
    void optimizePipeWireForLatency();
    void optimizePipeWireForQuality();
    void optimizePipeWireForPowerSaving();
    
    // Data management
    void updateDeviceTable();
    void updateProfileTable();
    void updateEffectTable();
    void updateEasyEffectsPresetList();
    void updatePipeWireInfo();
    void updateMixerControls();
    void updateEffectChain();
    void updateInfoPanel();
    void parseDeviceList(const QString &output);
    void parseProfileList(const QString &output);
    void parseEasyEffectsPresetList(const QString &output);
    void parsePipeWireInfo(const QString &output);
    void parsePulseAudioDevices(const QString &output);
    void parsePipeWireDevices(const QString &output);
    void addDeviceToTable(const QString &name, const QString &description, 
                         const QString &state, const QString &system);
    void scanPipeWireDevicesAsync();
    void finishDeviceScan();
    QJsonObject parseDeviceInfo(const QString &line);
    QJsonObject parseProfileInfo(const QString &line);
    QJsonObject parseEffectInfo(const QString &line);
    
    // Helper methods
    void showProgress(const QString &title, const QString &message);
    void hideProgress();
    void showError(const QString &title, const QString &message);
    void showSuccess(const QString &title, const QString &message);
    void showInfo(const QString &title, const QString &message);
    QString formatVolume(int volume);
    QString formatLatency(int latency);
    QString formatSampleRate(int sampleRate);
    QString formatBufferSize(int bufferSize);
    QString getDeviceIcon(const QString &deviceType);
    QString getProfileIcon(const QString &profileType);
    QString getEffectIcon(const QString &effectType);
    QString getDeviceStatusColor(const QString &status);
    bool isEasyEffectsAvailable();
    bool isPipeWireAvailable();
    bool isPulseAudioAvailable();
    bool isAlsaAvailable();
    bool isJackAvailable();
    QString getAudioSystem();
    QStringList getAvailableAudioSystems();
    
    // Member variables
    SystemUtils *m_systemUtils;
    PrivilegedExecutor *m_privilegedExecutor;
    
    // UI components
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_toolbarLayout;
    QHBoxLayout *m_controlLayout;
    QTabWidget *m_tabWidget;
    
    // Device tab
    QWidget *m_deviceTab;
    QVBoxLayout *m_deviceLayout;
    QTableWidget *m_deviceTable;
    QHBoxLayout *m_deviceButtonLayout;
    QPushButton *m_setDefaultDeviceButton;
    QPushButton *m_testDeviceButton;
    QPushButton *m_propertiesDeviceButton;
    QPushButton *m_calibrateDeviceButton;
    QPushButton *m_suspendDeviceButton;
    QPushButton *m_resumeDeviceButton;
    QPushButton *m_resetDeviceButton;
    QPushButton *m_refreshDeviceButton;
    
    // Profile tab
    QWidget *m_profileTab;
    QVBoxLayout *m_profileLayout;
    QTableWidget *m_profileTable;
    QHBoxLayout *m_profileButtonLayout;
    QPushButton *m_loadProfileButton;
    QPushButton *m_saveProfileButton;
    QPushButton *m_createProfileButton;
    QPushButton *m_deleteProfileButton;
    QPushButton *m_duplicateProfileButton;
    QPushButton *m_importProfileButton;
    QPushButton *m_exportProfileButton;
    QPushButton *m_refreshProfileButton;
    
    // Effect tab
    QWidget *m_effectTab;
    QVBoxLayout *m_effectLayout;
    QTableWidget *m_effectTable;
    QHBoxLayout *m_effectButtonLayout;
    QPushButton *m_enableEffectButton;
    QPushButton *m_disableEffectButton;
    QPushButton *m_configureEffectButton;
    QPushButton *m_resetEffectButton;
    QPushButton *m_addEffectButton;
    QPushButton *m_removeEffectButton;
    QPushButton *m_refreshEffectButton;
    
    // EasyEffects tab
    QWidget *m_easyEffectsTab;
    QVBoxLayout *m_easyEffectsLayout;
    QListWidget *m_easyEffectsPresetList;
    QHBoxLayout *m_easyEffectsButtonLayout;
    QPushButton *m_installEasyEffectsButton;
    QPushButton *m_startEasyEffectsButton;
    QPushButton *m_stopEasyEffectsButton;
    QPushButton *m_autostartEasyEffectsButton;
    QPushButton *m_loadEasyEffectsPresetButton;
    QPushButton *m_saveEasyEffectsPresetButton;
    QPushButton *m_deleteEasyEffectsPresetButton;
    QPushButton *m_resetEasyEffectsPresetButton;
    QPushButton *m_refreshEasyEffectsButton;
    
    // PipeWire tab
    QWidget *m_pipeWireTab;
    QVBoxLayout *m_pipeWireLayout;
    QTextEdit *m_pipeWireInfoText;
    QHBoxLayout *m_pipeWireButtonLayout;
    QPushButton *m_installPipeWireButton;
    QPushButton *m_startPipeWireButton;
    QPushButton *m_stopPipeWireButton;
    QPushButton *m_restartPipeWireButton;
    QPushButton *m_autostartPipeWireButton;
    QPushButton *m_graphPipeWireButton;
    QPushButton *m_optimizeLatencyButton;
    QPushButton *m_optimizeQualityButton;
    QPushButton *m_optimizePowerButton;
    QPushButton *m_refreshPipeWireButton;
    
    // Control panel
    QGroupBox *m_controlPanel;
    QGridLayout *m_controlLayout2;
    QLabel *m_systemLabel;
    QComboBox *m_systemComboBox;
    
    // Simplified interface controls
    QSlider *m_masterVolumeSlider;
    QLabel *m_masterVolumeLabel;
    QSlider *m_inputVolumeSlider;
    QLabel *m_inputVolumeLabel;
    QComboBox *m_sampleRateCombo;
    QComboBox *m_bufferSizeCombo;
    
    // Mixer panel
    QGroupBox *m_mixerPanel;
    QHBoxLayout *m_mixerLayout;
    QScrollArea *m_mixerScrollArea;
    QWidget *m_mixerWidget;
    QHBoxLayout *m_mixerWidgetLayout;
    
    // Effect chain
    QGroupBox *m_effectChain;
    QHBoxLayout *m_effectChainLayout;
    QScrollArea *m_effectChainScrollArea;
    QWidget *m_effectChainWidget;
    QHBoxLayout *m_effectChainWidgetLayout;
    
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
    QMenu *m_deviceContextMenu;
    QMenu *m_profileContextMenu;
    QMenu *m_effectContextMenu;
    QMenu *m_easyEffectsContextMenu;
    QMenu *m_pipeWireContextMenu;
    
    // Background workers
    AudioDeviceWorker *m_deviceWorker;
    QTimer *m_refreshTimer;
    
    // Data
    QList<QJsonObject> m_devices;
    QList<QJsonObject> m_profiles;
    QList<QJsonObject> m_effects;
    QList<QJsonObject> m_easyEffectsPresets;
    QList<QJsonObject> m_cards;
         QJsonObject m_pipeWireInfo;
     QJsonObject m_currentProfile;
    
    // Settings
    bool m_autoRefresh;
    int m_refreshInterval;
    QString m_currentAudioSystem;
    QString m_currentOutputDevice;
    QString m_currentInputDevice;
    int m_masterVolume;
    bool m_masterMute;
    
    // State
    bool m_isScanning;
    QMutex m_dataMutex;
    
    // Constants
    static const int DEVICE_TABLE_NAME_COLUMN = 0;
    static const int DEVICE_TABLE_TYPE_COLUMN = 1;
    static const int DEVICE_TABLE_STATUS_COLUMN = 2;
    static const int DEVICE_TABLE_VOLUME_COLUMN = 3;
    static const int DEVICE_TABLE_CHANNELS_COLUMN = 4;
    static const int DEVICE_TABLE_SAMPLE_RATE_COLUMN = 5;
    static const int DEVICE_TABLE_LATENCY_COLUMN = 6;
    
    static const int PROFILE_TABLE_NAME_COLUMN = 0;
    static const int PROFILE_TABLE_TYPE_COLUMN = 1;
    static const int PROFILE_TABLE_DESCRIPTION_COLUMN = 2;
    static const int PROFILE_TABLE_ACTIVE_COLUMN = 3;
    
    static const int EFFECT_TABLE_NAME_COLUMN = 0;
    static const int EFFECT_TABLE_TYPE_COLUMN = 1;
    static const int EFFECT_TABLE_ENABLED_COLUMN = 2;
    static const int EFFECT_TABLE_PARAMETERS_COLUMN = 3;
};

#endif // AUDIOMANAGER_H 