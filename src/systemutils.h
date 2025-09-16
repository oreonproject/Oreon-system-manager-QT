#ifndef SYSTEMUTILS_H
#define SYSTEMUTILS_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class SystemUtils : public QObject
{
    Q_OBJECT

public:
    explicit SystemUtils(QObject *parent = nullptr);
    
    // System information
    static QString getDistroName();
    static QString getKernelVersion();
    static QString getDesktopEnvironment();
    static bool isSystemdAvailable();
    static bool isFlatpakAvailable();
    static bool isDockerAvailable();
    static bool isDistroboxAvailable();
    
    // Process management
    static QProcess* createProcess(QObject *parent = nullptr);
    static QPair<int, QString> runCommand(const QString &command, const QStringList &args = QStringList());
    static void runCommandAsync(const QString &command, const QStringList &args, QObject *receiver, const char* slot);
    
    // File system utilities
    static bool fileExists(const QString &path);
    static bool isWritable(const QString &path);
    static QString readFile(const QString &path);
    static bool writeFile(const QString &path, const QString &content);
    static QStringList listFiles(const QString &directory, const QString &pattern = "*");
    
    // Network utilities
    static bool isOnline();
    static QString downloadString(const QString &url);
    
    // Package management helpers
    static bool isDnfAvailable();
    static bool isYumAvailable();
    static QStringList getEnabledRepos();
    static QStringList getAvailableRepos();
    
    // Container utilities
    static QStringList getDockerContainers();
    static QStringList getDistroboxContainers();
    static bool isContainerRunning(const QString &containerName);
    
    // Audio utilities
    static bool isPipeWireRunning();
    static bool isEasyEffectsInstalled();
    static QStringList getAudioDevices();
    
    // Driver utilities
    static QStringList getLoadedKernelModules();
    static QStringList getAvailableDrivers();
    static bool isNvidiaDriverInstalled();
    static bool isAmdDriverInstalled();

signals:
    void commandFinished(int exitCode, const QString &output);
    void commandError(const QString &error);

private slots:
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onProcessError(QProcess::ProcessError error);

private:
    static QNetworkAccessManager *networkManager;
};

#endif // SYSTEMUTILS_H 