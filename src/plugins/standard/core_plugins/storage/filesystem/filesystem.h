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
#ifndef STORAGEFILESYSTEM_H
#define STORAGEFILESYSTEM_H

#include "storageinterface.h"
#include "globals.h"

static const QString ModuleName("FileSystem");

class StorageFileSystem : public CuteReport::StorageInterface
{
    Q_OBJECT
    Q_INTERFACES(CuteReport::StorageInterface)
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteReport.StorageInterface/1.0")
#endif

    Q_PROPERTY(QString objectsRootPath READ objectsRootPath WRITE setObjectsRootPath NOTIFY objectsRootPathChanged)
    Q_PROPERTY(bool askForOverwrite READ askForOverwrite WRITE setAskForOverwrite NOTIFY askForOverwriteChanged)

public:
    explicit StorageFileSystem(QObject * parent = 0);
    ~StorageFileSystem();

    virtual void moduleInit();

    virtual StorageFileSystem * createInstance(QObject * parent = 0) const;
    virtual StorageFileSystem * clone();

    virtual QString urlScheme() const;

    virtual QString urlHint(const QString & key);
    virtual QStringList urlHints();

    virtual QString localCachedFileName(const QString & url);

    virtual bool saveObject(const QString &url, const QVariant &objectData);
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

    void setObjectsRootPath(const QString & path);
    const QString &objectsRootPath() const;

    bool askForOverwrite();
    void setAskForOverwrite(bool b);

signals:
    void objectsRootPathChanged(QString);
    void askForOverwriteChanged(bool);

private:
    void initHints();
    QString urlToLocal(const QString &url);
    QString pathCutOff(const QString & absPath);

private:
    static QMap<QString, QString> m_urlHints;
    QString m_lastError;
    QPointer<CuteReport::StorageHelperInterface> m_helper;
    QString m_localDefaultPath;
    QString m_objectsPath;
    bool m_askForOverwrite;
};

#endif // STORAGEFILESYSTEM_H
