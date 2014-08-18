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
#include "overlay.h"
#include "overlayscripting.h"

#include <QIcon>

using namespace CuteReport;


Overlay::Overlay(QObject * parent)
    : CuteReport::BandInterface(*new OverlayPrivate, parent)
{
    Q_D(Overlay);
    setResizeFlags(ResizeBottom | ResizeTop | ResizeLeft | ResizeRight);
    d->rect = QRectF(0,0,50,20);
}


Overlay::Overlay(OverlayPrivate &dd, QObject * parent)
    :CuteReport::BandInterface(dd, parent)
{
    setResizeFlags(FixedPos | ResizeBottom);
}



Overlay::~Overlay()
{
}


BaseItemInterface *Overlay::clone()
{
    Q_D(Overlay);
    return new Overlay(*d, parent());
}


QByteArray Overlay::serialize()
{
    Q_D(Overlay);
    QByteArray ba;
    QDataStream s( &ba, QIODevice::ReadWrite );
    s << *d;
    return ba;
}


void Overlay::deserialize(QByteArray &data)
{
    Q_D(Overlay);
    QDataStream s( &data, QIODevice::ReadWrite );
    s >> *d;
}


QDataStream &operator<<(QDataStream &s, const OverlayPrivate &p) {
    s << static_cast<const BandInterfacePrivate&>(p);
    return s;
}


QDataStream &operator>>(QDataStream &s, OverlayPrivate &p) {
    s >> static_cast<BandInterfacePrivate&>(p);
    return s;
}


CuteReport::BaseItemInterface * Overlay::createInstance(QObject * parent) const
{
    return new Overlay(parent);
}


bool Overlay::canContain(QObject * object)
{
    return (!dynamic_cast<CuteReport::BandInterface*>(object) && dynamic_cast<CuteReport::BaseItemInterface*>(object));
}


QIcon Overlay::itemIcon() const
{
    return QIcon(":/overlay.png");
}


QString Overlay::moduleName() const
{
    return tr("Overlay");
}


QString Overlay::itemGroup() const
{
	return tr("Bands");
}


void Overlay::setOrder(int order)
{
    Q_D(Overlay);
    int oldOrder = d->order;
    d->order = order;

    if (d->order != oldOrder) {
        emit orderChanged(d->order);
        emit changed();
    }
}


CuteReport::RenderedItemInterface * Overlay::render(int customDPI)
{
    Q_UNUSED(customDPI);
    Q_D(Overlay);

    emit renderingBefore();

    OverlayPrivate * pCurrent = d;
    OverlayPrivate * pNew = new OverlayPrivate(*d);

    d_ptr = pNew;
    emit rendering();
    d_ptr = pCurrent;

    CuteReport::RenderedItemInterface * view =  (d->order >= 0) ? BandInterface::render(customDPI) : 0;

    emit rendered(view);
    emit renderingAfter();

    return view;
}


CuteReport::RenderedItemInterface * Overlay::renderNewPage(int customDPI)
{
    Q_D(Overlay);
    if (d->order < 0)
       return BandInterface::render(customDPI);
    else
        return 0;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(overlay, Overlay)
#endif
