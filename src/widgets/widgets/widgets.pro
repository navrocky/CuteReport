! include( ../../../common.pri ) {
    error( Can not find the common.pri file! )
}

TARGET = CuteReportWidgets
TEMPLATE =  lib

lessThan(QT_MAJOR_VERSION, 5) {
    CONFIG += designer
} else {
    QT += designer
}

INCLUDEPATH = ../../core
DEPENDPATH = $$INCLUDEPATH

win32 {
    DEFINES += CUTEREPORT_WIDGET_EXPORTS
    TARGET_EXT = .dll
}

LIBS += -L../../../$$BUILD_DIR -lCuteReport

include (combobox.pri)
include (objectpreview.pri)
include (reportpreview.pri)
include (stdstoragedialog.pri)
include (emptydialog.pri)
include (exportdialog.pri)

contains(DEFINES, SYSTEMINSTALL) {
    DESTDIR = ../../../$$BUILD_DIR
    target.path += $$REPORT_LIBS_PATH
    INSTALLS += target
} else {
    DESTDIR = ../../../$$BUILD_DIR/$$REPORT_LIBS_PATH
}

HEADERS += \
    functions_gui.h

SOURCES += \
    functions_gui.cpp

contains(DEFINES, STATIC_WIDGETS) {
    CONFIG += static
    CONFIG += uitools
}
