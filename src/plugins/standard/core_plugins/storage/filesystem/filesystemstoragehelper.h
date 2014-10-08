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
#ifndef FILESYSTEMSTORAGEHELPER_H
#define FILESYSTEMSTORAGEHELPER_H

#include <filesystem.h>
#include <storageinterface.h>

namespace Ui {
class FileSystemStorageHelper;
}

class  FileSystemStorageHelper : public CuteReport::StorageHelperInterface
{
    Q_OBJECT
    
public:
    explicit FileSystemStorageHelper(StorageFileSystem * storage, VisibleOptions visibleOptions = AllOptions);
    ~FileSystemStorageHelper();

public slots:
    virtual void load();
    virtual void save();
    
private slots:
    void objectsFolderChooseClicked();

private:
    void connectObject();
    void disconnectObject();

    Ui::FileSystemStorageHelper *ui;
    StorageFileSystem * m_storage;
};

#endif // FILESYSTEMSTORAGEHELPER_H
