#include "JApp/Plugins/PluginManager.h"
#include "JApp/Plugins/Plugin.h"
#include "JApp/Plugins/LoadTask.h"
#include <JApp/Log.h>

#include <QCoreApplication>
#include <QTimer>
#include <QPluginLoader>
#include <QDir>
#include <QFileInfo>
#include <QThread>
#include <QTimer>

using namespace JApp;

PluginManager::PluginManager(QString directory, QObject *parent) : QObject(parent)
    , m_directory(directory)
    , m_files(QFileInfoList())
    , m_loadProgress(0.0)
    , m_loadMessage("")
    , m_initializationProgress(0.0)
    , m_initializationMessage("")
    , m_loadTaskThread(nullptr)
    , m_loadTask(nullptr)
{

}

PluginManager::~PluginManager()
{
    if (m_loadTaskThread && m_loadTaskThread->isRunning()) {
        m_loadTaskThread->quit();
        m_loadTaskThread->wait();
    }

    if (m_loadTaskThread) {
        m_loadTaskThread->deleteLater();
    }
}

QString JApp::PluginManager::directory() const
{
    return m_directory;
}

qreal JApp::PluginManager::loadProgress() const
{
    return m_loadProgress;
}

QString PluginManager::loadMessage() const
{
    return m_loadMessage;
}

qreal PluginManager::initializationProgress() const
{
    return m_initializationProgress;
}

QString PluginManager::initializationMessage() const
{
    return m_initializationMessage;
}


bool JApp::PluginManager::load()
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

    m_files = pluginsDir.entryInfoList(filters, QDir::Files);

    if (m_files.isEmpty()) {
        LOG_WARN() << "No plugins found in directory:" << m_directory;
        return false;
    }

    m_loadTaskThread = new QThread();
    m_loadTask = new LoadTask();
    m_loadTask->setPluginFiles(m_files);
    m_loadTask->moveToThread(m_loadTaskThread);

    connect(m_loadTask, &LoadTask::taskMessageChanged, this, &PluginManager::onLoadTaskMessageChanged);
    connect(m_loadTask, &LoadTask::pluginLoaded, this, &PluginManager::onPluginLoaded);
    connect(m_loadTask, &LoadTask::taskFinished, this, &PluginManager::onLoadTaskFinished);
    connect(m_loadTaskThread, &QThread::started, m_loadTask, &LoadTask::start);
    connect(m_loadTaskThread, &QThread::finished, this, &PluginManager::cleanUpTaskThread);

    m_loadTaskThread->start();

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
    setLoadProgress(qreal(m_loaders.count()) / qreal(m_files.count()));
    m_pluginsToInitialize.enqueue(jappPlugin);
    if (m_pluginsToInitialize.size() == 1)
        QTimer::singleShot(0, this, &PluginManager::processPluginInitializationQueue);
}

void PluginManager::onPluginError(QString pluginFile, QString errorMessage)
{
    LOG_WARN() << QString("Error while loading %1: %2").arg(pluginFile).arg(errorMessage);
}

void PluginManager::onLoadTaskMessageChanged(QString message)
{
    setLoadMessage(message);

}

void PluginManager::onLoadTaskFinished(bool success, QString message)
{
    setLoadProgress(1.0);

    if (success) {
        emit loadFinished();
    } else {
        LOG_WARN() << QString("Failed to load plugins: %1").arg(message);
    }

}

void JApp::PluginManager::setLoadProgress(qreal progress)
{
    if (qFuzzyCompare(progress, m_loadProgress))
        return;

    m_loadProgress = progress;
    emit loadProgressChanged(m_loadProgress);
}

void PluginManager::setLoadMessage(const QString &loadMessage)
{
    if (m_loadMessage == loadMessage)
        return;

    m_loadMessage = loadMessage;
    emit loadMessageChanged(m_loadMessage);
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

    if (m_loadTask) {
        m_loadTask = nullptr;
    }

    if (m_loadTaskThread) {
        m_loadTaskThread->deleteLater();
        m_loadTaskThread = nullptr;
    }
}

void PluginManager::processPluginInitializationQueue()
{
    if (m_pluginsToInitialize.isEmpty()) {
        if (m_initializedPlugins.count() == m_loaders.count()) {
            setInitializationMessage("All plugins initialized.");
            emit initializationFinished();
        }
        return;
    }
    JApp::Plugin* p = m_pluginsToInitialize.dequeue();
    setInitializationMessage(QString("Initializing %1...").arg(p->name()));
    QCoreApplication::processEvents();
    p->initialize();
    m_initializedPlugins.append(p);
    setInitializationProgress(qreal(m_initializedPlugins.count()) / qreal(m_files.count()));
    processPluginInitializationQueue();
}
