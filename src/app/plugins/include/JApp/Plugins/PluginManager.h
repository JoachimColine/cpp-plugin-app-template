#pragma once

#include <QObject>
#include <QList>
#include <QDir>
#include <QThread>

class QPluginLoader;

namespace JApp {

class Plugin;
class LoadPluginsTask;

class PluginManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString directory READ directory CONSTANT)
    Q_PROPERTY(qreal loadingProgress READ loadingProgress NOTIFY loadingProgressChanged)
    Q_PROPERTY(QString loadingMessage READ loadingMessage NOTIFY loadingMessageChanged)

public:
    explicit PluginManager(QString directory, QObject *parent = nullptr);
    ~PluginManager();

    QString directory() const;
    qreal loadingProgress() const;
    QString loadingMessage() const;

    bool loadPlugins();

    QList<JApp::Plugin*> loadedPlugins() const;

signals:
    void loadingProgressChanged(qreal progress);
    void loadingMessageChanged(const QString& loadingMessage);
    void pluginsLoaded();

private slots:
    void onPluginLoaded(QPluginLoader* loader, QObject* plugin);
    void onPluginError(QString pluginFile, QString errorMessage);
    void onLoadingTaskUpdated(qreal loadingProgress, QString loadingMessage);
    void onLoadingTaskFinished(bool success, QString message);
    void cleanUpTaskThread();

private:
    void setLoadingProgress(qreal progress);
    void setLoadingMessage(const QString& loadingMessage);

private:
    QString m_directory;
    QList<QPluginLoader*> m_loaders;
    QList<JApp::Plugin*> m_plugins;
    qreal m_loadingProgress;
    QString m_loadingMessage;

    QThread* m_loadPluginsTaskThread;
    LoadPluginsTask* m_loadPluginsTask;
};

}
