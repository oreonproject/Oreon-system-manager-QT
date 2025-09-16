#ifndef REPOSITORYMANAGER_H
#define REPOSITORYMANAGER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSplitter>
#include <QTabWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QListWidget>
#include <QProgressBar>
#include <QTimer>

class SystemUtils;
class PrivilegedExecutor;

struct RepositoryInfo {
    QString id;
    QString name;
    QString baseUrl;
    QString mirrorList;
    bool enabled;
    QString status;
    QString description;
    QString gpgCheck;
    QString gpgKey;
    QString cost;
    QString priority;
};

struct FlatpakRemote {
    QString name;
    QString url;
    QString title;
    QString description;
    bool enabled;
    bool isSystem;
    QString filter;
};

class RepositoryManager : public QWidget
{
    Q_OBJECT

public:
    explicit RepositoryManager(QWidget *parent = nullptr);
    ~RepositoryManager();
    
    void setSystemUtils(SystemUtils* systemUtils);
    void setPrivilegedExecutor(PrivilegedExecutor* privilegedExecutor);

public slots:
    void refreshRepositories();
    void refreshFlatpakRemotes();
    void enableRepository(const QString &repoId);
    void disableRepository(const QString &repoId);
    void addRepository();
    void removeRepository();
    void editRepository();
    void addFlatpakRemote();
    void removeFlatpakRemote();
    void enableFlatpakRemote(const QString &remoteName);
    void disableFlatpakRemote(const QString &remoteName);
    void installFlatpak();
    void addPredefinedFlatpakRemote(const QString &remoteName, const QString &url);
    void quickAddFlathub();
    void onRepositoryActionSuccess(const QString &output);
    void onRepositoryActionError(const QString &error);
    void onRepositoryActionProgress(const QString &progress);
    void showProgress(const QString &message);
    void hideProgress();

private slots:
    void onRepoTableItemChanged(QTableWidgetItem *item);
    void onRepoSelectionChanged();
    void onFlatpakTableItemChanged(QTableWidgetItem *item);
    void onFlatpakSelectionChanged();
    void showRepositoryDetails(int row, int column);
    void showFlatpakDetails(int row, int column);

private:
    void setupUI();
    void setupConnections();
    void setupDNFTab();
    void setupFlatpakTab();
    void createPredefinedRemotes();
    void updateRepositoryTable(const QList<RepositoryInfo> &repositories);
    void updateFlatpakTable(const QList<FlatpakRemote> &remotes);
    void updateRepositoryDetails(const RepositoryInfo &repo);
    void updateFlatpakDetails(const FlatpakRemote &remote);
    void updateButtonStates();
    QList<RepositoryInfo> parseRepositoryList(const QString &output);
    QList<FlatpakRemote> parseFlatpakRemotes(const QString &output);
    RepositoryInfo parseRepositoryInfo(const QString &block);
    
    // UI Components
    QVBoxLayout *m_mainLayout;
    QTabWidget *m_tabWidget;
    QSplitter *m_mainSplitter;
    
    // DNF Repositories Tab
    QWidget *m_dnfTab;
    QVBoxLayout *m_dnfLayout;
    QSplitter *m_dnfSplitter;
    QGroupBox *m_repoListGroup;
    QTableWidget *m_repoTable;
    QLabel *m_repoCountLabel;
    QGroupBox *m_repoActionGroup;
    QPushButton *m_addRepoButton;
    QPushButton *m_removeRepoButton;
    QPushButton *m_editRepoButton;
    QPushButton *m_refreshRepoButton;
    QGroupBox *m_repoDetailsGroup;
    QTextEdit *m_repoDetailsText;
    
    // Flatpak Tab
    QWidget *m_flatpakTab;
    QVBoxLayout *m_flatpakLayout;
    QSplitter *m_flatpakSplitter;
    QGroupBox *m_flatpakListGroup;
    QTableWidget *m_flatpakTable;
    QLabel *m_flatpakCountLabel;
    QGroupBox *m_flatpakActionGroup;
    QPushButton *m_addFlatpakButton;
    QPushButton *m_removeFlatpakButton;
    QPushButton *m_installFlatpakButton;
    QPushButton *m_refreshFlatpakButton;
    QGroupBox *m_flatpakDetailsGroup;
    QTextEdit *m_flatpakDetailsText;
    
    // Status and Progress
    QLabel *m_statusLabel;
    QProgressBar *m_progressBar;
    
    // Predefined Flatpak remotes
    QGroupBox *m_predefinedGroup;
    QVBoxLayout *m_predefinedLayout;
    QLabel *m_predefinedLabel;
    QPushButton *m_flathubButton;
    QPushButton *m_fedoraButton;
    QPushButton *m_gnomeButton;
    QPushButton *m_kdeButton;
    
    // Progress section
    QGroupBox *m_progressGroup;
    QTextEdit *m_progressOutput;
    
    // Backend components
    SystemUtils *m_systemUtils;
    PrivilegedExecutor *m_privilegedExecutor;
    
    // Data
    QList<RepositoryInfo> m_repositories;
    QList<FlatpakRemote> m_flatpakRemotes;
    
    // Constants
    static const int REPO_COLUMN_NAME = 0;
    static const int REPO_COLUMN_ENABLED = 1;
    static const int REPO_COLUMN_URL = 2;
    static const int REPO_COLUMN_STATUS = 3;
    
    static const int FLATPAK_COLUMN_NAME = 0;
    static const int FLATPAK_COLUMN_ENABLED = 1;
    static const int FLATPAK_COLUMN_URL = 2;
    static const int FLATPAK_COLUMN_TITLE = 3;
};

#endif // REPOSITORYMANAGER_H 