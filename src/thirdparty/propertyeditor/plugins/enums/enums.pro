! include( ../property_plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

HEADERS += enums.h
SOURCES += enums.cpp
TARGET = EnumsProperty

