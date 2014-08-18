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

#include "sqldataset.h"
#include "sqldatasethelper.h"
#include "ui_sqldatasethelper.h"
#include "sqlsyntaxhighlighter.h"
#include "reportcore.h"
#include "stdstoragedialog.h"

#include <QFileDialog>

using namespace CuteReport;

SqlDatasetHelper::SqlDatasetHelper(SqlDataset *dataset) :
    m_ui(new Ui::SqlDatasetEditor),
    m_dataset(dataset)
{
    m_ui->setupUi(this);

    m_ui->bExtended->setChecked(true);
    m_ui->frame->setVisible(true);

//    SQLSyntaxHighlighter * syntax = new SQLSyntaxHighlighter(m_ui->textEdit->document());

    m_ui->driversList->addItems(QSqlDatabase::drivers());

    load();

    connect(m_dataset, SIGNAL(dbhostChanged(QString)), this, SLOT(slotDbhostChanged(QString)));
    connect(m_dataset, SIGNAL(dbnameChanged(QString)), this, SLOT(slotDbnameChanged(QString)));
    connect(m_dataset, SIGNAL(dbuserChanged(QString)), this, SLOT(slotDbuserChanged(QString)));
    connect(m_dataset, SIGNAL(dbpasswdChanged(QString)), this, SLOT(slotDbpasswdChanged(QString)));
    connect(m_dataset, SIGNAL(driverChanged(QString)), this, SLOT(slotDriverChanged(QString)));

    connect(m_ui->databaseField, SIGNAL(editingFinished()), this, SLOT(slotDatabaseEdited()));
    connect(m_ui->hostField, SIGNAL(editingFinished()), this, SLOT(slotHostEdited()));
    connect(m_ui->userField, SIGNAL(editingFinished()), this, SLOT(slotUserEdited()));
    connect(m_ui->passwordField, SIGNAL(editingFinished()), this, SLOT(slotpasswordEdited()));
    connect(m_ui->driver, SIGNAL(editingFinished()), this, SLOT(slotDriverEdited()));
    connect(m_ui->textEdit, SIGNAL(editingFinished()), this, SLOT(slotQueryEdited()));
    connect(m_ui->bCopyDriverName, SIGNAL(clicked()), this, SLOT(slotCopyDriverName()));
    connect(m_ui->bExtended, SIGNAL(toggled(bool)), this, SLOT(slotExtendedOptionsToggled(bool)));
    connect(m_ui->bLoadFile, SIGNAL(clicked()), this, SLOT(slotLoadFile()));
}

SqlDatasetHelper::~SqlDatasetHelper()
{
    delete m_ui;
}

void SqlDatasetHelper::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

//void SqlDatasetHelper::setDataset(SqlDataset *dtst)
//{
//    Q_ASSERT(dtst);
//    m_dataset = dtst;
//    if (! dynamic_cast<SqlDataset*> (dtst))
//		return;

//    m_ui->textEdit->setPlainText(dynamic_cast<SqlDataset*> (dtst)->text());
//}

void SqlDatasetHelper::save()
{
    m_dataset->setdbhost(m_ui->hostField->text());
    m_dataset->setdbname(m_ui->databaseField->text());
    m_dataset->setdbuser(m_ui->userField->text());
    m_dataset->setdbpasswd(m_ui->passwordField->text());
    m_dataset->setQuery( m_ui->textEdit->toPlainText());
    m_dataset->setDriver(m_ui->driver->text());
}

void SqlDatasetHelper::load()
{
    m_ui->hostField->setText(m_dataset->dbhost());
    m_ui->databaseField->setText(m_dataset->dbname());
    m_ui->userField->setText(m_dataset->dbuser());
    m_ui->passwordField->setText(m_dataset->dbpasswd());
    m_ui->driver->setText(m_dataset->driver());
    m_ui->textEdit->setText(m_dataset->query());
}


void SqlDatasetHelper::slotDbhostChanged(QString host)
{
    if (host != m_ui->hostField->text())
        m_ui->hostField->setText(host);
}


void SqlDatasetHelper::slotDbnameChanged(QString name)
{
    if (name != m_ui->databaseField->text())
        m_ui->databaseField->setText(name);
}


void SqlDatasetHelper::slotDbuserChanged(QString user)
{
    if (user != m_ui->userField->text())
        m_ui->userField->setText(user);
}


void SqlDatasetHelper::slotDbpasswdChanged(QString passwd)
{
    if (passwd != m_ui->passwordField->text())
        m_ui->passwordField->setText(passwd);
}


void SqlDatasetHelper::slotDriverChanged(QString driver)
{
    if (driver != m_ui->driver->text())
        m_ui->driver->setText(driver);
}


void SqlDatasetHelper::slotHostEdited()
{
    m_dataset->setdbhost(m_ui->hostField->text());
}


void SqlDatasetHelper::slotDatabaseEdited()
{
    m_dataset->setdbname(m_ui->databaseField->text());
}


void SqlDatasetHelper::slotUserEdited()
{
    m_dataset->setdbuser(m_ui->userField->text());
}


void SqlDatasetHelper::slotpasswordEdited()
{
    m_dataset->setdbpasswd(m_ui->passwordField->text());
}

void SqlDatasetHelper::slotDriverEdited()
{
    m_dataset->setDriver(m_ui->driver->text());
}


void SqlDatasetHelper::slotQueryEdited()
{
    m_dataset->setQuery( m_ui->textEdit->toPlainText());
}

void SqlDatasetHelper::slotCopyDriverName()
{
    m_dataset->setDriver(m_ui->driversList->currentText());
}


void SqlDatasetHelper::slotExtendedOptionsToggled(bool b)
{
    m_ui->frame->setVisible(b);
}


void SqlDatasetHelper::slotLoadFile()
{
    CuteReport::ReportCore * reportCore = m_dataset->reportCore();
    CuteReport::ReportInterface * report = static_cast<CuteReport::ReportInterface*> (m_dataset->parent());

    CuteReport::StdStorageDialog d(reportCore, report, window());
    d.setWindowTitle(tr("Open Database file"));
    if (!d.exec())
        return;

    QString resultUrl = d.currentObjectUrl();

    if (!resultUrl.isEmpty())
        m_dataset->setdbname(resultUrl);
}
