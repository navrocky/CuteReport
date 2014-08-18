! include( ../../plugins.pri ) {
    error( Can not find the common.pri file! )
}

! include ( svg.pri ) {
    error( Can not find item .pri file! )
}

TARGET = ExportSVG

RESOURCES += \
    svg.qrc
