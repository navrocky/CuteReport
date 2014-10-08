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
#include "columntypemodel.h"

ColumnTypeModel::ColumnTypeModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_items.append(QVariant::String);
    m_items.append(QVariant::Int);
    m_items.append(QVariant::Double);
    m_items.append(QVariant::DateTime);
    m_items.append(QVariant::Bool);

    m_itemById.insert(QVariant::String, tr("String"));
    m_itemById.insert(QVariant::Int, tr("Integer"));
    m_itemById.insert(QVariant::Double, tr("Double"));
    m_itemById.insert(QVariant::DateTime, tr("DateTime"));
    m_itemById.insert(QVariant::Bool, tr("Boolean"));
}

QVariant::Type ColumnTypeModel::type(int row)
{
    if (row == -1 || row > rowCount() - 1) {
        return QVariant::Invalid;
    }

    return m_items.at(row);
}


QModelIndex ColumnTypeModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return createIndex(row, column);
}

QModelIndex ColumnTypeModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int ColumnTypeModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_items.count();
}

int ColumnTypeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

QVariant ColumnTypeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }


    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch(index.column()) {
        case IdCol : return m_items.at(index.row());
        case NameCol : return m_itemById.value(m_items.at(index.row()));
        }
    }

    return QVariant();
}
