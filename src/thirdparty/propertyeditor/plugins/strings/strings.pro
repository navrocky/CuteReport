! include( ../property_plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

HEADERS += strings.h
SOURCES += strings.cpp
TARGET = StringsProperty
