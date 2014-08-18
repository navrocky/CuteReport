! include( ../../plugins.pri ) {
    error( Could not find the common.pri file! )
}

! include ( barcode.pri ) {
    error( Could not find item .pri file! )
}


TARGET = Barcode

DEFINES += ZINT_VERSION=\\\"2.4.4\\\"
DEFINES += NO_PNG

win32-msvc* {
    DEFINES += _CRT_SECURE_NO_WARNINGS
    QMAKE_CFLAGS += /TP /wd4018 /wd4244 /wd4305
    QMAKE_CXXFLAGS += /TP /wd4018 /wd4244 /wd4305
}

INCLUDEPATH += zint zint/backend zint/backend_qt4

HEADERS += \
    zint/backend_qt4/qzint.h \
    zint/backend/zint.h \
    zint/backend/sjis.h \
    zint/backend/rss.h \
    zint/backend/reedsol.h \
    zint/backend/qr.h \
    zint/backend/pdf417.h \
    zint/backend/ms_stdint.h \
    zint/backend/maxipng.h \
    zint/backend/maxicode.h \
    zint/backend/large.h \
    zint/backend/gs1.h \
    zint/backend/gridmtx.h \
    zint/backend/gb2312.h \
    zint/backend/font.h \
    zint/backend/dmatrix.h \
    zint/backend/composite.h \
    zint/backend/common.h \
    zint/backend/code49.h \
    zint/backend/code1.h \
    zint/backend/aztec.h \

SOURCES += \
    zint/backend_qt4/qzint.cpp \
    zint/backend/library.c \
    zint/backend/upcean.c \
    zint/backend/telepen.c \
    zint/backend/svg.c \
    zint/backend/rss.c \
    zint/backend/render.c \
    zint/backend/reedsol.c \
    zint/backend/qr.c \
    zint/backend/ps.c \
    zint/backend/postal.c \
    zint/backend/png.c \
    zint/backend/plessey.c \
    zint/backend/pdf417.c \
    zint/backend/medical.c \
    zint/backend/maxicode.c \
    zint/backend/large.c \
    zint/backend/imail.c \
    zint/backend/gs1.c \
    zint/backend/gridmtx.c \
    zint/backend/dmatrix.c \
    zint/backend/dllversion.c \
    zint/backend/composite.c \
    zint/backend/common.c \
    zint/backend/code128.c \
    zint/backend/code49.c \
    zint/backend/code16k.c \
    zint/backend/code1.c \
    zint/backend/code.c \
    zint/backend/aztec.c \
    zint/backend/auspost.c \
    zint/backend/2of5.c

