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
 ****************************************************************************/

#include "functions.h"

#include <QtCore>
#include <QWidget>


namespace CuteReport
{

static const char * unitStr[] = {QT_TR_NOOP("Millimeter"), QT_TR_NOOP("Point"), QT_TR_NOOP("Inch"), QT_TR_NOOP("Pica"),  QT_TR_NOOP("Didot"),
                                 QT_TR_NOOP("Cicero"), QT_TR_NOOP("Pixel"), QT_TR_NOOP("NotDefiled") };
static const char * unitShortStr[] = {QT_TR_NOOP("mm"), QT_TR_NOOP("point"), QT_TR_NOOP("inch"), QT_TR_NOOP("pica"),  QT_TR_NOOP("didot"),
                                      QT_TR_NOOP("cicero"), QT_TR_NOOP("px"), QT_TR_NOOP("N/D") };

static const QString variablesRegExp("(?:^|[^\\\\](?=\\$))\\$\\{(\\w+)\\}");
static const int variablesRegExpNum = 1;

QRectF convertUnit(const QRectF & rect, Unit unitIn, Unit unitOut, int dpi)
{
    if (unitIn == unitOut)
        return rect;

    // first: convert to mm
    QRectF rectMM;
    switch (unitIn) {
    case UnitNotDefined:
    case Millimeter:
        rectMM = rect;
        break;
    case Inch:
        rectMM = QRectF( QPointF( inch2mm(rect.left()), inch2mm(rect.top())),
                         QPointF( inch2mm(rect.right()), inch2mm(rect.bottom())) );
        break;
    case Pixel:
        rectMM = QRectF( QPointF ( pixel2mm(rect.left(), dpi), pixel2mm(rect.top(), dpi)),
                         QPointF ( pixel2mm(rect.right(), dpi), pixel2mm(rect.bottom(), dpi)) );
        break;
    default: return QRectF();
    }

    // second: convert to output unit
    switch (unitOut) {
    case UnitNotDefined:
    case Millimeter:
        return rectMM;
    case Inch:
        return QRectF( QPointF( mm2inch(rectMM.left()), mm2inch(rectMM.top())),
                       QPointF( mm2inch(rectMM.right()), mm2inch(rectMM.bottom())) );
    case Pixel:
        return QRectF( QPointF( mm2pixel_ceil(rectMM.left(), dpi), mm2pixel_ceil(rectMM.top(), dpi)),
                       QPointF( mm2pixel_ceil(rectMM.right(), dpi), mm2pixel_ceil(rectMM.bottom(), dpi)) );
    default: return QRectF();
    }

    return rect;
}


QPointF convertUnit(const QPointF & point, Unit unitIn, Unit unitOut, int dpi)
{
    if (unitIn == unitOut)
        return point;

    QPointF pointMM;
    // first: convert to mm
    switch (unitIn) {
    case UnitNotDefined:
    case Millimeter:
        pointMM = point; break;
    case Inch:
        pointMM = QPointF(inch2mm(point.x()), inch2mm(point.y())); break;
    case Pixel:
        pointMM = QPointF(pixel2mm(point.x(), dpi), pixel2mm(point.y(), dpi)); break;
    default: return QPointF();
    }

    // second: convert to output meassure
    switch (unitOut) {
    case UnitNotDefined:
    case Millimeter:
        return pointMM;
    case Inch:
        return QPointF(mm2inch(pointMM.x()), mm2inch(pointMM.y()));
    case Pixel:
        return QPointF(mm2pixel_ceil(pointMM.x(), dpi), mm2pixel_ceil(pointMM.y(), dpi));
    default: return QPointF();
    }

    return point;
}


QSizeF convertUnit(const QSizeF & size, Unit unitIn, Unit unitOut, int dpi)
{
    if (unitIn == unitOut)
        return size;

    QSizeF sizeMM;
    // first: convert to mm
    switch (unitIn) {
    case UnitNotDefined:
    case Millimeter:
        sizeMM = size; break;
    case Inch:
        sizeMM = QSizeF(inch2mm(size.width()), inch2mm(size.height())); break;
    case Pixel:
        sizeMM = QSizeF(pixel2mm(size.width(), dpi), pixel2mm(size.height(), dpi)); break;
    default:
        sizeMM = size;
    }

    // second: convert to output meassure
    switch (unitOut) {
    case UnitNotDefined:
    case Millimeter:
        return sizeMM;
    case Inch:
        return QSizeF(mm2inch(sizeMM.width()), mm2inch(sizeMM.height()));
    case Pixel:
        return QSizeF(mm2pixel_ceil(sizeMM.width(), dpi), mm2pixel_ceil(sizeMM.height(), dpi));
    default:
        return QSizeF();
    }

    return size;
}


qreal convertUnit(const qreal & value, Unit unitIn, Unit unitOut, int dpi)
{
    if (unitIn == unitOut)
        return value;

    qreal valueMM;
    // first: convert to mm
    switch (unitIn) {
    case UnitNotDefined:
    case Millimeter:
        valueMM = value; break;
    case Inch:
        valueMM = inch2mm(value); break;
    case Pixel:
        valueMM = pixel2mm(value, dpi); break;
    default:
        valueMM = value;
    }

    // second: convert to output meassure
    switch (unitOut) {
    case UnitNotDefined:
    case Millimeter:
        return valueMM;
    case Inch:
        return mm2inch(valueMM);
    case Pixel:
        return mm2pixel_ceil(valueMM, dpi);
    default:
        return 0;
    }

    return value;
}



CuteReport::Unit unitFromString(const QString & str)
{
    QString strLower = str.toLower();
    for (int i = 0; i<CuteReport::UNIT_MAX; ++i) {
        if (QString(CuteReport::unitStr[i]).toLower() == strLower)
            return CuteReport::Unit(i);
        if (QString(CuteReport::unitShortStr[i]).toLower() == strLower)
            return CuteReport::Unit(i);
    }
    return CuteReport::UnitNotDefined;
}


QString unitToFullString(CuteReport::Unit unit)
{
    return CuteReport::unitStr[unit];
}


QString unitToShortString(CuteReport::Unit unit)
{
    return CuteReport::unitShortStr[unit];
}


QSet<QString> findVariables(const QString & str)
{
    QSet<QString> set;

    QRegExp rx(variablesRegExp);
    rx.setMinimal(true);
    int pos = 0;
    while (pos >= 0) {
        pos = rx.indexIn(str, pos);
        if (pos != -1) {
            set.insert( rx.cap(variablesRegExpNum) );
            pos += rx.cap(0).length();
        }
    }

    return set;
}


QString setVariablesValue(const QString & str, const QVariantHash & values)
{
    QString strOut = str;
    QRegExp rx(variablesRegExp);
    int pos = 0;
    while (pos >= 0) {
        pos = rx.indexIn(strOut, pos);
        if (pos != -1) {
            QString strOrig = rx.cap(0);
            QString varName = rx.cap(variablesRegExpNum);
            QVariant varValueVariant = values.value(varName);
            QString varValue;
            switch (varValueVariant.type()) {
                case QVariant::String: varValue = values.value(varName).toString(); break;
                default: varValue = "NonString";
            }
            strOut.replace(QString("${%1}").arg(varName), varValue);
            pos += strOrig.length();
        }
    }
    return strOut;
}


bool isStringValued(const QString & str, const QVariantHash & values, QStringList * missedVariables)
{
    QSet<QString> variables =  findVariables(str);
    bool result = true;

    foreach (const QString & variable, variables)
        if (!values.contains(variable) || values.value(variable).toString().isEmpty()) {
            if (missedVariables) {
                result = false;
                missedVariables->append(variable);
            }
            else
                return false;
        }
    return result;
}


}// namespace
