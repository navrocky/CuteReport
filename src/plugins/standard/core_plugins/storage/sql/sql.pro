! include( ../../plugins.pri ) {
    error( Can not find the common.pri file! )
}

QT       += sql

TARGET = StorageSQL

TEMPLATE = lib

SOURCES += sql.cpp \
    sqlstoragehelper.cpp

HEADERS += sql.h\
    sqlstoragehelper.h

FORMS += \
    sqlstoragehelper.ui
