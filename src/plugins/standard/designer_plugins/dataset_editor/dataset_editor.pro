! include( ../designerModules.pri ) {
    error( Can not find the designerModules.pri file! )
}

TARGET = DatasetEditor

SOURCES += \
    datasetcontainer.cpp \
    dataseteditor.cpp \

HEADERS +=  \
    datasetcontainer.h \
    dataseteditor.h \

FORMS += datasetcontainer.ui \

RESOURCES += \
    dataset_editor.qrc

