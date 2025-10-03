#include "JApp/Plugins/PluginManager.h"
#include "JApp/Plugins/Plugin.h"
#include "JApp/Plugins/LoadPluginsTask.h"
#include <JApp/Log.h>

#include <QTimer>
#include <QPluginLoader>
#include <QDir>
#include <QFileInfo>
#include <QThread>
#include <QTimer>

using namespace JApp;

PluginManager::PluginManager(QString directory, QObject *parent) : QObject(parent)
    , m_directory(directory)
    , m_loadingProgress(0.0)
    , m_loadingMessage("")
    , m_initializationProgress(0.0)
    , m_initializationMessage("")
    , m_loadPluginsTaskThread(nullptr)
    , m_loadPluginsTask(nullptr)
{

}

PluginManager::~PluginManager()
{
    if (m_loadPluginsTaskThread && m_loadPluginsTaskThread->isRunning()) {
        m_loadPluginsTaskThread->quit();
        m_loadPluginsTaskThread->wait();
    }

    if (m_loadPluginsTaskThread) {
        m_loadPluginsTaskThread->deleteLater();
    }
}

QString JApp::PluginManager::directory() const
{
    return m_directory;
}

qreal JApp::PluginManager::loadingProgress() const
{
    return m_loadingProgress;
}

QString PluginManager::loadingMessage() const
{
    return m_loadingMessage;
}

qreal PluginManager::initializationProgress() const
{
    return m_initializationProgress;
}

QString PluginManager::initializationMessage() const
{
    return m_initializationMessage;
}


bool JApp::PluginManager::loadPlugins()
{
    /* TODO
    if (false) {
        LOG_WARN() << "Already loading plugins";
        return false;
    }
    */

    QDir pluginsDir(m_directory);

#if defined(Q_OS_WIN)
    const QStringList filters = {"*.dll"};
#else
    const QStringList filters = {"*.so"};
#endif

    QFileInfoList files = pluginsDir.entryInfoList(filters, QDir::Files);

    if (files.isEmpty()) {
        LOG_WARN() << "No plugins found in directory:" << m_directory;
        return false;
    }

    setLoadingProgress(0.0);

    m_loadPluginsTaskThread = new QThread();
    m_loadPluginsTask = new LoadPluginsTask();
    m_loadPluginsTask->setPluginFiles(files);
    m_loadPluginsTask->moveToThread(m_loadPluginsTaskThread);

    connect(m_loadPluginsTask, &LoadPluginsTask::taskUpdated, this, &PluginManager::onLoadingTaskUpdated);
    connect(m_loadPluginsTask, &LoadPluginsTask::pluginLoaded, this, &PluginManager::onPluginLoaded);
    connect(m_loadPluginsTask, &LoadPluginsTask::taskFinished, this, &PluginManager::onLoadingTaskFinished);
    connect(m_loadPluginsTaskThread, &QThread::started, m_loadPluginsTask, &LoadPluginsTask::start);
    connect(m_loadPluginsTaskThread, &QThread::finished, this, &PluginManager::cleanUpTaskThread);

    m_loadPluginsTaskThread->start();

    return true;
}


QList<JApp::Plugin*> JApp::PluginManager::plugins() const
{
    return m_initializedPlugins;
}

void PluginManager::onPluginLoaded(QPluginLoader *loader, QObject *plugin)
{
    if (loader == nullptr) {
        LOG_WARN() << "Loader is null after loading";
        return;
    }

    if (plugin == nullptr) {
        LOG_WARN() << "Plugin is null after loading: " << loader->fileName();
        loader->deleteLater();
        return;
    }

    JApp::Plugin* jappPlugin = qobject_cast<JApp::Plugin*>(plugin);
    if (jappPlugin == nullptr) {
        LOG_WARN() << "Loaded plugin does not implement JApp plugin interface: " << loader->fileName();
        loader->deleteLater();
        plugin->deleteLater();
        return;
    }

    m_loaders.append(loader);
    m_pluginsToInitialize.enqueue(jappPlugin);
    if (m_pluginsToInitialize.size() == 1)
        // Let some time go to process events, e.g. splashscreen info update
        QTimer::singleShot(10, this, [this]() { processPluginInitializationQueue(); });
}

void PluginManager::onPluginError(QString pluginFile, QString errorMessage)
{
    LOG_WARN() << QString("Error while loading %1: %2").arg(pluginFile).arg(errorMessage);
}

void PluginManager::onLoadingTaskUpdated(QString loadingMessage)
{
    setLoadingMessage(loadingMessage);

}

void PluginManager::onLoadingTaskFinished(bool success, QString message)
{
    emit pluginsLoaded();
    setLoadingProgress(1.0);
}

void JApp::PluginManager::setLoadingProgress(qreal progress)
{
    if (qFuzzyCompare(progress, m_loadingProgress))
        return;

    m_loadingProgress = progress;
    emit loadingProgressChanged(m_loadingProgress);
}

void PluginManager::setLoadingMessage(const QString &loadingMessage)
{
    if (m_loadingMessage == loadingMessage)
        return;

    m_loadingMessage = loadingMessage;
    emit loadingMessageChanged(m_loadingMessage);
}

void PluginManager::setInitializationProgress(qreal progress)
{
    if (qFuzzyCompare(progress, m_initializationProgress))
        return;

    m_initializationProgress = progress;
    emit initializationProgressChanged(m_initializationProgress);
}

void PluginManager::setInitializationMessage(const QString &initializationMessage)
{
    if (m_initializationMessage == initializationMessage)
        return;

    m_initializationMessage = initializationMessage;
    emit initializationMessageChanged(m_initializationMessage);
}

void JApp::PluginManager::cleanUpTaskThread()
{
    LOG_INFO() << "Cleaning up load plugins task thread";

    if (m_loadPluginsTask) {
        m_loadPluginsTask = nullptr;
    }

    if (m_loadPluginsTaskThread) {
        m_loadPluginsTaskThread->deleteLater();
        m_loadPluginsTaskThread = nullptr;
    }
}

void PluginManager::processPluginInitializationQueue()
{
    if (m_pluginsToInitialize.isEmpty()) {
        return;
    }
    JApp::Plugin* p = m_pluginsToInitialize.dequeue();
    p->initialize();
    m_initializedPlugins.append(p);
    setLoadingProgress(m_initializedPlugins.size());

    // Let some time go to process events, e.g. splashscreen info update
    QTimer::singleShot(10, this, [this]() { processPluginInitializationQueue(); });
}
