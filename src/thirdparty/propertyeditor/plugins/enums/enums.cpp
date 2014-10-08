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
#include <QComboBox>

#include "enums.h"

Enums::Enums(QObject* parent, QObject* object, int property, const PropertyModel* propertyModel): PropertyInterface(parent, object, property, propertyModel)
{
}


QWidget* Enums::createEditor(QWidget * parent, const QModelIndex & index)
{
    Q_UNUSED(index);

    const QMetaObject * metaObject = object()->metaObject();
    QMetaProperty metaProperty = metaObject->property(objectProperty());
    const char *typeName = metaProperty.typeName();
    int enumIdx = metaObject->indexOfEnumerator(typeName);

    QStringList excludeList;

    if (int index = metaObject->indexOfProperty(QString("_%1_exclude").arg(metaProperty.name()).toLatin1())) {
        excludeList = metaObject->property(index).read(object()).toStringList();
    }

    cb = new QComboBox(parent);

    for (int en = 0; en < metaObject->enumerator(enumIdx).keyCount(); ++en) {
        QString itemName = metaObject->enumerator(enumIdx).key(en);
        int itemData = metaObject->enumerator(enumIdx).value(en);
        if (!excludeList.contains(itemName))
            cb->insertItem(en, itemName, itemData);
    }

    connect(cb, SIGNAL(currentIndexChanged(int)), this, SLOT(setValue(int)));

    return cb;
}


void Enums::setValue(int item)
{
//    PropertyInterface::setValue(cb->itemData(item));
    QVariant data = cb->itemData(item);

    QVariant old_data = object()->metaObject()->property(objectProperty()).read(object());
    QVariant newValue(old_data);
    int * intData = reinterpret_cast<int*>(newValue.data());
    *intData = data.toInt();
    bool ret = object()->metaObject()->property(objectProperty()).write(object(), newValue);

    if (ret)
        emit(propertyChanged(object(), QString(object()->metaObject()->property(objectProperty()).name()), old_data, data));
}


QVariant Enums::data(const QModelIndex & index)
{
    const QMetaObject * metaObject = object()->metaObject();
    QMetaProperty metaProperty = metaObject->property(objectProperty());
    const char *typeName = metaProperty.typeName();
    int enumIdx = metaObject->indexOfEnumerator(typeName);

    if (!index.isValid() || !object() || -1 == objectProperty())
        return QVariant();

    switch (index.column())
    {
        case 0:
            return metaProperty.name();
        case 1: {
            return metaObject->enumerator(enumIdx).valueToKey(*reinterpret_cast<int*>(value().data()));
            }
    }
    return QVariant();
}


bool Enums::setData(QVariant data, const QModelIndex & index)
{
    Q_UNUSED(index);
    return PropertyInterface::setValue(data);

//    QVariant old_data = object()->metaObject()->property(objectProperty()).read(object());
//    QVariant newValue(old_data);
//    int * intData = reinterpret_cast<int*>(newValue.data());
//    *intData = data.toInt();
//    bool ret = object()->metaObject()->property(objectProperty()).write(object(), data);

//    qDebug() << object()->metaObject()->property(objectProperty()).read(object());

//    if (ret)
//        emit(propertyChanged(object(), QString(object()->metaObject()->property(objectProperty()).name()), old_data, data));

//    return ret;
}


void Enums::setEditorData(QWidget * editor, const QModelIndex & /*index*/)
{
    QComboBox * cb = dynamic_cast<QComboBox *>(editor);
    int intValue = *reinterpret_cast<const int *>(value().constData());
    int index = cb->findData(intValue);
    cb->setCurrentIndex(index);
}


bool Enums::canHandle(QObject * object, int property)const
{
    if (object->metaObject()->property(property).isEnumType() && !object->metaObject()->property(property).isFlagType())
        return true;
    return false;
}


PropertyInterface* Enums::createInstance(QObject * object, int property, const PropertyModel * propertyModel) const
{
    return new Enums(parent(), object, property, propertyModel);
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(EnumsProperty, Enums)
#endif
