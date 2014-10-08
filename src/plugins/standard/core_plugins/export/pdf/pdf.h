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
#ifndef EXPORTPDF_H
#define EXPORTPDF_H

#include "exportinterface.h"
#include "globals.h"
#include "types.h"

namespace CuteReport {
}


class ExportPDF : public CuteReport::ExportInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteReport.ExportInterface/1.0")
#endif

    Q_INTERFACES(CuteReport::ExportInterface)

public:
    explicit ExportPDF(QObject *parent = 0);
    ~ExportPDF();

    virtual QString moduleShortName() const {return QString("PDF");}
    virtual QString suitName() const { return "Standard"; }
    virtual QString format() const {return QString("PDF");}
    virtual QIcon icon() const;

    virtual ExportInterface *clone() const;
    virtual ExportInterface * createInstance( QObject * parent = 0) const;
    virtual CuteReport::ExportHelperInterface * helper();

    virtual bool process(CuteReport::ReportInterface * report, const QString & outputUrl);
    virtual bool processWithDialog(CuteReport::ReportInterface * report, const QString & outputUrl = QString());

    virtual QString lastError();

protected:
    ExportPDF(const ExportPDF & dd, QObject * parent);

private:
    bool process_(CuteReport::ReportInterface * report, const QString &outputUrl, bool forceShowDialog = false);

    QString m_lastError;
};


#endif // EXPORTPDF_H
