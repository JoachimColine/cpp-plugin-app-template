#include "JApp/Core/Models/LogModel.h"
#include <JApp/LogFormatter.h>

namespace JApp::Core::Models {

LogModel::LogModel(QObject *parent)
    : QAbstractListModel(parent)
{}

void LogModel::addLog(const Log& log) {
    beginInsertRows(QModelIndex(), m_logs.size(), m_logs.size());
    m_logs.insert(log.timestamp, log);
    endInsertRows();
}

void LogModel::clear() {
    beginResetModel();
    m_logs.clear();
    endResetModel();
}

int LogModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_logs.size();
}

int LogModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 7;
}

QVariant LogModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= m_logs.size())
        return QVariant();

    auto it = m_logs.constBegin();
    std::advance(it, index.row());
    const auto &log = it.value();

    if (role == Qt::DisplayRole) {
        switch(index.column()) {
        case 0: return log.timestamp.toString("yyyy-MM-dd hh:mm:ss.zzz");
        case 1: return LogFormatter::logLevelToString(log.level);
        case 2: return log.file;
        case 3: return log.function;
        case 4: return log.line;
        case 5: return log.threadId;
        case 6: return log.message;
        case 7: return log.category;
        default: return QVariant();
        }
    }

    switch(role) {
        case TimestampRole: return log.timestamp;
        case CategoryRole:  return log.category;
        case LevelRole:     return static_cast<int>(log.level);
        case FileRole:      return log.file;
        case FunctionRole:  return log.function;
        case LineRole:      return log.line;
        case MessageRole:   return log.message;
        case ThreadRole:    return log.threadId;
        default:            return QVariant();
    }
}

QVariant LogModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();

    switch (section) {
    case 0: return "Date and time";
    case 1: return "Level";
    case 2: return "File";
    case 3: return "Function";
    case 4: return "Line";
    case 5: return "Thread";
    case 6: return "Message";
    default: return QVariant();
    }
}

QHash<int, QByteArray> LogModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[TimestampRole] = "timestamp";
    roles[CategoryRole]  = "category";
    roles[LevelRole]     = "level";
    roles[FileRole]      = "file";
    roles[FunctionRole]  = "function";
    roles[LineRole]      = "line";
    roles[MessageRole]   = "message";
    return roles;
}

}
