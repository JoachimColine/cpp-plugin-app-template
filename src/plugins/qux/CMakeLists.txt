qt_add_plugin(qux
    SHARED
    CLASS_NAME QuxPlugin
    QuxPlugin.cpp
)

target_link_libraries(qux PRIVATE
    JApp
)

set_target_properties(qux PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins"
    PREFIX ""
)
