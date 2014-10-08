! include( ../../../../common.pri ) {
    error( Can not find the common.pri file! )
}

TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH += ../../../../core \
               ../../../../widgets/widgets \
               ../../../../thirdparty/propertyeditor/lib \
               ../../../../apps/designer \
               ../../../../apps/designerCore \
               ../../../../apps/designerCore/widgets/ \
               ../../../../apps/designerCore/widgets/objectinspector/ \

DEPENDPATH += $$INCLUDEPATH

LIBS += -L../../../../../$$BUILD_DIR
LIBS += -lCuteReport
LIBS += -lPropertyEditor
LIBS += -lCuteDesigner
LIBS += -lCuteReportWidgets

contains(DEFINES, SYSTEMINSTALL) {
    DESTDIR = ../../../../../$$BUILD_DIR/$$REPORT_DESIGNER_PLUGINS_PATH
    target.path = $$REPORT_DESIGNER_PLUGINS_PATH
    INSTALLS += target
} else {
    DESTDIR = ../../../../../$$BUILD_DIR/$$REPORT_DESIGNER_PLUGINS_PATH
}

contains(DEFINES, STATICPLUGINS_DESIGNER) {
    CONFIG += static
}
