add_subdirectory(logging)
add_subdirectory(core)
add_subdirectory(plugins)
add_subdirectory(startup)

add_library(JApp INTERFACE)

target_link_libraries(JApp INTERFACE
    JApp::Logging
    JApp::Core
    JApp::Plugins
    JApp::Startup
)
