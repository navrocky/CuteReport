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
#include "types.h"

namespace CuteReport
{

Margins::Margins()
    : m_left(0), m_top(0), m_right(0),  m_bottom(0)
{

}

Margins::Margins(qreal left, qreal top, qreal right, qreal bottom)
{
    m_left = left;
    m_top = top;
    m_right = right;
    m_bottom = bottom;
}


Margins::~Margins()
{

}

qreal Margins::left() const
{
    return m_left;
}

qreal Margins::top() const
{
    return m_top;
}

qreal Margins::right() const
{
    return m_right;
}

qreal Margins::bottom() const
{
    return m_bottom;
}

void Margins::setLeft(qreal value)
{
    m_left=value;
}

void Margins::setTop(qreal value)
{
    m_top=value;
}

void Margins::setRight(qreal value)
{
    m_right = value;
}

void Margins::setBottom(qreal value)
{
    m_bottom = value;
}

void Margins::setCoords(qreal x1, qreal y1, qreal x2, qreal y2)
{
    m_left = x1;
    m_top = y1;
    m_right = x2;
    m_bottom = y2;
}


bool Margins::operator ==(const Margins & margins)
{
    return ((m_left == margins.m_left) && (m_top == margins.m_top) && (m_right == margins.right()) && (m_bottom == margins.m_bottom));
}


QDataStream &operator<<(QDataStream &s, const Margins &p)
{
    s << p.left() << p.top() << p.right() << p.bottom();
    return s;
}

QDataStream &operator>>(QDataStream &s, Margins &p)
{
    qreal x1, y1, x2, y2;
    s >> x1; s >> y1; s >> x2; s >> y2;
    p.setCoords(x1,y1,x2,y2);
    return s;
}



Units::Units()
{
    m_mm = 0.5;
    m_inch=0.05;
    m_pixel=1;
}


Units::Units(qreal mm, qreal inch, int pixel)
{
    m_mm = mm;
    m_inch = inch;
    m_pixel = pixel;
}

Units::~Units()
{
}

void Units::setMM(qreal value)
{
    m_mm = value;
}

void Units::setInch(qreal value)
{
    m_inch = value;
}

void Units::setPixel(int value)
{
    m_pixel = value;
}

qreal Units::mm() const
{
    return m_mm;
}

qreal Units::inch() const
{
    return m_inch;
}

int Units::pixel() const
{
    return m_pixel;
}


} //namespace
