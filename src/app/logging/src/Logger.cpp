#include "JApp/Logger.h"
#include <QApplication>
#include <QThread>
#include <QFileInfo>
#include <QDebug>
#include <QRegularExpression>
#include <JApp/Log.h>
#include <JApp/LogFormatter.h>
#include <iostream>

using namespace JApp;

Logger* Logger::s_instance = nullptr;

Logger::Logger(QObject* parent)
    : QObject(parent)
    , m_flushTimer(new QTimer(this))
    , m_initialized(false)
    , m_flushNeeded(false)
{
}

Logger::~Logger()
{
    shutdown();
}

void Logger::createLogFile()
{
    m_logFile = std::make_unique<QFile>(createLogFilePath());
    if (m_logFile->open(QIODevice::WriteOnly | QIODevice::Append)) {
        m_logStream = std::make_unique<QTextStream>(m_logFile.get());
        m_logStream->setEncoding(QStringConverter::Utf8);
    } else {
        std::cout << "Failed to open log file: " << m_logFile->fileName().toStdString() << std::endl;
    }

    emit logFilePathChanged(m_logFile->fileName());
}

Logger& Logger::instance()
{
    if (!s_instance) {
        s_instance = new Logger();
    }
    return *s_instance;
}

void Logger::initialize(const LogConfig& config)
{
    if (m_initialized) return;
    {
        QMutexLocker locker(&m_mutex);

        m_config = config;

        if (m_config.logDirectory.isEmpty()) {
            m_config.logDirectory = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/logs";
        }

        ensureLogDirectory();

        if (hasFlag(m_config.target, OutputTarget::File)) {
            createLogFile();
        }

        setupMessageHandler();

        m_flushTimer->setInterval(m_config.flushIntervalMs);
        connect(m_flushTimer, &QTimer::timeout, this, &Logger::flushLogs);
        m_flushTimer->start();

    } // Release mutex.

    m_initialized = true;

    LOG_INFO() << QString("Logger initialized - Target: %1, Level: %2, Directory: %3")
                  .arg(static_cast<int>(m_config.target))
                  .arg(static_cast<int>(m_config.minLevel))
                  .arg(m_config.logDirectory);
}

void Logger::shutdown()
{
    if (!m_initialized) return;

    QMutexLocker locker(&m_mutex);
    
    if (m_flushTimer->isActive()) {
        m_flushTimer->stop();
    }
    
    if (m_logStream) {
        m_logStream->flush();
        m_logStream.reset();
    }
    
    if (m_logFile) {
        m_logFile->close();
        m_logFile.reset();
    }
    
    qInstallMessageHandler(nullptr);
}

void Logger::setLogLevel(LogLevel level)
{
    QMutexLocker locker(&m_mutex);
    m_config.minLevel = level;
}

void Logger::setOutputTarget(OutputTarget target)
{
    QMutexLocker locker(&m_mutex);
    m_config.target = target;
}

void Logger::setLogDirectory(const QString& directory)
{
    QMutexLocker locker(&m_mutex);
    m_config.logDirectory = directory;
    ensureLogDirectory();
}

QString Logger::logFilePath() const
{
    return m_logFile->fileName();
}
void Logger::handleLog(const Log& log)
{
    if (!m_initialized) return;

    if (log.level < m_config.minLevel) {
        return;
    }
    
    QMutexLocker locker(&m_mutex);
    
    QString formattedLog = LogFormatter::logToString(log);
    
    if (hasFlag(m_config.target, OutputTarget::Console)) {
        if (log.level >= LogLevel::Critical) {
            std::cerr << formattedLog.toStdString() << std::endl;
        } else {
            std::cout << formattedLog.toStdString() << std::endl;
        }
    }
    
    if (hasFlag(m_config.target, OutputTarget::File) && m_logStream) {
        if (m_logFile->size() > m_config.maxFileSize) {
            rotateLogFile();
        }
        
        *m_logStream << formattedLog << Qt::endl;
        m_flushNeeded = true;
    }
}

void Logger::flushLogs()
{
    if (!m_initialized || !m_flushNeeded) return;

    bool flushed = false;

    {
        QMutexLocker locker(&m_mutex);
        if (m_logStream) {
            m_logStream->flush();
            m_flushNeeded = false;
            flushed = true;
        }
    } // Release mutex.

    if (flushed) {
        emit logsFlushed();
    }
}

void Logger::setupMessageHandler()
{
    qInstallMessageHandler(&Logger::messageHandler);
}

void Logger::messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& message)
{
    if (!s_instance) {
        return;
    }

    Log log {
        QDateTime::currentDateTime(),
        context.category ? QString(context.category) : QString("?"),
        qtMsgTypeToLogLevel(type),
        context.file ? LogFormatter::simplifyLogFileName(QString(context.file)) : QString("?"),
        context.function ? LogFormatter::simplifyLogFunctionName(QString(context.function)) : QString("?"),
        context.line,
        message,
        QString("%1").arg(reinterpret_cast<quintptr>(QThread::currentThread()), 0, 16)
    };
    
    s_instance->handleLog(log);
}

LogLevel Logger::qtMsgTypeToLogLevel(QtMsgType type)
{
    switch (type) {
        case QtDebugMsg:    return LogLevel::Debug;
        case QtInfoMsg:     return LogLevel::Info;
        case QtWarningMsg:  return LogLevel::Warning;
        case QtCriticalMsg: return LogLevel::Critical;
        case QtFatalMsg:    return LogLevel::Fatal;
        default:            return LogLevel::Unknown;
    }
}

void Logger::rotateLogFile()
{
    if (!m_logFile || !m_logStream) return;
    
    m_logStream->flush();
    m_logFile->close();

    createLogFile();
}

QString Logger::createLogFilePath()
{
    return QString("%1/%2_%3.log")
        .arg(m_config.logDirectory)
        .arg(m_config.logFilePrefix)
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_hh'h'mm'm'ss's'"));
}

void Logger::ensureLogDirectory()
{
    QDir dir;
    if (!dir.exists(m_config.logDirectory)) {
        dir.mkpath(m_config.logDirectory);
    }
}
