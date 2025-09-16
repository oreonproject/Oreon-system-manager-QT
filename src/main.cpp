#include <QApplication>
#include <QStyleFactory>
#include <QDir>
#include <QStandardPaths>
#include <QLoggingCategory>
#include <QCommandLineParser>
#include <QSystemTrayIcon>
#include <QIcon>
#include "mainwindow.h"

Q_LOGGING_CATEGORY(oreonApp, "oreon.app")

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("Oreon System Manager");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Oreon");
    app.setOrganizationDomain("oreon.local");
    app.setDesktopFileName("oreon-system-manager");
    
    // Set application icon using system theme
    app.setWindowIcon(QIcon::fromTheme("preferences-desktop-personal"));
    
    // Command line parser
    QCommandLineParser parser;
    parser.setApplicationDescription("Oreon System Manager - Advanced Linux System Management Tool");
    parser.addHelpOption();
    parser.addVersionOption();
    
    QCommandLineOption minimizedOption(QStringList() << "m" << "minimized",
                                      "Start minimized to system tray");
    parser.addOption(minimizedOption);
    
    parser.process(app);
    
    // Check if system tray is available
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        qCritical() << "System tray is not available on this system.";
    }
    
    // Create and show main window
    MainWindow window;
    
    if (parser.isSet(minimizedOption)) {
        window.hide();
    } else {
        window.show();
    }
    
    return app.exec();
} 