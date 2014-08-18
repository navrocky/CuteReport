!include( ../../../common.pri ) {
    error( Can not find the common.pri file! )
}

QT       += core

#QT       -= gui

TARGET = cutereport_cli

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    console.cpp

HEADERS += \
    console.h

INCLUDEPATH += ../../core \
            ../../core/log \
            ../../widgets/widgets/

DEPENDPATH += $$INCLUDEPATH

LIBS += -L../../../$$BUILD_DIR -lCuteReport -lCuteReportWidgets

contains(DEFINES, SYSTEMINSTALL) {
    DESTDIR = ../../../$$BUILD_DIR
    target.path += $$REPORT_DESIGNER_PATH
    INSTALLS += target
} else {
    DESTDIR = ../../../$$BUILD_DIR/$$REPORT_DESIGNER_PATH
}
