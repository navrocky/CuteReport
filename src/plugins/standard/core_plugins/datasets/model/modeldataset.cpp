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
#include "modeldataset.h"
#include "modeldatasethelper.h"
#include "testmodel.h"
#include "proxymodel.h"
#include "clonemodel.h"

#include <QIcon>

ModelDataset::ModelDataset(QObject *parent) :
    CuteReport::DatasetInterface(parent),
    m_helper(0),
    m_sourceModel(0),
    m_testModel(new TestModel(this)),    
    m_sourceModelName(QString("model1")),
    m_currentRow(0),
    m_isPopulated(false)
{    
}

ModelDataset::ModelDataset(const ModelDataset &dd, QObject *parent) :
    CuteReport::DatasetInterface(parent),
    m_helper(0),
    m_sourceModel(0),
    m_testModel(new TestModel(this)),
    m_currentRow(0),
    m_isPopulated(false)
{
    m_sourceModelName = dd.m_sourceModelName;
    m_testModelData = dd.m_testModelData;

    if (dd.m_isPopulated) {
        populate();
        setCurrentRow(dd.m_currentRow);
    }
}


ModelDataset::~ModelDataset()
{
}

void ModelDataset::setSourceModelName(QString name)
{
    m_sourceModelName = name;
}

QString ModelDataset::sourceModelName() const
{
    return m_sourceModelName;
}

QByteArray ModelDataset::testModelData() const
{
    return m_testModelData;
}

void ModelDataset::setTestModelData(QByteArray data)
{
    m_testModelData = data;
}

QAbstractItemModel *ModelDataset::model()
{
    return sourceModel();
}

QAbstractItemModel *ModelDataset::sourceModel() const
{
    return !m_sourceModel ? m_testModel : m_sourceModel;
}

void ModelDataset::setSourceModel(QAbstractItemModel *model)
{
    m_sourceModel = model;
}

TestModel *ModelDataset::testModel() const
{
    return m_testModel;
}

CuteReport::DatasetInterface *ModelDataset::objectClone() const
{
    return new ModelDataset(*this, parent());
}

QString ModelDataset::moduleShortName() const
{
    return QString("Model");
}

CuteReport::DatasetHelperInterface *ModelDataset::helper()
{
    if (!m_helper) {
        m_helper = new ModelDatasetHelper(this);
    }

    return m_helper;
}

QIcon ModelDataset::icon()
{
    return QIcon(":/images/model.png");
}

bool ModelDataset::firstRow()
{
    emit(beforeFirst());
    m_currentRow = 0;
    bool ret = rows();
    emit(afterFirst());
    return ret;
}

bool ModelDataset::lastRow()
{
    emit(beforeLast());
    m_currentRow = sourceModel()->rowCount();
    bool ret = m_currentRow < sourceModel()->rowCount() ? true:false;
    emit(afterLast());
    return ret;
}

bool ModelDataset::nextRow()
{
    emit(beforeNext());
    m_currentRow++;
    bool ret = m_currentRow < rows();
    emit(afterNext());
    return ret;
}

bool ModelDataset::previousRow()
{
    emit(beforePrevious());
    m_currentRow--;
    bool ret = m_currentRow >= 0;
    emit(afterPrevious());
    return ret;
}

bool ModelDataset::populate()
{
    emit beforePopulate();

    CuteReport::ReportInterface * report = dynamic_cast<CuteReport::ReportInterface*>(parent());

    if (report->variables().contains(sourceModelName())) {
        QVariant var = report->variables().value(sourceModelName());

        if (var.type() == QVariant::LongLong) {

            QAbstractItemModel *model = (QAbstractItemModel*)(var.toLongLong());
            if (model) {
                m_cloneModel = new CloneModel(this);
                m_cloneModel->populate(model);

                m_sourceModel = m_cloneModel;
            } else {
                qWarning() << "Variable '" << sourceModelName() << "contains, type long, but can't cast to QAbstractItemModel";
            }
        }
    }

    if (!m_sourceModel) {
        m_testModel->load(testModelData());
    }

    m_isPopulated = true;
    m_currentRow = -1;

    emit afterPopulate();
    return true;
}

bool ModelDataset::isPopulated()
{
    return m_isPopulated;
}

void ModelDataset::setPopulated(bool b)
{
    m_isPopulated = b;
}

void ModelDataset::reset()
{
    m_isPopulated = false;
    m_testModel->clear();
    m_currentRow = -1;
}

void ModelDataset::resetCursor()
{
    m_currentRow = -1;
}

int ModelDataset::currentRow()
{
    return m_currentRow;
}

bool ModelDataset::setCurrentRow(int index)
{
    emit(beforeSeek(index));
    m_currentRow = index;
    bool ret = (m_currentRow >=0 && m_currentRow < sourceModel()->rowCount() ? true:false);
    emit(afterSeek(index));
    return ret;
}

int ModelDataset::rows()
{
    return sourceModel()->rowCount();
}

CuteReport::DatasetInterface *ModelDataset::createInstance(QObject *parent) const
{
    return new ModelDataset(parent);
}


int ModelDataset::columnIndexByName(QString name) const
{
    int col = -1;
    QString tmp = name;
    if (tmp.contains("field", Qt::CaseInsensitive)) {
        tmp.remove("field", Qt::CaseInsensitive);

        bool ok = false;
        col = tmp.toInt(&ok);

        if (!ok) {
            col = -1;
            qWarning() << name << "is incorrect field name";
        }
    }
    return col - 1;
}

QString ModelDataset::lastError()
{
    return QString();
}

int ModelDataset::columns()
{
    return sourceModel()->columnCount();
}

QVariant ModelDataset::value(int index) const
{
    if (m_currentRow == -1) {
        return QVariant(QVariant::Invalid);
    }

    if (index == -1 || index > sourceModel()->columnCount() - 1) {
        return QVariant(QVariant::Invalid);
    }

    return sourceModel()->index(m_currentRow, index).data(Qt::DisplayRole);
}

QVariant ModelDataset::value(const QString &field) const
{
    int col = columnIndexByName(field);
    if (col == -1) {
        return QVariant::Invalid;
    }

    return value(col);
}

QVariant ModelDataset::lookaheadValue(int index) const
{
    return m_currentRow + 1 < sourceModel()->rowCount() && index < sourceModel()->columnCount()
            ? sourceModel()->index(m_currentRow + 1, index).data()
            : QVariant(QVariant::Invalid);
}

QVariant ModelDataset::lookaheadValue(const QString &field) const
{
    return lookaheadValue(columnIndexByName(field));
}

QVariant ModelDataset::lookbackValue(int index) const
{
    return m_currentRow-1 >= 0 && index < sourceModel()->columnCount()
            ? sourceModel()->index(m_currentRow, index).data(Qt::DisplayRole)
            : QVariant(QVariant::Invalid);
}

QVariant ModelDataset::lookbackValue(const QString &field) const
{
    return lookbackValue(columnIndexByName(field));
}

QString ModelDataset::fieldName(int column)
{
    Q_UNUSED(column)
    return QString();
}

QVariant::Type ModelDataset::fieldType(int column)
{
    Q_UNUSED(column)
    return QVariant::String;
}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(DatasetModel, ModelDataset)
#endif



