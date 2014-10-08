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
#ifndef ITEMINTERFACE_H
#define ITEMINTERFACE_H

#include <QObject>
#include "baseiteminterface.h"
#include "globals.h"

namespace CuteReport
{

class ItemInterfacePrivate;

class CUTEREPORT_EXPORTS ItemInterface : public BaseItemInterface
{
    Q_OBJECT

    Q_INTERFACES(CuteReport::BaseItemInterface)
    Q_ENUMS(ShiftMode)

    Q_PROPERTY(QString shiftMode READ shiftModeStr WRITE setShiftModeStr NOTIFY shiftModeChanged)

    Q_PROPERTY(ShiftMode shiftMode READ shiftMode WRITE setShiftMode NOTIFY shiftModeChanged DESIGNABLE false)
    Q_PROPERTY(QStringList _shiftMode_variants READ _shiftMode_variants DESIGNABLE false)

public:
    enum ShiftMode { AlwaysShift, DontShift, ShiftWhenOverlapped };

    explicit ItemInterface(QObject * parent);
    virtual ~ItemInterface();

    virtual QByteArray serialize() const;
    virtual void deserialize(QByteArray &data);

    virtual ShiftMode shiftMode() const;
    virtual void setShiftMode(ItemInterface::ShiftMode shiftMode);
    virtual QString shiftModeStr() const;
    virtual void setShiftModeStr(const QString & shiftMode);

    virtual QString  _current_property_description() const;
    virtual QStringList _shiftMode_variants() const;

    static ShiftMode shiftModeFromString(const QString & mode);
    static QString shiftModeToString(ShiftMode mode);

signals:
    void shiftModeChanged(ShiftMode shiftMode);
    void shiftModeChanged(QString shiftMode);

protected:
    ItemInterface(ItemInterfacePrivate *dd, QObject * parent);

private:
    Q_DECLARE_PRIVATE(ItemInterface)
};

} // namespace

Q_DECLARE_INTERFACE(CuteReport::ItemInterface, "CuteReport.ItemInterface/1.0")
Q_DECLARE_METATYPE(CuteReport::ItemInterface::ShiftMode)

#endif // ITEMINTERFACE_H
