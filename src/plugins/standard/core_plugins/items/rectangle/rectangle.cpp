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

#include "rectangle.h"
#include "rectangle_p.h"
#include "rectangleitemscript.h"

#include <QtCore>
#include <QtGui>

inline void initMyResource() { Q_INIT_RESOURCE(rectangle); }

RectangleItem::RectangleItem(QObject *parent)
    :ItemInterface(new RectangleItemPrivate, parent)
{
    Q_INIT_RESOURCE(rectangle);
    Q_D(RectangleItem);
    setResizeFlags( ResizeLeft | ResizeRight | ResizeTop | ResizeBottom);
    d->rect = QRectF(0,0,40,20);
}


void RectangleItem::moduleInit()
{
    initMyResource();
}


RectangleItem::RectangleItem(RectangleItemPrivate *dd, QObject * parent)
    :ItemInterface(dd, parent)
{
    Q_INIT_RESOURCE(rectangle);
}


CuteReport::BaseItemInterface * RectangleItem::itemClone() const
{
    Q_D(const RectangleItem);
    return new RectangleItem(new RectangleItemPrivate(*d), parent());
}


QByteArray RectangleItem::serialize()
{
    Q_D(RectangleItem);
    QByteArray ba;
    QDataStream s( &ba, QIODevice::ReadWrite );
    s << *d;
    return ba;
}


void RectangleItem::deserialize(QByteArray &data)
{
    Q_D(RectangleItem);
    QDataStream s( &data, QIODevice::ReadWrite );
    s >> *d;
}


QDataStream &operator<<(QDataStream &s, const RectangleItemPrivate &p) {
    s << static_cast<const CuteReport::ItemInterfacePrivate&>(p);
    return s;
}


QDataStream &operator>>(QDataStream &s, RectangleItemPrivate &p) {
    s >> static_cast<CuteReport::ItemInterfacePrivate&>(p);
    return s;
}


CuteReport::BaseItemInterface *RectangleItem::createInstance(QObject * parent) const
{
    return new RectangleItem(parent);
}


CuteReport::BaseItemHelperInterface *RectangleItem::helper()
{
    return 0;
}


void RectangleItem::init_gui()
{
    if (m_gui)
        return;
    m_gui = new RectangleItemView(this);
}


bool RectangleItem::canContain(QObject * object)
{
    return (dynamic_cast<CuteReport::ItemInterface*>(object));
}


QIcon RectangleItem::itemIcon() const
{
    return QIcon(":/rectangle.png");
}


QString RectangleItem::moduleShortName() const
{
    return tr("Rectangle");
}


QString RectangleItem::itemGroup() const
{
    return QString("Shapes");
}


bool RectangleItem::renderPrepare()
{
    emit printBefore();
    setRenderingPointer(new RectangleItemPrivate(*(reinterpret_cast<RectangleItemPrivate*>(d_ptr))));
    emit printDataBefore();
    emit printDataAfter();
    return true;
}


RenderedItemInterface *RectangleItem::renderView()
{
    Q_D(RectangleItem);
    RenderedRectangleItem * view = new RenderedRectangleItem(this, new RectangleItemPrivate(*d));
    return view;
}


void RectangleItem::initScript(QScriptEngine *scriptEngine)
{
    registerRectangleItemScriptClass(scriptEngine);
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Rectangle, RectangleItem)
#endif
