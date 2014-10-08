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

#include "designeriteminterfaceobject.h"
#include "moduleinterface.h"

#include <QWidget>

DesignerItemInterfaceObject::DesignerItemInterfaceObject(CuteDesigner::Core * core)
    :DesignerItemInterface()
    , m_core(core)
{
}


bool DesignerItemInterfaceObject::moduleExists(const QString &moduleName)
{
    CuteDesigner::ModuleInterface * module = findModule(moduleName);
    return module;
}


QIcon DesignerItemInterfaceObject::moduleIcon(const QString &moduleName)
{
    CuteDesigner::ModuleInterface * module = findModule(moduleName);
    return module ? module->icon() : QIcon();
}


QString DesignerItemInterfaceObject::moduleToolTip(const QString &moduleName)
{
    CuteDesigner::ModuleInterface * module = findModule(moduleName);
    return module ? module->toolTip() : QString();
}


QWidget *DesignerItemInterfaceObject::createWidget(const QString &moduleName)
{
    CuteDesigner::ModuleInterface * module = findModule(moduleName);
    if (!module)
        return 0;

    QWidget * widget = module->createWidget();
    widget->setAccessibleName(moduleName);
    return widget;
}


QString DesignerItemInterfaceObject::getResult(QWidget *toolWidget)
{
    CuteDesigner::ModuleInterface * module = findModule(toolWidget->accessibleName());
    if (!module)
        return QString();
    return module->getResult(toolWidget);
}


CuteDesigner::ModuleInterface *DesignerItemInterfaceObject::findModule(const QString &moduleName)
{
    foreach (CuteDesigner::ModuleInterface * module, m_core->modules()) {
        if (module->name() == moduleName)
            return module;
    }

    return 0;
}
