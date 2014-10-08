/***************************************************************************
 *   This file is part of the CuteReport project                           *
 *   Copyright (C) 2013 by Alexander Mikhalov                              *
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
#include "QtCore"
#include "csvdataset.h"
#include "csvdatasethelper.h"
#include "csvdatasetmodel.h"
#include "reportcore.h"
#include "functions.h"

#define DATASET_NAME "CSV"

using namespace CuteReport;

CsvDataset::CsvDataset(QObject *parent)
    : DatasetInterface(parent),
      m_firstRowIsHeader(false),
      m_currentRow(0),
      m_isPopulated(false),
      m_delimeter(","),
      m_keepData(false)
{
    m_model = new Model (this);
    m_fmodel = new QSortFilterProxyModel(this);
    m_fmodel->setSourceModel(m_model);
}


CsvDataset::CsvDataset(const CsvDataset &dd, QObject * parent)
    : DatasetInterface(parent),
      m_firstRowIsHeader(dd.m_firstRowIsHeader),
      m_currentRow(0),
      m_isPopulated(false),
      m_delimeter(dd.m_delimeter),
      m_keepData(dd.m_keepData)
{
    m_model = new Model (this);
    m_fmodel = new QSortFilterProxyModel(this);
    m_fmodel->setSourceModel(m_model);
    if (dd.m_isPopulated) {
        populate();
        setCurrentRow(dd.m_currentRow);
    }
}


CsvDataset::~CsvDataset()
{
}


QIcon CsvDataset::icon()
{
    QIcon icon(":/images/csv.jpeg");
    return icon;
}


DatasetInterface * CsvDataset::createInstance(QObject* parent) const
{
    return new CsvDataset(parent);
}


DatasetHelperInterface * CsvDataset::helper()
{
    if (!m_helper)
        m_helper = new CsvDatasetHelper(this);

    return m_helper;
}


DatasetInterface *CsvDataset::objectClone() const
{
    return new CsvDataset(*this, parent());
}


QString CsvDataset::moduleShortName() const
{
    return QString("CSV");
}


QSet<QString> CsvDataset::variables() const
{
    QSet<QString> set;

    set.unite( findVariables(m_fileName) );

    return set;
}


QString CsvDataset::lastError()
{
    return m_lastError;
}


QString	CsvDataset::fileName() const
{
    return m_fileName;
}


void CsvDataset::setFileName(const QString &str)
{
    if (m_fileName == str)
        return;

    m_fileName = str;

    emit fileNameChanged(m_fileName);
    emit changed();
}


QStringList CsvDataset::list() const
{
    return m_list;
}


void  CsvDataset::setList(const QStringList & list)
{
    m_list = list;
}


QString CsvDataset::delimeter () const
{
    return m_delimeter;
}


void CsvDataset::setDelimeter (const QString &str)
{
    if (m_delimeter == str)
        return;

    m_delimeter = str;

    emit delimiterChanged(m_delimeter);
    emit changed();
}


bool CsvDataset::keepData() const
{
    return m_keepData;
}


void CsvDataset::setKeepData(bool value)
{
    if (m_keepData == value)
        return;

    m_keepData = value;

    emit keepDataInternalChanged(m_keepData);
    emit changed();
}


bool CsvDataset::firstRowIsHeader()
{
    return m_firstRowIsHeader;
}


void CsvDataset::setFirstRowIsHeader(bool value)
{
    if (m_firstRowIsHeader == value)
        return;

    m_firstRowIsHeader = value;

    emit firstRowIsHeaderChanged(m_firstRowIsHeader);
    emit changed();
}


QString CsvDataset::fieldName(int column )
{
    return m_model->headerData ( column, Qt::Horizontal).toString();
}


QVariant::Type CsvDataset::fieldType(int column)
{
    return QVariant::String;
}


QAbstractItemModel * CsvDataset::model()
{
    return m_fmodel;
}


bool CsvDataset::populate()
{
    emit beforePopulate();

    QStringList list;
    if (m_keepData) {
        list = m_list;
    } else {
        if (m_fileName.isEmpty()) {
            m_lastError = tr("filename is empty. Please enter filename first");
            return false;
        }

        CuteReport::ReportInterface * report = static_cast<CuteReport::ReportInterface*> (parent());

        QStringList missedVariables;
        if (!isStringValued(m_fileName, report->variables(), &missedVariables)) {
            m_lastError = QString("Variable is not defined in \'directory\' property: %1").arg(missedVariables.join(", "));
            return false;
        }

        QString fileName = report ? setVariablesValue(m_fileName, report->variables()) : m_fileName;
        fileName = reportCore()->localCachedFileName(fileName, report);

        QFile data(fileName);
        if (data.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream in(&data);
            do {
                list.append(in.readLine());
            } while (!in.atEnd() );
        } else {
            m_lastError = tr("Can't open filename %1").arg(fileName);
            return false;
        }
    }

    Array array;

    m_model->setHeader((m_firstRowIsHeader && list.size()) ? list[0].split(m_delimeter) : QStringList());

    for (int i = (m_firstRowIsHeader ? 1:0); i<list.count(); i++) {
        array.append(list.at(i).split(m_delimeter));
    }
    m_model->setArray(array);

    m_isPopulated = true;
    m_currentRow = -1;

    emit afterPopulate();
    return true;
}


bool CsvDataset::isPopulated()
{
    return m_isPopulated;
}


void CsvDataset::setPopulated(bool b)
{
    m_isPopulated = b;
}


void CsvDataset::reset()
{
    m_isPopulated = false;
    m_model->clear();
    m_currentRow = -1;
    m_lastError = "";
}


void CsvDataset::resetCursor()
{
     m_currentRow = -1;
}


bool CsvDataset::firstRow()
{
    emit(beforeFirst());
    m_currentRow = 0;
    bool ret = rows();
    emit(afterFirst());
    return ret;
}


bool CsvDataset::lastRow()
{
    emit(beforeLast());
    m_currentRow = m_fmodel->rowCount();
    bool ret = m_currentRow < m_fmodel->rowCount() ? true:false;
    emit(afterLast());
    return ret;
}


bool CsvDataset::nextRow()
{
    emit(beforeNext());
    m_currentRow++;
    bool ret = m_currentRow < rows();
    emit(afterNext());
    return ret;
}


bool CsvDataset::previousRow()
{
    emit(beforePrevious());
    m_currentRow--;
    bool ret = m_currentRow >= 0;
    emit(afterPrevious());
    return ret;
}


int CsvDataset::currentRow()
{
    return m_currentRow;
}


bool CsvDataset::setCurrentRow(int index)
{
    emit(beforeSeek(index));
    m_currentRow = index;
    bool ret = (m_currentRow >=0 && m_currentRow < m_fmodel->rowCount() ? true:false);
    emit(afterSeek(index));
    return ret;
}


int CsvDataset::rows()
{
    return m_fmodel->rowCount();
}


int CsvDataset::columns()
{
    if (!m_isPopulated)
        populate();
    return m_fmodel->columnCount();
}


QVariant CsvDataset::value(int index) const
{
    return m_fmodel->data( m_fmodel->index(m_currentRow,index) );
}


QVariant CsvDataset::value(const QString & /*field*/) const
{
    return QVariant();
    //    return m_model.record(m_currentRow).value(field);
}


QVariant CsvDataset::lookaheadValue(int /*index*/) const
{
    return QVariant();
    //    return m_currentRow+1 < m_model.rowCount() && index < m_model.columnCount() ?  m_model.record(m_currentRow + 1).value(index) : QVariant::Invalid;
}


QVariant CsvDataset::lookaheadValue(const QString & /*field*/) const
{
    return QVariant();
    //    return m_currentRow+1 < m_model.rowCount() ?  m_model.record(m_currentRow + 1).value(field) : QVariant::Invalid;
}


QVariant CsvDataset::lookbackValue(int /*index*/) const
{
    return QVariant();
    //    return m_currentRow-1 < 0 && index < m_model.columnCount() ?  m_model.record(m_currentRow + 1).value(index) : QVariant::Invalid;
}


QVariant CsvDataset::lookbackValue(const QString & /*field*/) const
{
    return QVariant();
    //    return m_currentRow-1 < 0  ?  m_model.record(m_currentRow + 1).value(field) : QVariant::Invalid;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(DatasetCSV, CsvDataset)
#endif
