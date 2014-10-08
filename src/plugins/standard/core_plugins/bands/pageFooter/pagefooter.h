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
#ifndef PAGEFOOTER_H
#define PAGEFOOTER_H

#include <bandinterface.h>
#include "bandinterface_p.h"
#include "globals.h"

class PageFooterPrivate;
class PageFooterView;

class  PageFooter : public CuteReport::BandInterface
{
	Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteReport.BandInterface/1.0")
#endif
    Q_INTERFACES(CuteReport::BandInterface)

    Q_PROPERTY(bool firstPage READ printOnFirstPage WRITE setPrintOnFirstPage NOTIFY printOnFirstPageChanged)
    Q_PROPERTY(bool lastPage READ printOnLastPage WRITE setPrintOnLastPage NOTIFY printOnLastPageChanged)
    Q_PROPERTY(bool once READ printOnce WRITE setPrintOnce NOTIFY printOnceChanged)

    Q_PROPERTY(QStringList _properties_hidden READ _hiddenProperties DESIGNABLE false)

public:
    PageFooter(QObject * parent = 0);
    ~PageFooter();

    virtual void moduleInit();

    virtual CuteReport::BaseItemInterface * createInstance(QObject * parent) const;
    virtual QByteArray serialize();
    virtual void deserialize(QByteArray &data);
    virtual bool canContain(QObject * object);

    BandInterface::LayoutType layoutType() const;
    int layoutPriority() const { return 100;}
    BandInterface::AccomodationType accommodationType() const {return AccomodationEveryPage;}

    virtual QIcon itemIcon() const;
    virtual QString moduleShortName() const;
    virtual QString suitName() const { return "Standard"; }
    virtual QString itemGroup() const;

    virtual bool printOnFirstPage() const;
    virtual void setPrintOnFirstPage(bool b);
    virtual bool printOnLastPage() const;
    virtual void setPrintOnLastPage(bool b);
    virtual bool printOnce() const;
    virtual void setPrintOnce(bool b);

    virtual void renderInit(CuteReport::RendererPublicInterface * renderer);
    virtual void renderReset();
    virtual bool renderPrepare();
    virtual bool renderNewPage();
    virtual CuteReport::RenderedItemInterface * renderView();

    virtual QString _current_property_description() const;
    QStringList _hiddenProperties() const;
signals:
    void printOnFirstPageChanged();
    void printOnLastPageChanged();
    void printOnceChanged();

private:
    Q_DECLARE_PRIVATE(PageFooter)

    PageFooter(PageFooterPrivate *dd, QObject * parent);
    virtual BaseItemInterface * itemClone() const;

    CuteReport::RendererPublicInterface * m_renderer;
};


class PageFooterPrivate : public CuteReport::BandInterfacePrivate
{
public:
    PageFooterPrivate(): BandInterfacePrivate(), onFirstPage(true), onLastPage(true), once(false){}
    virtual ~PageFooterPrivate(){}

    bool onFirstPage;
    bool onLastPage;
    bool once;
};

QDataStream &operator<<(QDataStream &s, const PageFooterPrivate &p);
QDataStream &operator>>(QDataStream &s, PageFooterPrivate &p);


#endif
