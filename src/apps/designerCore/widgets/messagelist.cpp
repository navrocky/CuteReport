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
#include "messagelist.h"
#include "ui_messagelist.h"
#include "messagemodel.h"
#include "messagedelegate.h"
//#include <QAbstractItemDelegate>

MessageList::MessageList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessageList)
{
    ui->setupUi(this);

    m_model = new MessageModel(this);
    ui->list->setModel(m_model);
//    ui->list->setItemDelegate( new MessageDelegate(ui->list));

    connect(ui->list, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(itemDoubleClicked(QModelIndex)));
    connect(ui->list->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(itemActivated(QModelIndex, QModelIndex)));
}


MessageList::~MessageList()
{
    delete ui;
}


void MessageList::addMessage(CuteReport::LogLevel level, const QString & sender, const QString & shortMessage, const QString & fullMessage)
{
    if (level == CuteReport::LogDebug)
        return;

    m_model->addMessage(level, sender, shortMessage, fullMessage);
}


void MessageList::clear()
{
    m_model->clear();
}


void MessageList::itemDoubleClicked(QModelIndex index)
{
    Q_UNUSED(index)
//    bool currentValue = m_model->isTextExtended(index.row());
//    m_model->setTextExtended(index.row(), !currentValue);
}


void MessageList::itemActivated(QModelIndex index, QModelIndex)
{
    ui->fullText->setText(m_model->extendedText(index.row()));
}
