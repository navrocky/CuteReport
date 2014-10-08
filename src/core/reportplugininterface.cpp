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
#include "reportplugininterface.h"
#include "reportcore.h"
#include "log/log.h"

namespace CuteReport
{


ReportPluginInterface::ReportPluginInterface(QObject *parent) :
    QObject(parent)
    ,m_reportCore(0)
{
    Log::refCounterInc();
}


ReportPluginInterface::ReportPluginInterface(const ReportPluginInterface & dd, QObject * parent)
    :QObject(parent)
    ,m_reportCore(dd.m_reportCore)
{
    setObjectName(dd.objectName());
    Log::refCounterInc();
}


ReportPluginInterface::~ReportPluginInterface()
{
    Log::refCounterDec();
}


ReportCore * ReportPluginInterface::reportCore() const
{
    return m_reportCore;
}


void ReportPluginInterface::setReportCore(ReportCore * reportCore)
{
    m_reportCore = reportCore;
}




int ReportPluginInterface::moduleVersion() const
{
    return 1;
}


#if QT_VERSION <= 0x050000
void ReportPluginInterface::setObjectName(const QString &name)
{
    QObject::setObjectName(name);
    emit objectNameChanged(name);
    emit changed();
}
#endif

} //namespace
