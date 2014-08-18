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

#include "charthistogram.h"
#include "chart_p.h"
#include "datasetinterface.h"
#include "reportinterface.h"
#include "functions.h"

#define RADIANS_TO_DEGREES 57.2957795

using namespace CuteReport;

ChartHistogram::ChartHistogram()
{
}


void ChartHistogram::paint(QPainter * painter, const BaseItemInterfacePrivate * data, RenderingType type)
{
    const ChartItemPrivate * const d = static_cast<const ChartItemPrivate*>(data);
    HistogramStruct * r = reinterpret_cast<HistogramStruct*>(d->r);


    painter->setPen(d->borderPen);

    if (type == RenderingReport) {
//        qDebug() << "test";
    }

    QPen pen = painter->pen();
    pen.setColor(Qt::red);
    painter->setPen(pen);

    painter->drawRect(r->chartAreaPaintRect);

    if (r->error) {
        painter->drawText(r->chartAreaPaintRect, Qt::AlignCenter, r->errorText);
        return;
    }

    painter->setFont(r->textFont);
    QPen textPen = QPen(d->textColor);
    QPen piecesPen = QPen(Qt::black);

    qreal deltaX = 0;
    qreal groupDeltaX = 0;

    for (int i=0; i< d->yValues.size(); ++i) {

        int xIndex = i / r->zArrayCount;
        int zIndex = i - (i / r->zArrayCount) * r->zArrayCount;

        qreal yValue = d->yValues[i];
        //        QString xValue = d->xValues.at(xIndex);
        //        QString zValue = d->zValues.at(zIndex);

        painter->setPen(piecesPen);
        painter->setBrush(QBrush( zIndex < d->zColors.size() ? d->zColors.at(zIndex) : QColor(Qt::white)));

        QRectF pieceRect = QRectF(r->chartRect.x() + deltaX, r->chartRect.bottom() - (yValue - r->piecesMin)*r->yPixelFactor,
                                  r->pieceW, yValue * r->yPixelFactor);
        painter->drawRect(pieceRect);


        if (d->drawValues) {

            if (d->drawValues & ChartItem::ValueInline) {
                if (r->inlineValueRotate) {
                    painter->save();
                    painter->translate( r->chartRect.x() + deltaX, r->chartRect.bottom() -
                                        ((yValue > 0) ? (yValue - r->piecesMin)*r->yPixelFactor  : -r->piecesMin*r->yPixelFactor)) ;
                    painter->rotate(-90);
                    QRectF textRect = QRectF(0, 0, r->maxValueWidth, r->pieceW);
                    painter->drawText(textRect, Qt::AlignCenter, QString::number(yValue));
                    painter->restore();
                } else {
                    QRectF textRect = QRectF(r->chartRect.x() + deltaX,
                                             r->chartRect.bottom() - ((yValue > 0) ? (yValue - r->piecesMin)*r->yPixelFactor : -r->piecesMin*r->yPixelFactor)- r->fontHeight,
                                             r->pieceW, r->fontHeight);
                    painter->setPen(textPen);
                    painter->drawText(textRect, Qt::AlignCenter, QString::number(yValue));
                }
            }

            if ((d->drawValues & ChartItem::ValueX) && !((i+1) % r->zArrayCount )) {
                if (r->titleAngle > 0) {
                    painter->setPen(textPen);
                    painter->save();
                    painter->translate(r->chartAreaPaintRect.x() + groupDeltaX, r->chartAreaPaintRect.bottom() - r->fontHeight * cos(r->titleAngle/RADIANS_TO_DEGREES) );
                    painter->rotate(-r->titleAngle);
                    QRectF titleRect = QRectF(0, 0, r->maxTitleWidth, r->fontHeight);
                    painter->drawText(titleRect, Qt::AlignVCenter | Qt::AlignRight, d->xValues.at(xIndex));
                    painter->restore();
                } else {
                    QRectF titleRect = QRectF(r->chartRect.x() + groupDeltaX, r->chartRect.bottom() + r->fontHeight * 0.5,
                                              r->pieceW * r->zArrayCount + r->pieceW * (r->zArrayCount-1) * d->xMarginFactor, r->fontHeight);
                    painter->setPen(textPen);
                    painter->drawText(titleRect, Qt::AlignCenter, d->xValues.at(xIndex));
                }
                groupDeltaX = deltaX + r->pieceW * (1 + d->xMarginFactor);
            }

        }

        deltaX += r->pieceW + r->pieceW * (( r->zArrayCount > 1 && (i+1) % r->zArrayCount) ? d->zMarginFactor : d->xMarginFactor) ;
    }

    // draw axis
    if (d->drawAxis) {
        QPen axisPen = QPen(d->axisColor);
        axisPen.setWidthF(3);
        painter->setPen(axisPen);

        if (d->drawAxis & ChartItem::AxisY) {
            painter->drawPolyline(r->axisY);
        }

        if ((d->drawAxis & ChartItem::AxisX)) {
            painter->drawPolyline(r->axisX);
        }
    }

    if (d->drawValues & ChartItem::ValueY) {
        painter->setPen(textPen);
        int digits = 0;
        int value = qMax(r->piecesMax, qAbs(r->piecesMin) );
        while ((value = value/10))
            ++digits;

        int factor = pow(10.0,digits);
        value = r->piecesMin < 0 ? r->piecesMin/factor : 0;
        int zeroDelta = r->piecesMin < 0 ? -r->piecesMin * r->yPixelFactor : 0;
        int y = r->chartRect.bottom() - value * factor * r->yPixelFactor - zeroDelta;
        ;
        while (y >= r->chartAreaPaintRect.top() + r->fontHeight) {
            painter->drawLine(r->chartRect.left() - r->fontHeight/4, y,
                              r->chartRect.left() + r->fontHeight/4, y);

            QRectF titleRect = QRectF(r->chartRect.x() - r->maxValueWidth,  y - r->fontHeight,
                                      r->maxValueWidth, r->fontHeight);
            painter->drawText(titleRect, Qt::AlignCenter, QString::number(value*factor));

            ++value;
            y = r->chartRect.bottom() - value * factor * r->yPixelFactor - zeroDelta;
        }

    }

}


void ChartHistogram::prepareData(ChartItemPrivate * d, CuteReport::ReportInterface * report, QRectF absoluteGeometry)
{
    QRectF absoluteGeometryMM = convertUnit(absoluteGeometry, d->unit, Millimeter, d->dpi);
    HistogramStruct * r = reinterpret_cast<HistogramStruct*>(d->r);

    d->xValues.clear();
    d->yValues.clear();
    d->zValues.clear();

    r->error = false;
    r->piecesMax = 0;
    r->piecesMin = 0;
    r->piecesSum = 0;
    int longestXIndex = -1;
    int biggestXLength = 0;
    r->zArrayCount = d->zFields.size();
    QFontMetricsF fontMetrix(ItemInterface::fontPrepared(d->font, d->dpi));
    int counter = -1;


    switch (d->legendType) {
    case ChartItem::LegendDisabled: r->chartArea = QRectF(0, 0, d->rect.width(), d->rect.height());
        break;
    case ChartItem::LegendRight: r->chartArea = QRectF(0, 0, d->rect.width() - d->legendSize, d->rect.height());
        break;
    case ChartItem::LegendBottom: r->chartArea = QRectF(0, 0, d->rect.width(), d->rect.height() - d->legendSize);
        break;
    }

    r->absPaintRect = convertUnit( absoluteGeometryMM, Millimeter, Pixel, d->dpi).toRect();
    r->chartAreaPaintRect = convertUnit(r->chartArea.translated(absoluteGeometryMM.topLeft()), Millimeter, Pixel, d->dpi).toRect();
    r->chartAreaPaintRect = QRect(r->chartAreaPaintRect.topLeft() - r->absPaintRect.topLeft() , QSize(r->chartAreaPaintRect.width(), r->chartAreaPaintRect.height() ));

    if (d->dataset.isEmpty()) {
        r->error = true;
        r->errorText = "Dataset name is not entered";
        return;
    }

    if (!report)
        return;

    CuteReport::DatasetInterface * dataset = report->dataset(d->dataset);
    if (!dataset)
        return;


    if (!dataset->isPopulated())
        dataset->populate();

    dataset->firstRow();

    /** currently: dataset line per xValue **/
    /** TODO: dataset line per zValue **/

    do {

        counter++;
//        int xIndex = counter / r->zArrayCount;
//        int zIndex = counter - (counter / r->zArrayCount) * r->zArrayCount;

//        if (zIndex == 0) {
            QVariant value = d->xField;
            d->xValues.append((value.type() == QVariant::Int) ? dataset->value(value.toInt()).toString() : dataset->value(value.toString()).toString());

            int length = fontMetrix.width(d->xValues.last());
            if (length > biggestXLength) {
                biggestXLength = length;
                longestXIndex = d->xValues.size()-1;
            }
//        }

        for (int i = 0; i<d->zFields.count(); ++i) {
            QVariant value = d->zFields[i];
            d->yValues.append((value.type() == QVariant::Int) ? dataset->value(value.toInt()).toReal() : dataset->value(value.toString()).toReal());

            if (d->zColors.size() < d->zFields.size()) {
                d->zColors.append( ChartItem::randomColor() );
                // TODO: use script
            }

            if (d->yValues.last() < r->piecesMin)
                r->piecesMin = d->yValues.last();
            if (d->yValues.last() > r->piecesMax)
                r->piecesMax = d->yValues.last();
            r->piecesSum += d->yValues.last();

        }

    } while (dataset->nextRow());

    r->xArrayCount = d->xValues.size();

    r->chartRect = r->chartAreaPaintRect;

    r->textFont = ItemInterface::fontPrepared(d->font, d->dpi);


    r->fontHeight = fontMetrix.height();
    r->titleAngle = 0;
    r->titleOffset = 0;
    r->maxTitleWidth = 0;
    r->maxValueWidth = 0;
    r->inlineValueRotate = false;

    if (d->drawValues)
        r->maxValueWidth = fontMetrix.width(QString::number(r->piecesMax) + "X");

    // HORISONTAL SPACE REARRANGING

    // draw axis
    if (d->drawAxis) {
        if (d->drawAxis & ChartItem::AxisY) {
            r->chartRect.setLeft(r->chartRect.left() + r->fontHeight/2);
            r->chartRect.setTop(r->chartRect.top() + r->fontHeight);
        }

        if (d->drawAxis & ChartItem::AxisX) {
            r->chartRect.setRight(r->chartRect.right() - r->fontHeight);
            r->chartRect.setBottom(r->chartRect.bottom() - r->fontHeight/2);
        }
    }


    if (d->drawValues & ChartItem::ValueY)
        r->chartRect.setLeft(r->chartRect.left() +  r->maxValueWidth);


    r->pieceW =  r->chartRect.width() / (r->xArrayCount*r->zArrayCount + (r->zArrayCount -1) * r->xArrayCount * d->zMarginFactor + (r->xArrayCount -1) * d->xMarginFactor);

    // VERTIACAL SPACE REARRANGING

    if (d->drawValues & ChartItem::ValueInline) {
        if ( (d->rotateValueInline == ChartItem::YesRotate) || ( (d->rotateValueInline == ChartItem::AutoRotate) && r->maxValueWidth > r->pieceW) ) {
            r->inlineValueRotate = true;
            r->chartRect.setTop(r->chartRect.top() + r->maxValueWidth - (d->drawAxis & ChartItem::AxisY ? r->fontHeight : 0) );
        }
    }


    if (d->drawValues & ChartItem::ValueX && longestXIndex >= 0) {
        r->maxTitleWidth = fontMetrix.width(d->xValues[longestXIndex]);
        qreal x;
        qreal areaWidth = r->pieceW * r->zArrayCount + r->pieceW * (r->zArrayCount-1) * d->zMarginFactor;

        if ((d->rotateValueX == ChartItem::YesRotate) || ( (d->rotateValueX == ChartItem::AutoRotate) && r->maxTitleWidth > areaWidth )) {
            r->titleOffset = qSqrt (r->fontHeight*r->fontHeight / 2);
            x = r->chartRect.x() + areaWidth - r->titleOffset;
            qreal value = x / r->maxTitleWidth;
            if (value >=-1 && value <=1)
                r->titleAngle = acos (x / r->maxTitleWidth) * RADIANS_TO_DEGREES;
        }

        if (r->titleAngle >0 )
            r->chartRect.setBottom(r->chartRect.bottom() - qSqrt(r->maxTitleWidth*r->maxTitleWidth - x*x) - r->fontHeight/2 );
        else
            r->chartRect.setBottom(r->chartRect.bottom() - r->fontHeight);
    }

    r->yPixelFactor = (r->chartRect.height() - (d->drawValues & ChartItem::ValueX ? r->fontHeight: 0) ) /
            ((r->piecesMax- r->piecesMin) > 0 ? (r->piecesMax - r->piecesMin) : 100);


    if (d->drawAxis & ChartItem::AxisY) {
        r->axisY.clear();
        r->axisY << r->chartRect.bottomLeft() << QPoint(r->chartRect.left(), r->chartAreaPaintRect.top() + r->fontHeight/2);
    }

    if ((d->drawAxis & ChartItem::AxisX)) {
        int y = r->chartRect.bottom() - ((r->piecesMin < 0) ? -r->piecesMin * r->yPixelFactor : 0);
        r->axisX.clear();
        r->axisX << QPoint(r->chartRect.left(), y) << QPoint(r->chartRect.right() + r->fontHeight/2, y);
    }


    if (r->yPixelFactor <=0) {
        r->error = true;
        r->errorText = "Too small area!";
    }

    if (d->xField.isEmpty()) {
        r->error = true;
        r->errorText = "Uncorrect zField value";
    }

    if (d->zFields.isEmpty()) {
        r->error = true;
        r->errorText = "Uncorrect zField value";
    }


}

QDataStream &operator<<(QDataStream &s, const HistogramStruct &p)
{
    s << p.chartArea << p.piecesMax << p.piecesMin << p.piecesSum << p.chartAreaPaintRect << p.chartAreaPaintRect << p.absPaintRect
         << p.textFont << p.chartRect << p.axisX << p.axisY << p.zArrayCount << p.xArrayCount << p.fontHeight << p.titleAngle << p.titleAngle
            << p.maxTitleWidth << p.maxTitleWidth << p.inlineValueRotate << p.pieceW << p.yPixelFactor << p.error << p.errorText;
    return s;
}


QDataStream &operator>>(QDataStream &s, HistogramStruct &p)
{
    s >> p.chartArea; s >> p.piecesMax; s >> p.piecesMin; s >> p.piecesSum; s >> p.chartAreaPaintRect; s >> p.absPaintRect; s >> p.textFont;
    s >> p.chartRect; s >> p.axisX; s >> p.axisY; s >> p.zArrayCount; s >> p.xArrayCount; s >> p.fontHeight; s >> p.titleAngle; s >> p.titleOffset;
    s >> p.maxTitleWidth; s >> p.maxValueWidth; s >> p.inlineValueRotate; s >> p.pieceW; s >> p.yPixelFactor; s >> p.error; s >> p.errorText;

    return s;
}
