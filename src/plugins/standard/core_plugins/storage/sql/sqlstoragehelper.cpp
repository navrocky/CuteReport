/***************************************************************************
 *   This file is part of the CuteReport project                           *
 *   Copyright (C) 2014 by Ivan Volkov                                     *
 *   wulff007@gmail.com                                                    *
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
 **                  GNU Lesser General Public License                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library.                                      *
 *   If not, see <http://www.gnu.org/licenses/>.                           *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/
#include "sqlstoragehelper.h"
#include "ui_sqlstoragehelper.h"

#include <QtSql>

SqlStorageHelper::SqlStorageHelper(StorageSql * storage, VisibleOptions visibleOptions) :
    CuteReport::StorageHelperInterface(storage, visibleOptions),
    ui(new Ui::SqlStorageHelper),
    m_storage(storage)
{
    ui->setupUi(this);

    ui->cmbDriver->addItems(QSqlDatabase::drivers());
    connect(ui->ckUseDefault, SIGNAL(toggled(bool)), ui->edConnection, SLOT(setDisabled(bool)));

    load();

    // sync signals
    connect(ui->edHost, SIGNAL(editingFinished()), this, SLOT(save()));
    connect(ui->cmbDriver, SIGNAL(editTextChanged(QString)), this, SLOT(save()));
    connect(ui->edPort, SIGNAL(valueChanged(int)), this, SLOT(save()));
    connect(ui->edUser, SIGNAL(editingFinished()), this, SLOT(save()));
    connect(ui->edPassword, SIGNAL(editingFinished()), this, SLOT(save()));
    connect(ui->edDatabase, SIGNAL(editingFinished()), this, SLOT(save()));
    connect(ui->edTable, SIGNAL(editingFinished()), this, SLOT(save()));
    connect(ui->edColId, SIGNAL(editingFinished()), this, SLOT(save()));
    connect(ui->edColName, SIGNAL(editingFinished()), this, SLOT(save()));
    connect(ui->edColData, SIGNAL(editingFinished()), this, SLOT(save()));
    connect(ui->ckUseDefault, SIGNAL(toggled(bool)), this, SLOT(save()));
    connect(ui->edConnection, SIGNAL(editingFinished()), this, SLOT(save()));

    connectObject();
}


SqlStorageHelper::~SqlStorageHelper()
{
    delete ui;
}


void SqlStorageHelper::load()
{
    if (ui->edHost->text() != m_storage->host())
        ui->edHost->setText(m_storage->host());
    if (ui->cmbDriver->lineEdit()->text() != m_storage->driver())
        ui->cmbDriver->lineEdit()->setText(m_storage->driver());
    if (ui->edPort->value() != m_storage->port())
        ui->edPort->setValue(m_storage->port());
    if (ui->edUser->text() != m_storage->user())
        ui->edUser->setText(m_storage->user());
    if (ui->edPassword->text() != m_storage->password())
        ui->edPassword->setText(m_storage->password());
    if (ui->edDatabase->text() != m_storage->database())
        ui->edDatabase->setText(m_storage->database());
    if (ui->edTable->text() != m_storage->tableName())
        ui->edTable->setText(m_storage->tableName());
    if (ui->edColId->text() != m_storage->columnId())
        ui->edColId->setText(m_storage->columnId());
    if (ui->edColName->text() != m_storage->columnName())
        ui->edColName->setText(m_storage->columnName());
    if (ui->edColData->text() != m_storage->columnData())
        ui->edColData->setText(m_storage->columnData());
    if (ui->ckUseDefault->isChecked() != m_storage->useAsDefaultConnection())
        ui->ckUseDefault->setChecked(m_storage->useAsDefaultConnection());
    if (ui->edConnection->text() != m_storage->connectionId())
        ui->edConnection->setText(m_storage->connectionId());
}


void SqlStorageHelper::save()
{
    disconnectObject();

    if (ui->edHost->text() != m_storage->host())
        m_storage->setHost(ui->edHost->text());
    if (ui->cmbDriver->lineEdit()->text() != m_storage->driver())
        m_storage->setDriver(ui->cmbDriver->currentText());
    if (ui->edPort->value() != m_storage->port())
        m_storage->setPort(ui->edPort->value());
    if (ui->edUser->text() != m_storage->user())
        m_storage->setUser(ui->edUser->text());
    if (ui->edPassword->text() != m_storage->password())
        m_storage->setPassword(ui->edPassword->text());
    if (ui->edDatabase->text() != m_storage->database())
        m_storage->setDatabase(ui->edDatabase->text());
    if (ui->edTable->text() != m_storage->tableName())
        m_storage->setTableName(ui->edTable->text());
    if (ui->edColId->text() != m_storage->columnId())
        m_storage->setColumnId(ui->edColId->text());
    if (ui->edColName->text() != m_storage->columnName())
        m_storage->setColumnName(ui->edColName->text());
    if (ui->edColData->text() != m_storage->columnData())
        m_storage->setColumnData(ui->edColData->text());
    if (ui->ckUseDefault->isChecked() != m_storage->useAsDefaultConnection())
        m_storage->setUseAsDefaultConnection(ui->ckUseDefault->isChecked());
    if (ui->edConnection->text() != m_storage->connectionId())
        m_storage->setConnectionId(ui->edConnection->text());

    connectObject();
}


void SqlStorageHelper::connectObject()
{
    connect(m_storage, SIGNAL(changed()), this, SLOT(load()));
}


void SqlStorageHelper::disconnectObject()
{
    disconnect(m_storage, SIGNAL(changed()), this, SLOT(load()));
}
