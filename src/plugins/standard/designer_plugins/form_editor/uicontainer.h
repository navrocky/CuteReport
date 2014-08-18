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
 ****************************************************************************/
#ifndef UICONTAINER_H
#define UICONTAINER_H

#include <QWidget>
#include <QHash>

namespace Ui {
class uicontainer;
}

class UiEditor;
class QTimer;
class QToolBar;
class QActionGroup;

struct Form {
    Form():index(0),widget(0){}
    int index;
    QWidget * widget;
};


class UIcontainer : public QWidget
{
    Q_OBJECT
    
public:
    explicit UIcontainer(UiEditor * editor, QWidget *designerWidget, QWidget *parent = 0);
    ~UIcontainer();

    void reloadSettings();
    void saveSettings();

    void addDesignerActions(QList<QAction*> list);
    void addDesignerActions(QActionGroup* group);

    void addTab(QString formName);
    void removeTab(const QString &formName);
    void removeAllTabs();

    void setCurrentTab(const QString &formName);

    QString tabName();
    void setCurrentTabName(const QString &newName);
    void setNewFormName(const QString &formName, const QString &newName);

//    void setContent(QString formName, QString xmlContent);
//    QString getContent(QString formName);

public slots:
//    void setInitialActiveTab();
//    void setInitialActiveTabDelayed();
//    void updateTabs();
//    void delayedUpdateTabs();

signals:
    void requestForNewForm();
    void requestForDelete(QString formName);
    void requestForSave(QString formName, QString xmlContent);
    void requestFileContent(QString);
    void activeFormChanged(QString formName);
    void requestForRename(QString formName);

private slots:
    void slotNewFormClicked();
    void slotDeleteFormClicked();
    void slotTabChanged(int);
    void slotFromFileClicked();
    void slotTabDoubleClicked(int index);

private:
    int findIndex(QString formName);
    Form currentForm();

private:
    Ui::uicontainer *ui;
    UiEditor * m_editor;
    QWidget * m_designerWidget;
    QHash<QString, Form> m_forms;
    QString m_currentForm;
    QTimer * m_updateTabTimer;
//    QTimer * initialTabActivateTimer;
};

#endif // UICONTAINER_H
