#ifndef PACKAGEMANAGER_H
#define PACKAGEMANAGER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSplitter>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextEdit>
#include <QLabel>
#include <QProgressBar>
#include <QComboBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QTimer>
#include <QThread>
#include <QProcess>
#include <QMutex>

class SystemUtils;
class PrivilegedExecutor;
class PackageSearchWorker;

struct PackageInfo {
    QString name;
    QString version;
    QString arch;
    QString repository;
    QString summary;
    QString description;
    QString size;
    QString installDate;
    bool isInstalled;
    bool isUpdateAvailable;
    QString updateVersion;
};

class PackageManager : public QWidget
{
    Q_OBJECT

public:
    explicit PackageManager(QWidget *parent = nullptr);
    ~PackageManager();
    
    void setSystemUtils(SystemUtils* systemUtils);
    void setPrivilegedExecutor(PrivilegedExecutor* privilegedExecutor);

public slots:
    void refreshPackageList();
    void searchPackages();
    void installSelectedPackages();
    void removeSelectedPackages();
    void updateSelectedPackages();
    void updateAllPackages();
    void showPackageDetails(int row, int column);
    void onPackageSelectionChanged();
    void onSearchFinished(const QList<PackageInfo> &packages);
    void onPackageActionSuccess(const QString &output);
    void onPackageActionError(const QString &error);
    void onPackageActionProgress(const QString &progress);
    void clearSearch();
    void selectAllPackages();
    void deselectAllPackages();

private slots:
    void onSearchTextChanged();
    void onSearchTimeout();
    void onFilterChanged();

private:
    void setupUI();
    void setupConnections();
    void updatePackageTable(const QList<PackageInfo> &packages);
    void updatePackageDetails(const PackageInfo &package);
    void updateButtonStates();
    void startSearch();
    void cancelSearch();
    QList<PackageInfo> getSelectedPackages();
    void showProgress(const QString &message);
    void hideProgress();
    void applyPackageFilter();
    
    // UI Components
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_searchLayout;
    QHBoxLayout *m_buttonLayout;
    QHBoxLayout *m_filterLayout;
    QSplitter *m_mainSplitter;
    QSplitter *m_rightSplitter;
    
    // Search section
    QGroupBox *m_searchGroup;
    QLineEdit *m_searchEdit;
    QPushButton *m_searchButton;
    QPushButton *m_clearSearchButton;
    QComboBox *m_searchTypeCombo;
    QComboBox *m_filterCombo;
    QLabel *m_searchStatusLabel;
    
    // Package list section
    QGroupBox *m_packageListGroup;
    QTableWidget *m_packageTable;
    QLabel *m_packageCountLabel;
    QPushButton *m_selectAllButton;
    QPushButton *m_deselectAllButton;
    
    // Action buttons
    QGroupBox *m_actionGroup;
    QPushButton *m_installButton;
    QPushButton *m_removeButton;
    QPushButton *m_updateButton;
    QPushButton *m_updateAllButton;
    QPushButton *m_refreshButton;
    
    // Package details section
    QGroupBox *m_detailsGroup;
    QTextEdit *m_detailsText;
    
    // Progress section
    QGroupBox *m_progressGroup;
    QProgressBar *m_progressBar;
    QLabel *m_progressLabel;
    QTextEdit *m_progressOutput;
    
    // Backend components
    SystemUtils *m_systemUtils;
    PrivilegedExecutor *m_privilegedExecutor;
    PackageSearchWorker *m_searchWorker;
    QThread *m_searchThread;
    
    // State management
    QList<PackageInfo> m_allPackages;
    QList<PackageInfo> m_filteredPackages;
    QTimer *m_searchTimer;
    QString m_currentSearchTerm;
    bool m_isSearching;
    QMutex m_searchMutex;
    
    // Constants
    static const int SEARCH_DELAY_MS = 500;
    static const int COLUMN_NAME = 0;
    static const int COLUMN_VERSION = 1;
    static const int COLUMN_ARCH = 2;
    static const int COLUMN_REPO = 3;
    static const int COLUMN_STATUS = 4;
    static const int COLUMN_SIZE = 5;
    static const int COLUMN_SUMMARY = 6;
};

class PackageSearchWorker : public QObject
{
    Q_OBJECT

public:
    explicit PackageSearchWorker(QObject *parent = nullptr);
    
public slots:
    void searchPackages(const QString &searchTerm, const QString &searchType);
    void refreshAllPackages();
    void cancel();

signals:
    void searchFinished(const QList<PackageInfo> &packages);
    void searchError(const QString &error);
    void searchProgress(const QString &message);

private:
    QList<PackageInfo> parsePackageList(const QString &output, bool installedOnly = false);
    QList<PackageInfo> parsePackageInfo(const QString &output);
    PackageInfo parsePackageInfoBlock(const QString &block);
    QString formatPackageSize(qint64 bytes);
    
    QProcess *m_process;
    bool m_cancelled;
    QMutex m_mutex;
};

#endif // PACKAGEMANAGER_H 