! include( ../../plugins.pri ) {
    error( Can not find the common.pri file! )
}


TARGET = StorageResource

HEADERS += \
    resource.h \
    resourcestoragehelper.h \

SOURCES += \
    resource.cpp \
    resourcestoragehelper.cpp \

FORMS += \
    resourcestoragehelper.ui \

RESOURCES += \
    resource_storage.qrc
