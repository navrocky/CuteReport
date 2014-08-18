! include( ../designerModules.pri ) {
    error( Can not find the designerModules.pri file! )
}

TARGET = ReportEditor

SOURCES += \
    reportcontainer.cpp \
    reporteditor.cpp \
    reportproperties.cpp \

HEADERS +=  \
    reportcontainer.h \
    reporteditor.h \
    reportproperties.h \

FORMS += reportcontainer.ui \
    reportproperties.ui \

RESOURCES += \
    reporteditor.qrc

