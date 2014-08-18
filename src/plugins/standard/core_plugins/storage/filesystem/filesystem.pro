! include( ../../plugins.pri ) {
    error( Couldn\\\'t find the common.pri file! )
}


TARGET = StorageFileSystem

HEADERS += \
    filesystem.h \
    filesystemstoragehelper.h \

SOURCES += \
    filesystem.cpp \
    filesystemstoragehelper.cpp \

FORMS += \
    filesystemstoragehelper.ui \

RESOURCES += \
    filesystem.qrc
