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
#include "modeldatasethelper.h"
#include "ui_modeldatasethelper.h"
#include "modeldataset.h"
#include "columntypemodel.h"
#include "testmodel.h"
#include "proxymodel.h"

#include <QStringListModel>
#include <QShortcut>

ModelDatasetHelper::ModelDatasetHelper(ModelDataset *dataset) :
    CuteReport::DatasetHelperInterface(),
    ui(new Ui::ModelDatasetHelper),
    m_dataset(dataset),
    m_proxyModel(new ProxyModel(this))
{
    ui->setupUi(this);

    m_typeModel = new ColumnTypeModel(this);
    ui->cmbCoulmnType->setModel(m_typeModel);
    ui->cmbCoulmnType->setModelColumn(ColumnTypeModel::NameCol);

    ui->labType->setVisible(false);
    ui->cmbCoulmnType->setVisible(false);

    m_proxyModel->setSourceModel(m_dataset->sourceModel());
    ui->tableView->setModel(m_proxyModel);

    load();

    connect(ui->cmbCoulmnType, SIGNAL(currentIndexChanged(int)), SLOT(onCmbTypeChanged()));
    connect(ui->btnAddRow, SIGNAL(clicked()), SLOT(addRow()));
    connect(ui->btnDeleteRow, SIGNAL(clicked()), SLOT(removeRow()));
    connect(ui->btnAddColumn, SIGNAL(clicked()), SLOT(addColumn()));
    connect(ui->btnDeleteColumn, SIGNAL(clicked()), SLOT(removeColumn()));

    QShortcut *scRemove = new QShortcut(this);
    scRemove->setKey(QKeySequence::Delete);
    connect(scRemove, SIGNAL(activated()), SLOT(scRemove()));
}

ModelDatasetHelper::~ModelDatasetHelper()
{
    delete ui;
}

void ModelDatasetHelper::save()
{
    m_dataset->setSourceModelName(ui->edModelName->text());
    m_dataset->setTestModelData(m_dataset->testModel()->save());
}

void ModelDatasetHelper::load()
{
    ui->edModelName->setText(m_dataset->sourceModelName());
    m_dataset->testModel()->load(m_dataset->testModelData());
}

void ModelDatasetHelper::addRow()
{
    int row = currentIndex().isValid()
            ? currentIndex().row()
            : 0;
    m_dataset->testModel()->insertRow(row);
}

void ModelDatasetHelper::removeRow()
{
    if (!currentIndex().isValid()) {
        return;
    }

    m_dataset->testModel()->removeRow(currentIndex().row());
}

void ModelDatasetHelper::addColumn()
{
    int col = currentIndex().isValid()
            ? currentIndex().column()
            : 0;

    m_dataset->testModel()->insertColumn(col);
}

void ModelDatasetHelper::removeColumn()
{
    if (!currentIndex().isValid()) {
        return;
    }

    m_dataset->testModel()->removeColumn(currentIndex().column());
}

void ModelDatasetHelper::onCmbTypeChanged()
{
}

void ModelDatasetHelper::scRemove()
{
    QModelIndexList cols = ui->tableView->horizontalHeader()->selectionModel()->selectedColumns();
    if (cols.count() > 0) {
        QListIterator<QModelIndex> iter(cols);
        while (iter.hasNext()) {
            QModelIndex col = iter.next();
            ui->tableView->model()->removeColumn(col.column());
        }
    }

    QModelIndexList rows = ui->tableView->verticalHeader()->selectionModel()->selectedRows();
    if (rows.count() > 0) {
        QListIterator<QModelIndex> iter(rows);
        while (iter.hasNext()) {
            QModelIndex row = iter.next();
            ui->tableView->model()->removeRow(row.row());
        }
    }

    QModelIndexList idxs = ui->tableView->selectionModel()->selectedIndexes();
    if (idxs.count() > 0) {
        QListIterator<QModelIndex> iter(idxs);
        while (iter.hasNext()) {
            QModelIndex index = iter.next();
            ui->tableView->model()->setData(index, QString());
        }
    }
}

QVariant::Type ModelDatasetHelper::currentColumnType() const
{
    return m_typeModel->type(ui->cmbCoulmnType->currentIndex());
}

void ModelDatasetHelper::setCurrentColumnType(QVariant::Type type)
{
    for (int i = 0; i < m_typeModel->rowCount(); i++) {
        if (m_typeModel->type(i) == type) {
            ui->cmbCoulmnType->setCurrentIndex(i);
            break;
        }
    }
}

QModelIndex ModelDatasetHelper::currentIndex() const
{
    return ui->tableView->currentIndex();
}

TestModel *ModelDatasetHelper::model()
{
    return m_dataset->testModel();
}
