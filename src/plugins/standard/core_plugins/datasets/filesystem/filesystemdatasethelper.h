/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2012-2014 by Mikhalov Alexander                         *
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

#ifndef FILESYSTEMDATASETEDITOR_H
#define FILESYSTEMDATASETEDITOR_H

#include "filesystemdatasethelper.h"
#include "filesystemdataset.h"

namespace Ui {
    class FileSystemDatasetHelper;
}

class FileSystemDatasetHelper : public CuteReport::DatasetHelperInterface
{
    Q_OBJECT
    Q_DISABLE_COPY(FileSystemDatasetHelper)
public:
    explicit FileSystemDatasetHelper( FileSystemDataset * dataset);
    virtual ~FileSystemDatasetHelper();

    virtual void save();
    virtual void load();

protected:
    virtual void changeEvent(QEvent *e);

private slots:
    void on_bBrowse_clicked();

private:
    Ui::FileSystemDatasetHelper *m_ui;
    FileSystemDataset * m_dataset;
    int m_filter;
};

#endif // FILESYSTEMDATASETEDITOR_H
