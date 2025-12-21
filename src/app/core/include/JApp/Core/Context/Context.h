#pragma once
#include <JApp/Core/Context/ObjectManager.h>

namespace JApp::Core {

class Context {
public:
    static Context& instance();

    ObjectManager& objectManager();

private:
    Context();
    static Context* s_instance;
};

}
