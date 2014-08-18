! include( ../../plugins.pri ) {
    error( Can not find the common.pri file! )
}

! include ( arc.pri ) {
    error( Can not find item .pri file! )
}

TARGET = Arc

HEADERS += \
    arc_p.h
