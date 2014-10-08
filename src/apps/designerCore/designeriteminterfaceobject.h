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
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/

#ifndef DESIGNERITEMINTERFACEOBJECT_H
#define DESIGNERITEMINTERFACEOBJECT_H

#include "designeriteminterface.h"
#include "core.h"

class DesignerItemInterfaceObject : public CuteReport::DesignerItemInterface
{
public:
    explicit DesignerItemInterfaceObject(CuteDesigner::Core * core);

    virtual bool moduleExists(const QString & moduleName);
    virtual QIcon  moduleIcon(const QString & moduleName);
    virtual QString  moduleToolTip(const QString & moduleName);

    virtual QWidget * createWidget(const QString & moduleName);
    virtual QString getResult(QWidget * toolWidget);

private:
    CuteDesigner::ModuleInterface * findModule(const QString & moduleName);

    CuteDesigner::Core * m_core;

};

#endif // DESIGNERITEMINTERFACEOBJECT_H
