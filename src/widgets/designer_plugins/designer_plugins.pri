! include( $$PWD/../../../common.pri ) {
    error( Can not find the common.pri file! )
}

TEMPLATE = lib

CONFIG -= debug_and_release
CONFIG += plugin

lessThan(QT_MAJOR_VERSION, 5) {
    CONFIG += designer
} else {
    QT += designer
}

INCLUDEPATH = $$PWD/../../widgets/widgets/ \
        $$PWD/../../core/

#DEPENDPATH = $$INCLUDEPATH

contains(DEFINES, SYSTEMINSTALL) {
    #DESTDIR = ../../../../$$BUILD_DIR/$$QTDESIGNER_PLUGINS_PATH
    #target.path += $$PROPERTYEDITOR_LIBS_PATH
    #INSTALLS += target
} else {
    DESTDIR = ../../../../$$BUILD_DIR/$$QTDESIGNER_PLUGINS_PATH
}
