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

#ifndef PROPERTYEDITORUNIT_H
#define PROPERTYEDITORUNIT_H

#include <propertyinterface.h>

/**
    @author Alexander <foxysoft@gmail.com>
*/
using namespace PropertyEditor;

class UnitProp : public PropertyInterface
{
	Q_OBJECT
    Q_INTERFACES(PropertyEditor::PropertyInterface)

    Q_PROPERTY(qreal mm READ mm WRITE setMM USER true)
    Q_PROPERTY(qreal inch READ inch WRITE setInch USER true)
    Q_PROPERTY(int pixel READ pixel WRITE setPixel USER true)


public:
    UnitProp(QObject* parent = 0, QObject* object = 0, int property = -1, const PropertyModel * propertyModel = 0);

	QWidget* createEditor(QWidget * parent, const QModelIndex & index);

	QVariant data(const QModelIndex & index);
	bool setData(QVariant data, const QModelIndex & index);

    void setEditorData(QWidget * /*editor*/, const QModelIndex & /*index*/){}

	bool canHandle(QObject * object, int property)const;

    qreal mm();
    void setMM(qreal unit);

    qreal inch();
    void setInch(qreal unit);

    int pixel();
    void setPixel(int unit);



	Qt::ItemFlags flags(const QModelIndex &index);

	PropertyInterface* createInstance(QObject * object, int property, const PropertyModel * propertyModel) const;
};


#endif
