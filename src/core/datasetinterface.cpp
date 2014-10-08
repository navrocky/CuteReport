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
#include <QSortFilterProxyModel>
#include "datasetinterface.h"


namespace CuteReport
{


DatasetInterface::DatasetInterface(QObject *parent)
    : ReportPluginInterface(parent), m_filterColumn(-1)
{
}


DatasetInterface *DatasetInterface::clone() const
{
    DatasetInterface * newDataset = this->objectClone();
    newDataset->setReportCore(this->reportCore());
    return newDataset;
}


QString	DatasetInterface::parentDataset()
{
    return m_parentDataset;
}


void DatasetInterface::setParentDataset(QString pDataset)
{
    m_parentDataset = pDataset;
}


void DatasetInterface::setFilterCondition(QString list)
{
    m_filterCondition = list;
}


QString DatasetInterface::filterCondition()
{
    return m_filterCondition;
}


int DatasetInterface::filterColumn()
{
    return m_filterColumn;
}


void DatasetInterface::setFilterColumn(int col)
{
    m_filterColumn = col;
}


QVariant DatasetInterface::value(int /*index*/) const{return QVariant();}
QVariant DatasetInterface::value(const QString & /*field*/) const{return QVariant();}
QVariant DatasetInterface::lookaheadValue(int /*index*/) const{return QVariant();}
QVariant DatasetInterface::lookaheadValue(const QString & /*field*/) const{return QVariant();}
QVariant DatasetInterface::lookbackValue(int /*index*/) const{return QVariant();}
QVariant DatasetInterface::lookbackValue(const QString & /*field*/) const{return QVariant();}
QAbstractItemModel * DatasetInterface::model() {return 0;}
QString DatasetInterface::lastError() {return QString();}
QString DatasetInterface::fieldName(int /*column*/ ) {return tr("Unknown");}


void DatasetInterface::setFilter ( const int col, const QString & str, Qt::CaseSensitivity cs)
{
    emit beforeSetFilter (col, str);
    QSortFilterProxyModel* _model = dynamic_cast<QSortFilterProxyModel*>( model() );
    if (_model ) {
        //	_model->setFilterRegExp (regExp);
        _model->setFilterFixedString(str);
        _model->setFilterCaseSensitivity(cs);
        _model->setFilterKeyColumn(col);
    } else
        qWarning("WARNING: please reimplement setFilter () in plugin \'%s\'", this->metaObject()->className());

    emit afterSetFilter (col, str);

}


} // namespace



