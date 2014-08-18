! include( ../../../../common.pri ) {
    error( Can not find the common.pri file! )
}

TEMPLATE = lib

INCLUDEPATH += ../../../../thirdparty/propertyeditor/lib \
        ../../../../core

DEPENDPATH=$$INCLUDEPATH

CONFIG += plugin

LIBS += -L../../../../../$$BUILD_DIR -lCuteReport -lPropertyEditor

contains(DEFINES, SYSTEMINSTALL) {
    DESTDIR = ../../../../../$$BUILD_DIR
    target.path += $$PROPERTYEDITOR_PLUGINS_PATH
    INSTALLS += target
} else {
    DESTDIR = ../../../../../$$BUILD_DIR/$$PROPERTYEDITOR_PLUGINS_PATH
}
