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
#include "dataseteditor.h"
#include "datasetcontainer.h"
#include "reportcore.h"
#include "datasetinterface.h"
#include "propertyeditor.h"
#include "reportinterface.h"
#include "core.h"
#include "renamedialog.h"
#include "mainwindow.h"

using namespace CuteDesigner;

inline void initMyResource() { Q_INIT_RESOURCE(dataset_editor); }

DatasetEditor::DatasetEditor(QObject *parent) :
    ModuleInterface(parent),
    m_currentDataset(0),
    ui(0)
{
}


DatasetEditor::~DatasetEditor()
{
    if (ui)
        delete ui;
}


void DatasetEditor::init(Core *core)
{
    initMyResource();
    ModuleInterface::init(core);

    ui = new DatasetContainer(this);
    ui->setEnabled(false);
    ui->addDatasetPlugins(core->reportCore()->datasetModules());
    ui->setEnabled(core->currentReport());

    m_propertyEditor = core->createPropertyEditor(ui);
    ui->addPropertyEditor(m_propertyEditor);

    connect(ui.data(), SIGNAL(requestForCreateDataset(QString)) , this, SLOT(slotRequestForCreateDataset(QString)), Qt::QueuedConnection);
    connect(ui.data(), SIGNAL(requestForDeleteDataset()), this, SLOT(slotRequestForRemoveDataset()), Qt::QueuedConnection);
    connect(ui.data(), SIGNAL(currentTabChanged(QString)), this, SLOT(slotCurrentTabChanged(QString)), Qt::QueuedConnection);
    connect(ui.data(), SIGNAL(requestForPopulateDataset()), this, SLOT(slotRequestForPopulatedataset()), Qt::QueuedConnection);
    connect(ui.data(), SIGNAL(requestForRename(QString)), this, SLOT(slotRequestForRename(QString)));

    connect(core, SIGNAL(currentReportChanged(CuteReport::ReportInterface*)), this, SLOT(slotCurrentReportChanged(CuteReport::ReportInterface*)));
}


void DatasetEditor::reloadSettings()
{
    if (ui)
        ui->reloadSettings();
}


void DatasetEditor::saveSettings()
{
    if (ui)
        ui->saveSettings();
}


QWidget * DatasetEditor::view()
{
    return ui;
}


void DatasetEditor::sync()
{
    if (m_currentDataset)
        m_currentDataset->helper()->save();
}


void DatasetEditor::activate()
{
    core()->reportCore()->log(CuteReport::LogDebug, "DatasetEditor", "activate");
}


void DatasetEditor::deactivate()
{
    sync();
    core()->reportCore()->log(CuteReport::LogDebug, "DatasetEditor", "deactivate");
}


QIcon DatasetEditor::icon()
{
    return QIcon(":/images/database_48.png");
}


QString DatasetEditor::name()
{
    return QString("Datasets");
}


void DatasetEditor::slotRequestForCreateDataset(QString name)
{
    sync();
    core()->reportCore()->log(CuteReport::LogDebug, "DatasetEditor", "slotCreateDataset:" + name);
    CuteReport::DatasetInterface * dataset = core()->reportCore()->createDatasetObject(core()->currentReport(), name);
    if (dataset) {
        m_currentDataset = dataset;
        m_datasets.insert(m_currentDataset, m_currentDataset->objectName());
        preprocessDataset(m_currentDataset);
        ui->addTab(dataset->helper() ? dataset->helper() : new QWidget(ui), dataset->icon(), dataset->objectName());
        m_propertyEditor->setObject(dataset);
        ui->setCurrentTab(dataset->objectName());
    }
}


void DatasetEditor::slotRequestForRemoveDataset()
{
    if (!m_currentDataset)
        return;

    QList<CuteReport::DatasetInterface*> datasets = core()->currentReport()->datasets();
    int index = -1;
    for (int i=0; i<datasets.count(); i++)
        if (datasets[i] == m_currentDataset) {
            index = i;
            break;
        }


    postprocessDataset(m_currentDataset);
    ui->deleteTab(m_currentDataset->objectName());
    m_datasets.remove(m_currentDataset);
    delete m_currentDataset;


    datasets = core()->currentReport()->datasets();

    if (index > -1) {
        if (index > datasets.count() -1)
                index = datasets.count() -1;
        if (index < 0)
            index = 0;

        CuteReport::DatasetInterface* newCurrentObject = datasets.count() ? datasets[index] : 0;
        m_currentDataset = newCurrentObject;
        core()->setCurrentDataset(m_currentDataset);
        m_propertyEditor->setObject(m_currentDataset);
        if (newCurrentObject) {
            ui->setCurrentTab(newCurrentObject->objectName());
        }
    }
}


void DatasetEditor::slotCurrentTabChanged(QString name)
{
    sync();
    CuteReport::DatasetInterface * dataset = core()->reportCore()->datasetByName(name, core()->currentReport());
    if (dataset) {
        m_currentDataset = dataset;
        core()->setCurrentDataset(m_currentDataset);
        m_propertyEditor->setObject(dataset);
    }
}


void DatasetEditor::slotRequestForPopulatedataset()
{
    core()->reportCore()->log(CuteReport::LogDebug, "DatasetEditor", "slotRequestForPopulatedataset");
    if (!m_currentDataset)
        return;

    m_currentDataset->helper()->save();
    m_currentDataset->populate();
    ui->setModel(m_currentDataset->model());
    ui->setError(m_currentDataset->lastError());
}


void DatasetEditor::slotCurrentReportChanged(CuteReport::ReportInterface * report)
{
    CuteReport::ReportCore::log(CuteReport::LogDebug, "DatasetEditor", "slotCurrentReportChanged");
    ui->setEnabled(core()->currentReport());

    ui->deleteAllTabs();

    if (m_currentReport) {
        if (m_currentReport)
            foreach(CuteReport::DatasetInterface * dataset, m_datasets.keys())
                postprocessDataset(dataset);

    }

    m_datasets.clear();

    if (report) {
        QMap<QString, CuteReport::DatasetInterface *> sortedList;
        foreach (CuteReport::DatasetInterface * dataset, report->datasets()) {
            sortedList.insert(dataset->objectName(), dataset);
        }

        QMap<QString, CuteReport::DatasetInterface *>::iterator i = sortedList.begin();

        while (i != sortedList.end()) {
            m_datasets.insert(*i, (*i)->objectName());
            ui->addTab((*i)->helper(), (*i)->icon(), (*i)->objectName());
            preprocessDataset((*i));
            ++i;
        }

        m_currentDataset = report->datasets().count() ? report->datasets()[0] : 0;


        if (m_currentDataset)
            ui->setCurrentTab(m_currentDataset->objectName());

    } else {
        m_currentDataset = 0;
    }

    m_propertyEditor->setObject(m_currentDataset);
    m_currentReport = report;
}


void DatasetEditor::preprocessDataset(CuteReport::DatasetInterface *dataset)
{
    connect(dataset, SIGNAL(objectNameChanged(QString)), this, SLOT(datasetNameChanged(QString)));
}


void DatasetEditor::postprocessDataset(CuteReport::DatasetInterface * dataset)
{
    disconnect(dataset, 0, this, 0);
}


void DatasetEditor::datasetNameChanged(QString name)
{
    CuteReport::DatasetInterface * dataset = static_cast<CuteReport::DatasetInterface*>(sender());

    if (dataset) {
        ui->changeTabText(m_datasets.value(dataset), name);
        m_datasets.insert(dataset, dataset->objectName());
    }
}


void DatasetEditor::slotRequestForRename(QString currentName)
{
    CuteReport::DatasetInterface * dataset = core()->currentReport()->dataset(currentName);
    RenameDialog d(dataset, core()->currentReport(), core()->mainWindow());
    d.setWindowTitle("Dataset renaming");
    if (d.exec() == QDialog::Accepted) {
        dataset->setObjectName(d.newName());
    }
}

//suit_end_namespace

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(DatasetEditor, DatasetEditor)
#endif
