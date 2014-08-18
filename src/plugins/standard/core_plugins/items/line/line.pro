! include( ../../plugins.pri ) {
    error( Can not find the common.pri file! )
}

! include ( line.pri ) {
    error( Can not find item .pri file! )
}

TARGET = Line

