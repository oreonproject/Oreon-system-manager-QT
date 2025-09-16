#include "packagemanager.h"
#include "systemutils.h"
#include "privilegedexecutor.h"

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
#include <QMap>
#include <QHeaderView>
#include <QMessageBox>
#include <QApplication>
#include <QRegularExpression>
#include <QDateTime>
#include <QIcon>
#include <QDebug>
#include <QScrollBar>

PackageManager::PackageManager(QWidget *parent)
    : QWidget(parent)
    , m_mainLayout(nullptr)
    , m_searchLayout(nullptr)
    , m_buttonLayout(nullptr)
    , m_filterLayout(nullptr)
    , m_mainSplitter(nullptr)
    , m_rightSplitter(nullptr)
    , m_searchGroup(nullptr)
    , m_searchEdit(nullptr)
    , m_searchButton(nullptr)
    , m_clearSearchButton(nullptr)
    , m_searchTypeCombo(nullptr)
    , m_filterCombo(nullptr)
    , m_searchStatusLabel(nullptr)
    , m_packageListGroup(nullptr)
    , m_packageTable(nullptr)
    , m_packageCountLabel(nullptr)
    , m_selectAllButton(nullptr)
    , m_deselectAllButton(nullptr)
    , m_actionGroup(nullptr)
    , m_installButton(nullptr)
    , m_removeButton(nullptr)
    , m_updateButton(nullptr)
    , m_updateAllButton(nullptr)
    , m_refreshButton(nullptr)
    , m_detailsGroup(nullptr)
    , m_detailsText(nullptr)
    , m_progressGroup(nullptr)
    , m_progressBar(nullptr)
    , m_progressLabel(nullptr)
    , m_progressOutput(nullptr)
    , m_systemUtils(nullptr)
    , m_privilegedExecutor(nullptr)
    , m_searchWorker(nullptr)
    , m_searchThread(nullptr)
    , m_searchTimer(nullptr)
    , m_isSearching(false)
{
    m_systemUtils = new SystemUtils(this);
    m_privilegedExecutor = new PrivilegedExecutor(this);
    
    // Create timer before setupConnections() to avoid nullptr error
    m_searchTimer = new QTimer(this);
    m_searchTimer->setSingleShot(true);
    m_searchTimer->setInterval(SEARCH_DELAY_MS);
    
    setupUI();
    setupConnections();
    
    m_searchWorker = new PackageSearchWorker();
    m_searchThread = new QThread(this);
    m_searchWorker->moveToThread(m_searchThread);
    
    connect(m_searchThread, &QThread::started, m_searchWorker, &PackageSearchWorker::refreshAllPackages);
    connect(m_searchWorker, &PackageSearchWorker::searchFinished, this, &PackageManager::onSearchFinished);
    connect(m_searchWorker, &PackageSearchWorker::searchError, this, &PackageManager::onPackageActionError);
    connect(m_searchWorker, &PackageSearchWorker::searchProgress, this, &PackageManager::onPackageActionProgress);
    
    m_searchThread->start();
    
    updateButtonStates();
    showProgress("Loading package information...");
}

PackageManager::~PackageManager()
{
    if (m_searchThread) {
        m_searchWorker->cancel();
        m_searchThread->quit();
        m_searchThread->wait(5000);
        delete m_searchWorker;
    }
}

void PackageManager::setSystemUtils(SystemUtils* systemUtils)
{
    m_systemUtils = systemUtils;
}

void PackageManager::setPrivilegedExecutor(PrivilegedExecutor* privilegedExecutor)
{
    m_privilegedExecutor = privilegedExecutor;
}

void PackageManager::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(6);
    m_mainLayout->setContentsMargins(6, 6, 6, 6);
    
    m_mainSplitter = new QSplitter(Qt::Horizontal, this);
    m_mainLayout->addWidget(m_mainSplitter);
    
    QWidget *leftWidget = new QWidget(this);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setSpacing(6);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    
    m_searchGroup = new QGroupBox("Search Packages", leftWidget);
    QVBoxLayout *searchGroupLayout = new QVBoxLayout(m_searchGroup);
    
    m_searchLayout = new QHBoxLayout();
    m_searchEdit = new QLineEdit(m_searchGroup);
    m_searchEdit->setPlaceholderText("Search packages...");
    m_searchButton = new QPushButton(QIcon::fromTheme("edit-find"), "Search", m_searchGroup);
    m_clearSearchButton = new QPushButton(QIcon::fromTheme("edit-clear"), "Clear", m_searchGroup);
    
    m_searchLayout->addWidget(m_searchEdit);
    m_searchLayout->addWidget(m_searchButton);
    m_searchLayout->addWidget(m_clearSearchButton);
    
    m_filterLayout = new QHBoxLayout();
    m_searchTypeCombo = new QComboBox(m_searchGroup);
    m_searchTypeCombo->addItems({"Name", "Summary", "Description", "All"});
    m_filterCombo = new QComboBox(m_searchGroup);
    m_filterCombo->addItems({"All Packages", "Installed Only", "Available Only", "Updates Available"});
    
    m_filterLayout->addWidget(new QLabel("Search in:", m_searchGroup));
    m_filterLayout->addWidget(m_searchTypeCombo);
    m_filterLayout->addWidget(new QLabel("Filter:", m_searchGroup));
    m_filterLayout->addWidget(m_filterCombo);
    m_filterLayout->addStretch();
    
    m_searchStatusLabel = new QLabel("Ready", m_searchGroup);
    
    searchGroupLayout->addLayout(m_searchLayout);
    searchGroupLayout->addLayout(m_filterLayout);
    searchGroupLayout->addWidget(m_searchStatusLabel);
    
    leftLayout->addWidget(m_searchGroup);
    
    m_packageListGroup = new QGroupBox("Package List", leftWidget);
    QVBoxLayout *packageListLayout = new QVBoxLayout(m_packageListGroup);
    
    m_packageTable = new QTableWidget(0, 7, m_packageListGroup);
    QStringList headers = {"Name", "Version", "Arch", "Repository", "Status", "Size", "Summary"};
    m_packageTable->setHorizontalHeaderLabels(headers);
    m_packageTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_packageTable->setAlternatingRowColors(true);
    m_packageTable->setSortingEnabled(true);
    m_packageTable->horizontalHeader()->setStretchLastSection(true);
    m_packageTable->verticalHeader()->setVisible(false);
    
    m_packageTable->setColumnWidth(COLUMN_NAME, 200);
    m_packageTable->setColumnWidth(COLUMN_VERSION, 120);
    m_packageTable->setColumnWidth(COLUMN_ARCH, 80);
    m_packageTable->setColumnWidth(COLUMN_REPO, 120);
    m_packageTable->setColumnWidth(COLUMN_STATUS, 100);
    m_packageTable->setColumnWidth(COLUMN_SIZE, 80);
    
    packageListLayout->addWidget(m_packageTable);
    
    QHBoxLayout *packageControlLayout = new QHBoxLayout();
    m_packageCountLabel = new QLabel("0 packages", m_packageListGroup);
    m_selectAllButton = new QPushButton("Select All", m_packageListGroup);
    m_deselectAllButton = new QPushButton("Deselect All", m_packageListGroup);
    
    packageControlLayout->addWidget(m_packageCountLabel);
    packageControlLayout->addStretch();
    packageControlLayout->addWidget(m_selectAllButton);
    packageControlLayout->addWidget(m_deselectAllButton);
    
    packageListLayout->addLayout(packageControlLayout);
    
    leftLayout->addWidget(m_packageListGroup);
    
    m_mainSplitter->addWidget(leftWidget);
    
    QWidget *rightWidget = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setSpacing(6);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    
    m_actionGroup = new QGroupBox("Actions", rightWidget);
    QGridLayout *actionLayout = new QGridLayout(m_actionGroup);
    
    m_installButton = new QPushButton(QIcon::fromTheme("list-add"), "Install", m_actionGroup);
    m_removeButton = new QPushButton(QIcon::fromTheme("list-remove"), "Remove", m_actionGroup);
    m_updateButton = new QPushButton(QIcon::fromTheme("system-upgrade"), "Update", m_actionGroup);
    m_updateAllButton = new QPushButton(QIcon::fromTheme("system-upgrade"), "Update All", m_actionGroup);
    m_refreshButton = new QPushButton(QIcon::fromTheme("view-refresh"), "Refresh", m_actionGroup);
    
    actionLayout->addWidget(m_installButton, 0, 0);
    actionLayout->addWidget(m_removeButton, 0, 1);
    actionLayout->addWidget(m_updateButton, 1, 0);
    actionLayout->addWidget(m_updateAllButton, 1, 1);
    actionLayout->addWidget(m_refreshButton, 2, 0, 1, 2);
    
    rightLayout->addWidget(m_actionGroup);
    
    m_detailsGroup = new QGroupBox("Package Details", rightWidget);
    QVBoxLayout *detailsLayout = new QVBoxLayout(m_detailsGroup);
    
    m_detailsText = new QTextEdit(m_detailsGroup);
    m_detailsText->setReadOnly(true);
    m_detailsText->setMaximumHeight(200);
    
    detailsLayout->addWidget(m_detailsText);
    
    rightLayout->addWidget(m_detailsGroup);
    
    m_progressGroup = new QGroupBox("Progress", rightWidget);
    QVBoxLayout *progressLayout = new QVBoxLayout(m_progressGroup);
    
    m_progressBar = new QProgressBar(m_progressGroup);
    m_progressBar->setVisible(false);
    m_progressLabel = new QLabel("Ready", m_progressGroup);
    
    m_progressOutput = new QTextEdit(m_progressGroup);
    m_progressOutput->setReadOnly(true);
    m_progressOutput->setMaximumHeight(150);
    m_progressOutput->setStyleSheet("QTextEdit { background-color: #2b2b2b; color: #ffffff; font-family: monospace; font-size: 9pt; }");
    
    progressLayout->addWidget(m_progressBar);
    progressLayout->addWidget(m_progressLabel);
    progressLayout->addWidget(m_progressOutput);
    
    rightLayout->addWidget(m_progressGroup);
    
    m_mainSplitter->addWidget(rightWidget);
    
    m_mainSplitter->setSizes({600, 400});
    m_mainSplitter->setStretchFactor(0, 1);
    m_mainSplitter->setStretchFactor(1, 0);
}

void PackageManager::setupConnections()
{
    connect(m_searchEdit, &QLineEdit::textChanged, this, &PackageManager::onSearchTextChanged);
    connect(m_searchButton, &QPushButton::clicked, this, &PackageManager::searchPackages);
    connect(m_clearSearchButton, &QPushButton::clicked, this, &PackageManager::clearSearch);
    connect(m_searchTimer, &QTimer::timeout, this, &PackageManager::onSearchTimeout);
    connect(m_filterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PackageManager::onFilterChanged);
    
    connect(m_packageTable, &QTableWidget::cellClicked, this, &PackageManager::showPackageDetails);
    connect(m_packageTable, &QTableWidget::itemSelectionChanged, this, &PackageManager::onPackageSelectionChanged);
    
    connect(m_selectAllButton, &QPushButton::clicked, this, &PackageManager::selectAllPackages);
    connect(m_deselectAllButton, &QPushButton::clicked, this, &PackageManager::deselectAllPackages);
    
    connect(m_installButton, &QPushButton::clicked, this, &PackageManager::installSelectedPackages);
    connect(m_removeButton, &QPushButton::clicked, this, &PackageManager::removeSelectedPackages);
    connect(m_updateButton, &QPushButton::clicked, this, &PackageManager::updateSelectedPackages);
    connect(m_updateAllButton, &QPushButton::clicked, this, &PackageManager::updateAllPackages);
    connect(m_refreshButton, &QPushButton::clicked, this, &PackageManager::refreshPackageList);
    
    connect(m_privilegedExecutor, &PrivilegedExecutor::taskFinished, 
            [this](int taskId, int exitCode, const QString &output) {
                Q_UNUSED(taskId); Q_UNUSED(exitCode);
                onPackageActionSuccess(output);
            });
    connect(m_privilegedExecutor, &PrivilegedExecutor::taskError,
            [this](int taskId, const QString &error) {
                Q_UNUSED(taskId);
                onPackageActionError(error);
            });
    connect(m_privilegedExecutor, &PrivilegedExecutor::taskProgress,
            [this](int taskId, const QString &progress) {
                Q_UNUSED(taskId);
                onPackageActionProgress(progress);
            });
}

void PackageManager::refreshPackageList()
{
    if (m_isSearching) {
        m_searchWorker->cancel();
    }
    
    showProgress("Refreshing package list...");
    m_searchStatusLabel->setText("Refreshing...");
    
    QMetaObject::invokeMethod(m_searchWorker, "refreshAllPackages", Qt::QueuedConnection);
}

void PackageManager::searchPackages()
{
    QString searchTerm = m_searchEdit->text().trimmed();
    if (searchTerm.isEmpty()) {
        refreshPackageList();
        return;
    }
    
    startSearch();
}

void PackageManager::startSearch()
{
    QString searchTerm = m_searchEdit->text().trimmed();
    QString searchType = m_searchTypeCombo->currentText().toLower();
    
    if (searchTerm.isEmpty()) {
        return;
    }
    
    m_currentSearchTerm = searchTerm;
    m_isSearching = true;
    
    showProgress(QString("Searching for '%1'...").arg(searchTerm));
    m_searchStatusLabel->setText(QString("Searching for '%1'...").arg(searchTerm));
    
    QMetaObject::invokeMethod(m_searchWorker, "searchPackages", Qt::QueuedConnection,
                            Q_ARG(QString, searchTerm), Q_ARG(QString, searchType));
}

void PackageManager::clearSearch()
{
    m_searchEdit->clear();
    m_currentSearchTerm.clear();
    refreshPackageList();
}

void PackageManager::onSearchTextChanged()
{
    m_searchTimer->stop();
    m_searchTimer->start();
}

void PackageManager::onSearchTimeout()
{
    if (!m_searchEdit->text().trimmed().isEmpty()) {
        startSearch();
    }
}

void PackageManager::onFilterChanged()
{
    applyPackageFilter();
}

void PackageManager::onSearchFinished(const QList<PackageInfo> &packages)
{
    m_isSearching = false;
    m_allPackages = packages;
    
    hideProgress();
    
    if (m_currentSearchTerm.isEmpty()) {
        m_searchStatusLabel->setText(QString("Found %1 packages").arg(packages.size()));
    } else {
        m_searchStatusLabel->setText(QString("Found %1 packages for '%2'").arg(packages.size()).arg(m_currentSearchTerm));
    }
    
    applyPackageFilter();
}

void PackageManager::applyPackageFilter()
{
    QString filter = m_filterCombo->currentText();
    m_filteredPackages.clear();
    
    for (const PackageInfo &pkg : m_allPackages) {
        bool include = false;
        
        if (filter == "All Packages") {
            include = true;
        } else if (filter == "Installed Only") {
            include = pkg.isInstalled;
        } else if (filter == "Available Only") {
            include = !pkg.isInstalled;
        } else if (filter == "Updates Available") {
            include = pkg.isUpdateAvailable;
        }
        
        if (include) {
            m_filteredPackages.append(pkg);
        }
    }
    
    updatePackageTable(m_filteredPackages);
}

void PackageManager::updatePackageTable(const QList<PackageInfo> &packages)
{
    m_packageTable->setRowCount(packages.size());
    m_packageTable->setSortingEnabled(false);
    
    for (int i = 0; i < packages.size(); ++i) {
        const PackageInfo &pkg = packages[i];
        
        QTableWidgetItem *nameItem = new QTableWidgetItem(pkg.name);
        nameItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
        nameItem->setCheckState(Qt::Unchecked);
        m_packageTable->setItem(i, COLUMN_NAME, nameItem);
        
        QString version = pkg.version;
        if (pkg.isUpdateAvailable && !pkg.updateVersion.isEmpty()) {
            version += QString(" -> %1").arg(pkg.updateVersion);
        }
        QTableWidgetItem *versionItem = new QTableWidgetItem(version);
        versionItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_packageTable->setItem(i, COLUMN_VERSION, versionItem);
        
        QTableWidgetItem *archItem = new QTableWidgetItem(pkg.arch);
        archItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_packageTable->setItem(i, COLUMN_ARCH, archItem);
        
        QTableWidgetItem *repoItem = new QTableWidgetItem(pkg.repository);
        repoItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_packageTable->setItem(i, COLUMN_REPO, repoItem);
        
        QString status;
        if (pkg.isInstalled) {
            status = pkg.isUpdateAvailable ? "Update Available" : "Installed";
        } else {
            status = "Available";
        }
        QTableWidgetItem *statusItem = new QTableWidgetItem(status);
        statusItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        
        if (pkg.isInstalled) {
            if (pkg.isUpdateAvailable) {
                statusItem->setBackground(QColor(255, 165, 0, 100));
            } else {
                statusItem->setBackground(QColor(0, 255, 0, 100));
            }
        }
        
        m_packageTable->setItem(i, COLUMN_STATUS, statusItem);
        
        QTableWidgetItem *sizeItem = new QTableWidgetItem(pkg.size);
        sizeItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_packageTable->setItem(i, COLUMN_SIZE, sizeItem);
        
        QTableWidgetItem *summaryItem = new QTableWidgetItem(pkg.summary);
        summaryItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_packageTable->setItem(i, COLUMN_SUMMARY, summaryItem);
    }
    
    m_packageTable->setSortingEnabled(true);
    m_packageTable->sortItems(COLUMN_NAME, Qt::AscendingOrder);
    
    m_packageCountLabel->setText(QString("%1 packages").arg(packages.size()));
    
    updateButtonStates();
}

void PackageManager::showPackageDetails(int row, int column)
{
    if (row < 0 || row >= m_filteredPackages.size()) {
        return;
    }
    
    // If user clicked on the name column (checkbox column), update button states
    if (column == COLUMN_NAME) {
        updateButtonStates();
    }
    
    const PackageInfo &pkg = m_filteredPackages[row];
    updatePackageDetails(pkg);
}

void PackageManager::updatePackageDetails(const PackageInfo &package)
{
    QString details = QString(
        "<h3>%1</h3>"
        "<p><b>Version:</b> %2</p>"
        "<p><b>Architecture:</b> %3</p>"
        "<p><b>Repository:</b> %4</p>"
        "<p><b>Size:</b> %5</p>"
        "<p><b>Status:</b> %6</p>"
        "%7"
        "<p><b>Summary:</b> %8</p>"
        "<p><b>Description:</b><br>%9</p>"
    ).arg(package.name)
     .arg(package.version)
     .arg(package.arch)
     .arg(package.repository)
     .arg(package.size)
     .arg(package.isInstalled ? "Installed" : "Available")
     .arg(package.isUpdateAvailable ? QString("<p><b>Update Available:</b> %1</p>").arg(package.updateVersion) : "")
     .arg(package.summary)
     .arg(package.description.isEmpty() ? "No description available." : package.description);
    
    m_detailsText->setHtml(details);
}

void PackageManager::onPackageSelectionChanged()
{
    updateButtonStates();
}

void PackageManager::updateButtonStates()
{
    QList<PackageInfo> selectedPackages = getSelectedPackages();
    
    bool hasSelection = !selectedPackages.isEmpty();
    bool hasInstalled = false;
    bool hasAvailable = false;
    bool hasUpdates = false;
    
    for (const PackageInfo &pkg : selectedPackages) {
        if (pkg.isInstalled) {
            hasInstalled = true;
        }
        if (!pkg.isInstalled) {
            hasAvailable = true;
        }
        if (pkg.isUpdateAvailable) {
            hasUpdates = true;
        }
    }
    
    m_installButton->setEnabled(hasAvailable);
    m_removeButton->setEnabled(hasInstalled);
    m_updateButton->setEnabled(hasUpdates);
    m_updateAllButton->setEnabled(true);
    m_refreshButton->setEnabled(true);
    
    m_selectAllButton->setEnabled(m_packageTable->rowCount() > 0);
    m_deselectAllButton->setEnabled(hasSelection);
}

QList<PackageInfo> PackageManager::getSelectedPackages()
{
    QList<PackageInfo> selectedPackages;
    
    for (int i = 0; i < m_packageTable->rowCount(); ++i) {
        QTableWidgetItem *item = m_packageTable->item(i, COLUMN_NAME);
        if (item && item->checkState() == Qt::Checked) {
            if (i < m_filteredPackages.size()) {
                selectedPackages.append(m_filteredPackages[i]);
            }
        }
    }
    
    return selectedPackages;
}

void PackageManager::selectAllPackages()
{
    for (int i = 0; i < m_packageTable->rowCount(); ++i) {
        QTableWidgetItem *item = m_packageTable->item(i, COLUMN_NAME);
        if (item) {
            item->setCheckState(Qt::Checked);
        }
    }
    updateButtonStates();
}

void PackageManager::deselectAllPackages()
{
    for (int i = 0; i < m_packageTable->rowCount(); ++i) {
        QTableWidgetItem *item = m_packageTable->item(i, COLUMN_NAME);
        if (item) {
            item->setCheckState(Qt::Unchecked);
        }
    }
    updateButtonStates();
}

void PackageManager::installSelectedPackages()
{
    QList<PackageInfo> selectedPackages = getSelectedPackages();
    QStringList packageNames;
    
    for (const PackageInfo &pkg : selectedPackages) {
        if (!pkg.isInstalled) {
            packageNames.append(pkg.name);
        }
    }
    
    if (packageNames.isEmpty()) {
        QMessageBox::information(this, "No Packages", "No available packages selected for installation.");
        return;
    }
    
    QString message = QString("Are you sure you want to install the following packages?\n\n%1")
                     .arg(packageNames.join(", "));
    
    int result = QMessageBox::question(this, "Install Packages", message,
                                     QMessageBox::Yes | QMessageBox::No);
    
    if (result == QMessageBox::Yes) {
        showProgress("Installing packages...");
        m_privilegedExecutor->installPackages(packageNames, this, nullptr, nullptr, nullptr);
    }
}

void PackageManager::removeSelectedPackages()
{
    QList<PackageInfo> selectedPackages = getSelectedPackages();
    QStringList packageNames;
    
    for (const PackageInfo &pkg : selectedPackages) {
        if (pkg.isInstalled) {
            packageNames.append(pkg.name);
        }
    }
    
    if (packageNames.isEmpty()) {
        QMessageBox::information(this, "No Packages", "No installed packages selected for removal.");
        return;
    }
    
    QString message = QString("Are you sure you want to remove the following packages?\n\n%1")
                     .arg(packageNames.join(", "));
    
    int result = QMessageBox::question(this, "Remove Packages", message,
                                     QMessageBox::Yes | QMessageBox::No);
    
    if (result == QMessageBox::Yes) {
        showProgress("Removing packages...");
        m_privilegedExecutor->removePackages(packageNames, this, nullptr, nullptr, nullptr);
    }
}

void PackageManager::updateSelectedPackages()
{
    QList<PackageInfo> selectedPackages = getSelectedPackages();
    QStringList packageNames;
    
    for (const PackageInfo &pkg : selectedPackages) {
        if (pkg.isUpdateAvailable) {
            packageNames.append(pkg.name);
        }
    }
    
    if (packageNames.isEmpty()) {
        QMessageBox::information(this, "No Updates", "No packages with available updates selected.");
        return;
    }
    
    QString message = QString("Are you sure you want to update the following packages?\n\n%1")
                     .arg(packageNames.join(", "));
    
    int result = QMessageBox::question(this, "Update Packages", message,
                                     QMessageBox::Yes | QMessageBox::No);
    
    if (result == QMessageBox::Yes) {
        showProgress("Updating packages...");
        m_privilegedExecutor->updatePackages(packageNames, this, nullptr, nullptr, nullptr);
    }
}

void PackageManager::updateAllPackages()
{
    QString message = "Are you sure you want to update all packages on the system?";
    int result = QMessageBox::question(this, "Update All Packages", message,
                                     QMessageBox::Yes | QMessageBox::No);
    
    if (result == QMessageBox::Yes) {
        showProgress("Updating all packages...");
        m_privilegedExecutor->updatePackages(QStringList(), this, nullptr, nullptr, nullptr);
    }
}

void PackageManager::onPackageActionSuccess(const QString &output)
{
    hideProgress();
    m_progressOutput->append(QString("[%1] Operation completed successfully")
                           .arg(QDateTime::currentDateTime().toString()));
    
    if (!output.isEmpty()) {
        m_progressOutput->append(output);
    }
    
    // Scroll to bottom to show latest progress
    QScrollBar *scrollBar = m_progressOutput->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
    
    // Force refresh of package list
    QTimer::singleShot(1000, this, [this]() {
        refreshPackageList();
    });
}

void PackageManager::onPackageActionError(const QString &error)
{
    hideProgress();
    m_progressOutput->append(QString("[%1] Operation failed: %2")
                           .arg(QDateTime::currentDateTime().toString())
                           .arg(error));
    
    // Scroll to bottom to show latest progress
    QScrollBar *scrollBar = m_progressOutput->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
    
    QMessageBox::critical(this, "Operation Failed", error);
}

void PackageManager::onPackageActionProgress(const QString &progress)
{
    if (!progress.isEmpty()) {
        m_progressOutput->append(progress);
        
        // Scroll to bottom to show latest progress
        QScrollBar *scrollBar = m_progressOutput->verticalScrollBar();
        scrollBar->setValue(scrollBar->maximum());
    }
}

void PackageManager::showProgress(const QString &message)
{
    m_progressBar->setVisible(true);
    m_progressBar->setRange(0, 0);
    m_progressLabel->setText(message);
}

void PackageManager::hideProgress()
{
    m_progressBar->setVisible(false);
    m_progressLabel->setText("Ready");
}

PackageSearchWorker::PackageSearchWorker(QObject *parent)
    : QObject(parent)
    , m_process(nullptr)
    , m_cancelled(false)
{
}

void PackageSearchWorker::searchPackages(const QString &searchTerm, const QString &searchType)
{
    QMutexLocker locker(&m_mutex);
    m_cancelled = false;
    
    if (m_process) {
        m_process->kill();
        m_process->deleteLater();
    }
    
    m_process = new QProcess(this);
    m_process->setProcessChannelMode(QProcess::MergedChannels);
    
    QStringList args;
    args << "search" << "--quiet" << searchTerm;
    
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [this](int exitCode, QProcess::ExitStatus exitStatus) {
                if (m_cancelled) return;
                
                if (exitCode == 0) {
                    QString output = m_process->readAll();
                    QList<PackageInfo> packages = parsePackageList(output);
                    emit searchFinished(packages);
                } else {
                    emit searchError("Search failed");
                }
                
                m_process->deleteLater();
                m_process = nullptr;
            });
    
    m_process->start("dnf", args);
}

void PackageSearchWorker::refreshAllPackages()
{
    QMutexLocker locker(&m_mutex);
    m_cancelled = false;
    
    emit searchProgress("Loading package information...");
    
    // Get installed packages first
    QProcess *installedProcess = new QProcess(this);
    connect(installedProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, installedProcess](int exitCode, QProcess::ExitStatus exitStatus) {
        if (m_cancelled) {
            installedProcess->deleteLater();
            return;
        }
        
        QMap<QString, PackageInfo> packageMap;
        
        if (exitCode == 0) {
            QString installedOutput = installedProcess->readAllStandardOutput();
            QList<PackageInfo> installedPackages = parsePackageList(installedOutput, true);
            
            // Add installed packages to map
            for (const PackageInfo &pkg : installedPackages) {
                packageMap[pkg.name] = pkg;
            }
        }
        
        installedProcess->deleteLater();
        
        emit searchProgress("Checking for available updates...");
        
        // Get available packages
        QProcess *availableProcess = new QProcess(this);
        connect(availableProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this, [this, availableProcess, packageMap](int exitCode, QProcess::ExitStatus exitStatus) mutable {
            if (m_cancelled) {
                availableProcess->deleteLater();
                return;
            }
            
            if (exitCode == 0) {
                QString availableOutput = availableProcess->readAllStandardOutput();
                QList<PackageInfo> availablePackages = parsePackageList(availableOutput, false);
                
                // Merge available packages with installed ones
                for (const PackageInfo &pkg : availablePackages) {
                    if (packageMap.contains(pkg.name)) {
                        // Package is installed, check if there's an update
                        PackageInfo &installedPkg = packageMap[pkg.name];
                        if (pkg.version != installedPkg.version) {
                            installedPkg.isUpdateAvailable = true;
                            installedPkg.updateVersion = pkg.version;
                        }
                    } else {
                        // Package is available but not installed
                        packageMap[pkg.name] = pkg;
                    }
                }
            }
            
            availableProcess->deleteLater();
            
            QList<PackageInfo> allPackages = packageMap.values();
            emit searchFinished(allPackages);
        });
        
        availableProcess->start("dnf", QStringList() << "list" << "available" << "--quiet");
    });
    
    installedProcess->start("dnf", QStringList() << "list" << "installed" << "--quiet");
}

void PackageSearchWorker::cancel()
{
    QMutexLocker locker(&m_mutex);
    m_cancelled = true;
    
    if (m_process) {
        m_process->kill();
    }
}

QList<PackageInfo> PackageSearchWorker::parsePackageList(const QString &output, bool installedOnly)
{
    QList<PackageInfo> packages;
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    
    for (const QString &line : lines) {
        if (line.startsWith("Last metadata") || line.startsWith("Available") || 
            line.startsWith("Installed") || line.isEmpty()) {
            continue;
        }
        
        QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (parts.size() >= 3) {
            PackageInfo pkg;
            
            QString nameArch = parts[0];
            int dotPos = nameArch.lastIndexOf('.');
            if (dotPos > 0) {
                pkg.name = nameArch.left(dotPos);
                pkg.arch = nameArch.mid(dotPos + 1);
            } else {
                pkg.name = nameArch;
                pkg.arch = "noarch";
            }
            
            pkg.version = parts[1];
            pkg.repository = parts[2];
            pkg.isInstalled = installedOnly;
            pkg.isUpdateAvailable = false;
            
            packages.append(pkg);
        }
    }
    
    return packages;
}

QList<PackageInfo> PackageSearchWorker::parsePackageInfo(const QString &output)
{
    QList<PackageInfo> packages;
    QStringList blocks = output.split("\n\n", Qt::SkipEmptyParts);
    
    for (const QString &block : blocks) {
        PackageInfo pkg = parsePackageInfoBlock(block);
        if (!pkg.name.isEmpty()) {
            packages.append(pkg);
        }
    }
    
    return packages;
}

PackageInfo PackageSearchWorker::parsePackageInfoBlock(const QString &block)
{
    PackageInfo pkg;
    QStringList lines = block.split('\n', Qt::SkipEmptyParts);
    
    for (const QString &line : lines) {
        if (line.startsWith("Name")) {
            pkg.name = line.split(":").last().trimmed();
        } else if (line.startsWith("Version")) {
            pkg.version = line.split(":").last().trimmed();
        } else if (line.startsWith("Arch")) {
            pkg.arch = line.split(":").last().trimmed();
        } else if (line.startsWith("From repo")) {
            pkg.repository = line.split(":").last().trimmed();
        } else if (line.startsWith("Summary")) {
            pkg.summary = line.split(":").last().trimmed();
        } else if (line.startsWith("Description")) {
            pkg.description = line.split(":").last().trimmed();
        } else if (line.startsWith("Size")) {
            pkg.size = line.split(":").last().trimmed();
        }
    }
    
    return pkg;
}

QString PackageSearchWorker::formatPackageSize(qint64 bytes)
{
    const QStringList units = {"B", "KB", "MB", "GB", "TB"};
    int unitIndex = 0;
    double size = bytes;
    
    while (size >= 1024.0 && unitIndex < units.size() - 1) {
        size /= 1024.0;
        unitIndex++;
    }
    
    return QString("%1 %2").arg(QString::number(size, 'f', 1)).arg(units[unitIndex]);
}

 