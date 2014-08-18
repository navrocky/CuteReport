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
#include "core.h"
#include "moduleinterface.h"
#include "reportcore.h"
#include "mainwindow.h"
#include "reportinterface.h"
#include "pageinterface.h"
#include "iteminterface.h"
#include "datasetinterface.h"
#include "stylehelper.h"
#include "storagesettingsdialog.h"
#include "storageinterface.h"
#include "rendererinterface.h"
#include "propertyeditorcore.h"

#include "QSettings"
#include "QApplication"
#include <QtGui>
#include <QMenuBar>
#include <QMessageBox>

using namespace CuteReport;

namespace CuteDesigner {

static const QString MODULENAME = "Designer::Core";

bool modulePriorityLessThan(ModuleInterface * i1, ModuleInterface * i2)
{
    return i1->priority() < i2->priority();
}

Core::Core(QObject *parent) :
    QObject(parent),
    m_mainWindow(0),
    m_currentReport(0),
    m_currentPage(0),
    m_currentDataset(0),
    m_currentForm(0)
{
    m_settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "ExaroLogic", "Cute Report", this);
    if (m_settings->value("Core/VersionMajor").toInt() < 1) {
        QString fileName = m_settings->fileName();
        delete m_settings;
        QFile::remove(fileName);
        m_settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "ExaroLogic", "Cute Report", this);
    }

    m_reportCore = new ReportCore(m_settings, true);
    connect(m_reportCore, SIGNAL(metricUpdated(CuteReport::MetricType,QVariant)), this, SLOT(showMetric(CuteReport::MetricType, QVariant)));
    connect(m_reportCore, SIGNAL(rendererDone(CuteReport::ReportInterface*,bool)), this, SLOT(_rendererDone(CuteReport::ReportInterface*,bool)));


    connect(PropertyEditor::PropertyEditorCore::createInstance(), SIGNAL(log(int,QString,QString,QString)), this, SLOT(propertyEditorLog(int,QString,QString,QString)));
    PropertyEditor::PropertyEditorCore::instance()->inc();
    PropertyEditor::PropertyEditorCore::instance()->init();

    StyleHelper::setBaseColor(QApplication::palette().color(QPalette::Highlight).darker());

    setupMainWindow();

    initModules();

    foreach (ModuleInterface * module , m_guiModules) {
        m_mainWindow->addTab(module->view(), module->icon(), module->name());
    }

    foreach (ModuleInterface * module , m_guiModules)
        module->reloadSettings();

    foreach (ModuleInterface * module , m_nonGuiModules)
        module->reloadSettings();

    m_mainWindow->reloadSettings();

    appendMenus();

    setSettingValue("Core/VersionMajor", m_reportCore->versionMajor());
    setSettingValue("Core/VersionMinor", m_reportCore->versionMinor());

    QTimer::singleShot(0, this, SLOT(slotInitDone()));
}


Core::~Core()
{
    PropertyEditor::PropertyEditorCore::instance()->dec();
    saveSettings();
}


void Core::saveSettings()
{
    m_mainWindow->saveSettings();

    foreach (ModuleInterface * module , m_guiModules)
        module->saveSettings();

    setSettingValue("Designer/LastReportURL", m_currentReport ? m_currentReport->filePath() : "");
    setSettingValue("Designer/PrimaryStorage", m_reportCore->defaultStorage()->moduleName());
    setSettingValue("Designer/PrimaryRenderer", m_reportCore->defaultRenderer()->moduleName());
    m_settings->setValue( QString("Designer/Storage_%1_options").arg(m_reportCore->defaultStorage()->moduleName()), m_reportCore->moduleOptionsStr(m_reportCore->defaultStorage()));
    m_settings->setValue( QString("Designer/Renderer_%1_options").arg(m_reportCore->defaultRenderer()->moduleName()), m_reportCore->moduleOptionsStr(m_reportCore->defaultRenderer()));

    setSettingValue("Designer/PrimaryRenderer", m_reportCore->defaultRenderer()->moduleName());

    delete m_reportCore;
}



void Core::restoreSettings()
{
    m_mainWindow->reloadSettings();

    m_reportCore->setDefaultStorage( getSettingValue("Designer/PrimaryStorage").toString() );
    m_reportCore->setDefaultRenderer( getSettingValue("Designer/PrimaryRenderer").toString() );
    m_reportCore->setModuleOptionsStr(m_reportCore->defaultStorage(), getSettingValue("Designer/Storage_%1_options").toString() );
    m_reportCore->setModuleOptionsStr(m_reportCore->defaultRenderer(), getSettingValue("Designer/Renderer_%1_options").toString() );
}


void Core::slotInitDone()
{
    StorageInterface * storage = 0;
    QString storageName = getSettingValue("Designer/PrimaryStorage").toString();
    if (!storageName.isEmpty()) {
        storage = m_reportCore->storageModule(storageName);
    }

    if (!storage) {
        StorageSettingsDialog d(this);
        if (d.exec() == QDialog::Accepted) {
            storage = d.currentStorage();
        }
    }

    restoreSettings();

    m_mainWindow->show();

    emit initDone();

    QString reportURL = getSettingValue("Designer/LastReportURL").toString();
    if (!reportURL.isEmpty())
        emit requestForReport(reportURL);
}


void Core::initModules()
{
    ModuleInterface::setCore(this);

    QFileInfoList files;
    QStringList dirs;
    dirs << REPORT_DESIGNER_PLUGINS_PATH;

    foreach (const QString & dirStr, dirs) {
        QDir dir(dirStr);
        m_reportCore->log(CuteReport::LogDebug, MODULENAME, "Designer plugin dir: " + dir.absolutePath() );
        files += dir.entryInfoList(QDir::Files);
    }

    QPluginLoader loader;
    //    loader.setLoadHints(QLibrary::ResolveAllSymbolsHint|QLibrary::ExportExternalSymbolsHint);
    QObject *plugin;

    foreach(const QFileInfo & fileName, files) {
        m_reportCore->log(CuteReport::LogDebug, MODULENAME, "Loading plugins: " + fileName.fileName());


        loader.setFileName(fileName.absoluteFilePath());
        if (!loader.load()) {
            m_reportCore->log(LogWarning, MODULENAME, QString("Error while loading plugin \'%1\': %2 ").arg(fileName.fileName()).arg(loader.errorString()));
            continue;
        }

        plugin = loader.instance();

        if (plugin) {
            plugin->setParent(this);
            CuteDesigner::ModuleInterface* module = qobject_cast<CuteDesigner::ModuleInterface*>(plugin);
            qDebug() << module->name();
            if (module) {
                if (module->view())
                    m_guiModules.append(module);
                else
                    m_nonGuiModules.append(module);
            } else
                m_reportCore->log(LogWarning, MODULENAME, QString("Error while loading plugin \'%1\': it's not a CuteDesigner::ModuleInterface' ").arg(fileName.fileName()));
        } else
            m_reportCore->log(LogWarning, MODULENAME, QString("Error while creating instance of plugin \'%1\': %2 ").arg(fileName.fileName()).arg(loader.errorString()));
    }

    qSort(m_guiModules.begin(), m_guiModules.end(), modulePriorityLessThan);

    m_currentModuleIndex = -1;
}


void Core::setupMainWindow()
{
    m_mainWindow = new MainWindow(this);
    m_mainWindow->setWindowTitle("CuteReport");
    connect(m_mainWindow, SIGNAL(closed()), this, SLOT(slotMainWindowClosed()));
}


MainWindow * Core::mainWindow()
{
    return m_mainWindow;
}


void Core::appendMenus()
{
    if (!m_mainWindow)
        return;

    QMenuBar * menubar = m_mainWindow->menuBar ();

    QHash<int, DesignerMenu*> mainMenu;
    QHash<QString, DesignerMenu*> menuTitles;
    QList<DesignerMenu*> deleteList;

    // add original QMainMenu actions
    int priority = menubar->actions().count() * 100;
    foreach(QAction * action, menubar->actions()) {
        DesignerMenu * menu = new DesignerMenu();
        menu->menu = action->menu();
        menu->mainPriority = priority;
        menu->subPriority = 100;
//        menubar->removeAction(action);
        menuTitles.insertMulti(action->text(), menu);
        priority -= 100;
    }

    menubar->clear();

    // add Modules' actions
    foreach (ModuleInterface * module , m_guiModules) {
        QList<CuteDesigner::DesignerMenu*> menus = module->mainMenu();
        foreach (CuteDesigner::DesignerMenu * menu,  menus) {
            menuTitles.insertMulti(menu->menu->title(), menu);
        }
    }
    foreach (ModuleInterface * module , m_nonGuiModules) {
        QList<CuteDesigner::DesignerMenu*> menus = module->mainMenu();
        foreach (CuteDesigner::DesignerMenu * menu,  menus) {
            menuTitles.insertMulti(menu->menu->title(), menu);
        }
    }

    // menu concatenating for the same mainMenu title
    foreach(QString title, menuTitles.keys()) {
        QList<DesignerMenu*> list = menuTitles.values(title);
        if (list.count() > 1) {
            QHash<int, DesignerMenu*> subMenu;
            int priority = list[0]->mainPriority;
            for (int i = 0; i<list.count(); ++i) {
                if (list[i]->mainPriority > priority)
                    priority = list[i]->mainPriority;
                subMenu.insert(list[i]->subPriority, list[i]);
            }

            QList<int> subMenuPriorities = subMenu.uniqueKeys();
            qSort(subMenuPriorities.begin(), subMenuPriorities.end(), qGreater<int>());

            DesignerMenu * commonSubMenu = new DesignerMenu();
            commonSubMenu->menu = new QMenu(m_mainWindow);
            commonSubMenu->menu->setTitle(title);

            deleteList.append(commonSubMenu);

            foreach (int priority, subMenuPriorities) {
                QList<DesignerMenu*> subMenus = subMenu.values(priority);
                foreach(DesignerMenu* subMenu, subMenus)
                    commonSubMenu->menu->addActions(subMenu->menu->actions());
                if (priority != subMenuPriorities.last())
                    commonSubMenu->menu->addSeparator();
            }

            mainMenu.insert(priority, commonSubMenu);

        } else
            mainMenu.insert(list[0]->mainPriority, list[0]);
    }


    QList<int> mainMenuPriorities = mainMenu.uniqueKeys();
    qSort(mainMenuPriorities.begin(), mainMenuPriorities.end(), qGreater<int>());


    foreach (int priority, mainMenuPriorities) {
        QList<DesignerMenu*> menus = mainMenu.values(priority);
        foreach (DesignerMenu* menu, menus) {
            menubar->addMenu(menu->menu);
        }
    }

    // cleaning up
    foreach (DesignerMenu * menu, deleteList)
        delete menu;
    foreach (DesignerMenu * menu, menuTitles)
        delete menu;
}



void Core::setSettingValue(QString key, QVariant value)
{
    m_settings->setValue(key, value);
}


QVariant Core::getSettingValue(QString key, QVariant defaultValue)
{
    return m_settings->value(key, defaultValue);
}


QSettings * Core::settings()
{
    return m_settings;
}


void Core::switchToModule(ModuleInterface * module)
{
    int index = m_guiModules.indexOf(module);
    if (index >=0)
        m_mainWindow->switchToTab(index);
}



void Core::sync()
{
    if (m_currentReport)
        foreach (ModuleInterface* module, m_guiModules)
            module->sync();
}


void Core::addToStatusBar(QLabel * label)
{
    m_mainWindow->addToStatusBar(label);
}


void Core::setDocumentTitles(int id, const QString & name)
{
    if (name.isEmpty())
        m_documentTitles.remove(id);
    else
        m_documentTitles.insert(id, name);

    QStringList list;
    QMap<int, QString>::iterator i;
    for (i = m_documentTitles.begin(); i != m_documentTitles.end(); ++i)
        list.append(i.value());
    list.append("Cute Report");

    m_mainWindow->setWindowTitle(list.join(" - "));
}


void Core::showMetric(MetricType type, const QVariant &value)
{
    m_mainWindow->showMetric(type, value);
}


CuteReport::ReportInterface* Core::currentReport() const
{
    return m_currentReport;
}


void Core::setCurrentReport(CuteReport::ReportInterface * report)
{
    if (report == m_currentReport)
        return;

    m_reportCore->log(CuteReport::LogDebug, MODULENAME, "setCurrentReport: " + (report ? report->objectName() : "not defined"));
    m_currentReport = report;

    emit currentReportChanged(m_currentReport);
}


void Core::setCurrentPage(CuteReport::PageInterface * page)
{
    if ( m_currentPage == page)
        return;

    m_currentPage = page;
    emit currentPageChanged(m_currentPage);
}


CuteReport::PageInterface* Core::currentPage()
{
    return m_currentPage;
}


void Core::setCurrentDataset(CuteReport::DatasetInterface* dataset)
{
    if (m_currentDataset == dataset)
        return;

    m_currentDataset = dataset;
    emit currentDatasetChanged(m_currentDataset);
}


CuteReport::DatasetInterface* Core::currentDataset()
{
    return m_currentDataset;
}


void Core::setCurrentForm(CuteReport::FormInterface * form)
{
    if (form == m_currentForm)
        return;

    m_currentForm = form;

}


CuteReport::FormInterface* Core::currentForm()
{
    return m_currentForm;
}


bool Core::newPage()
{
    return true;
}


void Core::render()
{
    if (!m_currentReport)
        return;

    m_reportCore->log(CuteReport::LogDebug, MODULENAME, "render()");

    sync();


    if (m_reportCore->render(m_currentReport)) {
        emit renderingStarted();
        //            m_mainWindow->showProgressDialog("", "Cancel", 0, 100);
        //            connect(m_reportCore, SIGNAL(rendererDone(CuteReport::ReportInterface*,bool)), this, SLOT(_rendererDone(CuteReport::ReportInterface*,bool)));
        //            connect(m_reportCore, SIGNAL(rendererProcessingPage(CuteReport::ReportInterface*,int,int)), this, SLOT(_rendererProcessingPage(CuteReport::ReportInterface*,int,int)));
        //            connect(m_reportCore, SIGNAL(rendererPageRendered(CuteReport::ReportInterface*,CuteReport::RenderedPageInterface*,int)), this, SLOT(_rendererPageRendered(CuteReport::ReportInterface*,CuteReport::RenderedPageInterface*,int)));
    }

}


void Core::stopRenderer()
{
    m_reportCore->log(CuteReport::LogDebug, MODULENAME, "stopRender()");
    int count = m_reportCore->rendererModules().count();
    if (!count) {
        Q_ASSERT(0);
    } else if (count == 1) {
        m_reportCore->stopRendering(m_currentReport);
    } else {
        // TODO: dialog with choosing renderer if more then 1
    }
}

void Core::moduleSwitched(int index)
{
    //    m_reportCore->log(CuteReport::LogDebug, MODULENAME, QString("moduleSwitched index: %1").arg(index));

    if (m_guiModules.isEmpty())
        return;

    if (m_currentModuleIndex >=0)
        m_guiModules.at(m_currentModuleIndex)->deactivate();
    m_guiModules.at(index)->activate();
    if (m_mainWindow)
        m_mainWindow->setStdActions(m_guiModules.at(index)->stdActions());

    m_currentModuleIndex = index;
}


void Core::_newPeportPage()
{
    CuteReport::PageInterface * newPage = m_reportCore->createPageObject(m_currentReport);
    if (!newPage) {
        m_reportCore->log(LogWarning, MODULENAME, "There is no page plugin in the report core");
        return;
    }

    //    /// adding new manipulator if needed
    //    if (!m_pageManipulators.contains(newPage->pageManupilatorID())) {
    //        CuteReport::PageManipulatorInterface * newManipulator = newPage->createManupulator(this);
    //        Q_ASSERT(newPage->pageManupilatorID() == newManipulator->pageManupilatorId());
    //        m_pageManipulators.insert(newPage->pageManupilatorID(), newManipulator);
    //    }

    m_currentReport->addPage(newPage);

    emit newPage_after(newPage);

    //    changeCurrentPage(newPage);
}


void Core::_afterItemRemoved(BaseItemInterface *item)
{
    Q_UNUSED(item)
    emit activeObjectChanged(m_currentPage);
}


void Core::slotMainWindowClosed()
{
    //    m_mainWindow->saveSettings();
    //    foreach (ModuleInterface * module , m_modules)
    //        module->saveSettings();
}


void Core::propertyEditorLog(int logLevel, const QString &module, const QString &shortMessage, const QString &fullMessage)
{
    ReportCore::log(CuteReport::LogLevel(logLevel), module, shortMessage, fullMessage);
}


void Core::_rendererDone(ReportInterface * report, bool successful)
{
    Q_UNUSED(report)

    if (!successful) {
        QMessageBox::critical(m_mainWindow, tr("CuteReport Designer"),
                              tr("Errors found while generating report.\n"
                                 "Press log button on the bottom left corner for detailed information."));
    }
}


} //namespaca

