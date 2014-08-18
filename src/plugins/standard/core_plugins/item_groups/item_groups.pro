!include( ../../../../../common.pri ) {
    error( Can not find the common.pri file! )
}

SOURCES = fake.cpp

PRIVATE_IMAGES.files = group_*.png
INSTALLS += PRIVATE_IMAGES

contains(DEFINES, SYSTEMINSTALL) {
    PRIVATE_IMAGES.path = $$REPORT_IMAGES_PATH
} else {
    PRIVATE_IMAGES.path = $$OUT_PWD/../../../../../$$BUILD_DIR/$$REPORT_IMAGES_PATH
}
