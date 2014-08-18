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

#ifndef CHORD_P_H
#define CHORD_P_H

#include "iteminterface_p.h"
#include "iteminterface.h"
#include "chord.h"

class ChordItemPrivate : public CuteReport::ItemInterfacePrivate
{
public:
    ChordItemPrivate()
        :ItemInterfacePrivate(),
          startAngle(0),
          spanAngle(180)
    {}

    ChordItemPrivate(const ChordItemPrivate & p)
        :ItemInterfacePrivate(p),
          pen(p.pen),
          brush(p.brush),
          startAngle(p.startAngle),
          spanAngle(p.spanAngle)
    {}

    virtual ~ChordItemPrivate(){}

    QPen pen;
    QBrush brush;
    int startAngle;
    int spanAngle;
};

QDataStream &operator<<(QDataStream &s, const ChordItemPrivate &p);
QDataStream &operator>>(QDataStream &s, ChordItemPrivate &p);

#endif // CHORD_P_H
