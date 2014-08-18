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

#ifndef RENDEREDREPORTINTERFACE_H
#define RENDEREDREPORTINTERFACE_H

#include <QObject>
#include "pageinterface.h"

namespace CuteReport
{

class RendererInterface;

class CUTEREPORT_EXPORTS RenderedReportInterface : public QObject
{
    Q_OBJECT
public:
    explicit RenderedReportInterface(QObject *parent = 0);
    virtual ~RenderedReportInterface(){}

    virtual int pageCount() const = 0;
    virtual CuteReport::RenderedPageInterface * page(int number) = 0;

    virtual int dpi() const = 0;
    virtual void setDpi(int dpi) = 0;

    virtual void setRenderedPages(const QList<CuteReport::RenderedPageInterface*> & renderedPages) = 0;
};


}  //namespace

#endif // RENDEREDREPORTINTERFACE_H
