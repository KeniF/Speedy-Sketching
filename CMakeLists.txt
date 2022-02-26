cmake_minimum_required(VERSION 3.5)

project(Speedy-Sketching VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt6 COMPONENTS OpenGLWidgets REQUIRED)
find_package(Qt6 COMPONENTS OpenGL REQUIRED)

set(PROJECT_SOURCES
        algorithms.cpp
        algorithms.h
        circle.cpp
        circle.h
        cone.cpp
        cone.h
        cylinder.cpp
        cylinder.h
        display.cpp
        display.h
        freehand.cpp
        freehand.h
        line.cpp
        line.h
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        object.cpp
        object.h
        point.cpp
        point.h
        rect.cpp
        rect.h
        scene.cpp
        scene.h
        shape.cpp
        shape.h
        sphere.cpp
        sphere.h
        strokes.cpp
        strokes.h
        triangle.cpp
        triangle.h
        ui_mainwindow.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Speedy-Sketching
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

qt_add_resources(Speedy-Sketching "app_images"
    PREFIX "/"
    FILES
        icons/circle.png
        icons/cone.png
        icons/copy.png
        icons/cut.png
        icons/cylinder_d.png
        icons/cylinder_t.png
        icons/downarrow.png
        icons/extrusion.png
        icons/filenew.png
        icons/fileopen.png
        icons/filesave.png
        icons/freehand.png
        icons/help.png
        icons/paste.png
        icons/rectangle.png
        icons/redo.png
        icons/remove.png
        icons/sphere.png
        icons/triangle.png
        icons/undo.png
        icons/uparrow.png
        icons/zoomin.png
        icons/zoomout.png
)
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Speedy-Sketching APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Speedy-Sketching SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Speedy-Sketching
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Speedy-Sketching PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Speedy-Sketching PRIVATE Qt6::OpenGLWidgets)
target_link_libraries(Speedy-Sketching PRIVATE Qt6::OpenGL)

set_target_properties(Speedy-Sketching PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Speedy-Sketching)
endif()
