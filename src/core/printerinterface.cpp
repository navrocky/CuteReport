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
#include "printerinterface.h"

namespace CuteReport {

PrinterInterface::PrinterInterface(QObject *parent) :
    ReportPluginInterface(parent), m_currentProperty(0)
{
}


PrinterInterface::PrinterInterface(const PrinterInterface &dd, QObject *parent) :
    ReportPluginInterface(dd, parent),
    m_printerName(dd.m_printerName),
    m_currentProperty(dd.m_currentProperty)
{
}


PrinterInterface::~PrinterInterface()
{

}

QString PrinterInterface::printerName() const
{
    return m_printerName;
}


void PrinterInterface::setPrinterName(const QString & printerName)
{
    if (printerName == m_printerName)
        return;

    m_printerName = printerName;

    emit printerNameChanged(m_printerName);
    emit changed();
}


QString PrinterInterface::_current_property_description() const
{
    QString propertyName = metaObject()->property(m_currentProperty).name();

    if (propertyName == "printerName")
        return tr("Current printer object name");

    return QString();
}


} //namespace
