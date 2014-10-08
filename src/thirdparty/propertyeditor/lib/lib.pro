! include( ../../../../common.pri ) {
    error( Can not find the common.pri file! )
}

TEMPLATE = lib

CONFIG += exceptions rtti shared

macx{
    CONFIG += lib_bundle
}

win32 {
    DEFINES += PROPERTYEDITOR_LIB_EXPORTS
    TARGET_EXT = .dll
}

HEADERS += propertyeditor.h \
             propertymodel.h \
             propertyinterface.h \
             propertydelegate.h \
             propertyvalidator.h \
             globals.h \
             propertyeditorcore.h

SOURCES += propertyeditor.cpp \
             propertymodel.cpp \
             propertyinterface.cpp \
             propertydelegate.cpp \
             propertyvalidator.cpp \
             propertyeditorcore.cpp

FORMS += propertyeditor.ui

TARGET = PropertyEditor

INCLUDEPATH += .. .
DEPENDPATH=$$INCLUDEPATH

contains(DEFINES, SYSTEMINSTALL) {
    DESTDIR = ../../../../$$BUILD_DIR/
    target.path = $$PROPERTYEDITOR_LIBS_PATH
    include.path = $$PROPERTYEDITOR_HEADERS_PATH
    include.files = propertyeditor.h propertyinterface.h propertyvalidator.h
    INSTALLS += include target
} else {
    DESTDIR = ../../../../$$BUILD_DIR/$$PROPERTYEDITOR_LIBS_PATH
}

contains(DEFINES, STATIC_PROPERTYEDITOR) {
    CONFIG += static
}

