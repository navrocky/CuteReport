/***************************************************************************
 *   This file is part of the propertypropertyChanged project                       *
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

#include <QDebug>
#include <QMetaProperty>
#include <QPluginLoader>
#include <QDir>
#include <QMetaClassInfo>
#include <QApplication>

#include "ui_propertyeditor.h"
#include "propertyeditor.h"
#include "propertyinterface.h"
#include "propertymodel.h"
#include "propertydelegate.h"
#include "propertyeditorcore.h"
#include "propertyvalidator.h"


namespace PropertyEditor
{

EditorWidget::EditorWidget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::propertyeditor),
      m_object(0), m_model(0), m_delegate(0), m_pluginManager(0)
{
    init();
}


EditorWidget::EditorWidget(PluginManager * pluginManager, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::propertyeditor),
      m_object(0), m_model(0), m_delegate(0), m_pluginManager(pluginManager)
{
    init();
}


EditorWidget::~EditorWidget()
{
    qDebug() << "PropertyEditor DTOR";
}


void EditorWidget::init()
{
    ui->setupUi(this);
    m_delegate = new PropertyDelegate(this);
    ui->view->setItemDelegate(m_delegate);
}


PropertyModel *EditorWidget::model()
{
    if (!m_model) {
        if (!m_pluginManager) {
            m_pluginManager = new PluginManager(this);
        }
        m_model = new PropertyModel(&m_pluginManager->plugins(), this);
        ui->view->setModel(m_model);
        connect (ui->view->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(currentRowChanged(QModelIndex,QModelIndex)));
    }
    return m_model;
}


PluginManager *EditorWidget::pluginManager() const
{
    return m_pluginManager;
}


void EditorWidget::setPluginManager(PluginManager *pluginManager)
{
    delete pluginManager;
    m_pluginManager = pluginManager;
}



void EditorWidget::setValidator(QVariant::Type type, PropertyValidator * validator)
{
    m_validators[type]=validator;
}


PropertyValidator* EditorWidget::validator(QVariant::Type type)
{
	return m_validators[type];
}


void EditorWidget::clearValidators()
{
	m_validators.clear();
}


void EditorWidget::setObject(QObject * object)
{
	if (object == m_object)
		return;
	m_object = object;
    model()->setObject(object);
	emit(objectChanged(object));
}


void EditorWidget::resetProperties()
{
    model()->resetModel();
}


QObject *EditorWidget::object() const
{
	return m_object;
}


void EditorWidget::setSizeHint(int s)
{
    model()->setSizeHint(s);
}


void EditorWidget::currentRowChanged(const QModelIndex &current, const QModelIndex &)
{
    if (current.parent().isValid()) {    // only top level values are supported;
        ui->description->clear();
        return;
    }
    ui->description->setText(model()->property(current.row())->propertyDescription());
}


}
