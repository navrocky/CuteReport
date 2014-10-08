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
#ifndef PAGEHEADER_H
#define PAGEHEADER_H

#include <bandinterface.h>
#include "bandinterface_p.h"
#include "globals.h"

class PageHeaderPrivate;
class PageHeaderView;

class  PageHeader : public CuteReport::BandInterface
{
	Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteReport.BandInterface/1.0")
#endif
    Q_INTERFACES(CuteReport::BandInterface)

    Q_PROPERTY(bool firstPage READ printOnFirstPage WRITE setPrintOnFirstPage NOTIFY printOnFirstPageChanged)
    Q_PROPERTY(bool once READ printOnce WRITE setPrintOnce NOTIFY printOnceChanged)

public:
    PageHeader(QObject * parent = 0);
    virtual ~PageHeader();

    virtual void moduleInit();

    virtual CuteReport::BaseItemInterface *createInstance(QObject * parent) const;
    virtual QByteArray serialize();
    virtual void deserialize(QByteArray &data);
    virtual bool canContain(QObject * object);

    virtual void renderInit(CuteReport::RendererPublicInterface * renderer);
    virtual void renderReset();
    virtual bool renderPrepare();
    virtual bool renderNewPage();
    virtual CuteReport::RenderedItemInterface * renderView();

    int layoutPriority() const { return 100;}
    BandInterface::AccomodationType accommodationType() const {return AccomodationEveryPage;}

    virtual QIcon itemIcon() const;
    virtual QString moduleShortName() const;
    virtual QString suitName() const { return "Standard"; }
    virtual QString itemGroup() const;

    virtual bool printOnFirstPage() const;
    virtual void setPrintOnFirstPage(bool b);
    virtual bool printOnce() const;
    virtual void setPrintOnce(bool b);

    QString _current_property_description() const;

signals:
    void printOnFirstPageChanged();
    void printOnceChanged();

protected:
    PageHeader(PageHeaderPrivate *dd, QObject * parent);
    virtual CuteReport::BaseItemInterface *itemClone() const;

private:
    Q_DECLARE_PRIVATE(PageHeader)
    CuteReport::RendererPublicInterface * m_renderer;
};


class  PageHeaderPrivate : public CuteReport::BandInterfacePrivate
{
public:
    explicit PageHeaderPrivate(): BandInterfacePrivate(), onFirstPage(true), once(false){}
    virtual ~PageHeaderPrivate(){}

    bool onFirstPage;
    bool once;
};

QDataStream &operator<<(QDataStream &s, const PageHeaderPrivate &p);
QDataStream &operator>>(QDataStream &s, PageHeaderPrivate &p);

//SUIT_END_NAMESPACE
#endif
