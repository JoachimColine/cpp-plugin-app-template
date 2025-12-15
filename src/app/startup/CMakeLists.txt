file(GLOB_RECURSE SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

add_library(startup STATIC ${SRC_FILES})

target_link_libraries(startup PUBLIC
    Qt6::Core
    Qt6::Widgets
    JApp::Core
    JApp::Logging
    JApp::Plugins
)

target_include_directories(startup
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(startup PROPERTIES
    AUTOMOC ON
)

add_library(JApp::Startup ALIAS startup)
