! include( ../property_plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

TARGET = BoolProperty
SOURCES += bool.cpp
HEADERS += bool.h
RESOURCES += bool.qrc



