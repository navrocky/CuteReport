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
#include <QtGui>
#include <QtScript>
#include "pageinterface.h"
#include "rendererpublicinterface.h"

#include "detailheader.h"
#include "detailheader_p.h"
#include "detailheaderscripting.h"

using namespace CuteReport;


DetailHeader::DetailHeader(QObject * parent):
    CuteReport::BandInterface(*new DetailHeaderPrivate, parent)
   ,m_renderer(0)
{
    Q_D(DetailHeader);
    d->rect = QRectF(0,0,50,20);
    setResizeFlags(FixedPos | ResizeBottom);
}


DetailHeader::DetailHeader(DetailHeaderPrivate &dd, QObject * parent)
    :CuteReport::BandInterface(dd, parent)
    ,m_renderer(0)
{
}


DetailHeader::~DetailHeader()
{
}


BaseItemInterface *DetailHeader::clone()
{
    Q_D(DetailHeader);
    return new DetailHeader(*d, parent());
}


QByteArray DetailHeader::serialize()
{
    Q_D(DetailHeader);
    QByteArray ba;
    QDataStream s( &ba, QIODevice::ReadWrite );
    s << *d;
    return ba;
}


void DetailHeader::deserialize(QByteArray &data)
{
    Q_D(DetailHeader);
    QDataStream s( &data, QIODevice::ReadWrite );
    s >> *d;
}


QDataStream &operator<<(QDataStream &s, const DetailHeaderPrivate &p) {
    s << static_cast<const BandInterfacePrivate&>(p);
    s << p.dataset << p.condition << p.lastConditionResult << p.startNewPage << p.printOnNewPageOnce
         << p.lastPageNumber << p.reprintOnNewPage << p.resetDetailNumber;

    return s;
}


QDataStream &operator>>(QDataStream &s, DetailHeaderPrivate &p) {
    s >> static_cast<BandInterfacePrivate&>(p);
    s >> p.dataset; s >> p.condition; s>> p.lastConditionResult; s>>p.startNewPage; s>> p.printOnNewPageOnce;
    s >> p.lastPageNumber; s >> p.reprintOnNewPage; s >> p.resetDetailNumber;
    return s;
}


BaseItemInterface *DetailHeader::createInstance(QObject * parent) const
{
    return new DetailHeader(parent);
}


bool DetailHeader::canContain(QObject * object)
{
    return (!qobject_cast<BandInterface*>(object) && qobject_cast<BaseItemInterface*>(object));
}


QIcon DetailHeader::itemIcon() const
{
    return QIcon(":/detailHeader.png");
}


QString DetailHeader::moduleName() const
{
    return tr("Detail Header");
}


QString DetailHeader::itemGroup() const
{
    return QString("Bands");
}


QString DetailHeader::dataset() const
{
    Q_D(const DetailHeader);
    return d->dataset;
}


void DetailHeader::setDataset(const QString & dataset)
{
    Q_D(DetailHeader);
    if (d->dataset == dataset)
        return;
    d->dataset = dataset;
    emit datasetChanged(d->dataset);
    emit changed();
}


QString DetailHeader::condition() const
{
    Q_D(const DetailHeader);
    return d->condition;
}


void DetailHeader::setCondition(const QString & condition)
{
    Q_D(DetailHeader);
    if (d->condition == condition)
        return;

    d->condition = condition;
    emit conditionChanged(d->condition);
    emit changed();
}


bool DetailHeader::startNewPage() const
{
    Q_D(const DetailHeader);
    return d->startNewPage;
}


void DetailHeader::setStartNewPage(bool b)
{
    Q_D(DetailHeader);
    if (d->startNewPage == b)
        return;

    d->startNewPage = b;
    emit startNewpageChanged(d->startNewPage);
    emit changed();
}


bool DetailHeader::reprintOnNewPage() const
{
    Q_D(const DetailHeader);
    return d->reprintOnNewPage;
}


void DetailHeader::setReprintOnNewPage(bool b)
{
    Q_D(DetailHeader);
    if (d->reprintOnNewPage == b)
        return;

    d->reprintOnNewPage = b;
    emit reprintOnNewPageChanged(d->reprintOnNewPage);
    emit changed();
}


bool DetailHeader::resetDetailNumber() const
{
    Q_D(const DetailHeader);
    return d->resetDetailNumber;
}


void DetailHeader::setResetDetailNumber(bool b)
{
    Q_D(DetailHeader);
    if (d->resetDetailNumber == b)
        return;

    d->resetDetailNumber = b;
    emit resetDetailNumberChanged(d->resetDetailNumber);
    emit changed();
}


void DetailHeader::renderInit(RendererPublicInterface * renderer)
{
    Q_D(DetailHeader);
    m_renderer = renderer;
    if (!d->dataset.isEmpty()) {
        m_renderer->registerBandToDatasetIteration(d->dataset, this);
    } else {
        m_renderer->error(this->objectName(), "\'dataset\' field is empty");
    }
}


void DetailHeader::renderReset()
{
    m_renderer = 0;
}


CuteReport::RenderedItemInterface * DetailHeader::render(int customDPI)
{
    Q_UNUSED(customDPI)
    Q_D(DetailHeader);

    emit renderingBefore();

    DetailHeaderPrivate * pCurrent = d;
    DetailHeaderPrivate * pNew = new DetailHeaderPrivate(*d);

    d_ptr = pNew;
    emit rendering();
    d_ptr = pCurrent;

    bool needRendering = false;
    bool doNotResetNumber = false;

    // do not make overhead if makes no sence
    if (!pNew->condition.isEmpty()) {

        QString condition = m_renderer->processString(pNew->condition, this);

        if (pNew->lastConditionResult != condition) {

            if (pNew->startNewPage && !pNew->lastConditionResult.isEmpty() &&
                    pNew->lastPageNumber == m_renderer->currentPageNumber()) {

                m_renderer->createNewPage();
                needRendering = true;

            } else
                needRendering = true;
        }

        d->lastConditionResult = condition;

    } else
        needRendering = true;

    if (pNew->reprintOnNewPage && pNew->lastPageNumber != m_renderer->currentPageNumber()) {
        if (!needRendering)                 // its reprinting, not general rendering
            doNotResetNumber = true;        // so we don't need to reset
        needRendering = true;
    }

    if (needRendering && pNew->resetDetailNumber && !doNotResetNumber)
        m_renderer->setValue("_line", 1);

     d->lastPageNumber = m_renderer->currentPageNumber();

     CuteReport::RenderedItemInterface * view = needRendering ? BandInterface::render(customDPI) : 0;

     emit rendered(view);
     emit renderingAfter();

     return view;
}


QString DetailHeader::_current_property_description() const
{
    QString propertyName = metaObject()->property(m_currentProperty).name();

    if (propertyName == "dataset")
        return tr("Name of dataset which band is joined to");
    else if (propertyName == "condition")
        return tr("Band is drawn if condition has changed");
    else if (propertyName == "reprintOnNewPage")
        return tr("If set to True then band will be drawn on every new page created");
    else if (propertyName == "forceNewPage")
        return tr("If True, new page is created every time band appears");
    else if (propertyName == "resetDetailNumber")
        return tr("if True, page number counter will be reset evry time band appears");
    else
        return BandInterface::_current_property_description();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(detailHeader, DetailHeader)
#endif
