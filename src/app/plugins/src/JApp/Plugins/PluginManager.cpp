#include "JApp/Plugins/PluginManager.h"
#include "JApp/Plugins/Plugin.h"
#include "JApp/Plugins/LoadPluginsTask.h"
#include <JApp/Log.h>

#include <QTimer>
#include <QPluginLoader>
#include <QDir>
#include <QFileInfo>
#include <QThread>

using namespace JApp;

PluginManager::PluginManager(QString directory, QObject *parent) : QObject(parent)
    , m_directory(directory)
    , m_loadingProgress(0.0)
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


QList<JApp::Plugin*> JApp::PluginManager::loadedPlugins() const
{
    return m_plugins;
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

    // TODO start initializing

    m_loaders.append(loader);
    m_plugins.append(jappPlugin);
}

void PluginManager::onPluginError(QString pluginFile, QString errorMessage)
{
    LOG_WARN() << QString("Error while loading %1: %2").arg(pluginFile).arg(errorMessage);
}

void PluginManager::onLoadingTaskUpdated(qreal loadingProgress, QString loadingMessage)
{
    setLoadingProgress(loadingProgress);
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

void JApp::PluginManager::cleanUpTaskThread()
{
    if (m_loadPluginsTask) {
        m_loadPluginsTask = nullptr;
    }

    if (m_loadPluginsTaskThread) {
        m_loadPluginsTaskThread->deleteLater();
        m_loadPluginsTaskThread = nullptr;
    }
}
