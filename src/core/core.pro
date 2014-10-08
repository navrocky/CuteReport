! include( ../../common.pri ) {
    error( Can not find the common.pri file! )
}

include (log/log.pri)

DEPENDPATH += $$INCLUDEPATH

lessThan(QT_MAJOR_VERSION, 5) {
    CONFIG += uitools dll
} else {
    QT += widgets
}

win32 {
    DEFINES += LIB_EXPORTS
    TARGET_EXT = .dll
}

TARGET = CuteReport
TEMPLATE =  lib

HEADERS += \
    globals.h \
    reportcore.h \
    pageinterface.h \
    datasetinterface.h \
    reportplugininterface.h \
    reportinterface.h \
    types.h \
    baseiteminterface.h \
    baseiteminterface_p.h \
    iteminterface.h \
    iteminterface_p.h \
    bandinterface.h \
    bandinterface_p.h \
    storageinterface.h \
    serializerinterface.h \
    rendererinterface.h \
    rendererpublicinterface.h \
    item_common/simplerendereditem.h \
    item_common/simpleitemview.h \
    renderediteminterface.h \
    renderediteminterface_p.h \
    iteminterfaceview.h \
    printerinterface.h \
    exportinterface.h \
    objectfactory.h \
    forminterface.h \
    cutereport.h \
    functions.h \
    dummyitem.h \
    dummyband.h \
    renderedreport.h \
    renderedreportinterface.h \
    scriptextensioninterface.h \
    importinterface.h \
    designeriteminterface.h

SOURCES += \
    globals.cpp \
    reportcore.cpp \
    pageinterface.cpp \
    datasetinterface.cpp \
    reportplugininterface.cpp \
    reportinterface.cpp \
    types.cpp \
    baseiteminterface.cpp \
    iteminterface.cpp \
    bandinterface.cpp \
    storageinterface.cpp \
    serializerinterface.cpp \
    rendererinterface.cpp \
    rendererpublicinterface.cpp \
    item_common/simplerendereditem.cpp \
    item_common/simpleitemview.cpp \
    renderediteminterface.cpp \
    iteminterfaceview.cpp \
    printerinterface.cpp \
    exportinterface.cpp \
    objectfactory.cpp \
    forminterface.cpp \
    cutereport.cpp \
    functions.cpp \
    dummyitem.cpp \
    dummyband.cpp \
    renderedreport.cpp \
    renderedreportinterface.cpp \
    scriptextensioninterface.cpp \
    importinterface.cpp \
    designeriteminterface.cpp


PRIVATE_HEADERS.files = reportcore.h reportinterface.h datasetinterface.h bandinterface.h iteminterface.h baseiteminterface.h \
                        exportinterface.h forminterface.h printerinterface.h rendererinterface.h pageinterface.h storageinterface.h \
                        serializerinterface.h scriptextensioninterface.h
INSTALLS += PRIVATE_HEADERS

contains(DEFINES, SYSTEMINSTALL) {
    DESTDIR = ../../$$BUILD_DIR
    target.path = $$REPORT_LIBS_PATH
    PRIVATE_HEADERS.path = $$REPORT_HEADERS_PATH
    INSTALLS += target
} else {
    DESTDIR = ../../$$BUILD_DIR/$$REPORT_LIBS_PATH
    PRIVATE_HEADERS.path = $$OUT_PWD/../../$$BUILD_DIR/$$REPORT_HEADERS_PATH
}

contains(DEFINES, STATIC_CORE) {
    CONFIG += static
}
