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

#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <iteminterface.h>
#include "iteminterfaceview.h"
#include "renderediteminterface.h"
#include "globals.h"

class EllipseItemPrivate;

class  EllipseItem : public CuteReport::ItemInterface
{
	Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteReport.ItemInterface/1.0")
#endif

    Q_INTERFACES(CuteReport::ItemInterface)

    Q_PROPERTY(QPen pen READ pen WRITE setPen NOTIFY penChanged)
    Q_PROPERTY(QBrush brush READ brush WRITE setBrush NOTIFY brushChanged)

public:

    explicit EllipseItem(QObject * parent = 0);

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

    static void paint(QPainter * painter, const QStyleOptionGraphicsItem *option, const CuteReport::BaseItemInterfacePrivate * data, const QRectF &boundingRect, CuteReport::RenderingType type = CuteReport::RenderingTemplate);

    QPen pen() const;
    void setPen(const QPen & pen);

    QBrush brush() const;
    void setBrush(const QBrush & brush);

    virtual void initScript(QScriptEngine * scriptEngine);

    virtual QString _current_property_description() const;

signals:
    void penChanged(QPen);
    void brushChanged(QBrush);

protected:
    explicit EllipseItem(EllipseItemPrivate *dd, QObject * parent);
    virtual CuteReport::BaseItemInterface * itemClone() const;

private:
    Q_DECLARE_PRIVATE(EllipseItem)
};


class  EllipseItemView : public CuteReport::ItemInterfaceView
{
    Q_INTERFACES(CuteReport::ItemInterfaceView)
public:
    EllipseItemView(CuteReport::BaseItemInterface * item):
        CuteReport::ItemInterfaceView(item){}

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)
    {
        Q_UNUSED(widget)
        EllipseItem::paint(painter, option, ptr(), boundingRect(), CuteReport::RenderingTemplate);
    }
};


class  RenderedEllipseItem : public CuteReport::RenderedItemInterface
{
public:
    explicit RenderedEllipseItem(CuteReport::BaseItemInterface * item, CuteReport::BaseItemInterfacePrivate *itemPrivateData)
        :RenderedItemInterface(item, itemPrivateData) {}

    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)
    {
        Q_UNUSED(widget)
        EllipseItem::paint(painter, option, d_ptr, boundingRect(), CuteReport::RenderingReport);
    }
};


#endif
