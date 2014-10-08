TEMPLATE = subdirs
SUBDIRS = standard \

!include( ../../common.pri ) {
    error( Can not find the common.pri file! )
}

contains(DEFINES, EXTENDEDSUITE) {
    SUBDIRS += extended
}
