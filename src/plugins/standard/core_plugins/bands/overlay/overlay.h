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
#ifndef OVERLAY_H
#define OVERLAY_H

#include <bandinterface.h>
#include "bandinterface_p.h"
#include "globals.h"

class OverlayPrivate;

class  Overlay : public CuteReport::BandInterface
{
	Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteReport.BandInterface/1.0")
#endif
    Q_INTERFACES(CuteReport::BandInterface)

public:
    Overlay(QObject * parent = 0);
    ~Overlay();

    virtual void moduleInit();

    virtual BaseItemInterface *createInstance(QObject * parent) const;
    virtual QByteArray serialize();
    virtual void deserialize(QByteArray &data);
    virtual bool canContain(QObject * object);

    CuteReport::BandInterface::LayoutType layoutType() const { return LayoutFree;}
    int layoutPriority() const { return 100;}
    CuteReport::BandInterface::AccomodationType accommodationType() const { return AccomodationEveryPage;}

    QIcon itemIcon() const;
    QString moduleShortName() const;
    virtual QString suitName() const { return "Standard"; }
    QString itemGroup() const;

    virtual bool renderPrepare();
    virtual CuteReport::RenderedItemInterface * renderView();

private:
    Q_DECLARE_PRIVATE(Overlay)

    Overlay(OverlayPrivate *dd, QObject * parent);
    virtual BaseItemInterface * itemClone() const;
};

class OverlayPrivate : public CuteReport::BandInterfacePrivate
{
};

QDataStream &operator<<(QDataStream &s, const OverlayPrivate &p);
QDataStream &operator>>(QDataStream &s, OverlayPrivate &p);

#endif
