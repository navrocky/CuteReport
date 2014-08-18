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

#include "filesystemdatasetmodel.h"
#include "filesystemdataset.h"
#include <QStringList>
#include <QtGui>

FSModel::FSModel(QObject *parent)
    : QAbstractTableModel(parent), m_filters(QDir::Files | QDir::Hidden | QDir::NoSymLinks), m_pathAppearance(FileSystemDataset::NoPath),
      m_maxNumber(-1)
{
}


FSModel::~FSModel()
{
}


void FSModel::populate()
{
    m_data.clear();
    QDir::Filters filters = (QDir::Filters)m_filters | QDir::NoDot|QDir::NoDotDot;

    QDir dir;
    QList<QStringList> levels;
    levels.append( QStringList() << m_dir);
    int level;
    while (!levels.isEmpty()) {
        level = levels.size();
        dir.setPath(levels.last().takeFirst());
        QFileInfoList list;
        if (m_nameFilters.isEmpty())
            list = dir.entryInfoList(filters);
        else
            list = dir.entryInfoList(m_nameFilters, filters);

        if ( (m_maxNumber == -1) || (list.size() + m_data.size() < m_maxNumber))
            m_data += list;
        else
            m_data += list.mid(0, m_maxNumber - m_data.size());

        if (level < m_recursionLevel) {
            QStringList list = dir.entryList(QDir::AllDirs|QDir::NoDotAndDotDot);
            QStringList fullPathList;
            for (int i = 0; i<list.size(); ++i) {
                QString path =  dir.path() + "/" + list.at(i);
                //                path.replace("//","/");
                fullPathList << path;
            }
            if (fullPathList.size())
                levels.append(fullPathList);
        }

        while (levels.size() && levels.last().size() == 0)
            levels.removeLast();
    }

    for (int i = 0; i< MAXFIELD; ++i) {
        m_fields.insert(headerData(i, Qt::Horizontal).toString(),i);
    }

    beginResetModel();
    endResetModel();
}


void FSModel::setDir(const QString & dir)
{
    m_dir = dir;
}


void FSModel::setFilters(int filters)
{
    m_filters = filters;
}


void FSModel::setRecursionLevel(int recursionLevel)
{
    m_recursionLevel = recursionLevel;
}


void FSModel::setMaxNumber(int value)
{
    m_maxNumber = value;
}


void FSModel::setPathAppearance(int value)
{
    m_pathAppearance = value;
}


void FSModel::setNameFilters(const QStringList &nameFilters)
{
    m_nameFilters = nameFilters;
}


void FSModel::clear()
{
    m_data.clear();
    m_fields.clear();

    beginResetModel();
    endResetModel();
}


int FSModel::fieldIndex(const QString &name)
{
    return m_fields.value(name);
}


int FSModel::columnCount(const QModelIndex &/*parent*/) const
{
    return MAXFIELD;
}


int FSModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_data.size();
}


QVariant FSModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    int column = index.column();
    QFileInfo data = m_data[index.row()];

    if (column >= MAXFIELD)
        return QVariant();


    switch (column) {
    case FILENAME:  switch ( (FileSystemDataset::PathAppearance) m_pathAppearance) {
        case FileSystemDataset::NoPath:         return data.fileName();
        case FileSystemDataset::AbsolutePath:   return data.absoluteFilePath();
        case FileSystemDataset::RelativePath:   return data.absoluteFilePath().remove(m_dir).remove(0,1);
        }
        break;
    case TYPE:      return data.isDir() ? "dir" : "file";
    case FILESIZE:  return data.size();
    case CREATED:   return data.created();
    case MODIFIED:  return data.lastModified();
    case READ:      return data.lastRead();
    case OWNER:     return data.owner();
    case FORMAT:    return QImageReader::imageFormat(data.absoluteFilePath());
    }

    return QVariant();
}


Qt::ItemFlags FSModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


QVariant FSModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch(section) {
        case FILENAME:  return tr("Name");
        case TYPE:      return tr("Type");
        case FILESIZE:  return tr("Size");
        case CREATED:   return tr("Created");
        case MODIFIED:  return tr("Modified");
        case READ:      return tr("Read");
        case OWNER:     return tr("Owner");
        case FORMAT:    return tr("Format");
        }
    }

    return QVariant();
}

