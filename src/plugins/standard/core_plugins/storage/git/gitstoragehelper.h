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
#ifndef GITSTORAGEHELPER_H
#define GITSTORAGEHELPER_H

#include <storageinterface.h>

namespace Ui {
class GitStorageHelper;
}

class StorageGit;

class  GitStorageHelper : public CuteReport::StorageHelperInterface
{
    Q_OBJECT
    
public:
    explicit GitStorageHelper(StorageGit * storage, VisibleOptions visibleOptions = AllOptions);
    ~GitStorageHelper();


    void showSyncMessage(const QString &message);

public slots:
    virtual void load();
    virtual void save();
    
private slots:
    void slotSync();
    void slotBinaryBrowse();
    void localPathChooseClicked();
    bool setGitVersion();

private:
    void connectObject();
    void disconnectObject();

    Ui::GitStorageHelper *ui;
    StorageGit * m_storage;
};

#endif // GITSTORAGEHELPER_H
