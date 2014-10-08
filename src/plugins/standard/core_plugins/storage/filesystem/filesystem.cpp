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
#include "filesystem.h"
#include "reportcore.h"
#include "filesystemstoragehelper.h"

#include <QtGui>

inline void initMyResource() { Q_INIT_RESOURCE(filesystem); }

using namespace CuteReport;

QMap<QString, QString> StorageFileSystem::m_urlHints;

StorageFileSystem::StorageFileSystem(QObject * parent)
    : StorageInterface(parent),
      m_askForOverwrite(true)
{
    m_objectsPath = QString(REPORT_EXAMPLES_PATH);
    //    m_path = QString(REPORT_EXAMPLES_PATH) + "/reports";
    m_localDefaultPath = QString(QDir::homePath() + "/" + REPORT_VARS_PATH + "/file_storage/");
    m_localDefaultPath.replace(QRegExp("/+"), "/");
    if (!QString(REPORT_VARS_PATH).isEmpty()) {
        QDir dir(m_localDefaultPath);
        if (!dir.exists())
            dir.mkpath(m_localDefaultPath);
    }

    if (m_urlHints.isEmpty()) {
        initHints();
    }
}


StorageFileSystem::~StorageFileSystem()
{
    delete m_helper;
}


void StorageFileSystem::moduleInit()
{
    initMyResource();
}


void StorageFileSystem::initHints()
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


StorageFileSystem * StorageFileSystem::createInstance(QObject * parent) const
{
    return new StorageFileSystem(parent);
}


StorageFileSystem * StorageFileSystem::clone()
{
    return new StorageFileSystem(*this);
}


QString StorageFileSystem::urlScheme() const
{
    return "file";
}


QString StorageFileSystem::urlHint(const QString & key)
{
    return urlScheme() + ":" + m_urlHints.value(key);
}


QStringList StorageFileSystem::urlHints()
{
    return m_urlHints.keys();
}


QString StorageFileSystem::localCachedFileName(const QString & url)
{
    return urlToLocal(url);
}


bool StorageFileSystem::saveObject(const QString &url, const QVariant &objectData)
{
    QString absoluteFilePath = urlToLocal(url);
    if (absoluteFilePath.isEmpty())
        return false;

    QFile file(absoluteFilePath);
    if (file.open(QIODevice::WriteOnly)){
        file.write(objectData.toByteArray());
        file.close();
        return true;
    } else {
        m_lastError = QString("File \'%1\' cannot be opened: local path is \'%2\'").arg(url).arg(absoluteFilePath);
        return false;
    }
}


QVariant StorageFileSystem::loadObject(const QString & url)
{
    QString absoluteFilePath = urlToLocal(url);
    if (absoluteFilePath.isEmpty())
        return QVariant();

    QFile file(absoluteFilePath);

    if (!file.open(QIODevice::ReadOnly)) {
        m_lastError = QString("File \'%1\' cannot be opened: local path is \'%2\'").arg(url).arg(absoluteFilePath);
        return QVariant();
    }

    QByteArray ba(file.readAll());
    file.close();

    return ba;
}


QList<StorageObjectInfo> StorageFileSystem::objectsList(const QString & url, const QStringList & nameFilters,
                                             QDir::Filters filters, QDir::SortFlags sort, bool * ok)
{
    QList<StorageObjectInfo> list;

    QString absoluteFilePath = urlToLocal(url);
    if (absoluteFilePath.isEmpty()) {
        if (ok)
            *ok = false;
        return list;
    }

    QString root = urlToLocal("");
    if (!absoluteFilePath.startsWith(root)) {
        m_lastError = "Url is out of root";
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

        //        QString ext = fileInfo.suffix();
        //        if (ext.indexOf(QRegExp("([Jj][Pp][eE]?[gG])|([Pp][Nn][gG]|[Bb][Mm][Pp])|([Ii][Cc][Oo])")) == 0)
        //            objectInfo.type = FileImage;

        list.append(objectInfo);
    }

    return list;
}


QList<StorageObjectInfo> StorageFileSystem::objectsList(const QString & url, bool *ok)
{
    return objectsList(url, QStringList(), QDir::Files | QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot,
                       QDir::Name | QDir::DirsFirst, ok );
}


bool StorageFileSystem::objectExists(const QString & url)
{
    QString absoluteFilePath = urlToLocal(url);
    if (absoluteFilePath.isEmpty())
        return false;

    QString root = urlToLocal("");
    if (!absoluteFilePath.startsWith(root))
        return false;

    QFileInfo file (absoluteFilePath);

    return file.exists();
}


QString StorageFileSystem::urlToLocal(const QString &url)
{
    QUrl u(url);
    if (!u.scheme().isEmpty() && u.scheme() != this->urlScheme() ) {
        m_lastError = QString("Not appropriate scheme \'%1\' for storage scheme \'%2\'").arg(u.scheme()).arg(urlScheme());
        return QString();
    }

    QFileInfo fileInfo(u.path());

    QString absoluteFilePath;

    if (!fileInfo.isAbsolute()) {
        if (m_objectsPath.isEmpty())
            absoluteFilePath = m_localDefaultPath + "/" + fileInfo.filePath();
        else {
            QFileInfo fInfo(m_objectsPath);
            if (fInfo.isAbsolute())
                absoluteFilePath = m_objectsPath + "/" + fileInfo.filePath();
            else
                absoluteFilePath =  QDir::currentPath() + "/" + m_objectsPath + "/" + fileInfo.filePath();
        }
    } else
        absoluteFilePath = fileInfo.absoluteFilePath();

    absoluteFilePath.replace(QRegExp("/+"), "/");

    return absoluteFilePath;
}


QString StorageFileSystem::pathCutOff(const QString & absPath)
{
    QFileInfo pathIn(absPath);
    QFileInfo path(urlToLocal(""));

    QString resultFileName;

    if (pathIn.absoluteFilePath().startsWith(path.absolutePath())) {
        resultFileName = pathIn.absoluteFilePath().remove(0, path.absolutePath().size());
        if (resultFileName[0] == '/')
            resultFileName.remove(0,1);
    } else
        resultFileName =  absPath;

    return resultFileName;
}


QString StorageFileSystem::lastError() const
{
    return m_lastError;
}



CuteReport::StorageHelperInterface * StorageFileSystem::helper()
{
    if (!m_helper)
        m_helper = new FileSystemStorageHelper(this);
    return m_helper;
}


void StorageFileSystem::setObjectsRootPath(const QString & path)
{
    if (path == m_objectsPath)
        return;

    m_objectsPath = path;

    emit objectsRootPathChanged(m_objectsPath);
    emit changed();
}


const QString & StorageFileSystem::objectsRootPath() const
{
    return m_objectsPath;
}


bool StorageFileSystem::askForOverwrite()
{
    return m_askForOverwrite;
}


void StorageFileSystem::setAskForOverwrite(bool b)
{
    if (b == m_askForOverwrite)
        return;

    m_askForOverwrite = b;

    emit askForOverwriteChanged(m_askForOverwrite);
    emit changed();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(StorageFileSystem, StorageFileSystem)
#endif
