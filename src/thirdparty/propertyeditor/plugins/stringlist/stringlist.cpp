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

#include "stringlist.h"
#include "stringlisteditor.h"

//inline void initMyResource()
//{
//	Q_INIT_RESOURCE(stringlist);
//}


StringList::StringList(QObject* parent, QObject* object, int property, const PropertyModel * propertyModel): PropertyInterface(parent, object, property, propertyModel)
{
//	initMyResource();
}


QWidget* StringList::createEditor(QWidget * parent, const QModelIndex & index)
{
	Q_UNUSED(index);
	QPushButton * bt = new QPushButton(parent);
	bt->setText(tr("Change stringlist"));
	connect(bt, SIGNAL(pressed()), this, SLOT(buttonPressed()));
	return bt;
}

void StringList::buttonPressed()
{
	int result;
	QStringList lst = StringListEditor::getStringList(0, value().toStringList(), &result);
	if (result == QDialog::Accepted)
		setValue(lst);
}

QVariant StringList::data(const QModelIndex & index)
{
	if (!index.isValid() || !object() || -1 == objectProperty())
		return QVariant();

	switch (index.column())
	{
		case 0:
			return object()->metaObject()->property(objectProperty()).name();
		case 1:
        QString s = "{" + value().toStringList().join(", ") + "}";
        return s;
	}
	return QVariant();
}

bool StringList::setData(QVariant data, const QModelIndex & index)
{
	Q_UNUSED(index);
	return PropertyInterface::setValue(data);
}


bool StringList::canHandle(QObject * object, int property)const
{
	if (object->metaObject()->property(property).isEnumType() || object->metaObject()->property(property).isFlagType())
		return false;

	switch (object->property(object->metaObject()->property(property).name()).type())
	{
		case QVariant::StringList:
			return true;
		default:
			return false;
	}
}

PropertyInterface* StringList::createInstance(QObject * object, int property, const PropertyModel * propertyModel) const
{
	return new StringList(parent(), object, property, propertyModel);
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(StringListProperty, StringList)
#endif
