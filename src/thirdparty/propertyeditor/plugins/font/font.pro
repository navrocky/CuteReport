! include( ../../../../common.pri ) {
    error( Couldn\'t find the common.pri file! )
}

TEMPLATE = lib

CONFIG += exceptions \
dll \
plugin \
  rtti

HEADERS += font.h \
changefont.h

SOURCES += font.cpp \
changefont.cpp

TARGET = FontProperty

DESTDIR = .

INCLUDEPATH += $$PROPERTY_EDITOR_INSTALL_HEADERS

macx{
	QMAKE_LFLAGS +=  -F$$[PROPERTY_EDITOR_INSTALL_LIBS]
 	LIBS += -framework PropertyEditor
}else{
	LIBS += -L$$PROPERTY_EDITOR_INSTALL_LIBS -lPropertyEditor 
}


target.path = $$PROPERTY_EDITOR_INSTALL_PLUGINS

INSTALLS += target

