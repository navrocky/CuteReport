! include( ../../../common.pri ) {
    error( Can not find the common.pri file! )
}

TARGET = cutereport_gui
TEMPLATE = app

SOURCES += main.cpp \

HEADERS  += \

INCLUDEPATH += \
    ../../core \
    ../../thirdparty/propertyeditor/lib/  \
    ../designerCore/ \
    ../designerCore/widgets/ \
    widgets \
    widgets/objectinspector \
    ../../libs/propertyeditor/lib/ \

DEPENDPATH += $$INCLUDEPATH

LIBS += -L../../../$$BUILD_DIR -lCuteReport -lCuteDesigner -lPropertyEditor

contains(DEFINES, SYSTEMINSTALL) {
    DESTDIR = ../../../$$BUILD_DIR
    target.path += $$REPORT_DESIGNER_PATH
    INSTALLS += target
} else {
    DESTDIR = ../../../$$BUILD_DIR/$$REPORT_DESIGNER_PATH
}
