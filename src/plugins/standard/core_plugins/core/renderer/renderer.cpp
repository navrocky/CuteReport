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
#include "renderer.h"
#include <QtCore>
#include "types.h"
#include "datasetinterface.h"
#include "pageinterface.h"
#include "bandinterface.h"
#include "rendereriteminterface.h"
#include "renderediteminterface.h"
#include "rendererdata.h"
#include "rendererprocessor.h"
#include "reportcore.h"
#include "objectfactory.h"
#include "renderedreport.h"

using namespace CuteReport;

static const QString MODULENAME = "Renderer";

Renderer::Renderer(QObject *parent) :
    RendererInterface(parent),
    m_data(0),
    m_antialiasing(false),
    m_textAntialiasing(false),
    m_smoothPixmapTransform(false),
    m_dpi(0),
    m_delay(0),
    m_resultReady(false)
{
}


Renderer::~Renderer()
{
    delete m_data;
}


Renderer::Renderer(const Renderer &dd, QObject * parent)
    :RendererInterface(dd, parent),
      m_data(0),
      m_antialiasing(dd.antialiasing()),
      m_textAntialiasing(dd.textAntialiasing()),
      m_smoothPixmapTransform(dd.smoothPixmapTransform()),
      m_dpi(dd.dpi()),
      m_delay(dd.delay()),
      m_resultReady(dd.m_resultReady)
{
}


RendererInterface *Renderer::createInstance(QObject *parent) const
{
    return new Renderer(parent);
}


RendererInterface * Renderer::clone() const
{
    return new Renderer(*this, 0);
}


QString Renderer::moduleShortName() const
{
    return QString("Renderer");
}


void Renderer::run(ReportInterface *report)
{
    if (isRunning()) {
        ReportCore::log(LogInfo, MODULENAME, "Renderer can not be started, because previous task is not completed yet");
        return;
    } else
        ReportCore::log(LogInfo, MODULENAME, "started");
    RendererData * data;
    m_resultReady = false;

    // remove previous processor if still running
    if (m_data != 0) {
        data = m_data;
        if (m_data && m_data->processor)
            m_data->processor->terminate();
        data->clear();
    } else {
        data = new RendererData();
        m_data = data;
    }

    data->renderer = this;
    data->origReport = report;

    RendererProcessor * renderer = new RendererProcessor(data);
    connect(renderer, SIGNAL(processingPage(int,int)), this, SIGNAL(processingPage(int,int)), Qt::QueuedConnection);
    connect(renderer, SIGNAL(started()), this, SIGNAL(started()), Qt::QueuedConnection);
    connect(renderer, SIGNAL(done(bool)), this , SLOT(slotProcessorDone(bool)), Qt::QueuedConnection);
    connect(renderer, SIGNAL(log(CuteReport::LogLevel,QString,QString)), this, SLOT(slotLog(CuteReport::LogLevel,QString,QString)), Qt::QueuedConnection);

    renderer->start();
}


void Renderer::slotLog(CuteReport::LogLevel level, const QString & shortMessage, const QString & fullMessage) const
{
    ReportCore::log(level, "Renderer::Processor", shortMessage, fullMessage);
}


void Renderer::stop()
{
    ReportCore::log(LogInfo, MODULENAME, "cancelled");
    if (m_data && m_data->processor)
        m_data->processor->terminate();
}


bool Renderer::isRunning()
{
    return m_data && m_data->processor;
}


CuteReport::RenderedReportInterface * Renderer::takeRenderedReport()
{
    if (!m_resultReady)
        return 0;

    RenderedReport * renReport = new RenderedReport();
    renReport->setRenderedPages(m_data->pages);
    renReport->setDpi(m_data->dpi);
    m_data->pages.clear();

    return renReport;
}


ReportInterface * Renderer::report()
{
    return m_data ? m_data->origReport : 0;
}


void Renderer::slotProcessorDone(bool successfull)
{
    m_resultReady = true;
    ReportCore::log(LogDebug, MODULENAME, "slotProcessorDone");
    ReportCore::log(LogInfo, MODULENAME, "done");
    emit done(successfull);
}


void Renderer::slotReportDestroyed(QObject * object)
{
    ReportInterface * report = reinterpret_cast<CuteReport::ReportInterface *>(object);
    if (m_data && m_data->processor)
        m_data->processor->terminate();
    disconnect(report, SIGNAL(destroyed(QObject*)), this, SLOT(slotReportDestroyed(QObject*)));
    if(m_data) {
        delete m_data;
        m_data = 0;
    }
}


void Renderer::setAntialiasing(bool b)
{
    if (b == m_antialiasing)
        return;

    m_antialiasing = b;

    emit antialiasingChanged(m_antialiasing);
    emit changed();
}


bool Renderer::antialiasing() const
{
    return m_antialiasing;
}


void Renderer::setTextAntialiasing(bool b)
{
    if (b == m_textAntialiasing)
        return;

    m_textAntialiasing = b;

    emit textAntialiasingChanged(m_textAntialiasing);
    emit changed();
}


bool Renderer::textAntialiasing() const
{
    return m_textAntialiasing;
}


void Renderer::setSmoothPixmapTransform(bool b)
{
    if (b == m_smoothPixmapTransform)
        return;

    m_smoothPixmapTransform = b;

    emit smoothPixmapTransformChanged(m_smoothPixmapTransform);
    emit changed();
}


bool Renderer::smoothPixmapTransform() const
{
    return m_smoothPixmapTransform;
}


void Renderer::setDpi(int dpi)
{
    if (dpi == m_dpi)
        return;

    m_dpi = dpi;
    if (m_dpi < 0)
        m_dpi = 0;

    emit dpiChanged(m_dpi);
    emit changed();
}


int Renderer::dpi() const
{
    return m_dpi;
}


void Renderer::setDelay(int delay)
{
    if (delay == m_delay)
        return;

    m_delay = delay;
    if (m_delay < 0)
        m_delay = 0;

    emit delayChanged(m_delay);
    emit changed();
}


int Renderer::delay() const
{
    return m_delay;
}


QString Renderer::_current_property_description() const
{
    QString propertyName = metaObject()->property(m_currentProperty).name();

    if (propertyName == "antialiasing")
        return tr("Use antialiasing while rendering");
    else if (propertyName == "textAntialiasing")
        return tr("Use text antialiasing while rendering");
    else if (propertyName == "smoothPixmapTransform")
        return tr("Use smooth pixmap transformation");
    else if (propertyName == "dpi")
        return tr("Dot Per Inch for rendered page");
    else if (propertyName == "iterationDelay")
        return tr("Use iteration delay for debug");

    return RendererInterface::_current_property_description();
}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Renderer, Renderer)
#endif
