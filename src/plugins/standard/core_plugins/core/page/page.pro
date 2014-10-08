!include( ../../plugins.pri ) {
    error( Can not find the common.pri file! )
}


TARGET = Page

HEADERS += \
    page.h \
    pagegui.h \
    layoutmanager.h \
    pagemanipulator.h \
    pageitemselection.h \
    magnets.h \
    scene.h \
    view.h


SOURCES += \
    page.cpp \
    pagegui.cpp \
    layoutmanager.cpp \
    pagemanipulator.cpp \
    pageitemselection.cpp \
    magnets.cpp \
    scene.cpp \
    view.cpp

RESOURCES += page.qrc

