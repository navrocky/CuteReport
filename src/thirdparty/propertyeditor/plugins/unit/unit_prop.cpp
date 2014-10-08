/***************************************************************************
 *   This file is part of the CuteReport project                           *
 *   Copyright (C) 2012 by Alexander Mikhalov                              *
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

#include <QtCore>
#include <QMetaProperty>
#include <QMetaEnum>
#include "unit_prop.h"
#include "types.h"

UnitProp::UnitProp(QObject* parent, QObject* object, int property, const PropertyModel * propertyModel): PropertyInterface(parent, object, property, propertyModel)
{
	refreshPropertis(propertyModel);
}


QWidget* UnitProp::createEditor(QWidget * parent, const QModelIndex & index)
{
	Q_UNUSED(index);
	Q_UNUSED(parent);
	return 0;
}

QVariant UnitProp::data(const QModelIndex & index)
{
    switch (index.column())
    {
    case 0:
        return object()->metaObject()->property(objectProperty()).name();
    case 1:
        Units units= value().value<Units>();
        return QString("[mm=%1 inch=%2 pix=%3]").arg(units.mm()).arg(units.inch()).arg(units.pixel());
    }
    return QVariant();
}

bool UnitProp::setData(QVariant data, const QModelIndex & index)
{
	Q_UNUSED(index);
    Q_UNUSED(data);
    //return PropertyInterface::setValue(data);
    return false;
}


bool UnitProp::canHandle(QObject * object, int property) const
{
	if (object->metaObject()->property(property).isEnumType() || object->metaObject()->property(property).isFlagType())
		return false;
//    qDebug("name %s", object->metaObject()->property(property).name());
    return object->property(object->metaObject()->property(property).name()).canConvert<Units>();
}



qreal UnitProp::mm()
{
    return value().value<Units>().mm();
}

void UnitProp::setMM(qreal unit)
{
    Units u = value().value<Units>();
    u.setMM(unit);
    QVariant v;
    v.setValue(u);
    setValue(v);
}

qreal UnitProp::inch()
{
    return value().value<Units>().inch();
}

void UnitProp::setInch(qreal unit)
{
    Units u = value().value<Units>();
    u.setInch(unit);
    QVariant v;
    v.setValue(u);
    setValue(v);
}

int UnitProp::pixel()
{
    return value().value<Units>().pixel();
}

void UnitProp::setPixel(int unit)
{
    Units u = value().value<Units>();
    u.setPixel(unit);
    QVariant v;
    v.setValue(u);
    setValue(v);
}


Qt::ItemFlags UnitProp::flags(const QModelIndex &/*index*/)
{
	return Qt::ItemIsEnabled;
}

PropertyInterface* UnitProp::createInstance(QObject * object, int property, const PropertyModel * propertyModel) const
{
    return new UnitProp(parent(), object, property, propertyModel);
}

Q_EXPORT_PLUGIN2(unitProp, UnitProperty)
