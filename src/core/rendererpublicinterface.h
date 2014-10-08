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
#ifndef RENDERERTOITEMINTERFACE_H
#define RENDERERTOITEMINTERFACE_H

#include <QObject>
#include <QRectF>
#include "globals.h"
#include "types.h"

namespace CuteReport
{

class BaseItemInterface;
class BandInterface;
class DatasetInterface;
class StorageInterface;
class FormInterface;
class RenderedPageInterface;
class RenderedItemInterface;

class CUTEREPORT_EXPORTS RendererPublicInterface : public QObject
{
    Q_OBJECT
public:
    enum PageDrawState {EmptyPage = 0, BackgroundDone = 1, HeadersDone = 2, FootersDone = 3, DrawingContent = 4, ContentDone = 5, ForegroundDone = 6};

    explicit RendererPublicInterface(QObject *parent = 0);
    
    /** set/read global variable accessible to any item */
    Q_INVOKABLE virtual void setValue(const QString & valueName, const QVariant & value) = 0;
    Q_INVOKABLE virtual QVariant getValue(const QString & valueName) = 0;

    Q_INVOKABLE virtual QString processString(const QString & string, const QString & delimiterBegin, const QString & delimiterEnd, const CuteReport::BaseItemInterface * item ) = 0;
    Q_INVOKABLE virtual QString processString(const QString & string, const CuteReport::BaseItemInterface * item ) = 0;

    Q_INVOKABLE virtual int currentPageNumber() = 0; // 1,2,3...
    Q_INVOKABLE virtual PageDrawState currentState() = 0;

    Q_INVOKABLE virtual QRectF pageFreeSpace() = 0;
    Q_INVOKABLE virtual QPointF currentBandDelta() = 0; // mm

    /** manage bands that will be processed by renderer for each dataset iteration */
    Q_INVOKABLE virtual void registerBandToDatasetIteration(const QString &datasetName, const QString & objectName) = 0;
    Q_INVOKABLE virtual void registerBandToDatasetIteration(const QString & datasetName, CuteReport::BandInterface * band) = 0;
    Q_INVOKABLE virtual void unregisterBandFromDatasetIteration(const QString & datasetName, const QString & objectName) = 0;
    Q_INVOKABLE virtual void unregisterBandFromDatasetIteration(const QString &datasetName, CuteReport::BandInterface * band) = 0;
    Q_INVOKABLE virtual BandsList bandRegisteredToDataset(const QString & datasetName) = 0;

    /** Register strings that have to be processed before printing in some kind
     *  All sctring that have scripting, functions, etc should be registered before rendering
     *  For method without delimiter entire string is evaluation
    */
    Q_INVOKABLE virtual void registerEvaluationString(const QString & string, const QString & delimiterBegin, const QString & delimiterEnd, CuteReport::BaseItemInterface *item) = 0;
    Q_INVOKABLE virtual void registerEvaluationString(const QString & string, CuteReport::BaseItemInterface*) = 0;

    /** method for clearing all accumulated aggregate values at the end of current iteration
     *  keep in mind values will not be reset immediately, so they will be accessable till iteration changed */
    Q_INVOKABLE virtual void resetAggregateFunctions(CuteReport::BandInterface * band) = 0;

    Q_INVOKABLE virtual CuteReport::DatasetInterface * dataset(const QString &datasetname) = 0;
    Q_INVOKABLE virtual void iterateDataset(CuteReport::DatasetInterface * dtst) = 0;
    Q_INVOKABLE virtual void iterateDataset(const QString & objectName) = 0;

    Q_INVOKABLE virtual void processBand(CuteReport::BandInterface * band) = 0;
    Q_INVOKABLE virtual void processBand(const QString & objectName) = 0;

    Q_INVOKABLE virtual void createNewPage() = 0;

    Q_INVOKABLE virtual QVariant getStorageObject(const QString & objectUrl) = 0;

    Q_INVOKABLE virtual BandInterface * lastProcessedBand() = 0;
    Q_INVOKABLE virtual CuteReport::BandInterface * currentProcessingBand() = 0;
    Q_INVOKABLE virtual void prepareCurrentPage() = 0;
    Q_INVOKABLE virtual void postprocessCurrentPage() = 0;
    Q_INVOKABLE virtual quint32 lastProcessedItemId(const QString & itemName) = 0;
    Q_INVOKABLE virtual CuteReport::RenderedItemInterface * lastProcessedItemPointer(const QString & itemName) = 0;

    Q_INVOKABLE virtual void error(const QString & sender, const QString & errorMessage) = 0;

    Q_INVOKABLE virtual QString moduleName() = 0;

    Q_INVOKABLE virtual void run() = 0;

signals:
    void reportStart();
    void bandBefore(CuteReport::BandInterface * band);
    void bandAfter(CuteReport::BandInterface * band);
    void bandGeometryAfter(CuteReport::BandInterface * band);
    void itemBefore(CuteReport::BaseItemInterface * item);
    void itemAfter(CuteReport::BaseItemInterface * item);
    void itemGeometryAfter(CuteReport::BaseItemInterface * item);
    void datasetBefore(CuteReport::DatasetInterface * dataset);
    void datasetAfter(CuteReport::DatasetInterface * dataset);
    void datasetIteration(CuteReport::DatasetInterface * dataset);
    void pageBefore(CuteReport::RenderedPageInterface * page);
    void pageAfter(CuteReport::RenderedPageInterface * page);
    void formBefore(CuteReport::FormInterface * dataset);
    void formAfter(CuteReport::FormInterface * dataset);
    void reportDone();
};

}

#endif // RENDERERTOITEMINTERFACE_H
