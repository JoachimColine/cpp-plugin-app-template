#pragma once

#include "JApp/Core/Models/LogModel.h"
#include <QFileSystemWatcher>
#include <QFile>
#include <QTextStream>

namespace JApp::Core::Models {

class FileWatcherLogModel : public LogModel
{
    Q_OBJECT
    Q_PROPERTY(QString logFilePath READ logFilePath WRITE setLogFilePath NOTIFY logFilePathChanged)

public:
    explicit FileWatcherLogModel(QObject *parent = nullptr);
    ~FileWatcherLogModel();

    QString logFilePath() const;
    void setLogFilePath(const QString &path);

signals:
    void logFilePathChanged(const QString &path);
    void fileError(const QString &errorMessage);

private slots:
    void onLogFileChanged(const QString &path);

private:
    void reset();
    void initializeLogFile();
    void readNewLines();

    QFileSystemWatcher m_fileWatcher;
    QString m_logFilePath;
    QFile m_logFile;
    qint64 m_lastPosition = 0;
    bool m_initialized = false;
};

}
