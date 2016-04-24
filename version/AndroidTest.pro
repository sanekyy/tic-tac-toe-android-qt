#-------------------------------------------------
#
# Project created by QtCreator 2015-10-11T22:35:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AndroidTest
TEMPLATE = app


SOURCES += main.cpp\
    Segment.cpp \
    MyWidget.cpp \
    mainwidget.cpp

RESOURCES += pictures.qrc \
    pictures.qrc

HEADERS  += widget.h \
    Segment.h \
    MyWidget.h \
    mainwidget.h \
    myapplication.h

CONFIG += mobility
MOBILITY = 


