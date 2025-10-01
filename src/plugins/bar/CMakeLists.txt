project(bar_plugin)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_library(bar SHARED
    BarPlugin.cpp
)

target_include_directories(bar PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(bar PRIVATE JApp Qt6::Core)

set_target_properties(bar PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins"
)

set_target_properties(bar PROPERTIES
    OUTPUT_NAME "bar"
    PREFIX ""
)
