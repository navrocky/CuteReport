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

#include "optionsdialogstoragepage.h"
#include "ui_optionsdialogstoragepage.h"
#include "core.h"
#include "reportcore.h"
#include "storageinterface.h"

OptionsDialogStoragePage::OptionsDialogStoragePage(CuteDesigner::Core *core) :
    OptionsDialogPageInterface(core),
    ui(new Ui::OptionsDialogStoragePage),
    m_core(core),
    m_currentStorage(0)
{
    ui->setupUi(this);

    ui->modules->addItems(m_core->reportCore()->moduleNames(CuteReport::StorageModule));

    CuteReport::StorageInterface* defaultStorage  = m_core->reportCore()->defaultStorage();

    if (defaultStorage) {
        ui->modules->setCurrentIndex(ui->modules->findText(defaultStorage->moduleFullName()));
        currentModuleChanged(defaultStorage->moduleFullName());
    }

    connect(ui->modules, SIGNAL(currentIndexChanged(QString)), this, SLOT(currentModuleChanged(QString)));
    connect(ui->cbDefault, SIGNAL(toggled(bool)), this, SLOT(defaultStateChanged(bool)));
}


OptionsDialogStoragePage::~OptionsDialogStoragePage()
{
    deactivate();
    delete ui;
}


void OptionsDialogStoragePage::activate()
{
}


void OptionsDialogStoragePage::deactivate()
{
    if (m_currentStorage) {
        m_currentStorage->helper()->save();
        m_core->settings()->setValue(QString("CuteReport/Storage_%1_options").arg(m_currentStorage->moduleFullName().replace("::","_")),
                                     m_core->reportCore()->moduleOptionsStr(m_currentStorage));
    }
}


QListWidgetItem * OptionsDialogStoragePage::createButton(QListWidget * listWidget)
{
    QListWidgetItem *configButton = new QListWidgetItem(listWidget);
    configButton->setIcon(QIcon(":/images/storage_96x96.jpeg"));
    configButton->setText(tr("Storage"));
    configButton->setTextAlignment(Qt::AlignHCenter);
    configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    return configButton;
}


CuteReport::StorageInterface *OptionsDialogStoragePage::currentStorage()
{
    return m_currentStorage;
}


void OptionsDialogStoragePage::currentModuleChanged(QString moduleName)
{
    if (m_currentStorage) {
        m_currentStorage->helper()->save();
        m_core->settings()->setValue(QString("CuteReport/Storage_%1_options").arg(m_currentStorage->moduleFullName().replace("::","_")),
                                     m_core->reportCore()->moduleOptionsStr(m_currentStorage)) ;
        delete m_currentStorage->helper();
    }

    CuteReport::StorageInterface* module = m_core->reportCore()->storageModule(moduleName);

    if (module) {
        m_currentStorage = module;
        m_core->reportCore()->setModuleOptionsStr(m_currentStorage, m_core->settings()->value(QString("CuteReport/Storage_%1_options")
                                                                                              .arg(m_currentStorage->moduleFullName().replace("::","_"))).toString());
        ui->optionsLayout->addWidget(m_currentStorage->helper());
        ui->cbDefault->blockSignals(true);
        ui->cbDefault->setChecked(m_core->reportCore()->defaultStorage() && m_core->reportCore()->defaultStorage()->moduleFullName() == m_currentStorage->moduleFullName());
        ui->cbDefault->blockSignals(false);
    }
}


void OptionsDialogStoragePage::defaultStateChanged(bool state)
{
    m_core->reportCore()->setDefaultStorage(state ? m_currentStorage->moduleFullName() : "");
    m_core->settings()->setValue(QString("CuteReport/PrimaryStorage"), m_core->reportCore()->defaultStorage()->moduleFullName());
}

