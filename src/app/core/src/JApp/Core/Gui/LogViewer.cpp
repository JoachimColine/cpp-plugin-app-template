#include <JApp/Core/Gui/LogViewer.h>
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

    m_headerLabel = new QLabel("Log Viewer", this);
    QFont headerFont = m_headerLabel->font();
    headerFont.setBold(true);
    headerFont.setPointSize(headerFont.pointSize() + 2);
    m_headerLabel->setFont(headerFont);
    m_mainLayout->addWidget(m_headerLabel);

    m_logTableView = new QTableView(this);
    m_logTableView->setAlternatingRowColors(true);
    m_logTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_logTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_logTableView->setSortingEnabled(true);
    m_logTableView->horizontalHeader()->setStretchLastSection(true);
    m_logTableView->verticalHeader()->setVisible(false);
    m_logTableView->setModel(m_logModel);
    m_mainLayout->addWidget(m_logTableView);

    setLayout(m_mainLayout);
    resize(800, 600);

    LOG_INFO() << "ok";
}
