/***************************************************************************
 *   This file is part of the CuteReport project                           *
 *   Copyright (C) 2013 by Alexander Mikhalov                              *
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
#ifndef CSVDATASET_H
#define CSVDATASET_H

#include <datasetinterface.h>
#include <QtSql>
#include <QSortFilterProxyModel>
#include <reportinterface.h>
#include "globals.h"

class Model;

class CsvDataset : public CuteReport::DatasetInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteReport.DatasetInterface/1.0")
#endif
    Q_INTERFACES(CuteReport::DatasetInterface)

    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(QString delimeter READ delimeter WRITE setDelimeter NOTIFY delimiterChanged)
    Q_PROPERTY(bool keepDataInternal READ keepData WRITE setKeepData NOTIFY keepDataInternalChanged)
    Q_PROPERTY(bool firstRowIsHeader READ firstRowIsHeader WRITE setFirstRowIsHeader NOTIFY firstRowIsHeaderChanged)
    Q_PROPERTY(QStringList internalData READ list WRITE setList DESIGNABLE false)

public:
    explicit CsvDataset(QObject *parent = 0);
    virtual ~CsvDataset();

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
    virtual QVariant::Type fieldType(int column );

    QString	fileName() const;
    void setFileName(const QString &str);
    QStringList	list() const;
    void setList(const QStringList &list);
    QString delimeter () const;
    void setDelimeter (const QString &str);
    bool keepData() const;
    void setKeepData(bool value);
    bool firstRowIsHeader();
    void setFirstRowIsHeader(bool value);

    virtual QString moduleShortName() const;
    virtual QString suitName() const { return "Standard"; }

    virtual QSet<QString> variables() const;

signals:
    void fileNameChanged(QString);
    void delimiterChanged(QString);
    void keepDataInternalChanged(bool);
    void firstRowIsHeaderChanged(bool);

private:
    explicit CsvDataset(const CsvDataset &dd, QObject * parent);
    virtual DatasetInterface * objectClone() const;

    bool m_firstRowIsHeader;
    int m_currentRow;
    bool m_isPopulated;
    QString m_fileName;
    QStringList m_list;
    QString m_delimeter;
    bool m_keepData;
    Model * m_model;
    QSortFilterProxyModel * m_fmodel;
    QPointer<CuteReport::DatasetHelperInterface> m_helper;

    QString m_lastError;
};

#endif




/*
#ifndef CSVDATASET_H
#define CSVDATASET_H

#include <dataset.h>
#include <dataseteditor.h>
#include "csvdatasetmodel.h"
#include <QSortFilterProxyModel>

class CsvDataset : public Report::DataSet
{
	Q_OBJECT
	Q_INTERFACES(Report::DataSet);
	Q_PROPERTY(QString fileName READ fileName WRITE setFileName)
	Q_PROPERTY(QStringList list READ list WRITE setList DESIGNABLE false)
	Q_PROPERTY(QString delimeter READ delimeter WRITE setDelimeter)

public:
	CsvDataset(QObject *parent = 0);
	~CsvDataset();

	DataSet * createInstance(QObject* parent = 0);
	DataSetEditor * createEditor();
	QAbstractItemModel * model();
	QString name();
	QString lastError();

	bool first();
	bool last();
	bool next();
	bool previous();
	bool populate();
	bool isPopulated();
	bool seek(int index);
	int size();
	QVariant value(int index) const;
	QVariant value(const QString & field) const;
	QVariant lookaheadValue(int index) const;
	QVariant lookaheadValue(const QString & field) const;
	QVariant lookbackValue(int index) const;
	QVariant lookbackValue(const QString & field) const;
	QString fieldName(int column );

	QString	    fileName();
	void	    setFileName(QString str);
	QStringList	    list();
	void	    setList(QStringList list);
	QString delimeter ();
	void setDelimeter (QString str);

private:
	int m_currentRow;
	bool m_isPopulated;
	QString m_fileName;
	QStringList m_list;
	QString m_delimeter;
	QString m_error;
	Model * m_model;
	QSortFilterProxyModel * m_fmodel;
	DataSetEditor ed;
};
*/

//#endif
