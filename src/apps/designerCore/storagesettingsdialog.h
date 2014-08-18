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
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/

#ifndef STORAGESETTINGSDIALOG_H
#define STORAGESETTINGSDIALOG_H

#include <QDialog>
#include <QHash>

namespace Ui {
class StorageSettingsDialog;
}

namespace CuteDesigner {
class Core;
}

namespace CuteReport {
class StorageHelperInterface;
class StorageInterface;
}

class OptionsDialogStoragePage;

class StorageSettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit StorageSettingsDialog(CuteDesigner::Core * core);
    ~StorageSettingsDialog();

    CuteReport::StorageInterface *currentStorage();
    
//private slots:
//    void currentModuleChanged(QString module);

private:
    Ui::StorageSettingsDialog *ui;
    CuteDesigner::Core * m_core;
    OptionsDialogStoragePage * m_page;
//    QHash<QString, CuteReport::StorageOptionsInterface*> m_options;
//    CuteReport::StorageOptionsInterface * m_currentOptions;
};

#endif // STORAGESETTINGSDIALOG_H
