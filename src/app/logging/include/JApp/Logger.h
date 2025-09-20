#pragma once

#include <JApp/LogDefinitions.h>
#include <QObject>
#include <QMutex>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>
#include <QStandardPaths>
#include <QLoggingCategory>
#include <QTimer>
#include <memory>

namespace JApp {

class Logger : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString logFilePath READ logFilePath NOTIFY logFilePathChanged)

public:
    static Logger& instance();
    
    void initialize(const LogConfig& config = LogConfig());
    void shutdown();
    
    void setLogLevel(LogLevel level);
    void setOutputTarget(OutputTarget target);
    void setLogDirectory(const QString& directory);

    QString logFilePath() const;

signals:
    void logFilePathChanged(const QString& logFilePath);

private slots:
    void flushLogs();

private:
    explicit Logger(QObject* parent = nullptr);
    ~Logger();
    
    void setupMessageHandler();
    void rotateLogFile();
    void createLogFile();
    void handleLog(const Log& log);
    QString createLogFilePath();
    void ensureLogDirectory();

    static void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& message);
    static LogLevel qtMsgTypeToLogLevel(QtMsgType type);

private:
    bool m_initialized;
    LogConfig m_config;
    std::unique_ptr<QFile> m_logFile;
    std::unique_ptr<QTextStream> m_logStream;
    QMutex m_mutex;
    QTimer* m_flushTimer;
    static Logger* s_instance;
};

inline bool hasFlag(OutputTarget target, OutputTarget flag) {
    return (static_cast<int>(target) & static_cast<int>(flag)) != 0;
}

}
