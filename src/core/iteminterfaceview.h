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
#ifndef ITEMINTERFACEVIEW_H
#define ITEMINTERFACEVIEW_H

#include <QGraphicsObject>
#include "globals.h"

//static int ItemInterfaceViewType = QGraphicsItem::UserType + 345680;

namespace CuteReport
{

class BaseItemInterface;
class BaseItemInterfacePrivate;

class CUTEREPORT_EXPORTS ItemInterfaceView: public QGraphicsRectItem
{
public:
    enum { Type = UserType + 101 };

    explicit ItemInterfaceView(BaseItemInterface * item);
    virtual ~ItemInterfaceView();

    BaseItemInterface * coreItem() const;

    void paint(QPainter * painter, const QStyleOptionGraphicsItem *option, BaseItemInterfacePrivate *data = 0);

    int type() const { return Type; }

protected:
    CuteReport::BaseItemInterfacePrivate *ptr();

    BaseItemInterface * m_item;
};

}

Q_DECLARE_INTERFACE(CuteReport::ItemInterfaceView, "CuteReport.ItemInterfaceView/1.0")
#endif // ITEMINTERFACEVIEW_H
