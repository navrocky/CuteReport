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
#ifndef SCRIPTEXTENSIONINTERFACE_H
#define SCRIPTEXTENSIONINTERFACE_H

#include "reportplugininterface.h"
#include "reportinterface.h"
#include "globals.h"

#include <QtScript/QScriptExtensionPlugin>

namespace CuteReport {

class CUTEREPORT_EXPORTS ScriptExtensionInterface : public ReportPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(CuteReport::ReportPluginInterface)

public:
    explicit ScriptExtensionInterface(QObject *parent = 0);
    virtual ~ScriptExtensionInterface();

    virtual QStringList keys() const = 0;
    virtual void initialize(const QString &key, QScriptEngine *engine) = 0;

protected:
    friend class ReportCore;
};


}

Q_DECLARE_INTERFACE(CuteReport::ScriptExtensionInterface, "CuteReport.ScriptExtensionInterface/1.0")

#endif // SCRIPTEXTENSIONINTERFACE_H
