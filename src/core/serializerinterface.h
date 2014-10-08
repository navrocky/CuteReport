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
#ifndef SERIALIZERINTERFACE_H
#define SERIALIZERINTERFACE_H

#include "reportplugininterface.h"

namespace CuteReport
{

class CUTEREPORT_EXPORTS SerializerInterface : public ReportPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(CuteReport::ReportPluginInterface)

public:
    explicit SerializerInterface(QObject * parent);
    virtual ~SerializerInterface();

    virtual QByteArray serialize(const QObject * object, bool * ok = 0) = 0;
    virtual QObject * deserialize(const QByteArray &data, bool * ok = 0) = 0;

    virtual QString lastError() = 0;

signals:

protected:
    virtual SerializerInterface * createInstance(QObject * parent = 0) const = 0;
    explicit SerializerInterface(const SerializerInterface & source)
        :ReportPluginInterface(source.parent()){}

    friend class ReportCore;
};

}

Q_DECLARE_INTERFACE(CuteReport::SerializerInterface, "CuteReport.SerializerInterface/1.0")

#endif // SERIALIZERINTERFACE_H
