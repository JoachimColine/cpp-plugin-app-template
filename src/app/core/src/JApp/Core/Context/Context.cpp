#include <JApp/Core/Context/Context.h>

using namespace JApp::Core;

Context* Context::s_instance = nullptr;

Context::Context()
{

}

Context& Context::instance()
{
    if (!s_instance) {
        s_instance = new Context();
    }
    return *s_instance;
}

ObjectManager& Context::objectManager()
{
    return ObjectManager::instance();
}
