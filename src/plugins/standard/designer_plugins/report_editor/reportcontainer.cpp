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
#include "reportcontainer.h"
#include "ui_reportcontainer.h"
#include "reporteditor.h"

#include <QDebug>

ReportContainer::ReportContainer(ReportEditor * reportEditor, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReportContainer),
    m_reportEditor(reportEditor)
{
    ui->setupUi(this);
//    ui->cbDatasets->clear();


    connect(ui->newReport, SIGNAL(clicked()), this, SIGNAL(requestForNewReport()));
    connect(ui->openReport, SIGNAL(clicked()), this, SIGNAL(requestForOpenReport()));
    connect(ui->saveReport, SIGNAL(clicked()), this, SIGNAL(requestForSaveReport()));
    connect(ui->closeReport, SIGNAL(clicked()), this, SIGNAL(requestForCloseReport()));
    connect(ui->openTemplate, SIGNAL(clicked()), this, SIGNAL(requestForTemplate()));
    connect(ui->tabs, SIGNAL(CurrentChanged(int)), this, SLOT(slotCurrentTabChanged(int)));
    connect(ui->tabs, SIGNAL(tabDoubleClicked(int)), this, SLOT(slotTabDoubleClicked(int)));

//    connect(ui->cbDatasets, SIGNAL(activated(int)), this, SLOT(slotNewDataset(int)));
//    connect(ui->pbDelete, SIGNAL(clicked()), this, SLOT(slotDeleteDataset()));
//    connect(ui->datasetTabs, SIGNAL(CurrentChanged(int)), this, SLOT(slotCurrentDatasetChanged(int)));
//    connect(ui->bProperties, SIGNAL(toggled(bool)), this, SLOT(slotPropertyButtonToggled(bool)));
//    connect(ui->bTest, SIGNAL(clicked()), this, SLOT(slotShowTestPage()));
//    connect(ui->bCloseTest, SIGNAL(clicked()), this, SLOT(slotShowDatasetpage()));

    ui->tabs->hide();
}


ReportContainer::~ReportContainer()
{
    delete ui;
}


void ReportContainer::saveSettings()
{
//    m_core->setSettingValue("DatasetEditor/splitterState", ui->splitter->saveState());
//    m_core->setSettingValue("DatasetEditor/splitter2State", ui->splitterTestPage->saveState());
    m_reportEditor->core()->setSettingValue("ReportProperties/tabMode", ui->tabs->mode());
//    m_core->setSettingValue("DatasetEditor/propertiesShown", ui->bProperties->isChecked());
}


void ReportContainer::reloadSettings()
{
//    ui->splitter->restoreState((m_core->getSettingValue("DatasetEditor/splitterState").toByteArray()));
//    ui->splitterTestPage->restoreState((m_core->getSettingValue("DatasetEditor/splitter2State").toByteArray()));
//    ui->bProperties->setChecked(m_core->getSettingValue("DatasetEditor/propertiesShown").toBool());

//    ui->tabs->SetCurrentIndex(0);
    FancyTabWidget::Mode default_mode = FancyTabWidget::Mode_LargeSidebar;
    ui->tabs->SetMode(FancyTabWidget::Mode(m_reportEditor->core()->getSettingValue("ReportProperties/tabMode", default_mode).toInt()));
//    ui->stackedWidget->setCurrentIndex(0);
}


void ReportContainer::addTab(QWidget * const widget, const QIcon & icon, const QString & name)
{
    ui->tabs->AddTab(widget, icon, name);
    ui->tabs->updateState();
}


void ReportContainer::removeTab(const QString & name)
{
    ui->tabs->deleteTab(name);
    ui->tabs->updateState();
}


void ReportContainer::removeTab(int index)
{
    ui->tabs->deleteTab(index);
    ui->tabs->updateState();
}


void ReportContainer::setCurrentTab(QString name)
{
    ui->tabs->blockSignals(true);
    for (int i=0; i<ui->tabs->tabsCount(); ++i)
        if (ui->tabs->tabText(i) == name) {
            ui->tabs->SetCurrentIndex(i);
            break;
        }
    ui->tabs->blockSignals(false);
}


void ReportContainer::setCurrentTab(int index)
{
    ui->tabs->blockSignals(true);
    ui->tabs->SetCurrentIndex(index);
    ui->tabs->blockSignals(false);
}


void ReportContainer::changeTabText(QString oldText, QString newText)
{
    for (int i=0; i< ui->tabs->tabsCount(); ++i) {
        if (ui->tabs->tabText(i) == oldText) {
            ui->tabs->changeText(newText, i);
            return;
        }
    }
}


void ReportContainer::changeTabText(int index, QString newText)
{
    ui->tabs->changeText(newText, index);
}


void ReportContainer::slotCurrentTabChanged(int index)
{
//    emit currentTabChanged(ui->tabs->current_text());
    emit currentTabChanged(index);
}


void ReportContainer::slotTabDoubleClicked(int index)
{
//    emit requestForRenameReport(ui->tabs->tabText(index));
    emit requestForRenameReport(index);
}
