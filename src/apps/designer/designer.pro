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
    ../../thirdparty/propertyeditor/lib/ \
    ../../thirdparty/propertyeditor/ \
    ../../plugins/standard/designer_plugins \
    ../../plugins/standard/core_plugins \

DEPENDPATH += $$INCLUDEPATH


contains(DEFINES, SYSTEMINSTALL) {
    DESTDIR = ../../../$$BUILD_DIR
    target.path += $$REPORT_DESIGNER_PATH
    INSTALLS += target
} else {
    DESTDIR = ../../../$$BUILD_DIR/$$REPORT_DESIGNER_PATH
}


contains(DEFINES, STATICBUILD) {
    CONFIG += static
    lessThan(QT_MAJOR_VERSION, 5) {
        CONFIG += designer
    } else {
        QT += designer
    }
}

LIBS += -L../../../$$BUILD_DIR -lPropertyEditor

contains(DEFINES, STATICPLUGINS_PROPERTYEDITOR) {
    include(../../thirdparty/propertyeditor/propertyeditor_static.pri)
    HEADERS += propertyeditor_static.h
    LIBS += -L../../../$$BUILD_DIR/$$PROPERTYEDITOR_PLUGINS_PATH
}

contains(DEFINES, STATICPLUGINS_DESIGNER) {
    include(../../plugins/standard/designer_plugins/designer_static.pri)
    HEADERS += designer_static.h
    LIBS += -L../../../$$BUILD_DIR/$$REPORT_DESIGNER_PLUGINS_PATH

    contains(DEFINES, EXTENDEDSUITE) {
        include(../../plugins/extended/designer_plugins/designer_staticExt.pri)
        HEADERS += designer_staticExt.h
        INCLUDEPATH += ../../plugins/extended/designer_plugins
    }
}

contains(DEFINES, STATICPLUGINS_CORE) {
    include(../../plugins/standard/core_plugins/core_static.pri)
    HEADERS += core_static.h
    LIBS += -L../../../$$BUILD_DIR/$$REPORT_PLUGINS_PATH

    contains(DEFINES, EXTENDEDSUITE) {
        include(../../plugins/extended/core_plugins/core_staticExt.pri)
        HEADERS += core_staticExt.h
        INCLUDEPATH +=  ../../plugins/extended/core_plugins
    }
}

LIBS += -lCuteDesigner -lCuteReportWidgets -lCuteReport
