# full log information in release build
add_compile_definitions(QT_MESSAGELOGCONTEXT)

add_subdirectory(app)
add_subdirectory(plugins)

qt_add_executable(${PROJECT_NAME}
    ${RESOURCES_FILE}
    main.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    JApp
    Qt6::Core
    Qt6::Widgets
)
