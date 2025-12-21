#pragma once
#include <QSharedPointer>
#include <QMap>
#include <QString>
#include <QMutex>
#include <QMutexLocker>
#include <JApp/Log.h>
#include "ManagedObject.h"

namespace JApp::Core {

class ObjectManager {
public:
    static ObjectManager& instance();

    template <typename T>
    void registerObject(const QString& key, QSharedPointer<T> object) {
        QMutexLocker locker(&m_mutex);
        if (m_objects.contains(key)) {
            LOG_WARN() << QString("An object with key '%1' is already registered.").arg(key);
        } else {
            m_objects.insert(key, object);
        }
    }

    template <typename T>
    QSharedPointer<T> getObject(const QString& key) {
        QMutexLocker locker(&m_mutex);
        if (!m_objects.contains(key)) {
            LOG_WARN() << QString("No object found for key %1").arg(key);
        }
        return qSharedPointerCast<T>(m_objects.value(key, nullptr));
    }

    void unregisterObject(const QString& key);

private:
    ObjectManager();
    QMap<QString, QSharedPointer<ManagedObject>> m_objects;
    mutable QMutex m_mutex;
    static ObjectManager* s_instance;
};

}
