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
#include "sql.h"
#include "sqlstoragehelper.h"

#include <QtSql>
#include <QDebug>


using namespace CuteReport;

const QString __connectName = "cutereport_designer";

StorageSql::StorageSql(QObject *parent) :
    StorageInterface(parent),
    m_helper(0)
{
}


StorageSql::~StorageSql()
{
    delete m_helper;
}


int StorageSql::moduleVersion() const
{
    return 1;
}


QString StorageSql::moduleShortName() const
{
    return ModuleName;
}


QString StorageSql::suitName() const
{
    return "Standard";
}


CuteReport::StorageInterface *StorageSql::clone()
{
    return new StorageSql(*this);
}


CuteReport::StorageHelperInterface *StorageSql::helper()
{
    if (!m_helper)
        m_helper = new SqlStorageHelper(this);
    return m_helper;
}


QString StorageSql::urlScheme() const
{
    return "sql";
}


QString StorageSql::localCachedFileName(const QString &url)
{
    return  QString(url);
}


bool StorageSql::saveObject(const QString &url, const QVariant &objectData)
{
    QString dataField;
    QString name;
    if (!parseUrl(url, dataField, name)) {
        return false;
    }

    if (m_columnName == m_columnId) {
        m_columnName.clear();
    }


    QString id = idInName(name);

    QSqlDatabase db = getConnection();
    QSqlQuery sql(db);

    if (objectExists(url)) {
        if (id.isEmpty()) {
            m_lastError = tr("id is not set! ");
            return false;
        }

        sql.prepare(QString("UPDATE %1 SET %2 = :data %4 WHERE %3 = :id")
                    .arg(m_tableName)
                    .arg(dataField)
                    .arg(m_columnId)
                    .arg(!m_columnName.isEmpty() ? QString(",%1 = :name").arg(m_columnName) : QString()));

        sql.bindValue(":id", id);
    } else {
        sql.prepare(QString("INSERT INTO %1(%2 %3 %4) VALUES (%6 :data %5)")
                    .arg(m_tableName)
                    .arg(!id.isEmpty() ? m_columnId + ',' : QString())
                    .arg(dataField)
                    .arg(!m_columnName.isEmpty() ? ',' + m_columnName : QString())
                    .arg(!m_columnName.isEmpty() ? ",:name" : QString())
                    .arg(!id.isEmpty() ? ":id," : QString()));

        if (!id.isEmpty()) {
            sql.bindValue(":id", id);
        }
    }



    if (!m_columnName.isEmpty()) {
        sql.bindValue(":name", clearName(name));
    }

    sql.bindValue(":data", objectData.canConvert(QVariant::String)
                  ? objectData.toString()
                  : objectData.toByteArray());



    //qDebug() << sql.
    if (!sql.exec()) {
        qDebug() << sql.lastQuery();
        m_lastError = sql.lastError().text();
        qCritical() << m_lastError;
        return false;
    }

    return true;
}


QVariant StorageSql::loadObject(const QString &url)
{
    QString dataField;
    QString name;

    if (!parseUrl(url, dataField, name)) {
        return QVariant();
    }

    QString id = idInName(name);
    if (id.isEmpty()) {
        QString title = clearName(name);
        if (!title.isEmpty()) {
            id = title;
        } else {
            m_lastError = tr("Report id is not correct!");
            return QVariant();
        }
    }

    QSqlDatabase db = getConnection();
    QSqlQuery sql(db);
    sql.prepare(QString("SELECT %1 FROM %2 WHERE %3 = :id")
             .arg(dataField)
             .arg(m_tableName)
             .arg(m_columnId));

    sql.bindValue(":id", id);

    if (!sql.exec()) {
        m_lastError = sql.lastError().text();
        qCritical() << sql.lastError();
        return QVariant();
    }

    if (!sql.next()) {
        m_lastError = tr("Object(id:\"%1\") not found").arg(id);
        qCritical() << m_lastError;
        return QVariant();
    }

    return sql.value(0);
}


bool StorageSql::objectExists(const QString &url)
{
    QString name;
    QString data;
    if (!parseUrl(url, data, name)) {
        return false;
    }

    QString id = idInName(name);

    if (id.isEmpty()) {
        id = clearName(name);
    }

    QSqlDatabase db = getConnection();
    QSqlQuery sql(db);
    sql.prepare(QString("SELECT COUNT(*) FROM %1 WHERE %2 = :id")
             .arg(m_tableName)
             .arg(m_columnId));

    sql.bindValue(":id",id);

    if (!sql.exec()) {
        qCritical() << sql.lastError();
        m_lastError = sql.lastError().text();
        return false;
    }

    sql.next();
    return sql.value(0).toInt() > 0;
}


QString StorageSql::lastError() const
{
    return m_lastError;
}


QList<CuteReport::StorageObjectInfo> StorageSql::objectsList(const QString &url, bool *ok)
{
    Q_UNUSED(url)
    QList<StorageObjectInfo> list;

    QSqlDatabase db = getConnection();
    QSqlQuery sql(db);

    sql.exec(QString("SELECT %1, %2 FROM %3 ORDER BY %1")
                     .arg(m_columnId)
                     .arg(!m_columnName.isEmpty() ? m_columnName : m_columnId)
                     .arg(m_tableName));

    if (sql.lastError().isValid()) {
        ok = false;
        m_lastError = sql.lastError().text();
        qCritical() << sql.lastError();
        return list;
    }

    while (sql.next()) {
        StorageObjectInfo objectInfo;
        objectInfo.url = urlScheme() + ":<" + sql.value(0).toString() + "> " + sql.value(1).toString();
        objectInfo.type = FileReport;

        list.append(objectInfo);
    }

    return list;

}


QList<CuteReport::StorageObjectInfo> StorageSql::objectsList(const QString &url, const QStringList &nameFilters, QDir::Filters filters, QDir::SortFlags sort, bool *ok)
{
    Q_UNUSED(nameFilters);
    Q_UNUSED(filters);
    Q_UNUSED(sort);

    if (filters & QDir::Dirs) {
        return QList<CuteReport::StorageObjectInfo>();
    }

    return objectsList(url, ok);
}


CuteReport::StorageInterface *StorageSql::createInstance(QObject *parent) const
{
    return new StorageSql(parent);
}


QString StorageSql::connectionId() const
{
    return m_connectionId;
}


void StorageSql::setConnectionId(const QString &connectionId)
{
    if (m_connectionId == connectionId) return;
    m_connectionId = connectionId.simplified();
    emit connectionIdChanged(m_connectionId);
    emit changed();
}


bool StorageSql::useAsDefaultConnection() const
{
    return m_useAsDefaultConnection;
}


void StorageSql::setUseAsDefaultConnection(bool useAsDefaultConnection)
{
    if (m_useAsDefaultConnection == useAsDefaultConnection) return;
    m_useAsDefaultConnection = useAsDefaultConnection;
    emit useAsDefaultConnectionChanged(m_useAsDefaultConnection);
    emit changed();
}


QString StorageSql::password() const
{
    return m_password;
}


void StorageSql::setPassword(const QString &password)
{
    if (m_password == password) return;
    m_password = password;
    emit passwordChanged(m_password);
    emit changed();
}


QString StorageSql::user() const
{
    return m_user;
}


void StorageSql::setUser(const QString &user)
{
    if (user == m_user) return;
    m_user = user;
    emit userChanged(m_user);
    emit changed();
}


int StorageSql::port() const
{
    return m_port;
}


void StorageSql::setPort(int port)
{
    if (m_port == port)  return;
    m_port = port;
    emit portChanged(m_port);
    emit changed();
}


QString StorageSql::database() const
{
    return m_database;
}


void StorageSql::setDatabase(const QString &database)
{
    if  (database == m_database) return;
    m_database = database;
    emit databaseChanged(m_database);
    emit changed();
}


QString StorageSql::host() const
{
    return m_host;
}


void StorageSql::setHost(const QString &host)
{
    if (m_host  == host) return;
    m_host = host;
    emit hostChanged(m_host);
    emit changed();
}


QString StorageSql::driver() const
{
    return m_driver;
}


void StorageSql::setDriver(const QString &driver)
{
    if (driver == m_driver) return;
    m_driver = driver;
    emit driverChanged(m_driver);
    emit changed();
}


QString StorageSql::columnData() const
{
    return m_columnData;
}


void StorageSql::setColumnData(const QString &columnData)
{
    if (m_columnData == columnData) return;
    m_columnData = columnData;
    emit columnDataChanged(m_columnData);
    emit changed();
}


QString StorageSql::columnName() const
{
    return m_columnName;
}


void StorageSql::setColumnName(const QString &columnName)
{
    if (columnName == m_columnName) return;
    m_columnName = columnName;
    emit columnNameChanged(m_columnName);
    emit changed();
}


QString StorageSql::columnId() const
{
    return m_columnId;
}


void StorageSql::setColumnId(const QString &columnId)
{
    if (columnId == m_columnId) return;
    m_columnId = columnId;
    emit columnIdChanged(m_columnId);
    emit changed();
}


QString StorageSql::tableName() const
{
    return m_tableName;
}


void StorageSql::setTableName(const QString &tableName)
{
    if (tableName == m_tableName) return;
    m_tableName = tableName;
    emit tableNameChanged(m_tableName);
    emit changed();
}


QString StorageSql::cleanupUrl(const QString &url)
{
    QString path = url;
    path.remove(QRegExp("^sql:"));
    path.replace(QRegExp("/+"), "/");
    if (path[0] == '/')
        path.remove(0,1);
    return path;
}


QSqlDatabase StorageSql::getConnection()
{
    QSqlDatabase db;

    if (!m_useAsDefaultConnection) {
        db = QSqlDatabase::database(m_connectionId.isEmpty() ? QSqlDatabase::database().connectionName() : m_connectionId);
    } else {
        db = QSqlDatabase::database(__connectName);
    }

    if (!db.isValid()) {

        if (!QSqlDatabase::contains(__connectName)) {
            db = QSqlDatabase::addDatabase(m_driver, __connectName);
        } else {
            db = QSqlDatabase::database(__connectName);
        }

        db.setHostName(m_host);
        db.setPort(m_port);
        db.setDatabaseName(m_database);
        db.setUserName(m_user);
        db.setPassword(m_password);
    }

    if (!db.open()) {
        m_lastError = db.lastError().text();
        qCritical() << db.lastError();
    }

    return db;
}


bool StorageSql::parseUrl(const QString &url, QString &dataField, QString &name)
{
    QString nUrl = cleanupUrl(url);
    QStringList params = nUrl.split('/');

    if (params.count() == 0) {
        m_lastError = tr("url not valid \"%1\"").arg(url);
        qCritical() << m_lastError;
        return false;
    }

    dataField = m_columnData;
    if (params.count() > 1) {
        dataField = params.first();
        name = params.at(1);
    } else {
        name = params.first();
    }

    return true;
}


QString StorageSql::idInName(const QString &name)
{
    QRegExp rx("<.+>");
    int pos = rx.indexIn(name);
    if (pos > -1) {
        QString tmp = name.mid(pos, rx.matchedLength());
        tmp.remove('<');
        tmp.remove('>');

        return tmp;
    }

    return QString();
}


QString StorageSql::clearName(const QString &name)
{
    QString tmp = name;
    return tmp.remove(QRegExp("<.+>"));
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(StorageSQL, StorageSql)
#endif
