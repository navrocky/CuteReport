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
#include "reportproperties.h"
#include "ui_reportproperties.h"
#include "reportinterface.h"
#include "reportcore.h"
#include "core.h"
#include "printerinterface.h"
#include "rendererinterface.h"
#include "storageinterface.h"
#include "propertyeditor.h"
#include <QDebug>

namespace PropertyEditor{

ReportProperties::ReportProperties(CuteDesigner::Core * core, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReportProperties),
    m_core(core),
    m_report(0)
{
    ui->setupUi(this);

//    int maxWidth = qMax(ui->labelPrinting->width(), ui->labelRendering->width());
//    ui->labelPrinting->setMinimumWidth(maxWidth);
//    ui->labelRendering->setMinimumWidth(maxWidth);

//    maxWidth = qMax(ui->renderingModules->width(), ui->printModules->width());
//    ui->renderingModules->setMinimumWidth(maxWidth);
//    ui->printModules->setMinimumWidth(maxWidth);

    ui->tabWidget->setCurrentIndex(0);

    connect(ui->assignRenderer, SIGNAL(clicked()), this, SLOT(setNewRendererModule()));
    connect(ui->assignPrinter, SIGNAL(clicked()), this, SLOT(setNewPrinterModule()));
    connect(ui->assignStorage, SIGNAL(clicked()), this, SLOT(setNewStorageModule()));
    connect(ui->bDeleteStorage, SIGNAL(clicked()), this, SLOT(deleteCurrentStorage()));
    connect(ui->bRendererDelete, SIGNAL(clicked()), this, SIGNAL(requestForDeleteRenderer()));
    connect(ui->bPrinterDelete, SIGNAL(clicked()), this, SIGNAL(requestForDeletePrinter()));
    connect(ui->bDefaultStorage, SIGNAL(clicked()), this, SLOT(setDefaultStorage()));
    connect(ui->bResetDefaultStorage, SIGNAL(clicked()), this, SLOT(clearDefaultStorage()));
    connect(ui->storageList, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(storagesListIndexChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
    connect(ui->variables, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(variableItemChanged(QTableWidgetItem*)));
}

ReportProperties::~ReportProperties()
{
    delete ui;
}


void ReportProperties::saveSettings()
{
//    m_core->setSettingValue("DatasetEditor/splitterState", ui->splitter->saveState());
//    m_core->setSettingValue("DatasetEditor/splitter2State", ui->splitterTestPage->saveState());
//    m_core->setSettingValue("DatasetEditor/tabMode", ui->datasetTabs->mode());
//    m_core->setSettingValue("DatasetEditor/propertiesShown", ui->bProperties->isChecked());
}


void ReportProperties::reloadSettings()
{
//    ui->splitter->restoreState((m_core->getSettingValue("DatasetEditor/splitterState").toByteArray()));
//    ui->splitterTestPage->restoreState((m_core->getSettingValue("DatasetEditor/splitter2State").toByteArray()));
//    ui->bProperties->setChecked(m_core->getSettingValue("DatasetEditor/propertiesShown").toBool());

//    ui->datasetTabs->SetCurrentIndex(0);
//    FancyTabWidget::Mode default_mode = FancyTabWidget::Mode_LargeSidebar;
//    ui->datasetTabs->SetMode(FancyTabWidget::Mode(m_core->getSettingValue("Preview/tabMode", default_mode).toInt()));
//    ui->stackedWidget->setCurrentIndex(0);
}


void ReportProperties::addRendererPropertyEditor(PropertyEditor::EditorWidget * propertyEditor)
{
    m_rendererPropertyEditor = propertyEditor;
    ui->propertyEditorFrame->layout()->addWidget(propertyEditor);
}


void ReportProperties::addPrinterPropertyEditor(PropertyEditor::EditorWidget * propertyEditor)
{
    m_printerPropertyEditor = propertyEditor;
    ui->printerPropertyEditorFrame->layout()->addWidget(propertyEditor);
}


void ReportProperties::connectReport(CuteReport::ReportInterface * report)
{
    if (m_report)
        disconnectReport();

    m_report = report;

    if (!m_report)
        return;

    ui->url->setText(m_report->filePath());
    ui->reportName->setText(m_report->name());
    ui->author->setText(m_report->author());
    ui->description->setText(m_report->description());

    ui->printModules->clear();
    ui->renderingModules->clear();
    ui->storageModules->clear();
    ui->printModules->addItems(m_core->reportCore()->moduleNames(CuteReport::PrinterModule));
    ui->renderingModules->addItems(m_core->reportCore()->moduleNames(CuteReport::RendererModule));
    ui->storageModules->addItems(m_core->reportCore()->moduleNames(CuteReport::StorageModule));

    ui->rendererName->setText(m_report->renderer() ? m_report->renderer()->moduleFullName() : "");
    ui->printerName->setText(m_report->printer() ? m_report->printer()->moduleFullName() : "");
    ui->lGlobalRenderer->setVisible(!m_report->renderer());
    ui->lGlobalPrinter->setVisible(!m_report->printer());

    foreach (const QString &name, m_report->storagesName())
        ui->storageList->addTopLevelItem(new QTreeWidgetItem ( ui->storageList, QStringList() << name));
    ui->leDefaultStorage->setText(m_report->defaultStorageName());

    connect(m_report.data(), SIGNAL(filePathChanged(QString)), this, SLOT(setGUIReportFilePath(QString)));
    connect(m_report.data(), SIGNAL(nameChanged(QString)), this, SLOT(setGUIReportName(QString)));
    connect(m_report.data(), SIGNAL(authorChanged(QString)), this, SLOT(setGUIReportAuthor(QString)));
    connect(m_report.data(), SIGNAL(descriptionChanged(QString)), this, SLOT(setGUIReportDescription(QString)));
    connect(m_report.data(), SIGNAL(printerChanged(CuteReport::PrinterInterface*)), this, SLOT(setGUIReportPrinter(CuteReport::PrinterInterface*)));
    connect(m_report.data(), SIGNAL(rendererChanged(CuteReport::RendererInterface*)), this, SLOT(setGUIReportRenderer(CuteReport::RendererInterface*)));
    connect(m_report.data(), SIGNAL(storageAdded(CuteReport::StorageInterface*)), this, SLOT(addGUIReportStorage(CuteReport::StorageInterface*)));
    connect(m_report.data(), SIGNAL(storageDeleted(CuteReport::StorageInterface*)), this, SLOT(removeGUIReportStorage(CuteReport::StorageInterface*)));
    connect(m_report.data(), SIGNAL(defaultStorageNameChanged(QString)), this, SLOT(setGUIDefaultStorage(QString)));
    connect(m_report.data(), SIGNAL(variablesChanged()), this, SLOT(updateGUIvariables()));

    connect(ui->reportName, SIGNAL(editingFinished ()), this, SLOT(saveAll()));
    connect(ui->author, SIGNAL(editingFinished ()), this, SLOT(saveAll()));
    connect(ui->description, SIGNAL(textChanged()), this, SLOT(saveAll()));
}


void ReportProperties::disconnectReport()
{
    if (m_report) {
        disconnect(m_report, 0, this, 0);
    }
}


//void ReportProperties::syncData()
//{
//}


void ReportProperties::setGUIReportFilePath(const QString & url)
{
    if (ui->url->text() != url)
        ui->url->setText(url);
}


void ReportProperties::setGUIReportName(const QString & reportName)
{
    if (ui->reportName->text() != reportName)
        ui->reportName->setText(reportName);
}


void ReportProperties::setGUIReportAuthor(const QString & reportAuthor)
{
    if (ui->author->text() != reportAuthor)
        ui->author->setText(reportAuthor);
}


void ReportProperties::setGUIReportDescription(const QString & reportDescription)
{
    if (ui->description->toPlainText() != reportDescription)
        ui->description->setPlainText(reportDescription);
}


void ReportProperties::setGUIReportPrinter(CuteReport::PrinterInterface* printer)
{
    ui->printerName->blockSignals(true);
    ui->printerName->setText(printer ? printer->moduleFullName() : "");
    ui->lGlobalPrinter->setVisible(!printer);
    m_printerPropertyEditor->setVisible(printer);
    ui->printerName->blockSignals(false);
}


void ReportProperties::setGUIReportRenderer(CuteReport::RendererInterface* renderer)
{
    ui->rendererName->blockSignals(true);
    ui->rendererName->setText(renderer ? renderer->moduleFullName() : "");
    ui->lGlobalRenderer->setVisible(!renderer);
    m_rendererPropertyEditor->setVisible(renderer);
    ui->rendererName->blockSignals(false);
}


void ReportProperties::addGUIReportStorage(CuteReport::StorageInterface * storage)
{
    QTreeWidgetItem * newStorage = new QTreeWidgetItem ( ui->storageList, QStringList() << storage->objectName() << storage->moduleFullName() );
    ui->storageList->addTopLevelItem(new QTreeWidgetItem (newStorage));
}


void ReportProperties::removeGUIReportStorage(CuteReport::StorageInterface *storage)
{
    qDeleteAll(ui->storageList->findItems(storage->objectName(), Qt::MatchExactly));
    updateLayout();
}


void ReportProperties::setGUIDefaultStorage(const QString & storageName)
{
    ui->leDefaultStorage->setText(storageName);
}


void ReportProperties::updateGUIvariables()
{
    ui->variables->setRowCount(m_report->variables().keys().size());

    int row = -1;
    foreach (const QString & key, m_report->variables().keys()) {
        ++row;
        QTableWidgetItem *newName = new QTableWidgetItem(key);
        QTableWidgetItem *newValue = new QTableWidgetItem(m_report->variables().value(key).toString());
        newName->setFlags(Qt::ItemIsEnabled);
        newValue->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);
        ui->variables->setItem(row, 0, newName);
        ui->variables->setItem(row, 1, newValue);
    }
}


void ReportProperties::variableItemChanged(QTableWidgetItem * item)
{
    if (!m_report || !item || item->column() != 1)
        return;

    int row = item->row();
    m_report->setVariableValue(ui->variables->item(row,0)->text(), item->text());
}


void ReportProperties::setNewRendererModule()
{
    CuteReport::RendererInterface * renderer = m_core->reportCore()->rendererModule(ui->renderingModules->currentText());
    emit requestForNewRenderer(renderer);
}


void ReportProperties::setNewPrinterModule()
{
    const CuteReport::PrinterInterface * printer = m_core->reportCore()->printerModule(ui->printModules->currentText());
    emit requestForNewPrinter(printer);
}


void ReportProperties::setNewStorageModule()
{
    const CuteReport::StorageInterface * storage = m_core->reportCore()->storageModule(ui->storageModules->currentText());
    emit requestForNewStorage(storage);
}


void ReportProperties::deleteCurrentStorage()
{
    if (!ui->storageList->currentItem())
        return;
    QString storageName = ui->storageList->currentItem()->text(0);
    emit requestForDeleteStorage(storageName);
}


void ReportProperties::setDefaultStorage()
{
    if (!ui->storageList->currentItem())
        return;
    QString storageName = ui->storageList->currentItem()->text(0);
    emit requestForDefaultStorage(storageName);
}


void ReportProperties::clearDefaultStorage()
{
    emit requestForDefaultStorage(QString());
}


void ReportProperties::storagesListIndexChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
    Q_UNUSED(previous)
    if (!current) {
        delete m_currentStorageHelper;
        return;
    }

    QString storageName = current->text(0);
    CuteReport::StorageInterface * storage = 0;
    foreach (CuteReport::StorageInterface * st, m_report->storages()) {
        if (st->objectName() == storageName) {
            storage = st;
            break;
        }
    }

    if (!storage)
        return;

    if (m_currentStorageHelper)
        m_currentStorageHelper->save();
    delete m_currentStorageHelper;

    m_currentStorageHelper = storage->helper();
    ui->storageHelperLayout->addWidget(m_currentStorageHelper);
    ui->storageType->setText(storage->moduleFullName());
    updateLayout();
}


void ReportProperties::updateLayout()
{
//    if (m_currentStorageHelper) {
//        ui->storageHSpacer->changeSize(0,0, QSizePolicy::Maximum, QSizePolicy::Maximum);
//    } else {
//        ui->storageHSpacer->changeSize(0,0, QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
//    }
}


void ReportProperties::saveAll()
{
    if (m_report->name() != ui->reportName->text())
        m_report->setName(ui->reportName->text());
    if (m_report->author() != ui->author->text())
        m_report->setAuthor(ui->author->text());
    if (m_report->description() != ui->description->toPlainText())
        m_report->setDescription(ui->description->toPlainText());

    if (m_currentStorageHelper)
        m_currentStorageHelper->save();
}

}
