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
 ****************************************************************************/
#ifndef STORAGEINTERFACE_H
#define STORAGEINTERFACE_H

#include "reportcore.h"
#include "reportplugininterface.h"
#include "globals.h"
#include "types.h"
#include <QString>
#include <QWidget>

/** Storage interface implementation have to support 3 variants of file path:
 * 1) remote absolute url: git@bitbucket.org:alfox/report.git/object.txt
 * 2) local absolute path: /home/alex/temp/cutereport/report/object.txt
 * 3) local relative path: databases/object.txt
 *
 * only third is recomended for use in reports
*/

namespace CuteReport
{
class StorageHelperInterface;

class CUTEREPORT_EXPORTS StorageInterface : public ReportPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(CuteReport::ReportPluginInterface)

public:
    explicit StorageInterface(QObject * parent):ReportPluginInterface(parent) {}
    virtual ~StorageInterface(){}

    virtual StorageInterface * clone() = 0;
    virtual CuteReport::StorageHelperInterface * helper() {return 0;}

    virtual QString urlScheme() const = 0;

    virtual QString urlHint(const QString & key) {Q_UNUSED(key); return QString();}
    virtual QStringList urlHints() {return QStringList();}


    /** if module doesn't need time for sync or it isn't threaded it should return SyncSuccessful
     * In case it needs some time for threaded syncing module should return SyncInProgress and when
     * it finished emit signal syncFinished(SyncStatus)
     */
    virtual CuteReport::StorageSyncStatus sync() {return CuteReport::StorageSyncSuccessful;}                // if false, then storage cannot be synced correct;

    /** informs module for interrupt current syncing
     * it's not garantied it will be done immediately and still need to wait for syncFinished(SyncStatus) signal
     */
    virtual CuteReport::StorageSyncStatus interruptSync() {return CuteReport::StorageSyncSuccessful;}    // interrupts current syncing

    virtual QString localCachedFileName(const QString & url) = 0;

    virtual bool saveObject(const QString & url, const QVariant & objectData) = 0;
    virtual QVariant loadObject(const QString & url) = 0;

    /** must return ok false if path doesnt exist */
    virtual QList<StorageObjectInfo> objectsList(const QString & url, bool * ok = 0) = 0;
    virtual QList<StorageObjectInfo> objectsList(const QString & url, const QStringList & nameFilters, QDir::Filters filters,
                                                 QDir::SortFlags sort, bool * ok = 0) = 0;

    virtual bool objectExists(const QString & url) = 0;

    virtual QString rootUrl() { return urlScheme() + ":";}

    // TODO
    // async methods: return request id
//    virtual qint32 reportsInfoListRequest() const = 0;
//    virtual qint32 objectsInfoListRequest(CuteReport::ReportInterface * report, const QString & prefix) const= 0;
//    virtual qint32 saveObject(const QString & url, const QVariant & objectData, CuteReport::ReportInterface * report) = 0;
//    virtual qint32 loadObject(const QString & url, CuteReport::ReportInterface * report) = 0;

    virtual QString lastError() const = 0;

signals:
    void syncFinished(int syncStatus);
    // TODO
//    void reportsInfoListResult(StorageListResult result);
//    void objectsInfoListResult(StorageListResult result);
//    void progress(StorageProgress);

protected:
    virtual StorageInterface * createInstance(QObject * parent = 0) const = 0;
    explicit StorageInterface(const StorageInterface & source, QObject * parent = 0)
        :ReportPluginInterface(source, parent){}

    friend class ReportCore;
};


class CUTEREPORT_EXPORTS StorageHelperInterface: public QWidget
{
    Q_OBJECT
public:
    enum VisibleOptions{ReportsOptions, ObjectsOptions, AllOptions};
    StorageHelperInterface(StorageInterface *, VisibleOptions = AllOptions){}
    virtual void load() = 0;
    virtual void save() = 0;
};

}

Q_DECLARE_INTERFACE(CuteReport::StorageInterface, "CuteReport.StorageInterface/1.0")
Q_DECLARE_INTERFACE(CuteReport::StorageHelperInterface, "CuteReport.StorageHelperInterface/1.0")


#endif // STORAGEINTERFACE_H
