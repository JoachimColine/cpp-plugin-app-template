file(GLOB_RECURSE SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

add_library(plugins STATIC ${SRC_FILES})

target_link_libraries(plugins PUBLIC
    Qt6::Core
    Qt6::Widgets
    JApp::Logging
)

target_include_directories(plugins
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(plugins PROPERTIES
    AUTOMOC ON
)

add_library(JApp::Plugins ALIAS plugins)
