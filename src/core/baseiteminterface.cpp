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
#include "baseiteminterface.h"
#include "baseiteminterface_p.h"
#include "iteminterfaceview.h"
#include "renderediteminterface.h"
#include "pageinterface.h"
#include "reportinterface.h"
#include "bandinterface.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>

namespace CuteReport
{

BaseItemInterface::BaseItemInterface(QObject * parent) :
    ReportPluginInterface(parent),
    d_ptr(new BaseItemInterfacePrivate),
    orig_ptr(0)
{
    initMe();
}


BaseItemInterface::BaseItemInterface(BaseItemInterfacePrivate *dd, QObject * parent)
    :ReportPluginInterface(parent),
      d_ptr(dd),
      orig_ptr(0)
{
    initMe();
}


void BaseItemInterface::setRenderingPointer(BaseItemInterfacePrivate *r)
{
    Q_ASSERT(!orig_ptr);
    //delete orig_ptr;
    orig_ptr = d_ptr;
    d_ptr = r;
    d_ptr->renderingType = RenderingReport;
}


void BaseItemInterface::replaceRenderingPointer()
{
    if (!orig_ptr)
        return;
    delete d_ptr;
    d_ptr = orig_ptr;
    orig_ptr = 0;
}


BaseItemInterface::~BaseItemInterface()
{
    delete d_ptr;
    delete orig_ptr;
    qDeleteAll(dataStack);
}


void BaseItemInterface::moduleInit()
{

}


void BaseItemInterface::initMe()
{
    Q_D(BaseItemInterface);
    m_gui = 0;
    d->resizeFlags = ResizeTop | ResizeBottom | ResizeLeft | ResizeRight;
    m_inited = false;
    m_currentProperty = -1;
    m_page = 0;
    m_parentItem = 0;
}


void BaseItemInterface::init()
{
    if (m_inited)
        return;

    m_parentItem = qobject_cast<CuteReport::BaseItemInterface*>(parent());
    m_page = m_parentItem ? BaseItemInterface::itemPage(m_parentItem) : qobject_cast<CuteReport::PageInterface*>(parent());

    updateMeassure();

    m_inited = true;
}


void BaseItemInterface::check_gui()
{
    if (!m_gui)
        init_gui();
}


void BaseItemInterface::update_gui()
{
    if (!m_gui)
        return;

    QRectF itemPixelRect = convertUnit(absoluteGeometry(), d_ptr->unit, Pixel, d_ptr->dpi);

#if QT_VERSION >= 0x050000
    itemPixelRect.setBottom(itemPixelRect.bottom() -1);
    itemPixelRect.setRight(itemPixelRect.right() -1);
#endif

    QGraphicsItem * pageItem = page() ? page()->pageItem() : 0;
    QGraphicsItem * parentGuiItem = parentItem() ? (QGraphicsItem*)parentItem()->view() : pageItem;
    m_gui->setParentItem( parentGuiItem );

    QPointF itemPixelpos = itemPixelRect.topLeft();
    QPointF absPos = pageItem ? pageItem->mapToScene(itemPixelpos) : itemPixelpos;
    QPointF pos = parentGuiItem ? parentGuiItem->mapFromScene(absPos) : absPos;

    m_gui->setRect(0,0, itemPixelRect.width(), itemPixelRect.height());
    m_gui->setZValue(order());

    m_gui->setRotation(-d_ptr->rotation);

    QPointF transPos = BaseItemInterface::transformedPos(d_ptr, QRectF(pos, itemPixelRect.size()));
    m_gui->setPos(transPos);

    m_gui->update();
}


BaseItemInterface *BaseItemInterface::clone(bool withChildren, bool init) const
{
    BaseItemInterface * newItem = this->itemClone();
    newItem->setReportCore(this->reportCore());
    newItem->setObjectName(this->objectName());

    if (withChildren) {
        foreach (CuteReport::BaseItemInterface* child, findChildren<BaseItemInterface * >()) {
            if (child->parent() != this)     // only direct children
                continue;
            CuteReport::BaseItemInterface * childCopy = child->clone(withChildren, init);
            childCopy->setParent(newItem);
        }
    }
    if  (init)
        newItem->init();

    return newItem;
}


PageInterface * BaseItemInterface::page() const
{
    return m_page;
}


void BaseItemInterface::setPage(PageInterface * page)
{
    Q_D(BaseItemInterface);
    if (!QObject::parent())
        QObject::setParent(page);
    m_page = page;
    d->unit = m_page->unit();
    d->dpi = m_page->dpi();
    update_gui();
}


BaseItemInterface * BaseItemInterface::parentItem() const
{
    return m_parentItem;
}


void BaseItemInterface::setParentItem(BaseItemInterface *parentItem)
{
    if (parentItem == m_parentItem)
        return;

    if (m_parentItem)
        m_parentItem->childRemoved(this);

    QObject::setParent(parentItem);

    m_parentItem = parentItem;
    adjustOrder();

    if (m_parentItem)
        m_parentItem->childAdded(this);

    emit parentItemChanged(m_parentItem);
    emit changed();
}


CuteReport::ReportInterface * BaseItemInterface::reportObject()
{
    QObject * object = this->parent();
    while (!dynamic_cast<CuteReport::ReportInterface*> (object) && object)
        object = object->parent();

    return dynamic_cast<CuteReport::ReportInterface*> (object);
}


int BaseItemInterface::childLevel()
{
    return m_parentItem ? m_parentItem->childLevel() + 1 : 0;
}


BandInterface *BaseItemInterface::carrierBand()
{
    if (qobject_cast<CuteReport::BandInterface*> (this))
        return static_cast<CuteReport::BandInterface*>(this);

    if (parentItem())
        return parentItem()->carrierBand();
    else
        return 0;
}


BaseItemInterface * BaseItemInterface::topmostChildAt(const QPointF & localPos)
{
    QList<BaseItemInterface *>  list = childrenAt(localPos);

    while (true) {
        QList<BaseItemInterface *>  nextLevelList;
        foreach (BaseItemInterface * child, list)
            nextLevelList.append(child->childrenAt(child->mapFromParent(localPos)));
        if (nextLevelList.size())
            list = nextLevelList;
        else
            break;
    }

    if (list.size() == 1)
        return list[0];
    else if (list.size() > 1) {
        BaseItemInterface * item = list[0];
        for (int i = 1; i<list.size(); ++i) {
            if (list[i]->order() > item->order())
                item = list[i];
        }
        return item;
    }
    return 0;
}


QList<BaseItemInterface *> BaseItemInterface::childrenAt(const QPointF & localPos)
{
    QList<BaseItemInterface *>  list;

    foreach (BaseItemInterface * const item, findChildren<BaseItemInterface *>()){
        if (item->parent() != this)
            continue;
        QRectF childGeometry = item->geometry();
        if (childGeometry.contains(localPos)) {
            list.append(item);
        }
    }
    return list;
}


QList<BaseItemInterface *> BaseItemInterface::allChildrenAt(const QPointF & localPos)
{
    QList<BaseItemInterface *>  list;
    foreach (BaseItemInterface * const item, childrenAt(localPos)){
        if (item->parent() != this) // only direct children checking
            continue;
        QList<BaseItemInterface *> nextLevelList = item->allChildrenAt(item->mapFromParent(localPos));
        if (!nextLevelList.isEmpty())
            list.append(nextLevelList);
    }
    return list;
}


bool BaseItemInterface::canContain(QObject * object)
{
    return qobject_cast<BaseItemInterface*>(object);
}


ItemInterfaceView *BaseItemInterface::view()
{
    check_gui();
    return m_gui;
}


void BaseItemInterface::updateView()
{
    check_gui();
    m_gui->update();
}


void BaseItemInterface::updateViewIfExists()
{
    if (m_gui)
        m_gui->update();
}


void BaseItemInterface::saveState()
{
    BaseItemInterfacePrivate * state = new BaseItemInterfacePrivate(*d_ptr);
    dataStack.append(state);
}


void BaseItemInterface::restoreState()
{
    if (!dataStack.isEmpty()) {
        if (d_ptr)
            delete (d_ptr);
        d_ptr = dataStack.takeLast();
    }
}


void BaseItemInterface::renderInit(RendererPublicInterface *renderer)
{
    Q_UNUSED(renderer);
    delete orig_ptr;
    orig_ptr = 0;
    emit printInit();
}


void BaseItemInterface::renderReset()
{
    emit printReset();
}


bool BaseItemInterface::renderEnd()
{
    replaceRenderingPointer();
    emit printAfter();
    return true;
}


bool BaseItemInterface::selfRendering()
{
    return d_ptr->selfRendering;
}


bool BaseItemInterface::childrenSelfRendering()
{
    return d_ptr->childrenSelfRendering;
}


int BaseItemInterface::resizeFlags() const
{
    Q_D(const BaseItemInterface);
    return d->resizeFlags;
}


void BaseItemInterface::setResizeFlags(int resizeFlags)
{
    Q_D(BaseItemInterface);
    d->resizeFlags = resizeFlags;
}


bool BaseItemInterface::enabled() const
{
    Q_D(const BaseItemInterface);
    return d->enabled;
}


void BaseItemInterface::setEnabled(bool b)
{
    Q_D(BaseItemInterface);
    if (b == d->enabled)
        return;

    d->enabled = b;

    emit enabledChanged(d->enabled);
    emit changed();
}


int BaseItemInterface::order() const
{
    Q_D(const BaseItemInterface);
    return d->order;
}


void BaseItemInterface::setOrder(int order)
{
    Q_D(BaseItemInterface);
    int oldOrder = d->order;
    d->order = order;

    if (d->order < 0)
        d->order = 0;

    if (d->order == oldOrder)
        return;

    emit orderChanged(d->order);
    emit changed();
}


void BaseItemInterface::setDpi(int dpi)
{
    Q_D(BaseItemInterface);
    if (d->dpi == dpi)
        return;

    d->dpi = dpi;
    update_gui();

    emit dpiChanged(d->dpi);
    emit boundingRectChanged(boundingRect());
    emit changed();
}


int BaseItemInterface::dpi() const
{
    Q_D(const BaseItemInterface);
    return d->dpi;
}


QByteArray BaseItemInterface::serialize()
{
    QByteArray ba;
    QDataStream s( &ba, QIODevice::ReadWrite );
    s << *d_ptr;
    return ba;
}


void BaseItemInterface::deserialize(QByteArray & data)
{
    QDataStream s( &data, QIODevice::ReadWrite );
    s >> *d_ptr;
}


void BaseItemInterface::paintBegin(QPainter * painter, const QStyleOptionGraphicsItem *option, const BaseItemInterfacePrivate * data, const QRectF &boundingRect, RenderingType type )
{
    Q_UNUSED(type);

    painter->setRenderHints(QPainter::TextAntialiasing|QPainter::Antialiasing);
#if QT_VERSION >= 0x050000
    painter->setRenderHint(QPainter::Qt4CompatiblePainting);
#endif
    const BaseItemInterfacePrivate * const d = data /*? data : d_func()*/;

    /// border pen
    QPen borderPen = penPrepared(d->borderPen, d->dpi);
    painter->setPen(Qt::NoPen);
    painter->setOpacity(d->opacity);

    /// background brush
    painter->setBrush(d->bgBrush);

    QRect rect;
    rect.setSize((option->type == QStyleOption::SO_GraphicsItem) ? boundingRect.size().toSize() : option->exposedRect.size().toSize());

    int penwidth = borderPen.width()/2;
    if (borderPen.width()&1)
        rect = rect.adjusted( penwidth, penwidth, -penwidth-1, -penwidth-1 );
    else
        rect = rect.adjusted( penwidth, penwidth, -penwidth, -penwidth );

    painter->drawRect(rect);
    painter->save();
}


void BaseItemInterface::paintEnd(QPainter * painter, const QStyleOptionGraphicsItem *option, const BaseItemInterfacePrivate * data, const QRectF &boundingRect, RenderingType type )
{
    Q_UNUSED(type);

    const BaseItemInterfacePrivate * const d = data /*? data : d_func()*/;

    painter->restore();

    /// border pen
    QPen borderPen = penPrepared(d->borderPen, d->dpi);
    painter->setPen(borderPen);
    painter->setOpacity(d->opacity);

    /// background brush
    painter->setBrush(Qt::NoBrush);

    QRect rect;
    rect.setSize((option->type == QStyleOption::SO_GraphicsItem) ? boundingRect.size().toSize() : option->exposedRect.size().toSize());

    int penwidth = borderPen.width()/2;
    if (borderPen.width()&1)
        rect = rect.adjusted( penwidth, penwidth, -penwidth-1, -penwidth-1 );
    else
        rect = rect.adjusted( penwidth, penwidth, -penwidth, -penwidth );

    int _frame = d->frame;

    if(_frame & DrawLeft && _frame & DrawRight && _frame & DrawTop && _frame & DrawBottom) {
        painter->drawRect(rect);

    } else {
        painter->save();
        painter->setPen( QPen(Qt::NoPen));
        painter->drawRect(rect);

        if (type == RenderingTemplate) {
            QPen pen(QColor(128,128,128));
            pen.setStyle(Qt::DashDotLine);
            painter->setPen(pen);
            painter->setBrush(Qt::NoBrush);
            painter->setOpacity(0.5);
            painter->drawRect(rect);
        }

        painter->restore();

        if (_frame&DrawLeft)
            painter->drawLine(rect.left(), rect.top(), rect.left(), rect.bottom());

        if (_frame&DrawRight)
            painter->drawLine(rect.right(), rect.top(), rect.right(), rect.bottom());

        if (_frame&DrawTop)
            painter->drawLine(rect.left(), rect.top(), rect.right(), rect.top());

        if (_frame&DrawBottom)
            painter->drawLine(rect.left(), rect.bottom(), rect.right(), rect.bottom());
    }
}



QRectF BaseItemInterface::mapFromPage(const QRectF & rect, Unit inputUnit, Unit outputUnit) const
{
    Unit unitIn = (inputUnit == UnitNotDefined) ? d_ptr->unit : inputUnit;
    Unit unitOut = (outputUnit == UnitNotDefined) ? d_ptr->unit : outputUnit;
    QRectF inputRectMM =  convertUnit(rect, unitIn, Millimeter, d_ptr->dpi);
    QRectF resultRect = m_parentItem ? m_parentItem->mapFromPage(inputRectMM.translated(-d_ptr->rect.topLeft()), Millimeter, Millimeter) : inputRectMM.translated(-d_ptr->rect.topLeft());
    return convertUnit(resultRect, Millimeter, unitOut, d_ptr->dpi);
}


QPointF BaseItemInterface::mapFromPage(const QPointF & point, Unit inputMeassure, Unit outputMeassure) const
{
    Unit unitIn = (inputMeassure == UnitNotDefined) ? d_ptr->unit : inputMeassure;
    Unit unitOut = (outputMeassure == UnitNotDefined) ? d_ptr->unit : outputMeassure;
    QPointF inputPointMM =  convertUnit(point, unitIn, Millimeter, d_ptr->dpi);
    QPointF resultPoint = m_parentItem ? m_parentItem->mapFromPage(inputPointMM - d_ptr->rect.topLeft(), Millimeter, Millimeter) : inputPointMM - d_ptr->rect.topLeft();
    return convertUnit(resultPoint, Millimeter, unitOut, d_ptr->dpi);
}


QRectF BaseItemInterface::mapToPage(const QRectF & rect, Unit inputMeassure, Unit outputMeassure) const
{
    Unit unitIn = (inputMeassure == UnitNotDefined) ? d_ptr->unit : inputMeassure;
    Unit unitOut = (outputMeassure == UnitNotDefined) ? d_ptr->unit : outputMeassure;
    QRectF inputRectMM =  convertUnit(rect, unitIn, Millimeter, d_ptr->dpi);
    QRectF resultRect = m_parentItem ? m_parentItem->mapToPage(inputRectMM.translated(d_ptr->rect.topLeft()), Millimeter, Millimeter) : inputRectMM.translated(d_ptr->rect.topLeft());
    return convertUnit(resultRect, Millimeter, unitOut, d_ptr->dpi);
}


QPointF BaseItemInterface::mapToPage(const QPointF & point, Unit inputMeassure, Unit outputMeassure) const
{
    Unit unitIn = (inputMeassure == UnitNotDefined) ? d_ptr->unit : inputMeassure;
    Unit unitOut = (outputMeassure == UnitNotDefined) ? d_ptr->unit : outputMeassure;
    QPointF inputPointMM =  convertUnit(point, unitIn, Millimeter, d_ptr->dpi);
    QPointF resultPoint = m_parentItem ?  m_parentItem->mapFromPage(inputPointMM + d_ptr->rect.topLeft(), Millimeter, Millimeter) : inputPointMM + d_ptr->rect.topLeft();
    return convertUnit(resultPoint, Millimeter, unitOut, d_ptr->dpi);
}


QRectF BaseItemInterface::mapToParent(const QRectF & rect, Unit inputMeassure, Unit outputMeassure) const
{
    Unit unitIn = (inputMeassure == UnitNotDefined) ? d_ptr->unit : inputMeassure;
    Unit unitOut = (outputMeassure == UnitNotDefined) ? d_ptr->unit : outputMeassure;
    QRectF inputRectMM =  convertUnit(rect, unitIn, Millimeter, d_ptr->dpi);
    QRectF resultRect = inputRectMM.translated(d_ptr->rect.topLeft());
    return convertUnit(resultRect, Millimeter, unitOut, d_ptr->dpi);
}


QPointF BaseItemInterface::mapToParent(const QPointF & point, Unit inputMeassure, Unit outputMeassure) const
{
    Unit unitIn = (inputMeassure == UnitNotDefined) ? d_ptr->unit : inputMeassure;
    Unit unitOut = (outputMeassure == UnitNotDefined) ? d_ptr->unit : outputMeassure;
    QPointF inputPointMM =  convertUnit(point, unitIn, Millimeter, d_ptr->dpi);
    QPointF resultPoint = inputPointMM + d_ptr->rect.topLeft();
    return convertUnit(resultPoint, Millimeter, unitOut, d_ptr->dpi);
}


QRectF BaseItemInterface::mapFromParent(const QRectF & rect, Unit inputMeassure, Unit outputMeassure) const
{
    Unit unitIn = (inputMeassure == UnitNotDefined) ? d_ptr->unit : inputMeassure;
    Unit unitOut = (outputMeassure == UnitNotDefined) ? d_ptr->unit : outputMeassure;
    QRectF inputRectMM =  convertUnit(rect, unitIn, Millimeter, d_ptr->dpi);
    QRectF resultRect = inputRectMM.translated(-d_ptr->rect.topLeft());
    return convertUnit(resultRect, Millimeter, unitOut, d_ptr->dpi);
}


QPointF BaseItemInterface::mapFromParent(const QPointF & point, Unit inputUnit, Unit outputUnit) const
{
    Unit unitIn = (inputUnit == UnitNotDefined) ? d_ptr->unit : inputUnit;
    Unit unitOut = (outputUnit == UnitNotDefined) ? d_ptr->unit : outputUnit;
    QPointF inputPointMM =  convertUnit(point, unitIn, Millimeter, d_ptr->dpi);
    QPointF resultPoint = inputPointMM - d_ptr->rect.topLeft();
    return convertUnit(resultPoint, Millimeter, unitOut, d_ptr->dpi);
}


QRectF BaseItemInterface::absoluteGeometry(Unit outputUnit) const
{
    return mapToPage(QRectF(QPointF(0,0), QSizeF(d_ptr->rect.size())), Millimeter, outputUnit);
}


void BaseItemInterface::setAbsoluteGeometry(const QRectF & rect, Unit inputUnit)
{
    setGeometry(m_parentItem ? m_parentItem->mapFromPage(rect, inputUnit, Millimeter) : rect, Millimeter);
}


QRectF BaseItemInterface::absoluteBoundingRect(Unit unit) const
{
    QRectF rect = QRectF(QPointF(0,0), QSizeF(d_ptr->rect.size()));
    QRectF resRect = QRectF(QPointF(0,0), transform().mapRect(rect).size());
    return mapToPage(resRect, d_ptr->unit, unit);
}


QPolygonF BaseItemInterface::absolutePolygon(Unit unit) const
{
    return transform().map(QPolygonF(absoluteGeometry(unit)));
}


CuteReport::PageInterface * BaseItemInterface::itemPage(CuteReport::BaseItemInterface * item)
{
    if (item->page())
        return item->page();
    else {
        CuteReport::BaseItemInterface * parentItem = item->parentItem();
        if (parentItem)
            return itemPage(parentItem);
    }
    return 0;
}


QFont BaseItemInterface::fontPrepared(const QFont & font, qint16 dpi)
{
    QFont f(font);
    qreal pixels = f.pointSizeF()/72*dpi;
    f.setPixelSize(pixels);
    return f;
}


QPen BaseItemInterface::penPrepared(const QPen &pen, qint16 dpi)
{
    QPen p(pen);
    int pixels = p.widthF()/72*dpi;
    p.setWidth(pixels);
    return p;
}


const QRectF BaseItemInterface::adjustRect(QRectF & rect, const QPen & pen)
{
    Q_UNUSED(pen);
    return rect;
}


void BaseItemInterface::adjustOrder()
{
    if (!parent())
        return;

    int maxOrder = 0;
    foreach (BaseItemInterface * item, parent()->findChildren<CuteReport::BaseItemInterface*>()) {
        if (item == this || item->parent() != parent())
            continue;
        if (item->order() > maxOrder)
            maxOrder = item->order();
    }

    setOrder(maxOrder+1);
}


QString BaseItemInterface::_current_property_description() const
{
    QString propertyName = metaObject()->property(m_currentProperty).name();

    if (propertyName == "geometry")
        return tr("Item geomery");
    else if (propertyName == "frame")
        return tr("Item frame drawing sides");
    else if (propertyName == "opacity")
        return tr("Item opacity");
    else if (propertyName == "rotation")
        return tr("Rotation angle");
    else if (propertyName == "borderPen")
        return tr("Border pen properties");
    else if (propertyName == "backgroundBrush")
        return tr("Border pen background properies");
    else if (propertyName == "enabled")
        return tr("Is item enabled. Disabled items wont processed");
    else if (propertyName == "order")
        return tr("position order");

    return QString();
}


int BaseItemInterface::_current_property_precision() const
{
    return 1;
}

////////////////////////////////////////// shared data

Unit BaseItemInterface::unit() const
{
    Q_D(const BaseItemInterface);
    return d->unit;
}


void BaseItemInterface::setUnit(Unit unit)
{
    Q_D(BaseItemInterface);
    if (unit == d->unit)
        return;

    d->unit = unit;

    if (m_inited) {
        update_gui();
        emit unitChanged(d->unit);
        emit changed();
    }
}


QRectF BaseItemInterface::geometry(Unit unit) const
{
    Q_D(const BaseItemInterface);
    if (m_inited) {
        Unit u = (unit == UnitNotDefined) ? d->unit : unit;
        return convertUnit(d->rect, Millimeter, u, d->dpi);
    } else {
        return d->rect;
    }
}


QRectF BaseItemInterface::boundingRect(Unit unit) const
{
    QRectF rect = QRectF(QPointF(0,0), QSizeF(d_ptr->rect.size()));
    QRectF resRect = QRectF(d_ptr->rect.topLeft(), transform().mapRect(rect).size());
    return convertUnit(resRect, d_ptr->unit, unit, d_ptr->dpi);
}


QPolygonF BaseItemInterface::polygon(Unit unit) const
{
    return transform().map(QPolygonF(geometry(unit)));
}


void BaseItemInterface::setGeometry(const QRectF &rect, Unit unit)
{
    Q_D(BaseItemInterface);
    if (!m_inited) {
        d->rect = rect;
        return;
    }

    Unit u = (unit == UnitNotDefined) ? d->unit : unit;
    QRectF newRect = convertUnit(rect, u, Millimeter, d->dpi);

    if (d->rect == newRect)
        return;

    d->rect = newRect;

    if (d->rect.width() < d->minRectSize.width())
        d->rect.setWidth(d->minRectSize.width());
    if (d->rect.height() < d->minRectSize.height())
        d->rect.setHeight(d->minRectSize.height());

    update_gui();

    emit geometryChanged(geometry());
    emit boundingRectChanged(boundingRect());
    emit changed();
}



qreal BaseItemInterface::height(Unit unit) const
{
    Q_D(const BaseItemInterface);
    if (m_inited) {
        Unit u = (unit == UnitNotDefined) ? d->unit : unit;
        return convertUnit(d->rect, Millimeter, u, d->dpi).height();
    } else {
        return d->rect.height();
    }
}


void BaseItemInterface::setHeight(qreal height, Unit unit)
{
    Q_D(BaseItemInterface);

    if (!m_inited) {
        d->rect.setHeight(height);
        return;
    }

    Unit u = (unit == UnitNotDefined) ? d->unit : unit;
    qreal newValue = convertUnit(height, u, Millimeter, d->dpi);

    if (d->rect.height()  == newValue)
        return;

    d->rect.setHeight(newValue);

    if (d->rect.height() < d->minRectSize.height())
        d->rect.setHeight(d->minRectSize.height());

    update_gui();

    emit geometryChanged(geometry());
    emit boundingRectChanged(boundingRect());
    emit changed();
}


qreal BaseItemInterface::width(Unit unit) const
{
    Q_D(const BaseItemInterface);
    if (m_inited) {
        Unit u = (unit == UnitNotDefined) ? d->unit : unit;
        return convertUnit(d->rect, Millimeter, u, d->dpi).width();
    } else {
        return d->rect.width();
    }
}


void BaseItemInterface::setWidth(qreal width, Unit unit)
{
    Q_D(BaseItemInterface);

    if (!m_inited) {
        d->rect.setWidth(width);
        return;
    }

    Unit u = (unit == UnitNotDefined) ? d->unit : unit;
    qreal newValue = convertUnit(width, u, Millimeter, d->dpi);

    if (d->rect.width()  == newValue)
        return;

    d->rect.setWidth(newValue);

    if (d->rect.width() < d->minRectSize.width())
        d->rect.setWidth(d->minRectSize.width());

    update_gui();

    emit geometryChanged(geometry());
    emit boundingRectChanged(boundingRect());
    emit changed();
}


QSizeF BaseItemInterface::minSize(Unit unit) const
{
    Q_D(const BaseItemInterface);
    if (m_inited) {
        Unit u = (unit == UnitNotDefined) ? d->unit : unit;
        return convertUnit(d->minRectSize, Millimeter, u, d->dpi);
    } else {
        return d->minRectSize;
    }
}


qint8 BaseItemInterface::frame() const
{
    Q_D(const BaseItemInterface);
    return d->frame;
}


void BaseItemInterface::setFrame(Frames frame)
{
    Q_D(BaseItemInterface);
    if (d->frame == frame)
        return;

    d->frame = frame;

    update_gui();

    emit frameChanged(frame);
    emit changed();
}


qreal BaseItemInterface::opacity() const
{
    Q_D(const BaseItemInterface);
    return d->opacity;
}


void  BaseItemInterface::setOpacity(qreal opacity)
{
    Q_D(BaseItemInterface);
    if (d->opacity == qBound(0.0, opacity, 1.0))
        return;
    d->opacity = qBound(0.0, opacity, 1.0);

    update_gui();

    emit opacityChanged(opacity);
    emit changed();
}


qreal BaseItemInterface::rotation() const
{
    Q_D(const BaseItemInterface);
    return d->rotation;
}


void BaseItemInterface::setRotation(qreal angle)
{
    Q_D(BaseItemInterface);
    if (d->rotation == angle)
        return;

    d->rotation = angle;

    if (d->rotation < 0)
        d->rotation = 0;

    update_gui();

    emit rotationChanged(angle);
    emit changed();
    emit boundingRectChanged(boundingRect());
}


QPen BaseItemInterface::borderPen() const
{
    Q_D(const BaseItemInterface);
    return d->borderPen;
}


void BaseItemInterface::setBorderPen(const QPen & pen)
{
    Q_D(BaseItemInterface);
    if (d->borderPen == pen)
        return;
    d->borderPen = pen;

    update_gui();

    emit borderPenChanged(pen);
    emit changed();
}


QBrush BaseItemInterface::backgroundBrush() const
{
    Q_D(const BaseItemInterface);
    return d->bgBrush;
}


void BaseItemInterface::setBackgroundBrush(const QBrush & brush)
{
    Q_D(BaseItemInterface);
    if (d->bgBrush == brush)
        return;

    d->bgBrush = brush;

    update_gui();

    emit backgroundBrushChanged(brush);
    emit changed();
}


void BaseItemInterface::updateMeassure()
{
    Q_D(BaseItemInterface);
    d->rect = convertUnit(d->rect, d->unit, Millimeter, d->dpi);
}


QTransform BaseItemInterface::transform() const
{
    Q_D(const BaseItemInterface);
    return transform(d);
}


QTransform BaseItemInterface::transform(const BaseItemInterfacePrivate *d)
{
    QTransform trans;
    trans.rotate(-d->rotation);
    return trans;
}


QPointF BaseItemInterface::transformedPos(const BaseItemInterfacePrivate *d, const QRectF &rect)
{
    QTransform trans = transform(d);
    QRectF rotatedRect = trans.mapRect(QRectF(QPointF(0,0), rect.size()));
    QPointF pos = rect.topLeft() - rotatedRect.topLeft();
    return pos;
}


/*********************************************************************************************
 *
 *          ItemInterfacePrivate
 *
 *********************************************************************************************/

QDataStream &operator<<(QDataStream &s, const BaseItemInterfacePrivate &p) {
    s << p.borderPen << p.bgBrush << p.rect << p.frame << p.opacity << p.rotation   << p.enabled << p.order
      << p.dpi << (qint8)p.unit << p.selfRendering << p.childrenSelfRendering << p.minRectSize << (qint16) p.resizeFlags
      << (qint8)p.renderingType;

    return s;
}


QDataStream &operator>>(QDataStream &s, BaseItemInterfacePrivate &p) {
    qint8 unit;
    qint16 resizeFlags;
    qint8 renderingType;

    s >> p.borderPen; s >> p.bgBrush; s >> p.rect; s >> p.frame; s >> p.opacity; s >> p.rotation;   s >> p.enabled; s >> p.order;
    s >> p.dpi; s >> unit; s >> p.selfRendering; s >> p.childrenSelfRendering; s >> p.minRectSize; s >>resizeFlags;
    s >> renderingType;

    p.unit = (Unit)unit;
    p.resizeFlags = resizeFlags;
    p.renderingType = (RenderingType)renderingType;

    return s;
}


} //namespace
