qt_add_plugin(bar
    SHARED
    CLASS_NAME BarPlugin
    BarPlugin.cpp
)

target_link_libraries(bar PRIVATE
    JApp
    Qt6::Core
    Qt6::Widgets
)

set_target_properties(bar PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins"
    PREFIX ""
)
