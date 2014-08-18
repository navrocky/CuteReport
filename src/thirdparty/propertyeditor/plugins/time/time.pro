! include( ../property_plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

HEADERS += timeprop.h
SOURCES += timeprop.cpp
TARGET = TimeProperty
