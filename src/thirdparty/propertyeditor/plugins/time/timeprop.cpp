/***************************************************************************
 *   This file is part of the propertyEditor project                       *
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *   bog_dan_ro@yahoo.com                                                  *
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

#include <QtCore>
#include <QMetaProperty>
#include <QMetaEnum>
#include <QTimeEdit>

#include "timeprop.h"

TimeProp::TimeProp(QObject* parent, QObject* object, int property, const PropertyModel* propertyModel): PropertyInterface(parent, object, property, propertyModel)
{
}

QWidget* TimeProp::createEditor(QWidget * parent, const QModelIndex & index)
{
	Q_UNUSED(index);
	QTimeEdit *te = new QTimeEdit(parent);
	te->setTime(value().toTime());
	connect(te, SIGNAL(timeChanged(const QTime&)), this, SLOT(setValue(const QTime&)));
	return te;
}

void TimeProp::setValue(const QTime& t)
{
	PropertyInterface::setValue(t);
}

QVariant TimeProp::data(const QModelIndex & index)
{
	if (!index.isValid() || !object() || -1 == objectProperty())
		return QVariant();
	switch (index.column())
	{
		case 0:
			return object()->metaObject()->property(objectProperty()).name();
		case 1:
			return value().toTime().toString("hh:mm:ss");
	}
	return QVariant();
}

bool TimeProp::setData(QVariant data, const QModelIndex & index)
{
	Q_UNUSED(index);
	return PropertyInterface::setValue(data);
}

void TimeProp::setEditorData(QWidget * editor, const QModelIndex & /*index*/)
{
	dynamic_cast<QTimeEdit *>(editor)->setTime(value().toTime());
}


bool TimeProp::canHandle(QObject * object, int property)const
{
	if (object->metaObject()->property(property).isEnumType() || object->metaObject()->property(property).isFlagType())
		return false;

	switch (object->property(object->metaObject()->property(property).name()).type())
	{
		case QVariant::Time:
			return true;
		default:
			return false;
	}
}

PropertyInterface* TimeProp::createInstance(QObject * object, int property, const PropertyModel * propertyModel) const
{
	return new TimeProp(parent(), object, property, propertyModel);
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(TimeProperty, TimeProp)
#endif
