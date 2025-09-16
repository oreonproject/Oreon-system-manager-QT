#ifndef CONTAINERMANAGER_H
#define CONTAINERMANAGER_H

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
#include <atomic>

class SystemUtils;
class PrivilegedExecutor;

class ContainerSearchWorker : public QThread
{
    Q_OBJECT
    
public:
    ContainerSearchWorker(const QString &containerType, QObject *parent = nullptr);
    void setParameters(const QString &searchTerm, const QString &containerType);
    void stop();
    
protected:
    void run() override;
    
private:
    void searchContainersAsync();
    void searchDockerImagesAsync();
    
    QString m_searchTerm;
    QString m_containerType;
    QMutex m_mutex;
    std::atomic<bool> m_stopRequested;
    
signals:
    void containerFound(const QJsonObject &containerInfo);
    void imageFound(const QJsonObject &imageInfo);
    void searchFinished();
    void errorOccurred(const QString &error);
    
};

class ContainerManager : public QWidget
{
    Q_OBJECT

public:
    explicit ContainerManager(QWidget *parent = nullptr);
    ~ContainerManager();
    
    void setSystemUtils(SystemUtils *utils);
    void setPrivilegedExecutor(PrivilegedExecutor *executor);
    
    // Public interface
    void refreshContainers();
    void refreshImages();
    void refreshDistroboxContainers();
    
public slots:
    void searchContainers();
    void searchImages();
    void onContainerActionTriggered();
    void onImageActionTriggered();
    void onDistroboxActionTriggered();
    
private slots:
    void onContainerFound(const QJsonObject &containerInfo);
    void onImageFound(const QJsonObject &imageInfo);
    void onSearchFinished();
    void onSearchError(const QString &error);
    void onContainerTableContextMenu(const QPoint &pos);
    void onImageTableContextMenu(const QPoint &pos);
    void onDistroboxTableContextMenu(const QPoint &pos);
    void onContainerSelectionChanged();
    void onImageSelectionChanged();
    void onDistroboxSelectionChanged();
    void onProgressUpdated(const QString &taskId, int progress, const QString &message);
    void onTaskCompleted(const QString &taskId, bool success, const QString &message);
    void onTaskFinished(int taskId, int exitCode, const QString &output);
    void onTaskProgress(int taskId, const QString &progress);
    void onTaskStarted(int taskId, const QString &description);
    void onTaskError(int taskId, const QString &error);
    void onRefreshTimer();
    void showCreateContainerDialog();
    void showCreateDistroboxDialog();
    void showContainerLogs();
    void showContainerInspect();
    void showImageInspect();
    void exportContainer();
    void importContainer();
    void pruneContainers();
    void pruneImages();
    void pullImage();
    void buildImage();
    void tagImage();
    void pushImage();
    void saveImage();
    void loadImage();
    
private:
    // UI setup
    void setupUI();
    void setupContainerTab();
    void setupImageTab();
    void setupDistroboxTab();
    void setupToolbar();
    void setupSearchBar();
    void setupProgressArea();
    void setupContextMenus();
    void updateTheme();
    
    // Container operations
    void startContainer(const QString &containerId);
    void stopContainer(const QString &containerId);
    void restartContainer(const QString &containerId);
    void removeContainer(const QString &containerId);
    void pauseContainer(const QString &containerId);
    void unpauseContainer(const QString &containerId);
    void killContainer(const QString &containerId);
    void renameContainer(const QString &containerId, const QString &newName);
    void executeInContainer(const QString &containerId, const QString &command);
    void attachToContainer(const QString &containerId);
    void copyFromContainer(const QString &containerId, const QString &srcPath, const QString &destPath);
    void copyToContainer(const QString &containerId, const QString &srcPath, const QString &destPath);
    void commitContainer(const QString &containerId, const QString &imageName);
    void createContainer(const QString &imageName, const QString &containerName, const QStringList &options);
    
    // Image operations
    void removeImage(const QString &imageId);
    void pullImageByName(const QString &imageName, const QString &tag = "latest");
    void buildImageFromDockerfile(const QString &dockerfilePath, const QString &imageName, const QString &tag);
    void tagImageWithName(const QString &imageId, const QString &repository, const QString &tag);
    void pushImageToRegistry(const QString &imageName, const QString &tag);
    void saveImageToFile(const QString &imageId, const QString &filePath);
    void loadImageFromFile(const QString &filePath);
    void exportImageToFile(const QString &imageId, const QString &filePath);
    void importImageFromFile(const QString &filePath, const QString &repository, const QString &tag);
    void historyImage(const QString &imageId);
    
    // Distrobox operations
    void createDistroboxContainer(const QString &name, const QString &image, const QStringList &options);
    void enterDistroboxContainer(const QString &name);
    void stopDistroboxContainer(const QString &name);
    void removeDistroboxContainer(const QString &name);
    void listDistroboxContainers();
    void upgradeDistroboxContainer(const QString &name);
    void generateDistroboxEntry(const QString &name, const QString &appName);
    
    // Data management
    void updateContainerTable();
    void updateImageTable();
    void updateDistroboxTable();
    void parseContainerList(const QString &output);
    void parseImageList(const QString &output);
    void parseDistroboxList(const QString &output);
    QJsonObject parseContainerJson(const QString &line);
    QJsonObject parseImageJson(const QString &line);
    
    // Helper methods
    void showProgress(const QString &title, const QString &message);
    void hideProgress();
    void showError(const QString &title, const QString &message);
    void showSuccess(const QString &title, const QString &message);
    void showInfo(const QString &title, const QString &message);
    QString formatSize(qint64 bytes);
    QString formatDuration(const QDateTime &started);
    QString getStatusColor(const QString &status);
    QString getContainerIcon(const QString &status);
    QString getImageIcon(const QString &type);
    bool isDockerAvailable();
    bool isDistroboxAvailable();
    bool isPodmanAvailable();
    
    // Member variables
    SystemUtils *m_systemUtils;
    PrivilegedExecutor *m_privilegedExecutor;
    
    // UI components
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_toolbarLayout;
    QHBoxLayout *m_searchLayout;
    QTabWidget *m_tabWidget;
    
    // Container tab
    QWidget *m_containerTab;
    QVBoxLayout *m_containerLayout;
    QTableWidget *m_containerTable;
    QHBoxLayout *m_containerButtonLayout;
    QPushButton *m_startContainerButton;
    QPushButton *m_stopContainerButton;
    QPushButton *m_restartContainerButton;
    QPushButton *m_removeContainerButton;
    QPushButton *m_createContainerButton;
    QPushButton *m_logsContainerButton;
    QPushButton *m_inspectContainerButton;
    QPushButton *m_attachContainerButton;
    QPushButton *m_execContainerButton;
    QPushButton *m_refreshContainerButton;
    
    // Image tab
    QWidget *m_imageTab;
    QVBoxLayout *m_imageLayout;
    QTableWidget *m_imageTable;
    QHBoxLayout *m_imageButtonLayout;
    QPushButton *m_pullImageButton;
    QPushButton *m_buildImageButton;
    QPushButton *m_removeImageButton;
    QPushButton *m_tagImageButton;
    QPushButton *m_pushImageButton;
    QPushButton *m_saveImageButton;
    QPushButton *m_loadImageButton;
    QPushButton *m_inspectImageButton;
    QPushButton *m_refreshImageButton;
    
    // Distrobox tab
    QWidget *m_distroboxTab;
    QVBoxLayout *m_distroboxLayout;
    QTableWidget *m_distroboxTable;
    QHBoxLayout *m_distroboxButtonLayout;
    QPushButton *m_createDistroboxButton;
    QPushButton *m_enterDistroboxButton;
    QPushButton *m_stopDistroboxButton;
    QPushButton *m_removeDistroboxButton;
    QPushButton *m_upgradeDistroboxButton;
    QPushButton *m_generateEntryButton;
    QPushButton *m_refreshDistroboxButton;
    
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
    QMenu *m_containerContextMenu;
    QMenu *m_imageContextMenu;
    QMenu *m_distroboxContextMenu;
    
    // Background workers
    ContainerSearchWorker *m_searchWorker;
    QTimer *m_refreshTimer;
    
    // Data
    QList<QJsonObject> m_containers;
    QList<QJsonObject> m_images;
    QList<QJsonObject> m_distroboxContainers;
    
    // Settings
    bool m_autoRefresh;
    int m_refreshInterval;
    QString m_defaultRuntime; // docker or podman
    
    // State
    bool m_isSearching;
    QMutex m_dataMutex;
    
    // Constants
    static const int CONTAINER_TABLE_ID_COLUMN = 0;
    static const int CONTAINER_TABLE_NAME_COLUMN = 1;
    static const int CONTAINER_TABLE_IMAGE_COLUMN = 2;
    static const int CONTAINER_TABLE_STATUS_COLUMN = 3;
    static const int CONTAINER_TABLE_CREATED_COLUMN = 4;
    static const int CONTAINER_TABLE_PORTS_COLUMN = 5;
    static const int CONTAINER_TABLE_SIZE_COLUMN = 6;
    
    static const int IMAGE_TABLE_ID_COLUMN = 0;
    static const int IMAGE_TABLE_REPOSITORY_COLUMN = 1;
    static const int IMAGE_TABLE_TAG_COLUMN = 2;
    static const int IMAGE_TABLE_SIZE_COLUMN = 3;
    static const int IMAGE_TABLE_CREATED_COLUMN = 4;
    
    static const int DISTROBOX_TABLE_NAME_COLUMN = 0;
    static const int DISTROBOX_TABLE_STATUS_COLUMN = 1;
    static const int DISTROBOX_TABLE_IMAGE_COLUMN = 2;
    static const int DISTROBOX_TABLE_PATH_COLUMN = 3;
};

#endif // CONTAINERMANAGER_H 