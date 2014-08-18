/***************************************************************************
 *   This file is part of the CuteReport project                           *
 *   Copyright (C) 2012 by Alexander Mikhalov                              *
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

#ifndef BARCODE_P_H
#define BARCODE_P_H

#include "iteminterface_p.h"
#include "iteminterface.h"
#include "barcode.h"

using namespace CuteReport;

class BarcodeItemPrivate : public ItemInterfacePrivate
{
public:
    BarcodeItemPrivate()
        :ItemInterfacePrivate(),
        barcodeType(BarcodeItem::CODE128),
        paintType(BarcodeItem::KeepAspectRatio),
        frameType(BarcodeItem::NO_FRAME),
        barcodeHeight(15),
        barcodeWhitespaceWidth(0),
        barcodeBoderWidth(0),
        barcodeColor(Qt::black),
        barcodeWidth(0),
        barcodeSecurityLevel(-1),
        drawTextType(BarcodeItem::NO_TEXT),
        pdf417Max(928),
        script("05072006"),
        testText("05072006"),
        exCode39_43Extension(false),
        msiPlessey(BarcodeItem::MSI_PLESSEY_)
    {}

    BarcodeItemPrivate(const BarcodeItemPrivate & p)
        :ItemInterfacePrivate(p),
          barcodeType(p.barcodeType),
          paintType(p.paintType),
          frameType(p.frameType),
          barcodeHeight(p.barcodeHeight),
          barcodeWhitespaceWidth(p.barcodeWhitespaceWidth),
          barcodeBoderWidth(p.barcodeBoderWidth),
          barcodeColor(p.barcodeColor),
          barcodeWidth(p.barcodeWidth),
          barcodeSecurityLevel(p.barcodeSecurityLevel),
          drawTextType(p.drawTextType),
          pdf417Max(p.pdf417Max),
          script(p.script),
          testText(p.testText),
          exCode39_43Extension(p.exCode39_43Extension),
          msiPlessey(p.msiPlessey)
    {}
    virtual ~BarcodeItemPrivate(){}

    BarcodeItem::BarcodeTypes barcodeType;
    BarcodeItem::PaintTypes paintType;
    BarcodeItem::FrameTypes frameType;
    int barcodeHeight;
    int barcodeWhitespaceWidth;
    int barcodeBoderWidth;
    QColor barcodeColor;
//    QColor barcodeBackgroundColor;
    int barcodeWidth;
    int barcodeSecurityLevel;
    BarcodeItem::DrawTextTypes drawTextType;
    int pdf417Max;
    QString primaryMessage;
    QString script;
    QString testText;
    bool	exCode39_43Extension;
    BarcodeItem::MsiPlessey msiPlessey;
};

QDataStream &operator<<(QDataStream &s, const BarcodeItemPrivate &p);
QDataStream &operator>>(QDataStream &s, BarcodeItemPrivate &p);

#endif // BARCODE_P_H
