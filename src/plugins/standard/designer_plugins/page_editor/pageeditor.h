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
#ifndef PAGEEDITOR_H
#define PAGEEDITOR_H

#include "moduleinterface.h"
#include <QPointer>
#include <QIcon>
#include <QHash>

namespace PropertyEditor {
class PropertyEditor;
}

namespace CuteReport {
class PageInterface;
class ReportInterface;
class PageManipulatorInterface;
class PageAction;
}

class PageEditorContainer;
class ObjectInspector;

namespace CuteDesigner
{


class PageEditor : public ModuleInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteDesigner.ModuleInterface/1.0")
#endif
    Q_INTERFACES(CuteDesigner::ModuleInterface)
public:
    explicit PageEditor(QObject *parent = 0);
    ~PageEditor();
    virtual void reloadSettings();
    virtual void saveSettings();
    virtual void activate();
    virtual void deactivate();
    virtual void sync();
    virtual QWidget * view();
    virtual QIcon icon();
    virtual QString name();
    virtual qint16 priority() { return 200;}

    virtual QList<CuteDesigner::DesignerMenu*> mainMenu();

signals:
    
public slots:

private slots:
    void slotActiveObjectChanged(QObject * object);
    void slotSelectionChanged();
    void slotUpdateObjectInspector();
    void slotReportChanged(CuteReport::ReportInterface * report);
    void slotRequestForCreatePage(QString moduleName);
    void slotRequestForDeletePage(QString pageName);
    void slotRequestForClonePage(QString pageName);
    void slotCurrentPageChangedByGUI(QString pageName);
    void slotCurrentPageChangedByCore(CuteReport::PageInterface* page);
    void slotChangeCurrentPage(CuteReport::PageInterface* page);
    void slotRequestForRenamePage(QString pageName);
    void slotNewPage();
    void slotDeletePage();

private:
    QPointer<PageEditorContainer> ui;
    PropertyEditor::PropertyEditor *m_propertyEditor;
    ObjectInspector * m_objectInspector;

    QHash<CuteReport::PageInterface*, QString> m_pages;
    QPointer<CuteReport::PageInterface> m_currentPage;
    QPointer<QObject> m_activeObject;
    CuteReport::PageManipulatorInterface * m_currentManipulator;
    QHash<int,CuteReport::PageManipulatorInterface *> m_pageManipulators;
    QPointer<CuteReport::ReportInterface> m_currentReport;
    bool m_isActive;
};

} //namespace
#endif // PAGEEDITOR_H
