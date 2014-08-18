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
#ifndef PAGEGUI_H
#define PAGEGUI_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QLabel>
#include "page.h"

#define PAGEITEM_ID QGraphicsItem::UserType + 33001
#define PAGEBORDER_ID QGraphicsItem::UserType + 33002

#define PAGE_BORDER 10
#define PAGE_SHADOW 3

class QGraphicsScene;
class QGraphicsView;
class QGraphicsRectItem;

namespace CuteReport {
class BaseItemInterface;
class ItemInterfaceView;
}

class PageItem;
class PageView;
class GraphicsView;
class Magnets;

class PageGUI : public QObject
{
    Q_OBJECT
public:
    explicit PageGUI(Page *page = 0);
    ~PageGUI();

    Page * page() { return m_page;}

    CuteReport::PageViewInterface *createView(QWidget *parent = 0);
    QList <CuteReport::PageViewInterface*> views();

    bool canSceneContainItemAt(QPoint scenePos, CuteReport::BaseItemInterface * item);

    QList<QLabel*> statusBarLabels();

    void itemAdded(CuteReport::BaseItemInterface *item);
    void updateItem(CuteReport::BaseItemInterface * item, bool withChildren = true);
    void updateItems();
    void updateChildItems();

    void redrawPageBorder();
    void redrawPageGrid();
    void redrawPageMargin();
//    void redrawItemViews();
    //    void deleteView(PageViewInterface *view);

    QList<CuteReport::BaseItemInterface *> selectedItems();
    void setSelectedItems(QList<CuteReport::BaseItemInterface *> selected);
    CuteReport::BaseItemInterface * currentItem();
    void setCurrentItem(CuteReport::BaseItemInterface *item);

    void setItemAjustedGeometry(CuteReport::BaseItemInterface *item, const QRectF &geometry);
    Magnets * magnets() const {return m_magnets;}

    void setIgnoreObjectSelection(bool b);
//    qreal zoom();
//    void setZoom(qreal zoom);

    QGraphicsItem * pageItem();
    
signals:
    void activeObjectChanged(QObject * object);
    
public slots:
    void slotDropItem(QString className, QPointF pagePos);
    void itemBeforeDestroyed(CuteReport::BaseItemInterface *item);

private slots:
    void viewDestroyed(QObject *view);
//    void viewDestroyed(PageView *view);
    void slotPaperSizeChanged(QSizeF size);
    void slotMarginChanged(CuteReport::Margins margins);
    void slotMousePressed(QMouseEvent *event, QPointF scenePos);
    void slotMouseReleased(QMouseEvent *event, QPointF scenePos);
    void slotMouseDoubleClicked(QMouseEvent *event, QPointF scenePos);
    void slotItemGeometryChanged(QRectF rect);
    void slotDPIChanged(int dpi);
    void slotBackgroundChanged(QColor color);
    void updateScene();

private:
    bool isItemSelected(CuteReport::BaseItemInterface * item);
    void setSelected(CuteReport::BaseItemInterface * item);
    void addToSelection(CuteReport::BaseItemInterface * item);
    void removeFromSelection(CuteReport::BaseItemInterface * item);
    void clearSelection();

//    int unitToPixelsX(qreal value);
//    int unitToPixelsY(qreal value);
//    void redrawItemView(CuteReport::ItemInterface* item);

private:
    Page * m_page;
    QGraphicsScene * m_scene;
    PageItem * m_pageItem;
    QGraphicsRectItem * m_pageMarginItem;
    QList<CuteReport::BaseItemInterface *> m_selectedItems;
    Magnets * m_magnets;
    bool m_ignoreObjectSelection;

    QPointer<QLabel> m_posLabel;
    QPointer<QLabel> m_geometryLabel;

    friend class Magnets;
    friend class PageView;
    friend class GraphicsView;
};


class PageItem : public QGraphicsRectItem
{
public:
//    explicit PageItem(QGraphicsItem *parent = 0);
    enum { Type = PAGEITEM_ID };

    int type() const
    {
        return Type;
    }


private:
//    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
};

// FIXME: set event filter and depricate PageView

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(Page * page, PageGUI * pageGui);

signals:
    void dropItem(QString classname, QPointF pos);
    void mouseReleased(QMouseEvent * event,  QPointF scenePos);
    void mousePressed (QMouseEvent * event,  QPointF scenePos);
    void mouseDoubleClicked (QMouseEvent * event,  QPointF scenePos);

protected:
    virtual void dragMoveEvent ( QDragMoveEvent * event );
    virtual void dropEvent(QDropEvent *event);
    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void mouseReleaseEvent (QMouseEvent * event);
    virtual void mouseMoveEvent ( QMouseEvent * event );
    virtual void mouseDoubleClickEvent(QMouseEvent * event);
    virtual void keyPressEvent ( QKeyEvent * event );

private:
    Page * m_page;
    PageGUI * m_pageGui;
};


class PageView: public CuteReport::PageViewInterface
{
    Q_OBJECT
public:
    explicit PageView(Page * page, PageGUI * pageGui, QWidget * parent = 0, Qt::WindowFlags f = 0);
    virtual void fit();

    GraphicsView * graphicsView() {return m_view;}
private:
    PageGUI * m_pageGui;
    GraphicsView * m_view;
};


//} //namespace
#endif // PAGEGUI_H
