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

#include "bandinterface.h"
#include "bandinterface_p.h"
#include "pageinterface.h"
#include "item_common/simpleitemview.h"
#include "item_common/simplerendereditem.h"
#include <QGraphicsScene>
#include <QPainter>

namespace CuteReport
{

BandInterface::BandInterface(QObject * parent)
    :BaseItemInterface(new BandInterfacePrivate, parent)
{
    initMe();
}


BandInterface::BandInterface(BandInterfacePrivate *dd, QObject * parent)
    :BaseItemInterface(dd, parent)
{
    initMe();
}


BandInterface::~BandInterface()
{
}


void BandInterface::initMe()
{
    setResizeFlags(FixedPos | ResizeTop | ResizeBottom);
}


BandInterface::LayoutType BandInterface::layoutType() const
{
    return LayoutTop;
}


int BandInterface::layoutPriority() const
{
    return 0;
}


BandInterface::AccomodationType BandInterface::accommodationType() const
{
    return AccomodationOnce;
}


void BandInterface::setGeometry(const QRectF & rect, Unit unit)
{
    Q_D(BandInterface);
    if (!m_inited) {
        d->rect = rect;
        return;
    }

    Unit u = (unit == UnitNotDefined) ? d->unit : unit;
    QRectF newRect = convertUnit(rect, u, Millimeter, d->dpi);

    if (d->rect  == newRect)
        return;

    d->rect = newRect;

    if (d->rect.width() < d->minRectSize.width())
        d->rect.setWidth(d->minRectSize.width());
    if (d->rect.height() < d->minRectSize.height())
        d->rect.setHeight(d->minRectSize.height());

    adjustSize();
    d->stretched = false;

    update_gui();

    emit geometryChanged(geometry());
    emit changed();
}


void BandInterface::setHeight(qreal height, Unit unit)
{
    Q_D(BandInterface);
    if (!m_inited) {
        d->rect.setHeight(height);
        return;
    }

    Unit u = (unit == UnitNotDefined) ? d->unit : unit;
    qreal newValue = convertUnit(height, u, Millimeter, d->dpi);

    if (d->rect.height()  == newValue)
        return;

    d->rect.setHeight(newValue);

    if (d->rect.height() < d->minRectSize.height())
        d->rect.setHeight(d->minRectSize.height());

    adjustSize();
    d->stretched = false;

    update_gui();

    emit geometryChanged(geometry());
    emit changed();

}


Margins BandInterface::margins() const
{
    Q_D(const BandInterface);
    return d->margins;
}


void BandInterface::setMargins(const Margins &margins)
{
    Q_D(BandInterface);
    if (d->margins == margins)
        return;
    d->margins = margins;
    emit marginsChanged(d->margins);
    emit changed();
}


bool BandInterface::stretchable() const
{
    Q_D( const BandInterface);
    return d->stretchable;
}


void BandInterface::setStretchable(bool value)
{
    Q_D(BandInterface);
    if (d->stretchable == value)
        return;
    d->stretchable = value;

    if (!m_inited)
        return;

    if (adjustSize()) {
        update_gui();
        emit geometryChanged(geometry());
        emit changed();
    }

    emit stretchableChanged(d->stretchable);
    emit changed();
}


bool BandInterface::showStretchability() const
{
    Q_D(const BandInterface);
    return d->showStretchability;
}


void BandInterface::setShowStretchability(bool value)
{
    Q_D(BandInterface);
    if (d->showStretchability == value)
        return;
    d->showStretchability = value;

    if (!m_inited)
        return;

    foreach (BaseItemInterface * child, findChildren<BaseItemInterface*>()) {
        if (d->stretchable && d->showStretchability)
            connect(child, SIGNAL(geometryChanged(QRectF)), this, SLOT(childGeometryChanged(QRectF)), Qt::UniqueConnection);
        else
            disconnect(child, SIGNAL(geometryChanged(QRectF)), this, SLOT(childGeometryChanged(QRectF)));
    }

    if (adjustSize()) {
        update_gui();
        emit geometryChanged(geometry());
        emit changed();
    }

    emit showStretchabilityChanged(d->showStretchability);
    emit changed();
}


QSizeF BandInterface::stretchOriginalSize() const
{
    Q_D( const BandInterface);
    return d->stretchOriginalSize;
}


void BandInterface::setStretchOriginalSize(const QSizeF &size)
{
    Q_D(BandInterface);
    if (d->stretchOriginalSize == size)
        return;
    d->stretchOriginalSize = size;
}


void BandInterface::init_gui()
{
    if (m_gui)
        return;
    m_gui = new SimpleItemView(this);
}


//RenderedItemInterface *BandInterface::renderView()
//{
//    Q_D(BandInterface);
//    return new SimpleRenderedItem(this, new BandInterfacePrivate(*d));
//}


QByteArray BandInterface::serialize() const
{
    Q_D(const BandInterface);
    QByteArray ba;
    QDataStream s( &ba, QIODevice::ReadWrite );
    s << *d;
    return ba;
}


void BandInterface::deserialize(QByteArray & data)
{
    Q_D(BandInterface);
    QDataStream s( &data, QIODevice::ReadWrite );
    s >> *d;
}


void BandInterface::childAdded(BaseItemInterface * item)
{
    Q_D(BandInterface);
    if (d->stretchable) {
        connect(item, SIGNAL(geometryChanged(QRectF)), this, SLOT(childGeometryChanged(QRectF)), Qt::UniqueConnection);
        if (adjustSize()) {
            update_gui();
            emit geometryChanged(geometry());
            emit changed();
        }
    }
}


void BandInterface::childRemoved(BaseItemInterface * item)
{
    Q_D(BandInterface);
    if (d->stretchable) {
        disconnect(item, SIGNAL(geometryChanged(QRectF)), this, SLOT(childGeometryChanged(QRectF)));
        if (adjustSize()) {
            update_gui();
            emit geometryChanged(geometry());
            emit changed();
        }
    }
}


bool BandInterface::adjustSize()
{
    Q_D(BandInterface);
    bool result = false;

    if (d->stretchable && d->showStretchability) {
        QSizeF size = d->stretched ? d->stretchOriginalSize : d->rect.size();
        QSizeF baseSize = size;
        foreach (BaseItemInterface * child, findChildren<BaseItemInterface*>()) {
            QRectF childRect = child->geometry(Millimeter);
            if ( childRect.bottom() > size.height())
                size.setHeight( childRect.bottom() );
        }
        if (size.height() > baseSize.height()) {
            if (!d->stretched) {
                d->stretched = true;
                d->stretchOriginalSize = d->rect.size();
            }
            d->rect.setSize(size);
            result = true;
        }
    } else {
        if (d->stretched) {
            d->stretched = false;
            if (d->rect.size() != d->stretchOriginalSize) {
                d->rect.setSize(d->stretchOriginalSize );
                result = true;
            }
        }
    }
    return result;
}


void BandInterface::childGeometryChanged(QRectF geometry)
{
    Q_UNUSED(geometry)

    if (adjustSize()) {
        update_gui();
        emit geometryChanged(this->geometry());
        emit changed();
    }
}


QDataStream &operator<<(QDataStream &s, const BandInterfacePrivate &p) {
    s << static_cast<const BaseItemInterfacePrivate&>(p);
    s << p.margins << p.stretchable << p.stretchOriginalSize << p.stretched << p.showStretchability;
    return s;
}


QDataStream &operator>>(QDataStream &s, BandInterfacePrivate &p) {
    s >> static_cast<BaseItemInterfacePrivate&>(p);
    s >> p.margins; s >> p.stretchable; s >> p.stretchOriginalSize; s >> p.stretched; s >> p.showStretchability;
    return s;
}



}
