#-------------------------------------------------
#
# Project created by QtCreator 2014-09-17T08:18:41
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MG
TEMPLATE = app

INCLUDEPATH += "/usr/include/x86_64-linux-gnu/qt5/"

SOURCES += src/main.cpp \
    src/openglwindow.cpp \
    src/octree.cpp

HEADERS  += \
    src/openglwindow.hpp \
    src/octree.hpp
