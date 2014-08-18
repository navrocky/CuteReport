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
#include "objectmodel.h"
#include "iteminterface.h"
#include "bandinterface.h"
#include "pageinterface.h"

ObjectModel::ObjectModel(QObject *parent)
		: QAbstractItemModel(parent),m_rootObject(0)
{

}

ObjectModel::~ObjectModel()
{
	deleter(m_rootObject);
}

void ObjectModel::deleter(ObjectStruct * os)
{
	if (!os)
		return;
	foreach(ObjectStruct * dos, os->children)
		deleter(dos);
	delete os;
}

ObjectModel::ObjectStruct * ObjectModel::addr(QObject * object)
{
	ObjectStruct * os = new ObjectStruct;
	os->object=object;
	foreach(QObject * ob, object->children())
        if (dynamic_cast<CuteReport::BaseItemInterface*>(ob) || dynamic_cast<CuteReport::PageInterface*>(ob) )
			os->children.push_back(addr(ob));
	return os;
}

void ObjectModel::setRootObject(QObject * rootObject)
{
    beginResetModel();

	deleter(m_rootObject);
    m_rootObject=rootObject ? addr(rootObject): 0;

    endResetModel();
}

QModelIndex ObjectModel::index ( int row, int column, const QModelIndex & parent ) const
{
	if (column)
		return QModelIndex();

	if (parent.isValid())
	{
		if (reinterpret_cast<ObjectStruct *>(parent.internalPointer())->children.size()>row)
		{
			reinterpret_cast<ObjectStruct *>(parent.internalPointer())->children[row]->parent=parent;
			return createIndex(row, column, reinterpret_cast<ObjectStruct *>(parent.internalPointer())->children[row]);
		}
	}
	else
		if (m_rootObject)
			return createIndex(row, column, m_rootObject);
	return QModelIndex();
}

QModelIndex ObjectModel::parent( const QModelIndex & index ) const
{
	if (!index.isValid())
		return QModelIndex();
	return reinterpret_cast<ObjectStruct *>(index.internalPointer())->parent;
}

int ObjectModel::rowCount ( const QModelIndex & parent ) const
{
	if (parent.isValid())
		return reinterpret_cast<ObjectStruct *>(parent.internalPointer())->children.size();
	//	return reinterpret_cast<ObjectStruct *>(parent.internalPointer())->object->children().size();
	return 1;
}

int ObjectModel::columnCount(const QModelIndex & /*parent*/) const
{
	return 1;
}

QVariant ObjectModel::data(const QModelIndex & index, int role) const
{
	QVariant ret;
	if (!index.isValid())
		return ret;
	switch(role)
	{
		case Qt::DisplayRole:
			ret=QString("%1 [%2]").arg(reinterpret_cast<ObjectStruct *>(index.internalPointer())->object->objectName()).arg(reinterpret_cast<ObjectStruct *>(index.internalPointer())->object->metaObject()->className ());
			break;
		default:
			break;
	}
	return ret;
}
