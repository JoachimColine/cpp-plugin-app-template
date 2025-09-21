#include "JApp/Core/Models/LogModel.h"

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

QVariant LogModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= m_logs.size())
        return QVariant();

    auto it = m_logs.constBegin();
    std::advance(it, index.row());
    const auto &log = it.value();

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
