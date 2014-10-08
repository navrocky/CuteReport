! include( ../../plugins.pri ) {
    error( Can not find the common.pri file! )
}


TARGET = SerializerXML

HEADERS += \
    serializerxml.h \

SOURCES += \
    serializerxml.cpp \

