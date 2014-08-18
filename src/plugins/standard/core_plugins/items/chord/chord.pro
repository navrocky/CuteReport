! include( ../../plugins.pri ) {
    error( Can not find the common.pri file! )
}

! include ( chord.pri ) {
    error( Can not find item .pri file! )
}

TARGET = Chord

HEADERS += \
    chord_p.h
