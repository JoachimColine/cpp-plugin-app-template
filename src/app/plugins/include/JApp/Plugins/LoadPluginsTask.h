#pragma once

#include <QObject>
#include <QFileInfoList>
#include <QList>

class QPluginLoader;

namespace JApp {

class Plugin;

class LoadPluginsTask : public QObject
{
    Q_OBJECT

public:
    explicit LoadPluginsTask(QObject *parent = nullptr);
    ~LoadPluginsTask();

    void setPluginFiles(const QFileInfoList &pluginFiles);

public slots:
    void start();

signals:
    void pluginLoaded(QPluginLoader* loader, QObject* plugin);
    void pluginError(QString pluginFile, QString errorMessage);
    void taskUpdated(QString message);
    void taskFinished(bool success, QString message);

private:
    QFileInfoList m_files;
};

}
