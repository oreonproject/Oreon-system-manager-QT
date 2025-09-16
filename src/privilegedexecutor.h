#ifndef PRIVILEGEDEXECUTOR_H
#define PRIVILEGEDEXECUTOR_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QTimer>
#include <QMutex>
#include <QQueue>

struct PrivilegedTask {
    QString command;
    QStringList args;
    QString description;
    int taskId;
    QObject *receiver;
    const char* successSlot;
    const char* errorSlot;
    const char* progressSlot;
};

class PrivilegedExecutor : public QObject
{
    Q_OBJECT

public:
    explicit PrivilegedExecutor(QObject *parent = nullptr);
    
    // Execute commands with elevated privileges
    int executeCommand(const QString &command, const QStringList &args = QStringList());
    void executeCommandAsync(const QString &command, const QStringList &args,
                           const QString &description, QObject *receiver,
                           const char* successSlot, const char* errorSlot,
                           const char* progressSlot = nullptr);
    
    // Package management operations
    void installPackages(const QStringList &packages, QObject *receiver,
                        const char* successSlot, const char* errorSlot,
                        const char* progressSlot = nullptr);
    void removePackages(const QStringList &packages, QObject *receiver,
                       const char* successSlot, const char* errorSlot,
                       const char* progressSlot = nullptr);
    void updatePackages(const QStringList &packages, QObject *receiver,
                       const char* successSlot, const char* errorSlot,
                       const char* progressSlot = nullptr);
    
    // Repository operations
    void enableRepository(const QString &repo, QObject *receiver,
                         const char* successSlot, const char* errorSlot);
    void disableRepository(const QString &repo, QObject *receiver,
                          const char* successSlot, const char* errorSlot);
    void addRepository(const QString &repoUrl, const QString &repoName,
                      QObject *receiver, const char* successSlot, const char* errorSlot);
    
    // System operations
    void enableSystemdService(const QString &service, QObject *receiver,
                             const char* successSlot, const char* errorSlot);
    void disableSystemdService(const QString &service, QObject *receiver,
                              const char* successSlot, const char* errorSlot);
    void restartSystemdService(const QString &service, QObject *receiver,
                              const char* successSlot, const char* errorSlot);
    
    // File operations
    void writeSystemFile(const QString &path, const QString &content,
                        QObject *receiver, const char* successSlot, const char* errorSlot);
    void deleteSystemFile(const QString &path, QObject *receiver,
                         const char* successSlot, const char* errorSlot);
    
    // Task management
    void cancelTask(int taskId);
    void cancelAllTasks();
    bool isTaskRunning(int taskId);
    QStringList getRunningTasks();
    
    // Utility methods
    static bool isPkexecAvailable();
    static bool isSudoAvailable();
    static QString getPrivilegeMethod();

signals:
    void taskStarted(int taskId, const QString &description);
    void taskFinished(int taskId, int exitCode, const QString &output);
    void taskError(int taskId, const QString &error);
    void taskProgress(int taskId, const QString &progress);
    void taskCancelled(int taskId);

private slots:
    void onProcessStarted();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onProcessError(QProcess::ProcessError error);
    void onProcessReadyRead();
    void processNextTask();

private:
    void enqueueTask(const PrivilegedTask &task);
    QString buildCommand(const QString &command, const QStringList &args);
    void startTask(const PrivilegedTask &task);
    void finishCurrentTask(int exitCode, const QString &output);
    void errorCurrentTask(const QString &error);
    
    QQueue<PrivilegedTask> m_taskQueue;
    QProcess *m_currentProcess;
    PrivilegedTask m_currentTask;
    bool m_isRunning;
    int m_nextTaskId;
    QMutex m_taskMutex;
    QTimer *m_processTimer;
    
    static QString s_privilegeMethod;
};

#endif // PRIVILEGEDEXECUTOR_H 