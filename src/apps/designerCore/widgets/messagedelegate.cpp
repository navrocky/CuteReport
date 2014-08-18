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
#include "messagedelegate.h"
#include <QDebug>

MessageDelegate::MessageDelegate(QObject *parent)
    :QStyledItemDelegate(parent)
{
}

//void MessageDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index)
//{

//}

//QSize MessageDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
//{
//    QSize size = option.fontMetrics.boundingRect(index.model()->data(index).toString()).size();
//    QSize screenSize = option.rect.size();
//    QSize result;


//    if (size.height() > screenSize.height() && screenSize.height())
//        result =  QSize( -1, size.height() / screenSize.height());
//    else
//        result =  QStyledItemDelegate::sizeHint(option, index);

//    return result;
//}
