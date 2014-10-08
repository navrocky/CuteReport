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
#ifndef MODULEINTERFACE_H
#define MODULEINTERFACE_H

#include <QObject>
#include <QPointer>
#include <QIcon>

#include "designer_globals.h"
#include "core.h"

class QAction;
class QMenu;

namespace CuteDesigner {

class Core;

struct  DesignerAction  {
    QAction * action;
    QString name;
    QString parentName;
};

struct DESIGNER_EXPORTS DesignerMenu {
    DesignerMenu():menu(0), mainPriority(0), subPriority(0){}
    DesignerMenu(QWidget * parent,  const QString & title, int _mainPriority, int _subPriority);
    QMenu * menu;
    int mainPriority;
    int subPriority;
};


class DESIGNER_EXPORTS ModuleInterface : public QObject
{
    Q_OBJECT
public:
    explicit ModuleInterface(QObject *parent = 0);
    virtual ~ModuleInterface();

    virtual void init(CuteDesigner::Core *core);
    virtual CuteDesigner::Core * core();

    virtual void reloadSettings() = 0;
    virtual void saveSettings() = 0;

    virtual void activate() = 0;
    virtual void deactivate() = 0;
    virtual void sync() = 0;

    // module might have no view;
    virtual QWidget * view() = 0;

    // for modules that can have some instancies ie widget, like editors, etc.
    virtual QWidget * createWidget(QWidget * parent = 0) {Q_UNUSED(parent); return 0;}
    virtual QString getResult(QWidget * editorWidget) {Q_UNUSED(editorWidget); return QString();}

    virtual QIcon icon() = 0;
    virtual QString name() = 0;
    virtual QString toolTip() {return QString();}
    virtual qint16 priority() = 0;

    virtual QList<CuteDesigner::DesignerMenu*> mainMenu();
    virtual CuteDesigner::Core::StdActions stdActions();
    
signals:
    void stdActionsChanged(CuteDesigner::Core::StdActions actions);

protected:
    QAction * createAction(const QString & objectName, const QString & text, const QString & iconPath, const QString & keySequence, const char * method);

    CuteDesigner::Core * m_core;
};

} //namespace

Q_DECLARE_INTERFACE(CuteDesigner::ModuleInterface, "CuteDesigner.ModuleInterface/1.0")
#endif // MODULEINTERFACE_H
