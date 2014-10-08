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

#ifndef PAGEEDITORCONTAINER_H
#define PAGEEDITORCONTAINER_H

#include <QWidget>
#include <QList>
#include "core.h"


namespace Ui {
class PageEditorContainer;
}

namespace CuteReport {
class PageAction;
class PageInterface;
}

class PageEditor;

class PageEditorContainer : public QWidget
{
    Q_OBJECT
    
public:
    explicit PageEditorContainer(PageEditor * pageEditor, QWidget *parent = 0);
    ~PageEditorContainer();

    void init();

    void addTab(QWidget * widget, QIcon icon, const QString &name);
    void removeTab(const QString & name);
    void removeAllTabs();
    void setCurrentTab(const QString &pageName);
    void setNewPageName(const QString &pageName, const QString &newName);

    void addPagePlugins(QList<CuteReport::PageInterface*> pages);
    void addPropertyEditor(QWidget * widget);
    void addObjectInspector(QWidget * widget);

    void addItem(const QIcon &icon, const QString &name, const QString &suiteName, const QString &group);
    void setPageActions(QList<CuteReport::PageAction*> actions);

    void reloadSettings();
    void saveSettings();

public slots:
//    void slotNewPageActions(QList<CuteReport::PageAction*> actions);

signals:
    void requestForCreatePage(QString pluginName);
    void requestForDeletePage(QString pageName);
    void requestForClonePage(QString pageName);
    void requestForRenamePage(QString pageName);
    void currentTabChanged(QString pageName);

private slots:
    void slotDeleteClicked();
    void slotCreateClicked();
    void slotCloneClicked();
    void slotCurrentTabChanged(int);
    void slotTabDoubleClicked(int index);
    
private:
    Ui::PageEditorContainer *ui;
    PageEditor * m_pageEditor;
};

#endif // PAGEEDITORCONTAINER_H
