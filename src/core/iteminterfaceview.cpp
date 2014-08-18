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
#include "iteminterfaceview.h"
#include "iteminterface.h"
#include "pageinterface.h"
#include <QtCore>
#include <QtGui>

using namespace CuteReport;

ItemInterfaceView::ItemInterfaceView(BaseItemInterface *item)
    :m_item(item)
{
    update();
}


ItemInterfaceView::~ItemInterfaceView()
{
}


BaseItemInterface *ItemInterfaceView::coreItem() const
{
    return m_item;
}


//void ItemInterfaceView::setRect(QRectF rect)
//{
//    if (m_rect == rect)
//        return;

//    m_rect = rect;
//    prepareGeometryChange();
//    update();
//    emit rectChanged(m_rect);
//}


//QRectF ItemInterfaceView::rect() const
//{
//    return m_rect;
//}


//QRectF ItemInterfaceView::boundingRect() const
//{
//    return QRectF(QPoint(0,0), m_rect.bottomRight());
//}


//void ItemInterfaceView::updateView()
//{
//    QRect rect = m_item->page()->mapToPixel(coreItem()->geometry());
//    if (m_rect != rect) {
//        m_rect = rect;
//        setPos(m_rect.topLeft());
//        prepareGeometryChange();
//    }
//    QGraphicsObject::update();
//}


void ItemInterfaceView::paint(QPainter * painter, const QStyleOptionGraphicsItem *option, BaseItemInterfacePrivate *data)
{
    BaseItemInterface::paintBegin(painter, option, data ? data : coreItem()->d_ptr, boundingRect(), RenderingTemplate);
    BaseItemInterface::paintEnd(painter, option, data ? data : coreItem()->d_ptr, boundingRect(), RenderingTemplate);
}


BaseItemInterfacePrivate * ItemInterfaceView::ptr()
{
    return coreItem()->d_ptr;
}
