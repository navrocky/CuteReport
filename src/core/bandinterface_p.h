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
#ifndef BANDINTERFACEPRIVATE_H
#define BANDINTERFACEPRIVATE_H

#include "globals.h"
#include "types.h"
#include "iteminterface_p.h"

namespace CuteReport
{

class CUTEREPORT_EXPORTS BandInterfacePrivate : public BaseItemInterfacePrivate
{
public:
    BandInterfacePrivate()
        :BaseItemInterfacePrivate(),
          stretchable(false),
          stretched(false),
          showStretchability(false)
    {
    }

    BandInterfacePrivate(const BandInterfacePrivate & p)
        :BaseItemInterfacePrivate(p),
          margins(p.margins),
          stretchable(p.stretchable),
          stretchOriginalSize(p.stretchOriginalSize),
          stretched(p.stretched),
          showStretchability(p.showStretchability)
    {}

    Margins margins;
    bool stretchable;
    QSizeF stretchOriginalSize;
    bool stretched;
    bool showStretchability;
};

CUTEREPORT_EXPORTS QDataStream &operator<<(QDataStream &s, const BandInterfacePrivate &p);
CUTEREPORT_EXPORTS QDataStream &operator>>(QDataStream &s, BandInterfacePrivate &p);

}

#endif // BANDINTERFACEPRIVATE_H
