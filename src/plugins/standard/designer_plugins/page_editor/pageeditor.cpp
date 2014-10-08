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
#include "pageeditor.h"
#include "pageeditorcontainer.h"
#include "propertyeditor.h"
#include "pageinterface.h"
#include "reportinterface.h"
#include "reportplugininterface.h"
#include "iteminterface.h"
#include "bandinterface.h"
#include "objectinspector.h"
#include "reportcore.h"
#include "renamedialog.h"
#include "mainwindow.h"

#include <QLabel>
#include <QMenu>

const char * MODULENAME = "PageEditor";

using namespace CuteDesigner;

inline void initMyResource() { Q_INIT_RESOURCE(page_editor); }

PageEditor::PageEditor(QObject *parent) :
    ModuleInterface(parent),
    m_currentPage(0),
    m_activeObject(0),
    m_currentManipulator(0),
    m_isActive(false)
{
}


PageEditor::~PageEditor()
{
    delete ui;
}


void PageEditor::init(Core *core)
{
    initMyResource();
    ModuleInterface::init(core);

    ui = new PageEditorContainer(this);
    ui->init();
    ui->addPagePlugins(core->reportCore()->pageModules());

    m_propertyEditor = core->createPropertyEditor(ui);
    ui->addPropertyEditor(m_propertyEditor);
    m_objectInspector = new ObjectInspector(ui);
    ui->addObjectInspector(m_objectInspector);

    foreach (CuteReport::BaseItemInterface* itemPlugin, core->reportCore()->itemModules())
        if (dynamic_cast<CuteReport::BandInterface*>(itemPlugin))
            ui->addItem(itemPlugin->itemIcon(), itemPlugin->moduleShortName(), itemPlugin->suitName(), itemPlugin->itemGroup());

    foreach (CuteReport::BaseItemInterface* itemPlugin, core->reportCore()->itemModules())
        if (!dynamic_cast<CuteReport::BandInterface*>(itemPlugin)) {
            ui->addItem(itemPlugin->itemIcon(), itemPlugin->moduleShortName(), itemPlugin->suitName(),  itemPlugin->itemGroup());
        }

    //connect(core, SIGNAL(loadReport_after(CuteReport::ReportInterface*)), this, SLOT(slotReportCreated(CuteReport::ReportInterface*)));
    //connect(core, SIGNAL(newReport_after(CuteReport::ReportInterface*)), this, SLOT(slotReportCreated(CuteReport::ReportInterface*)));
    connect(core, SIGNAL(currentReportChanged(CuteReport::ReportInterface*)), this, SLOT(slotReportChanged(CuteReport::ReportInterface*)));
    connect(m_objectInspector, SIGNAL(objectChanged(QObject*)), this, SLOT(slotActiveObjectChanged(QObject*)));
    connect(m_objectInspector, SIGNAL(selectionChanged()), this, SLOT(slotSelectionChanged()));

    connect(ui.data(), SIGNAL(requestForCreatePage(QString)), this, SLOT(slotRequestForCreatePage(QString)));
    connect(ui.data(), SIGNAL(requestForClonePage(QString)), this, SLOT(slotRequestForClonePage(QString)));
    connect(ui.data(), SIGNAL(requestForDeletePage(QString)), this, SLOT(slotRequestForDeletePage(QString)));
    connect(ui.data(), SIGNAL(currentTabChanged(QString)), this, SLOT(slotCurrentPageChangedByGUI(QString)));
    connect(ui.data(), SIGNAL(requestForRenamePage(QString)), this, SLOT(slotRequestForRenamePage(QString)));

    ui->setEnabled(core->currentReport());
}


void PageEditor::reloadSettings()
{
    if (ui)
        ui->reloadSettings();
}


void PageEditor::saveSettings()
{
    if (ui)
        ui->saveSettings();
}


void PageEditor::activate()
{
    core()->reportCore()->log(CuteReport::LogDebug, MODULENAME, "activate");
    if (m_currentPage && m_currentManipulator)
        foreach (QLabel * label, m_currentManipulator->statusBarLabels()) {
            label->show();
            m_core->addToStatusBar(label);
        }

    m_isActive = true;
}


void PageEditor::deactivate()
{
    core()->reportCore()->log(CuteReport::LogDebug, MODULENAME, "deactivate");
    if (m_currentPage && m_currentManipulator) {
        foreach (QLabel * label, m_currentManipulator->statusBarLabels())
            label->hide();
    }
    m_isActive = false;
}


void PageEditor::sync()
{
    // no need to do something
    // changes commit immediately
}


QWidget * PageEditor::view()
{
    //    if (!ui)
    //        ui = new PageEditorContainer;
    return ui;
}


QIcon PageEditor::icon()
{
    return QIcon(":images/editor_48.png");
}


QString PageEditor::name()
{
    return QString("Pages");
}


QList<DesignerMenu *> PageEditor::mainMenu()
{
    QWidget * parent = core()->mainWindow();

    QList<CuteDesigner::DesignerMenu*> menus;

    CuteDesigner::DesignerMenu * reportMenu = new CuteDesigner::DesignerMenu(parent, "Page", 400, 400); // very high priority;  very high priority;
    menus.append(reportMenu);

    reportMenu->menu->addAction(createAction("actionNewPage", "New Page", ":/images/document-new.png", "Alt+P, Alt+N", SLOT(slotNewPage())));
    reportMenu->menu->addAction(createAction("actionDeletePage", "Delete Page", ":/images/document-close.png", "Alt+P, Alt+D", SLOT(slotDeletePage())));

    return menus;
}


void PageEditor::slotActiveObjectChanged(QObject * object)
{
    if (object && object == m_activeObject)
        return;

    if (m_activeObject && dynamic_cast<CuteReport::BaseItemInterface *> (m_activeObject.data())) {
        disconnect(m_activeObject, SIGNAL(parentItemChanged(CuteReport::BaseItemInterface*)), this,  SLOT(slotUpdateObjectInspector()));
    }

    m_activeObject = object;

    m_propertyEditor->setObject(object);
    if (!qobject_cast<ObjectInspector *> (sender())) {
        m_objectInspector->setRootObject(object ? m_currentPage : 0);
        m_objectInspector->selectObject(object);
    }

    CuteReport::BaseItemInterface * item = qobject_cast<CuteReport::BaseItemInterface *> (object);
    if (m_currentPage && item) {
        m_currentPage->setCurrentItem(item);
        connect(item, SIGNAL(parentItemChanged(CuteReport::BaseItemInterface*)), this, SLOT(slotUpdateObjectInspector()));
    }
}


void PageEditor::slotUpdateObjectInspector()
{
    m_objectInspector->setRootObject(m_currentPage);
    m_objectInspector->selectObject(m_activeObject);
}


void PageEditor::slotSelectionChanged()
{
    QList<CuteReport::BaseItemInterface *> list;
    foreach (QObject * object, m_objectInspector->selectedObjects()) {
        CuteReport::BaseItemInterface * item = qobject_cast<CuteReport::BaseItemInterface *>(object);
        if (item)
            list.append(item);
    }
    if (m_currentPage)
        m_currentPage->setSelectedItems(list);
}


void PageEditor::slotReportChanged(CuteReport::ReportInterface * report)
{
    ui->setEnabled(report);
    ui->removeAllTabs();
    foreach (CuteReport::PageInterface * page, m_pages) {
        if (page)
            page->disconnect(this, 0);
    }
    m_pages.clear();
    m_pageNames.clear();
    if (m_currentReport) {
        m_currentReport->disconnect(this, 0);
    }

    if (report) {
        foreach (CuteReport::PageInterface * page, report->pages()) {
            _processNewPage(page);
        }
        slotChangeCurrentPage( report->pages().count() ? report->pages()[0] : 0);
        if (m_currentPage)
            ui->setCurrentTab(m_currentPage->objectName());
        connect(report, SIGNAL(pageAdded(CuteReport::PageInterface*)), this, SLOT(slotPageCreatedOutside(CuteReport::PageInterface*)));
        connect(report, SIGNAL(pageDeleted(CuteReport::PageInterface*)), this, SLOT(slotPageDeletedOutside(CuteReport::PageInterface*)));
    } else {
        m_currentPage = 0;
        m_activeObject = 0;
        m_objectInspector->setRootObject(0);
        m_propertyEditor->setObject(0);
    }
    m_currentReport = report;
}


void PageEditor::slotRequestForCreatePage(QString moduleName)
{
    CuteReport::PageInterface * page = core()->reportCore()->createPageObject(core()->currentReport(), moduleName);
    if (!page)
        return;
    page->init();
    core()->currentReport()->addPage(page);
}


void PageEditor::slotPageCreatedOutside(CuteReport::PageInterface *page)
{
    if (!page || m_pages.contains(page))
        return;
    _processNewPage(page);
    slotChangeCurrentPage(page);
    ui->setCurrentTab(page->objectName());
}


void PageEditor::slotRequestForDeletePage(QString pageName)
{
    CuteReport::PageInterface * page = core()->reportCore()->pageByName(pageName, core()->currentReport());
    if (!page)
        return;

    QList<CuteReport::PageInterface*> pages = core()->currentReport()->findChildren<CuteReport::PageInterface*>();
    int index = -1;
    for (int i=0; i<pages.count(); i++)
        if (pages[i]->objectName() == pageName) {
            index = i;
            break;
        }

    ui->removeTab(pageName);
//    int index = m_pages.indexOf(page);
    m_pages.removeAt(index);
    m_pageNames.removeAt(index);
    m_currentReport->deletePage(page);
//    m_currentPage = 0;

//    pages = core()->currentReport()->findChildren<CuteReport::PageInterface*>();

//    if (index > pages.count() -1)
//        index = pages.count() -1;
//    if (index < 0)
//        index = 0;

//    slotChangeCurrentPage( pages.count() ? pages[index] : 0);

//    if (m_currentPage)
//        ui->setCurrentTab(m_currentPage->objectName());
}


void PageEditor::slotPageDeletedOutside(CuteReport::PageInterface *page)
{
    int index = m_pages.indexOf(page);
    m_pages.removeAt(index);
    m_pageNames.removeAt(index);
    ui->removeTab(page->objectName());
    if (m_currentPage == page) {
        m_currentPage = 0;
        int index = -1;
        if (index > m_pages.count() -1)
            index = m_pages.count() -1;
        if (index < 0)
            index = 0;
        slotChangeCurrentPage( m_pages.count() ? m_pages[index] : 0);
        if (m_currentPage)
            ui->setCurrentTab(m_currentPage->objectName());
    }
}


void PageEditor::slotRequestForClonePage(QString pageName)
{
    Q_UNUSED(pageName)
}


void PageEditor::slotCurrentPageChangedByGUI(QString pageName)
{
    CuteReport::PageInterface * newPage = core()->reportCore()->pageByName(pageName, core()->currentReport());
    if (newPage != m_currentPage) {
        slotChangeCurrentPage(newPage);
        core()->setCurrentPage(newPage);
    }
}


void PageEditor::slotCurrentPageChangedByCore(CuteReport::PageInterface* page)
{
    if (page != m_currentPage) {
        slotChangeCurrentPage(page);
        ui->setCurrentTab(page->objectName());
    }
}


void PageEditor::slotRequestForRenamePage(QString pageName)
{
    CuteReport::PageInterface * page = core()->reportCore()->pageByName(pageName, core()->currentReport());

    if (page) {
        RenameDialog d(page, core()->currentReport(), core()->mainWindow());
        d.setWindowTitle("Page renaming");
        if (d.exec() == QDialog::Accepted) {
            page->setObjectName(d.newName());
//            ui->setNewPageName( pageName, d.newName());
        }
    }
}


void PageEditor::slotPageNameChangedOutside(const QString & name)
{
    CuteReport::PageInterface * page = qobject_cast<CuteReport::PageInterface*>(sender());
    Q_ASSERT(page);
    int index = m_pages.indexOf(page);
    QString newName = page->objectName();
    QString oldName = m_pageNames.at(index);
    ui->setNewPageName(oldName, newName);
    m_pageNames[index] = newName;
}


void PageEditor::slotNewPage()
{
    QStringList modules = core()->reportCore()->moduleNames(CuteReport::PageModule);

    if (modules.size() == 0) {
        CuteReport::ReportCore::log(CuteReport::LogWarning, MODULENAME, "There is over no page modules");
        return;
    }

    if (modules.size() > 1)
        CuteReport::ReportCore::log(CuteReport::LogWarning, MODULENAME, "There are over 1 page modules.");

    slotRequestForCreatePage(modules.at(0));
}


void PageEditor::slotDeletePage()
{
    if (!m_currentPage)
        return;

    slotRequestForDeletePage(m_currentPage->objectName());
}


//void PageEditor::slotReportCreated(CuteReport::ReportInterface * report)
//{
//    connect(report, SIGNAL(pageAdded(CuteReport::PageInterface*)), this
//}


void PageEditor::_processNewPage(CuteReport::PageInterface *page)
{
    if (!page)
        return;

    m_pages.append(page);
    m_pageNames.append(page->objectName());
    ui->addTab(page->createView(), page->icon(), page->objectName());
    connect(page, SIGNAL(objectNameChanged(QString)), this, SLOT(slotPageNameChangedOutside(QString)));
}


void PageEditor::slotChangeCurrentPage(CuteReport::PageInterface* page)
{
    if (m_currentPage) {
        disconnect(m_currentPage, SIGNAL(activeObjectChanged(QObject*)), this, SLOT(slotActiveObjectChanged(QObject*)));
        if (m_currentManipulator)
            foreach (QLabel * label, m_currentManipulator->statusBarLabels())
                label->hide();
    }

    m_currentPage = page;

    if (!m_currentPage) {
        slotActiveObjectChanged(0);
        if (m_currentManipulator)
            m_currentManipulator->setActivePage(0);
        return;
    }

    connect(m_currentPage, SIGNAL(activeObjectChanged(QObject*)), this, SLOT(slotActiveObjectChanged(QObject*)));

    /// manage manipulator
    CuteReport::PageManipulatorInterface * oldManipulator = m_currentManipulator;
    if (!m_currentManipulator) {
        m_currentManipulator = m_currentPage->createManupulator(this);
        m_pageManipulators.insert(m_currentManipulator->pageManupilatorId(), m_currentManipulator);
    } else
        if (m_currentManipulator && m_currentPage->pageManupilatorID() != m_currentManipulator->pageManupilatorId()) {
            if (m_pageManipulators.contains(m_currentPage->pageManupilatorID())) {
                m_currentManipulator = m_pageManipulators.value(m_currentPage->pageManupilatorID());
            } else {
                m_currentManipulator = m_currentPage->createManupulator(this);
                m_pageManipulators.insert(m_currentManipulator->pageManupilatorId(), m_currentManipulator);
            }
        }

    if (m_currentManipulator) {
        m_currentManipulator->setActivePage(m_currentPage);

        if (m_isActive)
            foreach (QLabel * label, m_currentManipulator->statusBarLabels()) {
                label->show();
                m_core->addToStatusBar(label);
            }

        if (oldManipulator != m_currentManipulator)
            ui->setPageActions(m_currentManipulator->actions());
    }

    slotActiveObjectChanged(m_currentPage->currentItem() ? (QObject*)m_currentPage->currentItem() : (QObject*)m_currentPage);
}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(PageEditor, PageEditor)
#endif
