#pragma once

#include <QAbstractListModel>
#include <QMultiMap>
#include <JApp/Logger.h>

namespace JApp::Core::Models {

class LogModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum LogRoles {
        TimestampRole = Qt::UserRole + 1,
        CategoryRole,
        LevelRole,
        FileRole,
        FunctionRole,
        LineRole,
        MessageRole
    };

    explicit LogModel(QObject *parent = nullptr);
    void addLog(const JApp::Logger::Log& log);
    void clear();
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QMultiMap<QDateTime, JApp::Logger::Log> m_logs;
};

}
