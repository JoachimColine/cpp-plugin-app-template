#include "JApp/Plugins/LoadPluginsTask.h"
#include <JApp/Log.h>

#include <QApplication>
#include <QPluginLoader>
#include <QDir>
#include <QFileInfo>

using namespace JApp;

LoadPluginsTask::LoadPluginsTask(QObject *parent)
    : QObject(parent)
{
}

LoadPluginsTask::~LoadPluginsTask()
{
}

void LoadPluginsTask::setPluginFiles(const QFileInfoList &files)
{
    m_files = files;
}

void LoadPluginsTask::start()
{
    if (m_files.isEmpty()) {
        emit taskFinished(false, "No plugin files specified");
        return;
    }

    bool success = true;

    for (const QFileInfo &fileInfo : std::as_const(m_files)) {
        emit taskUpdated(QString("Loading %1...").arg(fileInfo.fileName()));

        QString filePath = fileInfo.absoluteFilePath();
        QString fileName = fileInfo.fileName();
        LOG_DEBUG() << "Attempting to load plugin:" << fileName;

        QPluginLoader* loader = new QPluginLoader(filePath);

        if (!loader->load()) {
            emit pluginError(fileName, loader->errorString());
            loader->deleteLater();
            continue;
        }

        QObject* plugin = loader->instance();
        if (!plugin) {
            emit pluginError(fileName, "Failed to get plugin instance");
            loader->unload();
            loader->deleteLater();
            continue;
        }

        LOG_DEBUG() << "Successfully loaded plugin: " << fileName;

        loader->moveToThread(QApplication::instance()->thread());
        plugin->moveToThread(QApplication::instance()->thread());

        emit pluginLoaded(loader, plugin);
    }

    emit taskFinished(success, "");
}
