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

#include "ellipse.h"
#include "ellipse_p.h"
#include "ellipsescripting.h"

#include <QtCore>
#include <QtGui>

using namespace CuteReport;

inline void initMyResource() { Q_INIT_RESOURCE(ellipse); }

EllipseItem::EllipseItem(QObject *parent)
    :ItemInterface(new EllipseItemPrivate, parent)
{
    Q_INIT_RESOURCE(ellipse);
    Q_D(EllipseItem);
    setResizeFlags( ResizeLeft | ResizeRight | ResizeTop | ResizeBottom);
    d->rect = QRectF(0,0,30,20);
    d->frame = 0;
}


void EllipseItem::moduleInit()
{
    initMyResource();
}


EllipseItem::EllipseItem(EllipseItemPrivate *dd, QObject * parent)
    :ItemInterface(dd, parent)
{
    Q_INIT_RESOURCE(ellipse);
}


CuteReport::BaseItemInterface * EllipseItem::itemClone() const
{
    Q_D(const EllipseItem);
    return new EllipseItem(new EllipseItemPrivate(*d), parent());
}


QByteArray EllipseItem::serialize()
{
    Q_D(EllipseItem);
    QByteArray ba;
    QDataStream s( &ba, QIODevice::ReadWrite );
    s << *d;
    return ba;
}


void EllipseItem::deserialize(QByteArray &data)
{
    Q_D(EllipseItem);
    QDataStream s( &data, QIODevice::ReadWrite );
    s >> *d;
}


QDataStream &operator<<(QDataStream &s, const EllipseItemPrivate &p) {
    s << static_cast<const ItemInterfacePrivate&>(p);
    return s;
}


QDataStream &operator>>(QDataStream &s, EllipseItemPrivate &p) {
    s >> static_cast<ItemInterfacePrivate&>(p);
    return s;
}


BaseItemInterface *EllipseItem::createInstance(QObject * parent) const
{
    return new EllipseItem(parent);
}


BaseItemHelperInterface *EllipseItem::helper()
{
    return 0;
}


void EllipseItem::init_gui()
{
    if (m_gui)
        return;
    m_gui = new EllipseItemView(this);
}


bool EllipseItem::canContain(QObject * object)
{
    return (dynamic_cast<CuteReport::BaseItemInterface*>(object));
}


QIcon EllipseItem::itemIcon() const
{
    return QIcon(":/ellipse.png");
}


QString EllipseItem::moduleShortName() const
{
    return tr("Ellipse");
}


QString EllipseItem::itemGroup() const
{
    return QString("Shapes");
}


bool EllipseItem::renderPrepare()
{
    emit printBefore();
    setRenderingPointer(new EllipseItemPrivate(*(reinterpret_cast<EllipseItemPrivate*>(d_ptr))));
    emit printDataBefore();
    emit printDataAfter();
    return true;
}


RenderedItemInterface *EllipseItem::renderView()
{
    Q_D(EllipseItem);
    RenderedEllipseItem * view = new RenderedEllipseItem(this, new EllipseItemPrivate(*d));
    return view;
}


void EllipseItem::paint(QPainter * painter, const QStyleOptionGraphicsItem *option, const BaseItemInterfacePrivate * data, const QRectF &boundingRect, RenderingType type)
{
    Q_UNUSED(option)
    Q_UNUSED(type)
    ItemInterface::paintBegin(painter, option, data, boundingRect, type);

    const EllipseItemPrivate * const d = static_cast<const EllipseItemPrivate*>(data);

    painter->setPen(d->pen);
    painter->setBrush(d->brush);

    painter->drawEllipse(boundingRect);

    ItemInterface::paintEnd(painter, option, data, boundingRect, type);
}


QPen EllipseItem::pen() const
{
    Q_D(const EllipseItem);
    return d->pen;
}


void EllipseItem::setPen(const QPen & pen)
{
    Q_D(EllipseItem);
    if (d->pen != pen) {
        d->pen = pen;
        update_gui();
        emit penChanged(d->pen);
        emit changed();
    }
}


QBrush EllipseItem::brush() const
{
    Q_D(const EllipseItem);
    return d->brush;
}


void EllipseItem::setBrush(const QBrush & brush)
{
    Q_D(EllipseItem);
    if (d->brush != brush) {
        d->brush = brush;
        update_gui();
        emit brushChanged(d->brush);
        emit changed();
    }
}


void EllipseItem::initScript(QScriptEngine *scriptEngine)
{
    registerEllipseItemScriptClass(scriptEngine);
}


QString EllipseItem::_current_property_description() const
{
    QString propertyName = metaObject()->property(m_currentProperty).name();

    if (propertyName == "pen")
        return tr("Pen parameters");
    else if (propertyName == "brush")
        return tr("Brush options for fill chord");
    else
        return ItemInterface::_current_property_description();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Ellipse, EllipseItem)
#endif
