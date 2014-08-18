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

#include "rendererprocessor.h"
#include "rendererdata.h"
#include "renderer.h"
#include "datasetinterface.h"
#include "forminterface.h"
#include "iteminterface.h"
#include "renderediteminterface.h"
#include "bandinterface.h"
#include "scriptextensioninterface.h"
#include "reportcore.h"
#include "limits"
#include "aggregatefunctions.h"
#include "functions.h"

#include <QScriptEngine>

using namespace CuteReport;

static bool cmpBands(BandInterface * a, BandInterface * b)
{
    if (a->layoutPriority() == b->layoutPriority())
        return a->order() < b->order();
    else
        return a->layoutPriority() < b->layoutPriority();
}


bool cmpBandsDesc(BandInterface * a, BandInterface * b)
{
    if (a->layoutPriority() == b->layoutPriority())
        return a->order() < b->order();
    else
        return a->layoutPriority() > b->layoutPriority();
}


Thread::Thread()
{
}

Thread::~Thread() {
    ReportCore::log(LogDebug, "Renderer::Thread", "Rendering Thread successfuly destroyed");
}

void Thread::stop() {
//    this->exit();
//    this->terminate();
//    this->wait();
//    this->deleteLater();
}


RendererProcessor::RendererProcessor(RendererData * data)
    :QObject(data->renderer),
      m_data(data),
      m_rendererItemInterface(0),
      m_terminate(false),
      m_currentRenderedPage(0),
      m_runs(false)
{
    m_antialiasing = data->renderer->antialiasing();
    m_textAntialiasing = data->renderer->textAntialiasing();
    m_smoothPixmapTransform = data->renderer->smoothPixmapTransform();
    m_dpi = data->renderer->dpi() ? data->renderer->dpi() : 300;
    m_delay = data->renderer->delay();

    QByteArray reportData = m_data->renderer->reportCore()->serialize(m_data->origReport);

    m_data->workingReportCopy = dynamic_cast<CuteReport::ReportInterface*>(m_data->renderer->reportCore()->deserialize(reportData));

    Q_ASSERT(m_data->workingReportCopy);
    if (m_data->workingReportCopy) {
        m_data->workingReportCopy->setParent(this);
        m_data->workingReportCopy->init();
    }

    m_aggregateFunctions = new AggregateFunctions();
}


RendererProcessor::~RendererProcessor()
{
    emit log(CuteReport::LogDebug, "RendererProcessor DTOR", "");

    //    if (m_data->processorThread) {
    //        m_data->processorThread->quit();
    //        m_data->processorThread->wait();
    //        m_data->processorThread->deleteLater();
    //    }
    if (m_currentRenderedPage)
        delete m_currentRenderedPage;
    delete m_aggregateFunctions;
}


void RendererProcessor::start()
{
    Thread * renderThread = new Thread();
    renderThread->setParent(m_data->renderer);

    m_data->processor = this;
    m_data->processorThread = renderThread;

    this->setParent(0);
    moveToThread(renderThread);

    emit log(CuteReport::LogDebug, QString("start thread id: %1  processor thread id: %2").arg((long)QThread::currentThread()).arg((long)renderThread), "");

    connect(renderThread, SIGNAL(started()), this, SLOT(_run()));
    connect(renderThread, SIGNAL(finished()), renderThread, SLOT(deleteLater()));
//    connect(this, SIGNAL(destroyed()), renderThread, SLOT(stop()), Qt::QueuedConnection);

    renderThread->start();
}


void RendererProcessor::terminate()
{
    QMutexLocker locker(&mutex);
    m_terminate = true;
}


void RendererProcessor::_run()
{
    initScriptEngine();

    bool resultOk = evaluateScript();

    if (resultOk && !m_data->workingReportCopy->script().contains("renderer.run")) {
        makeAutomaticConnections();
        m_data->scriptEngine->evaluate("renderer.run();");
        if (m_data->scriptEngine->hasUncaughtException()) {
            QString _message = QString("error evaluating renderer.run %1")
                               .arg(m_data->scriptEngine->uncaughtException().toString());
            qWarning() << _message;
            m_data->appendError(_message);
            resultOk = false;
        }
    }

    if (!resultOk && !m_runs) { // sometimes script containd error after renderer started.
        _done(false);
    }

    m_runs = false;
}


void RendererProcessor::_done(bool success)
{
    emit log(CuteReport::LogDebug, QString("_done"), "");
    m_data->dpi = m_dpi;
    resetScriptEngine();
    this->deleteLater();
    thread()->quit();
    emit done(success);
}


void RendererProcessor::run()
{
    m_runs = true;

    emit started();
    emit log(CuteReport::LogDebug, "started", "");
    emit log(CuteReport::LogDebug, QString("run thread id %1").arg((long)QThread::currentThread()), "");

    if (!m_data->workingReportCopy->pages().size())
        emit log(LogError, "report has no pages",  QString("report with name %1 has no pages").arg(m_data->workingReportCopy->objectName()));

    QList<PageInterface *> pages = m_data->workingReportCopy->pages();
    for (int i=0; i<pages.count() && !terminated(); ++i) {
        CuteReport::PageInterface * page = pages[i];
        emit log(LogInfo, QString("rendering page: %1").arg(page->objectName()), "");
        renderReportPage(page);
    }

    resetData();

    if (terminated()) {
        emit log(LogInfo, QString("terminated"), "");
        _done(false);
    } else
        _done(true);
}


void RendererProcessor::initScriptEngine()
{
    Q_ASSERT(m_data);
    m_data->scriptEngine = new QScriptEngine(this);
    QScriptEngine * scriptEngine = m_data->scriptEngine;

    ReportInterface * report = m_data->workingReportCopy;

    m_rendererItemInterface = new RendererItemInterface(this);
    scriptEngine->globalObject().setProperty("renderer", scriptEngine->newQObject(m_rendererItemInterface), QScriptValue::Undeletable);

    /// registering script extensions
    foreach (ScriptExtensionInterface * extension, m_data->renderer->reportCore()->scriptExtensionModules() ) {
        foreach (const QString & key, extension->keys())
            extension->initialize(key, scriptEngine);
    }

    /// registering additional metatypes
    qScriptRegisterQObjectMetaType<DatasetInterface*>(scriptEngine);

    /// adding datasets
    foreach(CuteReport::DatasetInterface * dtst, report->datasets()) {
        emit log(CuteReport::LogDebug, QString("Preparing dataset \'%1\'").arg(dtst->objectName()), "");
        scriptEngine->globalObject().setProperty(dtst->objectName(), scriptEngine->newQObject(dtst), QScriptValue::ReadOnly);
    }

    /// adding forms
    foreach(CuteReport::FormInterface * form, report->forms()){
        form->renderInit();
        scriptEngine->globalObject().setProperty(form->objectName(), scriptEngine->newQObject(form), QScriptValue::ReadOnly);
    }

    /// adding items
    QSet<QString> initedItems;
    foreach(CuteReport::PageInterface * page, report->pages()) {
        foreach(CuteReport::BaseItemInterface * item, page->items()) {
            scriptEngine->globalObject().setProperty(item->objectName(), scriptEngine->newQObject(item), QScriptValue::ReadOnly);
            if (!initedItems.contains(item->moduleName())) {
                item->initScript(scriptEngine);
                initedItems.insert(item->moduleName());
            }
        }
    }

    /// install default values
    setValue("_page", 0, QScriptValue::ReadOnly);
    setValue("_line", 0, QScriptValue::ReadOnly);
}


void RendererProcessor::makeAutomaticConnections()
{
    /*
    /// make automatic connections
    /// parse function on_OBJECTNAME_SIGNALNAME()

    QString str = m_data->workingReportCopy->script();
    QRegExp rx("function\\s+on_(\\w+)\\_(\\w+)\\s*\\((.*)\\)\\s*\\{");
    rx.setMinimal(true);
    int pos = 0;

    QString objectName;
    QString signalName;
    QString parameters;
    QString evaluateString;
    QScriptValue globalObject = m_data->scriptEngine->globalObject();

    while ((pos = rx.indexIn(str, pos)) != -1) {
        objectName = rx.cap(1);
        signalName = rx.cap(2);
        parameters = rx.cap(3);



//        m_data->scriptEngine->evaluate("renderer.run();");
//        evaluateString = QString("%1.%2.connect(on_%3_%4);")
//        m_data->scriptEngine->evaluate("renderer.run();");
//        if (m_data->scriptEngine->hasUncaughtException()) {
//            emit log(CuteReport::LogError, "Error evaluating renderer.run", m_data->scriptEngine->uncaughtException().toString());
//        }
        pos += rx.matchedLength();
    }

    m_data->scriptEngine;

//    memoitem.rendering.connect(memo_test);
*/
}


void RendererProcessor::resetScriptEngine()
{
    ReportInterface * report = m_data->workingReportCopy;
    foreach(CuteReport::FormInterface * form, report->forms()){
        form->renderReset();
    }
}


bool RendererProcessor::evaluateScript()
{
    m_data->scriptEngine->evaluate( setVariablesValue( m_data->origReport->script(), m_data->workingReportCopy->variables() ) );

    if (m_data->scriptEngine->hasUncaughtException()) {
        QString _message = QString("script error at line %1 \n%2")
                           .arg(m_data->scriptEngine->uncaughtExceptionLineNumber())
                           .arg(m_data->scriptEngine->uncaughtException().toString());
        m_data->appendError(_message);
        emit log(LogError, QString("Report's script evaluation error"), QString("error message: %1").arg(_message));
        return false;
    }
    return true;
}


void RendererProcessor::resetData()
{
    foreach (DatasetInterface * ds, m_data->workingReportCopy->datasets())
        ds->reset();

    foreach (PageInterface * page, m_data->workingReportCopy->pages())
        foreach (BaseItemInterface * item, page->items())
            item->renderReset();
}


void RendererProcessor::renderReportPage(CuteReport::PageInterface * page)
{
    emit log(CuteReport::LogDebug, QString("renderReportPage: %1").arg(page->objectName()), "");

    if (m_dpi > 0)
        page->setDpi(m_dpi);

    topBands.clear();
    bottomBands.clear();
    freeBands.clear();

    m_currentPage = page;
    m_currentDatasetLine=0;
    m_currentPageNumber = 0;
    setValue("_page", m_currentPageNumber, QScriptValue::ReadOnly);
    setValue("_line", m_currentDatasetLine+0, QScriptValue::ReadOnly);

    QList <BaseItemInterface*> items = page->items();
    for (int i=0; i<items.count() && !terminated(); ++i) {
        CuteReport::BaseItemInterface * item = items.at(i);
        CuteReport::BandInterface* band = qobject_cast<CuteReport::BandInterface*>(item);
        if (!band)
            continue;

        if (band->layoutType() == CuteReport::BandInterface::LayoutTop)
            topBands.append(band);
        if (band->layoutType() == CuteReport::BandInterface::LayoutBottom)
            bottomBands.append(band);
        if (band->layoutType() == CuteReport::BandInterface::LayoutFree)
            freeBands.append(band);
    }

    if (terminated())
        return;

    qSort(topBands.begin(), topBands.end(), cmpBandsDesc);
    qSort(bottomBands.begin(), bottomBands.end(), cmpBandsDesc);

    initBands(topBands);
    initBands(bottomBands);
    initBands(freeBands);

    createNewRenderingPage();

    for (int i=0; i<topBands.count() && !terminated(); ++i) {
        BandInterface * band = topBands.at(i);
        emit log(CuteReport::LogDebug, QString("checking band: %1").arg(band->objectName()), "");
        if (terminated())
            return;

        if (m_bandsDone.contains(band)) //already processed in group iterations
            continue;

        DatasetInterface * dataset = datasetRegisteredTo(band);
        if (dataset)
            processDataset(dataset);
        else
            processBand(band);
    }

    m_state = RendererPublicInterface::ContentDone;

    if (m_currentRenderedPage) {
        completePage(m_currentRenderedPage);
    }
}


void RendererProcessor::initBands(QList<CuteReport::BandInterface*> bands)
{
    foreach (CuteReport::BandInterface * band, bands) {
        band->renderInit(m_rendererItemInterface);
        foreach (CuteReport::BaseItemInterface * const child, band->findChildren<CuteReport::BaseItemInterface *>())
            child->renderInit(m_rendererItemInterface);
    }
}


void RendererProcessor::createNewRenderingPage()
{
    if (m_currentRenderedPage) {
        completePage(m_currentRenderedPage);
    }

    if (terminated())
        return;

    m_state = RendererPublicInterface::EmptyPage;

    ++m_currentPageNumber;
    setValue("_page", m_currentPageNumber, QScriptValue::ReadOnly);

    m_currentRenderedPage = m_currentPage->render();

    m_freeSpace = m_currentPage->pageRect();
    m_zValue = 0;

    emit processingPage(m_currentPageNumber, 0);
    emit m_rendererItemInterface->onPageBefore(m_currentRenderedPage);

    RenderedItemInterface * renderedView;

    foreach(BandInterface * band, freeBands) {   //process listFree first if it want paint on background
        if ((renderedView = band->renderNewPage()))
            processBand(band, renderedView);
    }

    m_state = RendererPublicInterface::BackgroundDone;

    foreach(BandInterface * band, topBands) {
        if ((renderedView = band->renderNewPage()))
            processBand(band, renderedView);
    }

    m_state = RendererPublicInterface::HeadersDone;

    for (int i = bottomBands.count()-1; i>=0 ;--i) {
        BandInterface * band = bottomBands.at(i);
        if ((renderedView = band->renderNewPage()))
            processBand(band, renderedView);
    }

    m_state = RendererPublicInterface::FootersDone;
}


void RendererProcessor::completePage(RenderedPageInterface* page)
{
    emit m_rendererItemInterface->onPageAfter(m_currentRenderedPage);

    RenderedItemInterface * renderedView;
    foreach(BandInterface * band, freeBands) {   //process listFree first if it want paint on foreground
        if ((renderedView = band->render()))
            processBand(band, renderedView);
    }

    //m_renderedPages.append(page);
    m_currentRenderedPage = 0;
    m_data->appendPage(page);

    if (m_delay > 0) {
        QWaitCondition waitCondition;
        QMutex mutex;
        waitCondition.wait(&mutex,m_delay);
    }
}


void RendererProcessor::processBand(CuteReport::BandInterface * band, RenderedItemInterface * renderedView)
{
    Q_ASSERT(band);

    emit log(CuteReport::LogDebug, QString("rendering band: %1").arg(band->objectName()), "");

    emit m_rendererItemInterface->onBandBefore(band);

    // FIXME: process it on rendered item since band can stretch
    if (band->layoutType() != BandInterface::LayoutFree && !canFitBandToPage(band) )
        createNewRenderingPage();

    m_processingBand  = band;

    RenderedItemInterface * renderedItem = renderedView ? renderedView : band->render();

    if (renderedItem) {

        if (band->layoutType() == BandInterface::LayoutFree)
            renderedItem->setZValue(band->order()>=0 ? (100 + band->order()) : (-100 - band->order()) ); // 100 reserverd for top and bottom

        QRectF geometry;

        /** in case item does not provide its own geometry logic, we are doing it here in renderer */
        if (band->selfRendering() ) {
            geometry = renderedItem->absoluteGeometry(Millimeter);
        } else {
            geometry.setSize(band->geometry(Millimeter).size());
            if (band->layoutType()== BandInterface::LayoutTop)
                geometry.moveTo(m_freeSpace.topLeft());
            else if (band->layoutType()== BandInterface::LayoutBottom)
                geometry.moveBottomLeft(m_freeSpace.bottomLeft());
            else if (band->layoutType()== BandInterface::LayoutFree)
                geometry = band->geometry(Millimeter);

//            QRectF geometryPixel = convertUnit( geometry, band->unit(), Pixel, band->dpi() );
//            renderedItem->setParentItem(m_currentRenderedPage);
//            renderedItem->setPos(geometryPixel.topLeft());
//            renderedItem->setRect(QRectF(QPointF(0,0), geometryPixel.size()));

            renderedItem->setParentItem(m_currentRenderedPage);
            renderedItem->setAbsoluteGeometry(geometry);
//            renderedItem->redraw();
        }

        m_bandDelta = geometry.topLeft() - band->absoluteGeometry().topLeft();

        QList<CuteReport::RenderedItemInterface*> renderedChildren;

        foreach (CuteReport::BaseItemInterface * const item, band->findChildren<CuteReport::BaseItemInterface *>()) {
            CuteReport::RenderedItemInterface* child = processItem(m_bandDelta, item, renderedItem, true);
            if (child)
                renderedChildren.append(child);
        }

        if (band->stretchable()) {
            foreach (RenderedItemInterface * child, renderedChildren) {
                QRectF childRect = child->absoluteGeometry(Millimeter);
                if ( childRect.bottom() > geometry.bottom())
                    geometry.setBottom( childRect.bottom() );
            }
            renderedItem->setAbsoluteGeometry(geometry);
//            renderedItem->redraw();
        }

        if (band->layoutType()== BandInterface::LayoutTop)
            m_freeSpace.setTop( qMax (m_freeSpace.top(), geometry.bottom() ) );  // band can be placed in its own logic to any position
        else if (band->layoutType()== BandInterface::LayoutBottom)
            m_freeSpace.setBottom( qMin( m_freeSpace.bottom(), geometry.top() ) );

    }

    m_lastProcessedBand = band;

    m_state = RendererPublicInterface::DrawingContent;

    emit m_rendererItemInterface->onBandAfter(band);

}


RenderedItemInterface * RendererProcessor::processItem(QPointF delta, CuteReport::BaseItemInterface * item, RenderedItemInterface * parent, bool withChildren, RenderedItemInterface * renderedView)
{
    Q_ASSERT(item);
    emit log(CuteReport::LogDebug, QString("rendering item: %1").arg(item->objectName()), "");

    emit m_rendererItemInterface->onItemBefore(item);

    RenderedItemInterface * renderedItem = renderedView ? renderedView : item->render();

    if (renderedItem) {
        QRectF geometry = item->absoluteGeometry(Millimeter);
        geometry.translate(delta);

        renderedItem->setParentItem(parent ? (QGraphicsItem *)parent : (QGraphicsItem *)m_currentRenderedPage);
        //        QPointF pos = item->mapToParent(QPointF(0,0), Millimeter, Pixel);
        //        renderedItem->setPos(pos);

//        renderedItem->setParentItem(m_currentRenderedPage);
        renderedItem->setAbsoluteGeometry(geometry);
//        renderedItem->redraw();

//        QRectF geometry = renderedItem->absoluteGeometry(Millimeter);
//        if (delta != QPointF()) {
//            geometry.translate(delta);
//            renderedItem->setAbsoluteGeometry(geometry);
//        }

//        renderedItem->setParentItem(parent ? (QGraphicsItem *)parent : (QGraphicsItem *)m_currentRenderedPage);
//        QPointF pos = item->mapToParent(QPointF(0,0), Millimeter, Pixel);
//        renderedItem->setPos(pos);

        emit m_rendererItemInterface->onItemAfter(item);

        if (withChildren) {
            foreach (CuteReport::BaseItemInterface * const child, item->findChildren<CuteReport::BaseItemInterface *>())
                processItem(delta, child, renderedItem, true);
        }
    }

    return renderedItem;
}


void RendererProcessor::processDataset(DatasetInterface * dtst)
{
    emit log(LogInfo, QString("rendering dataset: %1").arg(dtst->objectName()), "");

    emit m_rendererItemInterface->onDatasetBefore(dtst);

    /// store dynamic data
    int currentDatasetLine = m_currentDatasetLine;
    DatasetInterface * currentDataset = m_currentDataset;

    //    bool skipIteration = false;

    if (!dtst->isPopulated())
        if (!dtst->populate()) {
            emit log(CuteReport::LogError, QString("dataset \"%1\" error").arg(dtst->objectName()),
                     QString("%1: %2").arg(dtst->objectName()).arg(dtst->lastError()));
            terminate();
            return;
        }
    dtst->firstRow();

    m_currentDataset = dtst;
    m_currentDatasetLine = 0;
    setValue("_line", 0 , QScriptValue::ReadOnly);

    BandsList currentGroup = bandRegisteredToDataset(dtst->objectName());
    qSort(currentGroup.begin(), currentGroup.end(), cmpBands);


    do {
        if (terminated())
            return;

        setValue("_line",  getValue("_line").toInt() + 1, QScriptValue::ReadOnly);
        m_aggregateFunctions->processDatasetIteration(m_currentDataset);

        foreach(BandInterface * band, currentGroup) {
            //            q_Debug() << "next band in dataset group =" << band->objectName();
            processBand(band);
        }

        m_currentDatasetLine++;
    }
    while (dtst->nextRow());

    foreach (BandInterface * band, currentGroup)
        if (!m_bandsDone.contains(band))
            m_bandsDone.append(band);

    /// restore dynamic data
    m_currentDatasetLine = currentDatasetLine;
    m_currentDataset = currentDataset;

    emit m_rendererItemInterface->onDatasetAfter(dtst);
}


bool RendererProcessor::canFitBandToPage(BandInterface * band)
{
    return (m_freeSpace.top() + band->geometry().height() <= m_freeSpace.bottom());
}


bool RendererProcessor::terminated()
{
    QMutexLocker locker(&mutex);
    return m_terminate;
}


//void RenderProcessor::scriptException(QScriptValue value)
//{
//    q_Debug() << "RenderProcessor::scriptException  " << value.toString();
//    emit message (m_origReport, LogClitical, QString("script exception: %1").arg(value.toString()));
//}


void RendererProcessor::setValue(const QString & valueName, const QVariant &value, QScriptValue::PropertyFlags flag)
{
    m_data->scriptEngine->globalObject().setProperty(valueName, m_data->scriptEngine->newVariant(value), flag);
}


QVariant RendererProcessor::getValue(const QString & valueName)
{
    return m_data->scriptEngine->globalObject().property(valueName).toVariant();
}


CuteReport::DatasetInterface * RendererProcessor::datasetRegisteredTo(BandInterface * band)
{
    CuteReport::DatasetInterface * resultDataset = 0;
    foreach (QString dataset, m_datasetRegister.keys()) {
        BandsList list = m_datasetRegister.values(dataset);
        if (list.contains(band))
            resultDataset = m_data->renderer->reportCore()->datasetByName(dataset, m_data->workingReportCopy);
    }
    return resultDataset;
}


void RendererProcessor::registerBandToDatasetIteration(const QString & datasetName, const QString & objectName)
{
    BaseItemInterface * item = m_data->renderer->reportCore()->itemByName(objectName, m_currentPage);
    BandInterface * band = qobject_cast<BandInterface*>(item);
    if (band)
        registerBandToDatasetIteration(datasetName, band);
}


void RendererProcessor::registerBandToDatasetIteration(const QString & datasetName, CuteReport::BandInterface * band)
{
    if (band && !m_datasetRegister.values(datasetName).contains(band) )
        m_datasetRegister.insertMulti(datasetName, band);
}


void RendererProcessor::unregisterBandFromDatasetIteration(const QString & datasetName, const QString & objectName)
{
    BaseItemInterface * item = m_data->renderer->reportCore()->itemByName(objectName, m_currentPage);
    BandInterface * band = qobject_cast<BandInterface*>(item);

    if (band)
        unregisterBandFromDatasetIteration(datasetName, band);
}


void RendererProcessor::unregisterBandFromDatasetIteration(const QString & datasetName, CuteReport::BandInterface * band)
{
    if (band && !m_datasetRegister.values(datasetName).contains(band) ) {
        QMutableHashIterator<QString, BandInterface*> i(m_datasetRegister);
        while (i.hasNext()) {
            i.next();
            if  (i.value() == band)
                i.remove();
        }
    }
}


BandsList RendererProcessor::bandRegisteredToDataset(const QString & datasetName)
{
    return m_datasetRegister.values(datasetName);
}


const CuteReport::BandInterface * RendererProcessor::getBandForItem(const CuteReport::BaseItemInterface * item)
{
    if (!item)
        return 0;

    if (qobject_cast<CuteReport::BandInterface*> (item))
        return static_cast<const CuteReport::BandInterface*> (item);
    else
        return getBandForItem(item->parentItem());
}


QString RendererProcessor::processString(const QString & string, const QString & delimiterBegin, const QString & delimiterEnd, const CuteReport::BaseItemInterface * item)
{
    // escaping all delimiter characters for prevent problems
    QString regexp;
    for (int i=0; i<delimiterBegin.length(); ++i)
        regexp+=QString("\\") + delimiterBegin.at(i);
    regexp+="(.*)";
    for (int i=0; i<delimiterEnd.length(); ++i)
        regexp+= QString("\\") + delimiterEnd.at(i);


    QRegExp rx(regexp);
    rx.setMinimal(true);
    QString str( setVariablesValue(string, m_data->workingReportCopy->variables()) );

    int pos = 0;
    while ((pos = rx.indexIn(str, pos)) != -1) {
        int length = rx.matchedLength();

        QString expression = rx.cap(1);
        QString evaluationResult = _processString(expression, item);

        str.replace(pos, length, evaluationResult);
        pos += evaluationResult.length();
    }

    return str;
}


QString RendererProcessor::processString(const QString & string, const BaseItemInterface *item)
{
    return _processString( setVariablesValue(string, m_data->workingReportCopy->variables()), item);
}


QString RendererProcessor::_processString(const QString & string, const CuteReport::BaseItemInterface * item)
{
    QString expression = preprocessEvaluateString(string, item);
    QString evaluationResult;

    if (m_data->scriptEngine->canEvaluate(expression)) {
        QString evaluateStr = m_data->scriptEngine->evaluate(expression).toString();
        if (m_data->scriptEngine->hasUncaughtException()) {
            QString _message = QString("script error at line %1 \n%2")
                               .arg(m_data->scriptEngine->uncaughtExceptionLineNumber())
                               .arg(m_data->scriptEngine->uncaughtException().toString());
            emit log(LogError, QString("script evaluation error for item \'%1\'").arg(item->objectName()),
                     QString("script evaluation error for item \'%1\': %2").arg(item->objectName()).arg(_message));
            evaluationResult = "\'error\'";
        } else
            evaluationResult = evaluateStr;
    } else
        evaluationResult = "\'can't evaluate\'";

    return evaluationResult;
}


void RendererProcessor::registerEvaluationString(const QString & string, BaseItemInterface *item)
{
    if (string.isEmpty())
        return;

    QString str(string);
    /// replacing dataset value strings like data."field" to correct syntax data.value(field)

    QRegExp rx("(\\w+)\\.\\\"(.*)\\\"");
    rx.setMinimal(true);

    int pos = 0;
    while ((pos = rx.indexIn(str, pos)) != -1) {
        int length = rx.matchedLength();

        QString result = QString("%1.value(\"%2\")").arg(rx.cap(1)).arg(rx.cap(2));

        str.replace(pos, length, result);
        pos += result.length();
    }

    const BandInterface * band = getBandForItem(item);
    if (band) {
        m_aggregateFunctions->findAndRegisterAggregateFunctions(str, band);
    }
}




QString RendererProcessor::preprocessEvaluateString(QString str, const BaseItemInterface *item)
{
    /// replacing dataset value strings like data."field" to correct syntax data.value(field)

    QRegExp rx("(\\w+)\\.\\\"(.*)\\\"");
    rx.setMinimal(true);

    int pos = 0;
    while ((pos = rx.indexIn(str, pos)) != -1) {
        int length = rx.matchedLength();

        QString result = QString("%1.value(\"%2\")").arg(rx.cap(1)).arg(rx.cap(2));

        str.replace(pos, length, result);
        pos += result.length();
    }


    const BandInterface * band = getBandForItem(item);
    if (band) {
        str = m_aggregateFunctions->replaceWithRealValues(str, band);
    }

    return str;
}
