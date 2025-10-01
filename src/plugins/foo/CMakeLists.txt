project(foo_plugin)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_library(foo SHARED
    FooPlugin.cpp
)

target_include_directories(foo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(foo PRIVATE JApp Qt6::Core)

set_target_properties(foo PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins"
)

set_target_properties(foo PROPERTIES
    OUTPUT_NAME "foo"
    PREFIX ""
)
