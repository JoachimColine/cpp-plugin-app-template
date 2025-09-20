#pragma once

#include <JApp/LogDefinitions.h> 

namespace JApp::LogFormatter {
namespace Constants {
const char* const Debug   = "DEBUG";
const char* const Info    = "INFO ";
const char* const Warn    = "WARN ";
const char* const Error   = "ERROR";
const char* const Fatal   = "FATAL";
const char* const Unknown = "?    ";
}

    static QString logLevelToString(const LogLevel& level) {
        switch (level) {
        case LogLevel::Debug:    return Constants::Debug;
        case LogLevel::Info:     return Constants::Info;
        case LogLevel::Warning:  return Constants::Warn;
        case LogLevel::Critical: return Constants::Error;
        case LogLevel::Fatal:    return Constants::Fatal;
        default:                 return Constants::Unknown;
        }
    }

    static LogLevel logLevelFromString(const QString& string) {
        if (string.trimmed() == QString(Constants::Debug).trimmed())
            return LogLevel::Debug;
        if (string.trimmed() == QString(Constants::Info).trimmed())
            return LogLevel::Info;
        if (string.trimmed() == QString(Constants::Warn).trimmed())
            return LogLevel::Warning;
        if (string.trimmed() == QString(Constants::Error).trimmed())
            return LogLevel::Critical;
        if (string.trimmed() == QString(Constants::Fatal).trimmed())
            return LogLevel::Fatal;
        return LogLevel::Unknown;
    }


    static Log logFromString(const QString& string) {
        return Log();
	}

    static QString logToString(const Log& log) {
        QStringList parts;
        parts << log.timestamp.toString("yyyy-MM-dd hh:mm:ss.zzz");
        parts << QString("%1").arg(logLevelToString(log.level));
        parts << QString("%1").arg(log.category);
        QString funcInfo = log.function;
        funcInfo += QString(":%1").arg(log.line);
        parts << QString("%1").arg(funcInfo);
        parts << log.message;
        //parts << QString("%1").arg(reinterpret_cast<quintptr>(QThread::currentThread()), 0, 16);
        return parts.join(" | ");
	}
}
