qt_add_plugin(foo
    SHARED
    CLASS_NAME FooPlugin
    FooPlugin.cpp
)

target_link_libraries(foo PRIVATE
    JApp
    Qt6::Core
    Qt6::Widgets
)

set_target_properties(foo PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins"
    PREFIX ""
)
