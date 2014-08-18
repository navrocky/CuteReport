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

#ifndef PROPERTYEDITORSIZE_H
#define PROPERTYEDITORSIZE_H

#include <propertyinterface.h>

/**
    @author Alexander <foxysoft@gmail.com>
*/
using namespace PropertyEditor;

class MarginsProp : public PropertyInterface
{
	Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteReport.PropertyInterface/1.0")
#endif
    Q_INTERFACES(PropertyEditor::PropertyInterface)

	Q_PROPERTY(qreal left READ left WRITE setLeft USER true)
	Q_PROPERTY(qreal right READ right WRITE setRight USER true)
	Q_PROPERTY(qreal top READ top WRITE setTop USER true)
	Q_PROPERTY(qreal bottom READ bottom WRITE setBottom USER true)

public:
    MarginsProp(QObject* parent = 0, QObject* object = 0, int property = -1, const PropertyModel * propertyModel = 0);

	QWidget* createEditor(QWidget * parent, const QModelIndex & index);

	QVariant data(const QModelIndex & index);
	bool setData(QVariant data, const QModelIndex & index);

    void setEditorData(QWidget * /*editor*/, const QModelIndex & /*index*/){}

	bool canHandle(QObject * object, int property)const;

    qreal left();
    void setLeft(qreal left);

    qreal right();
    void setRight(qreal right);

    qreal top();
    void setTop(qreal top);

    qreal bottom();
    void setBottom(qreal buttom);

	Qt::ItemFlags flags(const QModelIndex &index);

	PropertyInterface* createInstance(QObject * object, int property, const PropertyModel * propertyModel) const;
};


#endif
