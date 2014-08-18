! include( ../../plugins.pri ) {
    error( Can not find the common.pri file! )
}

! include ( title.pri ) {
    error( Can not find item .pri file! )
}

TARGET = Title
