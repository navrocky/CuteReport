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


#include "dummyitem.h"
#include <QStyleOptionGraphicsItem>


namespace CuteReport {


DummyItem::DummyItem(QObject *parent)
    :ItemInterface(new DummyItemPrivate, parent)
{
    Q_D(DummyItem);
    setResizeFlags( ResizeLeft | ResizeRight | ResizeTop | ResizeBottom);
    d->rect = QRectF(0,0,40,20);
    d->originalModuleName = "Missed module";
}


DummyItem::DummyItem(DummyItemPrivate *dd, QObject * parent)
    :ItemInterface(dd, parent)
{
}



BaseItemInterface *DummyItem::itemClone() const
{
    Q_D(const DummyItem);
    return new DummyItem(new DummyItemPrivate(*d), parent());
}


QByteArray DummyItem::serialize()
{
    Q_D(DummyItem);
    QByteArray ba;
    QDataStream s( &ba, QIODevice::ReadWrite );
    s << *d;
    return ba;
}


void DummyItem::deserialize(QByteArray &data)
{
    Q_D(DummyItem);
    QDataStream s( &data, QIODevice::ReadWrite );
    s >> *d;
}


QDataStream &operator<<(QDataStream &s, const DummyItemPrivate &p) {
    s << static_cast<const CuteReport::ItemInterfacePrivate&>(p);
    s << p.originalModuleName;
    return s;
}


QDataStream &operator>>(QDataStream &s, DummyItemPrivate &p) {
    s >> static_cast<CuteReport::ItemInterfacePrivate&>(p);
    s >> p.originalModuleName;
    return s;
}


BaseItemInterface *DummyItem::createInstance(QObject * parent) const
{
    return new DummyItem(parent);
}


BaseItemHelperInterface *DummyItem::helper()
{
    return 0;
}


void DummyItem::init_gui()
{
    if (m_gui)
        return;
    m_gui = new DummyItemView(this);
}


bool DummyItem::canContain(QObject * object)
{
    return (dynamic_cast<CuteReport::BaseItemInterface*>(object));
}


QIcon DummyItem::itemIcon() const
{
    return QIcon();
}


QString DummyItem::moduleShortName() const
{
    return tr("DummyItem");
}


QString DummyItem::itemGroup() const
{
    return QString();
}


QString DummyItem::originalModuleName() const
{
    Q_D(const DummyItem);
    return d->originalModuleName;
}


void DummyItem::setOriginalModuleName(const QString & name)
{
    Q_D(DummyItem);
    d->originalModuleName = name;
}


bool DummyItem::renderPrepare()
{
    Q_D(DummyItem);
    emit printBefore();
    setRenderingPointer(new DummyItemPrivate(*d));
    emit printDataBefore();
    emit printDataAfter();
    return true;
}


RenderedItemInterface *DummyItem::renderView()
{
    Q_D(DummyItem);
    RenderedItemInterface * view = new RenderedDummyItem(this, new DummyItemPrivate(*d));
    return view;
}



void DummyItem::paint(QPainter * painter, const QStyleOptionGraphicsItem *option, const BaseItemInterfacePrivate * data, const QRectF &boundingRect, RenderingType type)
{
    Q_UNUSED(option)
    Q_UNUSED(type)
    ItemInterface::paintBegin(painter, option, data, boundingRect, type);

    const DummyItemPrivate * const d = static_cast<const DummyItemPrivate*>(data);

    /// border pen
    painter->setPen(QColor(Qt::black));

    QRectF rect;
    rect.setSize((option->type == QStyleOption::SO_GraphicsItem) ? boundingRect.size() : option->exposedRect.size());
    painter->fillRect(rect, QBrush(QColor(Qt::red), Qt::Dense3Pattern));
    painter->drawRect(rect);
    painter->drawText(rect, QString("\'%1\' missed module").arg(d->originalModuleName));

    ItemInterface::paintEnd(painter, option, data, boundingRect, type);
}


} //namespace
