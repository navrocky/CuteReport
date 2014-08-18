/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2012-2014 by Mikhalov Alexander                         *
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

#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include <QtCore>

typedef QList<QStringList>  Array;

class FSModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Fields {FILENAME, TYPE, FILESIZE, CREATED, MODIFIED, READ, OWNER, FORMAT, MAXFIELD};
    FSModel(QObject *parent = 0);
    ~FSModel();

    void setDir(const QString & dir);
    void setFilters(int filters);
    void setRecursionLevel(int recursionLevel);
    void setMaxNumber(int value);
    void setPathAppearance(int value);
    void setNameFilters(const  QStringList & nameFilters);

    void populate();
    void clear();

    int fieldIndex(const QString & name);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

private:
    QString m_dir;
    QFileInfoList m_data;
    int m_filters;
    int m_recursionLevel;
    int m_pathAppearance;
    QStringList m_nameFilters;
    int m_maxNumber;
    QHash<QString, int> m_fields;
};

#endif
