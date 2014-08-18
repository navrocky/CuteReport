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

#include "aggregatefunctions.h"
#include "bandinterface.h"
#include "datasetinterface.h"

const QString regExp1("(sum|min|max|avg)\\s*\\(\\s*(\\w+)\\.value\\(\\\"(\\w+)\\\"\\s*\\)\\s*(,\\s*(\\d))?\\s*\\)");
const QString regExp2("(count)\\s*\\(\\s*(\\w+)\\s*\\)");

AggregateFunctions::AggregateFunctions()
{
}


void AggregateFunctions::reset()
{
    m_delayedBandReset.clear();
    m_data.clear();
}


void AggregateFunctions::addFunction(const QString &bandName, const QString &datasetName, const QString &fieldName, const QString &functionName)
{
    BandDataIterator bandDataIt = m_data.find(bandName);
    if (bandDataIt == m_data.end()) {
        DatasetData d;
        bandDataIt = m_data.insert(bandName, d);
    }

    DatasetDataIterator datasetDataIt = bandDataIt.value().find(datasetName);
    if (datasetDataIt == bandDataIt.value().end()) {
        FieldData d;
        datasetDataIt = bandDataIt.value().insert(datasetName, d);
    }

    FieldDataIterator fieldDataIt = datasetDataIt.value().find(fieldName);
    if (fieldDataIt == datasetDataIt.value().end()) {
        FunctionData d;
        fieldDataIt = datasetDataIt.value().insert(fieldName, d);
    }

    FieldFunctionsIterator funcIt = fieldDataIt.value().find(functionName);
    if (funcIt == fieldDataIt.value().end()) {
        FunctionStruct s;
        fieldDataIt.value().insert(functionName, s);
    }
}


qreal AggregateFunctions::getValue(const QString &bandName, const QString &datasetName, const QString &fieldName, const QString &functionName, bool * error)
{
    BandDataIterator bandDataIt = m_data.find(bandName);
    if (bandDataIt != m_data.end()) {
        DatasetDataIterator datasetDataIt = bandDataIt.value().find(datasetName);
        if (datasetDataIt != bandDataIt.value().end()) {
            FieldDataIterator fieldDataIt = datasetDataIt.value().find(fieldName);
            if (fieldDataIt != datasetDataIt.value().end()) {
                FieldFunctionsIterator funcIt = fieldDataIt.value().find(functionName);
                if (funcIt != fieldDataIt.value().end()) {
                    FunctionStruct & s = funcIt.value();
                    return getValue(funcIt.key(), s);
                }
            }
        }
    }

    if (error)
        *error = true;
    return 0.0;
}


void AggregateFunctions::findAndRegisterAggregateFunctions(const QString &strIn, const CuteReport::BandInterface* band)
{
    if (!band)
        return;

    QString strOut = strIn;

    QRegExp reField(regExp1, Qt::CaseInsensitive);
    reField.setMinimal(false);
    int pos = 0;

    while ((pos = reField.indexIn(strOut, pos)) != -1) {
        int length = reField.matchedLength();

        QString funcName = reField.cap(1);
        QString datasetName = reField.cap(2);
        QString fieldName = reField.cap(3);

        addFunction(band->objectName(), datasetName, fieldName, funcName);

        pos += length;
    }

    reField.setPattern(regExp2);
    pos = 0;
    while ((pos = reField.indexIn(strOut, pos)) != -1) {
        int length = reField.matchedLength();

        QString funcName = reField.cap(1);
        QString datasetName = reField.cap(2);
        QString fieldName = reField.cap(3);

        addFunction(band->objectName(), datasetName, fieldName, funcName);

        pos += length;
    }

}


void AggregateFunctions::processDatasetIteration(CuteReport::DatasetInterface *dataset)
{
    foreach (const QString & bandName, m_delayedBandReset) {
        resetValuesForBand(bandName, false);
    }
    m_delayedBandReset.clear();

    BandDataIterator i;
    for (i = m_data.begin(); i != m_data.end(); ++i) {
        DatasetDataIterator dsIt= i.value().find(dataset->objectName());
        if (dsIt != i.value().end()) {
            FieldDataIterator fIt;
            for (fIt = dsIt.value().begin(); fIt != dsIt.value().end(); fIt++) {
                FieldFunctionsIterator funcIt;
                for (funcIt = fIt.value().begin(); funcIt != fIt.value().end(); funcIt++) {
                    FunctionStruct & s = funcIt.value();
                    accumulataValue(funcIt.key(), s, dataset->value(fIt.key()).toReal());
                }
            }
            break;
        }
    }
}

/// looking for aggregate functions and replace it with real value
/// name of aggregate function may be the same with other standard javascript function
/// so checking carefully with exact format func(datasetName.value("fieldName")

QString AggregateFunctions::replaceWithRealValues(const QString &strIn, const CuteReport::BandInterface* band)
{
    if (!band)
        return strIn;

    QString strOut = strIn;

    QRegExp reField(regExp1, Qt::CaseInsensitive);
    reField.setMinimal(false);
    int pos = 0;

    while ((pos = reField.indexIn(strOut, pos)) != -1) {
        int length = reField.matchedLength();

        QString funcName = reField.cap(1);
        QString datasetName = reField.cap(2);
        QString fieldName = reField.cap(3);
        QString precision = reField.cap(5);

        bool ok = true;
        qreal num = getValue(band->objectName(), datasetName, fieldName, funcName, &ok);
        if (ok) {
            QString result;
            if (precision.isEmpty())
                result = QString::number(num);
            else {
                int prec = precision.toInt();
                result = QString::number(num, 'f', prec);
            }
            strOut.replace(pos, length, result);
            pos += result.length() ;
        } else {
            pos += length;
        }
    }

    reField.setPattern(regExp2);
    pos = 0;
    while ((pos = reField.indexIn(strOut, pos)) != -1) {
        int length = reField.matchedLength();

        QString funcName = reField.cap(1);
        QString datasetName = reField.cap(2);
        QString fieldName = reField.cap(3);

        bool ok = true;
        qreal num = getValue(band->objectName(), datasetName, fieldName, funcName, &ok);
        if (ok) {
            QString result = QString::number(num);
            strOut.replace(pos, length, result);
            pos += result.length() ;
        } else {
            pos += length;
        }

    }

    return strOut;
}


void AggregateFunctions::resetValuesForDataset(const QString &datasetName)
{
    BandDataIterator i;
    for (i = m_data.begin(); i != m_data.end(); ++i) {
        DatasetDataIterator dsIt= i.value().find(datasetName);
        if (dsIt != i.value().end()) {
            FieldDataIterator fIt;
            for (fIt = dsIt.value().begin(); fIt != dsIt.value().end(); fIt++) {
                FieldFunctionsIterator funcIt;
                for (funcIt = fIt.value().begin(); funcIt != fIt.value().end(); funcIt++) {
                    FunctionStruct & s = funcIt.value();
                    s.value = 0;
                    s.iterationNumber = 0;
                }
            }
            break;
        }
    }
}


void AggregateFunctions::resetValuesForBand(const QString &bandName, bool delayedTillNextRecursion)
{
    if (delayedTillNextRecursion) {
        m_delayedBandReset.append(bandName);
        return;
    }

    BandDataIterator i = m_data.find(bandName);
    if (i == m_data.end())
        return;

    DatasetDataIterator dsIt;
    for (dsIt = i.value().begin(); dsIt != i.value().end(); ++dsIt) {
        if (dsIt != i.value().end()) {
            FieldDataIterator fIt;
            for (fIt = dsIt.value().begin(); fIt != dsIt.value().end(); fIt++) {
                FieldFunctionsIterator funcIt;
                for (funcIt = fIt.value().begin(); funcIt != fIt.value().end(); funcIt++) {
                    FunctionStruct & s = funcIt.value();
                    s.value = 0;
                    s.iterationNumber = 0;
                }
            }
        }
    }
}


qreal AggregateFunctions::getValue(const QString & funcName, FunctionStruct & funcStruct)
{
    if (funcName == "sum") {
        return funcStruct.value ;
    } else if (funcName  == "avg") {
        return (funcStruct.value / funcStruct.iterationNumber);
    } else if (funcName  == "min") {
        return funcStruct.value;
    } else if (funcName  == "max") {
        return funcStruct.value;
    } else if (funcName == "count")
        return funcStruct.iterationNumber;

    return 0.0;
}


void AggregateFunctions::accumulataValue(const QString & funcName, FunctionStruct & funcStruct, qreal value)
{
    if (funcName == "sum") {
        funcStruct.value += value;
    } else if (funcName  == "avg") {
        funcStruct.value += value;
    } else if (funcName  == "min") {
        if (funcStruct.iterationNumber == 0)
            funcStruct.value += value;
        else
            funcStruct.value = qMin(funcStruct.value, value);
    } else if (funcName  == "max") {
        if (funcStruct.iterationNumber == 0)
            funcStruct.value += value;
        else
            funcStruct.value = qMax(funcStruct.value, value);
    }

    funcStruct.iterationNumber++;
}
