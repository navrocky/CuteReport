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

#include "storagesettingsdialog.h"
#include "ui_storagesettingsdialog.h"
#include "core.h"
#include "reportcore.h"
#include "storageinterface.h"
#include "mainwindow.h"
#include "optionsdialogstoragepage.h"

StorageSettingsDialog::StorageSettingsDialog(CuteDesigner::Core * core) :
    QDialog(core->mainWindow()),
    ui(new Ui::StorageSettingsDialog),
    m_core(core)
{
    ui->setupUi(this);
    setWindowTitle("Storage settings");

    m_page = new OptionsDialogStoragePage(core);
    m_page->activate();
    ui->mainLayout->addWidget(m_page);
}


StorageSettingsDialog::~StorageSettingsDialog()
{
    delete m_page;
    delete ui;
}


CuteReport::StorageInterface *StorageSettingsDialog::currentStorage()
{
    return m_page->currentStorage();
}
