! include( ../designerModules.pri ) {
    error( Can not find the designerModules.pri file! )
}

TARGET = ScriptEditor

SOURCES += \
    scripteditor.cpp \
    qscriptsyntaxhighlighter.cpp \
    qscriptedit.cpp \

HEADERS +=  \
    scripteditor.h \
    qscriptsyntaxhighlighter_p.h \
    qscriptedit_p.h \
    qfunctions_p.h \

RESOURCES += \
    script_editor.qrc


#PRIVATE_IMAGES.files = images/*.png
#PRIVATE_IMAGES.path = $$REPORT_APPS_PLUGINS_IMAGES_PATH
#INSTALLS += target PRIVATE_IMAGES
