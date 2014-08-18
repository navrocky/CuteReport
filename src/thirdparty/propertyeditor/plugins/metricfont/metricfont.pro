! include( ../property_plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

HEADERS += font.h changefont.h  metricfontdialog.h
SOURCES += font.cpp changefont.cpp  metricfontdialog.cpp
TARGET = FontProperty
FORMS += metricfontdialog.ui

