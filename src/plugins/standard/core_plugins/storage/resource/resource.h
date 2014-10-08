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
#ifndef STORAGERESOURCE_H
#define STORAGERESOURCE_H

#include "storageinterface.h"
#include "globals.h"
#include "resourcestoragehelper.h"

static const QString ModuleName("Resource");

class StorageResource : public CuteReport::StorageInterface
{
    Q_OBJECT
    Q_INTERFACES(CuteReport::StorageInterface)
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteReport.StorageInterface/1.0")
#endif

    Q_PROPERTY(QString localPath READ localPath WRITE setLocalPath NOTIFY localPathChanged)
    Q_PROPERTY(QVariantHash objects READ objects WRITE setObjects NOTIFY objectsChanged DESIGNABLE false)
    Q_PROPERTY(QVariantHash reports READ reports WRITE setReports NOTIFY reportsChanged DESIGNABLE false)

public:
    explicit StorageResource(QObject * parent = 0);
    ~StorageResource();

    virtual void moduleInit();

    virtual CuteReport::StorageSyncStatus sync();               // if false, then storage cannot be synced correct;
    virtual CuteReport::StorageSyncStatus interruptSync();

    virtual StorageResource * createInstance(QObject * parent = 0) const;
    virtual StorageResource * clone();

    virtual QString urlScheme() const;

    virtual QString localCachedFileName(const QString & url);
    virtual bool saveObject(const QString & url, const QVariant & objectData);
    virtual QVariant loadObject(const QString & url);
    virtual QList<CuteReport::StorageObjectInfo> objectsList(const QString & url, bool * ok = 0);
    virtual QList<CuteReport::StorageObjectInfo> objectsList(const QString & url, const QStringList & nameFilters,
                                                 QDir::Filters filters, QDir::SortFlags sort, bool * ok = 0);

    virtual bool objectExists(const QString & url);

    virtual QString lastError() const;

    virtual int moduleVersion() const {return 1;}
    virtual QString moduleShortName() const {return ModuleName; }
    virtual QString suitName() const { return "Standard"; }

    // storage must care of deleting helper
    virtual CuteReport::StorageHelperInterface * helper();

    void setRemoteUrl(const QString & url);
    const QString &remoteUrl() const;

    void setLocalPath(const QString & localPath);
    const QString &localPath() const;

    void addObject(const QString & objectPath, const QVariant & object);
    void removeObject (const QString & objectPath);

    void setObjects(const QVariantHash & hash);
    QVariantHash & objects();

    void addReport(const QString & objectPath, const QVariant & report);
    void removeReport (const QString & reportName);

    void setReports(const QVariantHash & hash);
    QVariantHash & reports();


signals:
    void objectsChanged();
    void reportsChanged();
    void localPathChanged(QString);
    void objectAdded(const QString & objectPath);
    void objectRemoved(const QString & objectPath);
    void reportAdded(const QString & reportName);
    void reportRemoved(const QString & reportName);


private:
    explicit StorageResource(const StorageResource& p);
    inline QString absolutePath(const QString & filePath = QString()) const;
    QString convertToLocal(const QString & url) const;
    QString cleanupUrl(const QString & url);

private:
    QString m_lastError;
    QPointer<ResourceStorageHelper> m_helper;
    QString m_localPath;
    QString m_localDefaultPath;
    QVariantHash m_objects;
    QVariantHash m_reports;

};

#endif // STORAGERESOURCE_H
