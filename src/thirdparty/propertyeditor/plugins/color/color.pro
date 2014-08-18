! include( ../property_plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

HEADERS += color.h changecolor.h
SOURCES += color.cpp changecolor.cpp
TARGET = ColorProperty
