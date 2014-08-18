/***************************************************************************
 *   This file is part of the CuteReport project                           *
 *   Copyright (C) 2012-2013 by Alexander Mikhalov                              *
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
#ifndef EXPORTSVG_H
#define EXPORTSVG_H

#include "exportinterface.h"
#include "globals.h"
#include "types.h"

namespace CuteReport {
}


class ExporSVG : public CuteReport::ExportInterface
{
    Q_OBJECT
    Q_INTERFACES(CuteReport::ExportInterface)

public:
    explicit ExporSVG(QObject *parent = 0);
    ~ExporSVG();

    virtual QString moduleName() const {return QString("SVG");}
    virtual QString format() const {return QString("SVG");}
    virtual QIcon icon() const;

    CuteReport::ExportHelperInterface * helper();
    virtual ExportInterface * createInstance( QObject * parent = 0) const;

    virtual bool process(CuteReport::ReportInterface * report, const QString & outputUrl);
    virtual bool processWithDialog(CuteReport::ReportInterface * report, const QString & outputUrl = QString());

    virtual QString lastError();

signals:

private:
    bool process_(CuteReport::ReportInterface * report, const QString &outputUrl, bool forceShowDialog = false);

private:
    QString m_lastError;
};


#endif // EXPORTSVG_H
