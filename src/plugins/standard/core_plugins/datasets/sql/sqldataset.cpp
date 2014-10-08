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
#include "sqldataset.h"
#include "sqldatasethelper.h"
#include "reportcore.h"
#include "functions.h"

#include <QSqlDatabase>

using namespace CuteReport;

SqlDataset::SqlDataset(QObject *parent)
 : CuteReport::DatasetInterface(parent)
{
    m_currentRow = 0;
    m_isPopulated = false;
    m_model = new QSqlQueryModel  (this);
    m_fmodel = new QSortFilterProxyModel(this);
    m_fmodel->setSourceModel(m_model);
    m_connectionName = QString("_sqldataset_%1").arg(QTime::currentTime().msec());
}


SqlDataset::SqlDataset(const SqlDataset &dd, QObject * parent)
    : DatasetInterface(parent)
{
    m_currentRow = 0;
    m_isPopulated = false;
    m_queryText = dd.m_queryText;
    m_dbhost = dd.m_dbhost;
    m_dbname = dd.m_dbname;
    m_dbuser = dd.m_dbuser;
    m_dbpasswd = dd.m_dbpasswd;
    m_driver = dd.m_driver;
    m_connectionName = dd.m_connectionName;
    m_lastError = dd.m_lastError;
    m_model = new QSqlQueryModel(this);
    m_fmodel = new QSortFilterProxyModel(this);
    m_fmodel->setSourceModel(m_model);
    if (dd.m_isPopulated) {
        populate();
        setCurrentRow(dd.m_currentRow);
    }
}

SqlDataset::~SqlDataset()
{
    if (m_helper)
        delete m_helper;
}


QIcon SqlDataset::icon()
{
    QIcon icon(":/images/sql.png");
    return icon;
}


CuteReport::DatasetInterface * SqlDataset::createInstance(QObject* parent) const
{
    return new SqlDataset(parent);
}


DatasetInterface *SqlDataset::objectClone() const
{
    return new SqlDataset(*this, parent());
}


DatasetHelperInterface * SqlDataset::helper()
{
    if (!m_helper)
        m_helper = new SqlDatasetHelper(this);

    return m_helper;
}


QString SqlDataset::lastError()
{
    if (!db.isValid() || db.isOpenError() || !db.isOpen())
        return m_lastError;
    return m_model->lastError().text();
}


QString SqlDataset::fieldName(int column )
{
    if (!m_isPopulated)
        populate();
    return m_model->record().fieldName(column);
//    } else
//        return m_model->headerData ( column, Qt::Horizontal).toString();
}


QVariant::Type SqlDataset::fieldType(int column)
{
    return QVariant::String;
}



QAbstractItemModel * SqlDataset::model()
{
    return m_fmodel;
}


QString SqlDataset::query() const
{
    return m_queryText;
}


void SqlDataset::setQuery(const QString &str)
{
    if (m_queryText == str)
        return;

    m_queryText = str;

    emit queryChanged(m_queryText);
    emit variablesChanged();
    emit changed();
}


bool SqlDataset::populate()
{
    emit beforePopulate();

    reset();

    QString script;
    QString dbHost;
    QString dbName;
    QString dbUser;
    QString dbPassword;
    QString dbDriver;

    CuteReport::ReportInterface* report = dynamic_cast<CuteReport::ReportInterface*> (parent());

    if (report) {
        QStringList missedVariables;

        if (!isStringValued(m_queryText, report->variables(), &missedVariables)) {
            m_lastError = QString("Variable is not defined in \'query\' property: %1").arg(missedVariables.join(", "));
            return false;
        }
        script = setVariablesValue(m_queryText, report->variables());

        if (!isStringValued(m_dbhost, report->variables(), &missedVariables)) {
            m_lastError = QString("Variable is not defined in \'dbhost\' property: %1").arg(missedVariables.join(", "));
            return false;
        }
        dbHost = setVariablesValue(m_dbhost, report->variables());

        if (!isStringValued(m_dbname, report->variables(), &missedVariables)) {
            m_lastError = QString("Variable is not defined in \'dbname\' property: %1").arg(missedVariables.join(", "));
            return false;
        }
        dbName = setVariablesValue(m_dbname, report->variables());

        if (!isStringValued(m_dbuser, report->variables(), &missedVariables)) {
            m_lastError = QString("Variable is not defined in \'dbuser\' property: %1").arg(missedVariables.join(", "));
            return false;
        }
        dbUser = setVariablesValue(m_dbuser, report->variables());

        if (!isStringValued(m_dbpasswd, report->variables(), &missedVariables)) {
            m_lastError = QString("Variable is not defined in \'dbpassword\' property: %1").arg(missedVariables.join(", "));
            return false;
        }
        dbPassword = setVariablesValue(m_dbpasswd, report->variables());

        if (!isStringValued(m_driver, report->variables(), &missedVariables)) {
            m_lastError = QString("Variable is not defined in \'driver\' property: %1").arg(missedVariables.join(", "));
            return false;
        }
        dbDriver = setVariablesValue(m_driver, report->variables());

    } else {
        script = m_queryText;
        dbHost = m_dbhost;
        dbName = m_dbname;
        dbUser = m_dbuser;
        dbPassword = m_dbpasswd;
        dbDriver = m_driver;
    }

    ReportCore::log(CuteReport::LogDebug, "SqlDataset", QString("query = %1").arg(script));

    dbName = dbHost.isEmpty() ?  reportCore()->localCachedFileName(dbName, report) : dbName ;
    if (dbName.isEmpty()) {
        m_lastError = QString("Database name is empty.");
        return false;
    }

    if (dbDriver.isEmpty()) {
        m_lastError = QString("Database driver not defined.");
        return false;
    }

    if (script.isEmpty()) {
        m_lastError = QString("Query text is empty.");
        return false;
    }

    db = QSqlDatabase::database(m_connectionName, false);
    if (!db.isValid()) {
        db = QSqlDatabase::addDatabase(dbDriver, m_connectionName);
        if (!db.isValid()) {
            m_lastError = db.lastError().text();
            return false;
        }
    }

    db.setHostName(dbHost);
    db.setDatabaseName(dbName);
    db.setUserName(dbUser);
    db.setPassword(dbPassword);

    if (!db.open()) {
        m_lastError = QString("Can't open database with name \'%1\'").arg(dbName) + db.lastError().text();
        return false;
    }

    m_model->setQuery(script, db);
    bool ret = !m_model->lastError().isValid();

    if ( ret && !QSqlDatabase::database().driver()->hasFeature(QSqlDriver::QuerySize))
		while (m_model->canFetchMore())
			m_model->fetchMore();

    m_isPopulated = ret;
    m_currentRow = -1;
    m_fmodel->setSourceModel(m_model);

    emit afterPopulate();

    return ret;
}


bool SqlDataset::isPopulated()
{
    return m_isPopulated;
}


void SqlDataset::setPopulated(bool b)
{
	m_isPopulated = b;
}


void SqlDataset::reset()
{
    m_isPopulated = false;
    m_model->clear();
    m_currentRow = -1;
    m_lastError = "";
    db.close();
}


void SqlDataset::resetCursor()
{
    m_currentRow = -1;
}


bool SqlDataset::firstRow()
{
	emit(beforeFirst());

	m_currentRow = 0;
    bool ret = rows();

	emit(afterFirst());

	return ret;
}


bool SqlDataset::lastRow()
{
	emit(beforeLast());	

	m_currentRow = m_fmodel->rowCount();
	bool ret = m_fmodel->index(m_currentRow, 0).isValid();

	emit(afterLast());

	return ret;
}


bool SqlDataset::nextRow()
{
	emit(beforeNext());

	m_currentRow++;
    bool ret = m_currentRow < rows();

	emit(afterNext());

	return ret;
}


bool SqlDataset::previousRow()
{
	emit(beforePrevious());

	m_currentRow--;
	bool ret = m_currentRow >= 0;

	emit(afterPrevious());

	return ret;
}


int SqlDataset::currentRow()
{
    return m_currentRow;
}


bool SqlDataset::setCurrentRow(int index)
{
	emit(beforeSeek(index));

	m_currentRow = index;
	bool ret = m_fmodel->index(m_currentRow, 0).isValid();

	emit(afterSeek(index));

	return ret;
}


int SqlDataset::rows()
{
    return m_fmodel->rowCount();
}


int SqlDataset::columns()
{
    if (!m_isPopulated)
        populate();
    return m_fmodel->columnCount();
}


QVariant SqlDataset::value(int index) const
{
    return m_fmodel->data( m_fmodel->index(m_currentRow,index) );
}


QVariant SqlDataset::value(const QString & field) const
{
     return m_fmodel->data( m_fmodel->index(m_currentRow, m_model->record().indexOf(field) ) );
}


QVariant SqlDataset::lookaheadValue(int index) const
{
    return m_currentRow+1 < m_fmodel->rowCount() && index < m_fmodel->columnCount() ?  m_fmodel->data( m_fmodel->index(m_currentRow + 1,index) ) : QVariant::Invalid;
}


QVariant SqlDataset::lookaheadValue(const QString & field) const
{
    return m_currentRow+1 < m_fmodel->rowCount() ?  m_fmodel->data( m_fmodel->index(m_currentRow + 1, m_model->record().indexOf(field) ) ) : QVariant::Invalid;
}


QVariant SqlDataset::lookbackValue(int index) const
{
    return m_currentRow-1 < 0 && index < m_fmodel->columnCount() ?  m_fmodel->data( m_fmodel->index(m_currentRow - 1,index) ) : QVariant::Invalid;
}


QVariant SqlDataset::lookbackValue(const QString & field) const
{
    return m_currentRow-1 < 0  ?  m_fmodel->data( m_fmodel->index(m_currentRow - 1, m_model->record().indexOf(field) ) )  : QVariant::Invalid;
}


QSet<QString> SqlDataset::variables() const
{
    QSet<QString> set;
    set.unite( findVariables(m_queryText) );
    set.unite( findVariables(m_dbhost) );
    set.unite( findVariables(m_dbname) );
    set.unite( findVariables(m_dbuser) );
    set.unite( findVariables(m_dbpasswd) );
    set.unite( findVariables(m_driver) );
    return set;
}


QStringList SqlDataset::drivers() const
{
    return QSqlDatabase::drivers();
}


QString SqlDataset::driver() const
{
    return m_driver;
}


void SqlDataset::setDriver(QString driver)
{
    if (m_driver == driver)
        return;

    m_driver = driver;

    emit driverChanged(m_driver);
    emit changed();
}


QString SqlDataset::dbhost() const
{
    return m_dbhost;
}


void SqlDataset::setdbhost(QString host)
{
    if (m_dbhost == host)
        return;

    m_dbhost = host;

    emit dbhostChanged(m_dbhost);
    emit changed();
}


QString SqlDataset::dbuser() const
{
    return m_dbuser;
}


void SqlDataset::setdbuser(QString user)
{
    if (m_dbuser == user)
        return;

    m_dbuser = user;

    emit dbuserChanged(m_dbuser);
    emit changed();
}


QString SqlDataset::dbname() const
{
    return m_dbname;
}


void SqlDataset::setdbname(QString name)
{
    if (m_dbname == name)
        return;

    m_dbname = name;

    emit dbnameChanged(m_dbname);
    emit changed();
}


QString SqlDataset::dbpasswd() const
{
    return m_dbpasswd;
}


void SqlDataset::setdbpasswd(QString passwd)
{
    if (m_dbpasswd == passwd)
        return;

    m_dbpasswd = passwd;

    emit dbpasswdChanged(m_dbpasswd);
    emit changed();
}




#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(DatasetSQL, SqlDataset)
#endif
