add_subdirectory(app)

qt_add_executable(${PROJECT_NAME}
    ${RESOURCES_FILE}
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    JApp::Logging
    JApp::Core
    JApp::Plugins
    Qt6::Core
    Qt6::Widgets
)

# full log information in release build
target_compile_definitions(${PROJECT_NAME} PRIVATE QT_MESSAGELOGCONTEXT)
