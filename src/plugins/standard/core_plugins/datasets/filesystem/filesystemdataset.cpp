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
#include "filesystemdataset.h"
#include "filesystemdatasethelper.h"
#include "filesystemdatasetmodel.h"
#include "reportcore.h"
#include "functions.h"

#define DATASET_NAME "FileSystem"

using namespace CuteReport;

FileSystemDataset::FileSystemDataset(QObject *parent)
    : DatasetInterface(parent),
      m_filters(Dirs|AllDirs|Files|Readable|Writable|Executable|Hidden|System),
      m_currentRow(0),
      m_recursionLevel(0),
      m_maxNumber(-1),
      m_isPopulated(false),
      m_pathAppearance(NoPath)
{
    m_model = new FSModel (this);
    m_fmodel = new QSortFilterProxyModel(this);
    m_fmodel->setSourceModel(m_model);
}


FileSystemDataset::FileSystemDataset(const FileSystemDataset &dd, QObject * parent)
    : DatasetInterface(parent),
      m_filters(dd.m_filters),
      m_currentRow(0),
      m_recursionLevel(dd.m_recursionLevel),
      m_maxNumber(dd.m_maxNumber),
      m_isPopulated(false),
      m_pathAppearance(dd.m_pathAppearance)
{
    m_model = new FSModel (this);
    m_fmodel = new QSortFilterProxyModel(this);
    m_fmodel->setSourceModel(m_model);
    if (dd.m_isPopulated) {
        populate();
        setCurrentRow(dd.m_currentRow);
    }
}


FileSystemDataset::~FileSystemDataset()
{
}


QIcon FileSystemDataset::icon()
{
    QIcon icon(":/images/fs.jpeg");
    return icon;
}


DatasetInterface * FileSystemDataset::createInstance(QObject* parent) const
{
    return new FileSystemDataset(parent);
}


DatasetInterface *FileSystemDataset::objectClone() const
{
    return new FileSystemDataset(*this, parent());
}


DatasetHelperInterface * FileSystemDataset::helper()
{
    if (!m_helper)
        m_helper = new FileSystemDatasetHelper(this);

    return m_helper;
}


QString FileSystemDataset::moduleShortName() const
{
    return QString("FileSystem");
}


QString FileSystemDataset::lastError()
{
    return m_lastError;
}


QString	FileSystemDataset::dir() const
{
    return m_dir;
}


void FileSystemDataset::setDir(const QString &str)
{
    if (m_dir == str)
        return;

    QSet<QString> vars = variables();

    m_dir = str;
    if (m_dir.right(1) == "/")
        m_dir.remove(m_dir.length()-1, 1);

    emit dirChanged(m_dir);
    emit changed();

    if (vars != variables())
        variablesChanged();
}


FileSystemDataset::Filters FileSystemDataset::filters() const
{
    return m_filters;
}


void FileSystemDataset::setFilters(FileSystemDataset::Filters filters)
{
    if (m_filters == filters)
        return;

    QSet<QString> vars = variables();

    m_filters = filters;

    emit filtersChanged(m_filters);
    emit changed();

    if (vars != variables())
        variablesChanged();
}


QString FileSystemDataset::fieldName(int column )
{
    return m_model->headerData ( column, Qt::Horizontal).toString();
}


QVariant::Type FileSystemDataset::fieldType(int column)
{
    return QVariant::String;
}


int FileSystemDataset::recursionLevel() const
{
    return m_recursionLevel;
}


void FileSystemDataset::setRecursionLevel(int recursionLevel)
{
    if (m_recursionLevel == recursionLevel)
        return;

    m_recursionLevel = recursionLevel;

    emit recursionLevelChanged(m_recursionLevel);
    emit changed();
}


int FileSystemDataset::maxNumber() const
{
    return m_maxNumber;
}


void FileSystemDataset::setMaxNumber(int value)
{
    if (m_maxNumber == value)
        return;

    m_maxNumber = value;

    emit maxNumberChanged(m_maxNumber);
    emit changed();
}


FileSystemDataset::PathAppearance FileSystemDataset::pathAppearance() const
{
    return m_pathAppearance;
}


void FileSystemDataset::setPathAppearance(const PathAppearance &value)
{
    if (m_pathAppearance == value)
        return;

    m_pathAppearance = value;

    emit pathAppearanceChanged(m_pathAppearance);
    emit changed();
}


QStringList FileSystemDataset::nameFilters() const
{
    return m_nameFilters;
}


void FileSystemDataset::setNameFilters(const QStringList &filters)
{
    if (m_nameFilters == filters)
        return;

    m_nameFilters = filters;

    emit nameFiltersChanged(m_nameFilters);
    emit changed();
}


QSet<QString> FileSystemDataset::variables() const
{
    QSet<QString> set;

    set.unite( findVariables(m_dir) );
    foreach (const QString & filter, m_nameFilters)
        set.unite(findVariables(filter));

    return set;
}


QAbstractItemModel * FileSystemDataset::model()
{
    return m_fmodel;
}


bool FileSystemDataset::populate()
{
    emit beforePopulate();

    CuteReport::ReportInterface * report = dynamic_cast<CuteReport::ReportInterface*>(parent());

    if (report) {
        QStringList missedVariables;
        if (!isStringValued(m_dir, report->variables(), &missedVariables)) {
            m_lastError = QString("Variable is not defined in \'directory\' property: %1").arg(missedVariables.join(", "));
            return false;
        }
        m_model->setDir(setVariablesValue(m_dir, report->variables()));

        QStringList nameFilters;
        foreach (const QString & filter, m_nameFilters) {
            if (!isStringValued(filter, report->variables(), &missedVariables)) {
                m_lastError = QString("Variable is not defined in \'nameFilters\' property: %1").arg(missedVariables.join(", "));
                return false;
            }
            nameFilters << setVariablesValue(filter, report->variables());
        }
        m_model->setNameFilters(nameFilters);
    } else {
        m_model->setDir(m_dir);
        m_model->setNameFilters(m_nameFilters);
    }

    m_model->setFilters(m_filters);
    m_model->setPathAppearance(m_pathAppearance);
    m_model->setRecursionLevel(m_recursionLevel);
    m_model->setMaxNumber(m_maxNumber);

    m_model->populate();

    m_isPopulated = true;
    m_currentRow = -1;

    emit afterPopulate();
    return true;
}


bool FileSystemDataset::isPopulated()
{
    return m_isPopulated;
}


void FileSystemDataset::setPopulated(bool b)
{
    m_isPopulated = b;
}


void FileSystemDataset::reset()
{
    m_isPopulated = false;
    m_model->clear();
    m_currentRow = -1;
    m_lastError = "";
}


void FileSystemDataset::resetCursor()
{
    m_currentRow = -1;
}


bool FileSystemDataset::firstRow()
{
    emit(beforeFirst());
    m_currentRow = 0;
    bool ret = rows();
    emit(afterFirst());
    return ret;
}


bool FileSystemDataset::lastRow()
{
    emit(beforeLast());
    m_currentRow = m_fmodel->rowCount();
    bool ret = m_currentRow < m_fmodel->rowCount() ? true:false;
    emit(afterLast());
    return ret;
}


bool FileSystemDataset::nextRow()
{
    emit(beforeNext());
    m_currentRow++;
    bool ret = m_currentRow < rows();
    emit(afterNext());
    return ret;
}


bool FileSystemDataset::previousRow()
{
    emit(beforePrevious());
    m_currentRow--;
    bool ret = m_currentRow >= 0;
    emit(afterPrevious());
    return ret;
}


int FileSystemDataset::currentRow()
{
    return m_currentRow;
}


bool FileSystemDataset::setCurrentRow(int index)
{
    emit(beforeSeek(index));
    m_currentRow = index;
    bool ret = (m_currentRow >=0 && m_currentRow < m_fmodel->rowCount() ? true:false);
    emit(afterSeek(index));
    return ret;
}


int FileSystemDataset::rows()
{
    return m_fmodel->rowCount();
}


int FileSystemDataset::columns()
{
    if (!m_isPopulated)
        populate();
    return m_fmodel->columnCount();
}


QVariant FileSystemDataset::value(int index) const
{
    return m_fmodel->data( m_fmodel->index(m_currentRow,index) );
}


QVariant FileSystemDataset::value(const QString & field) const
{
    return m_fmodel->data( m_fmodel->index(m_currentRow, m_model->fieldIndex(field) ) );
}


QVariant FileSystemDataset::lookaheadValue(int index) const
{
    return m_currentRow+1 < m_fmodel->rowCount() && index < m_fmodel->columnCount()
            ? m_fmodel->data( m_fmodel->index(m_currentRow + 1, index ) )
            : QVariant(QVariant::Invalid);
}


QVariant FileSystemDataset::lookaheadValue(const QString & field) const
{
    return lookaheadValue(m_model->fieldIndex(field));
}


QVariant FileSystemDataset::lookbackValue(int index) const
{
    return m_currentRow-1 >= 0 && index < m_fmodel->columnCount()
            ? m_fmodel->data( m_fmodel->index(m_currentRow - 1, index ) )
            : QVariant(QVariant::Invalid);
}


QVariant FileSystemDataset::lookbackValue(const QString & field) const
{
    return lookbackValue(m_model->fieldIndex(field));
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(DatasetFileSystem, FileSystemDataset)
#endif
