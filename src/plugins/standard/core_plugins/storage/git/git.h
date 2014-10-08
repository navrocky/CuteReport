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
#ifndef STORAGEGIT_H
#define STORAGEGIT_H

#include "storageinterface.h"
#include "globals.h"
#include "syncmanager.h"
#include "gitstoragehelper.h"

static const QString ModuleName("GIT");

class StorageGit : public CuteReport::StorageInterface
{
    Q_OBJECT
    Q_INTERFACES(CuteReport::StorageInterface)
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteReport.StorageInterface/1.0")
#endif

    Q_PROPERTY(QString remoteURL READ remoteUrl WRITE setRemoteUrl NOTIFY remoteUrlChanged)
    Q_PROPERTY(QString localPath READ localPath WRITE setLocalPath NOTIFY localPathChanged)
    Q_PROPERTY(QString login READ login WRITE setLogin NOTIFY loginChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString gitBinaryPath READ gitBinaryPath WRITE setGitBinaryPath NOTIFY gitBinaryPathChanged)
    Q_PROPERTY(bool askForOverwrite READ askForOverwrite WRITE setAskForOverwrite NOTIFY askForOverwriteChanged)

public:

    struct SyncStruct {
        SyncManager manager;
        QFuture<bool> future;
        QFutureWatcher<bool> watcher;
    };

    explicit StorageGit(QObject * parent = 0);
    ~StorageGit();

    virtual void moduleInit();

    virtual CuteReport::StorageSyncStatus sync();               // if false, then storage cannot be synced correct;
    virtual CuteReport::StorageSyncStatus interruptSync();

    virtual StorageGit * createInstance(QObject * parent = 0) const;
    virtual StorageGit * clone();

    virtual QString urlScheme() const;

    virtual QString urlHint(const QString & key);
    virtual QStringList urlHints();

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

    void setLogin(const QString & login);
    QString login() const;

    void setPassword(const QString & password);
    QString password() const;

    void setGitBinaryPath(const QString & path);
    QString gitBinaryPath() const;

    bool askForOverwrite();
    void setAskForOverwrite(bool b);

private slots:
    void syncProcessFinished();

signals:
    void remoteUrlChanged(QString);
    void localPathChanged(QString);
    void loginChanged(QString);
    void passwordChanged(QString);
    void askForOverwriteChanged(bool);
    void gitBinaryPathChanged(QString);

private:
    bool isSyncing() const;
    void initHints();

private:
    StorageGit(const StorageGit& p );
    inline QString absolutePath(const QString & filePath = QString()) const;
    QString convertToLocal(const QString & url) const;
    QString pathCutOff(const QString & absPath);

private:
    static QMap<QString, QString> m_urlHints;
    QString m_lastError;
    QPointer<GitStorageHelper> m_helper;
    QString m_remoteUrl;
    QString m_localPath;
    QString m_localDefaultPath;
    QString m_login;
    QString m_password;
    bool m_askForOverwrite;
    QString m_gitBinaryPath;
    bool isBinaryCorrect;

    SyncStruct * m_syncStruct;

    friend class GitStorageHelper;
};

#endif // STORAGEGIT_H
