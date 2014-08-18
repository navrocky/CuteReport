! include( ../../plugins.pri ) {
    error( Can not find the common.pri file! )
}

! include ( detailHeader.pri ) {
    error( Can not find item .pri file! )
}

TARGET = DetailHeader

HEADERS += \
    detailheader_p.h
