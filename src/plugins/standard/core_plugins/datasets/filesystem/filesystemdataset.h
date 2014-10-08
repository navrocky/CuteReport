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
#ifndef FILESYSTEMDATASET_H
#define FILESYSTEMDATASET_H

#include <datasetinterface.h>
#include <QtSql>
#include <QSortFilterProxyModel>
#include <reportinterface.h>
#include "globals.h"


class FSModel;

class FileSystemDataset : public CuteReport::DatasetInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteReport.DatasetInterface/1.0")
#endif
    Q_INTERFACES(CuteReport::DatasetInterface)

    Q_FLAGS(Filters)
    Q_ENUMS(PathAppearance)

    Q_PROPERTY(QString directory READ dir WRITE setDir NOTIFY dirChanged)
    Q_PROPERTY(int recursionLevel READ recursionLevel WRITE setRecursionLevel NOTIFY recursionLevelChanged)
    Q_PROPERTY(int maxFiles READ maxNumber WRITE setMaxNumber NOTIFY maxNumberChanged)
    Q_PROPERTY(Filters filters READ filters WRITE setFilters NOTIFY filtersChanged)
    Q_PROPERTY(QStringList nameFilters READ nameFilters WRITE setNameFilters NOTIFY nameFiltersChanged)
    Q_PROPERTY(PathAppearance pathAppearance READ pathAppearance WRITE setPathAppearance NOTIFY pathAppearanceChanged)


public:
    enum Filter {
        Dirs          = 0x001,
        AllDirs	      = 0x400,
        Files	      = 0x002,
        Readable      = 0x010,
        Writable      = 0x020,
        Executable    = 0x040,
        Hidden	      = 0x100,
        System	      = 0x200,
        CaseSensitive = 0x800
    };
    Q_DECLARE_FLAGS(Filters, Filter)

    enum PathAppearance {NoPath, AbsolutePath, RelativePath};

    explicit FileSystemDataset(QObject *parent = 0);
    virtual ~FileSystemDataset();

    virtual QIcon icon();

    virtual DatasetInterface * createInstance(QObject* parent = 0) const;
    virtual CuteReport::DatasetHelperInterface * helper();
    virtual QAbstractItemModel * model();

    virtual QString lastError();

    virtual bool populate();
    virtual bool isPopulated();
    virtual void setPopulated(bool b);
    virtual void reset();
    virtual void resetCursor();
    virtual bool firstRow();
    virtual bool lastRow();
    virtual bool nextRow();
    virtual bool previousRow();
    virtual int  currentRow();
    virtual bool setCurrentRow(int index);
    virtual int rows();
    virtual int columns();
    virtual QVariant value(int index) const;
    virtual QVariant value(const QString & field) const;
    virtual QVariant lookaheadValue(int index) const;
    virtual QVariant lookaheadValue(const QString & field) const;
    virtual QVariant lookbackValue(int index) const;
    virtual QVariant lookbackValue(const QString & field) const;
    virtual QString fieldName(int column );
    virtual QVariant::Type fieldType(int column);

    virtual QString moduleShortName() const;
    virtual QString suitName() const { return "Standard"; }

    QString	dir() const;
    void setDir(const QString &str);

    Filters filters() const;
    void setFilters(Filters filters);

    int recursionLevel() const;
    void setRecursionLevel(int recursionLevel);

    int maxNumber() const;
    void setMaxNumber(int value);

    PathAppearance pathAppearance() const;
    void setPathAppearance(const PathAppearance &value);

    QStringList nameFilters() const;
    void setNameFilters(const QStringList &filters);

    virtual QSet<QString> variables() const;

signals:
    void dirChanged(QString);
    void recursionLevelChanged(int);
    void maxNumberChanged(int);
    void filtersChanged(FileSystemDataset::Filters);
    void pathAppearanceChanged(FileSystemDataset::PathAppearance);
    void nameFiltersChanged(const QStringList &);

private:
    explicit FileSystemDataset(const FileSystemDataset &dd, QObject * parent);
    virtual DatasetInterface * objectClone() const;

    Filters m_filters;
    int m_currentRow;
    QString m_dir;
    int m_recursionLevel;
    int m_maxNumber;
    bool m_isPopulated;
    PathAppearance m_pathAppearance;
    QStringList m_nameFilters;
    FSModel * m_model;
    QSortFilterProxyModel * m_fmodel;
    QPointer<CuteReport::DatasetHelperInterface> m_helper;

    QString m_lastError;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(FileSystemDataset::Filters)

#endif
