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

#ifndef AGGREGATEFUNCTIONS_H
#define AGGREGATEFUNCTIONS_H

#include <QHash>
#include <QPair>

namespace CuteReport {
class DatasetInterface;
class BandInterface;
}


/** functions: sum, min, max, avg, count
 *  format: function(dataset."fieldname" [, precision])
 */


class AggregateFunctions
{
public:
    struct FunctionStruct {
        FunctionStruct()
            : value(0), iterationNumber(0)
        {}
        qreal value;
        int iterationNumber;
    };

    AggregateFunctions();

    void reset();

    /** add new function to precess list, if not exists */
    void addFunction(const QString & bandName, const QString & datasetName, const QString & fieldName, const QString & functionName);

    qreal getValue(const QString & bandName, const QString & datasetName, const QString & fieldName, const QString & functionName, bool *error = 0);

    void findAndRegisterAggregateFunctions(const QString & strIn, const CuteReport::BandInterface *band);

    void processDatasetIteration(CuteReport::DatasetInterface * dataset);
    QString replaceWithRealValues(const QString & strIn, const CuteReport::BandInterface *band);

    void resetValuesForDataset(const QString & datasetName);
    void resetValuesForBand(const QString & bandName, bool delayedTillNextRecursion = true);
//    void clear();

private:
    qreal getValue(const QString & funcName, FunctionStruct & funcStruct);
    void accumulataValue(const QString & funcName, FunctionStruct & funcStruct, qreal value);

    typedef QHash<QString, FunctionStruct> FunctionData; // functionName, data
    typedef QHash<QString, FunctionStruct>::iterator FieldFunctionsIterator;

    typedef QHash<QString, FunctionData> FieldData; // fieldName, data
    typedef QHash<QString, FunctionData>::iterator  FieldDataIterator;

    typedef QHash<QString, FieldData> DatasetData;  // datasetName, DatasetFields
    typedef QHash<QString, FieldData>::iterator DatasetDataIterator;

    typedef QHash<QString, DatasetData> BandData; // bandName, DatasetFields
    typedef QHash<QString, DatasetData>::iterator BandDataIterator;

    BandData m_data;
    QList<QString> m_delayedBandReset;
};

#endif // AGGREGATEFUNCTIONS_H
