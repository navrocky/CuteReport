! include( ../../plugins.pri ) {
    error( Can not find the common.pri file! )
}


TARGET = xmlSerializer

HEADERS += \
    xml.h \

SOURCES += \
    xml.cpp \

