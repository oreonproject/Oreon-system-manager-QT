#include "repositorymanager.h"
#include "systemutils.h"
#include "privilegedexecutor.h"

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
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QRegularExpression>
#include <QDateTime>
#include <QIcon>
#include <QScrollBar>
#include <QDesktopServices>
#include <QUrl>
#include <QProcess>

RepositoryManager::RepositoryManager(QWidget *parent)
    : QWidget(parent)
    , m_mainLayout(nullptr)
    , m_tabWidget(nullptr)
    , m_mainSplitter(nullptr)
    , m_dnfTab(nullptr)
    , m_dnfLayout(nullptr)
    , m_dnfSplitter(nullptr)
    , m_repoListGroup(nullptr)
    , m_repoTable(nullptr)
    , m_repoCountLabel(nullptr)
    , m_repoActionGroup(nullptr)
    , m_addRepoButton(nullptr)
    , m_removeRepoButton(nullptr)
    , m_editRepoButton(nullptr)
    , m_refreshRepoButton(nullptr)
    , m_repoDetailsGroup(nullptr)
    , m_repoDetailsText(nullptr)
    , m_flatpakTab(nullptr)
    , m_flatpakLayout(nullptr)
    , m_flatpakSplitter(nullptr)
    , m_flatpakListGroup(nullptr)
    , m_flatpakTable(nullptr)
    , m_flatpakCountLabel(nullptr)
    , m_flatpakActionGroup(nullptr)
    , m_addFlatpakButton(nullptr)
    , m_removeFlatpakButton(nullptr)
    , m_installFlatpakButton(nullptr)
    , m_refreshFlatpakButton(nullptr)
    , m_flatpakDetailsGroup(nullptr)
    , m_flatpakDetailsText(nullptr)
    , m_predefinedGroup(nullptr)
    , m_predefinedLayout(nullptr)
    , m_predefinedLabel(nullptr)
    , m_flathubButton(nullptr)
    , m_fedoraButton(nullptr)
    , m_gnomeButton(nullptr)
    , m_kdeButton(nullptr)
    , m_progressGroup(nullptr)
    , m_progressBar(nullptr)
    , m_progressOutput(nullptr)
    , m_systemUtils(nullptr)
    , m_privilegedExecutor(nullptr)
    , m_statusLabel(nullptr)
{
    m_systemUtils = new SystemUtils(this);
    m_privilegedExecutor = new PrivilegedExecutor(this);
    
    setupUI();
    setupConnections();
    
    refreshRepositories();
    if (SystemUtils::isFlatpakAvailable()) {
        refreshFlatpakRemotes();
    }
}

RepositoryManager::~RepositoryManager()
{
}

void RepositoryManager::setSystemUtils(SystemUtils* systemUtils)
{
    m_systemUtils = systemUtils;
}

void RepositoryManager::setPrivilegedExecutor(PrivilegedExecutor* privilegedExecutor)
{
    m_privilegedExecutor = privilegedExecutor;
}

void RepositoryManager::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(6);
    m_mainLayout->setContentsMargins(6, 6, 6, 6);
    
    m_tabWidget = new QTabWidget(this);
    m_mainLayout->addWidget(m_tabWidget);
    
    setupDNFTab();
    setupFlatpakTab();
    
    m_progressGroup = new QGroupBox("Progress", this);
    QVBoxLayout *progressLayout = new QVBoxLayout(m_progressGroup);
    
    m_progressBar = new QProgressBar(m_progressGroup);
    m_progressBar->setVisible(false);
    
    m_progressOutput = new QTextEdit(m_progressGroup);
    m_progressOutput->setReadOnly(true);
    m_progressOutput->setMaximumHeight(120);
    m_progressOutput->setStyleSheet("QTextEdit { background-color: #2b2b2b; color: #ffffff; font-family: monospace; font-size: 9pt; }");
    
    progressLayout->addWidget(m_progressBar);
    progressLayout->addWidget(m_progressOutput);
    
    m_mainLayout->addWidget(m_progressGroup);
}

void RepositoryManager::setupDNFTab()
{
    m_dnfTab = new QWidget();
    m_dnfLayout = new QVBoxLayout(m_dnfTab);
    m_dnfLayout->setSpacing(6);
    m_dnfLayout->setContentsMargins(6, 6, 6, 6);
    
    m_dnfSplitter = new QSplitter(Qt::Horizontal, m_dnfTab);
    m_dnfLayout->addWidget(m_dnfSplitter);
    
    QWidget *leftWidget = new QWidget();
    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setSpacing(6);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    
    m_repoListGroup = new QGroupBox("DNF Repositories", leftWidget);
    QVBoxLayout *repoListLayout = new QVBoxLayout(m_repoListGroup);
    
    m_repoTable = new QTableWidget(0, 4, m_repoListGroup);
    QStringList repoHeaders = {"Repository ID", "Enabled", "Base URL", "Status"};
    m_repoTable->setHorizontalHeaderLabels(repoHeaders);
    m_repoTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_repoTable->setAlternatingRowColors(true);
    m_repoTable->setSortingEnabled(true);
    m_repoTable->horizontalHeader()->setStretchLastSection(true);
    m_repoTable->verticalHeader()->setVisible(false);
    
    m_repoTable->setColumnWidth(REPO_COLUMN_NAME, 200);
    m_repoTable->setColumnWidth(REPO_COLUMN_ENABLED, 80);
    m_repoTable->setColumnWidth(REPO_COLUMN_URL, 300);
    m_repoTable->setColumnWidth(REPO_COLUMN_STATUS, 100);
    
    repoListLayout->addWidget(m_repoTable);
    
    m_repoCountLabel = new QLabel("0 repositories", m_repoListGroup);
    repoListLayout->addWidget(m_repoCountLabel);
    
    leftLayout->addWidget(m_repoListGroup);
    
    m_repoActionGroup = new QGroupBox("Actions", leftWidget);
    QGridLayout *repoActionLayout = new QGridLayout(m_repoActionGroup);
    
    m_addRepoButton = new QPushButton(QIcon::fromTheme("list-add"), "Add Repository", m_repoActionGroup);
    m_removeRepoButton = new QPushButton(QIcon::fromTheme("list-remove"), "Remove Repository", m_repoActionGroup);
    m_editRepoButton = new QPushButton(QIcon::fromTheme("document-edit"), "Edit Repository", m_repoActionGroup);
    m_refreshRepoButton = new QPushButton(QIcon::fromTheme("view-refresh"), "Refresh", m_repoActionGroup);
    
    repoActionLayout->addWidget(m_addRepoButton, 0, 0);
    repoActionLayout->addWidget(m_removeRepoButton, 0, 1);
    repoActionLayout->addWidget(m_editRepoButton, 1, 0);
    repoActionLayout->addWidget(m_refreshRepoButton, 1, 1);
    
    leftLayout->addWidget(m_repoActionGroup);
    
    m_dnfSplitter->addWidget(leftWidget);
    
    m_repoDetailsGroup = new QGroupBox("Repository Details", m_dnfTab);
    QVBoxLayout *repoDetailsLayout = new QVBoxLayout(m_repoDetailsGroup);
    
    m_repoDetailsText = new QTextEdit(m_repoDetailsGroup);
    m_repoDetailsText->setReadOnly(true);
    
    repoDetailsLayout->addWidget(m_repoDetailsText);
    
    m_dnfSplitter->addWidget(m_repoDetailsGroup);
    
    m_dnfSplitter->setSizes({500, 300});
    
    m_tabWidget->addTab(m_dnfTab, QIcon::fromTheme("folder-remote"), "DNF Repositories");
}

void RepositoryManager::setupFlatpakTab()
{
    m_flatpakTab = new QWidget();
    m_flatpakLayout = new QVBoxLayout(m_flatpakTab);
    m_flatpakLayout->setSpacing(6);
    m_flatpakLayout->setContentsMargins(6, 6, 6, 6);
    
    // Flatpak Remotes Section
    QGroupBox *flatpakGroup = new QGroupBox("Flatpak Remotes");
    QVBoxLayout *flatpakLayout = new QVBoxLayout(flatpakGroup);
    
    // Table for remotes
    m_flatpakTable = new QTableWidget(0, 4);
    QStringList flatpakHeaders = {"Remote Name", "Enabled", "URL", "Title"};
    m_flatpakTable->setHorizontalHeaderLabels(flatpakHeaders);
    m_flatpakTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_flatpakTable->setAlternatingRowColors(true);
    m_flatpakTable->setSortingEnabled(true);
    m_flatpakTable->horizontalHeader()->setStretchLastSection(true);
    m_flatpakTable->verticalHeader()->setVisible(false);
    
    m_flatpakTable->setColumnWidth(0, 150);
    m_flatpakTable->setColumnWidth(1, 80);
    m_flatpakTable->setColumnWidth(2, 300);
    m_flatpakTable->setColumnWidth(3, 200);
    
    flatpakLayout->addWidget(m_flatpakTable);
    
    // Count label
    m_flatpakCountLabel = new QLabel("0 remotes");
    flatpakLayout->addWidget(m_flatpakCountLabel);
    
    // Action buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    m_addFlatpakButton = new QPushButton("Add Remote");
    m_addFlatpakButton->setToolTip("Add a new Flatpak remote");
    connect(m_addFlatpakButton, &QPushButton::clicked, this, &RepositoryManager::addFlatpakRemote);
    buttonLayout->addWidget(m_addFlatpakButton);
    
    m_removeFlatpakButton = new QPushButton("Remove Remote");
    m_removeFlatpakButton->setToolTip("Remove selected Flatpak remote");
    connect(m_removeFlatpakButton, &QPushButton::clicked, this, &RepositoryManager::removeFlatpakRemote);
    buttonLayout->addWidget(m_removeFlatpakButton);
    
    m_refreshFlatpakButton = new QPushButton("Refresh");
    m_refreshFlatpakButton->setToolTip("Refresh the list of Flatpak remotes");
    connect(m_refreshFlatpakButton, &QPushButton::clicked, this, &RepositoryManager::refreshFlatpakRemotes);
    buttonLayout->addWidget(m_refreshFlatpakButton);
    
    // Quick add Flathub button (if not exists)
    QPushButton *quickAddButton = new QPushButton("Quick Add Flathub");
    quickAddButton->setToolTip("Add Flathub repository if it doesn't exist");
    connect(quickAddButton, &QPushButton::clicked, this, &RepositoryManager::quickAddFlathub);
    buttonLayout->addWidget(quickAddButton);
    
    buttonLayout->addStretch();
    
    flatpakLayout->addLayout(buttonLayout);
    
    m_flatpakLayout->addWidget(flatpakGroup);
    
    // Status and Progress
    m_statusLabel = new QLabel("Ready");
    m_statusLabel->setStyleSheet("color: #666; font-size: 10px;");
    m_flatpakLayout->addWidget(m_statusLabel);
    
    m_progressBar = new QProgressBar();
    m_progressBar->setVisible(false);
    m_flatpakLayout->addWidget(m_progressBar);
    
    m_flatpakLayout->addStretch();
    
    if (!SystemUtils::isFlatpakAvailable()) {
        flatpakGroup->setEnabled(false);
        m_statusLabel->setText("Flatpak is not installed on this system");
    }
}

void RepositoryManager::createPredefinedRemotes()
{
    m_predefinedGroup = new QGroupBox("Quick Add Popular Remotes");
    m_predefinedLayout = new QVBoxLayout(m_predefinedGroup);
    
    m_predefinedLabel = new QLabel("Add popular Flatpak remotes with one click:");
    m_predefinedLayout->addWidget(m_predefinedLabel);
    
    QHBoxLayout *buttonRow1 = new QHBoxLayout();
    m_flathubButton = new QPushButton(QIcon::fromTheme("application-x-flatpak"), "Flathub");
    m_flathubButton->setToolTip("The main Flatpak repository");
    m_fedoraButton = new QPushButton(QIcon::fromTheme("fedora"), "Fedora");
    m_fedoraButton->setToolTip("Fedora Flatpak repository");
    
    buttonRow1->addWidget(m_flathubButton);
    buttonRow1->addWidget(m_fedoraButton);
    m_predefinedLayout->addLayout(buttonRow1);
    
    QHBoxLayout *buttonRow2 = new QHBoxLayout();
    m_gnomeButton = new QPushButton(QIcon::fromTheme("gnome"), "GNOME");
    m_gnomeButton->setToolTip("GNOME nightly applications");
    m_kdeButton = new QPushButton(QIcon::fromTheme("kde"), "KDE");
    m_kdeButton->setToolTip("KDE nightly applications");
    
    buttonRow2->addWidget(m_gnomeButton);
    buttonRow2->addWidget(m_kdeButton);
    m_predefinedLayout->addLayout(buttonRow2);
}

void RepositoryManager::setupConnections()
{
    connect(m_repoTable, &QTableWidget::cellClicked, this, &RepositoryManager::showRepositoryDetails);
    connect(m_repoTable, &QTableWidget::itemChanged, this, &RepositoryManager::onRepoTableItemChanged);
    connect(m_repoTable, &QTableWidget::itemSelectionChanged, this, &RepositoryManager::onRepoSelectionChanged);
    
    connect(m_addRepoButton, &QPushButton::clicked, this, &RepositoryManager::addRepository);
    connect(m_removeRepoButton, &QPushButton::clicked, this, &RepositoryManager::removeRepository);
    connect(m_editRepoButton, &QPushButton::clicked, this, &RepositoryManager::editRepository);
    connect(m_refreshRepoButton, &QPushButton::clicked, this, &RepositoryManager::refreshRepositories);
    
    connect(m_flatpakTable, &QTableWidget::cellClicked, this, &RepositoryManager::showFlatpakDetails);
    connect(m_flatpakTable, &QTableWidget::itemChanged, this, &RepositoryManager::onFlatpakTableItemChanged);
    connect(m_flatpakTable, &QTableWidget::itemSelectionChanged, this, &RepositoryManager::onFlatpakSelectionChanged);
    
    connect(m_privilegedExecutor, &PrivilegedExecutor::taskFinished,
            [this](int taskId, int exitCode, const QString &output) {
                Q_UNUSED(taskId); Q_UNUSED(exitCode);
                onRepositoryActionSuccess(output);
            });
    connect(m_privilegedExecutor, &PrivilegedExecutor::taskError,
            [this](int taskId, const QString &error) {
                Q_UNUSED(taskId);
                onRepositoryActionError(error);
            });
    connect(m_privilegedExecutor, &PrivilegedExecutor::taskProgress,
            [this](int taskId, const QString &progress) {
                Q_UNUSED(taskId);
                onRepositoryActionProgress(progress);
            });
}

void RepositoryManager::refreshRepositories()
{
    showProgress("Loading repository information...");
    
    auto result = SystemUtils::runCommand("dnf", {"repolist", "all", "--verbose"});
    if (result.first == 0) {
        m_repositories = parseRepositoryList(result.second);
        updateRepositoryTable(m_repositories);
    } else {
        onRepositoryActionError("Failed to load repository information");
    }
    
    hideProgress();
}

void RepositoryManager::refreshFlatpakRemotes()
{
    if (!SystemUtils::isFlatpakAvailable()) {
        return;
    }
    
    showProgress("Loading Flatpak remotes...");
    
    auto result = SystemUtils::runCommand("flatpak", {"remotes", "--show-details"});
    if (result.first == 0) {
        m_flatpakRemotes = parseFlatpakRemotes(result.second);
        updateFlatpakTable(m_flatpakRemotes);
    } else {
        onRepositoryActionError("Failed to load Flatpak remotes");
    }
    
    hideProgress();
}

void RepositoryManager::updateRepositoryTable(const QList<RepositoryInfo> &repositories)
{
    m_repoTable->setRowCount(repositories.size());
    m_repoTable->setSortingEnabled(false);
    
    for (int i = 0; i < repositories.size(); ++i) {
        const RepositoryInfo &repo = repositories[i];
        
        QTableWidgetItem *nameItem = new QTableWidgetItem(repo.id);
        nameItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_repoTable->setItem(i, REPO_COLUMN_NAME, nameItem);
        
        QTableWidgetItem *enabledItem = new QTableWidgetItem();
        enabledItem->setCheckState(repo.enabled ? Qt::Checked : Qt::Unchecked);
        enabledItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        m_repoTable->setItem(i, REPO_COLUMN_ENABLED, enabledItem);
        
        QTableWidgetItem *urlItem = new QTableWidgetItem(repo.baseUrl);
        urlItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_repoTable->setItem(i, REPO_COLUMN_URL, urlItem);
        
        QTableWidgetItem *statusItem = new QTableWidgetItem(repo.status);
        statusItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        if (repo.enabled) {
            statusItem->setBackground(QColor(0, 255, 0, 100));
        } else {
            statusItem->setBackground(QColor(255, 0, 0, 100));
        }
        m_repoTable->setItem(i, REPO_COLUMN_STATUS, statusItem);
    }
    
    m_repoTable->setSortingEnabled(true);
    m_repoCountLabel->setText(QString("%1 repositories").arg(repositories.size()));
    updateButtonStates();
}

void RepositoryManager::updateFlatpakTable(const QList<FlatpakRemote> &remotes)
{
    m_flatpakTable->setRowCount(remotes.size());
    m_flatpakTable->setSortingEnabled(false);
    
    for (int i = 0; i < remotes.size(); ++i) {
        const FlatpakRemote &remote = remotes[i];
        
        QTableWidgetItem *nameItem = new QTableWidgetItem(remote.name);
        nameItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_flatpakTable->setItem(i, FLATPAK_COLUMN_NAME, nameItem);
        
        QTableWidgetItem *enabledItem = new QTableWidgetItem();
        enabledItem->setCheckState(remote.enabled ? Qt::Checked : Qt::Unchecked);
        enabledItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        m_flatpakTable->setItem(i, FLATPAK_COLUMN_ENABLED, enabledItem);
        
        QTableWidgetItem *urlItem = new QTableWidgetItem(remote.url);
        urlItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_flatpakTable->setItem(i, FLATPAK_COLUMN_URL, urlItem);
        
        QTableWidgetItem *titleItem = new QTableWidgetItem(remote.title);
        titleItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_flatpakTable->setItem(i, FLATPAK_COLUMN_TITLE, titleItem);
    }
    
    m_flatpakTable->setSortingEnabled(true);
    m_flatpakCountLabel->setText(QString("%1 remotes").arg(remotes.size()));
    updateButtonStates();
}

void RepositoryManager::onRepoTableItemChanged(QTableWidgetItem *item)
{
    if (item->column() == REPO_COLUMN_ENABLED) {
        int row = item->row();
        if (row >= 0 && row < m_repositories.size()) {
            const RepositoryInfo &repo = m_repositories[row];
            bool enable = item->checkState() == Qt::Checked;
            
            if (enable != repo.enabled) {
                if (enable) {
                    enableRepository(repo.id);
                } else {
                    disableRepository(repo.id);
                }
            }
        }
    }
}

void RepositoryManager::onFlatpakTableItemChanged(QTableWidgetItem *item)
{
    if (item->column() == FLATPAK_COLUMN_ENABLED) {
        int row = item->row();
        if (row >= 0 && row < m_flatpakRemotes.size()) {
            const FlatpakRemote &remote = m_flatpakRemotes[row];
            bool enable = item->checkState() == Qt::Checked;
            
            if (enable != remote.enabled) {
                if (enable) {
                    enableFlatpakRemote(remote.name);
                } else {
                    disableFlatpakRemote(remote.name);
                }
            }
        }
    }
}

void RepositoryManager::showRepositoryDetails(int row, int column)
{
    if (row >= 0 && row < m_repositories.size()) {
        updateRepositoryDetails(m_repositories[row]);
    }
}

void RepositoryManager::showFlatpakDetails(int row, int column)
{
    if (row >= 0 && row < m_flatpakRemotes.size()) {
        updateFlatpakDetails(m_flatpakRemotes[row]);
    }
}

void RepositoryManager::updateRepositoryDetails(const RepositoryInfo &repo)
{
    QString details = QString(
        "<h3>%1</h3>"
        "<p><b>Name:</b> %2</p>"
        "<p><b>Enabled:</b> %3</p>"
        "<p><b>Base URL:</b> %4</p>"
        "<p><b>Mirror List:</b> %5</p>"
        "<p><b>Status:</b> %6</p>"
        "<p><b>GPG Check:</b> %7</p>"
        "<p><b>GPG Key:</b> %8</p>"
        "<p><b>Cost:</b> %9</p>"
        "<p><b>Priority:</b> %10</p>"
        "<p><b>Description:</b><br>%11</p>"
    ).arg(repo.id)
     .arg(repo.name.isEmpty() ? "N/A" : repo.name)
     .arg(repo.enabled ? "Yes" : "No")
     .arg(repo.baseUrl.isEmpty() ? "N/A" : repo.baseUrl)
     .arg(repo.mirrorList.isEmpty() ? "N/A" : repo.mirrorList)
     .arg(repo.status)
     .arg(repo.gpgCheck.isEmpty() ? "N/A" : repo.gpgCheck)
     .arg(repo.gpgKey.isEmpty() ? "N/A" : repo.gpgKey)
     .arg(repo.cost.isEmpty() ? "N/A" : repo.cost)
     .arg(repo.priority.isEmpty() ? "N/A" : repo.priority)
     .arg(repo.description.isEmpty() ? "No description available." : repo.description);
    
    m_repoDetailsText->setHtml(details);
}

void RepositoryManager::updateFlatpakDetails(const FlatpakRemote &remote)
{
    QString details = QString(
        "<h3>%1</h3>"
        "<p><b>Title:</b> %2</p>"
        "<p><b>Enabled:</b> %3</p>"
        "<p><b>URL:</b> %4</p>"
        "<p><b>Type:</b> %5</p>"
        "<p><b>Filter:</b> %6</p>"
        "<p><b>Description:</b><br>%7</p>"
    ).arg(remote.name)
     .arg(remote.title.isEmpty() ? "N/A" : remote.title)
     .arg(remote.enabled ? "Yes" : "No")
     .arg(remote.url)
     .arg(remote.isSystem ? "System" : "User")
     .arg(remote.filter.isEmpty() ? "None" : remote.filter)
     .arg(remote.description.isEmpty() ? "No description available." : remote.description);
    
    m_flatpakDetailsText->setHtml(details);
}

void RepositoryManager::enableRepository(const QString &repoId)
{
    showProgress(QString("Enabling repository %1...").arg(repoId));
    m_privilegedExecutor->enableRepository(repoId, this,
                                          SLOT(onRepositoryActionSuccess(QString)),
                                          SLOT(onRepositoryActionError(QString)));
}

void RepositoryManager::disableRepository(const QString &repoId)
{
    showProgress(QString("Disabling repository %1...").arg(repoId));
    m_privilegedExecutor->disableRepository(repoId, this,
                                           SLOT(onRepositoryActionSuccess(QString)),
                                           SLOT(onRepositoryActionError(QString)));
}

void RepositoryManager::addRepository()
{
    bool ok;
    QString url = QInputDialog::getText(this, "Add Repository",
                                       "Repository URL or .repo file URL:",
                                       QLineEdit::Normal, "", &ok);
    
    if (ok && !url.isEmpty()) {
        QString name = QInputDialog::getText(this, "Add Repository",
                                           "Repository name (optional):",
                                           QLineEdit::Normal, "", &ok);
        
        if (ok) {
            showProgress("Adding repository...");
            m_privilegedExecutor->addRepository(url, name.isEmpty() ? "custom-repo" : name, this,
                                               SLOT(onRepositoryActionSuccess(QString)),
                                               SLOT(onRepositoryActionError(QString)));
        }
    }
}

void RepositoryManager::removeRepository()
{
    int currentRow = m_repoTable->currentRow();
    if (currentRow >= 0 && currentRow < m_repositories.size()) {
        const RepositoryInfo &repo = m_repositories[currentRow];
        
        int result = QMessageBox::question(this, "Remove Repository",
                                         QString("Are you sure you want to remove repository '%1'?").arg(repo.id),
                                         QMessageBox::Yes | QMessageBox::No);
        
        if (result == QMessageBox::Yes) {
            showProgress(QString("Removing repository %1...").arg(repo.id));
            
            QString repoFile = QString("/etc/yum.repos.d/%1.repo").arg(repo.id);
            m_privilegedExecutor->deleteSystemFile(repoFile, this,
                                                  SLOT(onRepositoryActionSuccess(QString)),
                                                  SLOT(onRepositoryActionError(QString)));
        }
    }
}

void RepositoryManager::editRepository()
{
    QMessageBox::information(this, "Edit Repository", 
                           "Repository editing is not yet implemented. "
                           "You can manually edit repository files in /etc/yum.repos.d/");
}

void RepositoryManager::addFlatpakRemote()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Add Flatpak Remote",
                                        "Remote name:", QLineEdit::Normal, "", &ok);
    
    if (ok && !name.isEmpty()) {
        QString url = QInputDialog::getText(this, "Add Flatpak Remote",
                                          "Remote URL:", QLineEdit::Normal, "", &ok);
        
        if (ok && !url.isEmpty()) {
            showProgress(QString("Adding Flatpak remote %1...").arg(name));
            
            QStringList args;
            args << "remote-add" << "--if-not-exists" << name << url;
            
            m_privilegedExecutor->executeCommandAsync("flatpak", args,
                                                     QString("Adding Flatpak remote %1").arg(name),
                                                     this,
                                                     SLOT(onRepositoryActionSuccess(QString)),
                                                     SLOT(onRepositoryActionError(QString)));
        }
    }
}

void RepositoryManager::removeFlatpakRemote()
{
    int currentRow = m_flatpakTable->currentRow();
    if (currentRow >= 0 && currentRow < m_flatpakRemotes.size()) {
        const FlatpakRemote &remote = m_flatpakRemotes[currentRow];
        
        int result = QMessageBox::question(this, "Remove Flatpak Remote",
                                         QString("Are you sure you want to remove remote '%1'?").arg(remote.name),
                                         QMessageBox::Yes | QMessageBox::No);
        
        if (result == QMessageBox::Yes) {
            showProgress(QString("Removing Flatpak remote %1...").arg(remote.name));
            
            QStringList args;
            args << "remote-delete" << remote.name;
            
            m_privilegedExecutor->executeCommandAsync("flatpak", args,
                                                     QString("Removing Flatpak remote %1").arg(remote.name),
                                                     this,
                                                     SLOT(onRepositoryActionSuccess(QString)),
                                                     SLOT(onRepositoryActionError(QString)));
        }
    }
}

void RepositoryManager::enableFlatpakRemote(const QString &remoteName)
{
    showProgress(QString("Enabling Flatpak remote %1...").arg(remoteName));
    
    QStringList args;
    args << "remote-modify" << "--enable" << remoteName;
    
    m_privilegedExecutor->executeCommandAsync("flatpak", args,
                                             QString("Enabling Flatpak remote %1").arg(remoteName),
                                             this,
                                             SLOT(onRepositoryActionSuccess(QString)),
                                             SLOT(onRepositoryActionError(QString)));
}

void RepositoryManager::disableFlatpakRemote(const QString &remoteName)
{
    showProgress(QString("Disabling Flatpak remote %1...").arg(remoteName));
    
    QStringList args;
    args << "remote-modify" << "--disable" << remoteName;
    
    m_privilegedExecutor->executeCommandAsync("flatpak", args,
                                             QString("Disabling Flatpak remote %1").arg(remoteName),
                                             this,
                                             SLOT(onRepositoryActionSuccess(QString)),
                                             SLOT(onRepositoryActionError(QString)));
}

void RepositoryManager::installFlatpak()
{
    bool ok;
    QString appId = QInputDialog::getText(this, "Install Flatpak Application",
                                         "Application ID (e.g., org.gimp.GIMP):",
                                         QLineEdit::Normal, "", &ok);
    
    if (ok && !appId.isEmpty()) {
        showProgress(QString("Installing Flatpak application %1...").arg(appId));
        
        QStringList args;
        args << "install" << "-y" << appId;
        
        m_privilegedExecutor->executeCommandAsync("flatpak", args,
                                                 QString("Installing Flatpak application %1").arg(appId),
                                                 this,
                                                 SLOT(onRepositoryActionSuccess(QString)),
                                                 SLOT(onRepositoryActionError(QString)),
                                                 SLOT(onRepositoryActionProgress(QString)));
    }
}

void RepositoryManager::addPredefinedFlatpakRemote(const QString &remoteName, const QString &url)
{
    if (!SystemUtils::isFlatpakAvailable()) {
        QMessageBox::warning(this, "Flatpak Not Available", 
                           "Flatpak is not installed on this system. Please install Flatpak first.");
        return;
    }
    
    showProgress(QString("Adding %1 remote...").arg(remoteName));
    
    QStringList args;
    args << "remote-add" << "--if-not-exists" << remoteName << url;
    
    m_privilegedExecutor->executeCommandAsync("flatpak", args,
                                             QString("Adding %1 remote").arg(remoteName),
                                             this,
                                             SLOT(onRepositoryActionSuccess(QString)),
                                             SLOT(onRepositoryActionError(QString)));
}

void RepositoryManager::onRepositoryActionSuccess(const QString &output)
{
    hideProgress();
    m_progressOutput->append(QString("[%1] Operation completed successfully")
                           .arg(QDateTime::currentDateTime().toString()));
    
    if (!output.isEmpty()) {
        m_progressOutput->append(output);
    }
    
    refreshRepositories();
    if (SystemUtils::isFlatpakAvailable()) {
        refreshFlatpakRemotes();
    }
}

void RepositoryManager::onRepositoryActionError(const QString &error)
{
    hideProgress();
    m_progressOutput->append(QString("[%1] Operation failed: %2")
                           .arg(QDateTime::currentDateTime().toString())
                           .arg(error));
    
    QMessageBox::critical(this, "Operation Failed", error);
}

void RepositoryManager::onRepositoryActionProgress(const QString &progress)
{
    if (!progress.isEmpty()) {
        m_progressOutput->append(progress);
        QScrollBar *scrollBar = m_progressOutput->verticalScrollBar();
        scrollBar->setValue(scrollBar->maximum());
    }
}

void RepositoryManager::onRepoSelectionChanged()
{
    updateButtonStates();
}

void RepositoryManager::onFlatpakSelectionChanged()
{
    updateButtonStates();
}

void RepositoryManager::updateButtonStates()
{
    bool hasRepoSelection = m_repoTable->currentRow() >= 0;
    bool hasFlatpakSelection = m_flatpakTable->currentRow() >= 0;
    
    m_removeRepoButton->setEnabled(hasRepoSelection);
    m_editRepoButton->setEnabled(hasRepoSelection);
    
    if (SystemUtils::isFlatpakAvailable()) {
        m_removeFlatpakButton->setEnabled(hasFlatpakSelection);
    }
}

void RepositoryManager::showProgress(const QString &message)
{
    if (m_progressBar) {
        m_progressBar->setVisible(true);
        m_progressBar->setRange(0, 0); // Indeterminate
    }
    if (m_statusLabel) {
        m_statusLabel->setText(message);
    }
}

void RepositoryManager::hideProgress()
{
    if (m_progressBar) {
        m_progressBar->setVisible(false);
    }
    if (m_statusLabel) {
        m_statusLabel->setText("Ready");
    }
}

QList<RepositoryInfo> RepositoryManager::parseRepositoryList(const QString &output)
{
    QList<RepositoryInfo> repositories;
    QStringList lines = output.split('\n');
    
    RepositoryInfo currentRepo;
    bool inRepoSection = false;
    
    for (const QString &line : lines) {
        QString trimmedLine = line.trimmed();
        
        if (trimmedLine.startsWith("Repo-id")) {
            if (inRepoSection && !currentRepo.id.isEmpty()) {
                repositories.append(currentRepo);
            }
            currentRepo = RepositoryInfo();
            inRepoSection = true;
            currentRepo.id = trimmedLine.split(":").last().trimmed();
        } else if (inRepoSection) {
            if (trimmedLine.startsWith("Repo-name")) {
                currentRepo.name = trimmedLine.split(":").last().trimmed();
            } else if (trimmedLine.startsWith("Repo-status")) {
                QString status = trimmedLine.split(":").last().trimmed();
                currentRepo.enabled = status.contains("enabled", Qt::CaseInsensitive);
                currentRepo.status = status;
            } else if (trimmedLine.startsWith("Repo-baseurl")) {
                currentRepo.baseUrl = trimmedLine.split(":").last().trimmed();
            } else if (trimmedLine.startsWith("Repo-metalink")) {
                if (currentRepo.baseUrl.isEmpty()) {
                    currentRepo.baseUrl = trimmedLine.split(":").last().trimmed();
                }
            } else if (trimmedLine.startsWith("Repo-mirrors")) {
                currentRepo.mirrorList = trimmedLine.split(":").last().trimmed();
            } else if (trimmedLine.startsWith("Repo-description")) {
                currentRepo.description = trimmedLine.split(":").last().trimmed();
            }
        }
    }
    
    if (inRepoSection && !currentRepo.id.isEmpty()) {
        repositories.append(currentRepo);
    }
    
    return repositories;
}

QList<FlatpakRemote> RepositoryManager::parseFlatpakRemotes(const QString &output)
{
    QList<FlatpakRemote> remotes;
    QStringList lines = output.split('\n');
    
    for (const QString &line : lines) {
        QString trimmedLine = line.trimmed();
        if (trimmedLine.isEmpty() || trimmedLine.startsWith("Name")) {
            continue;
        }
        
        QStringList parts = trimmedLine.split('\t');
        if (parts.size() >= 4) {
            FlatpakRemote remote;
            remote.name = parts[0].trimmed();
            remote.enabled = !parts[1].contains("disabled");
            remote.url = parts[2].trimmed();
            remote.title = parts[3].trimmed();
            remote.isSystem = parts.size() > 4 && parts[4].contains("system");
            
            remotes.append(remote);
        }
    }
    
    return remotes;
}

RepositoryInfo RepositoryManager::parseRepositoryInfo(const QString &block)
{
    RepositoryInfo repo;
    QStringList lines = block.split('\n');
    
    for (const QString &line : lines) {
        QString trimmedLine = line.trimmed();
        if (trimmedLine.startsWith("repo id:")) {
            repo.id = trimmedLine.split(":").last().trimmed();
        } else if (trimmedLine.startsWith("repo name:")) {
            repo.name = trimmedLine.split(":").last().trimmed();
        } else if (trimmedLine.startsWith("repo status:")) {
            QString status = trimmedLine.split(":").last().trimmed();
            repo.enabled = status == "enabled";
            repo.status = status;
        } else if (trimmedLine.startsWith("repo baseurl:")) {
            repo.baseUrl = trimmedLine.split(":").last().trimmed();
        } else if (trimmedLine.startsWith("repo gpgcheck:")) {
            repo.gpgCheck = trimmedLine.split(":").last().trimmed();
        }
    }
    
    return repo;
}

void RepositoryManager::quickAddFlathub()
{
    if (!SystemUtils::isFlatpakAvailable()) {
        m_statusLabel->setText("Flatpak is not installed on this system");
        return;
    }
    
    // Check if Flathub already exists
    for (const FlatpakRemote &remote : m_flatpakRemotes) {
        if (remote.name == "flathub") {
            m_statusLabel->setText("Flathub is already added");
            QTimer::singleShot(3000, [this]() { m_statusLabel->setText("Ready"); });
            return;
        }
    }
    
    showProgress("Adding Flathub repository...");
    
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
        hideProgress();
        if (exitCode == 0) {
            m_statusLabel->setText("Flathub added successfully");
            refreshFlatpakRemotes();
        } else {
            m_statusLabel->setText("Failed to add Flathub");
        }
        QTimer::singleShot(3000, [this]() { m_statusLabel->setText("Ready"); });
        process->deleteLater();
    });
    
    process->start("flatpak", QStringList() << "remote-add" << "--if-not-exists" << "flathub" << "https://dl.flathub.org/repo/flathub.flatpakrepo");
} 