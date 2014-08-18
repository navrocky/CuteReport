/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2012 by Mikhalov Alexander                              *
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
#include "csvdatasethelper.h"
#include "csvdataset.h"
#include "ui_csvdatasethelper.h"
#include "reportcore.h"
#include "stdstoragedialog.h"

CsvDatasetHelper::CsvDatasetHelper(CsvDataset * dataset) :
    m_ui(new Ui::CsvDatasetHelper),
    m_dataset(dataset)
{
    m_ui->setupUi(this);

    load();

    if (dataset->keepData()) {
        CuteReport::ReportInterface * report = static_cast<CuteReport::ReportInterface*> (m_dataset->parent());
        QString fileName =  m_dataset->reportCore()->localCachedFileName(m_dataset->fileName(), report);

        QStringList list;
        QFile data(fileName);
        if (data.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream in(&data);
            do {
                list.append(in.readLine());
            } while (!in.atEnd() );
        }
        m_ui->text->setPlainText(list.join("\n"));
    } else {
        m_ui->text->setPlainText(dataset->list().join("\n"));
    }

}


CsvDatasetHelper::~CsvDatasetHelper()
{
    delete m_ui;
}


void CsvDatasetHelper::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void CsvDatasetHelper::load()
{
    m_ui->leFileName->setText( m_dataset->fileName());
    m_ui->leDelimeter->setText(m_dataset->delimeter());
    m_ui->cbKeepData->setCheckState(m_dataset->keepData() ? Qt::Checked : Qt::Unchecked);
    m_ui->leFileName->setText(m_dataset->fileName());
    m_ui->cbFirstRowIsHeader->setCheckState(m_dataset->firstRowIsHeader() ? Qt::Checked : Qt::Unchecked);
}


void CsvDatasetHelper::save()
{
    m_dataset->setFileName(m_ui->leFileName->text());
    m_dataset->setDelimeter(m_ui->leDelimeter->text());
    m_dataset->setKeepData((m_ui->cbKeepData->checkState() == Qt::Checked) ? true : false);
    m_dataset->setFirstRowIsHeader((m_ui->cbFirstRowIsHeader->checkState() == Qt::Checked) ? true : false);
    m_dataset->setList( m_ui->text->toPlainText().split('\n') );
}


void CsvDatasetHelper::on_bBrowse_clicked()
{
    CuteReport::ReportInterface * report = static_cast<CuteReport::ReportInterface*> (m_dataset->parent());
    CuteReport::StdStorageDialog d(m_dataset->reportCore(), m_dataset->reportCore()->rootWidget(), "Load CSV file");

    if( d.exec() != QDialog::Accepted)
        return;

    QString fileURL = d.currentObjectUrl();
    QString fileContent =  m_dataset->reportCore()->loadObject(fileURL, report).toString();

    m_ui->leFileName->setText(fileURL);
    m_ui->text->setPlainText(fileContent);
}

