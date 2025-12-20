#pragma once

namespace JApp::Core {

class Context {
public:
    static Context& instance();

    // to be filled with centralized resources
    // as the code grows

private:
    Context();
    static Context* s_instance;
};

}
