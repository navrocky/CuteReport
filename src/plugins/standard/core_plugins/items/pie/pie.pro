! include( ../../plugins.pri ) {
    error( Can not find the common.pri file! )
}

! include ( pie.pri ) {
    error( Can not find item .pri file! )
}

TARGET = Pie

HEADERS += \
    pie_p.h
