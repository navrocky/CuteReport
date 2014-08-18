! include( ../property_plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

HEADERS += numbers.h
SOURCES += numbers.cpp
TARGET = NumbersProperty
