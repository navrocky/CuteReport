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

Detail::Detail(QObject * parent)
    : BandInterface(*new DetailPrivate,parent)
{
    Q_D(Detail);
    d->rect = QRectF(0,0,50,10);
    setResizeFlags(FixedPos | ResizeBottom);
}


Detail::Detail(DetailPrivate &dd, QObject * parent)
    :CuteReport::BandInterface(dd, parent)
{
    setResizeFlags(FixedPos | ResizeBottom);
}


Detail::~Detail()
{
}


BaseItemInterface *Detail::clone()
{
    Q_D(Detail);
    return new Detail(*d, parent());
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


QString Detail::moduleName() const
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

    emit renderingInit();
}


void Detail::renderReset()
{
    Q_D(Detail);
    m_renderer = 0;
    d->alternateRow = false;
    emit renderingReset();
}


CuteReport::RenderedItemInterface * Detail::render(int customDPI)
{
    Q_UNUSED(customDPI)
    Q_D(Detail);

    emit renderingBefore();

    DetailPrivate * pCurrent = d;
    DetailPrivate * pNew = new DetailPrivate(*d);

    d_ptr = pNew;
    emit rendering();
    d_ptr = pCurrent;

    if (pNew->forceNewPage)
        m_renderer->createNewPage();

    CuteReport::RenderedItemInterface * view = 0;

    if (pNew->alternateRow) {
        if (pNew->alternateBrush == QBrush())
            pNew->bgBrush.setColor(pNew->bgBrush.color().darker(110));
        else
            pNew->bgBrush = pNew->alternateBrush;
        view = new SimpleRenderedItem(this, pNew);
    } else
        view = new SimpleRenderedItem(this, pNew);

    if (d->zebra)
        d->alternateRow = !d->alternateRow;

    emit rendered(view);
    emit renderingAfter();

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
Q_EXPORT_PLUGIN2(detail, Detail)
#endif

