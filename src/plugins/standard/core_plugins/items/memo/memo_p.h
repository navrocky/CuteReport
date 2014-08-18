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
#include "memo.h"

class MemoItemPrivate : public CuteReport::ItemInterfacePrivate
{
public:
    MemoItemPrivate()
        :ItemInterfacePrivate(),
          stretchMode(MemoItem::DontStretch),
          delimiters("[ , ]"),
          stretchFont(false),
          allowHtml(false),
          textMargin(1), //mm
          textDocument(0),
          adjustedFontPointSize(0),
          originalHeight(-1),
          showStretchability(true)
    {}
    MemoItemPrivate(const MemoItemPrivate & p)
        :ItemInterfacePrivate(p),
          stretchMode(p.stretchMode),
          textFlags(p.textFlags),
          text(p.text),
          font(p.font),
          textColor(p.textColor),
          delimiters(p.delimiters),
          stretchFont(p.stretchFont),
          allowHtml(p.allowHtml),
          textMargin(p.textMargin),
          textDocument(0),
          adjustedFontPointSize(p.adjustedFontPointSize),
          originalHeight(p.originalHeight),
          showStretchability(p.showStretchability)
    {}
    virtual ~MemoItemPrivate(){
        delete textDocument;
    }

    MemoItem::StretchMode stretchMode;
    MemoItem::TextFlags textFlags;
    QString text;
    QFont font;
    QColor textColor;
    QString delimiters;
    bool stretchFont;
    bool allowHtml;
    qreal textMargin;
    QTextDocument * textDocument;
    qreal adjustedFontPointSize;
    qreal originalHeight;
    bool showStretchability;
};

QDataStream &operator<<(QDataStream &s, const MemoItemPrivate &p);
QDataStream &operator>>(QDataStream &s, MemoItemPrivate &p);

#endif // IMAGEITEM_P_H
