#include "privilegedexecutor.h"
#include "systemutils.h"
#include <QDebug>
#include <QTemporaryFile>
#include <QStandardPaths>
#include <QDir>

QString PrivilegedExecutor::s_privilegeMethod;

PrivilegedExecutor::PrivilegedExecutor(QObject *parent)
    : QObject(parent)
    , m_currentProcess(nullptr)
    , m_isRunning(false)
    , m_nextTaskId(1)
    , m_processTimer(new QTimer(this))
{
    m_processTimer->setSingleShot(true);
    m_processTimer->setInterval(60000); // 1 minute timeout
    connect(m_processTimer, &QTimer::timeout, this, &PrivilegedExecutor::processNextTask);
}

int PrivilegedExecutor::executeCommand(const QString &command, const QStringList &args)
{
    QString privilegeMethod = getPrivilegeMethod();
    if (privilegeMethod.isEmpty()) {
        qWarning() << "No privilege escalation method available";
        return -1;
    }
    
    QStringList fullArgs;
    if (privilegeMethod == "pkexec") {
        fullArgs << command << args;
    } else if (privilegeMethod == "sudo") {
        fullArgs << "-n" << command << args;
    }
    
    auto result = SystemUtils::runCommand(privilegeMethod, fullArgs);
    return result.first;
}

void PrivilegedExecutor::executeCommandAsync(const QString &command, const QStringList &args,
                                           const QString &description, QObject *receiver,
                                           const char* successSlot, const char* errorSlot,
                                           const char* progressSlot)
{
    PrivilegedTask task;
    task.command = command;
    task.args = args;
    task.description = description;
    task.taskId = m_nextTaskId++;
    task.receiver = receiver;
    task.successSlot = successSlot;
    task.errorSlot = errorSlot;
    task.progressSlot = progressSlot;
    
    enqueueTask(task);
}

void PrivilegedExecutor::installPackages(const QStringList &packages, QObject *receiver,
                                        const char* successSlot, const char* errorSlot,
                                        const char* progressSlot)
{
    QString packageList = packages.join(" ");
    QString description = QString("Installing packages: %1").arg(packageList);
    
    QStringList args;
    args << "install" << "-y" << packages;
    
    executeCommandAsync("dnf", args, description, receiver, successSlot, errorSlot, progressSlot);
}

void PrivilegedExecutor::removePackages(const QStringList &packages, QObject *receiver,
                                       const char* successSlot, const char* errorSlot,
                                       const char* progressSlot)
{
    QString packageList = packages.join(" ");
    QString description = QString("Removing packages: %1").arg(packageList);
    
    QStringList args;
    args << "remove" << "-y" << packages;
    
    executeCommandAsync("dnf", args, description, receiver, successSlot, errorSlot, progressSlot);
}

void PrivilegedExecutor::updatePackages(const QStringList &packages, QObject *receiver,
                                       const char* successSlot, const char* errorSlot,
                                       const char* progressSlot)
{
    QString description;
    QStringList args;
    
    if (packages.isEmpty()) {
        description = "Updating all packages";
        args << "update" << "-y";
    } else {
        QString packageList = packages.join(" ");
        description = QString("Updating packages: %1").arg(packageList);
        args << "update" << "-y" << packages;
    }
    
    executeCommandAsync("dnf", args, description, receiver, successSlot, errorSlot, progressSlot);
}

void PrivilegedExecutor::enableRepository(const QString &repo, QObject *receiver,
                                         const char* successSlot, const char* errorSlot)
{
    QString description = QString("Enabling repository: %1").arg(repo);
    QStringList args;
    args << "config-manager" << "--enable" << repo;
    
    executeCommandAsync("dnf", args, description, receiver, successSlot, errorSlot);
}

void PrivilegedExecutor::disableRepository(const QString &repo, QObject *receiver,
                                          const char* successSlot, const char* errorSlot)
{
    QString description = QString("Disabling repository: %1").arg(repo);
    QStringList args;
    args << "config-manager" << "--disable" << repo;
    
    executeCommandAsync("dnf", args, description, receiver, successSlot, errorSlot);
}

void PrivilegedExecutor::addRepository(const QString &repoUrl, const QString &repoName,
                                      QObject *receiver, const char* successSlot, const char* errorSlot)
{
    QString description = QString("Adding repository: %1").arg(repoName);
    QStringList args;
    args << "config-manager" << "--add-repo" << repoUrl;
    
    executeCommandAsync("dnf", args, description, receiver, successSlot, errorSlot);
}

void PrivilegedExecutor::enableSystemdService(const QString &service, QObject *receiver,
                                             const char* successSlot, const char* errorSlot)
{
    QString description = QString("Enabling service: %1").arg(service);
    QStringList args;
    args << "enable" << service;
    
    executeCommandAsync("systemctl", args, description, receiver, successSlot, errorSlot);
}

void PrivilegedExecutor::disableSystemdService(const QString &service, QObject *receiver,
                                              const char* successSlot, const char* errorSlot)
{
    QString description = QString("Disabling service: %1").arg(service);
    QStringList args;
    args << "disable" << service;
    
    executeCommandAsync("systemctl", args, description, receiver, successSlot, errorSlot);
}

void PrivilegedExecutor::restartSystemdService(const QString &service, QObject *receiver,
                                              const char* successSlot, const char* errorSlot)
{
    QString description = QString("Restarting service: %1").arg(service);
    QStringList args;
    args << "restart" << service;
    
    executeCommandAsync("systemctl", args, description, receiver, successSlot, errorSlot);
}

void PrivilegedExecutor::writeSystemFile(const QString &path, const QString &content,
                                        QObject *receiver, const char* successSlot, const char* errorSlot)
{
    QString description = QString("Writing system file: %1").arg(path);
    
    // Create a temporary file with the content
    QTemporaryFile tempFile;
    if (!tempFile.open()) {
        if (receiver && errorSlot) {
            QMetaObject::invokeMethod(receiver, errorSlot, Q_ARG(QString, "Failed to create temporary file"));
        }
        return;
    }
    
    tempFile.write(content.toUtf8());
    tempFile.close();
    
    QStringList args;
    args << tempFile.fileName() << path;
    
    executeCommandAsync("cp", args, description, receiver, successSlot, errorSlot);
}

void PrivilegedExecutor::deleteSystemFile(const QString &path, QObject *receiver,
                                         const char* successSlot, const char* errorSlot)
{
    QString description = QString("Deleting system file: %1").arg(path);
    QStringList args;
    args << path;
    
    executeCommandAsync("rm", args, description, receiver, successSlot, errorSlot);
}

void PrivilegedExecutor::cancelTask(int taskId)
{
    QMutexLocker locker(&m_taskMutex);
    
    if (m_isRunning && m_currentTask.taskId == taskId) {
        if (m_currentProcess) {
            m_currentProcess->kill();
        }
        emit taskCancelled(taskId);
        return;
    }
    
    // Remove from queue
    for (int i = 0; i < m_taskQueue.size(); ++i) {
        if (m_taskQueue[i].taskId == taskId) {
            m_taskQueue.removeAt(i);
            emit taskCancelled(taskId);
            break;
        }
    }
}

void PrivilegedExecutor::cancelAllTasks()
{
    QMutexLocker locker(&m_taskMutex);
    
    if (m_isRunning && m_currentProcess) {
        m_currentProcess->kill();
        emit taskCancelled(m_currentTask.taskId);
    }
    
    for (const auto &task : m_taskQueue) {
        emit taskCancelled(task.taskId);
    }
    
    m_taskQueue.clear();
}

bool PrivilegedExecutor::isTaskRunning(int taskId)
{
    QMutexLocker locker(&m_taskMutex);
    return m_isRunning && m_currentTask.taskId == taskId;
}

QStringList PrivilegedExecutor::getRunningTasks()
{
    QMutexLocker locker(&m_taskMutex);
    QStringList tasks;
    
    if (m_isRunning) {
        tasks.append(m_currentTask.description);
    }
    
    for (const auto &task : m_taskQueue) {
        tasks.append(task.description);
    }
    
    return tasks;
}

bool PrivilegedExecutor::isPkexecAvailable()
{
    auto result = SystemUtils::runCommand("which", {"pkexec"});
    return result.first == 0;
}

bool PrivilegedExecutor::isSudoAvailable()
{
    auto result = SystemUtils::runCommand("which", {"sudo"});
    return result.first == 0;
}

QString PrivilegedExecutor::getPrivilegeMethod()
{
    if (!s_privilegeMethod.isEmpty()) {
        return s_privilegeMethod;
    }
    
    if (isPkexecAvailable()) {
        s_privilegeMethod = "pkexec";
    } else if (isSudoAvailable()) {
        s_privilegeMethod = "sudo";
    }
    
    return s_privilegeMethod;
}

void PrivilegedExecutor::onProcessStarted()
{
    emit taskStarted(m_currentTask.taskId, m_currentTask.description);
}

void PrivilegedExecutor::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QString output = m_currentProcess->readAllStandardOutput();
    finishCurrentTask(exitCode, output);
}

void PrivilegedExecutor::onProcessError(QProcess::ProcessError error)
{
    QString errorString = m_currentProcess->errorString();
    errorCurrentTask(errorString);
}

void PrivilegedExecutor::onProcessReadyRead()
{
    if (!m_currentProcess) return;
    
    QString output = m_currentProcess->readAllStandardOutput();
    if (!output.isEmpty()) {
        // Emit the signal for lambda connections
        emit taskProgress(m_currentTask.taskId, output.trimmed());
        
        // Also call the old SLOT mechanism if provided
        if (m_currentTask.progressSlot && m_currentTask.receiver) {
            QMetaObject::invokeMethod(m_currentTask.receiver, m_currentTask.progressSlot,
                                    Q_ARG(QString, output.trimmed()));
        }
    }
}

void PrivilegedExecutor::processNextTask()
{
    QMutexLocker locker(&m_taskMutex);
    
    if (m_isRunning || m_taskQueue.isEmpty()) {
        return;
    }
    
    PrivilegedTask task = m_taskQueue.dequeue();
    startTask(task);
}

void PrivilegedExecutor::enqueueTask(const PrivilegedTask &task)
{
    QMutexLocker locker(&m_taskMutex);
    m_taskQueue.enqueue(task);
    
    if (!m_isRunning) {
        QMetaObject::invokeMethod(this, "processNextTask", Qt::QueuedConnection);
    }
}

QString PrivilegedExecutor::buildCommand(const QString &command, const QStringList &args)
{
    QString fullCommand = command;
    for (const QString &arg : args) {
        fullCommand += " " + arg;
    }
    return fullCommand;
}

void PrivilegedExecutor::startTask(const PrivilegedTask &task)
{
    m_currentTask = task;
    m_isRunning = true;
    
    QString privilegeMethod = getPrivilegeMethod();
    if (privilegeMethod.isEmpty()) {
        errorCurrentTask("No privilege escalation method available");
        return;
    }
    
    m_currentProcess = new QProcess(this);
    m_currentProcess->setProcessChannelMode(QProcess::MergedChannels);
    
    connect(m_currentProcess, &QProcess::started, this, &PrivilegedExecutor::onProcessStarted);
    connect(m_currentProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &PrivilegedExecutor::onProcessFinished);
    connect(m_currentProcess, &QProcess::errorOccurred, this, &PrivilegedExecutor::onProcessError);
    connect(m_currentProcess, &QProcess::readyReadStandardOutput, this, &PrivilegedExecutor::onProcessReadyRead);
    
    QStringList fullArgs;
    if (privilegeMethod == "pkexec") {
        fullArgs << task.command << task.args;
    } else if (privilegeMethod == "sudo") {
        fullArgs << "-n" << task.command << task.args;
    }
    
    m_processTimer->start();
    m_currentProcess->start(privilegeMethod, fullArgs);
}

void PrivilegedExecutor::finishCurrentTask(int exitCode, const QString &output)
{
    m_processTimer->stop();
    
    if (m_currentTask.receiver) {
        if (exitCode == 0 && m_currentTask.successSlot) {
            QMetaObject::invokeMethod(m_currentTask.receiver, m_currentTask.successSlot,
                                    Q_ARG(QString, output));
        } else if (exitCode != 0 && m_currentTask.errorSlot) {
            QMetaObject::invokeMethod(m_currentTask.receiver, m_currentTask.errorSlot,
                                    Q_ARG(QString, output));
        }
    }
    
    emit taskFinished(m_currentTask.taskId, exitCode, output);
    
    if (m_currentProcess) {
        m_currentProcess->deleteLater();
        m_currentProcess = nullptr;
    }
    
    m_isRunning = false;
    QMetaObject::invokeMethod(this, "processNextTask", Qt::QueuedConnection);
}

void PrivilegedExecutor::errorCurrentTask(const QString &error)
{
    m_processTimer->stop();
    
    if (m_currentTask.receiver && m_currentTask.errorSlot) {
        QMetaObject::invokeMethod(m_currentTask.receiver, m_currentTask.errorSlot,
                                Q_ARG(QString, error));
    }
    
    emit taskError(m_currentTask.taskId, error);
    
    if (m_currentProcess) {
        m_currentProcess->deleteLater();
        m_currentProcess = nullptr;
    }
    
    m_isRunning = false;
    QMetaObject::invokeMethod(this, "processNextTask", Qt::QueuedConnection);
} 