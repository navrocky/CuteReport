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
#include "iteminterface.h"
#include "iteminterface_p.h"
#include "iteminterfaceview.h"
#include "renderediteminterface.h"
#include "pageinterface.h"
#include "reportinterface.h"
//#include "unit.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>

namespace CuteReport
{

static const char * ShiftModeStr[] = {"AlwaysShift", "DontShift", "ShiftWhenOverlapped" };
static const int ShiftModeNum = 3;

ItemInterface::ItemInterface(QObject * parent)
    :BaseItemInterface(new ItemInterfacePrivate, parent)
{
}


ItemInterface::ItemInterface(ItemInterfacePrivate *dd, QObject * parent)
    :BaseItemInterface(dd, parent)
{
}


ItemInterface::~ItemInterface()
{
}


QByteArray ItemInterface::serialize() const
{
    Q_D(const ItemInterface);
    QByteArray ba;
    QDataStream s( &ba, QIODevice::ReadWrite );
    s << *d;
    return ba;
}


void ItemInterface::deserialize(QByteArray & data)
{
    Q_D(ItemInterface);
    QDataStream s( &data, QIODevice::ReadWrite );
    s >> *d;
}


QDataStream &operator<<(QDataStream &s, const ItemInterfacePrivate &p) {
    s << static_cast<const BaseItemInterfacePrivate&>(p);
    s << p.shiftMode;
    return s;
}


QDataStream &operator>>(QDataStream &s, ItemInterfacePrivate &p) {
    s >> static_cast<BaseItemInterfacePrivate&>(p);

    qint8 shiftMode;
    s >> shiftMode;

    p.shiftMode = (ItemInterface::ShiftMode) shiftMode;
    return s;
}



ItemInterface::ShiftMode ItemInterface::shiftMode() const
{
    Q_D(const ItemInterface);
    return d->shiftMode;
}


void ItemInterface::setShiftMode(ItemInterface::ShiftMode shiftMode)
{
    Q_D(ItemInterface);
    if (d->shiftMode == shiftMode)
        return;

    d->shiftMode = shiftMode;

    emit shiftModeChanged(d->shiftMode);
    emit shiftModeChanged(shiftModeToString(d->shiftMode));
    emit changed();
}


QString ItemInterface::shiftModeStr() const
{
    return shiftModeToString(shiftMode());
}


void ItemInterface::setShiftModeStr(const QString &shiftMode)
{
    setShiftMode(shiftModeFromString(shiftMode));
}


QString ItemInterface::_current_property_description() const
{
    QString propertyName = metaObject()->property(m_currentProperty).name();

    if (propertyName == "shiftMode")
        return tr("Set item shifting mode. If above item size changed, current one can be shifted.");

    return BaseItemInterface::_current_property_description();
}


QStringList ItemInterface::_shiftMode_variants() const
{
    QStringList list;
    for (int i=0; i<ShiftModeNum; ++i) {
        list << shiftModeToString(ShiftMode(i));
    }
    return list;
}


ItemInterface::ShiftMode ItemInterface::shiftModeFromString(const QString &mode)
{
    for (int i=0; i<ShiftModeNum; ++i) {
        if (mode == ShiftModeStr[i]) {
            return ShiftMode(i);
        }
    }
    return DontShift;
}


QString ItemInterface::shiftModeToString(ItemInterface::ShiftMode mode)
{
    return ShiftModeStr[mode];
}



} //namespace
