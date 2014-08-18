! include( ../../plugins.pri ) {
    error( Can not find the common.pri file! )
}

! include ( printer.pri ) {
    error( Can not find item .pri file! )
}

TARGET = Printer

lessThan(QT_MAJOR_VERSION, 5) {
} else {
    QT += printsupport
}

