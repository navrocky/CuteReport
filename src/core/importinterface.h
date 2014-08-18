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
#ifndef IMPORTINTERFACE_H
#define IMPORTINTERFACE_H

#include "reportplugininterface.h"
#include "globals.h"

namespace CuteReport
{

class ReportInterface;

class CUTEREPORT_EXPORTS ImportInterface : public ReportPluginInterface
{
//    Q_INTERFACES(CuteReport::ReportPluginInterface)
    Q_OBJECT
public:
    explicit ImportInterface(QObject * parent):ReportPluginInterface(parent) {}
    virtual ~ImportInterface(){}

    virtual ImportInterface * clone() = 0;

    virtual QStringList fileExtensions() const = 0;
    virtual bool canHandle(const QString & url) const = 0;
    virtual CuteReport::ReportInterface * importReport(const QString & url) const = 0;

protected:
    virtual ImportInterface * createInstance(QObject * parent = 0) const = 0;
    explicit ImportInterface(const ImportInterface & source)
        :ReportPluginInterface(source.parent()){}

    friend class ReportCore;
};

}

Q_DECLARE_INTERFACE(CuteReport::ImportInterface, "CuteReport.ImportInterface/1.0")


#endif // IMPORTINTERFACE_H
