/***************************************************************************
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *   bogdan@licentia.eu                                                    *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/
#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

#include <QAbstractItemModel>

/**
	@author BogDan Vatra <taipan@omnidatagrup.ro>
*/

class ObjectModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	struct ObjectStruct
	{
		QObject * object;
		QModelIndex parent;
		QList<ObjectStruct *> children;
	} * m_rootObject;

public:
	ObjectModel(QObject *parent = 0);
	~ObjectModel();
	void setRootObject(QObject * rootObject);
	QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
	QModelIndex parent ( const QModelIndex & index ) const;
	int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
	int columnCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

private:
	void deleter(ObjectStruct * os);
	ObjectStruct * addr(QObject * object);
};

#endif
