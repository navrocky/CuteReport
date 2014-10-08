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
#ifndef MEMOITEM_P_H
#define MEMOITEM_P_H

#include "iteminterface_p.h"
#include "iteminterface.h"
#include "memo.h"

SUIT_BEGIN_NAMESPACE

class TemplateRenderingStruct;
class MemoItemPrivate;

struct FlowToStruct {
    FlowToStruct(): flowToPrivate(0)/*, textPart1(0), textPart2(0)*/, vShift2(0), flowId(0),
    marginPixel(0), textAddonWidth(-1) {}
    ~FlowToStruct() {
        delete flowToPrivate;
    }

    MemoItemPrivate * flowToPrivate;
    QPointF textAddonPos;
    qreal  textAddonWidth;
    QRectF clipRect;
    qreal vShift2;
    int pos1;
    int pos2;
    quint32 flowId;
    qreal marginPixel;
};


class MemoItemPrivate : public CuteReport::ItemInterfacePrivate
{
public:
    MemoItemPrivate()
        :ItemInterfacePrivate(),
          stretchMode(MemoItem::DontStretch),
          delimiters("[ , ]"),
          stretchFont(false),
          allowHtml(false),
          allowExpressions(true),
          textMargin(1), //mm
          textDocument(0),
          fontScale(0),
          originalHeight(-1),
          showStretchability(true)
    {
    }

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
          allowExpressions(p.allowExpressions),
          textMargin(p.textMargin),
          textDocument(0),
          fontScale(p.fontScale),
          originalHeight(p.originalHeight),
          showStretchability(p.showStretchability),
          absoluteRect(p.absoluteRect),
          //
          textPos(p.textPos)
    {
    }

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
    bool allowExpressions;
    qreal textMargin;
    QTextDocument * textDocument;
    qreal fontScale;
    qreal originalHeight;
    bool showStretchability;

    // rendering
    QPointF posDeltaMM;
    QRectF absoluteRect;
    QPointF textPos;
    QRectF textClipRect;
    qreal sw;
};


QDataStream &operator<<(QDataStream &s, const MemoItemPrivate &p);
QDataStream &operator>>(QDataStream &s, MemoItemPrivate &p);


SUIT_END_NAMESPACE

#endif // MEMOITEM_P_H
