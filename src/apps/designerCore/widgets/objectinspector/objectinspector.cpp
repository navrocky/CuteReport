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
#include "objectinspector.h"
#include "objectmodel.h"
#include "QDebug"

ObjectInspector::ObjectInspector(QWidget *parent) :
    QTreeView(parent)
{
	m_objectModel = new ObjectModel(this);
	setModel( m_objectModel );

	setHeaderHidden( true );
    setSelectionMode( QAbstractItemView::ExtendedSelection );

	m_selectionModel = new QItemSelectionModel( m_objectModel );
	setSelectionModel( m_selectionModel );

    connect( m_selectionModel, SIGNAL( currentChanged( const QModelIndex & , const QModelIndex & ) ), SLOT( slotObjectChanged( const QModelIndex & , const QModelIndex & ) ) );
    connect( m_selectionModel, SIGNAL( selectionChanged(QItemSelection,QItemSelection) ), this, SLOT(slotSelectionChanged(QItemSelection,QItemSelection)));
}

ObjectInspector::~ObjectInspector()
{
}


void ObjectInspector::setRootObject( QObject * rootObject)
{
    this->blockSignals(true);
    m_objectModel->setRootObject(rootObject);
    this->blockSignals(false);
}


void  ObjectInspector::selectObject( QObject * object, QItemSelectionModel::SelectionFlag selFlag)
{
	selectObject(object, m_objectModel->index( 0, 0 ), selFlag );
}


bool ObjectInspector::selectObject( QObject * object, QModelIndex index, QItemSelectionModel::SelectionFlag selFlag)
{
	if (!object)
		return false;

	if ( index.isValid() && reinterpret_cast<ObjectModel::ObjectStruct *>( index.internalPointer() )->object == object )
	{
		this->selectionModel()->setCurrentIndex( index, selFlag );
		return true;
	}

	for ( int i = 0; i < m_objectModel->rowCount( index );i++ )	{
		if ( selectObject( object, m_objectModel->index( i, 0, index ), selFlag ) )
			return true;
	}
	return false;
}


void ObjectInspector::slotObjectChanged( const QModelIndex & index, const QModelIndex & )
{
    if (index.isValid()) {
        ObjectModel::ObjectStruct * object = reinterpret_cast<ObjectModel::ObjectStruct *>( index.internalPointer() );
        emit objectChanged(object->object);
    }
}


void ObjectInspector::slotSelectionChanged(QItemSelection,QItemSelection)
{
    emit selectionChanged();
}


QObjectList ObjectInspector::selectedObjects()
{
    QObjectList list;
    foreach (const QModelIndex & index, m_selectionModel->selectedIndexes())
        if ( index.isValid() ) {
            ObjectModel::ObjectStruct * objStruct = reinterpret_cast<ObjectModel::ObjectStruct *>( index.internalPointer() );
            list.append(objStruct->object);
        }
    return list;
}
