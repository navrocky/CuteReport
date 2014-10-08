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

#include "renderedmemoitem.h"
#include "memo.h"
#include "memo_p.h"
#include "rendererpublicinterface.h"


SUIT_BEGIN_NAMESPACE


RenderedMemoItem::RenderedMemoItem(CuteReport::BaseItemInterface * item, CuteReport::BaseItemInterfacePrivate *itemPrivateData)
    :RenderedItemInterface(item, itemPrivateData)
{
}


RenderedMemoItem::~RenderedMemoItem()
{

}


void RenderedMemoItem::paint(QPainter * painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    MemoItem::paint(painter, option, d_ptr, boundingRect(), CuteReport::RenderingReport);
}


void RenderedMemoItem::redraw(bool withChildren)
{
    MemoItemPrivate * d = reinterpret_cast<MemoItemPrivate*>(d_ptr);
    d->absoluteRect = absoluteGeometry(CuteReport::Millimeter);
    MemoItem::adjust(d, QPointF());
    RenderedItemInterface::redraw(withChildren);
}


SUIT_END_NAMESPACE
