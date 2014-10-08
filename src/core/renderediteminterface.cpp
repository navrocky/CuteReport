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
#include "renderediteminterface.h"
#include "renderediteminterface_p.h"
#include "iteminterface_p.h"
#include "pageinterface.h"


using namespace CuteReport;

RenderedItemInterface::RenderedItemInterface(BaseItemInterface *item, BaseItemInterfacePrivate *itemPrivateData)
    : d_ptr(itemPrivateData)/*, m_item(item)*/
{
    if (d_ptr->selfRendering)
        d_ptr->initRenderingData();

    // rect now is absolute page geometry in MM
    QPointF absPos = item->absoluteGeometry(Millimeter).topLeft();
    d_ptr->rect.translate(absPos);

    redraw();
}


RenderedItemInterface::~RenderedItemInterface()
{
    delete d_ptr;
}


quint32 RenderedItemInterface::id() const
{
    return m_id;
}


void RenderedItemInterface::setId(quint32 id)
{
    m_id = id;
}


//void RenderedItemInterface::setParentItem(RenderedItemInterface *parent)
//{
//    qDebug() << "setParentItem";
//}


void RenderedItemInterface::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    paintBegin(painter, option, widget);
    paintEnd(painter, option, widget);
}


void RenderedItemInterface::redraw(bool withChildren)
{
    QRectF itemPixelRect = convertUnit(d_ptr->rect, Millimeter, Pixel, d_ptr->dpi);

#if QT_VERSION >= 0x050000
    itemPixelRect.setBottom(itemPixelRect.bottom() -1);
    itemPixelRect.setRight(itemPixelRect.right() -1);
#endif

    QPointF absPos = absolutePixelPos();
    QPointF pos;
    if (parentItem()) {
        pos = parentItem()->mapFromScene(absPos);
        //        qDebug() << "parentPos" << parentItem()->pos();
    } else {
        pos = absPos;
    }
    //    qDebug() << "pos" << pos;
    setRect(0,0, itemPixelRect.width(), itemPixelRect.height());

    setRotation(-d_ptr->rotation);

    QPointF transPos = BaseItemInterface::transformedPos(d_ptr, QRectF(pos, itemPixelRect.size()));
    setPos(transPos);

    if (withChildren) {
        QList<QGraphicsItem *> list = childItems();
        foreach (QGraphicsItem * item, list) {
            if (item->parentItem() == this && item->type() == RenderedItemInterface::Type) {
                RenderedItemInterface * rendItem = static_cast<RenderedItemInterface *>(item);
                rendItem->redraw(true);
            }
        }
    }
}


void RenderedItemInterface::paintBegin(QPainter * painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    ItemInterface::paintBegin(painter, option, d_ptr, boundingRect(), RenderingReport);
}


void RenderedItemInterface::paintEnd(QPainter * painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    ItemInterface::paintEnd(painter, option, d_ptr, boundingRect(), RenderingReport);
}


QRectF RenderedItemInterface::absoluteGeometry(Unit unit) const
{
    if (!d_ptr)
        return QRectF();
    Unit u = (unit == UnitNotDefined) ? d_ptr->unit : unit;
    return convertUnit(d_ptr->rect, Millimeter, u, d_ptr->dpi);

//    QGraphicsItem * parent = parentItem();
//    QRectF geometry = convertUnit(d_ptr->rect, d_ptr->unit, u, d_ptr->dpi);

//    while (parent) {
//        if (parent->type() == RenderedPageInterface::Type) {
//            RenderedItemInterface * parentItem = reinterpret_cast<RenderedItemInterface *>(parent);
//            if (parentItem)
//                geometry.translated(-parentItem->absoluteGeometry(Millimeter).topLeft());
//        }
//        parent = parent->parentItem();
//    }

//    return geometry;
}


void RenderedItemInterface::setAbsoluteGeometry(const QRectF & geometry, Unit unit)
{
    if (!d_ptr)
        return;
    Unit u = (unit == UnitNotDefined) ? d_ptr->unit : unit;
    d_ptr->rect = convertUnit(geometry, u, Millimeter, d_ptr->dpi);
}


QPointF RenderedItemInterface::absolutePixelPos() const
{
    QGraphicsItem * parent = parentItem();
    QPointF itemPixelpos = convertUnit(d_ptr->rect.topLeft(), d_ptr->unit, Pixel, d_ptr->dpi);

    while (parent) {
        if (parent->type() == RenderedPageInterface::Type) {
            //            QPointF pageAbsPos = parent->mapToScene(QPointF(0,0));
            QPointF itemMapped = parent->mapToScene(itemPixelpos);
            return itemMapped;
        }
        parent = parent->parentItem();
    }

    return itemPixelpos;
}


QRectF RenderedItemInterface::absoluteBoundingRect(Unit unit) const
{
    if (!d_ptr)
        return QRectF();
    Unit u = (unit == UnitNotDefined) ? d_ptr->unit : unit;

    QTransform trans;
    trans.rotate(d_ptr->rotation);
    int delta = d_ptr->rect.height() * sin(d_ptr->rotation * 3.14159265 /180);
    trans.translate(delta,0);

    QRectF rect = QRectF(QPointF(0,0), QSizeF(d_ptr->rect.size()));
    QRectF resRect = QRectF(d_ptr->rect.topLeft(), trans.mapRect(rect).size());
    return convertUnit(resRect, Millimeter, u, d_ptr->dpi);
}


int RenderedItemInterface::dpi()
{
    return d_ptr->dpi;
}


void RenderedItemInterface::setDpi(int dpi, bool withChildren)
{
    d_ptr->dpi = dpi;

    if (withChildren) {
        QList<QGraphicsItem *> list = childItems();
        foreach (QGraphicsItem * item, list) {
            if (item->parentItem() == this && item->type() == RenderedItemInterface::Type) {
                RenderedItemInterface * rendItem = static_cast<RenderedItemInterface *>(item);
                rendItem->setDpi(dpi, true);
            }
        }
    }
}
