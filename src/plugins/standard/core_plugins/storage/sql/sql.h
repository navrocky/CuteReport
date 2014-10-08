/***************************************************************************
 *   This file is part of the CuteReport project                           *
 *   Copyright (C) 2014 by Ivan Volkov                                     *
 *   wulff007@gmail.com                                                    *
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
#ifndef SQL_H
#define SQL_H

#include "storageinterface.h"
#include "globals.h"

static const QString ModuleName("SQL");

class QSqlDatabase;

class StorageSql : public CuteReport::StorageInterface
{
    Q_OBJECT
    Q_INTERFACES(CuteReport::StorageInterface)
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteReport.StorageInterface/1.0")
#endif

    Q_PROPERTY(QString tableName READ tableName WRITE setTableName NOTIFY tableNameChanged)
    Q_PROPERTY(QString columnId READ columnId WRITE setColumnId NOTIFY columnIdChanged)
    Q_PROPERTY(QString columnName READ columnName WRITE setColumnName NOTIFY columnNameChanged)
    Q_PROPERTY(QString columnData READ columnData WRITE setColumnData NOTIFY columnDataChanged)
    Q_PROPERTY(QString driver READ driver WRITE setDriver NOTIFY driverChanged)
    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(QString database READ database WRITE setDatabase NOTIFY databaseChanged)
    Q_PROPERTY(QString user READ user WRITE setUser NOTIFY userChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(bool useAsDefaultConnection READ useAsDefaultConnection WRITE setUseAsDefaultConnection NOTIFY useAsDefaultConnectionChanged)
    Q_PROPERTY(QString connectionId READ connectionId WRITE setConnectionId NOTIFY connectionIdChanged)

public:
    explicit StorageSql(QObject * parent = 0);
    ~StorageSql();

    // ReportPluginInterface interface
public:
    virtual int moduleVersion() const;
    virtual QString moduleShortName() const;
    virtual QString suitName() const;

    // StorageInterface interface
public:
    virtual CuteReport::StorageInterface *clone();
    virtual CuteReport::StorageHelperInterface *helper();
    virtual QString urlScheme() const;
    virtual QString localCachedFileName(const QString &url);
    virtual bool saveObject(const QString &url, const QVariant &objectData);
    virtual QVariant loadObject(const QString &url);
    virtual bool objectExists(const QString &url);
    virtual QString lastError() const;
    virtual QList<CuteReport::StorageObjectInfo> objectsList(const QString & url, bool * ok = 0);
    virtual QList<CuteReport::StorageObjectInfo> objectsList(const QString & url, const QStringList & nameFilters,
                                                 QDir::Filters filters, QDir::SortFlags sort, bool * ok = 0);

    QString tableName() const;
    void setTableName(const QString &tableName);

    QString columnId() const;
    void setColumnId(const QString &columnId);

    QString columnName() const;
    void setColumnName(const QString &columnName);

    QString columnData() const;
    void setColumnData(const QString &columnData);

    QString driver() const;
    void setDriver(const QString &driver);

    QString host() const;
    void setHost(const QString &host);

    QString database() const;
    void setDatabase(const QString &database);

    int port() const;
    void setPort(int port);

    QString user() const;
    void setUser(const QString &user);

    QString password() const;
    void setPassword(const QString &password);

    bool useAsDefaultConnection() const;
    void setUseAsDefaultConnection(bool useAsDefaultConnection);

    QString connectionId() const;
    void setConnectionId(const QString &connectionId);

signals:
    void tableNameChanged(QString);
    void columnIdChanged(QString);
    void columnNameChanged(QString);
    void columnDataChanged(QString);
    void driverChanged(QString);
    void hostChanged(QString);
    void portChanged(int);
    void databaseChanged(QString);
    void userChanged(QString);
    void passwordChanged(QString);
    void useAsDefaultConnectionChanged(bool);
    void connectionIdChanged(QString);

protected:
    virtual CuteReport::StorageInterface *createInstance(QObject *parent) const;

private:
    QString m_lastError;
    QPointer<CuteReport::StorageHelperInterface> m_helper;

    QString m_tableName;
    QString m_columnId;
    QString m_columnName;
    QString m_columnParent;
    QString m_columnData;

    QString m_driver;
    QString m_host;
    QString m_database;
    int m_port;
    QString m_user;
    QString m_password;
    bool m_useAsDefaultConnection;
    QString m_connectionId;

private:
    QString cleanupUrl(const QString & url);

    QSqlDatabase getConnection();

    bool parseUrl(const QString &url, QString &dataField, QString &name);
    QString idInName(const QString &name);
    QString clearName(const QString &name);
};

#endif // SQL_H
