file(GLOB_RECURSE SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

add_library(core STATIC ${SRC_FILES})

target_link_libraries(logging PUBLIC
    Qt6::Core
    Qt6::Widgets
)

target_include_directories(logging
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(logging PROPERTIES
    AUTOMOC ON
)

add_library(JApp::Core ALIAS core)
