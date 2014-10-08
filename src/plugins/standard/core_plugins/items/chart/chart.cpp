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

#include "chart.h"
#include "chart_p.h"
#include "pageinterface.h"
#include "rendererpublicinterface.h"
#include "datasetinterface.h"
#include "reportinterface.h"
#include "charthistogram.h"
#include <QtCore>
#include <QtGui>

#define RADIANS_TO_DEGREES 57.2957795

using namespace CuteReport;

inline void initResource() { Q_INIT_RESOURCE(chart); }

ChartItem::ChartItem(QObject *parent)
    :ItemInterface(new ChartItemPrivate, parent),
      m_renderer(0)
{
    Q_INIT_RESOURCE(chart);
    Q_D(ChartItem);
    setResizeFlags( ResizeLeft | ResizeRight | ResizeTop | ResizeBottom);

    d->font.setPointSize(8);
    d->axisColor = QColor(Qt::green);
    d->drawAxis = AxisX | AxisY | AxisZ;
    d->drawValues = ValueX | ValueY | ValueZ | ValueInline;
    d->rotateValueX = d->rotateValueY = d->rotateValueInline = AutoRotate;

    setLegendSize(40);
    setGeometry(QRectF(0,0,80,60));  // process legend
}


void ChartItem::moduleInit()
{
    initResource();
}


ChartItem::ChartItem(ChartItemPrivate *dd, QObject * parent)
    :ItemInterface(dd, parent)
{
    Q_INIT_RESOURCE(chart);
}


BaseItemInterface *ChartItem::itemClone() const
{
    Q_D(const ChartItem);
    return new ChartItem(new ChartItemPrivate(*d), parent());
}


QByteArray ChartItem::serialize()
{
    Q_D(ChartItem);
    QByteArray ba;
    QDataStream s( &ba, QIODevice::ReadWrite );
    s << *d;
    return ba;
}


void ChartItem::deserialize(QByteArray &data)
{
    Q_D(ChartItem);
    QDataStream s( &data, QIODevice::ReadWrite );
    s >> *d;
}


QDataStream &operator<<(QDataStream &s, const ChartItemPrivate &p) {
    s << static_cast<const ItemInterfacePrivate&>(p);
    s << (qint8)p.chartType << (qint8)p.legendType  << p.legendSize << p.colors << p.zFields << p.xField << p.dataset
      << p.font << p.textColor << p.axisColor << (qint8)p.drawAxis << (qint8)p.drawValues
      << (qint8)p.rotateValueX << (qint8)p.rotateValueY << (qint8)p.rotateValueInline
      << p.xMarginFactor << p.zMarginFactor
      << p.xValues << p.zValues << p.yValues << p.zColors;

    switch (p.chartType) {
    case ChartItem::Histogram:
        s << (*reinterpret_cast<HistogramStruct*>(p.r));
        break;
    }

    return s;
}


QDataStream &operator>>(QDataStream &s, ChartItemPrivate &p) {
    s >> static_cast<ItemInterfacePrivate&>(p);
    qint8  chartType, legendType, drawAxis, drawValues, rotateValueX, rotateValueY, rotateValueInline ;

    s >> chartType; s >> legendType; s >> p.legendSize; s >> p.colors; s >> p.zFields; s >> p.xField; s >> p.dataset;
    s >> p.font; s >> p.textColor; s >> p.axisColor; s >> drawAxis; s >> drawValues;
    s >> rotateValueX; s >> rotateValueY; s >> rotateValueInline;
    s >> p.xMarginFactor; s >> p.zMarginFactor;
    s >> p.xValues; s >> p.zValues; s >> p.yValues; s >> p.zColors;

    switch (chartType) {
    case ChartItem::Histogram:
        delete p.r;
        p.r = new HistogramStruct;
        s >> (*reinterpret_cast<HistogramStruct*>(p.r));
        break;
    }

    p.chartType = (ChartItem::ChartType)chartType;
    p.legendType = (ChartItem::LegendTypes)legendType;
    p.drawAxis = (ChartItem::Axis)drawAxis;
    p.drawValues = (ChartItem::Values)drawValues;
    p.rotateValueX = (ChartItem::ValueRotation)rotateValueX;
    p.rotateValueY = (ChartItem::ValueRotation)rotateValueY;
    p.rotateValueInline = (ChartItem::ValueRotation)rotateValueInline;

    return s;
}


CuteReport::BaseItemInterface * ChartItem::createInstance(QObject * parent) const
{
    return new ChartItem(parent);
}


BaseItemHelperInterface *ChartItem::helper()
{
    return 0;
}


void ChartItem::init_gui()
{
    Q_D(ChartItem);
    if (m_gui)
        return;
    prepareData(d, reportObject(), absoluteGeometry());
    m_gui = new ChartItemView(this);
}


bool ChartItem::canContain(QObject * object)
{
    return (dynamic_cast<CuteReport::BaseItemInterface*>(object));
}


QIcon ChartItem::itemIcon() const
{
    return QIcon(":/chart.png");
}


QString ChartItem::moduleShortName() const
{
    return tr("Chart");
}


QString ChartItem::itemGroup() const
{
    return QString("Shapes");
}


void ChartItem::renderInit(RendererPublicInterface * renderer)
{
    m_renderer = renderer;
}


void ChartItem::renderReset()
{
    m_renderer = 0;
}


bool ChartItem::renderPrepare()
{
    emit printBefore();
    setRenderingPointer(new ChartItemPrivate(*(reinterpret_cast<ChartItemPrivate*>(d_ptr))));
    emit printDataBefore();
    emit printDataAfter();
    return true;
}


RenderedItemInterface *ChartItem::renderView()
{
    Q_D(ChartItem);
    RenderedChartItem * view = new RenderedChartItem(this, new ChartItemPrivate(*d));
    return view;
}


void ChartItem::update_gui()
{
    Q_D(ChartItem);
    prepareData(d, reportObject(), absoluteGeometry());
    ItemInterface::update_gui();
}


void ChartItem::paint(QPainter * painter, const QStyleOptionGraphicsItem *option, const BaseItemInterfacePrivate * data, const QRectF &boundingRect, RenderingType type)
{
    ItemInterface::paintBegin(painter, option, data, boundingRect, type);

    const ChartItemPrivate * const d = static_cast<const ChartItemPrivate*>(data);

    switch (d->chartType) {
    case Histogram:
        ChartHistogram::paint(painter, data, type);
    }

    ItemInterface::paintEnd(painter, option, data, boundingRect, type);
}


void ChartItem::prepareData(ChartItemPrivate * d, CuteReport::ReportInterface * report, const QRectF absoluteGeometry)
{
    switch (d->chartType) {
    case Histogram:
        ChartHistogram::prepareData(d, report, absoluteGeometry);
    }

}


QColor ChartItem::randomColor()
{
    int r = rand() % 256;
    int g = rand() % 256;
    int b = rand() % 256;
    return QColor(r,g,b);
}


QStringList ChartItem::scriptingStrings()
{
    Q_D(ChartItem);
    return QStringList() << d->xField << d->xValues << d->zValues;
}


ChartItem::ChartType ChartItem::chartType() const
{
    Q_D(const ChartItem);
    return d->chartType;
}


void ChartItem::setChartType(ChartType chartType)
{
    Q_D(ChartItem);
    if (chartType == d->chartType)
        return;
    d->chartType = chartType;

    delete d->r;

    switch (d->chartType) {
    case Histogram:
        d->r = new HistogramStruct;
        break;
    }

    update_gui();
    emit chartTypeChanged(d->chartType);
    emit changed();
}


ChartItem::LegendTypes ChartItem::legendType() const
{
    Q_D(const ChartItem);
    return d->legendType;
}


void ChartItem::setLegendType(ChartItem::LegendTypes legendType)
{
    Q_D(ChartItem);
    if (legendType == d->legendType)
        return;
    d->legendType = legendType;
    update_gui();
    emit legendTypeChanged(d->legendType);
    emit changed();
}


//QPointF ChartItem::legendOffset() const
//{
//    Q_D(const ChartItem);
//    return d->legendOffset;
//}


//void ChartItem::setLegendOffset(QPointF offset)
//{
//    Q_D(ChartItem);
//    if (offset == d->legendOffset)
//        return;
//    d->legendOffset = offset;
//    update_gui();
//    emit legendOffsetChanged(d->legendOffset);
//}


qreal ChartItem::legendSize() const
{
    Q_D(const ChartItem);
    return d->legendSize;
}


void ChartItem::setLegendSize(qreal size)
{
    Q_D(ChartItem);
    if (size == d->legendSize)
        return;
    d->legendSize = size;
    update_gui();
    emit legendSizeChanged(d->legendSize);
    emit changed();
}


//ChartItem::LegendAnchor ChartItem::legendAnchor() const
//{
//    Q_D(const ChartItem);
//    return d->legendAnchor;
//}


//void ChartItem::setLegendAnchor(LegendAnchor anchor)
//{
//    Q_D(ChartItem);
//    if (anchor == d->legendAnchor)
//        return;
//    d->legendAnchor = anchor;
//    update_gui();
//    emit legendAnchorChanged(d->legendAnchor);
//}


QStringList ChartItem::colors() const
{
    Q_D(const ChartItem);
    return d->colors;
}


void ChartItem::setColors(const QStringList &colors)
{
    Q_D(ChartItem);
    if (d->colors == colors)
        return;
    d->colors = colors;
    update_gui();
    emit colorsChanged(d->colors);
    emit changed();
}


QStringList ChartItem::zFields() const
{
    Q_D(const ChartItem);
    return d->zFields;
}


void ChartItem::setzFields(const QStringList & values)
{
    Q_D(ChartItem);
    if (d->zFields == values)
        return;
    d->zFields = values;
    update_gui();
    emit zFieldsChanged(d->zFields);
    emit changed();
}


QString ChartItem::xField() const
{
    Q_D(const ChartItem);
    return d->xField;
}


void ChartItem::setxField(const QString & field)
{
    Q_D(ChartItem);
    if (d->xField == field)
        return;
    d->xField = field;
    update_gui();
    emit xFieldChanged(d->xField);
    emit changed();
}


QFont ChartItem::font() const
{
    Q_D(const ChartItem);
    return d->font;
}


void ChartItem::setFont(const QFont & font)
{
    Q_D(ChartItem);
    if (d->font != font) {
        d->font = font;
        update_gui();
        emit fontChanged(d->font);
        emit changed();
    }
}


QColor ChartItem::textColor() const
{
    Q_D(const ChartItem);
    return d->textColor;
}


void ChartItem::setTextColor(const QColor & color)
{
    Q_D(ChartItem);
    if (d->textColor != color) {
        d->textColor = color;
        update_gui();
        emit textColorChaged(d->textColor);
        emit changed();
    }
}


ChartItem::Axis ChartItem::drawAxis() const
{
    Q_D(const ChartItem);
    return d->drawAxis;
}


void ChartItem::setDrawAxis(Axis axis)
{
    Q_D(ChartItem);
    if (d->drawAxis == axis)
        return;

    d->drawAxis = axis;
    update_gui();
    emit drawAxisChanged(d->drawAxis);
    emit changed();
}


ChartItem::Values ChartItem::drawValues() const
{
    Q_D(const ChartItem);
    return d->drawValues;
}


void ChartItem::setDrawValues(Values values)
{
    Q_D(ChartItem);
    if (d->drawValues == values)
        return;

    d->drawValues = values;
    update_gui();
    emit drawValuesChanged(d->drawValues);
    emit changed();
}


ChartItem::ValueRotation ChartItem::valueRotationX() const
{
    Q_D(const ChartItem);
    return d->rotateValueX;
}


void ChartItem::setValueRotationX(const ChartItem::ValueRotation & value)
{
    Q_D(ChartItem);
    if (d->rotateValueX == value)
        return;
    d->rotateValueX = value;
    update_gui();
    emit valueRotationXChanged(d->rotateValueX);
    emit changed();
}


ChartItem::ValueRotation ChartItem::valueRotationY() const
{
    Q_D(const ChartItem);
    return d->rotateValueY;
}


void ChartItem::setValueRotationY(const ChartItem::ValueRotation & value)
{
    Q_D(ChartItem);
    if (d->rotateValueY == value)
        return;
    d->rotateValueY = value;
    update_gui();
    emit valueRotationYChanged(d->rotateValueY);
    emit changed();
}


ChartItem::ValueRotation ChartItem::valueRotationInline() const
{
    Q_D(const ChartItem);
    return d->rotateValueInline;
}

void ChartItem::setValueRotationInline(const ChartItem::ValueRotation & value)
{
    Q_D(ChartItem);
    if (d->rotateValueInline == value)
        return;
    d->rotateValueInline = value;
    update_gui();
    emit valueRotationInlineChanged(d->rotateValueInline);
    emit changed();
}


qreal ChartItem::xMarginFactor() const
{
    Q_D(const ChartItem);
    return d->xMarginFactor;
}


void ChartItem::setXMarginFactor(qreal value)
{
    Q_D(ChartItem);
    if (d->xMarginFactor == value)
        return;
    d->xMarginFactor = value;

    if (d->xMarginFactor <0)
        d->xMarginFactor = 0;

    update_gui();
    emit xMarginFactorChanged(d->xMarginFactor);
    emit changed();
}


qreal ChartItem::zMarginFactor() const
{
    Q_D(const ChartItem);
    return d->zMarginFactor;
}


void ChartItem::setZMarginFactor(qreal value)
{
    Q_D(ChartItem);
    if (d->zMarginFactor == value)
        return;
    d->zMarginFactor = value;

    if (d->zMarginFactor < -1)
        d->zMarginFactor = -1;

    update_gui();
    emit zMarginFactorChanged(d->zMarginFactor);
    emit changed();
}



QString ChartItem::dataset() const
{
    Q_D(const ChartItem);
    return d->dataset;
}


void ChartItem::setDataset(const QString & dataset)
{
    Q_D(ChartItem);
    if (d->dataset == dataset)
        return;

    d->dataset = dataset;
    update_gui();
    emit datasetChanged(d->dataset);
    emit changed();
}


QString ChartItem::_current_property_description() const
{
    QString propertyName = metaObject()->property(m_currentProperty).name();

    if (propertyName == "chartType")
        return tr("Chart type");
    else if (propertyName == "dataset")
        return tr("Dataset name where chart takes data");
    else if (propertyName == "xField")
        return tr("Dataset field where Chart takes data for draw X axis data");
    else if (propertyName == "zFields")
        return tr("Dataset field list where Chart takes data for draw Z axis data");
    else if (propertyName == "colors")
        return tr("Colors list for chart pieces");
    else if (propertyName == "textColor")
        return tr("Colors for all chart text");
    else if (propertyName == "font")
        return tr("Font for all chart text");
    else if (propertyName == "legendType")
        return tr("Value shows how to draw Legend");
    else if (propertyName == "legendSize")
        return tr("Legend size is enabled");
    else if (propertyName == "drawAxis")
        return tr("Which axis need to be drown");
    else if (propertyName == "drawValues")
        return tr("Which values need to be drown");
    else if (propertyName == "XValueRotation")
        return tr("Type of rotation for X axis values");
    else if (propertyName == "YValueRotation")
        return tr("Type of rotation for Y axis values");
    else if (propertyName == "InlineValueRotation")
        return tr("Type of rotation for inline values");
    else if (propertyName == "xMarginFactor")
        return tr("Factor for calculate space beetween values in X exis");
    else if (propertyName == "zMarginFactor")
        return tr("Factor for calculate space beetween values in Z exis");
    else
        return ItemInterface::_current_property_description();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Chart, ChartItem)
#endif
