! include( ../../plugins.pri ) {
    error( Can not find the common.pri file! )
}

! include ( form.pri ) {
    error( Can not find item .pri file! )
}

lessThan(QT_MAJOR_VERSION, 5) {
    CONFIG += uitools
} else {
    QT += uitools
}


TARGET = Form

HEADERS += \
    mainthreadobject.h

SOURCES += \
    mainthreadobject.cpp
