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

#ifndef CHART_P_H
#define CHART_P_H

#include "iteminterface_p.h"
#include "iteminterface.h"
#include "chart.h"

struct Piece {
    qreal value;
    QString title;
};


class   ChartRenderingStruct: public CuteReport::RenderingStruct
{
public:
    ChartRenderingStruct():
        renderer(0)
    {}

    ChartRenderingStruct(ChartRenderingStruct & p) :
        RenderingStruct(p),
        renderer(p.renderer)
    {}

    virtual ~ChartRenderingStruct(){}

    CuteReport::RendererPublicInterface * renderer;
};


class   HistogramStruct: public CuteReport::RenderingStruct
{
public:
    explicit HistogramStruct()
    :RenderingStruct(),
      piecesMax(0),
      piecesMin(0),
      piecesSum(0)
    {}

    HistogramStruct(const HistogramStruct & p) :
        RenderingStruct(p),
        chartArea(p.chartArea),
        piecesMax(p.piecesMax),
        piecesMin(p.piecesMin),
        piecesSum(p.piecesSum),
//        longestTitle(p.longestTitle),
        chartAreaPaintRect(p.chartAreaPaintRect),
        absPaintRect(p.absPaintRect),
        textFont(p.textFont),
        chartRect(p.chartRect),
        axisX(p.axisX),
        axisY(p.axisY),
        zArrayCount(p.zArrayCount),
        xArrayCount(p.xArrayCount),
        fontHeight(p.fontHeight),
        titleAngle(titleAngle),
        titleOffset(p.titleOffset),
        maxTitleWidth(p.maxTitleWidth),
        maxValueWidth(p.maxValueWidth),
        inlineValueRotate(p.inlineValueRotate),
        pieceW(p.pieceW),
        yPixelFactor(p.yPixelFactor)
    {}

    virtual ~HistogramStruct(){}

    QRectF chartArea;
    qreal piecesMax;
    qreal piecesMin;
    qreal piecesSum;
    QRect chartAreaPaintRect;
    QRect absPaintRect;
    QFont textFont;
    QRect chartRect;
    QPolygon axisX, axisY;
    int zArrayCount;
    int xArrayCount;
    qreal fontHeight;
    qreal titleAngle;
    qreal titleOffset;
    int maxTitleWidth;
    int maxValueWidth;
    bool inlineValueRotate;
    qreal pieceW;
    qreal yPixelFactor;
    bool error;
    QString errorText;
};

QDataStream &operator<<(QDataStream &s, const HistogramStruct &p);
QDataStream &operator>>(QDataStream &s, HistogramStruct &p);



class ChartItemPrivate : public CuteReport::ItemInterfacePrivate
{
public:
    ChartItemPrivate()
        :ItemInterfacePrivate(),
          chartType(ChartItem::Histogram),
          legendType(ChartItem::LegendDisabled),
          xMarginFactor(0.0),
          zMarginFactor(0.4)
    {
        r = new HistogramStruct;
    }

    ChartItemPrivate(const ChartItemPrivate & p)
        :ItemInterfacePrivate(p),
          chartType(p.chartType),
          legendType(p.legendType),
          legendSize(p.legendSize),
          colors(p.colors),
          zFields(p.zFields),
          xField(p.xField),
          dataset(p.dataset),
          font(p.font),
          textColor(p.textColor),
          axisColor(p.axisColor),
          drawAxis(p.drawAxis),
          drawValues(p.drawValues),
          rotateValueX(p.rotateValueX),
          rotateValueY(p.rotateValueY),
          rotateValueInline(p.rotateValueInline),
          xMarginFactor(p.xMarginFactor),
          zMarginFactor(p.zMarginFactor),
          xValues(p.xValues),
          zValues(p.zValues),
          yValues(p.yValues),
          zColors(p.zColors)
    {
        if (p.r) {
            switch (chartType) {
            case ChartItem::Histogram:
                r = new HistogramStruct(*reinterpret_cast<HistogramStruct*>(p.r));
                break;
            }
        } else
            r = 0;
    }

    virtual ~ChartItemPrivate()
    {
        delete r;
        r = 0;
    }

    ChartItem::ChartType chartType;
    ChartItem::LegendTypes legendType;
    qreal legendSize;
    QStringList colors;
    QStringList zFields;
    QString xField;
    QString dataset;
    QFont font;
    QColor textColor;
    QColor axisColor;
    ChartItem::Axis drawAxis;
    ChartItem::Values drawValues;
    ChartItem::ValueRotation rotateValueX, rotateValueY, rotateValueInline;
    qreal xMarginFactor;
    qreal zMarginFactor;

    QStringList xValues;
    QStringList zValues;
    QList<qreal> yValues;
    QList<QColor> zColors;
};


QDataStream &operator<<(QDataStream &s, const ChartItemPrivate &p);
QDataStream &operator>>(QDataStream &s, ChartItemPrivate &p);

#endif // CHART_P_H
