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
#include "messagemodel.h"

#include <QColor>

static const int myColumns = 2;

MessageModel::MessageModel(QObject *parent) :
    QAbstractTableModel(parent),
    m_limit(100)
{
}


void MessageModel::setMessageLimit(int limit)
{
    m_limit = limit;
    while (m_messages.count() > m_limit)
        m_messages.removeFirst();
}


void MessageModel::addMessage(CuteReport::LogLevel level, const QString & sender, const QString & shortMessage, const QString & fullMessage)
{
    Message msg;
    msg.level = level;
    msg.sender = sender;
    msg.shortMessage = shortMessage;
    msg.fullMessage = fullMessage;
    msg.showFullMessage = false;

    beginInsertRows(QModelIndex(), m_messages.count(), m_messages.count());
    m_messages.append(msg);
    endInsertRows();
    while (m_messages.count() > m_limit)
        m_messages.removeFirst();
}


void MessageModel::clear()
{
    beginResetModel();
    m_messages.clear();
    endResetModel();
}


bool MessageModel::isTextExtended(int row)
{
    return m_messages.at(row).showFullMessage;
}


void MessageModel::setTextExtended(int row, bool b)
{
    m_messages[row].showFullMessage = b;
    emit dataChanged(index(row, 0), index(row, myColumns));
}


QString MessageModel::extendedText(int row)
{
    return m_messages.at(row).fullMessage;
}


int MessageModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_messages.count();
}


int MessageModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return myColumns;
}


QVariant MessageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_messages.count() || index.row() < 0)
        return QVariant();

    Message msg = m_messages.at(index.row());

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
        case FieldSender: return msg.sender;
        case FieldMessage: return msg.showFullMessage ? msg.fullMessage : msg.shortMessage;
        }
    } else if (role == Qt::BackgroundColorRole) {
        switch (msg.level) {
        case CuteReport::LogInfo:     return QColor("#B7FFD2");
        case CuteReport::LogError:    return QColor("#FF6969");
        case CuteReport::LogWarning:  return QColor("#FFFF70");
        case CuteReport::LogCritical: return QColor("#FF3838");
        default:          return QColor(Qt::white);
        }
    } else if (role == AlternateTextRole) {
        return msg.showFullMessage;
    }

    return QVariant();
}


bool MessageModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() >= m_messages.count() || index.row() < 0)
        return false;

    if (role == AlternateTextRole) {
        Message msg = m_messages.at(index.row());
        msg.showFullMessage = value.toBool();
        emit dataChanged(this->index(index.row(), 0), this->index(index.row(), myColumns));
    }

    return true;
}


QVariant MessageModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case FieldSender:    return tr("Sender");
        case FieldMessage:  return tr("Message");
        }
    }

    return QVariant();
}


Qt::ItemFlags MessageModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled ;
}
