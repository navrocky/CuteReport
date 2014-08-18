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

#include "combobox.h"
#include "datasetinterface.h"
#include "reportcore.h"

using namespace CuteReport;

CuteReportComboBox::CuteReportComboBox(QWidget * parent)
		:QComboBox(parent)
		,m_ds(0)
{
}

void CuteReportComboBox::setDataset(CuteReport::DatasetInterface * ds)
{
    if (!ds)
        return;

    m_ds = dynamic_cast<CuteReport::DatasetInterface *>(ds);
    setModel(m_ds->model());
}

void CuteReportComboBox::setCurrent(int column, QString value)
{
	if (!m_ds)
		return;

	QAbstractItemModel * model = m_ds->model();
	if (!model)
		return;

	int row = 0;
	while (model->hasIndex(row,column))
	{
		if (model->data( model->index(row,column) ).toString() == value)
		{
			this->setCurrentIndex( row );
			return;
		}
		row++;
	}
}

QVariant CuteReportComboBox::current(int column)
{
	if (!m_ds)
		return QVariant();

	QAbstractItemModel * model = m_ds->model();
	if (!model)
		return QVariant();

	return model->data( model->index(this->currentIndex(),column) );
}
