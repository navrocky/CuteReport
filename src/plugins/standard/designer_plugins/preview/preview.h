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
#ifndef PREVIEW_H
#define PREVIEW_H

#include "moduleinterface.h"
#include <QPointer>
#include <QIcon>

namespace CuteReport {
class ReportInterface;
class RenderedPageInterface;
class ReportPreview;
}

class QProgressDialog;
class QGraphicsScene;
class QGraphicsDropShadowEffect;


class Preview : public CuteDesigner::ModuleInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteDesigner.ModuleInterface/1.0")
#endif
    Q_INTERFACES(CuteDesigner::ModuleInterface)

public:
    explicit Preview(QObject *parent = 0);
    ~Preview();

    virtual void init(CuteDesigner::Core *core);

    virtual void reloadSettings();
    virtual void saveSettings();
    virtual void activate();
    virtual void deactivate();
    virtual void sync(){}
    virtual QWidget * view();
    virtual QIcon icon();
    virtual QString name();
    virtual qint16 priority() {return 600;}

private slots:
    void slotCurrentReportChanged(CuteReport::ReportInterface*report);
    void slotRenderingStarted(CuteReport::ReportInterface * report);

private:
    QPointer<CuteReport::ReportPreview> ui;
};

//SUIT_END_NAMESPACE
#endif // PREVIEW_H
