! include( ../../plugins.pri ) {
    error( Can not find the common.pri file! )
}

! include ( overlay.pri ) {
    error( Can not find item .pri file! )
}

TARGET = Overlay
