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

#include <QObject>
#include <QPointF>

class PageGUI;
class QGraphicsItem;

namespace CuteReport {
    class BaseItemInterface;
}

class Page;

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

    explicit Magnets(PageGUI *pageGUI);
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

    QPointF delta(const QList<QPointF> & pointList, const QList<CuteReport::BaseItemInterface *> &excludeItems, MagnetDirection direction);

public slots:
     void setMagnetRate(int rate);


private:
//    int m_magnets;
//    int m_magnetRate;
    int m_magnetRange;
    Page * m_page;
    QList <QGraphicsItem *> m_gideLines;
};

#endif // MAGNETS_H
