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

#include <QPushButton>
#include <QFrame>
#include "bool.h"

//inline void initMyResource()
//{
//	Q_INIT_RESOURCE(resource);
//}

Bool::Bool(QObject* parent, QObject* object, int property, const PropertyModel * propertyModel): PropertyInterface(parent, object, property, propertyModel)
{
//	initMyResource();
}


QWidget* Bool::createEditor(QWidget * parent, const QModelIndex & index)
{
	Q_UNUSED(index);
	m_pushButton = new QPushButton(parent);
	m_pushButton->setCheckable(true);
	m_pushButton->setChecked(value().toBool());
	m_pushButton->setText(value().toString());
	m_pushButton->setIcon(QIcon(":/" + value().toString() + ".png"));
	connect(m_pushButton, SIGNAL(clicked(bool)), this, SLOT(setValue(bool)));
	m_pushButton->setFlat ( true );
	return m_pushButton;
}

void Bool::setValue(bool checked)
{
	PropertyInterface::setValue(checked);
	m_pushButton->setText(QVariant(checked).toString());
	m_pushButton->setIcon(QIcon(":/" + QVariant(checked).toString() + ".png"));
}

QVariant Bool::data(const QModelIndex & index)
{
	if (!index.isValid() || !object() || -1 == objectProperty())
		return QVariant();

	switch (index.column())
	{
		case 0:
			return object()->metaObject()->property(objectProperty()).name();
		case 1:
			return value();
	}
	return QVariant();
}

bool Bool::setData(QVariant data, const QModelIndex & index)
{
	Q_UNUSED(index);
	return PropertyInterface::setValue(data);
}

void Bool::setEditorData(QWidget * editor, const QModelIndex & /*index*/)
{
	dynamic_cast<QPushButton *>(editor)->setChecked(value().toBool());
	dynamic_cast<QPushButton *>(editor)->setText(value().toString());
}


bool Bool::canHandle(QObject * object, int property)const
{
	if (object->metaObject()->property(property).isEnumType() || object->metaObject()->property(property).isFlagType())
		return false;

    switch (object->metaObject()->property(property).type())
	{
		case QVariant::Bool:
			return true;
		default:
			return false;
	}
}

PropertyInterface* Bool::createInstance(QObject * object, int property, const PropertyModel * propertyModel) const
{
	return new Bool(parent(), object, property, propertyModel);
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(BoolProperty, Bool)
#endif

