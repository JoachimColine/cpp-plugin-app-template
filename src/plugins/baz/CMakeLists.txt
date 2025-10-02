qt_add_plugin(baz
    SHARED
    CLASS_NAME BazPlugin
    BazPlugin.cpp
)

target_link_libraries(baz PRIVATE
    JApp
)

set_target_properties(baz PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins"
    PREFIX ""
)
