! include( ../property_plugins.pri ) {
    error( Couldn\'t find the plugins.pri file! )
}

HEADERS += unit_prop.h
SOURCES += unit_prop.cpp
TARGET = UnitProperty
