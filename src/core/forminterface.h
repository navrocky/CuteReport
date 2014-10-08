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
#ifndef FORMINTERFACE_H
#define FORMINTERFACE_H

#include "reportplugininterface.h"
#include <QWidget>
#include <QVariant>
#include "globals.h"

namespace CuteReport
{

class FormHelperInterface;

class CUTEREPORT_EXPORTS FormInterface : public ReportPluginInterface
{
    Q_OBJECT
    Q_PROPERTY(QString data READ data WRITE setData NOTIFY dataChanged)

public:
    explicit FormInterface(QObject *parent = 0);
    virtual ~FormInterface(){}

    virtual void init(){}
    virtual FormHelperInterface * helper() {return 0;}
    virtual FormInterface * clone() const = 0;

    virtual QString data() const = 0;
    virtual void setData(const QString &data) = 0;

    virtual QStringList scriptingStrings(){return QStringList();}

    virtual void renderInit() = 0;
    virtual void renderReset() = 0;

    Q_INVOKABLE virtual void show() = 0;
    Q_INVOKABLE virtual QVariant invoke(const QString & objectName, const QString & method, const QVariant & arg1 = QVariant(),
                            const QVariant & arg2 = QVariant(), const QVariant & arg3 = QVariant(),
                            const QVariant & arg4 = QVariant(), const QVariant & arg5 = QVariant(), const QVariant & arg6 = QVariant()) = 0;

signals:
    void accepted();
    void rejected();
    void dataChanged(const QString &);
    void scriptingStringsChanged();

protected:
    virtual FormInterface * createInstance(QObject * parent) const = 0;

    friend class ReportCore;
};

class FormHelperInterface: public QWidget
{

};

}

Q_DECLARE_INTERFACE(CuteReport::FormInterface, "CuteReport.FormInterface/1.0")
#endif // FORMINTERFACE_H
