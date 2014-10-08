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
#ifndef MAGNETS_H
#define MAGNETS_H

#include "plugins_common.h"

#include <QObject>
#include <QPointF>

class QGraphicsItem;

namespace CuteReport {
    class BaseItemInterface;
}

SUIT_BEGIN_NAMESPACE
class PageGUI;
class Page;
SUIT_END_NAMESPACE

USING_SUIT_NAMESPACE

SUIT_BEGIN_NAMESPACE
class Magnets : public QObject
{
    Q_OBJECT
public:
    enum MagnetDirection { HDirection, VDirection, HVDirection};

    enum MagnetType
    {
        Left = 1,
        Right = 2,
        Top = 4,
        Bottom = 8,
        Vertical = 16,
        Horizontal = 32
    };

    explicit Magnets(SUIT_NAMESPACE::PageGUI *pageGUI);
    ~Magnets();
    
    int magnetsValue();
    void setMagnetsValue(int rate);

    int magnetRate() const;

    void setLeftMagnet(bool magneted);
    void setRightMagnet(bool magneted);
    void setTopMagnet(bool magneted);
    void setBottomMagnet(bool magneted);
    void setHorizontalMagnet(bool magneted);
    void setVerticalMagnet(bool magneted);

    void clear();

    QPointF delta(const QList<QPointF> & pointList, const QList<CuteReport::BaseItemInterface *> &checkingItems, MagnetDirection direction);

    qreal magnetRangeMM() const;

public slots:
    void setMagnetRate(int rate);


private:
    //    int m_magnets;
//    int m_magnetRate;
    qreal m_magnetRangeMM;
    Page * m_page;
    QList <QGraphicsItem *> m_gideLines;
};

SUIT_END_NAMESPACE

#endif // MAGNETS_H
