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
#include "title.h"
#include "titlescripting.h"
#include "item_common/simplerendereditem.h"

#include <QIcon>

using namespace CuteReport;

inline void initMyResource() { Q_INIT_RESOURCE(title); }

Title::Title(QObject * parent)
    : CuteReport::BandInterface(new TitlePrivate, parent)
{
    Q_D(Title);
    setResizeFlags(FixedPos | ResizeBottom);
    d->rect = QRectF(0,0,50,20);
}


Title::Title(TitlePrivate *dd, QObject * parent)
    :CuteReport::BandInterface(dd, parent)
{
    setResizeFlags(FixedPos | ResizeBottom);
}



Title::~Title()
{
}


void Title::moduleInit()
{
    initMyResource();
}


CuteReport::BaseItemInterface * Title::itemClone() const
{
    Q_D(const Title);
    return new Title(new TitlePrivate(*d), parent());
}


QByteArray Title::serialize()
{
    Q_D(Title);
    QByteArray ba;
    QDataStream s( &ba, QIODevice::ReadWrite );
    s << *d;
    return ba;
}


void Title::deserialize(QByteArray &data)
{
    Q_D(Title);
    QDataStream s( &data, QIODevice::ReadWrite );
    s >> *d;
}


QDataStream &operator<<(QDataStream &s, const TitlePrivate &p) {
    s << static_cast<const BandInterfacePrivate&>(p);
    return s;
}


QDataStream &operator>>(QDataStream &s, TitlePrivate &p) {
    s >> static_cast<BandInterfacePrivate&>(p);
    return s;
}


CuteReport::BaseItemInterface * Title::createInstance(QObject * parent) const
{
    return new Title(parent);
}


bool Title::canContain(QObject * object)
{
    return (!dynamic_cast<CuteReport::BandInterface*>(object) && dynamic_cast<CuteReport::BaseItemInterface*>(object));
}


QIcon Title::itemIcon() const
{
	return QIcon(":/title.png");
}


QString Title::moduleShortName() const
{
	return tr("Title");
}


QString Title::itemGroup() const
{
	return tr("Bands");
}


bool Title::renderPrepare()
{
    return false;
}


bool Title::renderNewPage()
{
    emit printBefore();
    setRenderingPointer(new TitlePrivate(*(reinterpret_cast<TitlePrivate*>(d_ptr))));
    emit printDataBefore();
    emit printDataAfter();
    return true;
}


RenderedItemInterface *Title::renderView()
{
    Q_D(Title);
    CuteReport::RenderedItemInterface * view = new SimpleRenderedItem(this, new TitlePrivate(*d));
    return view;
}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Title, Title)
#endif
