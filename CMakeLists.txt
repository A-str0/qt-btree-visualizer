cmake_minimum_required(VERSION 3.16)

project(course_part2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup(REQUIRES 6.10)

qt_add_executable(appcourse_part2
    main.cpp
    mainwindow.hpp
    mainwindow.cpp
    mainwindow.ui
    json_parser.hpp
    operation_result.hpp
    binary_tree.hpp
    binary_tree_visualizer.hpp
    tree_widget.hpp
    json_parser.cpp
    binary_tree_visualizer.cpp
    tree_widget.cpp
    binary_tree.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appcourse_part2 PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appcourse_part2
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appcourse_part2
    PRIVATE Qt6::Widgets
)

install(TARGETS appcourse_part2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
