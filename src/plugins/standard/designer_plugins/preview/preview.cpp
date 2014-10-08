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
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/
#include "preview.h"
#include "reportpreview.h"
#include "pageinterface.h"
#include "reportinterface.h"
#include "iteminterface.h"
#include "reportcore.h"
#include "printerinterface.h"
#include "core.h"

#include <QGraphicsScene>
#include <QGraphicsDropShadowEffect>
#include <QProgressDialog>

inline void initMyResource() { Q_INIT_RESOURCE(preview); }

using namespace CuteDesigner;

Preview::Preview(QObject *parent) :
    ModuleInterface(parent),
    ui(0)
{

}


Preview::~Preview()
{
    delete ui;
}


void Preview::init(Core *core)
{
    initMyResource();
    ModuleInterface::init(core);

    ui = new CuteReport::ReportPreview();
    ui->setEnabled(false);
    ui->setReportCore(core->reportCore());
    connect(core->reportCore(), SIGNAL(rendererStarted(CuteReport::ReportInterface*)), this, SLOT(slotRenderingStarted(CuteReport::ReportInterface*)));
    connect(core, SIGNAL(currentReportChanged(CuteReport::ReportInterface*)),
            this, SLOT(slotCurrentReportChanged(CuteReport::ReportInterface*)));
    ui->setEnabled(core->currentReport());
}


void Preview::reloadSettings()
{
}


void Preview::saveSettings()
{
}


QWidget * Preview::view()
{
    if (!ui) {
        ui = new CuteReport::ReportPreview();
    }
    return ui;
}


void Preview::activate()
{
    core()->sync(); // report run is possible so we are syncing
}


void Preview::deactivate()
{
}


QIcon Preview::icon()
{
    return QIcon(":/images/preview_48.png");
}


QString Preview::name()
{
    return QString("Preview");
}


void Preview::slotRenderingStarted(CuteReport::ReportInterface * report)
{
    Q_UNUSED(report)
    core()->switchToModule(this);
}


void Preview::slotCurrentReportChanged(CuteReport::ReportInterface *report)
{
    ui->connectReport(report);
}


//suit_end_namespace

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Preview, Preview)
#endif
