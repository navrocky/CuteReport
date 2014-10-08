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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "core.h"
#include "globals.h"
#include "types.h"

namespace Ui {
class MainWindow;
}

namespace CuteReport {
    class RenderedPageInterface;
}

namespace CuteDesigner{
class ModuleInterface;
class Core;
}

//class PageEditor;
class QProgressDialog;
class QLabel;


class  MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(CuteDesigner::Core *core);
    ~MainWindow();

    void reloadSettings();
    void saveSettings();

    void addTab(QWidget * widget, QIcon icon, QString name);
    void addTabSpacer();
    void switchToTab(int index);
    void addToStatusBar(QLabel * label);

    void showMetric(CuteReport::MetricType type, const QVariant &value);
    void setStdActions(CuteDesigner::Core::StdActions actions);

signals:
    void closeRequest();

private slots:
    void closeEvent(QCloseEvent* event);
    void slotNewReport_after(CuteReport::ReportInterface *report);
    void slotNewPage_after(CuteReport::PageInterface*);
    void slotLoadReport_after(CuteReport::ReportInterface *report);
    void slotTabsSwitched(int index);
    void slotAbout();
    void slotOptions();
    void slotUpdateTabs();


private:
    Ui::MainWindow *ui;
    CuteDesigner::Core * m_core;
//    QList<QAction *> defaultPageActions;
    QTimer m_updateTabTimer;
};

#endif // MAINWINDOW_H
