#pragma once

#include <QObject>
#include <QFileInfoList>
#include <QList>

class QPluginLoader;

namespace JApp {

class Plugin;

class LoadTask : public QObject
{
    Q_OBJECT

public:
    explicit LoadTask(QObject *parent = nullptr);
    ~LoadTask();

    void setPluginFiles(const QFileInfoList &pluginFiles);

public slots:
    void start();

signals:
    void pluginLoaded(QPluginLoader* loader, QObject* plugin);
    void pluginError(QString pluginFile, QString errorMessage);
    void taskMessageChanged(QString message);
    void taskFinished(bool success, QString message);

private:
    QFileInfoList m_files;
};

}
