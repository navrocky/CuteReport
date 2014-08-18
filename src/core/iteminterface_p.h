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
#ifndef ITEMINTERFACE_P_H
#define ITEMINTERFACE_P_H
#include <QtCore>
#include <QtGui>
#include "iteminterface.h"
#include "baseiteminterface_p.h"
#include "globals.h"

namespace CuteReport
{

class CUTEREPORT_EXPORTS ItemInterfacePrivate : public CuteReport::BaseItemInterfacePrivate
{
public:
    ItemInterfacePrivate()
        :BaseItemInterfacePrivate(),
        shiftMode(ItemInterface::AlwaysShift)
    {
    }

    ItemInterfacePrivate(const ItemInterfacePrivate & p)
        :BaseItemInterfacePrivate(p),
        shiftMode(p.shiftMode)
    {}

    ItemInterface::ShiftMode shiftMode;
};


CUTEREPORT_EXPORTS QDataStream &operator<<(QDataStream &s, const ItemInterfacePrivate &p);
CUTEREPORT_EXPORTS QDataStream &operator>>(QDataStream &s, ItemInterfacePrivate &p);

}


#endif // ITEMINTERFACE_P_H
