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

#include "scriptplugin.h"

#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_cutereport_bindings(QScriptValue &);

CuteReportScriptPlugin::CuteReportScriptPlugin(QObject *parent)
    :ScriptExtensionInterface(parent)
{

}


CuteReportScriptPlugin::~CuteReportScriptPlugin()
{

}


QString CuteReportScriptPlugin::moduleShortName() const
{
    return "Script";
}


QStringList CuteReportScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("cutereport");
    list << QLatin1String("cutereport.std");
    return list;
}


void CuteReportScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("cutereport")) {
    } else if (key == QLatin1String("cutereport.std")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_cutereport_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "CuteReportScriptPlugin::initialize", qPrintable(key));
    }
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Script, CuteReportScriptPlugin)
#endif
