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
 **                  GNU Lesser General Public License                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library.                                      *
 *   If not, see <http://www.gnu.org/licenses/>.                           *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/
#ifndef IMAGEITEM_P_H
#define IMAGEITEM_P_H

#include "iteminterface_p.h"
#include "iteminterface.h"
#include "imageitem.h"

class ImageItemPrivate : public CuteReport::ItemInterfacePrivate
{
public:
    ImageItemPrivate()
        : ItemInterfacePrivate(),
          scaleType(ImageItem::ScaleImage),
          sourceType(ImageItem::Static),
          keepAspectRatio(true),
          center(true)
    {}
    ImageItemPrivate(const ImageItemPrivate & p)
        :ItemInterfacePrivate(p),
          source(p.source),
          scaleType(p.scaleType),
          sourceType(p.sourceType),
          keepAspectRatio(p.keepAspectRatio),
          center(p.center)
    {
        if (sourceType == ImageItem::Static)
            image = p.image;
    }
    virtual ~ImageItemPrivate(){}

    QImage image;
    QString source;
    ImageItem::ScaleType scaleType;
    ImageItem::SourceType sourceType;
    bool keepAspectRatio;
    bool center;
};

QDataStream &operator<<(QDataStream &s, const ImageItemPrivate &p);
QDataStream &operator>>(QDataStream &s, ImageItemPrivate &p);

#endif // IMAGEITEM_P_H
