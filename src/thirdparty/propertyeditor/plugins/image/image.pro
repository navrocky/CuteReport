! include( ../property_plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

FORMS += imageDialog.ui
SOURCES += imagedialog.cpp \
        image.cpp
HEADERS += imagedialog.h image.h
TARGET = ImageProperty
