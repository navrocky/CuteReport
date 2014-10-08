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
#include "resource.h"
#include "reportinterface.h"
#include "objectfactory.h"
#include "reportcore.h"
#include "resourcestoragehelper.h"
#include <QtGui>

#define MODULENAME "StorageResource"

inline void initMyResource() { Q_INIT_RESOURCE(resource_storage); }

using namespace CuteReport;

//static const QString reportsPrefix = "reports";
//static const QString objectsPrefix = "";

StorageResource::StorageResource(QObject * parent)
    : StorageInterface(parent)
{
    m_localDefaultPath = QString(QDir::homePath() + "/" +REPORT_VARS_PATH + "/resource_storage/");
    m_localDefaultPath.replace(QRegExp("/+"), "/");
    if (!QString(REPORT_VARS_PATH).isEmpty()) {
        QDir dir(m_localDefaultPath);
        if (!dir.exists())
            dir.mkpath(m_localDefaultPath);
    }
}


StorageResource::StorageResource(const StorageResource& p):
    StorageInterface(p, p.parent()),
    m_lastError(p.lastError()),
    m_helper(0),
    m_localPath(p.localPath()),
    m_localDefaultPath(p.m_localDefaultPath),
    m_objects(p.m_objects),
    m_reports(p.m_reports)
{
}


StorageResource::~StorageResource()
{
    delete m_helper;
}


void StorageResource::moduleInit()
{
    initMyResource();
}


StorageResource * StorageResource::createInstance(QObject * parent) const
{
    return new StorageResource(parent);
}


QString StorageResource::urlScheme() const
{
    return "res";
}


StorageResource * StorageResource::clone()
{
    return new StorageResource(*this);
}


StorageSyncStatus StorageResource::sync()
{
    return CuteReport::StorageSyncInProgress;
}


StorageSyncStatus StorageResource::interruptSync()
{
    /// TODO: implement it
    return CuteReport::StorageSyncInProgress;
}


QString StorageResource::absolutePath(const QString & filePath) const
{
    QString path = (m_localPath.isEmpty() ? m_localDefaultPath : m_localPath) + "/" + filePath;
    path.replace(QRegExp("/+"), "/");
    return path;
}


QString StorageResource::convertToLocal(const QString & url) const
{
    if (url.isEmpty())
        return absolutePath();
    QUrl fileUrl(url);

    QString absoluteFilePath = absolutePath(fileUrl.path());

    return absoluteFilePath;
}


QString StorageResource::cleanupUrl(const QString & url)
{
    QString path = url;
    path.remove(QRegExp("^res:"));
    path.replace(QRegExp("/+"), "/");
    if (path[0] == '/')
        path.remove(0,1);
    return path;
}


QString StorageResource::localCachedFileName(const QString & url)
{
    return convertToLocal(url);
}


bool StorageResource::saveObject(const QString & url, const QVariant & objectData)
{
    QString objectPath = cleanupUrl(url);
    m_objects.insert(objectPath, objectData);
    return true;
}


QVariant StorageResource::loadObject(const QString & url)
{
    QString objectPath = cleanupUrl(url);
    return m_objects.value(objectPath);
}

QList<StorageObjectInfo> StorageResource::objectsList(const QString & url, const QStringList & nameFilters,
                                             QDir::Filters filters, QDir::SortFlags sort, bool * ok)
{
    Q_UNUSED(nameFilters);
    Q_UNUSED(filters);
    Q_UNUSED(sort);
    return objectsList(url, ok);
}


QList<StorageObjectInfo> StorageResource::objectsList(const QString & url, bool * ok)
{
    QList<StorageObjectInfo> list;
    QString prefix = cleanupUrl(url);

    foreach (const QString & path, m_objects.keys()){
        if (prefix.isEmpty() || path.startsWith(prefix)) {
            StorageObjectInfo objectInfo;
            objectInfo.url = urlScheme() + ":" + path;
//            objectInfo.size = fileInfo.size();
//            objectInfo.type = fileInfo.isDir() ? FileDir : FileUnknown;

            list.append(objectInfo);
//            QString str = path;
//            list.append(str.remove(0, prefix.size()));
        }
    }

    if (list.isEmpty()) {
        m_lastError = "Url path does not exist";
        if (ok)
            *ok = false;
        return list;
    }

    return list;

    //    QStringList list;
    //    foreach (const QString & path, m_objects.keys()){
    //        if (path.startsWith(prefix)) {
    //            QString str = path;
    //            list.append(str.remove(0, prefix.size()));
    //        }
    //    }

    //    return list;
}


bool StorageResource::objectExists(const QString & url)
{
    QString objectPath = cleanupUrl(url);
    return m_objects.contains(objectPath);
}


QString StorageResource::lastError() const
{
    return m_lastError;
}


CuteReport::StorageHelperInterface * StorageResource::helper()
{
    if (!m_helper)
        m_helper = new ResourceStorageHelper(this);
    return m_helper;
}


void StorageResource::setLocalPath(const QString & localPath)
{
    if (localPath == m_localPath)
        return;

    m_localPath = localPath;

    emit localPathChanged(m_localPath);
    emit changed();
}


const QString &StorageResource::localPath() const
{
    return m_localPath;
}


void StorageResource::addObject(const QString & objectPath, const QVariant & object)
{
    QString path = cleanupUrl(objectPath);
    m_objects.insert( path, object);

    emit objectAdded(objectPath);
    emit changed();
}


void StorageResource::removeObject (const QString & objectPath)
{
    m_objects.remove(objectPath);

    emit objectRemoved(objectPath);
    emit changed();
}


void StorageResource::setObjects(const QVariantHash & hash)
{
    m_objects = hash;

    emit objectsChanged();
    emit changed();
}


QVariantHash & StorageResource::objects()
{
    return m_objects;
}


void StorageResource::addReport(const QString & objectPath, const QVariant & report)
{
    QString name = objectPath.section("/", -1, -1);
    m_reports.insert(name, report);

    emit reportAdded(objectPath);
    emit changed();
}


void StorageResource::removeReport (const QString & reportName)
{
    m_reports.remove(reportName);

    emit objectRemoved(reportName);
    emit changed();
}


void StorageResource::setReports(const QVariantHash & hash)
{
    m_reports = hash;

    emit reportsChanged();
    emit changed();
}


QVariantHash & StorageResource::reports()
{
    return m_reports;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(StorageResource, StorageResource)
#endif
