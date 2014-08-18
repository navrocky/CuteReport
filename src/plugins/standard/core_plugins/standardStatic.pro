! include( ../../common.pri ) {
    error( Couldn\'t find the common.pri file! )
}

DEFINES += STATIC_SUITE

DESTDIR = $$REPORT_LIB_PATH

TARGET = StandardSuite
TEMPLATE =  lib

INCLUDEPATH += ../../libs/report/
INCLUDEPATH += bands/detail \
	bands/detailFooter \
	bands/detailHeader \
	bands/pageHeader \
	bands/pageFooter \
	bands/overlay \
	bands/summary \
	bands/title \
	datasets/sql \
	datasets/csv \
	items/arc \
        items/barcode \
        items/chord \
        items/ellipse \
        items/line \
        items/memo \
        items/picture \
        items/pie \
        items/rectangle \
        items/text \
        page \
        report \
	
	
DEPENDPATH += $$INCLUDEPATH


include( bands/detail/detail.pri )
include( bands/detailFooter/detailFooter.pri )
include( bands/detailHeader/detailHeader.pri )
include( bands/pageFooter/pageFooter.pri )
include( bands/pageHeader/pageHeader.pri )
include( bands/overlay/overlay.pri )
include( bands/summary/summary.pri )
include( bands/title/title.pri )
include( datasets/csv/csv.pri )
include( datasets/sql/sql.pri )
include( items/arc/arc.pri )
#include( items/barcode/barcode.pri )
include( items/chord/chord.pri )
include( items/ellipse/ellipse.pri )
include( items/line/line.pri )
include( items/memo/memo.pri )
include( items/picture/picture.pri )
include( items/pie/pie.pri )
include( items/rectangle/rectangle.pri )
#include( items/text/text.pri )
include( page/page.pri )
include( report/report.pri )

