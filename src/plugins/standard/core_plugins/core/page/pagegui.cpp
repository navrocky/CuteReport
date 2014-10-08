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
#include "pagegui.h"
#include "reportcore.h"
#include "iteminterface.h"
#include "iteminterfaceview.h"
#include "bandinterface.h"
#include "pageitemselection.h"
#include "magnets.h"
#include "emptydialog.h"
#include "scene.h"
#include "view.h"

#include <QLabel>
#include <QGraphicsSceneMouseEvent>
#include <QGridLayout>
#include <QScrollBar>


SUIT_BEGIN_NAMESPACE

using namespace CuteReport;

PageGUI::PageGUI(Page * page) :
    QObject(page),
    m_page(page),
    m_pageItem(0),
    m_pageMarginItem(0),
    m_ignoreObjectSelection(false),
    m_posLabel(0),
    m_geometryLabel(0)

  //    m_zoom(1.0)
{
    m_scene = new Scene(m_page, this);

    m_pageItem = new PageItem();
    m_pageItem->setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);
    m_pageItem->setBrush(QBrush(page->background()));

    m_scene->addItem(m_pageItem);
    m_scene->setBackgroundBrush(QBrush(QColor(Qt::gray)));

    m_magnets = new Magnets(this);

    updateScene();

    connect (m_scene, SIGNAL(mousePressed(QGraphicsSceneMouseEvent*)), this, SLOT(slotMousePressed(QGraphicsSceneMouseEvent*)));
    connect (m_scene, SIGNAL(mouseReleased(QGraphicsSceneMouseEvent*)), this, SLOT(slotMouseReleased(QGraphicsSceneMouseEvent*)));
    connect (m_scene, SIGNAL(mouseDoubleClicked(QGraphicsSceneMouseEvent*)), this, SLOT(slotMouseDoubleClicked(QGraphicsSceneMouseEvent*)));

    connect(m_page, SIGNAL(paperSizeChanged(QSizeF)), this, SLOT(slotPaperSizeChanged(QSizeF)));
    connect(m_page, SIGNAL(marginChanged()), this, SLOT(slotMarginChanged()));
    connect(m_page, SIGNAL(dpiChanged(int)), this, SLOT(slotDPIChanged(int)));
    connect(m_page, SIGNAL(unitChanged(CuteReport::Unit)), this, SLOT(slotUnitChanged(CuteReport::Unit)));
    connect(m_page, SIGNAL(backgroundChanged(QColor)), this, SLOT(slotBackgroundChanged(QColor)));
}


PageGUI::~PageGUI()
{
    delete m_geometryLabel;
    delete m_posLabel;
    delete m_magnets;
    delete m_scene;
}


void PageGUI::updateScene()
{
    QPoint borderPixels = convertUnit(QPointF(PAGE_BORDER, PAGE_BORDER), CuteReport::Millimeter, CuteReport::Pixel, m_page->dpi()).toPoint();
    QSize  paperSizePixels = m_page->paperSize(CuteReport::Pixel).toSize();

    m_pageItem->setPos(borderPixels);
    m_pageItem->setRect(QRect(QPoint(0,0), paperSizePixels));

    m_scene->setSceneRect(QRectF(0,0, paperSizePixels.width() + borderPixels.x()*2 , paperSizePixels.height() + borderPixels.y()*2 ));

    //    redrawPageBorder();
    redrawPageMargin();
    //    drawPageGrid();
    //    updateItems();
    emit sceneUpdated();
}


PageViewInterface *PageGUI::createView()
{
    PageView * newView = new PageView(m_page, this);
    return newView;
}


PageViewInterface *PageGUI::createSimpleView()
{
    PageView * newView = new PageView(m_page, this);
    return newView;
}


QList<PageViewInterface *> PageGUI::views()
{
    return findChildren<PageViewInterface*>();
}


bool PageGUI::canSceneContainItemAt(QPoint scenePos, BaseItemInterface *item)
{
#if QT_VERSION < 0x050000
    QGraphicsItem * currentItem = m_scene->itemAt(scenePos);
#else
    QGraphicsItem * currentItem = m_scene->itemAt(scenePos, QTransform());
#endif

    if (!currentItem)
        return false;
    if (currentItem == m_pageItem && qobject_cast<BandInterface*>(item))
        return true;

    return false;
}


void PageGUI::viewDestroyed(QObject * view)
{
    Q_UNUSED(view);
}


QList<QLabel*> PageGUI::statusBarLabels()
{
    QList<QLabel*> list;

    if (!m_posLabel) {
        m_posLabel = new QLabel();
        m_posLabel->setMinimumWidth(130);
    }

    if (!m_geometryLabel) {
        m_geometryLabel = new QLabel();
        m_geometryLabel->setMinimumWidth(190);
    }

    list.append(m_posLabel.data());
    list.append(m_geometryLabel.data());
    return list;
}


void PageGUI::itemBeforeDestroyed(BaseItemInterface * item)
{
    if (m_selectedItems.contains(item))
        m_selectedItems.removeAll(item);
    m_scene->removeItem(item->view());
    delete item->view();
}


void PageGUI::itemAdded(BaseItemInterface * item)
{    
    //    updateItem(item);
    connect (item, SIGNAL(geometryChanged(QRectF)), this, SLOT(slotItemGeometryChanged(QRectF)));
    updateItems();
}


void PageGUI::updateItem(BaseItemInterface *item, bool withChildren)
{
    //    ItemInterfaceView * view = item->view();
    //        QGraphicsItem* parentItem = item->parentItem() ? (QGraphicsItem*)item->parentItem()->view() : (QGraphicsItem*)m_pageItem;

    //    view->setParentItem(parentItem);

    item->check_gui();
    item->update_gui();

    if (m_selectedItems.contains(item)) {
        ItemSelection * selection = item->findChild<ItemSelection*>();
        selection->updateGeometry();
    }

    if (currentItem() == item && m_geometryLabel) {
        QRectF rect = item->geometry();
        m_geometryLabel->setText(QString("rect(%1, %2, %3, %4)").arg(rect.x(),4,'f',2,'0').arg(rect.y(),4,'f',2,'0')
                                 .arg(rect.height(),4,'f',2,'0').arg(rect.width(),4,'f',2,'0'));
    }


    if (withChildren)
        foreach (CuteReport::BaseItemInterface* item, item->findChildren<CuteReport::BaseItemInterface*>()) {
            updateItem(item, true);
        }

}


void PageGUI::updateItems()
{
    foreach (CuteReport::BaseItemInterface* item, m_page->items())
        updateItem(item);
}


void PageGUI::updateChildItems()
{
    foreach (CuteReport::BaseItemInterface* item, m_page->items())
        if (item->parentItem() == 0)
            updateItem(item, true);
}


QList<BaseItemInterface *> PageGUI::selectedItems()
{
    return m_selectedItems;
}


void PageGUI::setSelectedItems(QList<BaseItemInterface *> selected)
{
    // for keep sequence by first appending messed items
    BaseItemInterface * currentItem = m_selectedItems.count() ? m_selectedItems.at(0) : 0;
    foreach (BaseItemInterface * item, selected) {
        if (!m_selectedItems.contains(item))
            addToSelection(item);
    }

    foreach (BaseItemInterface * item, m_selectedItems) {
        if (!selected.contains(item))
            removeFromSelection(item);
    }

    // current item should be first
    if (currentItem && m_selectedItems.contains(currentItem) && m_selectedItems.at(0) != currentItem) {
        m_selectedItems.removeOne(currentItem);
        m_selectedItems.insert(0, currentItem);
    }

    if (!selected.count() && m_geometryLabel)
        m_geometryLabel->setText("");

}


BaseItemInterface *PageGUI::currentItem()
{
    return m_selectedItems.count() ? m_selectedItems.at(0) : 0;
}


void PageGUI::setCurrentItem(BaseItemInterface * item)
{
    if (m_selectedItems.contains(item)) {
        m_selectedItems.removeOne(item);
        m_selectedItems.insert(0, item);
    } else {
        addToSelection(item);
    }

    if (m_geometryLabel) {
        if (item) {
            QRectF rect = item->geometry();
            m_geometryLabel->setText(QString("rect(%1, %2, %3, %4)").arg(rect.x(),4,'f',2,'0').arg(rect.y(),4,'f',2,'0')
                                     .arg(rect.height(),4,'f',2,'0').arg(rect.width(),4,'f',2,'0'));
        } else
            m_geometryLabel->setText("");
    }

    emit activeObjectChanged(item);

}


void PageGUI::setItemAjustedAbsoluteGeometryMM(BaseItemInterface * item, const QRectF & geometry)
{
    QRectF newGeometry = geometry;
    if (m_page->useGrid()) {
        qreal pageStepMM =  convertUnit(m_page->gridStep(), m_page->unit(), Millimeter, m_page->dpi());
        newGeometry.setLeft(floor(geometry.left() / pageStepMM + pageStepMM/2) * pageStepMM);
        newGeometry.setTop(floor(geometry.top() / pageStepMM + pageStepMM/2) * pageStepMM);
        newGeometry.setRight(floor(geometry.right() / pageStepMM + pageStepMM/2) * pageStepMM);
        newGeometry.setBottom(floor(geometry.bottom() / pageStepMM + pageStepMM/2) * pageStepMM);
    }
    item->setAbsoluteGeometry(newGeometry, Millimeter);
}


void PageGUI::setIgnoreObjectSelection(bool b)
{
    m_ignoreObjectSelection = b;
}


QGraphicsItem *PageGUI::pageItem()
{
    return m_pageItem;
}


void PageGUI::redrawPageMargin()
{
    if (!m_pageItem)
        return;

    if (!m_pageMarginItem) {
        m_pageMarginItem = new QGraphicsRectItem(m_pageItem);
        m_pageMarginItem->setPen(QPen(QColor("#DDDDDD")));
    }

    //Margins margin = m_page->margins(CuteReport::Millimeter);
    QRectF rect = QRectF(QPointF(0,0), m_page->paperSize(CuteReport::Millimeter));

    rect.setTopLeft(rect.topLeft() + QPointF(m_page->marginLeft(Millimeter), m_page->marginTop(Millimeter)));
    rect.setBottomRight(rect.bottomRight() - QPointF(m_page->marginRight(Millimeter), m_page->marginBottom(Millimeter)));

    QRectF pixRect = convertUnit(rect, Millimeter, Pixel, page()->dpi());
    m_pageMarginItem->setPos(pixRect.topLeft());
    m_pageMarginItem->setRect(0,0, pixRect.width(), pixRect.height());
}


void PageGUI::redrawPageBorder()
{
    /*
    QGraphicsEffect * oldEffect = m_pageItem->graphicsEffect();
    if (oldEffect) {
        delete oldEffect;
    }

    QGraphicsDropShadowEffect * effect = new QGraphicsDropShadowEffect(this);
    effect->setOffset(m_page->mapToPixel(QPointF(PAGE_SHADOW, PAGE_SHADOW)));
    effect->setBlurRadius(mm2pixel(PAGE_SHADOW, m_page->dpi()));
    m_pageItem->setGraphicsEffect(effect);
    */
}


void PageGUI::redrawPageGrid()
{
    {
        QPen p;
        p.setColor(QColor(150,150,255));
        p.setWidth(1);

        for (int i=0;i<m_pageItem->boundingRect().width();i+=5)
        {
            QGraphicsLineItem * line= new QGraphicsLineItem(m_pageItem);
            line->setPen(p);
            line->setLine(i,0,i,m_pageItem->boundingRect().height());
        }
        for (int i=0;i<m_pageItem->boundingRect().height();i+=5)
        {
            QGraphicsLineItem * line= new QGraphicsLineItem(m_pageItem);
            line->setPen(p);
            line->setLine(0,i,m_pageItem->boundingRect().width(),i);
        }
    }
}


void PageGUI::slotPaperSizeChanged(QSizeF size)
{
    Q_UNUSED(size)
    qDebug("Extended::PageGUI::slotFormatChanged");
    updateScene();
}


void PageGUI::slotMarginChanged()
{
    redrawPageMargin();
}


void PageGUI::slotMousePressed(QGraphicsSceneMouseEvent *event)
{
//    m_magnets->clear();

    if (m_ignoreObjectSelection) {
        m_ignoreObjectSelection = false;
        return;
    }

    QPointF pagePos =  convertUnit(event->scenePos(), Pixel, page()->unit(), page()->dpi()) - convertUnit(QPointF(PAGE_BORDER, PAGE_BORDER), CuteReport::Millimeter, m_page->unit(), m_page->dpi());
    BaseItemInterface * item = m_page->itemAt(pagePos);

    qDebug() << "pagePos" << pagePos;


    if (event->modifiers()&Qt::ShiftModifier) {
        if (isItemSelected(item)) {
            if (m_selectedItems.count() > 1)
                removeFromSelection(item);
        } else {
            addToSelection(item);
        }
        if (m_selectedItems.count() > 1)
            // TODO: group selection properties instead of 0
            item = 0;
    } else
        setSelected(item);

    if (item)
        emit activeObjectChanged(item);
    else
        emit activeObjectChanged(m_page);
}


void PageGUI::slotMouseReleased(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
    m_magnets->clear();
}


void PageGUI::slotMouseDoubleClicked(QGraphicsSceneMouseEvent * event)
{
    Q_UNUSED(event)
    QPointF pagePos =  convertUnit(event->scenePos(), Pixel, page()->unit(), page()->dpi()) - convertUnit(QPointF(PAGE_BORDER, PAGE_BORDER), CuteReport::Millimeter, m_page->unit(), m_page->dpi());
    BaseItemInterface * item = m_page->itemAt(pagePos);

    if (!item)
        return;

    BaseItemHelperInterface * helper = item->helper();

    if (!helper)
        return;

    EmptyDialog d(m_page->reportCore()->rootWidget());
    d.setWindowTitle(QString("%1 (%2)").arg(item->moduleShortName(), item->suitName()));
    d.setHelperWidget(helper);
    if (d.exec() == QDialog::Accepted)
        helper->sync();
}


void PageGUI::slotDropItem(QString className, QPointF pagePos)
{
    m_page->addItem(className, pagePos);
}


bool PageGUI::isItemSelected(BaseItemInterface * item)
{
    return m_selectedItems.contains(item);
}


void PageGUI::setSelected(BaseItemInterface * item)
{
    clearSelection();
    addToSelection(item);
}


void PageGUI::addToSelection(BaseItemInterface * item)
{
    if (!item)
        return;
    m_selectedItems.insert(0, item);
    /*ItemSelection * selecter =*/ new SUIT_NAMESPACE::ItemSelection(this, item);
}


void PageGUI::removeFromSelection(BaseItemInterface * item)
{
    ItemSelection * sel = item->findChild<ItemSelection*>();
    Q_ASSERT(sel);
    delete sel;
    m_selectedItems.removeAll(item);
}


void PageGUI::clearSelection()
{
    foreach (BaseItemInterface * item, m_selectedItems) {
        ItemSelection * sel = item->findChild<ItemSelection*>();
        Q_ASSERT(sel);
        delete sel;
    }
    m_selectedItems.clear();
}


void PageGUI::slotItemGeometryChanged(QRectF rect)
{
    Q_UNUSED(rect);
    CuteReport::BaseItemInterface * item = qobject_cast<CuteReport::BaseItemInterface *>(sender());

    if (item) {
        this->updateItem(item);
    }
}


void PageGUI::slotDPIChanged(int dpi)
{
    Q_UNUSED(dpi)
    updateScene();
}


void PageGUI::slotBackgroundChanged(QColor color)
{
    m_pageItem->setBrush(color);
}


void PageGUI::slotUnitChanged(Unit unit)
{
    Q_UNUSED(unit)
    updateScene();
}


PageView::PageView(Page * page, PageGUI * pageGui, QWidget * parent, Qt::WindowFlags f)
    :PageViewInterface(parent, f),
      m_pageGui(pageGui)
{
    m_view = new View(page, pageGui);
    m_view->setScene(pageGui->m_scene);
    m_view->centerOn( 0, 0 );
    m_view->setRenderHints(/*QPainter::Antialiasing |*/ QPainter::SmoothPixmapTransform);
    m_view->setMouseTracking(true);

    connect (m_view, SIGNAL(dropItem(QString,QPointF)), pageGui, SLOT(slotDropItem(QString,QPointF)), Qt::QueuedConnection);
    connect (this, SIGNAL(destroyed(QObject*)), pageGui, SLOT(viewDestroyed(QObject*)));

    QLayout * layout = new QGridLayout(this);
    layout->setMargin(0);
    layout->addWidget(m_view);
    this->setLayout(layout);
}


void PageView::fit()
{
    m_view->horizontalScrollBar()->hide();
    m_view->verticalScrollBar()->hide();
    qreal xScale =  m_view->viewport()->width() / m_pageGui->m_scene->sceneRect().width();
    qreal yScale = m_view->viewport()->height() / m_pageGui->m_scene->sceneRect().height();
    qreal resultScale = qMin(xScale, yScale);
    m_view->resetTransform();
    m_view->scale(resultScale, resultScale);
}


View *PageView::graphicsView()
{
     return m_view;
}


SUIT_END_NAMESPACE
