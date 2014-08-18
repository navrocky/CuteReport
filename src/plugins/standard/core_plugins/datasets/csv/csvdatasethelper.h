/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2012 by Mikhalov Alexander                              *
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
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library.                                      *
 *   If not, see <http://www.gnu.org/licenses/>.                           *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ****************************************************************************/

#ifndef CSVDATASETEDITOR_H
#define CSVDATASETEDITOR_H

#include "csvdatasethelper.h"
#include "csvdataset.h"

namespace Ui {
    class CsvDatasetHelper;
}

class CsvDatasetHelper : public CuteReport::DatasetHelperInterface
{
    Q_OBJECT
    Q_DISABLE_COPY(CsvDatasetHelper)
public:
    explicit CsvDatasetHelper( CsvDataset * dataset);
    virtual ~CsvDatasetHelper();

    virtual void save();
    virtual void load();

protected:
    virtual void changeEvent(QEvent *e);
private slots:
    void on_bBrowse_clicked();

private:
    Ui::CsvDatasetHelper *m_ui;
    CsvDataset * m_dataset;
};

#endif // CSVDATASETEDITOR_H
