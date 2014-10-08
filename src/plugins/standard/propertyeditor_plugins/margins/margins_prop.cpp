/***************************************************************************
 *   This file is part of the CuteReport project                           *
 *   Copyright (C) 2012-2014 by Alexander Mikhalov                         *
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
#include "margins_prop.h"
#include "types.h"

MarginsProp::MarginsProp(QObject* parent, QObject* object, int property, const PropertyModel * propertyModel): PropertyInterface(parent, object, property, propertyModel)
{
	refreshPropertis(propertyModel);
}


QWidget* MarginsProp::createEditor(QWidget * parent, const QModelIndex & index)
{
	Q_UNUSED(index);
	Q_UNUSED(parent);
	return 0;
}

QVariant MarginsProp::data(const QModelIndex & index)
{
	switch (index.column())
	{
		case 0:
			return object()->metaObject()->property(objectProperty()).name();
		case 1:
            CuteReport::Margins margins= value().value<CuteReport::Margins>();
            return QString("[%1,%2,%3,%4]").arg(margins.left()).arg(margins.top()).arg(margins.right()).arg(margins.bottom());
	}
	return QVariant();
}

bool MarginsProp::setData(QVariant data, const QModelIndex & index)
{
	Q_UNUSED(index);
    Q_UNUSED(data);
    //return PropertyInterface::setValue(data);
    return false;
}


bool MarginsProp::canHandle(QObject * object, int property) const
{
	if (object->metaObject()->property(property).isEnumType() || object->metaObject()->property(property).isFlagType())
		return false;
    return object->property(object->metaObject()->property(property).name()).canConvert<CuteReport::Margins>();
}


qreal MarginsProp::left()
{
    return value().value<CuteReport::Margins>().left();
}


qreal MarginsProp::top()
{
    return value().value<CuteReport::Margins>().top();
}


qreal MarginsProp::right()
{
    return value().value<CuteReport::Margins>().right();
}


qreal MarginsProp::bottom()
{
    return value().value<CuteReport::Margins>().bottom();
}


void MarginsProp::setLeft(qreal left)
{
    CuteReport::Margins m = value().value<CuteReport::Margins>();
    m.setLeft(left);
    QVariant v;
    v.setValue(m);
    setValue(v);
}


void MarginsProp::setTop(qreal top)
{
    CuteReport::Margins m = value().value<CuteReport::Margins>();
    m.setTop(top);
    QVariant v;
    v.setValue(m);
    setValue(v);
}


void MarginsProp::setRight(qreal right)
{
    CuteReport::Margins m = value().value<CuteReport::Margins>();
    m.setRight(right);
    QVariant v;
    v.setValue(m);
    setValue(v);
}


void MarginsProp::setBottom(qreal bottom)
{
    CuteReport::Margins m = value().value<CuteReport::Margins>();
    m.setBottom(bottom);
    QVariant v;
    v.setValue(m);
    setValue(v);
}


Qt::ItemFlags MarginsProp::flags(const QModelIndex &/*index*/)
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


PropertyInterface* MarginsProp::createInstance(QObject * object, int property, const PropertyModel * propertyModel) const
{
    return new MarginsProp(parent(), object, property, propertyModel);
}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(marginsProp, MarginsProp)
#endif
