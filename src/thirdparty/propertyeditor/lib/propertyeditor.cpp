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

PropertyEditor::PropertyEditor(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::propertyeditor),
      m_object(0), m_model(0), m_delegate(0)
{
    PropertyEditorCore::instance()->inc();

    ui->setupUi(this);

    m_delegate = new PropertyDelegate(this);
    ui->view->setItemDelegate(m_delegate);

    m_model = new PropertyModel(this, 0, &PropertyEditorCore::instance()->plugins());
    ui->view->setModel(m_model);
    connect (ui->view->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(currentRowChanged(QModelIndex,QModelIndex)));
}


PropertyEditor::~PropertyEditor()
{
    qDebug() << "PropertyEditor DTOR";
    PropertyEditorCore::instance()->dec();
}


void PropertyEditor::setValidator(QVariant::Type type, PropertyValidator * validator)
{
	m_validators[type]=validator;
}

PropertyValidator* PropertyEditor::validator(QVariant::Type type)
{
	return m_validators[type];
}

void PropertyEditor::clearValidators()
{
	m_validators.clear();
}

void PropertyEditor::setObject(QObject * object)
{
	if (object == m_object)
		return;
	m_object = object;
	if (m_model)
		m_model->setObject(object);
	emit(objectChanged(object));
}

void PropertyEditor::resetProperties()
{
	if (m_model)
		m_model->resetModel();
}

QObject *PropertyEditor::object() const
{
	return m_object;
}

void PropertyEditor::setSizeHint(int s)
{
    if (m_model)
	m_model->setSizeHint(s);
}


void PropertyEditor::currentRowChanged(const QModelIndex &current, const QModelIndex &)
{
    if (current.parent().isValid()) {    // only top level values are supported;
        ui->description->clear();
        return;
    }
    if (m_model)
        ui->description->setText(m_model->property(current.row())->propertyDescription());
}


}
