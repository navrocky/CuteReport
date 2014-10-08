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
#ifndef REPORTPLUGININTERFACE_H
#define REPORTPLUGININTERFACE_H

#include <QObject>
#include <QStringList>

#include "globals.h"
#include "types.h"

namespace CuteReport
{

class ReportCore;


struct ModuleInfo {
    QString moduleName;
    int minVersion;
    int maxVersion;
    ModuleType type;
};


class CUTEREPORT_EXPORTS ReportPluginInterface : public QObject
{
    Q_OBJECT

    /// workaround for sending signal when objectName property changed
#if QT_VERSION <= 0x050000
    Q_PROPERTY(QString objectName READ objectName WRITE setObjectName NOTIFY objectNameChanged)
#endif

public:
    explicit ReportPluginInterface(QObject *parent = 0);
    virtual ~ReportPluginInterface();

    /** uses report core only for sending logs. It shall not set m_reportCore variable */
    virtual void moduleInit(){}

    ReportCore * reportCore() const;
    void setReportCore(ReportCore *reportCore);

    virtual int moduleVersion() const;
    virtual QString moduleShortName() const = 0;
    virtual QString suitName() const = 0;
    QString moduleFullName() const { return suitName() + "::" + moduleShortName();}

    // replaces modules in this with extended functionality without removing it from list
    virtual QStringList extendsModules() const {return QStringList();}

    // removes modules from module list
    virtual QStringList removesModules() const {return QStringList();}

    // defines mudule's dependencies including name maxVersion, MinVersion, type, etc.
    virtual QList<ModuleInfo> dependencies() const { return QList<ModuleInfo>();}

#if QT_VERSION <= 0x050000
    void setObjectName(const QString &name);
#endif

signals:
    void changed();
    void objectNameChanged(QString name);

protected:
    ReportPluginInterface(const ReportPluginInterface & dd, QObject * parent);

private:
    ReportCore * m_reportCore;
};

} //namespace

Q_DECLARE_INTERFACE(CuteReport::ReportPluginInterface, "CuteReport.ReportPluginInterface/1.0")
#endif // REPORTPLUGININTERFACE_H
