! include( ../property_plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

HEADERS += sizef.h
SOURCES += sizef.cpp
TARGET = SizeFProperty
