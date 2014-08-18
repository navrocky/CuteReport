/***************************************************************************
 *   This file is part of the CuteReport project                           *
 *   Copyright (C) 2012-2013 by Alexander Mikhalov                              *
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

#ifndef DESIGNERROOTWIDGET_H
#define DESIGNERROOTWIDGET_H

#include <QWidget>
#include "qdesigner_settings.h"

namespace Ui {
class DesignerRootWidget;
}

class QMdiArea;
class QMenuBar;
class QMdiSubWindow;
class QSettings;

class DesignerRootWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit DesignerRootWidget(QWidget *parent = 0 );
    ~DesignerRootWidget();

    void addFormEditor(QWidget * widget);
    void addWidgetBar(QWidget * widget);
    void addObjectInspector(QWidget * widget);
    void addPropertyEditor(QWidget * widget);
    void addActionEditor(QWidget * widget);
    void addSignalSlotEditor(QWidget * widget);
    void addResourceEditor(QWidget * widget);

    QMdiArea *mdiArea() const;

    QMenuBar *menuBar() const;
    void setMenuBar(QMenuBar *menubar);

    QMdiSubWindow *createMdiSubWindow(QWidget *fw, Qt::WindowFlags f, const QKeySequence &designerCloseActionShortCut);

    void reloadSettings(QSettings *settings);
    void saveSettings(QSettings *settings);
    
private:
    Ui::DesignerRootWidget *ui;
    QMenuBar * m_menubar;
};

#endif // DESIGNERROOTWIDGET_H
