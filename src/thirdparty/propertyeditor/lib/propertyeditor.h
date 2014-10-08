/***************************************************************************
 *   This file is part of the propertyEditor project                       *
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *   bog_dan_ro@yahoo.com                                                  *
 *   Copyright (C) 2013 by Mikhalov Alexander                              *
 *   alexander.mikhalov@gmail.com                                          *
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
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ****************************************************************************/

#ifndef PROPERTYEDITORPROPERTYEDITOR_H
#define PROPERTYEDITORPROPERTYEDITOR_H

#include "globals.h"
#include <QWidget>
#include <QVariant>
#include <QModelIndex>

namespace Ui {
class propertyeditor;
}

namespace PropertyEditor
{

class PropertyInterface;
class PropertyModel;
class PropertyDelegate;
class PropertyValidator;
class PluginManager;

class PROPERTYEDITOR_EXPORTS EditorWidget : public QWidget
{
	Q_OBJECT

public:
    explicit EditorWidget(QWidget *parent = 0);
    explicit EditorWidget(PluginManager * pluginManager, QWidget *parent = 0);
    ~EditorWidget();

	QObject *object() const;
	void setValidator(QVariant::Type type, PropertyValidator * validator);
	PropertyValidator* validator(QVariant::Type type);
	void clearValidators();
    void setSizeHint(int s);

    PluginManager *pluginManager() const;
    void setPluginManager(PluginManager *pluginManager);


public slots:
    void setObject(QObject * object);
    void resetProperties();

private slots:
    void currentRowChanged(const QModelIndex &current, const QModelIndex &);

signals:
    void propertyChanged(QObject * obj, const QString & propertyName, const QVariant & old_value, const QVariant & new_value);
    void objectChanged(QObject * obj);

private:
    void init();
    PropertyModel *model();

    PluginManager * m_pluginManager;
    Ui::propertyeditor *ui;
	QObject *m_object;
	PropertyModel* m_model;
	PropertyDelegate* m_delegate;
    QMap<int, PropertyValidator*> m_validators;
};

}

#endif
