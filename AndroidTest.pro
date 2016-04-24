#-------------------------------------------------
#
# Project created by QtCreator 2015-10-11T22:35:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = target
TEMPLATE = app


SOURCES += main.cpp\
    Segment.cpp \
    MyWidget.cpp \
    mainwidget.cpp

RESOURCES += pictures.qrc \

HEADERS  += widget.h \
    Segment.h \
    MyWidget.h \
    mainwidget.h \
    myapplication.h

CONFIG += mobility
MOBILITY = 

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

