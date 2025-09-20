#include "JApp/Core/Models/FileWatcherLogModel.h"
#include <JApp/LogFormatter.h>
#include <JApp/Log.h>
#include <QDateTime>
#include <QFileInfo>
#include <QDebug>

namespace JApp::Core::Models {

FileWatcherLogModel::FileWatcherLogModel(QObject *parent)
    : LogModel(parent)
{
    connect(&m_fileWatcher, &QFileSystemWatcher::fileChanged, this, &FileWatcherLogModel::onLogFileChanged);
}

FileWatcherLogModel::~FileWatcherLogModel()
{
    if (m_logFile.isOpen()) {
        m_logFile.close();
    }
}

QString FileWatcherLogModel::logFilePath() const
{
    return m_logFilePath;
}

void FileWatcherLogModel::setLogFilePath(const QString &path)
{
    if (m_logFilePath == path)
        return;

    if (!m_fileWatcher.files().isEmpty()) {
        m_fileWatcher.removePaths(m_fileWatcher.files());
    }

    // Close existing file if open
    if (m_logFile.isOpen()) {
        m_logFile.close();
    }

    m_logFilePath = path;
    m_initialized = false;
    m_lastPosition = 0;

    initializeLogFile();

    emit logFilePathChanged(m_logFilePath);
}

void FileWatcherLogModel::reset()
{
    clear();
    m_lastPosition = 0;

    if (m_logFile.isOpen()) {
        m_logFile.close();
    }

    initializeLogFile();
}

void FileWatcherLogModel::initializeLogFile()
{
    if (m_logFilePath.isEmpty())
        return;

    m_logFile.setFileName(m_logFilePath);

    // Open the file for reading
    if (!m_logFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit fileError(tr("Failed to open log file for read: %1").arg(m_logFile.errorString()));
        return;
    }

    // Add the file to the watcher
    m_fileWatcher.addPath(m_logFilePath);

    // Read existing content
    readNewLines();

    m_initialized = true;
}

void FileWatcherLogModel::onLogFileChanged(const QString &path)
{
    if (path != m_logFilePath)
        return;

    // Read new lines
    readNewLines();
}

void FileWatcherLogModel::readNewLines()
{
    LOG_INFO() << "reading new lines";
    if (!m_logFile.isOpen() && !m_logFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit fileError(tr("Failed to reopen log file for read: %1").arg(m_logFile.errorString()));
        return;
    }

    // Seek to where we left off
    if (!m_logFile.seek(m_lastPosition)) {
        emit fileError(tr("Failed to seek in log file"));
        return;
    }

    QTextStream stream(&m_logFile);
    QString line;

    while (!stream.atEnd()) {
        line = stream.readLine();
        addLog(LogFormatter::logFromString(line));
    }

    m_lastPosition = m_logFile.pos();
}

} // namespace JApp::Core::Models

