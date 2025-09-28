#include "JApp/Core/Models/LoggerWatcherLogModel.h"
#include <JApp/LogFormatter.h>
#include <JApp/Log.h>
#include <QDateTime>
#include <QFileInfo>
#include <QDebug>

namespace JApp::Core::Models {

LoggerWatcherLogModel::LoggerWatcherLogModel(JApp::Logger& watchedLogger, QObject *parent)
    : LogModel(parent)
    , m_watchedLogger(watchedLogger)
{
    connect(&m_watchedLogger, &JApp::Logger::logsFlushed, this, &LoggerWatcherLogModel::onLogsFlushed);
    connect(&m_watchedLogger, &JApp::Logger::logFilePathChanged, this, &LoggerWatcherLogModel::onLogFilePathChanged);
    reset();
}

LoggerWatcherLogModel::~LoggerWatcherLogModel()
{
    if (m_watchedLogFile.isOpen()) {
        m_watchedLogFile.close();
    }
}

void LoggerWatcherLogModel::reset()
{
    clear();
    m_lastPosition = 0;

    if (m_watchedLogFile.isOpen()) {
        m_watchedLogFile.close();
    }

    initializeWatchedLogFile();
}

void LoggerWatcherLogModel::initializeWatchedLogFile()
{
    m_watchedLogFile.setFileName(m_watchedLogger.logFilePath());
    if (!m_watchedLogFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        LOG_WARN() << QString("Failed to open log file for read: %1").arg(m_watchedLogFile.errorString());
        return;
    }
    readNewLines();
    m_initialized = true;
}

void LoggerWatcherLogModel::onLogsFlushed()
{
    readNewLines();
}

void LoggerWatcherLogModel::onLogFilePathChanged(const QString &logFilePath)
{
    Q_UNUSED(logFilePath)

    reset();
}

void LoggerWatcherLogModel::readNewLines()
{
    if (!m_watchedLogFile.isOpen() && !m_watchedLogFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        LOG_WARN() << QString("Failed to reopen log file for read: %1").arg(m_watchedLogFile.errorString());
        return;
    }

    if (!m_watchedLogFile.seek(m_lastPosition)) {
        LOG_WARN() << "Failed to seek in log file";
        return;
    }

    QTextStream stream(&m_watchedLogFile);
    QString line;

    while (!stream.atEnd()) {
        line = stream.readLine();
        addLog(LogFormatter::logFromString(line));
    }

    m_lastPosition = m_watchedLogFile.pos();
}

}

