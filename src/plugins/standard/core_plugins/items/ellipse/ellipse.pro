! include( ../../plugins.pri ) {
    error( Can not find the common.pri file! )
}

! include ( ellipse.pri ) {
    error( Can not find item .pri file! )
}

TARGET = Ellipse

HEADERS += \
    ellipse_p.h
