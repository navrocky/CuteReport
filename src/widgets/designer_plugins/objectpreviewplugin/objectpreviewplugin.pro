include($$PWD/../designer_plugins.pri)
include($$PWD/../../widgets/objectpreview.pri)

HEADERS += objectpreviewplugin.h
SOURCES += objectpreviewplugin.cpp

RESOURCES += \
    objectpreviewplugin.qrc

win32: DEFINES += NO_CUTEREPORT_WIDGET_EXPORTS
