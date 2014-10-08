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
#include "clonemodel.h"

CloneModel::CloneModel(QObject *parent) :
    QAbstractItemModel(parent),
    m_columnCount(0),
    m_populated(false)
{
}

void CloneModel::populate(QAbstractItemModel *sourceModel)
{
    m_populated = false;
    m_columnCount = sourceModel->columnCount();

    emit beginResetModel();
    m_items.clear();

    for (int row = 0; row < sourceModel->rowCount(); row++) {
        QList<QVariant> cols;
        for (int col = 0; col < sourceModel->columnCount(); col++) {
            cols.append(sourceModel->index(row, col).data(Qt::DisplayRole));
        }

        m_items.append(cols);
    }

    m_populated = true;
    emit endResetModel();
}


QModelIndex CloneModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return createIndex(row, column);
}

QModelIndex CloneModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int CloneModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_items.count();
}

int CloneModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_columnCount;
}

QVariant CloneModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant(QVariant::Invalid);
    }

    if (role == Qt::DisplayRole) {
        return m_items.at(index.row()).at(index.column());
    }

    return QVariant();
}
