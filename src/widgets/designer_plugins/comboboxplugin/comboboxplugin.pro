include($$PWD/../designer_plugins.pri)
include($$PWD/../../widgets/combobox.pri)

HEADERS += comboboxplugin.h 
SOURCES += comboboxplugin.cpp 

RESOURCES += comboboxplugin.qrc

win32: DEFINES += NO_CUTEREPORT_WIDGET_EXPORTS

