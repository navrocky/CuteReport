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
#ifndef SQLDATASET_H
#define SQLDATASET_H

#include <datasetinterface.h>
#include <QtSql>
#include <QSortFilterProxyModel>
#include <reportinterface.h>
#include "globals.h"

#define DATASET_NAME "SQL"

class SqlDataset : public CuteReport::DatasetInterface
{
	Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteReport.DatasetInterface/1.0")
#endif
    Q_INTERFACES(CuteReport::DatasetInterface)

    Q_PROPERTY(QString query READ query WRITE setQuery NOTIFY queryChanged)
    Q_PROPERTY(QString dbhost READ dbhost WRITE setdbhost NOTIFY dbhostChanged)
    Q_PROPERTY(QString dbname READ dbname WRITE setdbname NOTIFY dbnameChanged)
    Q_PROPERTY(QString dbuser READ dbuser WRITE setdbuser NOTIFY dbuserChanged)
    Q_PROPERTY(QString dbpasswd READ dbpasswd WRITE setdbpasswd NOTIFY dbpasswdChanged)
    Q_PROPERTY(QString driver READ driver WRITE setDriver NOTIFY driverChanged)

public:
    explicit SqlDataset(QObject *parent = 0);
    virtual ~SqlDataset();

    virtual QIcon icon();

    virtual DatasetInterface * createInstance(QObject* parent = 0) const;
    virtual CuteReport::DatasetHelperInterface * helper();
    virtual QAbstractItemModel * model();

    virtual QString lastError();

    virtual bool populate();
    virtual bool isPopulated();
    virtual void setPopulated(bool b);
    virtual void reset();
    virtual void resetCursor();
    virtual bool firstRow();
    virtual bool lastRow();
    virtual bool nextRow();
    virtual bool previousRow();
    virtual int  currentRow();
    virtual bool setCurrentRow(int index);
    virtual int rows();
    virtual int columns();
    virtual QVariant value(int index) const;
    virtual QVariant value(const QString & field) const;
    virtual QVariant lookaheadValue(int index) const;
    virtual QVariant lookaheadValue(const QString & field) const;
    virtual QVariant lookbackValue(int index) const;
    virtual QVariant lookbackValue(const QString & field) const;
    virtual QString fieldName(int column );
    virtual QVariant::Type fieldType(int column);

    QString	    query() const;
    void	    setQuery(const QString &str);
    QSet<QString> variables() const;

    QStringList drivers() const;
    QString driver() const;
    void setDriver(QString driver);
    QString dbhost() const;
    void setdbhost(QString host);
    QString dbuser() const;
    void setdbuser(QString user);
    QString dbname() const;
    void setdbname(QString name);
    QString dbpasswd() const;
    void setdbpasswd(QString passwd);

    virtual QString moduleShortName() const {return QString("SQL");}
    virtual QString suitName() const { return "Standard"; }

signals:
    void queryChanged(QString);
    void dbhostChanged(QString);
    void dbnameChanged(QString);
    void dbuserChanged(QString);
    void dbpasswdChanged(QString);
    void driverChanged(QString);

private:
    explicit SqlDataset(const SqlDataset &dd, QObject * parent);
    virtual DatasetInterface * objectClone() const;

	int m_currentRow;
	bool m_isPopulated;
	QString m_queryText;
	QSqlQueryModel * m_model;
	QSortFilterProxyModel * m_fmodel;
    QPointer<CuteReport::DatasetHelperInterface> m_helper;
    QString m_dbhost;
    QString m_dbname;
    QString m_dbuser;
    QString m_dbpasswd;
    QString m_driver;
    QSqlDatabase db;
    QString m_connectionName;
    QString m_lastError;
};

#endif
