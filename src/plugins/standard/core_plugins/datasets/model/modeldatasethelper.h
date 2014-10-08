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
#ifndef MODELDATASETHELPER_H
#define MODELDATASETHELPER_H

#include <QWidget>

#include "datasetinterface.h"

namespace Ui {
class ModelDatasetHelper;
}

class ModelDataset;
class TestModel;
class QStringListModel;
class ColumnTypeModel;
class ProxyModel;

class ModelDatasetHelper : public CuteReport::DatasetHelperInterface
{
    Q_OBJECT

public:
    explicit ModelDatasetHelper(ModelDataset *dataset);
    ~ModelDatasetHelper();

    virtual void save();
    virtual void load();

private slots:
    void addRow();
    void removeRow();
    void addColumn();
    void removeColumn();

    void onCmbTypeChanged();
    void scRemove();

private:
    Ui::ModelDatasetHelper *ui;
    ModelDataset *m_dataset;
    TestModel *m_model;
    ProxyModel *m_proxyModel;

    ColumnTypeModel *m_typeModel;

private:
    QVariant::Type currentColumnType() const;
    void setCurrentColumnType(QVariant::Type type);
    QModelIndex currentIndex() const;
    TestModel *model();
};

#endif // MODELDATASETHELPER_H
