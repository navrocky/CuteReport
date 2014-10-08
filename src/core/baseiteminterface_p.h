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
#ifndef BASEITEMINTERFACE_P_H
#define BASEITEMINTERFACE_P_H
#include <QtCore>
#include <QtGui>
#include "baseiteminterface.h"
#include "globals.h"

namespace CuteReport
{

class CUTEREPORT_EXPORTS  RenderingStruct
{
public:
    explicit RenderingStruct(){}
    explicit RenderingStruct(const RenderingStruct & p) {}

//    virtual qint8 type() const = 0;

    virtual ~RenderingStruct(){}
};


class CUTEREPORT_EXPORTS BaseItemInterfacePrivate
{
public:
    BaseItemInterfacePrivate():
        opacity(1),
        rotation(0),
        enabled(true),
        order(0),
        dpi(92),
        unit(Millimeter),
        selfRendering(false),
        childrenSelfRendering(false),
        minRectSize(1,1),
        renderingType(RenderingTemplate),
        r(0)
    {
        frame = BaseItemInterface::DrawLeft | BaseItemInterface::DrawRight | BaseItemInterface::DrawTop | BaseItemInterface::DrawBottom;
        borderPen.setWidth(0);
    }

    BaseItemInterfacePrivate(const BaseItemInterfacePrivate & p):
        borderPen(p.borderPen),
        bgBrush(p.bgBrush),
        rect(p.rect),
        frame(p.frame),
        opacity(p.opacity),
        rotation(p.rotation),
        enabled(p.enabled),
        order(p.order),
        dpi(p.dpi),
        unit(p.unit),
        selfRendering(p.selfRendering),
        childrenSelfRendering(p.childrenSelfRendering),
        minRectSize(p.minRectSize),
        resizeFlags(p.resizeFlags),
        renderingType(p.renderingType),
        r(0)
    {}

    virtual ~BaseItemInterfacePrivate() {
        delete r;
    }

    virtual void initRenderingData() { }


    QPen borderPen;
    QBrush bgBrush;
    QRectF rect;
    qint8 frame;
    qreal opacity;
    qreal rotation;
    bool enabled;
    qint16 order;
    qint16 dpi;
    Unit unit;
    bool selfRendering;
    bool childrenSelfRendering;
    QSizeF minRectSize;
    int resizeFlags;
    RenderingType renderingType;
    RenderingStruct * r;
};


CUTEREPORT_EXPORTS QDataStream &operator<<(QDataStream &s, const BaseItemInterfacePrivate &p);
CUTEREPORT_EXPORTS QDataStream &operator>>(QDataStream &s, BaseItemInterfacePrivate &p);

}


#endif // BASEITEMINTERFACE_P_H
