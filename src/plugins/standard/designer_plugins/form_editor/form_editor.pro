! include( ../designerModules.pri ) {
    error( Can not find the designerModules.pri file! )
}

TARGET = UiEditor

qtAddLibrary(QtDesignerComponents)
CONFIG += designer

QT += xml network
contains(QT_CONFIG, script): QT += script

SOURCES += \
    ui_editor.cpp \
    uicontainer.cpp \
    itemstoolwidget.cpp \
    newformdialog.cpp

HEADERS +=  \
    ui_editor.h \
    uicontainer.h \
    itemstoolwidget.h \
    newformdialog.h

HEADERS += \
#    qdesigner_toolwindow.h \
    qdesigner_formwindow.h \
    qdesigner_workbench.h \
    qdesigner_settings.h \
    qdesigner_actions.h \
    saveformastemplate.h \
    newform.h \
    versiondialog.h \
    designer_enums.h \
    preferencesdialog.h \
    designerrootwidget.h

SOURCES += main.cpp \
    qdesigner_formwindow.cpp \
    qdesigner_workbench.cpp \
    qdesigner_settings.cpp \
    qdesigner_actions.cpp \
    saveformastemplate.cpp \
    newform.cpp \
    versiondialog.cpp \
    preferencesdialog.cpp \
    designerrootwidget.cpp

INCLUDEPATH +=  \
    ../designerCore/widgets/ \
    qtdesigner/tools/designer/src/designer \
    qtdesigner/tools/designer/src/lib/sdk \
    qtdesigner/tools/designer/src/lib/shared \
    qtdesigner/tools/designer/src/lib/uilib \
    qtdesigner/include/ \
    qtdesigner/tools/shared/

DEPENDPATH += $$INCLUDEPATH

#include(qtdesigner/tools/designer/src/sharedcomponents.pri)
#include(qtdesigner/tools/shared/qttoolbardialog/qttoolbardialog.pri)

#LIBS += -lQtDesignerComponents -lQtDesigner

PRECOMPILED_HEADER=qdesigner_pch.h

FORMS += \
    uicontainer.ui \
    saveformastemplate.ui \
    preferencesdialog.ui \
    testmainwindow.ui \
    designerrootwidget.ui \
    newformdialog.ui

#PRIVATE_TEMPLATES.files = templates/*.ui
#PRIVATE_TEMPLATES.path = $$REPORT_APPS_PLUGINS_RES_PATH/uieditor_templates/
#INSTALLS += PRIVATE_TEMPLATES

RESOURCES += \
    form_editor.qrc

unix:!mac:LIBS += -lm

