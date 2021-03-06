# -------------------------------------------------
# Project created by QtCreator 2009-06-23T14:12:15
# -------------------------------------------------
QT += openglwidgets widgets core gui opengl
TARGET = SpeedySketching
TEMPLATE = app
DESTDIR = ./build
CONFIG += Console
SOURCES += main.cpp \
    mainwindow.cpp \
    display.cpp \
    line.cpp \
    point.cpp \
    algorithms.cpp \
    scene.cpp \
    shape.cpp \
    circle.cpp \
    strokes.cpp \
    triangle.cpp \
    freehand.cpp \
    sphere.cpp \
    rect.cpp \
    cylinder.cpp \
    cone.cpp
HEADERS += mainwindow.h \
    display.h \
    line.h \
    point.h \
    algorithms.h \
    scene.h \
    shape.h \
    circle.h \
    strokes.h \
    triangle.h \
    freehand.h \
    sphere.h \
    rect.h \
    cylinder.h \
    cone.h
FORMS += mainwindow.ui
RESOURCES += icons.qrc
