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

#include "line.h"
#include "line_p.h"
#include "lineitemscript.h"

#include <QtCore>
#include <QtGui>

using namespace CuteReport;

inline void initMyResource() { Q_INIT_RESOURCE(line); }

LineItem::LineItem(QObject *parent)
    :ItemInterface(new LineItemPrivate, parent)
{
    Q_INIT_RESOURCE(line);
    Q_D(LineItem);
    setResizeFlags( ResizeLeft | ResizeRight | ResizeTop | ResizeBottom);
    d->rect = QRectF(0,0,100,8);
    d->frame = 0;
}


void LineItem::moduleInit()
{
    initMyResource();
}


LineItem::LineItem(LineItemPrivate *dd, QObject * parent)
    :ItemInterface(dd, parent)
{
    Q_INIT_RESOURCE(line);
}


BaseItemInterface *LineItem::itemClone() const
{
    Q_D(const LineItem);
    return new LineItem(new LineItemPrivate(*d), parent());
}


QByteArray LineItem::serialize()
{
    Q_D(LineItem);
    QByteArray ba;
    QDataStream s( &ba, QIODevice::ReadWrite );
    s << *d;
    return ba;
}


void LineItem::deserialize(QByteArray &data)
{
    Q_D(LineItem);
    QDataStream s( &data, QIODevice::ReadWrite );
    s >> *d;
}


QDataStream &operator<<(QDataStream &s, const LineItemPrivate &p) {
    s << static_cast<const ItemInterfacePrivate&>(p);
    s << (qint8)p.lineStyle << p.linePen;
    return s;
}


QDataStream &operator>>(QDataStream &s, LineItemPrivate &p) {
    s >> static_cast<ItemInterfacePrivate&>(p);
    qint8 lineStyle;
    s >> lineStyle; s >> p.linePen;

    p.lineStyle = (LineItem::LineStyle)lineStyle;
    return s;
}


BaseItemInterface *LineItem::createInstance(QObject * parent) const
{
    return new LineItem(parent);
}


CuteReport::BaseItemHelperInterface * LineItem::helper()
{
    return 0;
}


void LineItem::init_gui()
{
    if (m_gui)
        return;
    m_gui = new LineItemView(this);
}


bool LineItem::canContain(QObject * object)
{
    return (dynamic_cast<CuteReport::BaseItemInterface*>(object));
}


QIcon LineItem::itemIcon() const
{
    return QIcon(":/line.png");
}


QString LineItem::moduleShortName() const
{
    return tr("Line");
}


QString LineItem::itemGroup() const
{
    return QString("Shapes");
}


bool LineItem::renderPrepare()
{
    emit printBefore();
    setRenderingPointer(new LineItemPrivate(*(reinterpret_cast<LineItemPrivate*>(d_ptr))));
    emit printDataBefore();
    emit printDataAfter();
    return true;
}


RenderedItemInterface *LineItem::renderView()
{
    Q_D(LineItem);
    RenderedLineItem * view = new RenderedLineItem(this, new LineItemPrivate(*d));
    return view;
}


void LineItem::paint(QPainter * painter, const QStyleOptionGraphicsItem *option, const BaseItemInterfacePrivate * data, const QRectF &boundingRect, RenderingType type)
{
    Q_UNUSED(option)
    Q_UNUSED(type)
    ItemInterface::paintBegin(painter, option, data, boundingRect, type);

    const LineItemPrivate * const d = static_cast<const LineItemPrivate*>(data);

    painter->setPen(d->linePen);

    switch (d->lineStyle)
    {
    case Vertical:
        painter->drawLine(boundingRect.x() + boundingRect.width() / 2, boundingRect.y(), boundingRect.x() +  boundingRect.width() / 2,
                          boundingRect.y() + boundingRect.height());
        break;
    case Horizontal:
        painter->drawLine(boundingRect.x(), boundingRect.y() + boundingRect.height() / 2,
                          boundingRect.x() + boundingRect.width(), boundingRect.y() + boundingRect.height() / 2);
        break;
    case BackwardDiagonal:
        painter->drawLine(boundingRect.right(), boundingRect.y(), boundingRect.x(), boundingRect.bottom());
        break;
    case ForwardDiagonal:
        painter->drawLine(boundingRect.x(), boundingRect.y(), boundingRect.right(), boundingRect.bottom());
        break;
    }

    ItemInterface::paintEnd(painter, option, data, boundingRect, type);
}


LineItem::LineStyle LineItem::lineStyle() const
{
    Q_D(const LineItem);
    return d->lineStyle;
}


void LineItem::setLineStyle(LineStyle lineStyle)
{
    Q_D(LineItem);
    if (d->lineStyle == lineStyle)
        return;

    d->lineStyle = lineStyle;
    update_gui();
    emit lineStyleChanged(d->lineStyle);
    emit lineStyleChanged(LineItem_LineStyle_toString(d->lineStyle));
    emit changed();
}


QString LineItem::lineStyleStr() const
{
    Q_D(const LineItem);
    return LineItem_LineStyle_toString(d->lineStyle);
}


void LineItem::setLineStyleStr(const QString &lineStyle)
{
    bool ok;
    LineItem::LineStyle value = LineItem_LineStyle_fromString(lineStyle, &ok);
    if (ok)
        setLineStyle(value);
}


QPen LineItem::linePen() const
{
    Q_D(const LineItem);
    return d->linePen;
}


void LineItem::setLinePen(const QPen & pen)
{
    Q_D(LineItem);
    if (d->linePen != pen) {
        d->linePen = pen;
        update_gui();
        emit linePenChanged(pen);
        emit changed();
    }
}


void LineItem::initScript(QScriptEngine *scriptEngine)
{
    registerLineItemScriptClass(scriptEngine);
}


QStringList LineItem::_lineStyle_variants() const
{
    return LineItem_LineStyle_variants();
}


QString LineItem::_current_property_description() const
{
    QString propertyName = metaObject()->property(m_currentProperty).name();

    if (propertyName == "linePen")
        return tr("Pen parameters");
    else if (propertyName == "lineStyle")
        return tr("Type of line direction");
    else
        return ItemInterface::_current_property_description();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Line, LineItem)
#endif
