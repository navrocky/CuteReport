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
 ***************************************************************************/
#ifndef QTSCRIPTSHELL_QABSTRACTITEMMODEL_H
#define QTSCRIPTSHELL_QABSTRACTITEMMODEL_H

#include <QAbstractItemModel>

#include <QtScript/qscriptvalue.h>

class QtScriptShell_QAbstractItemModel : public QAbstractItemModel
{
public:
    QtScriptShell_QAbstractItemModel(QObject*  parent = 0);
    ~QtScriptShell_QAbstractItemModel();

    QModelIndex  buddy(const QModelIndex&  index) const;
    bool  canFetchMore(const QModelIndex&  parent) const;
    void childEvent(QChildEvent*  arg__1);
    int  columnCount(const QModelIndex&  parent = QModelIndex()) const;
    void customEvent(QEvent*  arg__1);
    QVariant  data(const QModelIndex&  index, int  role = Qt::DisplayRole) const;
    bool  dropMimeData(const QMimeData*  data, Qt::DropAction  action, int  row, int  column, const QModelIndex&  parent);
    bool  event(QEvent*  arg__1);
    bool  eventFilter(QObject*  arg__1, QEvent*  arg__2);
    void fetchMore(const QModelIndex&  parent);
    Qt::ItemFlags  flags(const QModelIndex&  index) const;
    bool  hasChildren(const QModelIndex&  parent = QModelIndex()) const;
    QVariant  headerData(int  section, Qt::Orientation  orientation, int  role = Qt::DisplayRole) const;
    QModelIndex  index(int  row, int  column, const QModelIndex&  parent = QModelIndex()) const;
    bool  insertColumns(int  column, int  count, const QModelIndex&  parent = QModelIndex());
    bool  insertRows(int  row, int  count, const QModelIndex&  parent = QModelIndex());
    QMap<int , QVariant >  itemData(const QModelIndex&  index) const;
    QList<QModelIndex >  match(const QModelIndex&  start, int  role, const QVariant&  value, int  hits = 1, Qt::MatchFlags  flags = Qt::MatchFlags(Qt::MatchStartsWith|Qt::MatchWrap)) const;
    QMimeData*  mimeData(const QList<QModelIndex >&  indexes) const;
    QStringList  mimeTypes() const;
    QModelIndex  parent(const QModelIndex&  child) const;
    bool  removeColumns(int  column, int  count, const QModelIndex&  parent = QModelIndex());
    bool  removeRows(int  row, int  count, const QModelIndex&  parent = QModelIndex());
    void revert();
    int  rowCount(const QModelIndex&  parent = QModelIndex()) const;
    bool  setData(const QModelIndex&  index, const QVariant&  value, int  role = Qt::EditRole);
    bool  setHeaderData(int  section, Qt::Orientation  orientation, const QVariant&  value, int  role = Qt::EditRole);
    bool  setItemData(const QModelIndex&  index, const QMap<int , QVariant >&  roles);
    void sort(int  column, Qt::SortOrder  order = Qt::AscendingOrder);
    QSize  span(const QModelIndex&  index) const;
    bool  submit();
    Qt::DropActions  supportedDropActions() const;
    void timerEvent(QTimerEvent*  arg__1);

    QScriptValue __qtscript_self;
};

#endif // QTSCRIPTSHELL_QABSTRACTITEMMODEL_H
