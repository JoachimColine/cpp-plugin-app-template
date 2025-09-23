#include <JApp/Core/Gui/LogViewer.h>
#include <JApp/Core/Gui/LogLevelDelegate.h>
#include <JApp/Core/Models/FileWatcherLogModel.h>
#include <JApp/Log.h>
#include <QHeaderView>

using namespace JApp::Core::Gui;

LogViewer::LogViewer(QWidget *parent,
                     JApp::Core::Models::LogModel* logModel,
                     JApp::Logger* logger)
    : QWidget(parent)
    , m_logModel(logModel)
    , m_logger(logger)
{
    initialize();
    buildWidget();
}

LogViewer::~LogViewer()
{
}

void LogViewer::onRowClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        m_logTableView->resizeRowToContents(index.row());
    }
}

void LogViewer::initialize()
{
    if (m_logModel == nullptr) {
        auto logModel = new Models::FileWatcherLogModel(this);
        logModel->setLogFilePath(m_logger->logFilePath());
        m_logModel = logModel;
    }
}

void LogViewer::buildWidget()
{
    m_mainLayout = new QVBoxLayout(this);

    m_logTableView = new QTableView(this);
    m_logTableView->setAlternatingRowColors(true);
    m_logTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_logTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_logTableView->setSortingEnabled(true);
    m_logTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    m_logTableView->horizontalHeader()->setStretchLastSection(true);
    m_logTableView->verticalHeader()->setVisible(false);
    m_logTableView->setItemDelegateForColumn(1, new LogLevelDelegate(this));

    m_logTableView->setModel(m_logModel);

    m_logTableView->setWordWrap(true);

    m_logTableView->setColumnWidth(0, 135);
    m_logTableView->setColumnWidth(1, 50);
    m_logTableView->setColumnWidth(4, 20);

    m_mainLayout->addWidget(m_logTableView);

    setLayout(m_mainLayout);
    resize(1600, 800);

    connect(m_logTableView, &QTableView::clicked, this, &LogViewer::onRowClicked);

}
