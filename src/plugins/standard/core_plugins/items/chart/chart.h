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

#ifndef CHART_H
#define CHART_H

#include <iteminterface.h>
#include "iteminterfaceview.h"
#include "renderediteminterface.h"
#include "globals.h"


class ChartItemPrivate;

namespace CuteReport {
class ReportInterface;
}

class  ChartItem : public CuteReport::ItemInterface
{
	Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteReport.ItemInterface/1.0")
#endif
    Q_INTERFACES(CuteReport::ItemInterface)

    Q_ENUMS(ChartType LegendTypes ValueRotation)
    Q_FLAGS(Axis AxisFlag)
    Q_FLAGS(Values ValueFlag)

    // Legend Axis : X, Z

    Q_PROPERTY(ChartType chartType READ chartType WRITE setChartType NOTIFY chartTypeChanged)
    Q_PROPERTY(QString dataset READ dataset WRITE setDataset NOTIFY datasetChanged)
    Q_PROPERTY(QString xField READ xField WRITE setxField NOTIFY xFieldChanged) // => xFields
    Q_PROPERTY(QStringList zFields READ zFields WRITE setzFields NOTIFY zFieldsChanged) // =>zFields
    Q_PROPERTY(QStringList colors READ colors WRITE setColors NOTIFY colorsChanged)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY textColorChaged)
    Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY fontChanged)
    Q_PROPERTY(LegendTypes legendType READ legendType WRITE setLegendType NOTIFY legendTypeChanged)
    Q_PROPERTY(qreal legendSize READ legendSize WRITE setLegendSize NOTIFY legendSizeChanged)
    Q_PROPERTY(Axis drawAxis READ drawAxis WRITE setDrawAxis NOTIFY drawAxisChanged)
    Q_PROPERTY(Values drawValues READ drawValues WRITE setDrawValues NOTIFY drawValuesChanged)
    Q_PROPERTY(ValueRotation XValueRotation READ valueRotationX WRITE setValueRotationX NOTIFY valueRotationXChanged )
    Q_PROPERTY(ValueRotation YValueRotation READ valueRotationY WRITE setValueRotationY NOTIFY valueRotationYChanged )
    Q_PROPERTY(ValueRotation InlineValueRotation READ valueRotationInline WRITE setValueRotationInline NOTIFY valueRotationInlineChanged )
    Q_PROPERTY(qreal xMarginFactor READ xMarginFactor WRITE setXMarginFactor NOTIFY xMarginFactorChanged)  // => XmarginFactor
    Q_PROPERTY(qreal zMarginFactor READ zMarginFactor WRITE setZMarginFactor NOTIFY zMarginFactorChanged)  // => YMarginFactor

public:
    enum ChartType {Histogram};
    enum LegendTypes {LegendDisabled = 0, LegendRight = 1, LegendBottom = 2};
    enum AxisFlag {AxisX = 1, AxisY = 2, AxisZ = 4 };
    enum ValueFlag {ValueX = 1, ValueY = 2, ValueZ = 4, ValueInline = 8};
    enum ValueRotation {YesRotate = 1, NoRotate = 2, AutoRotate = 3};
    Q_DECLARE_FLAGS(Axis, AxisFlag)
    Q_DECLARE_FLAGS(Values, ValueFlag)

    ChartItem(QObject * parent = 0);

    virtual void moduleInit();

    virtual void init_gui();
    virtual void update_gui();

    virtual CuteReport::BaseItemInterface * createInstance(QObject * parent) const;
    virtual CuteReport::BaseItemHelperInterface * helper();
    virtual QByteArray serialize();
    virtual void deserialize(QByteArray &data);
    virtual bool canContain(QObject * object);

    virtual QIcon itemIcon() const;
    virtual QString moduleShortName() const;
    virtual QString suitName() const { return "Standard"; }
    virtual QString itemGroup() const;

    virtual void renderInit(CuteReport::RendererPublicInterface * renderer);
    virtual void renderReset();
    virtual bool renderPrepare();
    virtual CuteReport::RenderedItemInterface * renderView();
    virtual bool selfRendering() {return true;}

    static void paint(QPainter * painter, const QStyleOptionGraphicsItem *option, const CuteReport::BaseItemInterfacePrivate * data, const QRectF &boundingRect, CuteReport::RenderingType type = CuteReport::RenderingTemplate);

    void recalculateLegendRect();

    QString dataset()const;
    void setDataset(const QString & dataset);

    ChartType chartType() const;
    void setChartType(ChartType chartType);

    LegendTypes legendType() const;
    void setLegendType(LegendTypes legendType);

    qreal legendSize() const;
    void setLegendSize(qreal size);

    QStringList colors() const;
    void setColors(const QStringList & colors);

    QStringList zFields() const;
    void setzFields(const QStringList & values);

    QString xField() const;
    void setxField(const QString & field);

    QFont font() const;
    void setFont(const QFont & font);

    QColor textColor() const;
    void setTextColor(const QColor & color);

    ChartItem::Axis drawAxis() const;
    void setDrawAxis(Axis axis);

    ChartItem::Values drawValues() const;
    void setDrawValues(Values values);

    ChartItem::ValueRotation valueRotationX() const;
    void setValueRotationX(const ChartItem::ValueRotation & value);

    ChartItem::ValueRotation valueRotationY() const;
    void setValueRotationY(const ChartItem::ValueRotation & value);

    ChartItem::ValueRotation valueRotationInline() const;
    void setValueRotationInline(const ChartItem::ValueRotation & value);

    qreal xMarginFactor() const;
    void setXMarginFactor(qreal value);

    qreal zMarginFactor() const;
    void setZMarginFactor(qreal value);

    static void prepareData(ChartItemPrivate * d, CuteReport::ReportInterface *report, const QRectF absoluteGeometry);
    static QColor randomColor();

    virtual QStringList scriptingStrings();

    virtual QString _current_property_description() const;

signals:
    void chartTypeChanged(ChartItem::ChartType);
    void legendTypeChanged(ChartItem::LegendTypes);
    void legendOffsetChanged(QPointF offset);
    void legendSizeChanged(qreal size);
    void colorsChanged(QStringList colors);
    void zFieldsChanged(QStringList fields);
    void xFieldChanged(QString field);
    void datasetChanged(QString);
    void fontChanged(QFont);
    void textColorChaged(QColor);
    void drawAxisChanged(ChartItem::Axis);
    void drawValuesChanged(ChartItem::Values);
    void valueRotationXChanged(ChartItem::ValueRotation);
    void valueRotationYChanged(ChartItem::ValueRotation);
    void valueRotationInlineChanged(ChartItem::ValueRotation);
    void xMarginFactorChanged(qreal);
    void zMarginFactorChanged(qreal);

private:
    Q_DECLARE_PRIVATE(ChartItem)
    ChartItem(ChartItemPrivate *dd, QObject * parent);
    virtual BaseItemInterface * itemClone() const;
    CuteReport::RendererPublicInterface * m_renderer;
};


class  ChartItemView : public CuteReport::ItemInterfaceView
{
    Q_INTERFACES(CuteReport::ItemInterfaceView)
public:
    ChartItemView(CuteReport::BaseItemInterface * item):
        CuteReport::ItemInterfaceView(item),
        m_item(item)
    {}

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)
    {
        Q_UNUSED(widget)
        ChartItem::paint(painter, option, ptr(), boundingRect(), CuteReport::RenderingTemplate);
    }

private:
    CuteReport::BaseItemInterface * m_item;
};


class  RenderedChartItem : public CuteReport::RenderedItemInterface
{
public:
    explicit RenderedChartItem(CuteReport::BaseItemInterface * item, CuteReport::BaseItemInterfacePrivate *itemPrivateData)
        :RenderedItemInterface(item, itemPrivateData)
    {
        ChartItem::prepareData(reinterpret_cast<ChartItemPrivate*> (itemPrivateData), item->reportObject(), item->absoluteGeometry());
    }

    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)
    {
        Q_UNUSED(widget)
        ChartItem::paint(painter, option, d_ptr, boundingRect(), CuteReport::RenderingReport);
    }
};

Q_DECLARE_METATYPE(ChartItem::LegendTypes)
Q_DECLARE_METATYPE(ChartItem::ValueRotation)
Q_DECLARE_METATYPE(ChartItem::ChartType)
Q_DECLARE_OPERATORS_FOR_FLAGS(ChartItem::Axis)
Q_DECLARE_OPERATORS_FOR_FLAGS(ChartItem::Values)

#endif
