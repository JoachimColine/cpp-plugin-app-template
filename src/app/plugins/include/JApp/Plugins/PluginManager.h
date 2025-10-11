#pragma once

#include <QObject>
#include <QList>
#include <QQueue>
#include <QDir>
#include <QThread>

class QPluginLoader;

namespace JApp {

class Plugin;
class LoadTask;

class PluginManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString directory READ directory CONSTANT)
    Q_PROPERTY(qreal loadingProgress READ loadingProgress NOTIFY loadingProgressChanged)
    Q_PROPERTY(QString loadingMessage READ loadingMessage NOTIFY loadingMessageChanged)
    Q_PROPERTY(qreal initializationProgress READ initializationProgress NOTIFY initializationProgressChanged)
    Q_PROPERTY(QString initializationMessage READ initializationMessage NOTIFY initializationMessageChanged)

public:
    explicit PluginManager(QString directory, QObject *parent = nullptr);
    ~PluginManager();

    QString directory() const;
    qreal loadingProgress() const;
    QString loadingMessage() const;

    qreal initializationProgress() const;
    QString initializationMessage() const;

    bool loadPlugins();

    QList<JApp::Plugin*> plugins() const;

signals:
    void loadingProgressChanged(qreal progress);
    void loadingMessageChanged(const QString& loadingMessage);
    void initializationProgressChanged(qreal progress);
    void initializationMessageChanged(const QString& loadingMessage);
    void pluginsLoaded();

private slots:
    void onPluginLoaded(QPluginLoader* loader, QObject* plugin);
    void onPluginError(QString pluginFile, QString errorMessage);
    void onLoadingTaskUpdated(QString loadingMessage);
    void onLoadingTaskFinished(bool success, QString message);
    void cleanUpTaskThread();
    void processPluginInitializationQueue();

private:
    void setLoadingProgress(qreal progress);
    void setLoadingMessage(const QString& loadingMessage);
    void setInitializationProgress(qreal progress);
    void setInitializationMessage(const QString& loadingMessage);

private:
    QString m_directory;
    QFileInfoList m_files;
    QList<QPluginLoader*> m_loaders;
    QList<JApp::Plugin*> m_initializedPlugins;
    qreal m_loadingProgress;
    QString m_loadingMessage;
    qreal m_initializationProgress;
    QString m_initializationMessage;

    QQueue<JApp::Plugin*> m_pluginsToInitialize;

    QThread* m_loadTaskThread;
    LoadTask* m_loadTask;
};

}
