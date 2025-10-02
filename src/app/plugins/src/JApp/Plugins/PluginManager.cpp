#include "JApp/Plugins/PluginManager.h"
#include "JApp/Plugins/Plugin.h"
#include <JApp/Log.h>

#include <QPluginLoader>
#include <QDir>
#include <QFileInfo>

using namespace JApp;

PluginManager::PluginManager(QString directory, QObject *parent) : QObject(parent)
    , m_directory(directory)
    , m_loadingProgress(0.0)
    , m_unloadingProgress(0.0)
{

}

PluginManager::~PluginManager()
{
    unloadPlugins();
}

QString JApp::PluginManager::directory() const
{
    return m_directory;
}

qreal JApp::PluginManager::loadingProgress() const
{
    return m_loadingProgress;
}

qreal JApp::PluginManager::unloadingProgress() const
{
    return m_unloadingProgress;
}

bool JApp::PluginManager::loadPlugins()
{
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

    int totalFiles = files.size();
    int loadedCount = 0;

    for (const QFileInfo &fileInfo : files) {
        setLoadingProgress(static_cast<qreal>(loadedCount) / totalFiles);

        QString filePath = fileInfo.absoluteFilePath();
        QString fileName = fileInfo.fileName();
        LOG_DEBUG() << "Attempting to load plugin:" << fileName;

        QPluginLoader *loader = new QPluginLoader(filePath);

        if (!loader->load()) {
            LOG_WARN() << "Failed to load plugin " << fileName << ":" << loader->errorString();
            delete loader;
            continue;
        }

        QObject *plugin = loader->instance();
        if (!plugin) {
            LOG_WARN() << "Failed to get plugin instance:" << fileName;
            loader->unload();
            delete loader;
            continue;
        }

        JApp::Plugin *jappPlugin = qobject_cast<JApp::Plugin*>(plugin);
        if (!jappPlugin) {
            LOG_WARN() << "Plugin doesn't implement JApp::Plugin interface:" << fileName;
            loader->unload();
            delete loader;
            continue;
        }

        LOG_DEBUG() << "Initializing plugin:" << jappPlugin->name();
        if (!jappPlugin->initialize()) {
            LOG_WARN() << "Plugin initialization failed:" << jappPlugin->name();
            loader->unload();
            delete loader;
            continue;
        }

        m_loaders.append(loader);
        m_plugins.append(jappPlugin);
        loadedCount++;

        LOG_DEBUG() << "Successfully loaded plugin:" << jappPlugin->name();
    }

    return true;
}

bool JApp::PluginManager::unloadPlugins()
{
    return true; // TODO
}

QList<JApp::Plugin*> JApp::PluginManager::loadedPlugins() const
{
    return QList<JApp::Plugin*>();
}

void JApp::PluginManager::setLoadingProgress(qreal progress)
{
    if (qFuzzyCompare(progress, m_loadingProgress))
        return;

    m_loadingProgress = progress;
    emit loadingProgressChanged(m_loadingProgress);
}

void JApp::PluginManager::setUnloadingProgress(qreal progress)
{
    if (qFuzzyCompare(progress, m_unloadingProgress))
        return;

    m_unloadingProgress = progress;
    emit unloadingProgressChanged(m_unloadingProgress);
}
