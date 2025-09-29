#pragma once

#include <QAbstractListModel>
#include <JApp/LogDefinitions.h>

namespace JApp::Core::Models {

class LogModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int maxLogCount READ maxLogCount WRITE setMaxLogCount NOTIFY maxLogCountChanged)

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
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    int maxLogCount() const;
    void setMaxLogCount(int count);

signals:
    void maxLogCountChanged(int count);

protected:
    void addLog(const Log& log);
    void clear();

private:
    QVector<Log> m_logs;
    int m_maxLogCount;
};

}
