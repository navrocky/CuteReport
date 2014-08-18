! include( ../property_plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

HEADERS += rectf.h
SOURCES += rectf.cpp
TARGET = RectFProperty
