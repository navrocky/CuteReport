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
#ifndef RESOURCESTORAGEHELPER_H
#define RESOURCESTORAGEHELPER_H

#include <storageinterface.h>

namespace Ui {
class ResourceStorageHelper;
}

class StorageResource;
class QListWidgetItem;

class  ResourceStorageHelper : public CuteReport::StorageHelperInterface
{
    Q_OBJECT
    
public:
    explicit ResourceStorageHelper(StorageResource * storage, VisibleOptions visibleOptions = AllOptions);
    ~ResourceStorageHelper();

    void showSyncMessage(const QString &message);

public slots:
    virtual void load();
    virtual void save();
    
private slots:
    void slotSync();
    void itemTypeToggled(bool b);
    void addReport();
    void deleteReport();
    void addNewPrefix();
    void deletePrefix();
    void addObject();
    void deleteObject();
    void fillReports(const QString & currentReport = QString());
    void fillPrefixes(const QString & currentPrefix = QString());
    void fillObjects(const QString &currentObject = QString());
    void slotCurrentPrefixChanged(QListWidgetItem* current, QListWidgetItem*);
    void slotCurrentItemChanged(QListWidgetItem* current, QListWidgetItem*);
    void slotCurrentReportChanged(QListWidgetItem* current, QListWidgetItem*);
    void updatePreview();
    void clearPreview();
    void currentPrefixChanged (QListWidgetItem* item);

private:
    void connectObject();
    void disconnectObject();

    Ui::ResourceStorageHelper *ui;
    StorageResource * m_storage;
    QMultiHash<QString, QString> m_objects;
    QString m_currentReport;
    QString m_currentObjectPrefix;
    QString m_currentObjectName;
};

#endif // RESOURCESTORAGEHELPER_H
