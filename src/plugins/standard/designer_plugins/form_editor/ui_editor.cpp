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
 ***************************************************************************/
#include <QScriptEngine>
#include <QtCore>
#include <QtGui>
#include <QDesignerFormWindowInterface>

#include "ui_editor.h"
#include "uicontainer.h"
#include "core.h"
#include "qdesigner_workbench.h"
#include "qdesigner_actions.h"
#include "reportinterface.h"
#include "forminterface.h"
#include "reportcore.h"
#include "qdesigner_formwindow.h"
//#include "newformdialog.h"
#include "stdstoragedialog.h"
#include "mainwindow.h"
#include "renamedialog.h"



UiEditor::UiEditor(QObject *parent)
    : ModuleInterface(parent)
{
    m_designer = new QDesignerWorkbench(0, core()->settings());
    m_widget = new UIcontainer(this, m_designer->widget());

    QList<QAction*> actionList;

    foreach(QAction * action, m_designer->actionManager()->editActions()->actions()) {
        replaceEmptyActionIcons(action);
        actionList.append(action);
    }

    foreach(QAction * action, m_designer->actionManager()->formActions()->actions()) {
        replaceEmptyActionIcons(action);
        actionList.append(action);
    }

    m_widget->addDesignerActions(actionList);

    connect(m_widget, SIGNAL(requestForNewForm()), this, SLOT(slotRequestForNewForm()));
    connect(m_widget, SIGNAL(requestForDelete(QString)), this, SLOT(slotRequestForDelete(QString)));
    connect(m_widget, SIGNAL(requestForSave(QString, QString)), this, SLOT(slotRequestForSave(QString,QString)));
    connect(m_widget, SIGNAL(requestFileContent(QString)), this, SLOT(slotRequestFileContent(QString)));
    connect(m_widget, SIGNAL(activeFormChanged(QString)), this, SLOT(slotActiveFormChanged(QString)));
    connect(m_widget, SIGNAL(requestForRename(QString)), this, SLOT(slotRequestForRename(QString)));

//    connect(m_designer, SIGNAL(formNameChanged(QString)), this, SLOT(slotFormNameChangedByDesigner(QString)));

    connect(core(), SIGNAL(currentReportChanged(CuteReport::ReportInterface*)), this, SLOT(slotCurrentReportChanged(CuteReport::ReportInterface*)));
    connect(core(), SIGNAL(saveReport_before(CuteReport::ReportInterface*)), this, SLOT(saveData(CuteReport::ReportInterface*)));

    if (core()->currentReport())
        this->slotCurrentReportChanged(core()->currentReport());

    m_widget->setEnabled(false);
}


UiEditor::~UiEditor()
{
    delete m_designer;
    delete m_widget;
}


void UiEditor::reloadSettings()
{
    m_widget->reloadSettings();
    m_designer->reloadSettings();
}


void UiEditor::saveSettings()
{
    m_widget->saveSettings();
    m_designer->saveSettings();
}


void UiEditor::activate()
{
    qDebug() << "UiEditor::activate()";
}


void UiEditor::deactivate()
{
    qDebug() << "UiEditor::deactivate()";
    saveCurrentForm();
}


void UiEditor::sync()
{
    saveCurrentForm();
}


QWidget * UiEditor::view()
{
    return m_widget;
}


QIcon UiEditor::icon()
{
    return QIcon(":images/form_48.png");
}


QString UiEditor::name()
{
    return QString("Forms");
}


bool UiEditor::isDirty()
{
    for (int i=0; i<m_designer->formWindowCount(); i++)
        if (m_designer->formWindow(i)->editor()->isDirty())
                return true;
    return false;
}



void UiEditor::saveCurrentForm(bool forceIfDirty)
{
    if (!m_designer->formWindowCount())
        return;
    if (!forceIfDirty && !isDirty())
        return;

    QString formName = m_designer->formWindowCount() ?
                m_designer->activeFormWindow()->editor()->mainContainer()->objectName() : QString();
    CuteReport::FormInterface * form = core()->reportCore()->formByName(formName, core()->currentReport());
    if (form && m_designer->formWindowCount()) {
        form->setData(m_designer->activeFormWindow()->editor()->contents());
        m_designer->activeFormWindow()->editor()->setDirty(false);
    }

    qDebug() << "contents:" << form->data();
}


void UiEditor::slotRequestForNewForm()
{
    CuteReport::StdStorageDialog d(core()->reportCore(), core()->currentReport(), core()->mainWindow(), "Load Template");
    d.setUrlHint("forms");
    if (!d.exec())
        return;

    if (m_designer->formWindowCount()) {
        saveCurrentForm();
        m_designer->closeCurrentForm();
    }

    QByteArray data = core()->reportCore()->loadObject(d.currentObjectUrl(), core()->currentReport()).toString().toAscii();
    QBuffer buffer(&data);

    QString error;
    QDesignerFormWindow * w = m_designer->openForm("Template", &buffer, &error);

    if (!w)
        return;

    CuteReport::FormInterface * form = core()->reportCore()->createFormObject(core()->currentReport());
    m_widget->addTab(form->objectName());
    m_widget->setCurrentTab(form->objectName());
    core()->setCurrentForm(form);

    w->editor()->mainContainer()->setObjectName(form->objectName());

//    qDebug() << m_designer->activeFormWindow()->editor()->mainContainer()->objectName();

    saveCurrentForm(true);
}


void UiEditor::slotRequestForDelete(QString formName)
{
    CuteReport::FormInterface * form = core()->reportCore()->formByName(formName, core()->currentReport());
    if (!form)
        return;

    QList<CuteReport::FormInterface*> forms = core()->currentReport()->forms();
    int index = -1;
    for (int i=0; i<forms.count(); i++)
        if (forms[i]->objectName() == formName) {
            index = i;
            break;
        }

    m_designer->actionManager()->closeForm();
    m_widget->removeTab(formName);
    delete form;

    forms = core()->currentReport()->forms();

    if (index > -1 && forms.count()) {
        if (index > forms.count() -1)
                index = forms.count() -1;
        if (index < 0)
            index = 0;


        QString error;
        QDesignerFormWindow * w = m_designer->openForm(forms[index]->objectName(), forms[index]->data(), &error);
        w->editor()->mainContainer()->setObjectName(form->objectName());
//        m_designer->createFormWithContents(forms[index]->data());
//        m_designer->setFormName(forms[index]->objectName());
    }
}


void UiEditor::slotRequestForSave(QString formName, QString xmlContent)
{
    Q_UNUSED(formName)
    Q_UNUSED(xmlContent)

    saveCurrentForm();
}


void UiEditor::slotRequestFileContent(QString fileName)
{
    QFile uiFile(fileName);
    if (uiFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        CuteReport::FormInterface * form = core()->reportCore()->createFormObject(core()->currentReport());
//        m_designer->createFormWithContents(&uiFile);
//        m_designer->setFormName(form->objectName());
        m_widget->addTab(form->objectName());

        saveCurrentForm(true);
        uiFile.close();
    }
}


void UiEditor::slotCurrentReportChanged(CuteReport::ReportInterface* report)
{
    Q_UNUSED(report)

    saveCurrentForm();
    m_widget->removeAllTabs();
    m_designer->closeCurrentForm();

    QList<CuteReport::FormInterface*> forms = core()->currentReport()->forms();
    if (forms.count()) {
        foreach(CuteReport::FormInterface * form, forms)
            m_widget->addTab(form->objectName());
        m_widget->setCurrentTab(forms[0]->objectName());

        QString error;
        QDesignerFormWindow * w = m_designer->openForm(forms[0]->objectName(), forms[0]->data(), &error);
        if (w)
            w->editor()->mainContainer()->setObjectName(forms[0]->objectName());
    }

    m_widget->setEnabled(core()->currentReport());
}


void UiEditor::saveData(CuteReport::ReportInterface*report)
{
    Q_UNUSED(report)
    saveCurrentForm();
}


void UiEditor::slotActiveFormChanged(QString formName)
{
    saveCurrentForm();
    m_designer->closeCurrentForm();

    CuteReport::FormInterface* form = core()->currentReport()->form(formName);
    Q_ASSERT(form);

//    qDebug() << "form data:" << form->data();

    QString error;
    QDesignerFormWindow * w = m_designer->openForm(form->objectName(), form->data(), &error);
    if (w)
        w->editor()->mainContainer()->setObjectName(form->objectName());
    else
        qDebug() << "UiEditor: "<< error;
//    m_designer->createFormWithContents(form->data());
//    m_designer->setFormName(form->objectName());
}


void UiEditor::slotFormNameChangedByDesigner(QString name)
{
    m_widget->setCurrentTabName(name);
}


void UiEditor::slotRequestForRename(QString name)
{
    Q_UNUSED(name);
//    CuteReport::FormInterface* form = core()->currentReport()->form(name);

//     if (form) {
//        RenameDialog d(form, core()->currentReport(), core()->mainWindow());
//        d.setWindowTitle("Form renaming");
//        if (d.exec() == QDialog::Accepted) {
//            form->setObjectName(d.newName());
//            m_widget->setNewFormName(name, form->objectName());
//        }
//    }
}


void UiEditor::replaceEmptyActionIcons(QAction* action)
{
    if (!action->icon().isNull())
        return;

//    qDebug() << action->text();
//    qDebug() << action->iconText();
    if (action->iconText() == "Preview")
        action->setIcon( QIcon(":images/form_ok_48.png"));
    else if (action->iconText() == "View Code")
        action->setIcon( QIcon(":images/text-xml.png"));
    else if (action->iconText() == "Form Settings")
        action->setIcon( QIcon(":images/settings3.png"));

}

Q_EXPORT_PLUGIN2(uiEditor, UiEditor)
