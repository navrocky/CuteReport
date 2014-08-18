! include( ../../plugins.pri ) {
    error( Couldn't find the common.pri file! )
}

! include ( image.pri ) {
    error( Couldn't find item .pri file! )
}

TARGET = Image
