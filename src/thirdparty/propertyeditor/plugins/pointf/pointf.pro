! include( ../property_plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

HEADERS += pointf.h
SOURCES += pointf.cpp
TARGET = PointFProperty
