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

#include "arc.h"
#include "arc_p.h"
#include <QtCore>
#include <QtGui>

#include "arcscripting.h"

using namespace CuteReport;

inline void initResource() { Q_INIT_RESOURCE(arc); }

ArcItem::ArcItem(QObject *parent)
    :ItemInterface(new ArcItemPrivate, parent)
{
    Q_INIT_RESOURCE(arc);
    Q_D(ArcItem);
    setResizeFlags( ResizeLeft | ResizeRight | ResizeTop | ResizeBottom);
    d->rect = QRectF(0,0,30,20);
    d->frame = 0;
}


void ArcItem::moduleInit()
{
    initResource();
}


ArcItem::ArcItem(ArcItemPrivate *dd, QObject * parent)
    :ItemInterface(dd, parent)
{
    Q_INIT_RESOURCE(arc);
}


CuteReport::BaseItemInterface * ArcItem::itemClone() const
{
    Q_D(const ArcItem);
    return new ArcItem(new ArcItemPrivate(*d), parent());
}


QByteArray ArcItem::serialize()
{
    Q_D(ArcItem);
    QByteArray ba;
    QDataStream s( &ba, QIODevice::ReadWrite );
    s << *d;
    return ba;
}


void ArcItem::deserialize(QByteArray &data)
{
    Q_D(ArcItem);
    QDataStream s( &data, QIODevice::ReadWrite );
    s >> *d;
}


QDataStream &operator<<(QDataStream &s, const ArcItemPrivate &p) {
    s << static_cast<const ItemInterfacePrivate&>(p);
    s << p.pen << (qint16)p.startAngle << (qint16)p.spanAngle;
    return s;
}


QDataStream &operator>>(QDataStream &s, ArcItemPrivate &p) {
    s >> static_cast<ItemInterfacePrivate&>(p);
    qint16 startAngle, spanAngle;

    s >> p.pen; s >> startAngle; s >> spanAngle;
    p.spanAngle = startAngle;
    p.spanAngle = spanAngle;

    return s;
}


BaseItemInterface *ArcItem::createInstance(QObject * parent) const
{
    return new ArcItem(parent);
}


BaseItemHelperInterface *ArcItem::helper()
{
    return 0;
}


void ArcItem::init_gui()
{
    if (m_gui)
        return;
    m_gui = new ArcItemView(this);
}


bool ArcItem::canContain(QObject * object)
{
    return (dynamic_cast<CuteReport::BaseItemInterface*>(object));
}


QIcon ArcItem::itemIcon() const
{
    return QIcon(":/arc.png");
}


QString ArcItem::moduleShortName() const
{
    return tr("Arc");
}


QString ArcItem::itemGroup() const
{
    return QString("Shapes");
}


bool ArcItem::renderPrepare()
{
    emit printBefore();
    setRenderingPointer(new ArcItemPrivate(*(reinterpret_cast<ArcItemPrivate*>(d_ptr))));
    emit printDataBefore();
    emit printDataAfter();
    return true;
}


RenderedItemInterface *ArcItem::renderView()
{
    Q_D(ArcItem);
    CuteReport::RenderedItemInterface * view = new RenderedArcItem(this, new ArcItemPrivate(*d));
    return view;
}


void ArcItem::paint(QPainter * painter, const QStyleOptionGraphicsItem *option, const BaseItemInterfacePrivate * data, const QRectF &boundingRect, RenderingType type)
{
    Q_UNUSED(option)
    Q_UNUSED(type)
    ItemInterface::paintBegin(painter, option, data, boundingRect, type);

    const ArcItemPrivate * const d = static_cast<const ArcItemPrivate*>(data);

    painter->setPen(d->pen);
    painter->drawArc(boundingRect, d->startAngle*16, d->spanAngle*16);

    ItemInterface::paintEnd(painter, option, data, boundingRect, type);
}


QPen ArcItem::pen() const
{
    Q_D(const ArcItem);
    return d->pen;
}


void ArcItem::setPen(const QPen & pen)
{
    Q_D(ArcItem);
    if (d->pen != pen) {
        d->pen = pen;
        update_gui();
        emit penChanged(d->pen);
        emit changed();
    }
}


int ArcItem::spanAngle() const
{
    Q_D(const ArcItem);
    return d->spanAngle;
}


void ArcItem::setSpanAngle(int span)
{
    Q_D(ArcItem);
    if (span == d->spanAngle)
        return;

    d->spanAngle = span;
    update_gui();
    emit spanAngleChanged(d->spanAngle);
    emit changed();
}


int ArcItem::startAngle() const
{
    Q_D(const ArcItem);
    return d->startAngle;
}


void ArcItem::setStartAngle(int start)
{
    Q_D(ArcItem);
    if (d->startAngle == start)
        return;
    d->startAngle = start;
    update_gui();
    emit startAngleChanged(d->startAngle);
    emit changed();
}


void ArcItem::initScript(QScriptEngine *scriptEngine)
{
    registerArcItemScriptClass(scriptEngine);
}


QString ArcItem::_current_property_description() const
{
    QString propertyName = metaObject()->property(m_currentProperty).name();

    if (propertyName == "pen")
        return tr("Pen parameters");
    else if (propertyName == "startAngle")
        return tr("A number of degrees where arc started");
    else if (propertyName == "spanAngle")
        return tr("A number of degrees from startAngle");
    else
        return ItemInterface::_current_property_description();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Arc, ArcItem)
#endif
