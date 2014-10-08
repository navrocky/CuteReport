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
#ifndef ITEMSELECTION_H
#define ITEMSELECTION_H

#include "iteminterface.h"
#include "plugins_common.h"

#include <QtCore/QHash>
#include <QtCore/QPointer>
#include <QGraphicsItem>

class QMouseEvent;
class QPaintEvent;

SUIT_BEGIN_NAMESPACE
class ItemSelection;
class PageGUI;
class ItemHandle;
class ItemSelection;
SUIT_END_NAMESPACE

USING_SUIT_NAMESPACE

SUIT_BEGIN_NAMESPACE
class ItemHandle: public QGraphicsItem
{
public:
    enum Type
    {
        LeftTop,
        Top,
        RightTop,
        Right,
        RightBottom,
        Bottom,
        LeftBottom,
        Left,

        TypeCount
    };

    ItemHandle(Type t, ItemSelection *s);
    void setItem(CuteReport::ItemInterfaceView *i);
    void setActive(bool a);
    void updateCursor();

    void setEnabled(bool) {}

    QRectF boundingRect () const;

protected:
    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

private:
    CuteReport::ItemInterfaceView *m_itemView;
    const Type m_type;
    QPointF m_origPressPos;
    ItemSelection *m_sel;
    QRectF m_geom, m_origGeom;
    bool m_active;
    bool m_useMagnets;
    QList<CuteReport::BaseItemInterface*> m_checkingItems;
};


class ItemSelection: public QGraphicsObject
{
    Q_OBJECT
public:
    ItemSelection(PageGUI * pageGUI, CuteReport::BaseItemInterface * item);
    ~ItemSelection();


    CuteReport::ItemInterfaceView *item() const;
    PageGUI * pageGUI() {return m_pageGui;}

    QRectF boundingRect () const;

public slots:
    void updateActive();
    void updateGeometry();
    void update();

protected:
    virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *e);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e);

private:
    ItemHandle *m_handles[ItemHandle::TypeCount];
    CuteReport::BaseItemInterface * m_item;
    QRectF m_boundingRect;
    QPointF m_origPressPos;
    QRectF m_geom, m_origGeom;
    bool m_moved;
    PageGUI * m_pageGui;
    QList<CuteReport::BaseItemInterface*> m_checkingItems;
};


SUIT_END_NAMESPACE

#endif // ITEMSELECTION_H
