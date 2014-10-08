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

#ifndef PROPERTYEDITORPROPERTYMODEL_H
#define PROPERTYEDITORPROPERTYMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include <QPointer>


namespace PropertyEditor
{

class PropertyInterface;

class PropertyModel : public QAbstractItemModel
{
	Q_OBJECT

public:
    PropertyModel(const QList<PropertyInterface *> *plugins, QObject* parent = 0);

	~PropertyModel();

	void setObject(QObject * object);
	void refreshProperties();

	QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex & parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	QModelIndex parent(const QModelIndex & index) const;

	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	PropertyInterface* createPropery(QObject* object, int property) const;
    PropertyInterface* property(int number) const;

	void resetModel();
    void setSizeHint(int s);

private:
	void clearCreatedProperties();
    QStringList propertiesHidden();

private:
	QObject* m_object;
	QList<PropertyInterface*> m_properties;
    const QList<PropertyInterface*> *m_plugins;
	mutable QList< QPointer<PropertyInterface> >  m_createdProperties;
    int m_sizeHint;
};

}

#endif
