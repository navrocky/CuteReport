!include( $$PWD/../../../../common.pri ) {
    error( Can not find the common.pri file! )
}

CONFIG += plugin
TEMPLATE = lib

INCLUDEPATH += ../../lib/
DEPENDPATH=$$INCLUDEPATH

LIBS += -L../../../../../$$BUILD_DIR -lPropertyEditor

contains(DEFINES, SYSTEMINSTALL) {
    DESTDIR = ../../../../../$$BUILD_DIR/$$PROPERTYEDITOR_PLUGINS_PATH
    target.path += $$PROPERTYEDITOR_PLUGINS_PATH
    INSTALLS += target
} else {
    DESTDIR = ../../../../../$$BUILD_DIR/$$PROPERTYEDITOR_PLUGINS_PATH
}

contains(DEFINES, STATICPLUGINS_PROPERTYEDITOR) {
    CONFIG += static
}
