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
#include "git.h"
#include "reportinterface.h"
#include "reportcore.h"
#include "gitstoragehelper.h"
#include "syncmanager.h"

#include <QtGui>
#if QT_VERSION >= 0x050000
#include <QtConcurrent>
#endif

#define MODULENAME "StorageGit"

inline void initMyResource() { Q_INIT_RESOURCE(gitresources); }

using namespace CuteReport;

QMap<QString, QString> StorageGit::m_urlHints;

StorageGit::StorageGit(QObject * parent)
    : StorageInterface(parent),
      m_askForOverwrite(false),
      m_gitBinaryPath("git"),
      m_syncStruct(0)
{
    m_remoteUrl = "https://github.com/AlFoX/CuteReport_examples.git";
    m_localDefaultPath = QString(QDir::homePath() + "/" +REPORT_VARS_PATH + "/git_storage/");
    m_localDefaultPath.replace(QRegExp("/+"), "/");
    if (!QString(REPORT_VARS_PATH).isEmpty()) {
        QDir dir(m_localDefaultPath);
        if (!dir.exists())
            dir.mkpath(m_localDefaultPath);
    }

    if (m_urlHints.isEmpty()) {
        initHints();
    }

    isBinaryCorrect = SyncManager::checkBinary(m_gitBinaryPath);
}


StorageGit::StorageGit( const StorageGit& p ):
    StorageInterface(p.parent()),
    m_lastError(p.lastError()),
    m_helper(0),
    m_remoteUrl(p.remoteUrl()),
    m_localPath(p.localPath()),
    m_localDefaultPath(p.m_localDefaultPath),
    m_login(p.m_login),
    m_password(p.m_password),
    m_askForOverwrite(p.m_askForOverwrite),
    m_gitBinaryPath(p.m_gitBinaryPath),
    m_syncStruct(0)
{
}


StorageGit::~StorageGit()
{
    /// force to wait for result for prevent crash
    if (m_syncStruct)
        m_syncStruct->future.waitForFinished();
    delete m_helper;
    delete m_syncStruct;
}


void StorageGit::moduleInit()
{
    initMyResource();
}


void StorageGit::initHints()
{
    m_urlHints.clear();

    QFile file(":/urlHints.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        reportCore()->log(LogError, ModuleName, "Hints are not recognized");
        return;
    }

    QTextStream in(&file);
    int linename = 0;
    while (!in.atEnd()) {
        linename++;
        QString line = in.readLine();
        if (line.left(1) == QString("#"))   // commented out
            continue;
        QString key = line.section("|",0,0).trimmed();
        QString value  = line.section("|", 1,1).trimmed();

        m_urlHints.insert(key, value);
    }
}


StorageGit * StorageGit::createInstance(QObject * parent) const
{
    return new StorageGit(parent);
}


StorageGit * StorageGit::clone()
{
    return new StorageGit(*this);
}


QString StorageGit::urlScheme() const
{
    return "git";
}


QString StorageGit::urlHint(const QString & key)
{
    return urlScheme() + ":" + m_urlHints.value(key);
}


QStringList StorageGit::urlHints()
{
    return m_urlHints.keys();
}


CuteReport::StorageSyncStatus StorageGit::sync()
{
    if (!isBinaryCorrect && !(isBinaryCorrect = SyncManager::checkBinary(m_gitBinaryPath))) {
        if (m_helper)
            m_helper->showSyncMessage("Error: GIT binary path is not correct!");
        return CuteReport::StorageSyncFailed;
    } else {
        if (m_helper)
            m_helper->showSyncMessage("Sync started...");
    }

    SyncManager check(m_gitBinaryPath);
    bool exists = check.isLocalRepoExists(absolutePath());

    m_syncStruct = new SyncStruct();
    m_syncStruct->manager.setGitPath(m_gitBinaryPath);
    connect(&m_syncStruct->watcher, SIGNAL(finished()), this, SLOT(syncProcessFinished()));

    if (exists) {
        m_syncStruct->future = QtConcurrent::run(&m_syncStruct->manager, &SyncManager::pull, absolutePath());
    } else {
        m_syncStruct->future = QtConcurrent::run(&m_syncStruct->manager, &SyncManager::clone, remoteUrl(), absolutePath());
    }
    m_syncStruct->watcher.setFuture(m_syncStruct->future);
    return CuteReport::StorageSyncInProgress;
}


StorageSyncStatus StorageGit::interruptSync()
{
    /// TODO: implement it
    return CuteReport::StorageSyncInProgress;
}


void StorageGit::syncProcessFinished()
{
    bool result = m_syncStruct->future.result();
    reportCore()->log(result ? CuteReport::LogDebug : LogError, m_syncStruct->manager.fullOutput(), MODULENAME);

    if (m_helper)
        m_helper->showSyncMessage(m_syncStruct->manager.output() + m_syncStruct->manager.stdOutput() + m_syncStruct->manager.stdError());

    delete m_syncStruct;
    m_syncStruct = 0;

    emit syncFinished(int(result ? CuteReport::StorageSyncSuccessful : CuteReport::StorageSyncFailed));
    emit changed();
}


bool StorageGit::isSyncing() const
{
    return (bool)m_syncStruct;
}


QString StorageGit::absolutePath(const QString & filePath) const
{
    QString path = (m_localPath.isEmpty() ? m_localDefaultPath : m_localPath) + "/" + filePath;
    path.replace(QRegExp("/+"), "/");
    return path;
}


QString StorageGit::convertToLocal(const QString & url) const
{
    if (url.isEmpty())
        return absolutePath();
    QUrl remoteUrl(m_remoteUrl);
    QUrl fileUrl(url);
    QFileInfo fileInfo(fileUrl.path());

    QString absoluteFilePath;

    if ( (remoteUrl.host() == fileUrl.host()) &&  (fileUrl.path().startsWith(remoteUrl.path()) ) )   {               /// remote url
        absoluteFilePath = absolutePath(fileUrl.path().remove(remoteUrl.path()));
    } else if (!fileInfo.isAbsolute()) {
        absoluteFilePath = absolutePath(fileUrl.path());
    } else {

    }

    absoluteFilePath.replace(QRegExp("/+"), "/");
    return absoluteFilePath;
}


QString StorageGit::pathCutOff(const QString & absPath)
{
    QFileInfo pathIn(absPath);
    QFileInfo path(absolutePath());

    QString resultFileName;

    if (pathIn.absoluteFilePath().startsWith(path.absolutePath())) {
        resultFileName = pathIn.absoluteFilePath().remove(0, path.absolutePath().size());
        if (resultFileName[0] == '/')
            resultFileName.remove(0,1);
    } else
        resultFileName =  absPath;

    return resultFileName;
}


QString StorageGit::localCachedFileName(const QString & url)
{
    return convertToLocal(url);
}


bool StorageGit::saveObject(const QString & url, const QVariant & objectData)
{
    QFile file(convertToLocal(url));

    if (file.open(QIODevice::WriteOnly)){
        file.write(objectData.toByteArray());
        file.close();
        return true;
    } else {
        m_lastError = QString("File \'%1\' cannot be opened").arg(file.fileName());
        return false;
    }
}


QVariant StorageGit::loadObject(const QString & url)
{
    QFile file(convertToLocal(url));

    if (!file.open(QIODevice::ReadOnly)) {
        m_lastError = QString("Can't open file with name \'%1\'").arg(file.fileName());
        return 0;
    }

    QByteArray ba(file.readAll());
    file.close();

    return ba;
}


QList<StorageObjectInfo> StorageGit::objectsList(const QString & url, const QStringList & nameFilters,
                                             QDir::Filters filters, QDir::SortFlags sort, bool * ok)
{
    QList<StorageObjectInfo> list;
    QString absoluteFilePath = convertToLocal(url);
    if (absoluteFilePath.isEmpty()) {
        if (ok)
            *ok = false;
        return list;
    }

    QDir dir (absoluteFilePath);

    if (!dir.exists()) {
        m_lastError = "Url path does not exist";
        if (ok)
            *ok = false;
        return list;
    }

    QFileInfoList infoList = dir.entryInfoList(nameFilters, filters | QDir::NoDotAndDotDot, sort);

    foreach (const QFileInfo &fileInfo, infoList) {
        StorageObjectInfo objectInfo;
        objectInfo.url = urlScheme() + ":" + pathCutOff(fileInfo.absoluteFilePath());
        objectInfo.size = fileInfo.size();
        objectInfo.type = fileInfo.isDir() ? FileDir : FileUnknown;

        list.append(objectInfo);
    }

    return list;
}


QList<StorageObjectInfo> StorageGit::objectsList(const QString & url, bool * ok)
{
    return objectsList(url, QStringList(), QDir::Files | QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot,
                       QDir::Name | QDir::DirsFirst, ok );
}


bool StorageGit::objectExists(const QString & url)
{
    QString absoluteFilePath = convertToLocal(url);
    if (absoluteFilePath.isEmpty())
        return false;

    QFileInfo file (absoluteFilePath);

    return file.exists();
}


QString StorageGit::lastError() const
{
    return m_lastError;
}


CuteReport::StorageHelperInterface * StorageGit::helper()
{
    if (!m_helper)
        m_helper = new GitStorageHelper(this);
    return m_helper;
}


void StorageGit::setRemoteUrl(const QString & url)
{
    if (url == m_remoteUrl)
        return;

    m_remoteUrl = url;

    emit remoteUrlChanged(m_remoteUrl);
    emit changed();
}


const QString & StorageGit::remoteUrl() const
{
    return m_remoteUrl;
}


void StorageGit::setLocalPath(const QString & localPath)
{
    if (localPath == m_localPath)
        return;

    m_localPath = localPath;

    emit localPathChanged(m_localPath);
    emit changed();
}


const QString &StorageGit::localPath() const
{
    return m_localPath;
}


void StorageGit::setLogin(const QString & login)
{
    if (m_login == login)
        return;

    m_login = login;

    emit localPathChanged(m_login);
    emit changed();
}


QString StorageGit::login() const
{
    return m_login;
}


void StorageGit::setPassword(const QString & password)
{
    if (m_password == password)
        return;

    m_password = password;

    emit localPathChanged(m_password);
    emit changed();
}


QString StorageGit::password() const
{
    return m_password;
}


void StorageGit::setGitBinaryPath(const QString & path)
{
    if (m_gitBinaryPath == path)
        return;

    m_gitBinaryPath = path;

    emit gitBinaryPathChanged(m_gitBinaryPath);
    emit changed();
}


QString StorageGit::gitBinaryPath() const
{
    return m_gitBinaryPath;
}


bool StorageGit::askForOverwrite()
{
    return m_askForOverwrite;
}


void StorageGit::setAskForOverwrite(bool b)
{
    if (b == m_askForOverwrite)
        return;

    m_askForOverwrite = b;

    emit askForOverwriteChanged(m_askForOverwrite);
    emit changed();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(StorageGIT, StorageGit)
#endif
