#include "mainwindow.h"
#include "packagemanager.h"
#include "repositorymanager.h"
#include "containermanager.h"
#include "audiomanager.h"
#include "systemutils.h"
#include "privilegedexecutor.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QTextEdit>
#include <QProgressBar>
#include <QLabel>
#include <QMessageBox>
#include <QMenuBar>
#include <QStatusBar>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QTimer>
#include <QCloseEvent>
#include <QSettings>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QUrl>
#include <QStyle>
#include <QStyleHints>
#include <QScreen>
#include <QScreen>
#include <QDateTime>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_tabWidget(nullptr)
    , m_progressBar(nullptr)
    , m_statusLabel(nullptr)
    , m_systemInfoLabel(nullptr)
    // System tray removed
    , m_packageManager(nullptr)
    , m_repositoryManager(nullptr)
    , m_containerManager(nullptr)
    , m_audioManager(nullptr)
    , m_systemUtils(nullptr)
    , m_privilegedExecutor(nullptr)
    , m_currentTaskId(-1)
{
    setObjectName("MainWindow");
    setWindowTitle("Oreon System Manager");
    setWindowIcon(QIcon::fromTheme("preferences-desktop-personal"));
    
    // Initialize settings
    // Initialize system components
    m_systemUtils = new SystemUtils(this);
    m_privilegedExecutor = new PrivilegedExecutor(this);
    
    // Create UI
    createUI();
    createMenuBar();
    createStatusBar();
    createTabs();
    setupConnections();
    
    // Apply theme and KDE integration
    setupKDEIntegration();
    applyTheme();
    
    // Show startup message
    m_statusLabel->setText("Oreon System Manager ready");
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void MainWindow::changeEvent(QEvent *event)
{
    QMainWindow::changeEvent(event);
}

// System tray methods removed

void MainWindow::aboutApplication()
{
    QMessageBox::about(this, "About Oreon System Manager",
                      "<h3>Oreon System Manager</h3>"
                      "<p>Version 1.0.0</p>"
                      "<p>Advanced Linux System Management Tool</p>"
                      "<p>Features:</p>"
                      "<ul>"
                      "<li>DNF Package Management</li>"
                      "<li>Repository Management</li>"
                      "<li>Container Management (Docker & Distrobox)</li>"
                      "<li>Pro Audio Setup</li>"
                      "</ul>"
                      "<p>Built with Qt6 and designed for KDE Plasma</p>");
}

// Preferences and system tray methods removed

void MainWindow::onTabChanged(int index)
{
    if (m_tabWidget && index >= 0 && index < m_tabWidget->count()) {
        QString tabName = m_tabWidget->tabText(index);
        m_statusLabel->setText(QString("Switched to %1 tab").arg(tabName));
    }
}

void MainWindow::onTaskStarted(int taskId, const QString &description)
{
    m_currentTaskId = taskId;
    m_currentTask = description;
    m_statusLabel->setText(QString("Running: %1").arg(description));
    m_progressBar->setVisible(true);
    m_progressBar->setRange(0, 0); // Indeterminate progress
}

void MainWindow::onTaskFinished(int taskId, int exitCode, const QString &output)
{
    if (taskId == m_currentTaskId) {
        m_progressBar->setVisible(false);
        if (exitCode == 0) {
            m_statusLabel->setText(QString("Completed: %1").arg(m_currentTask));
        } else {
            m_statusLabel->setText(QString("Failed: %1").arg(m_currentTask));
        }
        
        m_currentTaskId = -1;
        m_currentTask.clear();
    }
}

void MainWindow::onTaskError(int taskId, const QString &error)
{
    if (taskId == m_currentTaskId) {
        m_progressBar->setVisible(false);
        m_statusLabel->setText(QString("Error: %1").arg(m_currentTask));
        
        m_currentTaskId = -1;
        m_currentTask.clear();
    }
}

void MainWindow::onTaskProgress(int taskId, const QString &progress)
{
    if (taskId == m_currentTaskId && !progress.isEmpty()) {
        // Progress updates without logging
        m_statusLabel->setText(QString("Progress: %1").arg(progress));
    }
}

void MainWindow::createUI()
{
    // Simple, clean UI without logs or splitter
    m_tabWidget = new QTabWidget(this);
    m_tabWidget->setTabPosition(QTabWidget::North);
    m_tabWidget->setTabsClosable(false);
    m_tabWidget->setMovable(false);
    m_tabWidget->setDocumentMode(true);
    
    setCentralWidget(m_tabWidget);
    
    setMinimumSize(1000, 700);
    resize(1200, 800);
}

void MainWindow::createMenuBar()
{
    QMenu *fileMenu = menuBar()->addMenu("&File");
    
    m_quitAppAction = new QAction(QIcon::fromTheme("application-exit"), "&Quit", this);
    m_quitAppAction->setShortcut(QKeySequence::Quit);
    m_quitAppAction->setStatusTip("Exit application");
    connect(m_quitAppAction, &QAction::triggered, qApp, &QApplication::quit);
    fileMenu->addAction(m_quitAppAction);
    
    QMenu *helpMenu = menuBar()->addMenu("&Help");
    
    m_aboutAppAction = new QAction(QIcon::fromTheme("help-about"), "&About", this);
    m_aboutAppAction->setStatusTip("About Oreon System Manager");
    connect(m_aboutAppAction, &QAction::triggered, this, &MainWindow::aboutApplication);
    helpMenu->addAction(m_aboutAppAction);
}

void MainWindow::createStatusBar()
{
    m_statusLabel = new QLabel("Ready", this);
    statusBar()->addWidget(m_statusLabel, 1);
    
    m_progressBar = new QProgressBar(this);
    m_progressBar->setVisible(false);
    m_progressBar->setMaximumWidth(200);
    statusBar()->addPermanentWidget(m_progressBar);
    
    m_systemInfoLabel = new QLabel(this);
    statusBar()->addPermanentWidget(m_systemInfoLabel);
}

// System tray functionality removed

void MainWindow::createTabs()
{
    // Create all manager tabs with full functionality
    m_packageManager = new PackageManager(this);
    m_packageManager->setSystemUtils(m_systemUtils);
    m_packageManager->setPrivilegedExecutor(m_privilegedExecutor);
    m_tabWidget->addTab(m_packageManager, QIcon::fromTheme("system-software-install"), "Packages");
    
    m_repositoryManager = new RepositoryManager(this);
    m_repositoryManager->setSystemUtils(m_systemUtils);
    m_repositoryManager->setPrivilegedExecutor(m_privilegedExecutor);
    m_tabWidget->addTab(m_repositoryManager, QIcon::fromTheme("folder-remote"), "Repositories");
    
    m_containerManager = new ContainerManager(this);
    m_containerManager->setSystemUtils(m_systemUtils);
    m_containerManager->setPrivilegedExecutor(m_privilegedExecutor);
    m_tabWidget->addTab(m_containerManager, QIcon::fromTheme("application-x-ms-dos-executable"), "Containers");
    
    m_audioManager = new AudioManager(this);
    m_audioManager->setSystemUtils(m_systemUtils);
    m_audioManager->setPrivilegedExecutor(m_privilegedExecutor);
    m_tabWidget->addTab(m_audioManager, QIcon::fromTheme("audio-card"), "Audio");
}

void MainWindow::setupConnections()
{
    connect(m_tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
    
    connect(m_privilegedExecutor, &PrivilegedExecutor::taskStarted, this, &MainWindow::onTaskStarted);
    connect(m_privilegedExecutor, &PrivilegedExecutor::taskFinished, this, &MainWindow::onTaskFinished);
    connect(m_privilegedExecutor, &PrivilegedExecutor::taskError, this, &MainWindow::onTaskError);
    connect(m_privilegedExecutor, &PrivilegedExecutor::taskProgress, this, &MainWindow::onTaskProgress);
}

// Settings functionality removed

void MainWindow::updateSystemInfo()
{
    QString distro = SystemUtils::getDistroName();
    QString kernel = SystemUtils::getKernelVersion();
    QString desktop = SystemUtils::getDesktopEnvironment();
    
    QString systemInfo = QString("%1 | %2 | %3").arg(distro).arg(kernel).arg(desktop);
    m_systemInfoLabel->setText(systemInfo);
}

void MainWindow::setupKDEIntegration()
{
    setProperty("_kde_application_id", "oreon-system-manager");
    
    QString desktop = SystemUtils::getDesktopEnvironment();
    if (desktop.contains("KDE", Qt::CaseInsensitive) || desktop.contains("Plasma", Qt::CaseInsensitive)) {
        setStyleSheet(R"(
            QMainWindow {
                background-color: palette(window);
            }
            QTabWidget::pane {
                border: 1px solid palette(mid);
                background-color: palette(base);
            }
            QTabWidget::tab-bar {
                alignment: left;
            }
            QTabBar::tab {
                background-color: palette(button);
                color: palette(buttonText);
                padding: 8px 16px;
                margin-right: 2px;
                border-top-left-radius: 4px;
                border-top-right-radius: 4px;
            }
            QTabBar::tab:selected {
                background-color: palette(base);
                color: palette(text);
            }
            QTabBar::tab:hover {
                background-color: palette(light);
            }
        )");
    }
}

void MainWindow::applyTheme()
{
    // Standard Breeze theme - no custom styling needed
    // Let Qt use the system theme
    setStyleSheet("");
} 