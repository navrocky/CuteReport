/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2012-2014 by Mikhalov Alexander                         *
 *   alexander.mikhalov@gmail.com                                          *
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
 ****************************************************************************/
#include <QtGui>
#include <QFileDialog>

#include "filesystemdatasethelper.h"
#include "filesystemdataset.h"
#include "ui_filesystemdatasethelper.h"
#include "reportcore.h"

FileSystemDatasetHelper::FileSystemDatasetHelper(FileSystemDataset *dataset) :
    m_ui(new Ui::FileSystemDatasetHelper),
    m_dataset(dataset)
{
    m_ui->setupUi(this);

    int index = -1;
    for (int i = 0; i < m_dataset->metaObject()->enumeratorCount(); ++i) {
        QString enumName = m_dataset->metaObject()->enumerator(i).name();
        if (enumName == QString("PathAppearance")) {
            index = i;
            break;
        }
    }

    if (index > -1) {
        for (int en = 0;en < m_dataset->metaObject()->enumerator(index).keyCount();en++) {
            QString itemName = m_dataset->metaObject()->enumerator(index).key(en);
            int itemData = m_dataset->metaObject()->enumerator(index).value(en);
            m_ui->cbPathAppearance->insertItem(en, itemName, itemData);
        }
    }

    load();
}


FileSystemDatasetHelper::~FileSystemDatasetHelper()
{
    delete m_ui;
}


void FileSystemDatasetHelper::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void FileSystemDatasetHelper::load()
{
    m_ui->leDir->setText(m_dataset->dir());
    m_ui->recursionLevel->setValue(m_dataset->recursionLevel());
    m_ui->cbPathAppearance->setCurrentIndex(m_ui->cbPathAppearance->findData(m_dataset->pathAppearance()));
    m_ui->filter->setText(m_dataset->nameFilters().join("; "));
    m_ui->maxNumber->setValue(m_dataset->maxNumber());

    FileSystemDataset::Filters filters = m_dataset->filters();
    m_ui->cbFiles->setChecked(filters & FileSystemDataset::Files);
    m_ui->cbDirs->setChecked(filters & FileSystemDataset::Dirs);
    m_ui->cbAllDirectories->setChecked(filters & FileSystemDataset::AllDirs);
    m_ui->cbHidden->setChecked(filters & FileSystemDataset::Hidden) ;
    m_ui->cbReadable->setChecked(filters & FileSystemDataset::Readable) ;
    m_ui->cbWritable->setChecked(filters & FileSystemDataset::Writable) ;
    m_ui->cbExecutable->setChecked(filters & FileSystemDataset::Executable);
    m_ui->cbSystem->setChecked(filters & FileSystemDataset::System);
}


void FileSystemDatasetHelper::save()
{
    FileSystemDataset::Filters filters;
    if (m_ui->cbFiles->isChecked()) filters |= FileSystemDataset::Files;
    if (m_ui->cbDirs->isChecked()) filters |= FileSystemDataset::Dirs;
    if (m_ui->cbAllDirectories->isChecked()) filters |= FileSystemDataset::AllDirs;
    if (m_ui->cbHidden->isChecked()) filters |= FileSystemDataset::Hidden;
    if (m_ui->cbReadable->isChecked()) filters |= FileSystemDataset::Readable;
    if (m_ui->cbWritable->isChecked()) filters |= FileSystemDataset::Writable;
    if (m_ui->cbExecutable->isChecked()) filters |= FileSystemDataset::Executable;
    if (m_ui->cbSystem->isChecked()) filters |= FileSystemDataset::System;

    m_dataset->setFilters(filters);
    m_dataset->setDir(m_ui->leDir->text());
    m_dataset->setRecursionLevel(m_ui->recursionLevel->value());
    m_dataset->setPathAppearance((FileSystemDataset::PathAppearance) m_ui->cbPathAppearance->itemData(m_ui->cbPathAppearance->currentIndex()).toInt());
    m_dataset->setNameFilters(m_ui->filter->text().split(QRegExp(" *; *"), QString::SkipEmptyParts));
    m_dataset->setMaxNumber(m_ui->maxNumber->value());
}


void FileSystemDatasetHelper::on_bBrowse_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Directory"),
                                                    m_ui->leDir->text(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    m_ui->leDir->setText(dir);
}

