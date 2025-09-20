#include <JApp/Core/Gui/LogViewer.h>
#include <JApp/Core/Models/FileWatcherLogModel.h>
#include <JApp/Log.h>

using namespace JApp::Core::Gui;

LogViewer::LogViewer(QWidget *parent,
                     JApp::Logger* logger)
    : QWidget(parent)
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
    auto logModel = new Models::FileWatcherLogModel(this);
    logModel->setLogFilePath(m_logger->logFilePath());
    m_logModel = logModel;
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

    m_logListView = new QListView(this);
    m_logListView->setAlternatingRowColors(true);
    m_logListView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_logListView->setModel(m_logModel);
    m_mainLayout->addWidget(m_logListView);

    setLayout(m_mainLayout);
    resize(800, 600);

    LOG_INFO() << "ok";
}
