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
#include "csvdatasetmodel.h"
#include <QStringList>

Model::Model(QObject *parent)
    : QAbstractTableModel(parent), m_rows(0), m_columns(0)
{
}


Model::~Model()
{
}


void Model::setArray(Array array)
{
    m_array = array;
    m_rows = m_array.count();
    foreach (QStringList record, m_array)
        if (record.count() > m_columns)
            m_columns = record.count();

    beginResetModel();
    endResetModel();
}


void Model::setHeader(QStringList header)
{
    m_header = header;
}

void Model::clear()
{
    m_array.clear();
    m_rows = 0;
    m_columns = 0;

    beginResetModel();
    endResetModel();
}


int Model::columnCount(const QModelIndex &/*parent*/) const
{
    return m_columns;
}


int Model::rowCount(const QModelIndex &/*parent*/) const
{
    return m_rows;
}


QVariant Model::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    if (index.column() < m_columns)
        return m_array[index.row()][index.column()];
    else
        return QVariant();
}


Qt::ItemFlags Model::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section >= m_header.size())
            return  QVariant(QString::number(section));
        else
            return m_header[section];
    }

    return QVariant();
}

