! include( ../property_plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

HEADERS += size.h
SOURCES += size.cpp
TARGET = SizeProperty
