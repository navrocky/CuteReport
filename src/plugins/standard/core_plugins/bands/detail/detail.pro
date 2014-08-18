! include( ../../plugins.pri ) {
    error( Can not find the common.pri file! )
}

! include ( detail.pri ) {
    error( Can not find item .pri file! )
}

TARGET = Detail
