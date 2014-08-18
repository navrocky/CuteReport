TEMPLATE = subdirs
SUBDIRS = standard \


exists(extended/extended.pro) {
    SUBDIRS += extended
}
