#pragma once

#include <JApp/Core/Models/LogModel.h>
#include <JApp/Logger.h>
#include <QFile>

namespace JApp::Core::Models {

class LoggerWatcherLogModel : public LogModel
{
    Q_OBJECT

public:
    explicit LoggerWatcherLogModel(JApp::Logger& watchedLogger, QObject *parent = nullptr);
    ~LoggerWatcherLogModel();

private slots:
    void onLogsFlushed();
    void onLogFilePathChanged(const QString& logFilePath);

private:
    void reset();
    void initializeWatchedLogFile();
    void readNewLines();

    JApp::Logger& m_watchedLogger;
    QFile m_watchedLogFile;
    qint64 m_lastPosition = 0;
    bool m_initialized = false;
};

}
