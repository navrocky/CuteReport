! include( ../property_plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

HEADERS += pen.h
SOURCES += pen.cpp
TARGET = PenProperty
