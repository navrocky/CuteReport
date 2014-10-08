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
#ifndef RENDERER_H
#define RENDERER_H

#include <QPointer>
#include <QMutex>
#include <QThread>
#include "rendererinterface.h"
#include "reportinterface.h"
#include "pageinterface.h"

namespace CuteReport {
class BaseItemInterface;
class BandInterface;
class RendererPublicInterface;
class RenderedReportInterface;
}

class RenderProcessor;
class RendererItemInterface;
class RendererData;


class Renderer : public CuteReport::RendererInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteReport.RendererInterface/1.0")
#endif
    Q_INTERFACES(CuteReport::RendererInterface)

    Q_PROPERTY(bool antialiasing READ antialiasing WRITE setAntialiasing NOTIFY antialiasingChanged)
    Q_PROPERTY(bool textAntialiasing READ textAntialiasing WRITE setTextAntialiasing NOTIFY textAntialiasingChanged)
    Q_PROPERTY(bool smoothPixmapTransform READ smoothPixmapTransform WRITE setSmoothPixmapTransform NOTIFY textAntialiasingChanged)
//    Q_PROPERTY(int dpi READ dpi WRITE setDpi NOTIFY dpiChanged)
    Q_PROPERTY(int iterationDelay READ delay WRITE setDelay NOTIFY delayChanged)

public:
    explicit Renderer(QObject *parent = 0);
    ~Renderer();

    virtual RendererInterface * createInstance(QObject * parent) const;
    virtual RendererInterface * clone() const;

    virtual QString moduleShortName() const;
    virtual QString suitName() const { return "Standard"; }

    virtual void run(CuteReport::ReportInterface* report);
    virtual void stop();
    bool isRunning();

    virtual CuteReport::ReportInterface * report();

    virtual CuteReport::RenderedReportInterface * takeRenderedReport();

    void setAntialiasing(bool b);
    bool antialiasing() const;

    void setTextAntialiasing(bool b);
    bool textAntialiasing() const;

    void setSmoothPixmapTransform(bool b);
    bool smoothPixmapTransform() const;

    void setDpi(int dpi);
    int dpi() const;

    void setDelay(int delay);
    int delay() const;

    virtual QString _current_property_description() const;

signals:
    /**
     defined in CuteReport::RendererInterface
         void started();
         void message(int logLevel, QString msg);
         void done(bool errorsFound);
         void cancelled();
         void processingPage(int page, int total);
         void dpiChanged(int);
    */

    void antialiasingChanged(bool);
    void textAntialiasingChanged(bool);
    void smoothPixmapTransformChanged(bool);
    void workWithReportCopyChanged(bool);
    void delayChanged(int);


private slots:
    void slotProcessorDone(bool successfull);
    void slotReportDestroyed(QObject * object);
    void slotLog(CuteReport::LogLevel level, const QString & shortMessage, const QString & fullMessage) const;

private:
    Renderer(const Renderer &dd, QObject * parent);

    RendererData * m_data;

    bool m_antialiasing;
    bool m_textAntialiasing;
    bool m_smoothPixmapTransform;
    int m_dpi;
    int m_delay;
    bool m_resultReady;
};

#endif // RENDERER_H
