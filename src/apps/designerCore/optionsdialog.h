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

#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class OptionsDialog;
}

namespace CuteDesigner {
class Core;
}

class OptionsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit OptionsDialog(CuteDesigner::Core *core);
    ~OptionsDialog();
    
private slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::OptionsDialog *ui;
};


class OptionsDialogPageInterface: public QWidget
{
public:
    OptionsDialogPageInterface(CuteDesigner::Core *core);
    virtual ~OptionsDialogPageInterface();
    virtual QListWidgetItem * createButton(QListWidget * listWidget) = 0;

    virtual void activate();
    virtual void deactivate();
};

#endif // OPTIONSDIALOG_H
