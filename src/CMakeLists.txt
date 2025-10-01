# full log information in release build
add_compile_definitions(QT_MESSAGELOGCONTEXT)

add_subdirectory(app)

qt_add_executable(${PROJECT_NAME}
    ${RESOURCES_FILE}
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    JApp
    Qt6::Core
    Qt6::Widgets
)
