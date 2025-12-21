#pragma once

#include <QObject>
#include <QList>
#include <QQueue>
#include <QDir>
#include <QThread>

class QPluginLoader;

namespace JApp {
namespace Core {
class Context;
}
class Plugin;
class LoadTask;

class PluginsInitializer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString directory READ directory CONSTANT)
    Q_PROPERTY(qreal loadProgress READ loadProgress NOTIFY loadProgressChanged)
    Q_PROPERTY(QString loadMessage READ loadMessage NOTIFY loadMessageChanged)
    Q_PROPERTY(qreal initializationProgress READ initializationProgress NOTIFY initializationProgressChanged)
    Q_PROPERTY(QString initializationMessage READ initializationMessage NOTIFY initializationMessageChanged)

public:
    static PluginsInitializer& instance();
    ~PluginsInitializer();

    void setDirectory(QString directory);

    QString directory() const;
    qreal loadProgress() const;
    QString loadMessage() const;

    qreal initializationProgress() const;
    QString initializationMessage() const;

    bool initialize();

    QList<JApp::Plugin*> plugins() const;

signals:
    void loadProgressChanged(qreal progress);
    void loadMessageChanged(const QString& message);
    void initializationProgressChanged(qreal progress);
    void initializationMessageChanged(const QString& message);
    void initialized();

private slots:
    void onPluginLoaded(QPluginLoader* loader, QObject* plugin);
    void onPluginError(QString pluginFile, QString errorMessage);
    void onLoadTaskMessageChanged(QString message);
    void onLoadTaskFinished(bool success, QString message);
    void cleanUpTaskThread();
    void processPluginInitializationQueue();

private:
    void setLoadProgress(qreal progress);
    void setLoadMessage(const QString& loadMessage);
    void setInitializationProgress(qreal progress);
    void setInitializationMessage(const QString& initializationMessage);

private:
    PluginsInitializer();
    QString m_directory;
    QFileInfoList m_files;
    QList<QPluginLoader*> m_loaders;
    QList<JApp::Plugin*> m_initializedPlugins;
    qreal m_loadProgress;
    QString m_loadMessage;
    qreal m_initializationProgress;
    QString m_initializationMessage;
    QQueue<JApp::Plugin*> m_pluginsToInitialize;
    QThread* m_loadTaskThread;
    LoadTask* m_loadTask;
    Core::Context& m_context;

    static PluginsInitializer* s_instance;
};

}
