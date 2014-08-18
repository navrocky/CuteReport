! include( ../property_plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

HEADERS += datetime.h
SOURCES += datetime.cpp
TARGET = DateTimeProperty
