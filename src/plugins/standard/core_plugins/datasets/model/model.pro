! include( ../../plugins.pri ) {
    error( Can not find the common.pri file! )
}

! include ( model.pri ) {
    error( Can not find model.pri file! )
}

TARGET = DatasetModel

INCLUDEPATH += \
            models \

HEADERS += \

SOURCES += \
