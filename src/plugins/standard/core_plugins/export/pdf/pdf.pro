! include( ../../plugins.pri ) {
    error( Can not find the common.pri file! )
}

! include ( pdf.pri ) {
    error( Can not find item .pri file! )
}

TARGET = ExportPDF

RESOURCES += \
    pdf.qrc


lessThan(QT_MAJOR_VERSION, 5) {
} else {
    QT += printsupport
}
