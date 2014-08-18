! include( ../../plugins.pri ) {
    error( Can not find the common.pri file! )
}

! include ( rectangle.pri ) {
    error( Can not find item .pri file! )
}

TARGET = Rectangle

HEADERS += \
    rectangle_p.h
