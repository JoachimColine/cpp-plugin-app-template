#pragma once
#include <QSharedPointer>
#include <QMap>
#include <QString>
#include <JApp/Log.h>
#include "ManagedObject.h"

class ObjectManager {
public:
    template <typename T>
    void registerObject(const QString& key, QSharedPointer<T> object) {
        if (m_objects.contains(key)) {
            LOG_WARN() << QString("An object with key '%1' is already registered.").arg(key);
        } else {
            m_objects.insert(key, object);
        }
    }

    template <typename T>
    QSharedPointer<T> getObject(const QString& key) {
        if (!m_objects.contains(key)) {
            LOG_WARN() << QString("No object found for key %1").arg(key);
        }

        return qSharedPointerCast<T>(m_objects.value(key, nullptr));
    }

    void unregisterObject(const QString& key) {
        if (!m_objects.contains(key)) {
           LOG_WARN() << QString("No object found for key %1").arg(key);
        }

        m_objects.remove(key);
    }

private:
    QMap<QString, QSharedPointer<ManagedObject>> m_objects;
};
