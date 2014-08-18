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
#ifndef RENDEREDITEMINTERFACE_H
#define RENDEREDITEMINTERFACE_H

#include <QGraphicsRectItem>
#include "globals.h"
#include "types.h"

static int RenderedItemInterfaceType = QGraphicsItem::UserType + 17778;

namespace CuteReport {

class BaseItemInterface;
class BaseItemInterfacePrivate;

class CUTEREPORT_EXPORTS RenderedItemInterface : public QGraphicsRectItem
{

public:
    explicit RenderedItemInterface(BaseItemInterface * item, CuteReport::BaseItemInterfacePrivate *itemPrivateData);
    virtual ~RenderedItemInterface();

    BaseItemInterface * coreItem(){return m_item;}

    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    virtual void redraw(bool withChildren = true);

    virtual QRectF absoluteGeometry(Unit unit = UnitNotDefined) const;
    virtual void setAbsoluteGeometry(const QRectF & geometry, Unit unit = UnitNotDefined);

    virtual QPointF absolutePixelPos() const;

    virtual int dpi();
    virtual void setDpi(int dpi, bool withChildren = true);

    virtual int type() const { return RenderedItemInterfaceType; }

protected:
    BaseItemInterfacePrivate * const d_ptr;
    virtual void paintBegin(QPainter * painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual void paintEnd(QPainter * painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
    Q_DECLARE_PRIVATE(BaseItemInterface)
    BaseItemInterface * m_item;
};

}

Q_DECLARE_INTERFACE(CuteReport::RenderedItemInterface, "CuteReport.RenderedItemInterface/1.0")
Q_DECLARE_METATYPE(CuteReport::RenderedItemInterface*)

#endif // RENDEREDITEMINTERFACE_H
