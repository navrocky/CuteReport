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
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/
#include "cutereport.h"
#include "reportcore.h"

namespace CuteReport {



Report::Report(QSettings * settings, QObject *parent) :
    QObject(parent)
{
    m_core = new CuteReport::ReportCore(this, settings);
}


Report::Report(QObject *parent) :
    QObject(parent)
{
    m_core = new CuteReport::ReportCore(this);
}


Report::~Report()
{
}


CuteReport::ReportCore * Report::core() const
{
    return m_core;
}


ReportInterface * Report::loadReport(const QString & url)
{
    return m_core->loadReport(url, &m_lastError);
}


void Report::process(ReportInterface * report)
{
    m_core->render(report);
}


QString Report::lastError()
{
    return m_lastError;
}


void Report::setVariableValue(const QString & name, const QString & value)
{
    m_variables.insert(name, value);
}


void Report::clearVariables()
{

}


} //namespace
