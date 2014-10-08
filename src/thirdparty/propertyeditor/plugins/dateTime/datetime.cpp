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
#include <QDateTimeEdit>

#include "datetime.h"

DateTime::DateTime(QObject* parent, QObject* object, int property, const PropertyModel* propertyModel): PropertyInterface(parent, object, property, propertyModel)
{
}


QWidget* DateTime::createEditor(QWidget * parent, const QModelIndex & index)
{
	Q_UNUSED(index);
	QDateTimeEdit *de = new QDateTimeEdit(parent);
	de->setDateTime(value().toDateTime());
	connect(de, SIGNAL(dateTimeChanged(const QDateTime&)), this, SLOT(setValue(const QDateTime&)));
	return de;
}

void DateTime::setValue(const QDateTime& d)
{
	PropertyInterface::setValue(d);
}

QVariant DateTime::data(const QModelIndex & index)
{
	if (!index.isValid() || !object() || -1 == objectProperty())
		return QVariant();
	switch (index.column())
	{
		case 0:
			return object()->metaObject()->property(objectProperty()).name();
		case 1:
			return value().toDateTime().toString();
	}
	return QVariant();
}

bool DateTime::setData(QVariant data, const QModelIndex & index)
{
	Q_UNUSED(index);
	return PropertyInterface::setValue(data);
}

void DateTime::setEditorData(QWidget * editor, const QModelIndex & /*index*/)
{
	dynamic_cast<QDateTimeEdit *>(editor)->setDateTime(value().toDateTime());
}


bool DateTime::canHandle(QObject * object, int property)const
{
	if (object->metaObject()->property(property).isEnumType() || object->metaObject()->property(property).isFlagType())
		return false;

    switch (object->metaObject()->property(property).type())
	{
		case QVariant::DateTime:
			return true;
		default:
			return false;
	}
}

PropertyInterface* DateTime::createInstance(QObject * object, int property, const PropertyModel * propertyModel) const
{
	return new DateTime(parent(), object, property, propertyModel);
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(DateTimeProperty, DateTime)
#endif
