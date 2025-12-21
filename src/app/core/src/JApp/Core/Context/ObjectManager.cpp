#include <JApp/Core/Context/ObjectManager.h>

using namespace JApp::Core;

ObjectManager* ObjectManager::s_instance = nullptr;

ObjectManager::ObjectManager()
{

}

ObjectManager& ObjectManager::instance()
{
    if (!s_instance) {
        s_instance = new ObjectManager();
    }

    return *s_instance;
}

void ObjectManager::unregisterObject(const QString& key)
{
    QMutexLocker locker(&m_mutex);
    if (!m_objects.contains(key)) {
        LOG_WARN() << QString("No object found for key %1").arg(key);
    }
    m_objects.remove(key);
}
