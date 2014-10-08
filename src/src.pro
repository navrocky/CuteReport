!include( ../common.pri ) {
    error( Can not find the common.pri file! )
}

TEMPLATE = subdirs
SUBDIRS = \
        thirdparty \
        core \
        widgets \
        plugins \
        apps \

!contains(DEFINES, STATICBUILD) {
    widgets.depends = core
    plugins.depends = core widgets apps thirdparty
    apps.depends = core thirdparty widgets
}


