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
#include <QtCore>
#include <QBrush>
#include <QPainter>
#include <QIcon>
#include "reportinterface.h"
#include "reportcore.h"
#include "rendererpublicinterface.h"
#include "item_common/simplerendereditem.h"
#include <qmath.h>

#include "detail.h"
#include "detailscripting.h"

#define  MAX_COLUMNS 5

using namespace CuteReport;

inline void initMyResource() { Q_INIT_RESOURCE(detail); }

Detail::Detail(QObject * parent)
    : BandInterface(new DetailPrivate,parent)
{
    Q_D(Detail);
    d->rect = QRectF(0,0,50,10);
    setResizeFlags(FixedPos | ResizeBottom);
}


Detail::Detail(DetailPrivate *dd, QObject * parent)
    :CuteReport::BandInterface(dd, parent)
{
    //setResizeFlags(FixedPos | ResizeBottom);
}


Detail::~Detail()
{
}


void Detail::moduleInit()
{
    initMyResource();
}


BaseItemInterface *Detail::itemClone() const
{
    Q_D(const Detail);
    return new Detail(new DetailPrivate(*d), parent());
}


QByteArray Detail::serialize()
{
    Q_D(Detail);
    QByteArray ba;
    QDataStream s( &ba, QIODevice::ReadWrite );
    s << *d;
    return ba;
}


void Detail::deserialize(QByteArray & data)
{
    Q_D(Detail);
    QDataStream s( &data, QIODevice::ReadWrite );
    s >> *d;
}


QDataStream &operator<<(QDataStream &s, const DetailPrivate &p) {
    s << static_cast<const BandInterfacePrivate&>(p);
    s << p.dataset << p.zebra << p.alternateBrush << p.alternateRow << p.forceNewPage;
    return s;
}


QDataStream &operator>>(QDataStream &s, DetailPrivate &p) {
    s >> static_cast<BandInterfacePrivate&>(p);
    s >> p.dataset; s >> p.zebra; s >> p.alternateBrush; s >> p.alternateRow; s >> p.forceNewPage;
    return s;
}


CuteReport::BaseItemInterface * Detail::createInstance(QObject * parent) const
{
    return new Detail(parent);
}


bool Detail::canContain(QObject * object)
{
    return (!qobject_cast<BandInterface*>(object) && qobject_cast<BaseItemInterface*>(object));
}


QIcon Detail::itemIcon() const
{
    return QIcon(":/detail.png");
}


QString Detail::moduleShortName() const
{
    return tr("Detail");
}


QString Detail::itemGroup() const
{
    return QString("Bands");
}


QString Detail::dataset() const
{
    Q_D(const Detail);
    return d->dataset;
}


void Detail::setDataset(const QString & dataset)
{
    Q_D(Detail);
    if (d->dataset == dataset)
        return;
    d->dataset = dataset;
    emit datasetChanged(d->dataset);
    emit changed();
}


void Detail::renderInit(RendererPublicInterface * renderer)
{
    Q_D(Detail);
    m_renderer = renderer;
    if (!d->dataset.isEmpty()) {
        m_renderer->registerBandToDatasetIteration(d->dataset, this);
    } else {
        m_renderer->error(this->objectName(), "\'dataset\' field is empty");
    }

    emit printInit();
}


void Detail::renderReset()
{
    Q_D(Detail);
    m_renderer = 0;
    d->alternateRow = false;
    emit printReset();
}


bool Detail::renderPrepare()
{
    emit printBefore();
    setRenderingPointer( new DetailPrivate(*(reinterpret_cast<DetailPrivate*>(d_ptr))));
    Q_D(Detail);
    emit printDataBefore();

    if (d->forceNewPage)
        m_renderer->createNewPage();

    bool result = true;

    if (d->alternateRow) {
        if (d->alternateBrush == QBrush())
            d->bgBrush.setColor(d->bgBrush.color().darker(110));
        else
            d->bgBrush = d->alternateBrush;
    } else

    if (d->zebra)
        (reinterpret_cast<DetailPrivate*>(orig_ptr))->alternateRow = !d->alternateRow;

    emit printDataAfter();

    return result;
}


RenderedItemInterface * Detail::renderView()
{
    Q_D(Detail);
    CuteReport::RenderedItemInterface * view = new SimpleRenderedItem(this, new DetailPrivate(*d));
    return view;
}


bool Detail::zebra() const
{
    Q_D(const Detail);
    return d->zebra;
}


void Detail::setZebra(bool b)
{
    Q_D(Detail);
    if (d->zebra == b)
        return;
    d->zebra = b;
    emit zebraChanged(d->zebra);
    emit changed();
}


QBrush Detail::alternateBrush() const
{
    Q_D(const Detail);
    return d->alternateBrush;
}


void Detail::setAlternateBrush(const QBrush & brush)
{
    Q_D(Detail);
    if (d->alternateBrush == brush)
        return;
    d->alternateBrush = brush;
    emit alternateBrushChanged(d->alternateBrush);
    emit changed();
}


bool Detail::forceNewPage() const
{
    Q_D(const Detail);
    return d->forceNewPage;
}


void Detail::setForceNewPage(bool b)
{
    Q_D(Detail);
    if (d->forceNewPage == b)
        return;
    d->forceNewPage = b;
    emit forceNewPageChanged(d->forceNewPage);
    emit changed();
}


QString Detail::_current_property_description() const
{
    QString propertyName = metaObject()->property(m_currentProperty).name();

    if (propertyName == "dataset")
        return tr("Name of dataset which band is joined to");
    else if (propertyName == "zebra")
        return tr("If set to True then every even band is painted with other color");
    else if (propertyName == "evenBrush")
        return tr("Brush for painting even bands if \"zebra\" set to True");
    else if (propertyName == "forceNewPage")
        return tr("if True, each detail is drown on a new page");
    else
        return BandInterface::_current_property_description();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Detail, Detail)
#endif

