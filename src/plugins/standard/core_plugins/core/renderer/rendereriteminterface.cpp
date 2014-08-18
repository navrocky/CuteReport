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

#include "rendereriteminterface.h"
#include "rendererprocessor.h"
#include "renderer.h"
#include "rendererdata.h"
#include "pageinterface.h"
#include "bandinterface.h"
#include "reportcore.h"
#include "aggregatefunctions.h"
#include "storageinterface.h"

#include <QVariant>
#include <QScriptEngine>
#include <QHash>

using namespace CuteReport;

RendererItemInterface::RendererItemInterface(RendererProcessor *parent) :
    RendererPublicInterface(parent),
    m_processor(parent)
{
}


void RendererItemInterface::setValue(const QString & valueName, const QVariant &value)
{
    m_processor->setValue(valueName, value);
//    m_processor->m_scriptEngine->globalObject().setProperty(valueName, m_processor->m_scriptEngine->newVariant(value));
}



QVariant RendererItemInterface::getValue(const QString & valueName)
{
    return m_processor->getValue(valueName);
//    return m_processor->m_scriptEngine->globalObject().property(valueName).toVariant();
}


QString RendererItemInterface::processString(const QString & string, const QString & delimiterBegin, const QString & delimiterEnd, const CuteReport::BaseItemInterface * item)
{
    return m_processor->processString(string, delimiterBegin, delimiterEnd, item);
}


QString RendererItemInterface::processString(const QString & string, const CuteReport::BaseItemInterface * item)
{
    return m_processor->processString(string, item);
}


int RendererItemInterface::currentPageNumber()
{
    return m_processor->m_currentPageNumber;
}


RendererPublicInterface::PageDrawState RendererItemInterface::currentState()
{
    return (RendererPublicInterface::PageDrawState)m_processor->m_state;
}


QRectF RendererItemInterface::pageFreeSpace()
{
    return m_processor->m_freeSpace;
}


QPointF RendererItemInterface::currentBandDelta()
{
    return m_processor->m_bandDelta;
}


void RendererItemInterface::registerBandToDatasetIteration(const QString & datasetName, const QString & objectName)
{
    m_processor->registerBandToDatasetIteration(datasetName, objectName);
}


void RendererItemInterface::registerBandToDatasetIteration(const QString & datasetName, CuteReport::BandInterface * band)
{
    m_processor->registerBandToDatasetIteration(datasetName, band);
}


void RendererItemInterface::unregisterBandFromDatasetIteration(const QString & datasetName, const QString & objectName)
{
    m_processor->unregisterBandFromDatasetIteration(datasetName, objectName);
}

void RendererItemInterface::unregisterBandFromDatasetIteration(const QString & datasetName, CuteReport::BandInterface * band)
{
    m_processor->unregisterBandFromDatasetIteration(datasetName, band);
}


BandsList RendererItemInterface::bandRegisteredToDataset(const QString & datasetName)
{
    return m_processor->bandRegisteredToDataset(datasetName);
}


CuteReport::DatasetInterface * RendererItemInterface::dataset(const QString & datasetname)
{
    return m_processor->m_data->workingReportCopy->dataset(datasetname);
}


void RendererItemInterface::registerEvaluationString(const QString & string, BaseItemInterface * item)
{
    m_processor->registerEvaluationString(string, item);
}


void RendererItemInterface::resetAggregateFunctions(CuteReport::BandInterface * band)
{
    m_processor->m_aggregateFunctions->resetValuesForBand(band->objectName());
}


void RendererItemInterface::iterateDataset(CuteReport::DatasetInterface * dtst)
{
    // TODO
    Q_UNUSED(dtst)
}

void RendererItemInterface::iterateDataset(const QString & objectName)
{
    // TODO
    Q_UNUSED(objectName)
}

void RendererItemInterface::processBand(CuteReport::BandInterface * band)
{
    // TODO
    Q_UNUSED(band)
}

void RendererItemInterface::processBand(const QString & objectName)
{
    // TODO
    Q_UNUSED(objectName)
}


void RendererItemInterface::createNewPage()
{
    m_processor->createNewRenderingPage();
}


QVariant RendererItemInterface::getStorageObject(const QString &objectUrl)
{
    StorageInterface * storage = m_processor->m_data->workingReportCopy->storageByUrl(objectUrl);
    if (!storage)
        return QVariant(QVariant::Invalid);

    return storage->loadObject(objectUrl);
}


CuteReport::BandInterface * RendererItemInterface::lastProcessedBand()
{
    return m_processor->m_lastProcessedBand;
}


void RendererItemInterface::prepareCurrentPage()
{
    // TODO
}


void RendererItemInterface::postprocessCurrentPage()
{
    // TODO
}

QString RendererItemInterface::moduleName()
{
    return m_processor->m_data->renderer->moduleName();
}


void RendererItemInterface::run()
{
    m_processor->run();
}


//QString RendererItemInterface::preprocessEvaluateString(QString str, const ItemInterface *item)
//{

//}


//qreal RendererItemInterface::calculateAgregateFunction(const QString & functName, const QString & expression, qreal previousValue)
//{
//    return m_processor->calculateAgregateFunction(functName, expression, previousValue)
//}

