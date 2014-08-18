/***************************************************************************
 *   This file is part of the CuteReport project                           *
 *   Copyright (C) 2012-2014 by Alexander Mikhalov                         *
 *   alexander.mikhalov@gmail.com                                          *
 *                                                                         *
 **                   GNU General Public License Usage                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 **                  GNU Lesser General Public License                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library.                                      *
 *   If not, see <http://www.gnu.org/licenses/>.                           *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

QScriptValue qtscript_create_QObject_class(QScriptEngine *engine);
QScriptValue qtscript_create_Qt_class(QScriptEngine *engine);
QScriptValue qtscript_create_Global_class(QScriptEngine *engine);
QScriptValue qtscript_create_QPoint_class(QScriptEngine *engine);
QScriptValue qtscript_create_QPointF_class(QScriptEngine *engine);
QScriptValue qtscript_create_QSize_class(QScriptEngine *engine);
QScriptValue qtscript_create_QSizeF_class(QScriptEngine *engine);
QScriptValue qtscript_create_QRect_class(QScriptEngine *engine);
QScriptValue qtscript_create_QRectF_class(QScriptEngine *engine);
QScriptValue qtscript_create_QMargins_class(QScriptEngine *engine);
QScriptValue qtscript_create_QRegExp_class(QScriptEngine *engine);
QScriptValue qtscript_create_QTime_class(QScriptEngine *engine);
QScriptValue qtscript_create_QDate_class(QScriptEngine *engine);
QScriptValue qtscript_create_QDateTime_class(QScriptEngine *engine);
QScriptValue qtscript_create_QTimer_class(QScriptEngine *engine);
QScriptValue qtscript_create_QBuffer_class(QScriptEngine *engine);
QScriptValue qtscript_create_QAbstractTableModel_class(QScriptEngine *engine);
QScriptValue qtscript_create_QAbstractListModel_class(QScriptEngine *engine);
QScriptValue qtscript_create_QFont_class(QScriptEngine *engine);
QScriptValue qtscript_create_QFontMetrics_class(QScriptEngine *engine);
QScriptValue qtscript_create_QFontMetricsF_class(QScriptEngine *engine);
QScriptValue qtscript_create_QIcon_class(QScriptEngine *engine);
QScriptValue qtscript_create_QPixmap_class(QScriptEngine *engine);
QScriptValue qtscript_create_QImage_class(QScriptEngine *engine);
QScriptValue qtscript_create_QColor_class(QScriptEngine *engine);
QScriptValue qtscript_create_QBrush_class(QScriptEngine *engine);
QScriptValue qtscript_create_QPen_class(QScriptEngine *engine);
QScriptValue qtscript_create_QGradient_class(QScriptEngine *engine);
QScriptValue qtscript_create_QRadialGradient_class(QScriptEngine *engine);
QScriptValue qtscript_create_QLinearGradient_class(QScriptEngine *engine);
QScriptValue qtscript_create_QConicalGradient_class(QScriptEngine *engine);



static const char * const qtscript_cutereport_class_names[] = {
    "QObject"
    , "Qt"
    , "Global"
    , "QPoint"
    , "QPointF"
    , "QSize"
    , "QSizeF"
    , "QRect"
    , "QRectF"
    , "QMargins"
    , "QRegExp"
    , "QTime"
    , "QDate"
    , "QDateTime"
    , "QTimer"
    , "QBuffer"
    , "QAbstractTableModel"
    , "QAbstractListModel"
    , "QFont"
    , "QFontMetrics"
    , "QFontMetricsF"
    , "QIcon"
    , "QPixmap"
    , "QImage"
    , "QColor"
    , "QBrush"
    , "QPen"
    , "QGradient"
    , "QRadialGradient"
    , "QLinearGradient"
    , "QConicalGradient"
};

typedef QScriptValue (*QtBindingCreator)(QScriptEngine *engine);
static const QtBindingCreator qtscript_cutereport_class_functions[] = {
    qtscript_create_QObject_class
    , qtscript_create_Qt_class
    , qtscript_create_Global_class
    , qtscript_create_QPoint_class
    , qtscript_create_QPointF_class
    , qtscript_create_QSize_class
    , qtscript_create_QSizeF_class
    , qtscript_create_QRect_class
    , qtscript_create_QRectF_class
    , qtscript_create_QMargins_class
    , qtscript_create_QRegExp_class
    , qtscript_create_QTime_class
    , qtscript_create_QDate_class
    , qtscript_create_QDateTime_class
    , qtscript_create_QTimer_class
    , qtscript_create_QBuffer_class
    , qtscript_create_QAbstractTableModel_class
    , qtscript_create_QAbstractListModel_class
    , qtscript_create_QFont_class
    , qtscript_create_QFontMetrics_class
    , qtscript_create_QFontMetricsF_class
    , qtscript_create_QIcon_class
    , qtscript_create_QPixmap_class
    , qtscript_create_QImage_class
    , qtscript_create_QColor_class
    , qtscript_create_QBrush_class
    , qtscript_create_QPen_class
    , qtscript_create_QGradient_class
    , qtscript_create_QRadialGradient_class
    , qtscript_create_QLinearGradient_class
    , qtscript_create_QConicalGradient_class

};

void qtscript_initialize_cutereport_bindings(QScriptValue &extensionObject)
{
    QScriptEngine *engine = extensionObject.engine();
    for (int i = 0; i < 31; ++i) {
        extensionObject.setProperty(qtscript_cutereport_class_names[i],
            qtscript_cutereport_class_functions[i](engine),
            QScriptValue::SkipInEnumeration);
    }
}
