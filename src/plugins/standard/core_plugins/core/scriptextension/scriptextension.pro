! include( ../../plugins.pri ) {
    error( Can not find the common.pri file! )
}

TARGET = Script
QT += script


HEADERS += scriptplugin.h \
           $$PWD/qtscriptshell_QAbstractItemModel.h \
           $$PWD/qtscriptshell_QAbstractListModel.h \
           $$PWD/qtscriptshell_QAbstractTableModel.h \
           $$PWD/qtscriptshell_QBuffer.h \
           $$PWD/qtscriptshell_QObject.h \
           $$PWD/qtscriptshell_QImage.h \
           $$PWD/qtscriptshell_QPixmap.h \

SOURCES += scriptplugin.cpp \
           $$PWD/cutereport_init.cpp \
           $$PWD/qtscript_Global.cpp \
           $$PWD/qtscript_QAbstractItemModel.cpp \
           $$PWD/qtscript_QAbstractListModel.cpp \
           $$PWD/qtscript_QAbstractTableModel.cpp \
           $$PWD/qtscript_QBuffer.cpp \
           $$PWD/qtscript_QByteArray.cpp \
           $$PWD/qtscript_QDate.cpp \
           $$PWD/qtscript_QDateTime.cpp \
           $$PWD/qtscript_QMargins.cpp \
           $$PWD/qtscript_QObject.cpp \
           $$PWD/qtscript_QPoint.cpp \
           $$PWD/qtscript_QPointF.cpp \
           $$PWD/qtscript_QRect.cpp \
           $$PWD/qtscript_QRectF.cpp \
           $$PWD/qtscript_QRegExp.cpp \
           $$PWD/qtscript_QSize.cpp \
           $$PWD/qtscript_QSizeF.cpp \
           $$PWD/qtscript_QTime.cpp \
           $$PWD/qtscript_QTimer.cpp \
           $$PWD/qtscript_Qt.cpp \
           $$PWD/qtscriptshell_QAbstractItemModel.cpp \
           $$PWD/qtscriptshell_QAbstractListModel.cpp \
           $$PWD/qtscriptshell_QAbstractTableModel.cpp \
           $$PWD/qtscriptshell_QBuffer.cpp \
           $$PWD/qtscriptshell_QDataStream.cpp \
           $$PWD/qtscriptshell_QObject.cpp \
           $$PWD/qtscriptshell_QTimer.cpp \
           $$PWD/qtscriptshell_QImage.cpp \
           $$PWD/qtscriptshell_QPixmap.cpp \
           $$PWD/qtscript_QFont.cpp \
           $$PWD/qtscript_QFontMetrics.cpp \
           $$PWD/qtscript_QFontMetricsF.cpp \
           $$PWD/qtscript_QIcon.cpp \
           $$PWD/qtscript_QPixmap.cpp \
           $$PWD/qtscript_QImage.cpp \
           $$PWD/qtscript_QColor.cpp \
           $$PWD/qtscript_QBrush.cpp \
           $$PWD/qtscript_QPen.cpp \
           $$PWD/qtscript_QGradient.cpp \
           $$PWD/qtscript_QLinearGradient.cpp \
           $$PWD/qtscript_QRadialGradient.cpp \
           $$PWD/qtscript_QConicalGradient.cpp \
