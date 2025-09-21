#pragma once

#include <QAbstractListModel>
#include <QMultiMap>
#include <JApp/LogDefinitions.h>

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
        MessageRole,
        ThreadRole
    };

    explicit LogModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

protected:
    void addLog(const Log& log);
    void clear();

    QMultiMap<QDateTime, Log> m_logs;
};

}
