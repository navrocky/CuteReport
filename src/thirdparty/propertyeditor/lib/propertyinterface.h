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

#ifndef PROPERTYEDITORPROPERTYINTERFACE_H
#define PROPERTYEDITORPROPERTYINTERFACE_H

#include <QObject>
#include <QVariant>
#include <QModelIndex>
#include <QList>

#include "propertyvalidator.h"
#include "globals.h"

class QWidget;
class QMetaProperty;

namespace PropertyEditor
{
class PropertyModel;
class PROPERTYEDITOR_EXPORTS PropertyInterface: public QObject
{
	Q_OBJECT

public:
	PropertyInterface(QObject *parent = 0, QObject * object = 0, int property = -1, const PropertyModel * propertyModel = 0);
	virtual ~PropertyInterface();

	virtual int rowCount(const QModelIndex & parent);
	virtual int columnCount(const QModelIndex & parent);

	virtual void setObject(QObject * object);
	virtual QObject * object();

	virtual void setObjectProperty(int property);
	virtual int objectProperty();
	virtual QString objectPropertyName();

	virtual QWidget* createEditor(QWidget * parent, const QModelIndex & index) = 0;

	virtual QVariant data(const QModelIndex & index) = 0;
	virtual bool setData(QVariant data, const QModelIndex & index) = 0;

	virtual void setEditorData(QWidget * editor, const QModelIndex & index) = 0;

	QVariant value();
	bool setValue(QVariant data);

	void refreshPropertis(const PropertyModel * pm);
	PropertyValidator* validator(QVariant::Type type);

	virtual bool canHandle(QObject * object, int property) const = 0;
	virtual PropertyInterface* createInstance(QObject * object, int property, const PropertyModel * propertyModel) const = 0;

	virtual QModelIndex parentIndex(const QModelIndex & index);

	virtual void setParentIndex(const QModelIndex & parentIndex);

	virtual Qt::ItemFlags flags(const QModelIndex &index);

    virtual PropertyInterface* property(int prop) const;

    QStringList propertyVariants();
    QStringList propertyExclude();
    QString propertyDescription();
    int propertyPrecision();

signals:
	void propertyChanged(QObject * obj, const QString & propertyName, const QVariant & old_value, const QVariant & new_value);

private:
	QModelIndex  m_parentIndex;
	QObject * m_object;
	QList<PropertyInterface*> m_properties;
	int m_property;
    const QObject * m_propertyModel;
};

}

Q_DECLARE_INTERFACE(PropertyEditor::PropertyInterface, "eu.licentia.PropertyEditor.PropetyInterface/1.0")

#endif
