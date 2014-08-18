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
#include "iteminterface.h"
#include "pagefooter.h"
#include "rendererpublicinterface.h"
#include "pagefooterscripting.h"

using namespace CuteReport;


PageFooter::PageFooter(QObject * parent)
    :CuteReport::BandInterface(*new PageFooterPrivate, parent),
      m_renderer(0)
{
    Q_D(PageFooter);
    setResizeFlags(FixedPos | ResizeTop);
    d->rect = QRectF(0,0,50,20);
}


PageFooter::PageFooter(PageFooterPrivate &dd, QObject * parent)
    :CuteReport::BandInterface(dd, parent)
{

}


PageFooter::~PageFooter()
{
}


BaseItemInterface *PageFooter::clone()
{
    Q_D(PageFooter);
    return new PageFooter(*d, parent());
}


QByteArray PageFooter::serialize()
{
    Q_D(PageFooter);
    QByteArray ba;
    QDataStream s( &ba, QIODevice::ReadWrite );
    s << *d;
    return ba;
}


void PageFooter::deserialize(QByteArray &data)
{
    Q_D(PageFooter);
    QDataStream s( &data, QIODevice::ReadWrite );
    s >> *d;
}


QDataStream &operator<<(QDataStream &s, const PageFooterPrivate &p) {
    s << static_cast<const BandInterfacePrivate&>(p);
    s << p.onFirstPage << p.onLastPage << p.once;
    return s;
}


QDataStream &operator>>(QDataStream &s, PageFooterPrivate &p) {
    s >> static_cast<BandInterfacePrivate&>(p);
    s >> p.onFirstPage; s >> p.onLastPage; s >> p.once;
    return s;
}


BaseItemInterface *PageFooter::createInstance(QObject * parent) const
{
    return new PageFooter(parent);
}


BandInterface::LayoutType PageFooter::layoutType() const
{
    return CuteReport::BandInterface::LayoutBottom;
}


bool PageFooter::canContain(QObject * object)
{
    return (!qobject_cast<BandInterface*>(object) && qobject_cast<BaseItemInterface*>(object));
}


QIcon PageFooter::itemIcon() const
{
    return QIcon(":/pageFooter.png");
}


QString PageFooter::moduleName() const
{
    return tr("Page Footer");
}


QString PageFooter::itemGroup() const
{
    return QString("Bands");
}


void PageFooter::renderInit(RendererPublicInterface * renderer)
{
    m_renderer = renderer;
}


void PageFooter::renderReset()
{
    m_renderer = 0;
}


CuteReport::RenderedItemInterface * PageFooter::render(int customDPI)
{
    Q_UNUSED(customDPI);
    return 0;
}



CuteReport::RenderedItemInterface * PageFooter::renderNewPage(int customDPI)
{
    Q_D(PageFooter);

    emit renderingBefore();

    PageFooterPrivate * pCurrent = d;
    PageFooterPrivate * pNew = new PageFooterPrivate(*d);

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


bool PageFooter::printOnFirstPage() const
{
    Q_D(const PageFooter);
    return d->onFirstPage;
}


void PageFooter::setPrintOnFirstPage(bool b)
{
    Q_D(PageFooter);
    if (b == d->onFirstPage)
        return;

    d->onFirstPage = b;
    emit printOnFirstPageChanged();
    emit changed();
}


bool PageFooter::printOnLastPage() const
{
    Q_D(const PageFooter);
    return d->onLastPage;
}


void PageFooter::setPrintOnLastPage(bool b)
{
    Q_D(PageFooter);
    if (b == d->onLastPage)
        return;

    d->onLastPage = b;
    emit printOnLastPageChanged();
    emit changed();
}


bool PageFooter::printOnce() const
{
    Q_D(const PageFooter);
    return d->once;
}


void PageFooter::setPrintOnce(bool b)
{
    Q_D(PageFooter);
    if (b == d->once)
        return;

    d->once = b;
    emit printOnceChanged();
    emit changed();
}


QString PageFooter::_current_property_description() const
{
    QString propertyName = metaObject()->property(m_currentProperty).name();

    if (propertyName == "firstPage")
        return tr("If True, band is drawn on first page");
    if (propertyName == "lastPage")
        return tr("If True, band is drawn on last page");
    else if (propertyName == "once")
        return tr("If True, band appears only once");
    else
        return BandInterface::_current_property_description();
}


QStringList PageFooter::_hiddenProperties() const
{
    QStringList list;
    list << "stretchable";
    return list;
}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(PageFooter, PageFooter)
#endif
