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

#include <QtCore>
#include <QBrush>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "dummyband.h"

namespace CuteReport {


DummyBand::DummyBand(QObject * parent)
    : CuteReport::BandInterface(new DummyBandPrivate, parent)
{
    Q_D(DummyBand);
    setResizeFlags(FixedPos | ResizeBottom);
    d->rect = QRectF(0,0,50,20);
    d->layoutType = LayoutTop;
    d->layoutPriority = 20;
}


DummyBand::DummyBand(DummyBandPrivate *dd, QObject * parent)
    :CuteReport::BandInterface(dd, parent)
{
}


CuteReport::BaseItemInterface * DummyBand::createInstance(QObject * parent) const
{
    return new DummyBand(parent);
}


CuteReport::BaseItemInterface * DummyBand::itemClone() const
{
    Q_D(const DummyBand);
    return new DummyBand(new DummyBandPrivate(*d), parent());
}


QByteArray DummyBand::serialize()
{
    Q_D(DummyBand);
    QByteArray ba;
    QDataStream s( &ba, QIODevice::ReadWrite );
    s << *d;
    return ba;
}


void DummyBand::deserialize(QByteArray &data)
{
    Q_D(DummyBand);
    QDataStream s( &data, QIODevice::ReadWrite );
    s >> *d;
}


QDataStream &operator<<(QDataStream &s, const DummyBandPrivate &p) {
    s << static_cast<const BandInterfacePrivate&>(p);
    s << p.originalModuleName << (qint8)p.layoutType << p.layoutPriority;
    return s;
}


QDataStream &operator>>(QDataStream &s, DummyBandPrivate &p) {
    s >> static_cast<BandInterfacePrivate&>(p);
    qint8 layoutType;
    s >> p.originalModuleName; s >> layoutType; s >> p.layoutPriority;
    p.layoutType = (BandInterface::LayoutType)layoutType;
    return s;
}


bool DummyBand::canContain(QObject * object)
{
    return (!qobject_cast<BandInterface*>(object) && qobject_cast<BaseItemInterface*>(object));
}


bool DummyBand::renderPrepare()
{
    Q_D(DummyBand);
    emit printBefore();
    setRenderingPointer(new DummyBandPrivate(*d));
    emit printDataBefore();
    emit printDataAfter();
    return true;
}


RenderedItemInterface *DummyBand::renderView()
{
    Q_D(DummyBand);
    RenderedItemInterface * view = new RenderedDummyBand(this, new DummyBandPrivate(*d));
    return view;
}


QIcon DummyBand::itemIcon() const
{
    return QIcon();
}


QString DummyBand::moduleShortName() const
{
    return tr("DummyBand");
}


QString DummyBand::itemGroup() const
{
    return QString();
}


QString DummyBand::originalModuleName() const
{
    Q_D(const DummyBand);
    return d->originalModuleName;
}


void DummyBand::setOriginalModuleName(const QString & name)
{
    Q_D(DummyBand);
    d->originalModuleName = name;
}


CuteReport::BandInterface::LayoutType DummyBand::layoutType() const
{
    Q_D(const DummyBand);
    return d->layoutType;
}


void DummyBand::setLayoutType(CuteReport::BandInterface::LayoutType layoutType)
{
    Q_D(DummyBand);
    d->layoutType = layoutType;
}


int DummyBand::layoutPriority() const
{
    Q_D(const DummyBand);
    return d->layoutPriority;
}


void DummyBand::setLayoutPriority(int priority)
{
    Q_D(DummyBand);
    d->layoutPriority = priority;
}


void DummyBand::init_gui()
{
    if (m_gui)
        return;
    m_gui = new DummyBandView(this);
}


void DummyBand::paint(QPainter * painter, const QStyleOptionGraphicsItem *option, const BaseItemInterfacePrivate * data, const QRectF &boundingRect, RenderingType type)
{
    Q_UNUSED(option)
    Q_UNUSED(type)
    const DummyBandPrivate * const d = static_cast<const DummyBandPrivate*>(data);

    /// border pen
    painter->setPen(QColor(Qt::black));

    QRectF rect;
    rect.setSize((option->type == QStyleOption::SO_GraphicsItem) ? boundingRect.size() : option->exposedRect.size());
    painter->fillRect(rect, QBrush(QColor(Qt::red), Qt::Dense3Pattern));
    painter->drawRect(rect);
    painter->drawText(rect, QString("\'%1\' missed band").arg(d->originalModuleName));
}



} // namespace
