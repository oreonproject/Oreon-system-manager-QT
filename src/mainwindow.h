#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QStatusBar>
#include <QMenuBar>
// System tray removed
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QTimer>
#include <QCloseEvent>
// Settings removed

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QHBoxLayout;
class QSplitter;
class QTextEdit;
class QProgressBar;
QT_END_NAMESPACE

class PackageManager;
class RepositoryManager;
class ContainerManager;
class AudioManager;
class DriverManager;
class SystemUtils;
class PrivilegedExecutor;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;
    void changeEvent(QEvent *event) override;

private slots:
    void aboutApplication();
    void onTabChanged(int index);
    void onTaskStarted(int taskId, const QString &description);
    void onTaskFinished(int taskId, int exitCode, const QString &output);
    void onTaskError(int taskId, const QString &error);
    void onTaskProgress(int taskId, const QString &progress);

private:
    void createUI();
    void createMenuBar();
    void createStatusBar();
    void createTabs();
    void setupConnections();
    void updateSystemInfo();
    void setupKDEIntegration();
    void applyTheme();
    
    // UI components
    QTabWidget *m_tabWidget;
    QProgressBar *m_progressBar;
    QLabel *m_statusLabel;
    QLabel *m_systemInfoLabel;
    
    // System tray removed
    
    // Menu actions
    QAction *m_aboutAppAction;
    QAction *m_quitAppAction;
    
    // Tab managers
    PackageManager *m_packageManager;
    RepositoryManager *m_repositoryManager;
    ContainerManager *m_containerManager;
    AudioManager *m_audioManager;
    
    // System components
    SystemUtils *m_systemUtils;
    PrivilegedExecutor *m_privilegedExecutor;
    
    // State
    QString m_currentTask;
    int m_currentTaskId;
};

#endif // MAINWINDOW_H 