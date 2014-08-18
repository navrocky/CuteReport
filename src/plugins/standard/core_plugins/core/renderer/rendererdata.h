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

#ifndef RENDERERDATA_H
#define RENDERERDATA_H
#include "rendererinterface.h"
#include <QMutexLocker>
#include <QScriptEngine>
#include <QPointer>

class RendererProcessor;
class Renderer;
class Thread;


namespace CuteReport {
class FormInterface;
}

class RendererData
{
public:
    explicit RendererData();
    ~RendererData();

//    void setRunning(bool b = true);
    void clear();
//    void appendForm(CuteReport::FormInterface * form);
//    int formCounter();
//    CuteReport::FormInterface * form(int num);

    void appendPage(CuteReport::RenderedPageInterface * page);
    CuteReport::RenderedPageInterface* getPage(int number);
    int pagesCount();
    void appendError(QString error);
    QStringList errors();

    CuteReport::ReportInterface * origReport;
    QPointer<CuteReport::ReportInterface> workingReportCopy;
    Renderer * renderer;
    QPointer<QScriptEngine> scriptEngine;
    QPointer<RendererProcessor> processor;
    Thread * processorThread;
    int dpi;

    QList<CuteReport::RenderedPageInterface* > pages;
private:
    QStringList m_errors;
    QMutex mutex;
//    QList<QPointer<CuteReport::FormInterface> > forms;
};


#endif // RENDERERDATA_H
