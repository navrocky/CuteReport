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

#include "pageinterface.h"
#include "rendererpublicinterface.h"
#include "pageheader.h"
#include "pageheaderscripting.h"

using namespace CuteReport;


PageHeader::PageHeader(QObject * parent):
    CuteReport::BandInterface(*new PageHeaderPrivate, parent)
   ,m_renderer(0)
{
    Q_D(PageHeader);
    d->rect = QRectF(0,0,50,20);
    setResizeFlags(FixedPos | ResizeBottom);
}


PageHeader::PageHeader(PageHeaderPrivate &dd, QObject * parent)
    :CuteReport::BandInterface(dd, parent)
    ,m_renderer(0)
{
}


PageHeader::~PageHeader()
{
}


CuteReport::BaseItemInterface * PageHeader::clone()
{
    Q_D(PageHeader);
    return new PageHeader(*d, parent());
}


QByteArray PageHeader::serialize()
{
    Q_D(PageHeader);
    QByteArray ba;
    QDataStream s( &ba, QIODevice::ReadWrite );
    s << *d;
    return ba;
}


void PageHeader::deserialize(QByteArray &data)
{
    Q_D(PageHeader);
    QDataStream s( &data, QIODevice::ReadWrite );
    s >> *d;
}


QDataStream &operator<<(QDataStream &s, const PageHeaderPrivate &p) {
    s << static_cast<const BandInterfacePrivate&>(p);
    s << p.onFirstPage << p.once;
    return s;
}


QDataStream &operator>>(QDataStream &s, PageHeaderPrivate &p) {
    s >> static_cast<BandInterfacePrivate&>(p);
    s >> p.onFirstPage; s >> p.once;
    return s;
}


BaseItemInterface * PageHeader::createInstance(QObject * parent) const
{
    return new PageHeader(parent);
}


bool PageHeader::canContain(QObject * object)
{
    return (!qobject_cast<BandInterface*>(object) && qobject_cast<BaseItemInterface*>(object));
}


QIcon PageHeader::itemIcon() const
{
	return QIcon(":/pageHeader.png");
}


QString PageHeader::moduleName() const
{
    return tr("Page Header");
}


QString PageHeader::itemGroup() const
{
    return QString("Bands");
}


void PageHeader::renderInit(RendererPublicInterface * renderer)
{
    m_renderer = renderer;
}


void PageHeader::renderReset()
{
    m_renderer = 0;
}


CuteReport::RenderedItemInterface * PageHeader::render(int customDPI)
{
    Q_UNUSED(customDPI)
    return 0;
}


CuteReport::RenderedItemInterface * PageHeader::renderNewPage(int customDPI)
{
    Q_D(PageHeader);

    emit renderingBefore();

    PageHeaderPrivate * pCurrent = d;
    PageHeaderPrivate * pNew = new PageHeaderPrivate(*d);

    d_ptr = pNew;
    emit rendering();
    d_ptr = pCurrent;

    CuteReport::RenderedItemInterface * view = 0;

    if (m_renderer->currentPageNumber() == 1) {
        if (d->onFirstPage)
            view = BandInterface::render(customDPI);
    } else if (m_renderer->currentPageNumber() == 2){
        if (d->once) {
            if (!d->onFirstPage)
                view = BandInterface::render(customDPI);
        } else
            view = BandInterface::render(customDPI);
    } else
        view = d->once ? 0 : BandInterface::render(customDPI);

    emit rendered(view);
    emit renderingAfter();

    return view;
}


bool PageHeader::printOnFirstPage() const
{
    Q_D(const PageHeader);
    return d->onFirstPage;
}


void PageHeader::setPrintOnFirstPage(bool b)
{
    Q_D(PageHeader);
    if (b == d->onFirstPage)
        return;

    d->onFirstPage = b;
    emit printOnFirstPageChanged();
    emit changed();
}


bool PageHeader::printOnce() const
{
    Q_D(const PageHeader);
    return d->once;
}


void PageHeader::setPrintOnce(bool b)
{
    Q_D(PageHeader);
    if (b == d->once)
        return;
    d->once = b;
    emit printOnceChanged();
    emit changed();
}


QString PageHeader::_current_property_description() const
{
    QString propertyName = metaObject()->property(m_currentProperty).name();

    if (propertyName == "firstPage")
        return tr("If True, band is drawn on first page");
    else if (propertyName == "once")
        return tr("If True, band appears only once");
    else
        return BandInterface::_current_property_description();
}

//}//namespace

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(pageHeader, PageHeader)
#endif
