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
#ifndef MESSAGELIST_H
#define MESSAGELIST_H

#include <QWidget>
#include "types.h"

namespace Ui {
class MessageList;
}

class QTreeWidgetItem;
class MessageModel;

class MessageList : public QWidget
{
    Q_OBJECT
    
public:
    explicit MessageList(QWidget *parent = 0);
    ~MessageList();

    void addMessage(CuteReport::LogLevel level, const QString & sender, const QString & shortMessage, const QString & fullMessage);
    void clear();

private slots:
    void itemDoubleClicked(QModelIndex index);
    void itemActivated(QModelIndex index, QModelIndex);

private:
    Ui::MessageList *ui;
    MessageModel * m_model;
};

#endif // MESSAGELIST_H
