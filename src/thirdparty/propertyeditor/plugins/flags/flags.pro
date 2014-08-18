! include( ../property_plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

HEADERS += flags.h flagslist.h
SOURCES += flags.cpp flagslist.cpp
FORMS += flagsList.ui
TARGET = FlagsProperty
