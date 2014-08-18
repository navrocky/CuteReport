! include( ../property_plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

HEADERS += brush.h
SOURCES += brush.cpp
TARGET = BrushProperty
