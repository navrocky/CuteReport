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
#include "moduleinterface.h"
#include "core.h"

#include <QAction>
#include <QMenu>

namespace CuteDesigner {


ModuleInterface::ModuleInterface(QObject *parent) :
    QObject(parent)
{
}


ModuleInterface::~ModuleInterface()
{

}


CuteDesigner::Core * ModuleInterface::core()
{
    return m_core;
}


void ModuleInterface::init(CuteDesigner::Core *core)
{
    m_core = core;
}


QList<CuteDesigner::DesignerMenu*> ModuleInterface::mainMenu()
{
    return QList<CuteDesigner::DesignerMenu*>();
}

Core::StdActions ModuleInterface::stdActions()
{
    return Core::StdActions(0);
}


QAction * ModuleInterface::createAction(const QString & objectName, const QString & text, const QString & iconPath, const QString & keySequence, const char * method)
{
    QAction * action = new QAction(this);
    action->setObjectName(objectName);
    action->setText(text);
    QIcon icon;
    icon.addFile(iconPath, QSize(), QIcon::Normal, QIcon::Off);
    action->setIcon(icon);
    if (!keySequence.isEmpty())
        action->setShortcut(QKeySequence(keySequence));
    connect(action, SIGNAL(triggered()), this, method);
    return action;
}


DesignerMenu::DesignerMenu(QWidget * parent,  const QString & title, int _mainPriority, int _subPriority)
    :menu(new QMenu(title, parent)),
      mainPriority(_mainPriority),
      subPriority(_subPriority)
{

}


} //namespace
