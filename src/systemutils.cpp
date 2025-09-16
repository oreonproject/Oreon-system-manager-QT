#include "systemutils.h"
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QTextStream>
#include <QRegularExpression>
#include <QDebug>
#include <QProcessEnvironment>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QApplication>

QNetworkAccessManager *SystemUtils::networkManager = nullptr;

SystemUtils::SystemUtils(QObject *parent)
    : QObject(parent)
{
    if (!networkManager) {
        networkManager = new QNetworkAccessManager(this);
    }
}

QString SystemUtils::getDistroName()
{
    QString osRelease = readFile("/etc/os-release");
    QRegularExpression re("NAME=\"([^\"]+)\"");
    QRegularExpressionMatch match = re.match(osRelease);
    if (match.hasMatch()) {
        return match.captured(1);
    }
    return "Unknown Linux";
}

QString SystemUtils::getKernelVersion()
{
    auto result = runCommand("uname", {"-r"});
    return result.second.trimmed();
}

QString SystemUtils::getDesktopEnvironment()
{
    QString desktop = QProcessEnvironment::systemEnvironment().value("XDG_CURRENT_DESKTOP");
    if (desktop.isEmpty()) {
        desktop = QProcessEnvironment::systemEnvironment().value("DESKTOP_SESSION");
    }
    return desktop;
}

bool SystemUtils::isSystemdAvailable()
{
    return fileExists("/run/systemd/system");
}

bool SystemUtils::isFlatpakAvailable()
{
    auto result = runCommand("which", {"flatpak"});
    return result.first == 0;
}

bool SystemUtils::isDockerAvailable()
{
    auto result = runCommand("which", {"docker"});
    return result.first == 0;
}

bool SystemUtils::isDistroboxAvailable()
{
    auto result = runCommand("which", {"distrobox"});
    return result.first == 0;
}

QProcess* SystemUtils::createProcess(QObject *parent)
{
    QProcess *process = new QProcess(parent);
    process->setProcessChannelMode(QProcess::MergedChannels);
    return process;
}

QPair<int, QString> SystemUtils::runCommand(const QString &command, const QStringList &args)
{
    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);
    process.start(command, args);
    
    if (!process.waitForStarted(2000)) {
        return {-1, "Failed to start process"};
    }
    
    if (!process.waitForFinished(5000)) {
        process.kill();
        return {-1, "Process timed out"};
    }
    
    QString output = process.readAllStandardOutput();
    return {process.exitCode(), output};
}

void SystemUtils::runCommandAsync(const QString &command, const QStringList &args, QObject *receiver, const char* slot)
{
    QProcess *process = new QProcess(receiver);
    process->setProcessChannelMode(QProcess::MergedChannels);
    
    QObject::connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                     [receiver, slot](int exitCode, QProcess::ExitStatus status) {
                         QMetaObject::invokeMethod(receiver, slot, Q_ARG(int, exitCode), Q_ARG(int, static_cast<int>(status)));
                     });
    
    process->start(command, args);
}

bool SystemUtils::fileExists(const QString &path)
{
    return QFile::exists(path);
}

bool SystemUtils::isWritable(const QString &path)
{
    QFile file(path);
    return file.open(QIODevice::WriteOnly);
}

QString SystemUtils::readFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QString();
    }
    
    QTextStream stream(&file);
    return stream.readAll();
}

bool SystemUtils::writeFile(const QString &path, const QString &content)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    
    QTextStream stream(&file);
    stream << content;
    return true;
}

QStringList SystemUtils::listFiles(const QString &directory, const QString &pattern)
{
    QDir dir(directory);
    return dir.entryList(QStringList() << pattern, QDir::Files);
}

bool SystemUtils::isOnline()
{
    auto result = runCommand("ping", {"-c", "1", "-W", "3", "8.8.8.8"});
    return result.first == 0;
}

QString SystemUtils::downloadString(const QString &url)
{
    if (!networkManager) {
        networkManager = new QNetworkAccessManager();
    }
    
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Oreon System Manager/1.0");
    
    QNetworkReply *reply = networkManager->get(request);
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    
    QString result = reply->readAll();
    reply->deleteLater();
    return result;
}

bool SystemUtils::isDnfAvailable()
{
    auto result = runCommand("which", {"dnf"});
    return result.first == 0;
}

bool SystemUtils::isYumAvailable()
{
    auto result = runCommand("which", {"yum"});
    return result.first == 0;
}

QStringList SystemUtils::getEnabledRepos()
{
    auto result = runCommand("dnf", {"repolist", "enabled", "--quiet"});
    if (result.first != 0) {
        return QStringList();
    }
    
    QStringList repos;
    QStringList lines = result.second.split('\n');
    for (const QString &line : lines) {
        if (line.contains(' ')) {
            QString repo = line.split(' ').first();
            if (!repo.isEmpty() && repo != "repo") {
                repos.append(repo);
            }
        }
    }
    return repos;
}

QStringList SystemUtils::getAvailableRepos()
{
    auto result = runCommand("dnf", {"repolist", "all", "--quiet"});
    if (result.first != 0) {
        return QStringList();
    }
    
    QStringList repos;
    QStringList lines = result.second.split('\n');
    for (const QString &line : lines) {
        if (line.contains(' ')) {
            QString repo = line.split(' ').first();
            if (!repo.isEmpty() && repo != "repo") {
                repos.append(repo);
            }
        }
    }
    return repos;
}

QStringList SystemUtils::getDockerContainers()
{
    auto result = runCommand("docker", {"ps", "-a", "--format", "{{.Names}}"});
    if (result.first != 0) {
        return QStringList();
    }
    
    return result.second.split('\n', Qt::SkipEmptyParts);
}

QStringList SystemUtils::getDistroboxContainers()
{
    auto result = runCommand("distrobox", {"list", "--no-color"});
    if (result.first != 0) {
        return QStringList();
    }
    
    QStringList containers;
    QStringList lines = result.second.split('\n');
    for (const QString &line : lines) {
        if (line.contains('|')) {
            QString container = line.split('|').first().trimmed();
            if (!container.isEmpty() && container != "NAME") {
                containers.append(container);
            }
        }
    }
    return containers;
}

bool SystemUtils::isContainerRunning(const QString &containerName)
{
    auto result = runCommand("docker", {"ps", "--format", "{{.Names}}"});
    if (result.first != 0) {
        return false;
    }
    
    return result.second.contains(containerName);
}

bool SystemUtils::isPipeWireRunning()
{
    auto result = runCommand("systemctl", {"--user", "is-active", "pipewire"});
    return result.first == 0 && result.second.trimmed() == "active";
}

bool SystemUtils::isEasyEffectsInstalled()
{
    auto result = runCommand("which", {"easyeffects"});
    return result.first == 0;
}

QStringList SystemUtils::getAudioDevices()
{
    auto result = runCommand("pactl", {"list", "short", "sinks"});
    if (result.first != 0) {
        return QStringList();
    }
    
    QStringList devices;
    QStringList lines = result.second.split('\n');
    for (const QString &line : lines) {
        if (!line.isEmpty()) {
            QStringList parts = line.split('\t');
            if (parts.size() >= 2) {
                devices.append(parts[1]);
            }
        }
    }
    return devices;
}

QStringList SystemUtils::getLoadedKernelModules()
{
    auto result = runCommand("lsmod", {});
    if (result.first != 0) {
        return QStringList();
    }
    
    QStringList modules;
    QStringList lines = result.second.split('\n');
    for (const QString &line : lines) {
        if (!line.isEmpty()) {
            QString module = line.split(' ').first();
            if (!module.isEmpty() && module != "Module") {
                modules.append(module);
            }
        }
    }
    return modules;
}

QStringList SystemUtils::getAvailableDrivers()
{
    QStringList drivers;
    
    // Check for NVIDIA drivers
    auto nvidiaResult = runCommand("dnf", {"list", "available", "*nvidia*", "--quiet"});
    if (nvidiaResult.first == 0) {
        QStringList lines = nvidiaResult.second.split('\n');
        for (const QString &line : lines) {
            if (line.contains("nvidia")) {
                QString driver = line.split(' ').first();
                if (!driver.isEmpty()) {
                    drivers.append(driver);
                }
            }
        }
    }
    
    // Check for AMD drivers
    auto amdResult = runCommand("dnf", {"list", "available", "*amd*", "--quiet"});
    if (amdResult.first == 0) {
        QStringList lines = amdResult.second.split('\n');
        for (const QString &line : lines) {
            if (line.contains("amd")) {
                QString driver = line.split(' ').first();
                if (!driver.isEmpty()) {
                    drivers.append(driver);
                }
            }
        }
    }
    
    return drivers;
}

bool SystemUtils::isNvidiaDriverInstalled()
{
    auto result = runCommand("lsmod", {});
    return result.second.contains("nvidia");
}

bool SystemUtils::isAmdDriverInstalled()
{
    auto result = runCommand("lsmod", {});
    return result.second.contains("amdgpu");
}

void SystemUtils::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QProcess *process = qobject_cast<QProcess*>(sender());
    if (!process) return;
    
    QString output = process->readAllStandardOutput();
    emit commandFinished(exitCode, output);
    
    process->deleteLater();
}

void SystemUtils::onProcessError(QProcess::ProcessError error)
{
    QProcess *process = qobject_cast<QProcess*>(sender());
    if (!process) return;
    
    QString errorString = process->errorString();
    emit commandError(errorString);
    
    process->deleteLater();
} 