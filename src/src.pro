TEMPLATE = subdirs
SUBDIRS = \
        thirdparty \
        core \
        plugins \
        apps \
        widgets \

widgets.depends = core
apps.depends = core thirdparty
plugins.depends = core widgets apps thirdparty
