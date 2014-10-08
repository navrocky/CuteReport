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

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <iteminterface.h>
#include "iteminterfaceview.h"
#include "renderediteminterface.h"
#include "globals.h"

class RectangleItemPrivate;

class  RectangleItem : public CuteReport::ItemInterface
{
	Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteReport.ItemInterface/1.0")
#endif

    Q_INTERFACES(CuteReport::ItemInterface)

public:
    explicit RectangleItem(QObject * parent = 0);

    virtual void moduleInit();
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

    virtual bool renderPrepare();
    virtual CuteReport::RenderedItemInterface * renderView();

    virtual void initScript(QScriptEngine * scriptEngine);

private:
    Q_DECLARE_PRIVATE(RectangleItem)
    explicit RectangleItem(RectangleItemPrivate *dd, QObject * parent);
    virtual BaseItemInterface * itemClone() const;
};


//===============================================================================

class  RectangleItemView : public CuteReport::ItemInterfaceView
{
    Q_INTERFACES(CuteReport::ItemInterfaceView)
public:
    RectangleItemView(CuteReport::ItemInterface * item):
        CuteReport::ItemInterfaceView(item){}

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)
    {
        Q_UNUSED(widget)
        ItemInterfaceView::paint(painter, option);
    }
};


//===============================================================================


class  RenderedRectangleItem : public CuteReport::RenderedItemInterface
{
public:
    explicit RenderedRectangleItem(CuteReport::BaseItemInterface * item, CuteReport::BaseItemInterfacePrivate *itemPrivateData)
        :RenderedItemInterface(item, itemPrivateData) {}

    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)
    {
        Q_UNUSED(widget)
        RenderedItemInterface::paint(painter, option);
    }
};


#endif
