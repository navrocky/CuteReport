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
#ifndef REPORTEDITORCONTAINER_H
#define REPORTEDITORCONTAINER_H

#include <QWidget>
#include "core.h"

namespace Ui {
class ReportContainer;
}

class ReportEditor;

class ReportContainer : public QWidget
{
    Q_OBJECT    
public:
    explicit ReportContainer(ReportEditor * reportEditor, QWidget *parent = 0);
    ~ReportContainer();

    void reloadSettings();
    void saveSettings();

    void addTab(QWidget * const widget, const QIcon & icon, const QString & name);
    void removeTab(int index);
    void removeTab(const QString & name);
    void setCurrentTab(QString name);
    void setCurrentTab(int index);
    void changeTabText(QString oldText, QString newText);
    void changeTabText(int index, QString newText);

signals:
    void requestForNewReport();
    void requestForTemplate();
    void requestForOpenReport();
    void requestForSaveReport();
    void requestForCloseReport();
    void currentTabChanged(int index);
    void requestForRenameReport(int index);

private slots:
    void slotCurrentTabChanged(int index);
    void slotTabDoubleClicked(int index);

private:
    Ui::ReportContainer *ui;
    ReportEditor * m_reportEditor;
};

#endif // REPORTEDITORCONTAINER_H
