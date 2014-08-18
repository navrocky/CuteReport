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

#ifndef RENDEREDREPORT_H
#define RENDEREDREPORT_H

#include "renderedreportinterface.h"
#include "globals.h"

namespace CuteReport
{

class CUTEREPORT_EXPORTS RenderedReport : public RenderedReportInterface
{
    Q_OBJECT
public:
    explicit RenderedReport(QObject *parent = 0);
    virtual ~RenderedReport();

    virtual int pageCount() const;
    virtual CuteReport::RenderedPageInterface * page(int number);

    virtual int dpi() const;
    virtual void setDpi(int dpi);

    virtual void setRenderedPages(const QList<CuteReport::RenderedPageInterface*> & renderedPages);

private:
    QList<CuteReport::RenderedPageInterface* > m_pages;
    int m_dpi;
};


}  //namespace

#endif // RENDEREDREPORT_H
