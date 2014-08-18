/***************************************************************************
 *   This file is part of the CuteReport project                           *
 *   Copyright (C) 2012 by Alexander Mikhalov                              *
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
#ifndef SELECTER_H
#define SELECTER_H
#include <QList>
#include "pageitemselection.h"

//class GraphicsItemGroup;
//class QGraphicsScene;
class Page;

namespace Report
{
	class ItemInterface;
	class ReportEngine;
//	class PageInterface;
}

struct Item
{
    Report::ItemInterface * item;
    QPointF pos;
    ItemSelection * sel;
};

using namespace Report;

class Selecter: public QObject
{
    Q_OBJECT
public:
    explicit Selecter(Page * page);
    ~Selecter();

    QObject * itemSelected(Report::ItemInterface * item, QPointF pos = QPointF(), Qt::KeyboardModifiers keys = Qt::NoModifier);
    QObject * itemSelected(QObject * object, QPointF pos = QPointF(), Qt::KeyboardModifiers keys = Qt::NoModifier);

    void updateSelection();

    QRectF rect();
    QPointF pos();
    void setPos(QPointF pos);

    QObject * activeObject();
    QPointF activeObjectLastPressPos();

    void store();
    void restore();
    void reset();

    bool hasSelection();
    QList<Report::ItemInterface *> selectedItems();
    int selectedItemsCount();

	void alignItemsLeft();
	void alignItemsRight();
	void alignItemsTop();
	void alignItemsBottom();
	void alignItemsHeight();
	void alignItemsWidth();

signals:
	void activeObjectChanged(QObject*, QObject*); //new, old

private:
    void append (Report::ItemInterface * item);
    void remove (Report::ItemInterface * item);
    void free();

private:
    QObject * _itemSelected(Report::ItemInterface * item, QPointF pos, Qt::KeyboardModifiers keys);
    void setGuideItem(Report::ItemInterface * item);
    QRectF _rect();

private slots:
    void itemGeometryChanged(QObject* item, QRectF, QRectF);
    void guideGeometryChanged(QObject* item, QRectF newRect, QRectF oldRect);

private:
    QList<Item> items;
    QList<Item> storedItems;
    QObject  *m_activeObject, *m_prevActiveObject;
    QPointF m_activeObjectPressPos;
    Page * m_page;
    Report::ItemInterface * m_guide;
    QPointF m_guideLastPos;
    QRectF m_boundingRect;
    bool needRectRefresh;
};


//class GraphicsItemGroup: public QGraphicsItemGroup
//{
//    protected:
//    void paint ( QPainter * /*painter*/, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/) {}
//};


#endif // SELECTER_H
