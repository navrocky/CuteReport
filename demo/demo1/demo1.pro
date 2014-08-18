#-------------------------------------------------
#
# Project created by QtCreator 2013-07-18T19:30:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = demo1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += ../../src/core/ \
        ../../src/widgets/widgets/

DEPENDPATH += $$INCLUDEPATH

LIBS += -L/home/alex/work/projects_hdd/build-CuteReport-Desktop_4_8_4-Debug/build -lCuteReport
LIBS += -L/home/alex/work/projects_hdd/build-CuteReport-Desktop_4_8_4-Debug/build -lCuteReportWidgets
