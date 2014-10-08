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
#ifndef EXPORTNTERFACE_H
#define EXPORTNTERFACE_H

#include "reportplugininterface.h"
#include "reportinterface.h"
#include "globals.h"

#include <QtGui>
#include <QWidget>

namespace CuteReport {

class ExportHelperInterface;

class CUTEREPORT_EXPORTS ExportInterface : public ReportPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(CuteReport::ReportPluginInterface)

public:
    explicit ExportInterface(QObject *parent = 0);
    virtual ~ExportInterface();

    virtual QString format() const = 0;
    virtual QIcon icon() const = 0;

    virtual ExportInterface * clone() const = 0;
    virtual ExportInterface * createInstance( QObject * parent = 0) const = 0;
    virtual ExportHelperInterface * helper() = 0;

    virtual bool process(CuteReport::ReportInterface * report, const QString & outputUrl) = 0;
    virtual bool processWithDialog(CuteReport::ReportInterface * report, const QString & outputUrl = QString()) = 0;

    virtual QString lastError() = 0;

protected:
    ExportInterface(const ExportInterface & dd, QObject * parent);

signals:

};


class ExportHelperInterface: public QWidget
{
    Q_OBJECT
public:
    ExportHelperInterface(ExportInterface * /*exporter*/){}
};


}

Q_DECLARE_INTERFACE(CuteReport::ExportInterface, "CuteReport.ExportInterface/1.0")
Q_DECLARE_INTERFACE(CuteReport::ExportHelperInterface, "CuteReport.ExportHelperInterface/1.0")

#endif // EXPORTNTERFACE_H
