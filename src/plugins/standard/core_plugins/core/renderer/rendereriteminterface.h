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

#ifndef RENDERERITEMINTERFACE_H
#define RENDERERITEMINTERFACE_H

#include "rendererpublicinterface.h"

namespace CuteReport {
class BandInterface;
class DatasetInterface;
}


class RendererProcessor;

class RendererItemInterface : public CuteReport::RendererPublicInterface
{
    Q_OBJECT
public:
    explicit RendererItemInterface(RendererProcessor *parent = 0);
    
    Q_INVOKABLE virtual void setValue(const QString & valueName, const QVariant &value);
    Q_INVOKABLE virtual QVariant getValue(const QString & valueName);

    Q_INVOKABLE virtual QString processString(const QString & string, const QString & delimiterBegin, const QString & delimiterEnd, const CuteReport::BaseItemInterface * item);
    Q_INVOKABLE virtual QString processString(const QString & string, const CuteReport::BaseItemInterface *item);

    Q_INVOKABLE virtual int currentPageNumber();
    Q_INVOKABLE virtual PageDrawState currentState();

    Q_INVOKABLE virtual QRectF pageFreeSpace();
    Q_INVOKABLE virtual QPointF currentBandDelta();

    Q_INVOKABLE virtual void registerBandToDatasetIteration(const QString &datasetName, const QString & objectName);
    Q_INVOKABLE virtual void registerBandToDatasetIteration(const QString & datasetName, CuteReport::BandInterface * band);
    Q_INVOKABLE virtual void unregisterBandFromDatasetIteration(const QString & datasetName, const QString & objectName);
    Q_INVOKABLE virtual void unregisterBandFromDatasetIteration(const QString &datasetName, CuteReport::BandInterface * band);
    Q_INVOKABLE virtual CuteReport::BandsList bandRegisteredToDataset(const QString & datasetName);

    Q_INVOKABLE virtual void registerEvaluationString(const QString & string, const QString & delimiterBegin, const QString & delimiterEnd, CuteReport::BaseItemInterface *item);
    Q_INVOKABLE virtual void registerEvaluationString(const QString & string, CuteReport::BaseItemInterface *item);
    Q_INVOKABLE void resetAggregateFunctions(CuteReport::BandInterface * band);

    Q_INVOKABLE virtual CuteReport::DatasetInterface * dataset(const QString &datasetname);
    Q_INVOKABLE virtual void iterateDataset(CuteReport::DatasetInterface * dtst);
    Q_INVOKABLE virtual void iterateDataset(const QString & objectName);
    Q_INVOKABLE virtual void processBand(CuteReport::BandInterface * band);
    Q_INVOKABLE virtual void processBand(const QString & objectName);
    Q_INVOKABLE virtual void createNewPage();

    Q_INVOKABLE virtual QVariant getStorageObject(const QString & objectUrl);

    Q_INVOKABLE virtual CuteReport::BandInterface * lastProcessedBand();
    Q_INVOKABLE virtual CuteReport::BandInterface * currentProcessingBand();
    Q_INVOKABLE virtual void prepareCurrentPage();
    Q_INVOKABLE virtual void postprocessCurrentPage();
    Q_INVOKABLE virtual quint32 lastProcessedItemId(const QString & itemName);
    Q_INVOKABLE virtual CuteReport::RenderedItemInterface * lastProcessedItemPointer(const QString & itemName);

    Q_INVOKABLE virtual void error(const QString & /*sender*/, const QString & /*errorMessage*/){}

    Q_INVOKABLE QString moduleName();

    Q_INVOKABLE void run();

signals:
    
public slots:
    
private:
//    QString preprocessEvaluateString(QString str, const CuteReport::ItemInterface * item);
//    const CuteReport::BandInterface *getBandForItem(const CuteReport::ItemInterface *item);
//    qreal calculateAgregateFunction(const QString & functName, const QString & expression, qreal previousValue);

    RendererProcessor * m_processor;

    friend class RendererProcessor;
};

#endif // RENDERERITEMINTERFACE_H
