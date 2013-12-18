#-------------------------------------------------
#
# Project created by QtCreator 2013-12-14T00:49:15
#
#-------------------------------------------------
CONFIG   += C++11
QT       += core gui
QT           += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ssgui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ship.cpp \
    shipwidget.cpp

HEADERS  += mainwindow.h \
    ship.h \
    shipwidget.h

FORMS    += mainwindow.ui
