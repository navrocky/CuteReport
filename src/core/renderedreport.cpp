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

#include "renderedreport.h"

namespace CuteReport
{


RenderedReport::RenderedReport(QObject *parent) :
    RenderedReportInterface(parent),
    m_dpi(0)
{
}


RenderedReport::~RenderedReport()
{
    qDeleteAll(m_pages);
}


int RenderedReport::pageCount() const
{
    return m_pages.size();
}


RenderedPageInterface *RenderedReport::page(int number)
{
    if (number < 0 || number >= m_pages.size())
        return 0;
    return m_pages.at(number);
}


int RenderedReport::dpi() const
{
    return m_dpi;
}


void RenderedReport::setDpi(int dpi)
{
    m_dpi =  dpi;
}


void RenderedReport::setRenderedPages(const QList<CuteReport::RenderedPageInterface*> & renderedPages)
{
    qDeleteAll(m_pages);
    m_pages.clear();
    m_pages = renderedPages;
}

}  //namespace
