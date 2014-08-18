/***************************************************************************
 *   This file is part of the CuteReport project                           *
 *   Copyright (C) 2012 by Alexander Mikhalov                              *
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

#include "objectpreviewplugin.h"
#include "objectpreview.h"

#include <QtPlugin>

CuteReportObjectPreviewPlugin::CuteReportObjectPreviewPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void CuteReportObjectPreviewPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;

    initialized = true;
}

bool CuteReportObjectPreviewPlugin::isInitialized() const
{
    return initialized;
}

QWidget *CuteReportObjectPreviewPlugin::createWidget(QWidget *parent)
{
    return new CuteReport::ObjectPreview(parent);
}

QString CuteReportObjectPreviewPlugin::name() const
{
    return "ObjectPreview";
}

QString CuteReportObjectPreviewPlugin::group() const
{
    return "CuteReport";
}

QIcon CuteReportObjectPreviewPlugin::icon() const
{
    return QIcon(":images/objectpreview.png");
}

QString CuteReportObjectPreviewPlugin::toolTip() const
{
    return "";
}

QString CuteReportObjectPreviewPlugin::whatsThis() const
{
    return "";
}

bool CuteReportObjectPreviewPlugin::isContainer() const
{
    return false;
}

QString CuteReportObjectPreviewPlugin::domXml() const
{
    return "<widget class=\"ObjectPreview\" name=\"objectPreview\">\n"
           " <property name=\"geometry\">\n"
           "  <rect>\n"
           "   <x>0</x>\n"
           "   <y>0</y>\n"
           "   <width>400</width>\n"
           "   <height>300</height>\n"
           "  </rect>\n"
           " </property>\n"
           "</widget>\n";
}

QString CuteReportObjectPreviewPlugin::includeFile() const
{
    return "objectpreview.h";
}

Q_EXPORT_PLUGIN2(objectpreviewplugin, CuteReportObjectPreviewPlugin)
