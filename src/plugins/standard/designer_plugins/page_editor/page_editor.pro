! include( ../designerModules.pri ) {
    error( Can not find the designerModules.pri file! )
}

TARGET = PageEditor

SOURCES += \
    pageeditorcontainer.cpp \
    pageeditor.cpp \
    itemstoolwidget.cpp

HEADERS +=  \
    pageeditorcontainer.h \
    pageeditor.h \
    itemstoolwidget.h \

FORMS += pageeditorcontainer.ui \

RESOURCES += \
    page_editor.qrc

