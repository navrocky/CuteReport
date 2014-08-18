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
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ****************************************************************************/
#include "uicontainer.h"
#include "ui_uicontainer.h"
#include "ui_editor.h"
#include "core.h"
#include "fancytabs.h"
#include "stdstoragedialog.h"

#include <QtGui>

UIcontainer::UIcontainer(UiEditor *editor, QWidget * designerWidget, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::uicontainer),
    m_editor(editor),
    m_designerWidget(designerWidget),
    m_updateTabTimer(new QTimer(this))
{
    ui->setupUi(this);

    ui->designerActions->addAction(ui->actionNew_Form);
//    ui->designerActions->addAction(ui->actionOpen_From_File);
    ui->designerActions->addAction(ui->actionDelete_Form);

    QAction * separator = new QAction(this);
    separator->setSeparator(true);

    ui->designerActions->addAction(separator);

    connect(ui->actionNew_Form, SIGNAL(triggered()), this, SLOT(slotNewFormClicked()));
    connect(ui->actionDelete_Form, SIGNAL(triggered()), this, SLOT(slotDeleteFormClicked()));
    connect(ui->actionOpen_From_File, SIGNAL(triggered()), this, SLOT(slotFromFileClicked()));
    connect(ui->tabs, SIGNAL(currentChanged(int)), this, SLOT(slotTabChanged(int)));
    connect(ui->tabs, SIGNAL(tabDoubleClicked(int)), this, SLOT(slotTabDoubleClicked(int)));

    ui->tabs->setSingleWidget(m_designerWidget);
}


UIcontainer::~UIcontainer()
{
    delete ui;
}


void UIcontainer::reloadSettings()
{
//    ui->tabs->setCurrentIndex(0);
    FancyTabWidget::Mode default_mode = FancyTabWidget::Mode_LargeSidebar;
    ui->tabs->setMode(FancyTabWidget::Mode(m_editor->core()->getSettingValue("FormEditor/tabMode", default_mode).toInt()));
}


void UIcontainer::saveSettings()
{
    m_editor->core()->setSettingValue("FormEditor/tabMode", ui->tabs->mode());
}


void UIcontainer::addDesignerActions(QList<QAction*> list)
{
    foreach(QAction * action, list)
        ui->designerActions->addAction(action);
}


void UIcontainer::addDesignerActions(QActionGroup* group)
{
    foreach(QAction * action, group->actions())
        ui->designerActions->addAction(action);
}


void UIcontainer::addTab(QString formName)
{
    if (!m_forms.contains(formName)) {
        ui->tabs->addTab(QIcon(":images/form_48.png"), formName);
        Form form;
        form.index = ui->tabs->count() - 1;
        m_forms.insert(formName, form);
    }
}


void UIcontainer::removeTab(const QString &formName)
{
    if (!m_forms.contains(formName))
        return;

    Form form = m_forms.value(formName);
    delete form.widget;
    m_forms.remove(formName);

    int index = ui->tabs->currentIndex();
    ui->tabs->removeAllTabs();
    foreach(QString key, m_forms.keys()) {
//        Form form = m_forms.value(key);
        ui->tabs->addTab( QIcon(":images/form_48.png"), key);
    }

    if (ui->tabs->count()) {
        ui->tabs->blockSignals(true);
        ui->tabs->setCurrentIndex( index < ui->tabs->count() ? index : --index);
        ui->tabs->blockSignals(false);
//        delayedUpdateTabs();
    }
}


void UIcontainer::setCurrentTab(const QString &formName)
{
    int index = findIndex(formName);
    if (index >=0) {
        ui->tabs->blockSignals(true);
        ui->tabs->setCurrentIndex(index);
        ui->tabs->blockSignals(false);
    }
}


QString UIcontainer::tabName()
{
    return ui->tabs->currentText();
}


void UIcontainer::setCurrentTabName(const QString &newName)
{
    int index = ui->tabs->currentIndex();
    if (index >=0)
        ui->tabs->changeText(newName, index);
}


void UIcontainer::setNewFormName(const QString &formName, const QString &newName)
{
    for (int i=0; i< ui->tabs->count(); ++i)
        if (ui->tabs->tabText(i) == formName) {
            ui->tabs->changeText(newName, i);
            return;
        }
}


void UIcontainer::removeAllTabs()
{
    ui->tabs->removeAllTabs();
    m_forms.clear();
}


void UIcontainer::slotNewFormClicked()
{
    emit requestForNewForm();
}


void UIcontainer::slotDeleteFormClicked()
{
    if (!ui->tabs->count())
        return;

    emit requestForDelete(ui->tabs->currentText());
}


void UIcontainer::slotTabChanged(int index)
{
    qDebug() << "UIcontainer::slotTabChanged" << index;
    emit activeFormChanged(ui->tabs->currentText());
}


void UIcontainer::slotFromFileClicked()
{
    CuteReport::StdStorageDialog d(m_editor->core()->reportCore(), m_editor->core()->currentReport(), m_editor->core()->mainWindow(), "Load UI Form");
    if (!d.exec())
        return;

    QString selectedObjectUrl = d.currentObjectUrl();

    if ( !selectedObjectUrl.isEmpty() ) {
        emit requestFileContent(selectedObjectUrl);
    }
}


void UIcontainer::slotTabDoubleClicked(int index)
{
    emit requestForRename (ui->tabs->tabText(index));
}


int UIcontainer::findIndex(QString formName)
{
    if (!m_forms.contains(formName))
        return -1;

    Form form = m_forms.value(formName);
    return form.index;
}


Form UIcontainer::currentForm()
{
    int index = ui->tabs->currentIndex();
    foreach (Form form, m_forms)
        if (form.index == index)
            return form;
    return Form();
}
