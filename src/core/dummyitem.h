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


#ifndef DUMMYITEM_H
#define DUMMYITEM_H

#include "iteminterface.h"
#include "iteminterface_p.h"
#include "iteminterfaceview.h"
#include "renderediteminterface.h"
#include "globals.h"


namespace CuteReport {


class DummyItemPrivate;

class CUTEREPORT_EXPORTS DummyItem : public CuteReport::ItemInterface
{
    Q_OBJECT
    Q_INTERFACES(CuteReport::ItemInterface)

public:
    explicit DummyItem(QObject *parent = 0);

    virtual void init_gui();

    virtual CuteReport::BaseItemInterface * createInstance(QObject * parent) const;
    virtual CuteReport::BaseItemHelperInterface * helper();
    virtual QByteArray serialize();
    virtual void deserialize(QByteArray &data);
    virtual bool canContain(QObject * object);

    virtual QIcon itemIcon() const;
    virtual QString moduleShortName() const;
    virtual QString suitName() const { return "Standard"; }
    virtual QString itemGroup() const;

    QString originalModuleName() const;
    void setOriginalModuleName(const QString & name);

    virtual bool renderPrepare();
    virtual RenderedItemInterface * renderView();

    static void paint(QPainter * painter, const QStyleOptionGraphicsItem *option, const BaseItemInterfacePrivate * data, const QRectF &boundingRect, RenderingType type);


protected:
    DummyItem(DummyItemPrivate *dd, QObject * parent);
    virtual BaseItemInterface * itemClone() const;

private:
    Q_DECLARE_PRIVATE(DummyItem)
};

//===============================================================================

class CUTEREPORT_EXPORTS DummyItemPrivate : public CuteReport::ItemInterfacePrivate
{
public:
    DummyItemPrivate()
        :ItemInterfacePrivate()
    {}
    DummyItemPrivate(const DummyItemPrivate & p)
        :ItemInterfacePrivate(p)
    {}
    virtual ~DummyItemPrivate(){}

    QString originalModuleName;
};

CUTEREPORT_EXPORTS QDataStream &operator<<(QDataStream &s, const DummyItemPrivate &p);
CUTEREPORT_EXPORTS QDataStream &operator>>(QDataStream &s, DummyItemPrivate &p);

//===============================================================================

class CUTEREPORT_EXPORTS DummyItemView : public CuteReport::ItemInterfaceView
{
    Q_INTERFACES(CuteReport::ItemInterfaceView)
public:
    DummyItemView(CuteReport::BaseItemInterface * item):
        CuteReport::ItemInterfaceView(item){}

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)
    {
        Q_UNUSED(widget)
        DummyItem::paint(painter, option, ptr(), boundingRect(), RenderingTemplate);
    }
};

//===============================================================================

class CUTEREPORT_EXPORTS RenderedDummyItem : public CuteReport::RenderedItemInterface
{
public:
    explicit RenderedDummyItem(CuteReport::BaseItemInterface * item, CuteReport::BaseItemInterfacePrivate *itemPrivateData)
        :RenderedItemInterface(item, itemPrivateData) {}

    virtual void paintBegin(QPainter * painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)
    {
        Q_UNUSED(widget)
        DummyItem::paint(painter, option, d_ptr, boundingRect(), RenderingReport);
    }
};


} // namespace


#endif // DUMMYITEM_H
