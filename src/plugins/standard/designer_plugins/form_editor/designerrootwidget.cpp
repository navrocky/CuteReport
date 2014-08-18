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
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/

#include "designerrootwidget.h"
#include "ui_designerrootwidget.h"
#include <QtGui>


typedef QList<QAction *> ActionList;

DesignerRootWidget::DesignerRootWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DesignerRootWidget)
{
    ui->setupUi(this);
}

DesignerRootWidget::~DesignerRootWidget()
{
    delete ui;
}


QMdiArea *DesignerRootWidget::mdiArea() const
{
    return ui->mdiArea;
}


QMenuBar *DesignerRootWidget::menuBar() const
{
    return  m_menubar;
}

// Create a MDI subwindow for the form.
QMdiSubWindow *DesignerRootWidget::createMdiSubWindow(QWidget *fw, Qt::WindowFlags f, const QKeySequence &designerCloseActionShortCut)
{
    QMdiSubWindow *rc = mdiArea()->addSubWindow(fw, f);
    // Make action shortcuts respond only if focused to avoid conflicts with
    // designer menu actions
    if (designerCloseActionShortCut == QKeySequence(QKeySequence::Close)) {
        const ActionList systemMenuActions = rc->systemMenu()->actions();
        if (!systemMenuActions.empty()) {
            const ActionList::const_iterator cend = systemMenuActions.constEnd();
            for (ActionList::const_iterator it = systemMenuActions.constBegin(); it != cend; ++it) {
                if ( (*it)->shortcut() == designerCloseActionShortCut) {
                    (*it)->setShortcutContext(Qt::WidgetShortcut);
                    break;
                }
            }
        }
    }
    return rc;
}



void DesignerRootWidget::setMenuBar(QMenuBar *menubar)
{
    m_menubar = menubar;
}


void DesignerRootWidget::addFormEditor(QWidget * widget)
{
    ui->layoutForm->addWidget(widget);
}


void DesignerRootWidget::addWidgetBar(QWidget * widget)
{
    ui->layoutWidgetBar->addWidget(widget);
}


void DesignerRootWidget::addObjectInspector(QWidget * widget)
{
    ui->layoutObjectInspector->addWidget(widget);
}


void DesignerRootWidget::addPropertyEditor(QWidget * widget)
{
    ui->layoutProperty->addWidget(widget);
}


void DesignerRootWidget::addActionEditor(QWidget * widget)
{
    ui->tabs->addTab(widget, "Actions");
}


void DesignerRootWidget::addSignalSlotEditor(QWidget * widget)
{
    ui->tabs->addTab(widget, "Signals & Slots");
}


void DesignerRootWidget::addResourceEditor(QWidget * widget)
{
    ui->tabs->addTab(widget, "Resources");
}

void DesignerRootWidget::reloadSettings(QSettings *settings)
{
    QVariant value;
    if ((value = settings->value("UIDesigner/splitter1State")).isNull())
        ui->splitter->setSizes( QList<int>() <<  height()*0.8 << height()*0.2);
    else
        ui->splitter->restoreState(value.toByteArray());

    if ((value = settings->value("UIDesigner/splitter2State")).isNull())
        ui->splitter_2->setSizes( QList<int>() << width()*0.5 << width()*0.5  );
    else
        ui->splitter_2->restoreState(value.toByteArray());

    if ((value = settings->value("UIDesigner/splitter3State")).isNull())
        ui->splitter_3->setSizes( QList<int>() << width()*0.2 << width()*0.6 << width()*0.2 );
    else
        ui->splitter_3->restoreState(value.toByteArray());
}


void DesignerRootWidget::saveSettings(QSettings *settings)
{
    settings->setValue("UIDesigner/splitter1State", ui->splitter->saveState());
    settings->setValue("UIDesigner/splitter2State", ui->splitter_2->saveState());
    settings->setValue("UIDesigner/splitter3State", ui->splitter_3->saveState());
}
