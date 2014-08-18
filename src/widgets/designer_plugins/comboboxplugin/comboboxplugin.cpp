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

#include "comboboxplugin.h"
#include "combobox.h"

#include <QtPlugin>

CuteReportComboBoxPlugin::CuteReportComboBoxPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void CuteReportComboBoxPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;

    initialized = true;
}

bool CuteReportComboBoxPlugin::isInitialized() const
{
    return initialized;
}

QWidget *CuteReportComboBoxPlugin::createWidget(QWidget *parent)
{
    return new CuteReportComboBox(parent);
}

QString CuteReportComboBoxPlugin::name() const
{
    return "CuteReportComboBox";
}

QString CuteReportComboBoxPlugin::group() const
{
    return "CuteReport";
}

QIcon CuteReportComboBoxPlugin::icon() const
{
    return QIcon(":images/combobox.png");
}

QString CuteReportComboBoxPlugin::toolTip() const
{
    return "";
}

QString CuteReportComboBoxPlugin::whatsThis() const
{
    return "";
}

bool CuteReportComboBoxPlugin::isContainer() const
{
    return false;
}

QString CuteReportComboBoxPlugin::domXml() const
{
    return "<widget class=\"CuteReportComboBox\" name=\"comboBox\">\n"
           " <property name=\"geometry\">\n"
           "  <rect>\n"
           "   <x>0</x>\n"
           "   <y>0</y>\n"
           "   <width>80</width>\n"
           "   <height>20</height>\n"
           "  </rect>\n"
           " </property>\n"
           "</widget>\n";
}

QString CuteReportComboBoxPlugin::includeFile() const
{
    return "combobox.h";
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(cutereportcomboboxplugin, CuteReportComboBoxPlugin)
#endif
