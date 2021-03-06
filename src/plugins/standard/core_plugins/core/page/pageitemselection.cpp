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
#include "pageitemselection.h"
#include "page.h"
#include "pagegui.h"
#include "iteminterfaceview.h"
#include "bandinterface.h"
#include "reportcore.h"
#include "functions.h"
#include "magnets.h"

#include <QtGui>
#include <QGraphicsSceneMouseEvent>

#define BOUND 0
#define SIZE 8

//#define DEBUG

using namespace CuteReport;

ItemHandle::ItemHandle(Type t, ItemSelection *s) :
    QGraphicsItem (s),
    m_itemView(0),
    m_type(t),
    m_sel(s),
    m_active(true)
{
    //    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    updateCursor();
}


void ItemHandle::updateCursor()
{
    if (!m_active) {
        setCursor(Qt::ArrowCursor);
        return;
    }

    switch (m_type) {
    case LeftTop:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case Top:
        setCursor(Qt::SizeVerCursor);
        break;
    case RightTop:
        setCursor(Qt::SizeBDiagCursor);
        break;
    case Right:
        setCursor(Qt::SizeHorCursor);
        break;
    case RightBottom:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case Bottom:
        setCursor(Qt::SizeVerCursor);
        break;
    case LeftBottom:
        setCursor(Qt::SizeBDiagCursor);
        break;
    case Left:
        setCursor(Qt::SizeHorCursor);
        break;
    default:
        Q_ASSERT(0);
    }
}


void ItemHandle::setActive(bool a)
{
    m_active = a;

    if (a)
        setFlags(/*QGraphicsItem::ItemIsMovable |*/ QGraphicsItem::ItemIsSelectable);
    else
        setFlags(0);

    updateCursor();
}


void ItemHandle::setItem(CuteReport::ItemInterfaceView *i)
{
    m_itemView = i;
}


void ItemHandle::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //    painter->setPen( Qt::black);
    //    painter->drawRect(0, 0, SIZE - 1, SIZE - 1);
    if (m_active)
        painter->fillRect( 0, 0, SIZE - 1, SIZE - 1, QBrush(QColor("#096BFF")) );
    else
        painter->fillRect( 0, 0, SIZE - 1, SIZE - 1, QBrush(Qt::black) );
}


QRectF ItemHandle::boundingRect () const
{
    return QRectF(0,0,SIZE,SIZE);
}


void ItemHandle::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsItem::mousePressEvent(e );

    if (!(m_itemView && e->button() == Qt::LeftButton))
        return;

    if (!(m_active))
        return;

    m_origPressPos = e->scenePos();
    m_geom = m_origGeom = QRectF (m_itemView->pos(), m_itemView->boundingRect().size());

    // do not use magnets for bands
    m_useMagnets = !qobject_cast<CuteReport::BandInterface*>(m_itemView->coreItem());
}


void ItemHandle::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsItem::mouseMoveEvent( e );
    if (!(m_itemView && m_active && e->buttons() & Qt::LeftButton))
        return;

    const QPointF rp = e->scenePos();
    const QPointF d = rp - m_origPressPos;

    switch (m_type) {

    case LeftTop: {
        m_geom.setTopLeft(m_origGeom.topLeft() + d);
        QRectF rect = convertUnit(m_geom, Pixel, m_itemView->coreItem()->page()->unit(), m_itemView->coreItem()->page()->dpi());
        QRectF orig = m_itemView->coreItem()->geometry();

        // TODO: optimize
        if (m_useMagnets) {
            QRectF absoluteGeometry = m_itemView->coreItem()->absoluteGeometry();
            absoluteGeometry.setTopLeft(absoluteGeometry.topLeft() + (rect.topLeft() - orig.topLeft()));
            QList<QPointF> pointsList;
            QList<CuteReport::BaseItemInterface*> excludeItems;
            pointsList << absoluteGeometry.topLeft();
            excludeItems << m_itemView->coreItem();
            QPointF point = m_sel->pageGUI()->magnets()->delta(pointsList, excludeItems, Magnets::HVDirection);

            orig.setTopLeft(rect.topLeft() + point);
        } else
            orig.setTopLeft(rect.topLeft());

        m_sel->pageGUI()->setItemAjustedGeometry(m_itemView->coreItem(), orig);

    } break;

    case Top: {
        m_geom.setTop(m_origGeom.top() + d.y());
        QRectF rect = convertUnit(m_geom, Pixel, m_itemView->coreItem()->page()->unit(), m_itemView->coreItem()->page()->dpi());
        QRectF orig = m_itemView->coreItem()->geometry();

        // TODO: optimize
        if (m_useMagnets) {
            QRectF absoluteGeometry = m_itemView->coreItem()->absoluteGeometry();
            absoluteGeometry.setTop(absoluteGeometry.top() + (rect.top() - orig.top()));
            QList<QPointF> pointsList;
            QList<CuteReport::BaseItemInterface*> excludeItems;
            pointsList << absoluteGeometry.topLeft();
            excludeItems << m_itemView->coreItem();
            QPointF point = m_sel->pageGUI()->magnets()->delta(pointsList, excludeItems, Magnets::VDirection);

            orig.setTop(rect.top()+ point.y());
        } else
            orig.setTop(rect.top());

        m_sel->pageGUI()->setItemAjustedGeometry(m_itemView->coreItem(), orig);
    } break;

    case RightTop: {
        m_geom.setTopRight(m_origGeom.topRight() + d);
        QRectF rect = convertUnit(m_geom, Pixel, m_itemView->coreItem()->page()->unit(), m_itemView->coreItem()->page()->dpi());
        QRectF orig = m_itemView->coreItem()->geometry();

        // TODO: optimize
        if (m_useMagnets) {
            QRectF absoluteGeometry = m_itemView->coreItem()->absoluteGeometry();
            absoluteGeometry.setTopRight(absoluteGeometry.topRight() + (rect.topRight() - orig.topRight()));
            QList<QPointF> pointsList;
            QList<CuteReport::BaseItemInterface*> excludeItems;
            pointsList << absoluteGeometry.topRight();
            excludeItems << m_itemView->coreItem();
            QPointF point = m_sel->pageGUI()->magnets()->delta(pointsList, excludeItems, Magnets::HVDirection);

            orig.setTopRight(rect.topRight()+ point);
        } else
            orig.setTopRight(rect.topRight());

        m_sel->pageGUI()->setItemAjustedGeometry(m_itemView->coreItem(), orig);

    } break;

    case Right: {
        m_geom.setWidth(m_origGeom.width() + d.x());
        QRectF rect = convertUnit(m_geom, Pixel, m_itemView->coreItem()->page()->unit(), m_itemView->coreItem()->page()->dpi());
        QRectF orig = m_itemView->coreItem()->geometry();

        // TODO: optimize
        if (m_useMagnets) {
            QRectF absoluteGeometry = m_itemView->coreItem()->absoluteGeometry();
            absoluteGeometry.setRight(absoluteGeometry.right() + (rect.right() - orig.right()));
            QList<QPointF> pointsList;
            QList<CuteReport::BaseItemInterface*> excludeItems;
            pointsList << absoluteGeometry.bottomRight();
            excludeItems << m_itemView->coreItem();
            QPointF point = m_sel->pageGUI()->magnets()->delta(pointsList, excludeItems, Magnets::HDirection);

            orig.setRight(rect.right()+ point.x());
        } else
            orig.setRight(rect.right());

        m_sel->pageGUI()->setItemAjustedGeometry(m_itemView->coreItem(), orig);    } break;

    case RightBottom: {
        m_geom.setBottomRight(m_origGeom.bottomRight() + d);
        QRectF rect = convertUnit(m_geom, Pixel, m_itemView->coreItem()->page()->unit(), m_itemView->coreItem()->page()->dpi());
        QRectF orig = m_itemView->coreItem()->geometry();

        // TODO: optimize
        if (m_useMagnets) {
            QRectF absoluteGeometry = m_itemView->coreItem()->absoluteGeometry();
            absoluteGeometry.setBottomRight(absoluteGeometry.bottomRight() + (rect.bottomRight() - orig.bottomRight()));
            QList<QPointF> pointsList;
            QList<CuteReport::BaseItemInterface*> excludeItems;
            pointsList << absoluteGeometry.bottomRight();
            excludeItems << m_itemView->coreItem();
            QPointF point = m_sel->pageGUI()->magnets()->delta(pointsList, excludeItems, Magnets::HVDirection);

            orig.setBottomRight(rect.bottomRight() +point);
        } else
            orig.setBottomRight(rect.bottomRight());

        m_sel->pageGUI()->setItemAjustedGeometry(m_itemView->coreItem(), orig);
    } break;

    case Bottom: {
        m_geom.setHeight(m_origGeom.height() + d.y());
        QRectF rect = convertUnit(m_geom, Pixel, m_itemView->coreItem()->page()->unit(), m_itemView->coreItem()->page()->dpi());
        QRectF orig = m_itemView->coreItem()->geometry();

        // TODO: optimize
        if (m_useMagnets) {
            QRectF absoluteGeometry = m_itemView->coreItem()->absoluteGeometry();
            absoluteGeometry.setBottom(absoluteGeometry.bottom() + (rect.bottom() - orig.bottom()));
            QList<QPointF> pointsList;
            QList<CuteReport::BaseItemInterface*> excludeItems;
            pointsList << absoluteGeometry.bottomLeft();
            excludeItems << m_itemView->coreItem();
            QPointF point = m_sel->pageGUI()->magnets()->delta(pointsList, excludeItems, Magnets::VDirection);

            orig.setBottom(rect.bottom()+ point.y());
        } else
            orig.setBottom(rect.bottom());

        m_sel->pageGUI()->setItemAjustedGeometry(m_itemView->coreItem(), orig);
    } break;

    case LeftBottom: {
        m_geom.setBottomLeft(m_origGeom.bottomLeft()+ d);
        QRectF rect = convertUnit(m_geom, Pixel, m_itemView->coreItem()->page()->unit(), m_itemView->coreItem()->page()->dpi());
        QRectF orig = m_itemView->coreItem()->geometry();

        // TODO: optimize
        if (m_useMagnets) {
            QRectF absoluteGeometry = m_itemView->coreItem()->absoluteGeometry();
            absoluteGeometry.setBottomLeft(absoluteGeometry.bottomLeft() + (rect.bottomLeft() - orig.bottomLeft()));
            QList<QPointF> pointsList;
            QList<CuteReport::BaseItemInterface*> excludeItems;
            pointsList << absoluteGeometry.bottomLeft();
            excludeItems << m_itemView->coreItem();
            QPointF point = m_sel->pageGUI()->magnets()->delta(pointsList, excludeItems, Magnets::HVDirection);

            orig.setBottomLeft(rect.bottomLeft() + point);
        } else
            orig.setBottomLeft(rect.bottomLeft());

        m_sel->pageGUI()->setItemAjustedGeometry(m_itemView->coreItem(), orig);
    } break;

    case Left: {
        m_geom.setLeft(m_origGeom.left() + d.x());
        QRectF rect = convertUnit(m_geom, Pixel, m_itemView->coreItem()->page()->unit(), m_itemView->coreItem()->page()->dpi());
        QRectF orig = m_itemView->coreItem()->geometry();

        // TODO: optimize
        if (m_useMagnets) {
            QRectF absoluteGeometry = m_itemView->coreItem()->absoluteGeometry();
            absoluteGeometry.setLeft(absoluteGeometry.left() + (rect.left() - orig.left()));
            QList<QPointF> pointsList;
            QList<CuteReport::BaseItemInterface*> excludeItems;
            pointsList << absoluteGeometry.bottomLeft();
            excludeItems << m_itemView->coreItem();
            QPointF point = m_sel->pageGUI()->magnets()->delta(pointsList, excludeItems, Magnets::HDirection);

            orig.setLeft(rect.left()+ point.x());
        } else
            orig.setLeft(rect.left());

        m_sel->pageGUI()->setItemAjustedGeometry(m_itemView->coreItem(), orig);

        //        orig.setLeft(rect.left());
        //        m_sel->pageGUI()->setItemAjustedGeometry(m_itemView->coreItem(), orig);
    } break;

    default: break;

    } // end switch

}


void ItemHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsItem::mouseReleaseEvent( e );
    m_sel->pageGUI()->setIgnoreObjectSelection(true);
    if (e->button() != Qt::LeftButton || !m_active)
        return;
}



// ------------------ ItemSelection

ItemSelection::ItemSelection(PageGUI * pageGUI, BaseItemInterface *item)
    :m_item(item),
    m_pageGui(pageGUI)

{
    //    setAcceptedMouseButtons(0);
    for (int i = ItemHandle::LeftTop; i < ItemHandle::TypeCount; ++i)
        m_handles[i] = new ItemHandle(static_cast<ItemHandle::Type>(i), this);

    setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemClipsToShape, false);

    updateActive();
    updateGeometry();

    setParent(item);
    setZValue(100);
    setParentItem(m_pageGui->pageItem());

    connect(item, SIGNAL(dpiChanged(int)), this, SLOT(updateGeometry()));
}


ItemSelection::~ItemSelection()
{
}


void ItemSelection::updateActive()
{

    int flag = m_item->resizeFlags();

    bool active[ItemHandle::TypeCount];
    qFill(active, active + ItemHandle::TypeCount, false);

    //    qDebug("fixed = %i", flag&CuteReport::ItemInterface::Fixed);
    //    qDebug("top = %i", flag&CuteReport::ItemInterface::ResizeBottom);
    //    qDebug("bottom = %i", flag&CuteReport::ItemInterface::ResizeTop);

    active[ItemHandle::Left] = flag&CuteReport::BaseItemInterface::ResizeLeft;
    active[ItemHandle::Right] = flag&CuteReport::BaseItemInterface::ResizeRight;
    active[ItemHandle::Top] = flag&CuteReport::BaseItemInterface::ResizeTop;
    active[ItemHandle::Bottom] = flag&CuteReport::BaseItemInterface::ResizeBottom;

    active[ItemHandle::LeftTop] = flag&CuteReport::BaseItemInterface::ResizeTop && flag&CuteReport::BaseItemInterface::ResizeLeft;
    active[ItemHandle::RightTop] = flag&CuteReport::BaseItemInterface::ResizeTop && flag&CuteReport::BaseItemInterface::ResizeRight;
    active[ItemHandle::RightBottom] = flag&CuteReport::BaseItemInterface::ResizeBottom && flag&CuteReport::BaseItemInterface::ResizeRight;
    active[ItemHandle::LeftBottom] = flag&CuteReport::BaseItemInterface::ResizeBottom && flag&CuteReport::BaseItemInterface::ResizeLeft;

    /*
    qFill(active, active + ItemHandle::TypeCount, false);
    // Determine active handles
    switch (is) {
    case UnlaidOut:
    qFill(active, active + ItemHandle::TypeCount, true);
        break;
    case ManagedGridLayout: // Grid: Allow changing span
    active[ItemHandle::Left] = active[ItemHandle::Top] = active[ItemHandle::Right] = active[ItemHandle::Bottom] = true;
        break;
    case ManagedFormLayout:  // Form: Allow changing column span
        if (const unsigned operation = ChangeFormLayoutItemRoleCommand::possibleOperations(m_formWindow->core(), m_widget)) {
        active[ItemHandle::Left]  = operation & (ChangeFormLayoutItemRoleCommand::SpanningToField|ChangeFormLayoutItemRoleCommand::FieldToSpanning);
        active[ItemHandle::Right] = operation & (ChangeFormLayoutItemRoleCommand::SpanningToLabel|ChangeFormLayoutItemRoleCommand::LabelToSpanning);
        }
        break;
    default:
        break;
    }
*/
    for (int i = ItemHandle::LeftTop; i < ItemHandle::TypeCount; ++i)
        if (ItemHandle *h = m_handles[i]) {
            h->setItem(m_item->view());
            h->setActive(active[i]);
        }
}


void ItemSelection::updateGeometry()
{
    if (!m_item->view())
        return;

    prepareGeometryChange();

    m_boundingRect = m_item->absoluteGeometry(Pixel);
    setPos(QPointF ( m_boundingRect.left() - BOUND, m_boundingRect.top() - BOUND ) );
    const QRectF r(QPointF(BOUND,BOUND), m_boundingRect.size() + QSize(1,1));

    m_boundingRect = QRectF(0 ,0 , m_boundingRect.width() + 2*BOUND, m_boundingRect.height() + 2*BOUND);

    update();

    const int w = SIZE;
    const int h = SIZE;

    for (int i = ItemHandle::LeftTop; i < ItemHandle::TypeCount; ++i) {
        ItemHandle *hndl = m_handles[ i ];
        if (!hndl)
            continue;
        switch (i) {
        case ItemHandle::LeftTop:
            hndl->setPos(r.x() - w / 2, r.y() - h / 2);
            break;
        case ItemHandle::Top:
            hndl->setPos(r.x() + r.width() / 2 - w / 2, r.y() - h / 2);
            break;
        case ItemHandle::RightTop:
            hndl->setPos(r.x() + r.width() - w / 2, r.y() - h / 2);
            break;
        case ItemHandle::Right:
            hndl->setPos(r.x() + r.width() - w / 2, r.y() + r.height() / 2 - h / 2);
            break;
        case ItemHandle::RightBottom:
            hndl->setPos(r.x() + r.width() - w / 2, r.y() + r.height() - h / 2);
            break;
        case ItemHandle::Bottom:
            hndl->setPos(r.x() + r.width() / 2 - w / 2, r.y() + r.height() - h / 2);
            break;
        case ItemHandle::LeftBottom:
            hndl->setPos(r.x() - w / 2, r.y() + r.height() - h / 2);
            break;
        case ItemHandle::Left:
            hndl->setPos(r.x() - w / 2, r.y() + r.height() / 2 - h / 2);
            break;
        default:
            break;
        }
    }
}

QRectF ItemSelection::boundingRect () const
{
    return m_boundingRect;
}


void ItemSelection::update()
{
    for (int i = ItemHandle::LeftTop; i < ItemHandle::TypeCount; ++i) {
        ItemHandle *h = m_handles[ i ];
        if (h)
            h->update();
    }
}


CuteReport::ItemInterfaceView *ItemSelection::item() const
{
    return m_item->view();
}


void ItemSelection::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

#ifndef DEBUG
    Q_UNUSED(painter)

    //    painter->save();

    //    painter->translate( boundingRect().center());
    //    painter->rotate( m_itemView->rotate() );
    //    painter->translate( -m_itemView->boundingRect().center());

    //    m_itemView->paint(painter, option, widget);
    //    foreach (QGraphicsItem * item, m_itemView->childItems())
    //    {
    //	painter->save();
    ////	painter->translate( item->boundingRect().center());
    ////	painter->rotate( item->rotate() );
    ////	painter->translate( -item->boundingRect().center());
    //	item->paint(painter, option, widget);
    //	painter->restore();
    //    }
    //    painter->restore();

#else
    painter->save();
    painter->setOpacity( 0.3 );
    painter->fillRect( this->boundingRect(), QBrush(Qt::red));
    painter->restore();
#endif
}


void ItemSelection::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    m_origPressPos = convertUnit(e->scenePos(), Pixel, m_item->unit(), m_item->dpi());
    m_geom = m_origGeom = m_item->absoluteGeometry();
    m_moved = false;
}


void ItemSelection::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    if (m_item->resizeFlags() & BaseItemInterface::FixedPos)
        return;

    m_moved = true;
    const QPointF rp = convertUnit(e->scenePos(), Pixel, m_item->unit(), m_item->dpi());
    const QPointF d = rp - m_origPressPos;
    m_geom = m_origGeom.translated(d);

    QList<CuteReport::BaseItemInterface*> excludeItems;
    excludeItems << m_item;
    QList<QPointF> pointsList;
    pointsList << m_geom.bottomLeft() << m_geom.bottomRight() << m_geom.topLeft() << m_geom.topRight();

    QPointF correctionDelta = m_pageGui->magnets()->delta(pointsList, excludeItems, Magnets::HVDirection);
    m_geom = m_geom.translated(correctionDelta);

    qreal unit = m_pageGui->page()->gridStep();
    m_geom.moveLeft(floor(m_geom.left() / unit) * unit);
    m_geom.moveTop(floor(m_geom.top() / unit) * unit);

    m_item->setAbsoluteGeometry(m_geom);
}


void ItemSelection::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    Q_UNUSED(e)
    if (m_moved) {
        QPointF controlPoint = convertUnit(e->scenePos(), Pixel, m_pageGui->page()->unit(), m_pageGui->page()->dpi()) -
                convertUnit(QPointF(PAGE_BORDER, PAGE_BORDER), Millimeter, m_pageGui->page()->unit(), m_pageGui->page()->dpi());

        BaseItemInterface * parentItem = 0;

        QList<BaseItemInterface *> items = m_item->page()->itemsAt(controlPoint);
        for (int i=items.size()-1; i>=0; --i) {
            if (items[i] != m_item && items[i]->canContain(m_item)) {
                parentItem = items[i];
                break;
            }
        }


        if (parentItem  && !(m_item->findChildren<CuteReport::BaseItemInterface*>().contains(parentItem))) {
            QRectF absoluteGeometry = m_item->absoluteGeometry();
            m_item->setParentItem(parentItem);
            m_item->setAbsoluteGeometry(absoluteGeometry);
        }
        m_pageGui->setIgnoreObjectSelection(true);
    }
}

