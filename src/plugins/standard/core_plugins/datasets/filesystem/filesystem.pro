! include( ../../plugins.pri ) {
    error( Can not find the common.pri file! )
}

! include ( filesystem.pri ) {
    error( Can not find filesystem.pri file! )
}

TARGET = DatasetFileSystem
