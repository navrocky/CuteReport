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
 ****************************************************************************/
#ifndef DATASETINTERFACE_H
#define DATASETINTERFACE_H

#include "globals.h"
#include "reportplugininterface.h"
#include <QWidget>
#include <QAbstractTableModel>
#include <QVariant>
#include <QSet>

class DatasetEditorInterface;

namespace CuteReport
{

class ReportInterface;
class DatasetHelperInterface;

class CUTEREPORT_EXPORTS DatasetInterface : public ReportPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(CuteReport::ReportPluginInterface)

    Q_PROPERTY(QString parentDataset READ parentDataset WRITE setParentDataset)
    Q_PROPERTY(QString filterCondition READ filterCondition WRITE setFilterCondition)
    Q_PROPERTY(int filterColumn READ filterColumn WRITE setFilterColumn)

public:
    DatasetInterface(QObject *parent = 0);
    virtual ~DatasetInterface() {}

    virtual void init(){}
    virtual DatasetHelperInterface * helper() = 0;
    virtual DatasetInterface * clone() const;
    Q_INVOKABLE virtual QAbstractItemModel * model();

    virtual QIcon icon() = 0;
    Q_INVOKABLE virtual QString lastError();

    Q_INVOKABLE virtual bool firstRow() = 0;
    Q_INVOKABLE virtual bool lastRow() = 0;
    Q_INVOKABLE virtual bool nextRow() = 0;
    Q_INVOKABLE virtual bool previousRow() = 0;
    Q_INVOKABLE virtual bool populate() = 0;
    Q_INVOKABLE virtual bool isPopulated() = 0;
    Q_INVOKABLE virtual void setPopulated(bool b) = 0;
    Q_INVOKABLE virtual void reset() = 0;
    Q_INVOKABLE virtual void resetCursor() = 0;
    Q_INVOKABLE virtual int currentRow() = 0;
    Q_INVOKABLE virtual bool setCurrentRow(int index) = 0;
    Q_INVOKABLE virtual int rows() = 0;
    Q_INVOKABLE virtual int columns() = 0;
    Q_INVOKABLE virtual QVariant value(int index) const;
    Q_INVOKABLE virtual QVariant value(const QString & field) const;
    Q_INVOKABLE virtual QVariant lookaheadValue(int index) const;
    Q_INVOKABLE virtual QVariant lookaheadValue(const QString & field) const;
    Q_INVOKABLE virtual QVariant lookbackValue(int index) const;
    Q_INVOKABLE virtual QVariant lookbackValue(const QString & field) const;
    Q_INVOKABLE virtual QString fieldName(int column );
    Q_INVOKABLE virtual QVariant::Type fieldType(int column ) = 0;
    Q_INVOKABLE virtual void setFilter ( const int col, const QString & str, Qt::CaseSensitivity cs = Qt::CaseSensitive );

    Q_INVOKABLE QString	parentDataset();
    Q_INVOKABLE void	setParentDataset(QString pDataset);
    Q_INVOKABLE QString	filterCondition();
    Q_INVOKABLE void	setFilterCondition(QString str);
    Q_INVOKABLE int		filterColumn();
    Q_INVOKABLE void	setFilterColumn(int col);

    virtual QSet<QString> variables() const {return QSet<QString>();}

signals:
    void beforeNext();
    void afterNext();
    void beforePrevious();
    void afterPrevious();
    void beforeFirst();
    void afterFirst();
    void beforeLast();
    void afterLast();
    void beforeSeek(int index);
    void afterSeek(int index);
    void beforePopulate();
    void afterPopulate();
    void beforeSetFilter(int col, QString str);
    void afterSetFilter(int col, QString str);
    void variablesChanged();

protected:
    virtual DatasetInterface * createInstance(QObject* parent = 0) const = 0;
    virtual DatasetInterface * objectClone() const = 0;
private:
    QString m_parentDataset;
    QString m_filterCondition;
    int m_filterColumn;

    friend class ReportCore;
};

class DatasetHelperInterface: public QWidget
{
public:
    virtual void save() {}
    virtual void load() {}
};


}
Q_DECLARE_METATYPE(CuteReport::DatasetInterface*)
Q_DECLARE_INTERFACE(CuteReport::DatasetInterface, "CuteReport.DatasetInterface/1.0")

#endif
