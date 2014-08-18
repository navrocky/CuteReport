! include( ../../plugins.pri ) {
    error( Can not find the common.pri file! )
}


TARGET = StorageGIT

HEADERS += \
    git.h \
    gitstoragehelper.h \
    syncmanager.h

SOURCES += \
    git.cpp \
    gitstoragehelper.cpp \
    syncmanager.cpp

FORMS += \
    gitstoragehelper.ui \

RESOURCES += \
    gitresources.qrc

lessThan(QT_MAJOR_VERSION, 5) {
} else {
    QT += concurrent
}
