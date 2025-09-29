#pragma once
#include <QDateTime>

namespace JApp {

enum class LogLevel {
    Debug    = 0,
    Info     = 1,
    Warning  = 2,
    Critical = 3,
    Fatal    = 4,
    Unknown  = 5
};

enum class OutputTarget {
    Console = 0x01,
    File    = 0x02,
    Both    = Console | File
};

struct LogConfig {
    LogConfig(){}
    LogLevel minLevel     = LogLevel::Debug;
    OutputTarget target   = OutputTarget::Both;
    QString logDirectory;
    QString logFilePrefix = "japp";
    qint64 maxFileSize    = 10 * 1024 * 1024; // 10MB
    int flushIntervalMs   = 1000;
};

struct Log {
    QDateTime timestamp;
    QString   category;
    LogLevel  level;
    QString   file;
    QString   function;
    int       line;
    QString   message;
    QString   threadId;
};

}
