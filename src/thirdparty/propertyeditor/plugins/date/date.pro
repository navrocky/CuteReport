! include( ../property_plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

HEADERS += date.h
SOURCES += date.cpp
TARGET = DateProperty
