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
#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QAbstractTableModel>
#include "types.h"

const int AlternateTextRole = Qt::UserRole + 2;

struct Message {
    int level;
    QString sender;
    QString shortMessage;
    QString fullMessage;
    bool showFullMessage;
};


class MessageModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum Fields {FieldSender = 0, FieldMessage = 1};
    explicit MessageModel(QObject *parent = 0);

    void addMessage(CuteReport::LogLevel level, const QString & sender, const QString & shortMessage, const QString & fullMessage);
    void clear();
    void setMessageLimit(int limit);
    bool isTextExtended(int row);
    void setTextExtended(int row, bool b);
    QString extendedText(int row);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    
private:
    QList<Message> m_messages;
    int m_limit;
};

#endif // MESSAGEMODEL_H
