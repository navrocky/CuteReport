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

#ifndef CUTEREPORT_H
#define CUTEREPORT_H

#include <QObject>
#include <QVariantHash>

class QSettings;

namespace CuteReport {
class ReportCore;
class ReportInterface;

class Report : public QObject
{
    Q_OBJECT
public:
    explicit Report(QObject *parent = 0);

    /** Constructor for pass special QSettings object pointer.
     *  CuteReport uses QApplication::organizationName() and QApplication::applicationName()
     *  for settings if they set. In other case it will be using
     *  "ExaroLogic" as organisation and "Cute Report" as application name.
     *  Only first QSetting passed to CuteReport core will work. Other will be skipped and
     *  you have no chance to change it. So use appropriate constructor as early as possible in your application.
     */
    explicit Report(QSettings * settings, QObject *parent = 0);

    ~Report();

    /** load report from url
     *  url should look like: file:file_path, file://fullpath
     *                        git:file
     *  return 0 means error occured */
    ReportInterface * loadReport(const QString & url);

    /** process report to preview widget */
    void process(ReportInterface * report);

    /** process report to needed format */
//    void process(const QString & url, const QString & format, const QString & outputDir = QString());

    /** batch report list processing to needed format */
//    void batchProcess(const QStringList & list, const QString & format, const QString & outputDir = QString());

    QString lastError();

    /** variables listed here will be assigned to each report processed
        for set individual variables use ReportInterface instead
    */
    void setVariableValue(const QString & name, const QString & value);
    void clearVariables();

    CuteReport::ReportCore * core() const;
    
signals:
    void processed(ReportInterface * report);

public slots:
    
private:
    CuteReport::ReportCore * m_core;
    QString m_lastError;
    QVariantHash m_variables;
};

} //namespace
#endif // CUTEREPORT_H
