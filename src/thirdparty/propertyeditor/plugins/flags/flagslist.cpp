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

#include "flagslist.h"

#if QT_VERSION < 0x050000
FlagsList::FlagsList(QWidget* parent, Qt::WFlags fl): QDialog(parent, fl), Ui::flagsDialog()
#else
FlagsList::FlagsList(QWidget* parent, Qt::WindowFlags fl): QDialog(parent, fl), Ui::flagsDialog()
#endif
{
	setupUi(this);
	connect(checkAllButton, SIGNAL(pressed()), this, SLOT(checkAll()));
	connect(uncheckAllButton, SIGNAL(pressed()), this, SLOT(uncheckAll()));
}

void FlagsList::checkAll()
{
	for (int fl = 0;fl < list->count();fl++)
		list->item(fl)->setCheckState(Qt::Checked);
}

void FlagsList::uncheckAll()
{
	for (int fl = 0;fl < list->count();fl++)
		list->item(fl)->setCheckState(Qt::Unchecked);
}

FlagsList::~FlagsList()
{
}


