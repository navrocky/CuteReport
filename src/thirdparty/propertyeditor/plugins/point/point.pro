! include( ../property_plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

HEADERS += point.h
SOURCES += point.cpp
TARGET = PointProperty
