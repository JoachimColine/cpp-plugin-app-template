set(APP_VERSION_MAJOR "1")
set(APP_VERSION_MINOR "2")
set(APP_VERSION_PATCH "3")
set(APP_VERSION_BUILD "4")
set(APP_VERSION_FULL  "${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}.${APP_VERSION_PATCH}.${APP_VERSION_BUILD}")

add_subdirectory(logging)
add_subdirectory(core)
add_subdirectory(startup)

add_library(JApp INTERFACE)

target_link_libraries(JApp INTERFACE
    JApp::Logging
    JApp::Core
    JApp::Startup
)
