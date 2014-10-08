TEMPLATE = subdirs
SUBDIRS = src


!include( $$PWD/common.pri ) {
    error( Can not find the common.pri file! )
}

!contains(DEFINES, SYSTEMINSTALL) {
    unix:!macx {
        startscripts.files = $$PWD/pkg/cutereport.sh
        startscripts.path =  $$OUT_PWD/$$BUILD_DIR/$$REPORT_DESIGNER_PATH
        INSTALLS += startscripts
    }
    win32:{
        installscripts.files = $$PWD/pkg/install.iss
        installscripts.path = $$OUT_PWD/$$BUILD_DIR/
        INSTALLS += installscripts
        warning(dir = $$installscripts.path)
    }
}

ADDITIONAL_FILES.files = COPYING pkg/*.ico
DATASET_FILES.files = examples/datasets/*
REPORT_FILES.files = examples/reports/*
TEMPLATE_FILES.files = examples/templates/*
IMAGE_FILES.files = examples/images/*

INSTALLS += ADDITIONAL_FILES DATASET_FILES REPORT_FILES TEMPLATE_FILES IMAGE_FILES

contains(DEFINES, SYSTEMINSTALL) {
    ADDITIONAL_FILES.path = $$REPORT_RESOURCES_PATH
    DATASET_FILES.path = $$REPORT_EXAMPLES_PATH/datasets/
    REPORT_FILES.path = $$REPORT_EXAMPLES_PATH/reports/
    TEMPLATE_FILES.path = $$REPORT_EXAMPLES_PATH/templates/
    IMAGE_FILES.path = $$REPORT_EXAMPLES_PATH/images/
} else {
    ADDITIONAL_FILES.path = $$OUT_PWD/$$BUILD_DIR
    DATASET_FILES.path = $$OUT_PWD/$$BUILD_DIR/$$REPORT_EXAMPLES_PATH/datasets/
    REPORT_FILES.path = $$OUT_PWD/$$BUILD_DIR/$$REPORT_EXAMPLES_PATH/reports/
    TEMPLATE_FILES.path = $$OUT_PWD/$$BUILD_DIR/$$REPORT_EXAMPLES_PATH/templates/
    IMAGE_FILES.path = $$OUT_PWD/$$BUILD_DIR/$$REPORT_EXAMPLES_PATH/images/
}

