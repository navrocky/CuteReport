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
#ifndef LAYOUTMANAGER_H
#define LAYOUTMANAGER_H

#include <QtCore>
#include <QtGui>

//{

namespace CuteReport
{
class BandInterface;
class BaseItemInterface;
}

class Page;

typedef QMap<int, CuteReport::BandInterface*> BandMap;
typedef QList<CuteReport::BaseItemInterface *> ItemList;
typedef QList<CuteReport::BandInterface *> BandList;

class LayoutManager : public QObject
{
public:
//    explicit LayoutManager(PageInterface * page);

    static void itemAdded(Page * page, CuteReport::BaseItemInterface *item);
    static void updatePositions(Page *page);
    static void itemOrderChanged(Page * page, CuteReport::BaseItemInterface * item);
    static void bandOrderChanged(Page * page, CuteReport::BandInterface * band);
    static bool splitOnLayoutTypes(Page * page, BandMap * listTop, BandMap * listBottom, BandMap * listFree);
    static BandMap sortByPriority(BandList lc);
    static BandMap sortByPriority(ItemList lc);
    static BandList sortByOrder(BandList lc);


//    static void ItemDelete(ItemInterface * item/*, QObject * parent*/);
//    static void itemGeometryChanged(QObject * item);
//    static void itemChangeOrder(QObject * item, int order, int oldOrder, int bandMargin = 0);
//    static void updatePositions(QObject * item /*, int bandMargin = 0*/);

//    static bool splitOnLayoutTypes(QObject * item, BandMap * listTop, BandMap * listBottom, BandMap * listFree);
//    static bool splitOnLayoutTypesSorted(QObject * item, BandList * listTop, BandList * listBottom, BandList * listFree);
//    static BandMap sortByPriority(BandList lc);
//    static BandMap sortByPriority(ItemList lc);
//    static BandList sortByOrder(BandList lc);

//    static int margin();
//    static void setMargin(int value);
private:
//    PageInterface * m_page;
};

//}
#endif // LAYOUTMANAGER_H