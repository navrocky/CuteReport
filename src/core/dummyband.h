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

#ifndef DUMMYBAND_H
#define DUMMYBAND_H

#include <bandinterface.h>
#include "bandinterface_p.h"
#include "iteminterfaceview.h"
#include "renderediteminterface.h"
#include "globals.h"


namespace CuteReport {

class PageInterface;
class DummyBandPrivate;

class CUTEREPORT_EXPORTS DummyBand : public CuteReport::BandInterface
{
    Q_OBJECT
    Q_INTERFACES(CuteReport::BandInterface)
public:
    explicit DummyBand(QObject *parent = 0);

    virtual void init_gui();

    virtual BaseItemInterface *createInstance(QObject * parent) const;
    virtual QByteArray serialize();
    virtual void deserialize(QByteArray &data);
    virtual bool canContain(QObject * object);

    virtual bool renderPrepare();
    virtual RenderedItemInterface * renderView();

    static void paint(QPainter * painter, const QStyleOptionGraphicsItem *option, const BaseItemInterfacePrivate * data, const QRectF &boundingRect, RenderingType type);

    virtual QIcon itemIcon() const;
    virtual QString moduleShortName() const;
    virtual QString suitName() const { return "Standard"; }
    virtual QString itemGroup() const;

    QString originalModuleName() const;
    void setOriginalModuleName(const QString & name);

    CuteReport::BandInterface::LayoutType layoutType() const;
    void setLayoutType(CuteReport::BandInterface::LayoutType layoutType);
    int layoutPriority() const;
    void setLayoutPriority(int priority);

signals:

protected:
    DummyBand(DummyBandPrivate *dd, QObject * parent);
    virtual BaseItemInterface * itemClone() const;

private:
    Q_DECLARE_PRIVATE(DummyBand)

};


class CUTEREPORT_EXPORTS DummyBandPrivate : public CuteReport::BandInterfacePrivate
{
public:
    DummyBandPrivate():BandInterfacePrivate() {}
    virtual ~DummyBandPrivate(){}

    BandInterface::LayoutType layoutType;
    int layoutPriority;
    QString originalModuleName;
};


CUTEREPORT_EXPORTS QDataStream &operator<<(QDataStream &s, const DummyBandPrivate &p);
CUTEREPORT_EXPORTS QDataStream &operator>>(QDataStream &s, DummyBandPrivate &p);


//===============================================================================

class CUTEREPORT_EXPORTS DummyBandView : public CuteReport::ItemInterfaceView
{
    Q_INTERFACES(CuteReport::ItemInterfaceView)
public:
    DummyBandView(CuteReport::BaseItemInterface * item):
        CuteReport::ItemInterfaceView(item){}

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)
    {
        Q_UNUSED(widget)
        DummyBand::paint(painter, option, ptr(), boundingRect(), RenderingTemplate);
    }
};

//===============================================================================

class CUTEREPORT_EXPORTS RenderedDummyBand : public CuteReport::RenderedItemInterface
{
public:
    explicit RenderedDummyBand(CuteReport::BaseItemInterface * item, CuteReport::BaseItemInterfacePrivate *itemPrivateData)
        :RenderedItemInterface(item, itemPrivateData) {}

    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)
    {
        Q_UNUSED(widget)
        DummyBand::paint(painter, option, d_ptr, boundingRect(), RenderingReport);
    }
};


} // namespace

#endif // DUMMYBAND_H
