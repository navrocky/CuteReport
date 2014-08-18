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
#ifndef TITLE_H
#define TITLE_H

#include <bandinterface.h>
#include "bandinterface_p.h"
#include "globals.h"

class TitlePrivate;

class  Title : public CuteReport::BandInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteReport.BandInterface/1.0")
#endif
    Q_INTERFACES(CuteReport::BandInterface)

public:
    Title(QObject * parent = 0);
    ~Title();

    virtual BaseItemInterface *createInstance(QObject * parent) const;
    virtual BaseItemInterface * clone();
    virtual QByteArray serialize();
    virtual void deserialize(QByteArray &data);
    virtual bool canContain(QObject * object);

    CuteReport::BandInterface::LayoutType layoutType(){ return LayoutTop;}
    int layoutPriority() const { return 75;}
    CuteReport::BandInterface::AccomodationType accommodationType(){ return AccomodationFirstPage;}

    QIcon itemIcon() const;
    QString moduleName() const;
    QString itemGroup() const;

    virtual CuteReport::RenderedItemInterface * render(int customDPI = 0);
    virtual CuteReport::RenderedItemInterface * renderNewPage(int customDPI = 0);

protected:
    Title(TitlePrivate &dd, QObject * parent);

private:
    Q_DECLARE_PRIVATE(Title)
};

class TitlePrivate : public CuteReport::BandInterfacePrivate
{
};

QDataStream &operator<<(QDataStream &s, const TitlePrivate &p);
QDataStream &operator>>(QDataStream &s, TitlePrivate &p);

#endif
