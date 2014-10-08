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
#ifndef DESIGNERITEMINTERFACE_H
#define DESIGNERITEMINTERFACE_H

#include "globals.h"

#include <QString>
#include <QIcon>


namespace CuteReport
{

class CUTEREPORT_EXPORTS DesignerItemInterface
{
public:
    explicit DesignerItemInterface();
    virtual ~DesignerItemInterface(){;}

    virtual bool moduleExists(const QString & moduleName) = 0;
    virtual QIcon  moduleIcon(const QString & moduleName) = 0;
    virtual QString  moduleToolTip(const QString & moduleName) = 0;

    virtual QWidget * createWidget(const QString & moduleName) = 0;
    virtual QString getResult(QWidget * toolWidget) = 0;
};

} //namespace

#endif // DESIGNERITEMINTERFACE_H
