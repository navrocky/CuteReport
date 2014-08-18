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
    magnets.h


SOURCES += \
    page.cpp \
    pagegui.cpp \
    layoutmanager.cpp \
    pagemanipulator.cpp \
    pageitemselection.cpp \
    magnets.cpp

RESOURCES += page.qrc

#PRIVATE_IMAGES.files = images/*.png
#PRIVATE_IMAGES.path = $$REPORT_ICON_PATH
#INSTALLS += PRIVATE_IMAGES
