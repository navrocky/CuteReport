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
#include "filesystemstoragehelper.h"
#include "ui_filesystemstoragehelper.h"
#include "filesystem.h"
#include <QFileDialog>

using namespace CuteReport;

FileSystemStorageHelper::FileSystemStorageHelper(StorageFileSystem *storage, VisibleOptions visibleOptions) :
    StorageHelperInterface(storage),
    ui(new Ui::FileSystemStorageHelper),
    m_storage(storage)
{
    Q_UNUSED(visibleOptions);

    ui->setupUi(this);

    load();

    connect(ui->bChooseObjectsRoot, SIGNAL(clicked()), this, SLOT(objectsFolderChooseClicked()));

    // sync signals
    connect(ui->objectsRoot, SIGNAL(editingFinished()), this, SLOT(save()));
    connect(ui->askOverWrite, SIGNAL(clicked()), this, SLOT(save()));

    connectObject();
}


FileSystemStorageHelper::~FileSystemStorageHelper()
{
    delete ui;
}


void FileSystemStorageHelper::load()
{
    if (ui->objectsRoot->text() != m_storage->objectsRootPath())
        ui->objectsRoot->setText(m_storage->objectsRootPath());
    if (m_storage->askForOverwrite() != ui->askOverWrite->isChecked())
        ui->askOverWrite->setChecked(m_storage->askForOverwrite());
}


void FileSystemStorageHelper::save()
{
    disconnectObject();

    if (ui->objectsRoot->text() != m_storage->objectsRootPath())
        m_storage->setObjectsRootPath(ui->objectsRoot->text());
    if (m_storage->askForOverwrite() != ui->askOverWrite->isChecked())
        ui->askOverWrite->setChecked(m_storage->askForOverwrite());

    connectObject();
}


void FileSystemStorageHelper::objectsFolderChooseClicked()
{
    QFileDialog d(this);
    d.setFileMode(QFileDialog::Directory);
    d.setOption(QFileDialog::ShowDirsOnly, true);
    d.setDirectory(m_storage->objectsRootPath().isEmpty() ? m_storage->objectsRootPath() : m_storage->objectsRootPath());
    if (d.exec() == QDialog::Accepted) {
        ui->objectsRoot->setText(d.selectedFiles().at(0));
        m_storage->setObjectsRootPath(ui->objectsRoot->text());
    }
}


void FileSystemStorageHelper::connectObject()
{
    connect(m_storage, SIGNAL(changed()), this, SLOT(load()));
}


void FileSystemStorageHelper::disconnectObject()
{
    disconnect(m_storage, SIGNAL(changed()), this, SLOT(load()));
}
