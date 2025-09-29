#include "JApp/Plugins/PluginManager.h"

using namespace JApp;

PluginManager::PluginManager(QString directory, QObject *parent) : QObject(parent)
{

}

PluginManager::~PluginManager()
{

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

bool JApp::PluginManager::loadAllPlugins()
{
    return true;
}

bool JApp::PluginManager::unloadAllPlugins()
{
    return true;
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
