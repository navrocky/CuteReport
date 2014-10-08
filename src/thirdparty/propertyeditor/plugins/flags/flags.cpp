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
#include <QListWidget>

#include "flagslist.h"
#include "flags.h"

Flags::Flags(QObject* parent, QObject* object, int property, const PropertyModel* propertyModel): PropertyInterface(parent, object, property, propertyModel)
{
}

QWidget* Flags::createEditor(QWidget * parent, const QModelIndex & index)
{
	Q_UNUSED(index);
	QPushButton * bt = new QPushButton(parent);
	bt->setText(tr("Change flags"));
	connect(bt, SIGNAL(pressed()), this, SLOT(buttonPressed()));
	return bt;
}

void Flags::buttonPressed()
{
	FlagsList f;
	f.setWindowTitle(QString(object()->metaObject()->property(objectProperty()).name()) + tr(" flags"));
	f.list->clear();

	for (int en = 0;en < object()->metaObject()->enumerator(object()->metaObject()->indexOfEnumerator(object()->metaObject()->property(objectProperty()).typeName())).keyCount();en++)
	{
		f.list->addItem(object()->metaObject()->enumerator(object()->metaObject()->indexOfEnumerator(object()->metaObject()->property(objectProperty()).typeName())).key(en));
		f.list->item(en)->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		f.list->item(en)->setCheckState((value().toInt() & object()->metaObject()->enumerator(object()->metaObject()->indexOfEnumerator(object()->metaObject()->property(objectProperty()).typeName())).value(en)) ? Qt::Checked : Qt::Unchecked);
	}

	if (f.exec() == QDialog::Rejected)
		return;

	int flags = 0;
	for (int fl = 0;fl < f.list->count();fl++)
		if (f.list->item(fl)->checkState() == Qt::Checked)
			flags |= object()->metaObject()->enumerator(object()->metaObject()->indexOfEnumerator(object()->metaObject()->property(objectProperty()).typeName())).value(fl);

	setValue(flags);
}

QVariant Flags::data(const QModelIndex & index)
{
	if (!index.isValid() || !object() || -1 == objectProperty())
		return QVariant();
	switch (index.column())
	{
		case 0:
			return object()->metaObject()->property(objectProperty()).name();

		case 1:
			QString s = "[";
			for (int en = 0;en < object()->metaObject()->enumerator(object()->metaObject()->indexOfEnumerator(object()->metaObject()->property(objectProperty()).typeName())).keyCount();en++)
				if (value().toInt() & object()->metaObject()->enumerator(object()->metaObject()->indexOfEnumerator(object()->metaObject()->property(objectProperty()).typeName())).value(en))
				{
					if (s.size() > 1)
						s += QString("|") + QString(object()->metaObject()->enumerator(object()->metaObject()->indexOfEnumerator(object()->metaObject()->property(objectProperty()).typeName())).key(en));
					else
						s += QString(object()->metaObject()->enumerator(object()->metaObject()->indexOfEnumerator(object()->metaObject()->property(objectProperty()).typeName())).key(en));
				}
			s += "]";
			return s;
	}
	return QVariant();
}

bool Flags::setData(QVariant data, const QModelIndex & index)
{
	Q_UNUSED(index);
	return PropertyInterface::setValue(data);
}


bool Flags::canHandle(QObject * object, int property)const
{
	if (object->metaObject()->property(property).isFlagType())
		return true;
	return false;
}

PropertyInterface* Flags::createInstance(QObject * object, int property, const PropertyModel * propertyModel) const
{
	return new Flags(parent(), object, property, propertyModel);
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(FlagsProperty, Flags)
#endif
