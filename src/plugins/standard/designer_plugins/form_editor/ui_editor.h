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
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/
#ifndef UI_EDITOR_H
#define UI_EDITOR_H

#include "moduleinterface.h"
#include "globals.h"

namespace CuteReport {
class ReportInterface;
}

class UIcontainer;
class QDesignerWorkbench;

class UiEditor : public CuteDesigner::ModuleInterface
{
	Q_OBJECT
    Q_INTERFACES(CuteDesigner::ModuleInterface)
public:
    UiEditor(QObject* parent = 0);

    ~UiEditor();

    virtual void reloadSettings();
    virtual void saveSettings();
    virtual void activate();
    virtual void deactivate();
    virtual void sync();
    virtual QWidget * view();
    virtual QIcon icon();
    virtual QString name();
    virtual qint16 priority(){return 500;}

private slots:
    void saveCurrentForm(bool forceIfDirty = false);
    void slotRequestForNewForm();
    void slotRequestForDelete(QString formName);
//    void slotRequestForContent(QString formName);
    void slotRequestForSave(QString formName, QString xmlContent);
    void slotRequestFileContent(QString fileName);
    void slotCurrentReportChanged(CuteReport::ReportInterface*report);
    void saveData(CuteReport::ReportInterface*report);
    void slotActiveFormChanged(QString formName);
    void slotFormNameChangedByDesigner(QString name);
    void slotRequestForRename(QString name);

private:
    bool isDirty();
    void replaceEmptyActionIcons(QAction*action);

private:
    QPointer<UIcontainer> m_widget;
    QDesignerWorkbench * m_designer;
//    CuteReport::ReportInterface * m_currentReport;
};


#endif //UI_EDITOR_H
