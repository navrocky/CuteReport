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
#ifndef ITEMSTOOLBAR_H
#define ITEMSTOOLBAR_H

#include <QToolBar>
#include "QMenu"
#include "mainwindow.h"

class Menu;

class ItemsToolWidget : public QToolBar
{
    Q_OBJECT
public:
    ItemsToolWidget(QWidget * parent);
    void addItem(const QIcon &icon, const QString &name, const QString &className, const QString &group);

private slots:
    void showItems();
    void tryHideMenu();

protected:
    void leaveEvent ( QEvent * event );
//    Qt::Orientation orientation ();

private:
    QHash<QString, Menu*> menus;
    Menu * m_menu;
};

class Menu : public QMenu
{
    Q_OBJECT
public:
    Menu(QWidget * parent = 0);
protected:
    void leaveEvent ( QEvent * event );
    void mousePressEvent ( QMouseEvent * event );
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent ( QDragLeaveEvent * /*event*/ );
};


#endif // ITEMSTOOLBAR_H
