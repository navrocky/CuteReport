! include( ../designerModules.pri ) {
    error( Can not find the designerModules.pri file! )
}

TARGET = Preview

SOURCES += \
    preview.cpp \

HEADERS +=  \
    preview.h \

RESOURCES += \
    preview.qrc

