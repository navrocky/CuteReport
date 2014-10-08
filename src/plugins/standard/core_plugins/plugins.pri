! include( ../../../../common.pri ) {
    error( Can not find the common.pri file! )
}

TEMPLATE = lib
CONFIG += plugin

HEADERS += plugins_common.h

INCLUDEPATH +=  ../../ \
                ../../../ \
                ../../../../../core/ \
                ../../../../../widgets/widgets \

DEPENDPATH += $$INCLUDEPATH

LIBS += -L../../../../../../$$BUILD_DIR -lCuteReport -lCuteReportWidgets

contains(DEFINES, SYSTEMINSTALL) {
    DESTDIR = ../../../../../../$$BUILD_DIR/$$REPORT_PLUGINS_PATH
    target.path = $$REPORT_PLUGINS_PATH
    INSTALLS += target
} else {
    DESTDIR = ../../../../../../$$BUILD_DIR/$$REPORT_PLUGINS_PATH
}


contains(DEFINES, STATICPLUGINS_CORE) {
    CONFIG += static
}

