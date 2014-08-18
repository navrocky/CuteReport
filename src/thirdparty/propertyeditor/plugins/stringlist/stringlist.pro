! include( ../property_plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

TARGET = StringListProperty
SOURCES += stringlist.cpp stringlisteditor.cpp
HEADERS += stringlisteditor.h stringlist.h
FORMS += stringlisteditor.ui
