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

#include "summary.h"
#include "reportcore.h"
#include "summaryscripting.h"
#include "item_common/simplerendereditem.h"

using namespace CuteReport;

inline void initMyResource() { Q_INIT_RESOURCE(summary); }

Summary::Summary(QObject * parent)
    : CuteReport::BandInterface(new SummaryPrivate, parent)
{
    Q_D(Summary);
    setResizeFlags(FixedPos | ResizeBottom);
    d->rect = QRectF(0,0,50,20);
}


Summary::Summary(SummaryPrivate *dd, QObject * parent)
    :CuteReport::BandInterface(dd, parent)
{
}


Summary::~Summary()
{
    ReportCore::log(LogDebug, "Summary", "Summary DTOR" );
}


void Summary::moduleInit()
{
    initMyResource();
}


CuteReport::BaseItemInterface * Summary::createInstance(QObject * parent) const
{
    return new Summary(parent);
}


BaseItemInterface *Summary::itemClone() const
{
    Q_D(const Summary);
    return new Summary(new SummaryPrivate(*d), parent());
}


QByteArray Summary::serialize()
{
    Q_D(Summary);
    QByteArray ba;
    QDataStream s( &ba, QIODevice::ReadWrite );
    s << *d;
    return ba;
}


void Summary::deserialize(QByteArray &data)
{
    Q_D(Summary);
    QDataStream s( &data, QIODevice::ReadWrite );
    s >> *d;
}


QDataStream &operator<<(QDataStream &s, const SummaryPrivate &p) {
    s << static_cast<const BandInterfacePrivate&>(p);
    return s;
}


QDataStream &operator>>(QDataStream &s, SummaryPrivate &p) {
    s >> static_cast<BandInterfacePrivate&>(p);
    return s;
}


bool Summary::canContain(QObject * object)
{
    return (!qobject_cast<BandInterface*>(object) && qobject_cast<BaseItemInterface*>(object));
}


QIcon Summary::itemIcon() const
{
    return QIcon(":/summary.png");
}


QString Summary::moduleShortName() const
{
    return tr("Summary");
}


QString Summary::itemGroup() const
{
    return QString("Bands");
}


bool Summary::renderPrepare()
{
    emit printBefore();
    setRenderingPointer(new SummaryPrivate(*(reinterpret_cast<SummaryPrivate*>(d_ptr))));
    Q_D(Summary);
    emit printDataBefore();
    emit printDataAfter();
    return true;
}


RenderedItemInterface *Summary::renderView()
{
    Q_D(Summary);
    CuteReport::RenderedItemInterface * view = new SimpleRenderedItem(this, new SummaryPrivate(*d));
    return view;
}



#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Summary, Summary)
#endif
