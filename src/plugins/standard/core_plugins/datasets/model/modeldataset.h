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
#ifndef MODELDATASET_H
#define MODELDATASET_H

#include <datasetinterface.h>
#include <reportinterface.h>

#include "globals.h"

class ModelDatasetHelper;
class TestModel;
class ProxyModel;
class QAbstractItemModel;
class CloneModel;

class ModelDataset : public CuteReport::DatasetInterface
{
    Q_OBJECT

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteReport.DatasetInterface/1.0")
#endif
    Q_INTERFACES(CuteReport::DatasetInterface)

    Q_PROPERTY(QString sourceModelName READ sourceModelName WRITE setSourceModelName NOTIFY sourceModelNameChanged)
    Q_PROPERTY(QByteArray testModelData READ testModelData WRITE setTestModelData NOTIFY testModelDataChanged)

public:
    explicit ModelDataset(QObject *parent = 0);
    ~ModelDataset();

    void setSourceModelName(QString name);
    QString sourceModelName() const;

    QByteArray testModelData() const;
    void setTestModelData(QByteArray data);

    virtual QAbstractItemModel *model();

    QAbstractItemModel *sourceModel() const;
    void setSourceModel(QAbstractItemModel *model);

    TestModel *testModel() const;

signals:
    void sourceModelNameChanged(QString);
    void testModelDataChanged(QString);

private slots:


    // ReportPluginInterface interface
public:
    QString moduleShortName() const;
    QString suitName() const { return "Standard"; }

    CuteReport::DatasetHelperInterface *helper();
    QIcon icon();
    bool firstRow();
    bool lastRow();
    bool nextRow();
    bool previousRow();
    bool populate();
    bool isPopulated();
    void setPopulated(bool b);
    void reset();
    void resetCursor();
    int currentRow();
    bool setCurrentRow(int index);
    int rows();

    QString lastError();
    int columns();
    QVariant value(int index) const;
    QVariant value(const QString &field) const;
    QVariant lookaheadValue(int index) const;
    QVariant lookaheadValue(const QString &field) const;
    QVariant lookbackValue(int index) const;
    QVariant lookbackValue(const QString &field) const;
    QString fieldName(int column);
    QVariant::Type fieldType(int column);

protected:
    CuteReport::DatasetInterface *createInstance(QObject *parent) const;
    virtual DatasetInterface * objectClone() const;

private:
    ModelDatasetHelper *m_helper;
    QAbstractItemModel *m_sourceModel;
    TestModel *m_testModel;
    CloneModel *m_cloneModel;

    QString m_sourceModelName;
    QByteArray m_testModelData;

    int m_currentRow;
    bool m_isPopulated;
    qlonglong m_longPtr;


    //CuteReport::ReportInterface * m_report;
private:
    ModelDataset(const ModelDataset &dd, QObject * parent);

    int columnIndexByName(QString name) const;    
};

#endif // MODELDATASET_H
