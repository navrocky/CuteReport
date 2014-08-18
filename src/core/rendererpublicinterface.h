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

class CUTEREPORT_EXPORTS RendererPublicInterface : public QObject
{
    Q_OBJECT
public:
    enum PageDrawState {EmptyPage = 0, BackgroundDone = 1, HeadersDone = 2, FootersDone = 3, DrawingContent = 4, ContentDone = 5, ForegroundDone = 6};

    explicit RendererPublicInterface(QObject *parent = 0);
    
    /** set/read global variable accessible to any item */
    virtual void setValue(const QString & valueName, const QVariant & value) = 0;
    virtual QVariant getValue(const QString & valueName) = 0;

    virtual QString processString(const QString & string, const QString & delimiterBegin, const QString & delimiterEnd, const CuteReport::BaseItemInterface * item ) = 0;
    virtual QString processString(const QString & string, const CuteReport::BaseItemInterface * item ) = 0;

    virtual int currentPageNumber() = 0; // 1,2,3...
    virtual PageDrawState currentState() = 0;

    virtual QRectF pageFreeSpace() = 0;
    virtual QPointF currentBandDelta() = 0; // mm

    /** manage bands that will be processed by renderer for each dataset iteration */
    virtual void registerBandToDatasetIteration(const QString &datasetName, const QString & objectName) = 0;
    virtual void registerBandToDatasetIteration(const QString & datasetName, CuteReport::BandInterface * band) = 0;
    virtual void unregisterBandFromDatasetIteration(const QString & datasetName, const QString & objectName) = 0;
    virtual void unregisterBandFromDatasetIteration(const QString &datasetName, CuteReport::BandInterface * band) = 0;
    virtual BandsList bandRegisteredToDataset(const QString & datasetName) = 0;

    /** Register strings that have to be processed before printing in some kind
     *  All sctring that have scripting, functions, etc should be registered before rendering
    */
    virtual void registerEvaluationString(const QString & string, CuteReport::BaseItemInterface*) = 0;

    /** method for clearing all accumulated aggregate values at the end of current iteration
     *  keep in mind values will not be reset immediately, so they will be accessable till iteration changed */
    virtual void resetAggregateFunctions(CuteReport::BandInterface * band) = 0;

    virtual CuteReport::DatasetInterface * dataset(const QString &datasetname) = 0;
    virtual void iterateDataset(CuteReport::DatasetInterface * dtst) = 0;
    virtual void iterateDataset(const QString & objectName) = 0;

    virtual void processBand(CuteReport::BandInterface * band) = 0;
    virtual void processBand(const QString & objectName) = 0;

    virtual void createNewPage() = 0;

    virtual QVariant getStorageObject(const QString & objectUrl) = 0;

    virtual BandInterface * lastProcessedBand() = 0;
    virtual void prepareCurrentPage() = 0;
    virtual void postprocessCurrentPage() = 0;

    virtual void error(const QString & sender, const QString & errorMessage) = 0;

signals:
    void onBandBefore(CuteReport::BandInterface * band);
    void onBandAfter(CuteReport::BandInterface * band);
    void onBandGemetryAfter(CuteReport::BandInterface * band);
    void onItemBefore(CuteReport::BaseItemInterface * item);
    void onItemAfter(CuteReport::BaseItemInterface * item);
    void onItemGeometryAfter(CuteReport::BaseItemInterface * item);
    void onDatasetBefore(CuteReport::DatasetInterface * dataset);
    void onDatasetAfter(CuteReport::DatasetInterface * dataset);
    void onDatasetIteration(CuteReport::DatasetInterface * dataset);
    void onPageBefore(CuteReport::RenderedPageInterface * page);
    void onPageAfter(CuteReport::RenderedPageInterface * page);
    void onFormBefore(CuteReport::FormInterface * dataset);
    void onFormAfter(CuteReport::FormInterface * dataset);
};

}

#endif // RENDERERTOITEMINTERFACE_H

/*
    virtual void prepareCurrentPage();
    virtual void postprocessCurrentPage();
    virtual int currentPageNumber();
    virtual int currentDatasetRow();
    virtual QString currentDatasetName();
    virtual CuteReport::DataSet * currentDataset();
    virtual int currentDetailNumber();
    virtual void setDetailNumber(int num);
    virtual void newPage();
    virtual void processBand(BandInterface * band);
//    virtual void prepareDatasets();
    virtual void processDataset(DataSet * dtst);
    virtual QRectF pageFreeSpace();
    virtual void setPageFreeSpace (QRectF rect);
    virtual BandInterface * lastProcessedBand();
    */
