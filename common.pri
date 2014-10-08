VERSION = 1.0
REPORT_VERSION=$$VERSION

#DEFINES += SYSTEMINSTALL
#DEFINES += STATICBUILD
DEFINES += DEMO

BUILD_DIR = build
# path for variables in home dir
REPORT_VARS_PATH = "temp/cutereport/"
#local path for devoping version installation
USER_INSTALL_PATH = "/home/alex/temp/install/cutereport/"


contains(DEFINES, SYSTEMINSTALL) {

    REPORT_ROOT_PATH = "/usr"
    #systemwide install: for linux distributions

    REPORT_LIBS_PATH =                   $$REPORT_ROOT_PATH/lib/
    REPORT_HEADERS_PATH =                $$REPORT_ROOT_PATH/include/cutereport
    REPORT_RESOURCES_PATH =              $$REPORT_ROOT_PATH/share/cutereport
    REPORT_IMAGES_PATH =                 $$REPORT_ROOT_PATH/share/cutereport/images
    REPORT_PLUGINS_PATH =                $$REPORT_LIBS_PATH/cutereport/reportcore_plugins
    REPORT_EXAMPLES_PATH =               $$REPORT_ROOT_PATH/share/cutereport/examples

    REPORT_DESIGNER_PATH =               $$REPORT_ROOT_PATH/bin
    REPORT_DESIGNER_PLUGINS_PATH =       $$REPORT_LIBS_PATH/cutereport/designer_plugins
    REPORT_DESIGNER_LIBS_PATH =          $$REPORT_LIBS_PATH
    REPORT_DESIGNER_IMAGES_PATH =        $$REPORT_RESOURCES_PATH/designer/images
    REPORT_DESIGNER_RESOURCES_PATH =     $$REPORT_RESOURCES_PATH/designer/

    PROPERTYEDITOR_LIBS_PATH =           $$REPORT_LIBS_PATH
    PROPERTYEDITOR_HEADERS_PATH =        $$REPORT_ROOT_PATH/include/propertyeditor
    PROPERTYEDITOR_PLUGINS_PATH =        $$PROPERTYEDITOR_LIBS_PATH/propertyeditor

    QTDESIGNER_PLUGINS_PATH =

} else {

    REPORT_ROOT_PATH = $$USER_INSTALL_PATH
    # all in one folder install

    REPORT_LIBS_PATH =
    REPORT_HEADERS_PATH =                cutereport/include
    REPORT_RESOURCES_PATH =              cutereport/share
    REPORT_IMAGES_PATH =                 cutereport/images
    REPORT_PLUGINS_PATH =                cutereport/plugins
    REPORT_EXAMPLES_PATH =               cutereport/examples

    REPORT_DESIGNER_PATH =
    REPORT_DESIGNER_PLUGINS_PATH =       cutereport/designer/plugins
    REPORT_DESIGNER_LIBS_PATH =
    REPORT_DESIGNER_IMAGES_PATH=         cutereport/designer/images
    REPORT_DESIGNER_RESOURCES_PATH =     cutereport/designer/share

    PROPERTYEDITOR_LIBS_PATH =
    PROPERTYEDITOR_HEADERS_PATH =        cutereport/include/propertyeditor
    PROPERTYEDITOR_PLUGINS_PATH =        cutereport/propertyeditor/plugins

    QTDESIGNER_PLUGINS_PATH =            designer
}


# -------------------------------------------------------------
DEFINES += REPORT_VERSION=\\\"$$REPORT_VERSION\\\"
DEFINES += REPORT_VARS_PATH=\\\"$$REPORT_VARS_PATH\\\"

DEFINES += REPORT_LIBS_PATH=\\\"$$REPORT_LIBS_PATH\\\"
DEFINES += REPORT_HEADERS_PATH=\\\"$$REPORT_HEADERS_PATH\\\"
DEFINES += REPORT_RESOURCES_PATH=\\\"$$REPORT_RESOURCES_PATH\\\"
DEFINES += REPORT_IMAGES_PATH=\\\"$$REPORT_IMAGES_PATH\\\"
DEFINES += REPORT_PLUGINS_PATH=\\\"$$REPORT_PLUGINS_PATH\\\"
DEFINES += REPORT_EXAMPLES_PATH=\\\"$$REPORT_EXAMPLES_PATH\\\"

DEFINES += REPORT_DESIGNER_PATH=\\\"$$REPORT_DESIGNER_PATH\\\"
DEFINES += REPORT_DESIGNER_PLUGINS_PATH=\\\"$$REPORT_DESIGNER_PLUGINS_PATH\\\"
DEFINES += REPORT_DESIGNER_LIBS_PATH=\\\"$$REPORT_DESIGNER_LIBS_PATH\\\"
DEFINES += REPORT_DESIGNER_IMAGES_PATH=\\\"$$REPORT_DESIGNER_IMAGES_PATH\\\"
DEFINES += REPORT_DESIGNER_RESOURCES_PATH=\\\"$$REPORT_DESIGNER_RESOURCES_PATH\\\"

DEFINES += PROPERTYEDITOR_LIBS=\\\"$$PROPERTYEDITOR_LIBS_PATH\\\"
DEFINES += PROPERTYEDITOR_HEADERS_PATH=\\\"$$PROPERTYEDITOR_HEADERS_PATH\\\"
DEFINES += PROPERTYEDITOR_PLUGINS_PATH=\\\"$$PROPERTYEDITOR_PLUGINS_PATH\\\"


INCLUDEPATH += lib/
DEPENDPATH += $$INCLUDEPATH

lessThan(QT_MAJOR_VERSION, 5) {
    QT += script xml sql
} else {
    QT += script xml sql widgets
}

CONFIG -= debug_and_release

exists(src/plugins/extended/extended.pro) {
    DEFINES += EXTENDEDSUITE
}

contains(DEFINES, STATICBUILD) {
    DEFINES += STATIC_PROPERTYEDITOR
    DEFINES += STATICPLUGINS_PROPERTYEDITOR
    DEFINES += STATIC_DESIGNER
    DEFINES += STATICPLUGINS_DESIGNER
    DEFINES += STATIC_CORE
    DEFINES += STATICPLUGINS_CORE
    DEFINES += STATIC_WIDGETS
}
