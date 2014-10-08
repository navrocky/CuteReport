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

#ifndef BANDINTERFACE_H
#define BANDINTERFACE_H

#include "baseiteminterface.h"
#include "baseiteminterface_p.h"
#include "globals.h"

#define BANDVIEWTYPE QGraphicsItem::UserType + 102

namespace CuteReport
{

class BandInterfacePrivate;

class CUTEREPORT_EXPORTS BandInterface : public BaseItemInterface
{
	Q_OBJECT
    Q_INTERFACES(CuteReport::BaseItemInterface)

//    Q_PROPERTY(Margins margins READ margins WRITE setMargins NOTIFY marginsChanged)
    Q_PROPERTY(bool stretchable READ stretchable WRITE setStretchable NOTIFY stretchableChanged)
    Q_PROPERTY(bool showStretchability READ showStretchability WRITE setShowStretchability NOTIFY showStretchabilityChanged)
    Q_PROPERTY(QSizeF stretchOriginalSize READ stretchOriginalSize WRITE setStretchOriginalSize DESIGNABLE false)

public:
    enum LayoutType { LayoutUndefined, LayoutTop, LayoutBottom, LayoutFree };
    enum AccomodationType {AccomodationUndefined, AccomodationOnce, AccomodationFirstPage, AccomodationLastPage, AccomodationEveryPage};

public:
    explicit BandInterface(QObject * parent);
    virtual ~BandInterface();
    inline void initMe();

    virtual void init_gui();
    virtual bool renderPrepare() {return false;}
    virtual bool renderNewPage() {return false;}
//    virtual CuteReport::RenderedItemInterface * renderView();

    virtual LayoutType layoutType() const;
    virtual int layoutPriority() const;
    virtual AccomodationType accommodationType() const;

    virtual void setGeometry(const QRectF & rect, CuteReport::Unit unit = CuteReport::UnitNotDefined);
    virtual void setHeight(qreal height, CuteReport::Unit unit = CuteReport::UnitNotDefined);

    virtual QString dataset() const { return QString(); }
    virtual void setDataset(const QString & dataset) { Q_UNUSED(dataset); }

    virtual Margins margins() const;
    virtual void setMargins(const Margins & margins);

    virtual bool stretchable() const;
    virtual void setStretchable(bool value);

    virtual bool showStretchability() const;
    virtual void setShowStretchability(bool value);

    virtual QSizeF stretchOriginalSize() const;
    virtual void setStretchOriginalSize(const QSizeF &size);

    virtual QByteArray serialize() const;
    virtual void deserialize(QByteArray &data);

signals:
    void marginsChanged(Margins);
    void groupChanged(QString);
    void stretchableChanged(bool);
    void showStretchabilityChanged(bool);

private slots:
    void childGeometryChanged(QRectF geometry);

protected:
    explicit BandInterface(BandInterfacePrivate *dd, QObject * parent);

    virtual void childAdded(BaseItemInterface * item);
    virtual void childRemoved(BaseItemInterface * item);
    virtual bool adjustSize();

private:
    Q_DECLARE_PRIVATE(BandInterface)
};

} //namespace

Q_DECLARE_INTERFACE(CuteReport::BandInterface, "CuteReport.BandInterface/1.0")

#endif
