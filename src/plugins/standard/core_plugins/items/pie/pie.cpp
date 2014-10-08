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

#include "pie.h"
#include "pie_p.h"
#include "pieitemscript.h"

#include <QtCore>
#include <QtGui>

using namespace CuteReport;

inline void initMyResource() { Q_INIT_RESOURCE(pie); }

PieItem::PieItem(QObject *parent)
    :ItemInterface(new PieItemPrivate, parent)
{
    Q_INIT_RESOURCE(pie);
    Q_D(PieItem);
    setResizeFlags( ResizeLeft | ResizeRight | ResizeTop | ResizeBottom);
    d->rect = QRectF(0,0,30,30);
    d->frame = 0;
}


void PieItem::moduleInit()
{
    initMyResource();
}


PieItem::PieItem(PieItemPrivate *dd, QObject * parent)
    :ItemInterface(dd, parent)
{
    Q_INIT_RESOURCE(pie);
}


CuteReport::BaseItemInterface * PieItem::itemClone() const
{
    Q_D(const PieItem);
    return new PieItem(new PieItemPrivate(*d), parent());
}


QByteArray PieItem::serialize()
{
    Q_D(PieItem);
    QByteArray ba;
    QDataStream s( &ba, QIODevice::ReadWrite );
    s << *d;
    return ba;
}


void PieItem::deserialize(QByteArray &data)
{
    Q_D(PieItem);
    QDataStream s( &data, QIODevice::ReadWrite );
    s >> *d;
}


QDataStream &operator<<(QDataStream &s, const PieItemPrivate &p) {
    s << static_cast<const ItemInterfacePrivate&>(p);
    s << p.pen << (qint16)p.startAngle << (qint16)p.spanAngle;
    return s;
}


QDataStream &operator>>(QDataStream &s, PieItemPrivate &p) {
    s >> static_cast<ItemInterfacePrivate&>(p);
    qint16 startAngle, spanAngle;

    s >> p.pen; s >> startAngle; s >> spanAngle;
    p.spanAngle = startAngle;
    p.spanAngle = spanAngle;

    return s;
}


BaseItemInterface *PieItem::createInstance(QObject * parent) const
{
    return new PieItem(parent);
}


BaseItemHelperInterface *PieItem::helper()
{
    return 0;
}


void PieItem::init_gui()
{
    if (m_gui)
        return;
    m_gui = new PieItemView(this);
}


bool PieItem::canContain(QObject * object)
{
    return (dynamic_cast<CuteReport::BaseItemInterface*>(object));
}


QIcon PieItem::itemIcon() const
{
    return QIcon(":/pie.png");
}


QString PieItem::moduleShortName() const
{
    return tr("Pie");
}


QString PieItem::itemGroup() const
{
    return QString("Shapes");
}


bool PieItem::renderPrepare()
{
    emit printBefore();
    setRenderingPointer(new PieItemPrivate(*(reinterpret_cast<PieItemPrivate*>(d_ptr))));
    emit printDataBefore();
    emit printDataAfter();
    return true;
}


RenderedItemInterface *PieItem::renderView()
{
    Q_D(PieItem);
    RenderedPieItem * view = new RenderedPieItem(this, new PieItemPrivate(*d));
    return view;
}


void PieItem::paint(QPainter * painter, const QStyleOptionGraphicsItem *option, const BaseItemInterfacePrivate * data, const QRectF &boundingRect, RenderingType type)
{
    Q_UNUSED(option)
    Q_UNUSED(type)
    ItemInterface::paintBegin(painter, option, data, boundingRect, type);

    const PieItemPrivate * const d = static_cast<const PieItemPrivate*>(data);

    painter->setPen(d->pen);
    painter->setBrush(d->brush);

    painter->drawPie(boundingRect, d->startAngle*16, d->spanAngle*16);

    ItemInterface::paintEnd(painter, option, data, boundingRect, type);
}


QPen PieItem::pen() const
{
    Q_D(const PieItem);
    return d->pen;
}


void PieItem::setPen(const QPen & pen)
{
    Q_D(PieItem);
    if (d->pen != pen) {
        d->pen = pen;
        update_gui();
        emit penChanged(d->pen);
        emit changed();
    }
}


QBrush PieItem::brush() const
{
    Q_D(const PieItem);
    return d->brush;
}


void PieItem::setBrush(const QBrush & brush)
{
    Q_D(PieItem);
    if (d->brush != brush) {
        d->brush = brush;
        update_gui();
        emit brushChanged(d->brush);
        emit changed();
    }
}

int PieItem::spanAngle() const
{
    Q_D(const PieItem);
    return d->spanAngle;
}


void PieItem::setSpanAngle(int span)
{
    Q_D(PieItem);
    if (span == d->spanAngle)
        return;

    d->spanAngle = span;
    update_gui();
    emit spanAngleChanged(d->spanAngle);
    emit changed();
}


int PieItem::startAngle() const
{
    Q_D(const PieItem);
    return d->startAngle;
}


void PieItem::setStartAngle(int start)
{
    Q_D(PieItem);
    if (d->startAngle == start)
        return;
    d->startAngle = start;
    update_gui();
    emit startAngleChanged(d->startAngle);
    emit changed();
}


void PieItem::initScript(QScriptEngine *scriptEngine)
{
    registerPieItemScriptClass(scriptEngine);
}


QString PieItem::_current_property_description() const
{
    QString propertyName = metaObject()->property(m_currentProperty).name();

    if (propertyName == "pen")
        return tr("Pen parameters");
    else if (propertyName == "brush")
        return tr("Brush parameters");
    else if (propertyName == "startAngle")
        return tr("Start angle specified in a degree. Positive values for the angle mean counter-clockwise while negative values mean the clockwise direction. Zero degrees is at the 3 o'clock position.");
    else if (propertyName == "spanAngle")
        return tr("Span angle specified in degree. Positive values for the angle mean counter-clockwise while negative values mean the clockwise direction");
    else
        return ItemInterface::_current_property_description();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Pie, PieItem)
#endif
