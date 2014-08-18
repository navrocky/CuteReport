! include( ../property_plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

FORMS += pixmapDialog.ui
SOURCES += pixmapdialog.cpp pixmap.cpp
HEADERS += pixmapdialog.h pixmap.h
TARGET = PixmapProperty
