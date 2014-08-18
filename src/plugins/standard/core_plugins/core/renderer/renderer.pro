! include( ../../plugins.pri ) {
    error( Can not find the common.pri file! )
}

TARGET = Renderer

HEADERS += \
    renderer.h \
    rendereriteminterface.h \
    rendererprocessor.h \
    rendererdata.h \
    aggregatefunctions.h

SOURCES += \
    renderer.cpp \
    rendereriteminterface.cpp \
    rendererprocessor.cpp \
    rendererdata.cpp \
    aggregatefunctions.cpp
