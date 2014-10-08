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
#include "QtCore"
#include "layoutmanager.h"
#include "page.h"
#include "iteminterface.h"
#include "bandinterface.h"

using namespace CuteReport;


SUIT_BEGIN_NAMESPACE


int layoutMargin = 0;

void LayoutManager::updatePositions(Page * page)
{
    Q_ASSERT(page);

    BandMap listTop;
    BandMap listBottom;
    BandMap listFree;

    if (!LayoutManager::splitOnLayoutTypes(page, &listTop, &listBottom, &listFree))
        return;

    QRectF activeArea = page->pageRect();

    QList<int> pList;

    qreal yPos = activeArea.top();
    pList = listTop.uniqueKeys();

    for (int i = pList.count()-1; i>=0 ;i--)
    {
        BandList orderedList = sortByOrder(listTop.values(pList.at(i)));

        for (int j = 0; j<orderedList.count() ;j++)
        {
            BandInterface * band = orderedList.at(j);
//            int flags = band->resizeFlags();
            QRectF geometry = band->geometry();
            QRectF newGeometry;

            newGeometry.setX(activeArea.x());
            newGeometry.setY(yPos);
            newGeometry.setWidth(activeArea.width());
            newGeometry.setHeight(geometry.height());

            /* causes problem while user plays with format size. So keep size unchangable.
             *
            if (newGeometry.bottom() > activeArea.bottom() && newGeometry.top() < activeArea.bottom())
                newGeometry.setBottom(activeArea.bottom());
            */
            band->setGeometry(newGeometry);

            yPos += newGeometry.height();
        }
    }

    yPos = activeArea.bottom();
    pList = listBottom.uniqueKeys();

    for (int i = pList.count()-1; i>=0 ;i--)
    {
        BandList orderList = sortByOrder(listBottom.values(pList.at(i)));
        for (int j = orderList.count() -1; j>=0 ;j--)
        {
            BandInterface * band = orderList.at(j);
//            int flags = band->resizeFlags();
            QRectF geometry = band->geometry();
            QRectF newGeometry;

            newGeometry.setX(activeArea.x());
            newGeometry.setY(yPos - geometry.height());
            newGeometry.setWidth(activeArea.width());
            newGeometry.setHeight(geometry.height());

            if (newGeometry.top() < activeArea.top())
                newGeometry.setTop(activeArea.top());

//            band->blockSignals(true);
            band->setGeometry(newGeometry);
//            band->blockSignals(false);

            yPos -= newGeometry.height();
        }
    }
}

bool itemOrderLessThan(BaseItemInterface * i1, BaseItemInterface * i2)
{
    return i1->order() < i2->order();
}

void LayoutManager::itemOrderChanged(Page * page, BaseItemInterface *item)
{
     CuteReport::BandInterface* band = qobject_cast<CuteReport::BandInterface*>(item);
     if (band)
         return bandOrderChanged(page, band);

     BaseItemInterface * parent = item->parentItem();
     QList<BaseItemInterface *> items = page->items();
     QList<BaseItemInterface*> list;

     foreach(BaseItemInterface * iItem, items) {
         CuteReport::BandInterface* iBand = qobject_cast<CuteReport::BandInterface*>(iItem);
         if (!iBand && iItem != item && iItem->parentItem() == parent)
             list.append(iItem);
     }

     qSort(list.begin(), list.end(), itemOrderLessThan);

     int curOrder=0;
     foreach(BaseItemInterface * iItem, list) {
         if (curOrder == item->order())
             ++curOrder;
         iItem->blockSignals(true);
         iItem->setOrder(curOrder);
         iItem->blockSignals(false);

        ++curOrder;
     }

     // magic number = max order
     if (item->order() == 999) {
         item->blockSignals(true);
         item->setOrder(curOrder);
         item->blockSignals(false);
     }
}




void LayoutManager::bandOrderChanged(Page * page, BandInterface * band)
{
    Q_ASSERT(band);

    if (band->layoutType() == BandInterface::LayoutFree)
        return;

    QList<BaseItemInterface *> items = page->items();
    QList<BandInterface *> bands;
    QMap<int, BandInterface*> orders;
    for (int i = 0;i < items.size();i++) {
        CuteReport::BandInterface* iBand = qobject_cast<CuteReport::BandInterface*>(items[i]);
        if (iBand) {
            bands.append(iBand);
            orders.insert(iBand->order(), iBand);
        }
    }

    int max_order = 0;
    int order = band->order();
    int oldOrder;
    for (int i = 0;i < INT_MAX;i++) {
        if (!orders.contains(i)) {
            oldOrder = i;
            break;
        }
    }


    for (int i = 0;i < bands.size();i++)
    {
        CuteReport::BandInterface* iBand = bands[i];
        if (iBand && iBand->layoutType() == band->layoutType() && iBand->layoutPriority() == band->layoutPriority() && band != iBand)
        {
            max_order++;
            if (order != INT_MAX)
            {
                if (order < oldOrder) {
                    if (iBand->order() >= order && iBand->order() < oldOrder) {
                        iBand->blockSignals(true);
                        iBand->setOrder(iBand->order() + 1);
                        iBand->blockSignals(false);
                    }
                }
                else
                    if (iBand->order() <= order && iBand->order() > oldOrder && oldOrder >= 0) {
                        iBand->blockSignals(true);
                        iBand->setOrder(iBand->order() - 1);
                        iBand->blockSignals(false);
                    }
            }
        }
    }

    if (order < 0)
        order = 0;
    if (order > max_order)
        order = max_order;

    band->blockSignals(true);
    band->setOrder(order);
    band->blockSignals(false);

    updatePositions( page );
}


bool LayoutManager::splitOnLayoutTypes(Page* page, BandMap * listTop, BandMap * listBottom, BandMap * listFree)
{
    QList<BaseItemInterface *> lc = page->items();

    if (lc.isEmpty())
        return false;

    for (int i = 0;i < lc.size();i++)
    {
        CuteReport::BandInterface * iBand;
        if (qobject_cast<CuteReport::BandInterface*>(lc[i]))
            iBand = qobject_cast<CuteReport::BandInterface*>(lc[i]);
        else
            continue;

        switch (iBand->layoutType())
        {
        case BandInterface::LayoutTop:
            listTop->insertMulti(iBand->layoutPriority(), iBand);
            break;
        case BandInterface::LayoutBottom:
            listBottom->insertMulti(iBand->layoutPriority(), iBand);
            break;
        case BandInterface::LayoutFree:
            listFree->insertMulti(iBand->layoutPriority(), iBand);
            break;
        case BandInterface::LayoutUndefined:
            return false;
        }
    }
    return true;

}


BandMap LayoutManager::sortByPriority(ItemList lc)
{
    BandMap nList;
    for (int i = 0;i < lc.size();i++)
    {
        CuteReport::BandInterface * iBand;

        if (dynamic_cast<CuteReport::BandInterface*>(lc[i]))
            iBand = dynamic_cast<CuteReport::BandInterface*>(lc[i]);
        else
            continue;
        nList.insert(iBand->layoutPriority(), iBand);
    }
    return nList;
}

BandMap LayoutManager::sortByPriority(BandList lc)
{
    BandMap nList;
    for (int i = 0;i < lc.size();i++)
        nList.insert(lc[i]->layoutPriority(), lc[i]);
    return nList;
}

BandList LayoutManager::sortByOrder(BandList lc)
{
    BandMap nList;
    for (int i = 0;i < lc.size();i++)
        nList.insertMulti(lc[i]->order(), lc[i]);
    return nList.values();
}


void LayoutManager::itemAdded(Page * page, BaseItemInterface * item)
{
    Q_ASSERT(item);
    Q_ASSERT(page);

    BandInterface * band = qobject_cast<CuteReport::BandInterface*>(item);
    if (!band || band->layoutType() == BandInterface::LayoutFree)
        return;

    QList<CuteReport::BaseItemInterface *> itemList = page->items();
    qreal deltaY = 0;
    int maxOrder = -1;

    for (int i = 0;i < itemList.size();i++)
    {
        CuteReport::BandInterface * iBand = qobject_cast<CuteReport::BandInterface*>(itemList[i]);

        if (!iBand || band == iBand || band->parent() != iBand->parent() || band->layoutType() != iBand->layoutType())
            continue;

            switch (band->layoutType()) {
            case BandInterface::LayoutTop:
                if (iBand->layoutPriority() > band->layoutPriority()) {
                    deltaY += iBand->geometry().height();
                } else if (iBand->layoutPriority() == band->layoutPriority()) {
                    deltaY += iBand->geometry().height();
                    maxOrder = qMax( iBand->order(), maxOrder);
                } else {
                    QRectF geometry = iBand->geometry();
                    geometry.translate(0, band->geometry().height());

                    iBand->blockSignals(true);
                    iBand->setGeometry(geometry);
                    iBand->blockSignals(false);
                }

                break;

            case BandInterface::LayoutBottom:
                if (iBand->layoutPriority() >= band->layoutPriority())
                    deltaY += /*iBand->indentation() +*/ iBand->geometry().height();
                else {
                    QRectF geometry = iBand->geometry();
                    geometry.translate(0, -band->geometry().height());
                    band->blockSignals(true);
                    iBand->setGeometry(geometry);
                    band->blockSignals(false);
                }
                if (iBand->layoutPriority() == band->layoutPriority())
                    iBand->setOrder(iBand->order() + 1);
                break;
            default:
                break;
            }
    }

    if (band->layoutType() == BandInterface::LayoutTop)  {
        band->setGeometry(QRectF(page->pageRect().x(), page->pageRect().y() + deltaY, band->geometry().width(), band->geometry().height()));
        band->setOrder(maxOrder + 1);
    }

    if (band->layoutType() == BandInterface::LayoutBottom) {
        band->setGeometry(QRectF(page->pageRect().x(), page->pageRect().y() +page->pageRect().height() - band->geometry().height() - deltaY,
                                 band->geometry().width(), band->geometry().height()));
        band->setOrder(0);
    }

    if (band->resizeFlags()|BandInterface::ResizeRight) {
        QRectF rect = band->geometry();
        rect.setWidth(page->pageRect().width());
        band->setGeometry(rect);
    }

}


SUIT_END_NAMESPACE
