! include( ../../common.pri ) {
    error( Couldn\'t find the common.pri file! )
}

TEMPLATE = lib
DEFINES += STATICTYPES

INCLUDEPATH += lib \
	plugins/bool \
	plugins/brush \
	plugins/color \
	plugins/date \
	plugins/dateTime \
	plugins/enums \
	plugins/flags \
	plugins/font \
	plugins/numbers \
	plugins/pen \
	plugins/pixmap \

DEPENDPATH += $$INCLUDEPATH

CONFIG +=   exceptions \
dll \
  rtti

#win32: DEFINES += LIB_EXPORTS

HEADERS += propertyeditor.h \
 propertymodel.h \
 propertyinterface.h \
 propertydelegate.h \
 bool.h \
 brush.h \
 color.h \
 date.h \
 datetime.h \
 enums.h \
 flags.h \
 font.h \
 changefont.h \
 numbers.h \
 pen.h \
 pixmap.h \
 pixmapdialog.h

SOURCES += propertyeditor.cpp \
 propertymodel.cpp \
 propertyinterface.cpp \
 propertydelegate.cpp \
 bool.cpp \
 brush.cpp \
 color.cpp \
 date.cpp \
 datetime.cpp \
 enums.cpp \
 flags.cpp \
 font.cpp \
 changefont.cpp \
 numbers.cpp \
 pen.cpp \
 pixmap.cpp \
 pixmapdialog.cpp
 
RESOURCES += bool.qrc \

FORMS += flagsList.ui \
	pixmapDialog.ui


TARGET = PropertyEditor

DESTDIR = $$PROPERTY_EDITOR_INSTALL_LIBS


