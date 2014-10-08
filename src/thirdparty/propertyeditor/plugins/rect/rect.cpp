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
#include "rect.h"

Rect::Rect(QObject* parent, QObject* object, int property, const PropertyModel* propertyModel): PropertyInterface(parent, object, property, propertyModel)
{
	refreshPropertis(propertyModel);
}


QWidget* Rect::createEditor(QWidget * parent, const QModelIndex & index)
{
	Q_UNUSED(index);
	Q_UNUSED(parent);
	return 0;
}

QVariant Rect::data(const QModelIndex & index)
{
	switch (index.column())
	{
		case 0:
			return object()->metaObject()->property(objectProperty()).name();
		case 1:
			return QString("[(%1,%2) %3,%4]").arg(value().toRect().x()).arg(value().toRect().y()).arg(value().toRect().width()).arg(value().toRect().height());
	}
	return QVariant();
}

bool Rect::setData(QVariant data, const QModelIndex & index)
{
	Q_UNUSED(data);
	Q_UNUSED(index);
	return false;
}

bool Rect::canHandle(QObject * object, int property) const
{
	if (object->metaObject()->property(property).isEnumType() || object->metaObject()->property(property).isFlagType())
		return false;

	switch (object->property(object->metaObject()->property(property).name()).type())
	{
		case QVariant::Rect:
			return true;
		default:
			return false;
	}
	return false;
}

int Rect::x()
{
	return value().toRect().x();
}

void Rect::setX(int x)
{
	QRect s = value().toRect();
	s.setX(x);
	setValue(s);
}

int Rect::y()
{
	return value().toRect().y();
}

void Rect::setY(int y)
{
	QRect s = value().toRect();
	s.setY(y);
	setValue(s);
}


int Rect::height()
{
	return value().toRect().height();
}

void Rect::setHeight(int height)
{
	QRect s = value().toRect();
	s.setHeight(height);
	setValue(s);
}

int Rect::width()
{
	return value().toRect().width();
}

void Rect::setWidth(int width)
{
	QRect s = value().toRect();
	s.setWidth(width);
	setValue(s);
}

Qt::ItemFlags Rect::flags(const QModelIndex &/*index*/)
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

PropertyInterface* Rect::createInstance(QObject * object, int property, const PropertyModel * propertyModel) const
{
	return new Rect(parent(), object, property, propertyModel);
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(RectProperty, Rect)
#endif
