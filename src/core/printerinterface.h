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
#ifndef PRINTERINTERFACE_H
#define PRINTERINTERFACE_H

#include "reportplugininterface.h"
#include "reportinterface.h"
#include "globals.h"

#include <QtGui>
#include <QWidget>

namespace CuteReport {

class PrinterHelperInterface;

class CUTEREPORT_EXPORTS PrinterInterface : public ReportPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(CuteReport::ReportPluginInterface)

    Q_PROPERTY(QString printerName READ printerName WRITE setPrinterName NOTIFY printerNameChanged)

    Q_PROPERTY(int _current_property READ _currentProperty WRITE _setCurrentProperty DESIGNABLE false)
    Q_PROPERTY(QString _current_property_description READ _current_property_description DESIGNABLE false)

public:
    explicit PrinterInterface(QObject *parent = 0);
    virtual ~PrinterInterface();

    virtual PrinterHelperInterface * helper() = 0;
    virtual PrinterInterface * createInstance(QObject * parent = 0) const = 0;
    virtual PrinterInterface * clone() const = 0;

    virtual bool print(CuteReport::ReportInterface * report) = 0;

    QString printerName() const;
    void setPrinterName(const QString & printerName);

    virtual QString lastError() = 0;

    virtual void _setCurrentProperty(int num) { m_currentProperty = num; }
    virtual int _currentProperty() { return m_currentProperty; }
    virtual QString _current_property_description() const;

signals:
    void printerNameChanged(const QString &);

protected:
    explicit PrinterInterface(const PrinterInterface &dd, QObject * parent);

    QString m_printerName;
    int m_currentProperty;

    friend class ReportCore;
};


class PrinterHelperInterface: public QWidget
{
    Q_OBJECT
public:
    PrinterHelperInterface(PrinterInterface * /*printer*/){}
};


}

Q_DECLARE_INTERFACE(CuteReport::PrinterInterface, "CuteReport.PrinterInterface/1.0")
Q_DECLARE_INTERFACE(CuteReport::PrinterHelperInterface, "CuteReport.PrinterHelperInterface/1.0")
//Q_DECLARE_INTERFACE(CuteReport::PrinterOptionsInterface, "CuteReport.PrinterOptionsInterface/1.0")

#endif // PRINTERINTERFACE_H
