! include( ../property_plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

HEADERS += rect.h
SOURCES += rect.cpp
TARGET = RectProperty
