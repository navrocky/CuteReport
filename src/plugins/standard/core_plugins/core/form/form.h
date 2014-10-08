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
#ifndef FORM_H
#define FORM_H

#include "forminterface.h"
#include <QPointer>

class MainThreadObject;

namespace CuteReport {
}

class Form : public CuteReport::FormInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteReport.FormInterface/1.0")
#endif
    Q_INTERFACES(CuteReport::FormInterface)

public:
    explicit Form(QObject *parent = 0);
    virtual ~Form();

    virtual QString moduleShortName() const {return "Form";}
    virtual QString suitName() const { return "Standard"; }
    virtual CuteReport::FormHelperInterface * helper();
    virtual CuteReport::FormInterface * createInstance(QObject * parent) const;
    virtual CuteReport::FormInterface * clone() const;

    virtual QString data() const;
    virtual void setData(const QString &data);

    virtual void renderInit();
    virtual void renderReset();

    virtual void show();
    virtual QVariant invoke(const QString & objectName, const QString & method, const QVariant & arg1 = QVariant(),
                            const QVariant & arg2 = QVariant(), const QVariant & arg3 = QVariant(),
                            const QVariant & arg4 = QVariant(), const QVariant & arg5 = QVariant(), const QVariant & arg6 = QVariant());

private:
    explicit Form(const Form &dd, QObject * parent);

    QString m_data;
    MainThreadObject* m_processor;
};

#endif // FORM_H
