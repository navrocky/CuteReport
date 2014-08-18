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
#include "gitstoragehelper.h"
#include "ui_gitstoragehelper.h"
#include "git.h"
#include <QFileDialog>

using namespace CuteReport;

GitStorageHelper::GitStorageHelper(StorageGit *storage, VisibleOptions visibleOptions) :
    StorageHelperInterface(storage),
    ui(new Ui::GitStorageHelper),
    m_storage(storage)
{
    Q_UNUSED(visibleOptions);

    ui->setupUi(this);

    load();

    connect(ui->bLocalPath, SIGNAL(clicked()), this, SLOT(localPathChooseClicked()));
    connect(ui->bSync, SIGNAL(clicked()), this, SLOT(slotSync()));
    connect(ui->bGitBinary, SIGNAL(clicked()), this, SLOT(slotBinaryBrowse()));
    connect(ui->gitBinaryPath, SIGNAL(returnPressed()), this, SLOT(setGitVersion()));

    QTimer::singleShot(0, this, SLOT(setGitVersion()));
}


GitStorageHelper::~GitStorageHelper()
{
    delete ui;
}


void GitStorageHelper::load()
{
    ui->askOverWrite->setChecked(m_storage->askForOverwrite());
    ui->remoteUrl->setText(m_storage->remoteUrl());
    ui->localPath->setText(m_storage->localPath());
    ui->password->setText(m_storage->password());
    ui->login->setText(m_storage->login());
    ui->gitBinaryPath->setText(m_storage->gitBinaryPath());
}


void GitStorageHelper::save()
{
    m_storage->setAskForOverwrite(ui->askOverWrite->isChecked());
    m_storage->setRemoteUrl(ui->remoteUrl->text());
    m_storage->setLocalPath(ui->localPath->text());
    m_storage->setLogin(ui->login->text());
    m_storage->setPassword(ui->password->text());
    m_storage->setGitBinaryPath(ui->gitBinaryPath->text());
}


void GitStorageHelper::slotSync()
{
    save();
    m_storage->sync();
}


bool GitStorageHelper::setGitVersion()
{
    QString version;
    bool result = SyncManager::checkBinary(ui->gitBinaryPath->text(), &version);
    version = version.trimmed();
    ui->gitVersion->setText("Version: " + (version.isEmpty() ? "unknown" : version));

    QPalette pal(palette());
    if (!result) {
        pal.setColor(QPalette::Base, QColor(255,155,155));
    }

    ui->gitBinaryPath->setPalette(pal);

    return result;
}


void GitStorageHelper::showSyncMessage(const QString & message)
{
    ui->output->setText(message);
}


void GitStorageHelper::slotBinaryBrowse()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Git binary");
    ui->gitBinaryPath->setText(fileName);
    m_storage->isBinaryCorrect = setGitVersion();
}


void GitStorageHelper::localPathChooseClicked()
{
    QString localPath = m_storage->localPath().isEmpty() ? m_storage->m_localDefaultPath : m_storage->localPath();
    QString dir = QFileDialog::getExistingDirectory(
                this,
                "Select directory",
                localPath);

    if (dir.isEmpty())
        return;

    ui->localPath->setText(dir);

}
