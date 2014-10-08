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
/***************************************************************************
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *   bogdan@licentia.eu                                                    *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include <QtCore>
#include <QtGui>

#include "barcode.h"
#include "barcode_p.h"
#include "rendererpublicinterface.h"
#include "zint/backend_qt4/qzint.h"
#include "barcodescripting.h"
#include "reportcore.h"

#define MODULENAME "BarcodeItem"

using namespace Zint;

inline void initResource() { Q_INIT_RESOURCE(barcode); }

BarcodeItem::BarcodeItem(QObject* parent)
    : ItemInterface(new BarcodeItemPrivate, parent)
{
    Q_INIT_RESOURCE(barcode);
    m_renderer = 0;
    Q_D(BarcodeItem);
    setResizeFlags( ResizeLeft | ResizeRight | ResizeTop | ResizeBottom);
    d->rect = QRectF(0,0,50,20);
    //    d->frame = 0;
}


BarcodeItem::BarcodeItem(BarcodeItemPrivate *dd, QObject * parent)
    :ItemInterface(dd, parent)
{
    Q_INIT_RESOURCE(barcode);
    m_renderer = 0;
}


void BarcodeItem::moduleInit()
{
    initResource();
}


BaseItemInterface *BarcodeItem::itemClone() const
{
    Q_D(const BarcodeItem);
    return new BarcodeItem(new BarcodeItemPrivate(*d), parent());
}


QByteArray BarcodeItem::serialize()
{
    Q_D(BarcodeItem);
    QByteArray ba;
    QDataStream s( &ba, QIODevice::ReadWrite );
    s << *d;
    return ba;
}


void BarcodeItem::deserialize(QByteArray &data)
{
    Q_D(BarcodeItem);
    QDataStream s( &data, QIODevice::ReadWrite );
    s >> *d;
}


QDataStream &operator<<(QDataStream &s, const BarcodeItemPrivate &p) {
    s << static_cast<const ItemInterfacePrivate&>(p);
//    s << p.pen << (qint16)p.startAngle << (qint16)p.spanAngle;
    return s;
}


QDataStream &operator>>(QDataStream &s, BarcodeItemPrivate &p) {
    s >> static_cast<ItemInterfacePrivate&>(p);
//    qint16 startAngle, spanAngle;

//    s >> p.pen; s >> startAngle; s >> spanAngle;
//    p.spanAngle = startAngle;
//    p.spanAngle = spanAngle;

    return s;
}


BaseItemInterface *BarcodeItem::createInstance(QObject * parent) const
{
    return new BarcodeItem(parent);
}


BaseItemHelperInterface *BarcodeItem::helper()
{
    return 0;
}


void BarcodeItem::init_gui()
{
    if (m_gui)
        return;
    m_gui = new BarcodeItemView(this);
}


bool BarcodeItem::canContain(QObject * object)
{
    return (dynamic_cast<CuteReport::BaseItemInterface*>(object));
}


void BarcodeItem::renderInit(RendererPublicInterface * renderer)
{
    Q_D(const BarcodeItem);

    m_renderer = renderer;
    m_renderer->registerEvaluationString(d->script, this);
    m_renderer->registerEvaluationString(d->primaryMessage, this);
}


void BarcodeItem::renderReset()
{
    m_renderer = 0;
}


bool BarcodeItem::renderPrepare()
{
    Q_D(BarcodeItem);
    emit printBefore();
    setRenderingPointer(new BarcodeItemPrivate(*(reinterpret_cast<BarcodeItemPrivate*>(d_ptr))));
    emit printDataBefore();
    d->script = m_renderer->processString(d->script, "[", "]", this);
    d->primaryMessage = m_renderer->processString(d->primaryMessage, "[", "]", this);
    emit printDataAfter();
    return true;
}


RenderedItemInterface *BarcodeItem::renderView()
{
    Q_D(BarcodeItem);
    RenderedBarcodeItem * view = new RenderedBarcodeItem(this, new BarcodeItemPrivate(*d));
    return view;
}


void BarcodeItem::paint(QPainter * painter, const QStyleOptionGraphicsItem *option, const BaseItemInterfacePrivate * data, const QRectF &boundingRect, RenderingType type)
{
    Q_UNUSED(option)
    Q_UNUSED(type)
    ItemInterface::paintBegin(painter, option, data, boundingRect, type);

    const BarcodeItemPrivate * const d = static_cast<const BarcodeItemPrivate*>(data);

    QString text = (type == RenderingReport) ? d->script : d->testText;

    Zint::QZint bc;

    bc.setSymbol(d->barcodeType);
//    switch(d->barcodeType)
//    {
//    case  MSI_PLESSEY:
//        bc.setMsiExtraSymbology(d->msiPlessey);
//        break;
//    case  CODE39:
//        bc.setCode39ExtraSymbology(d->exCode39_43Extension);
//        break;
//    case  EXCODE39:
//        bc.setExcode39ExtraSymbology(d->exCode39_43Extension);
//        break;
//    }
    bc.setPrimaryMessage(d->primaryMessage);
    bc.setBorderType((Zint::QZint::BorderType)d->frameType);
    bc.setHeight(d->barcodeHeight);
    bc.setWidth(d->barcodeWidth);
    bc.setSecurityLevel(d->barcodeSecurityLevel);
    bc.setBorderWidth(d->barcodeBoderWidth);
    bc.setPdf417CodeWords(d->pdf417Max);
    bc.setFgColor(d->barcodeColor);
    bc.setBgColor(QColor(0,0,0,0));
    bc.setText(text);

    bc.render(*painter,QRectF( boundingRect.x() + 1, boundingRect.y() +1 +((d->drawTextType==DRAW_ABOVE) ? painter->fontMetrics().height():0),
                               boundingRect.width() -2, boundingRect.height() -2 -((d->drawTextType==DRAW_ABOVE || d->drawTextType==DRAW_BELOW) ? painter->fontMetrics().height():0)),
                                (Zint::QZint::AspectRatioMode)d->paintType);

    switch (d->drawTextType)
    {
    case DRAW_TOP:
        painter->drawText(boundingRect, Qt::AlignTop| Qt::AlignHCenter, text);
        break;
    case DRAW_BOTTOM:
        painter->drawText(boundingRect, Qt::AlignBottom| Qt::AlignHCenter, text);
        break;
    case DRAW_ABOVE:
        painter->drawText(boundingRect, Qt::AlignTop| Qt::AlignHCenter, text);
        break;
    case DRAW_BELOW:
        painter->drawText(boundingRect, Qt::AlignBottom| Qt::AlignHCenter, text);
        break;
    default:
        break;
    }


    ItemInterface::paintEnd(painter, option, data, boundingRect, type);
}


BarcodeItem::BarcodeTypes BarcodeItem::barcodeType() const
{
    Q_D(const BarcodeItem);
    return d->barcodeType;
}


void BarcodeItem::setBarcodeType(BarcodeTypes barcodeType)
{
    Q_D(BarcodeItem);
    if (barcodeType == d->barcodeType)
        return;

    d->barcodeType = barcodeType;
    update_gui();
    emit barcodeTypeChanged(d->barcodeType);
    emit barcodeTypeChanged(BarcodeItem_BarcodeTypes_toString(d->barcodeType));
    emit changed();
}


QString BarcodeItem::barcodeTypeStr() const
{
    Q_D(const BarcodeItem);
    return BarcodeItem_BarcodeTypes_toString(d->barcodeType);
}


void BarcodeItem::setBarcodeTypeStr(const QString &barcodeType)
{
    bool ok;
    BarcodeItem::BarcodeTypes value = BarcodeItem_BarcodeTypes_fromString(barcodeType, &ok);
    if (ok)
        setBarcodeType(value);
}


BarcodeItem::PaintTypes BarcodeItem::paintType() const
{
    Q_D(const BarcodeItem);
    return d->paintType;
}


void BarcodeItem::setPaintType(PaintTypes paintType)
{
    Q_D(BarcodeItem);
    if (d->paintType == paintType)
        return;

    d->paintType = paintType;
    update_gui();

    emit paintTypeChanged(d->paintType);
    emit paintTypeChanged(BarcodeItem_PaintTypes_toString(d->paintType));
    emit changed();
}


QString BarcodeItem::paintTypeStr() const
{
    Q_D(const BarcodeItem);
    return BarcodeItem_PaintTypes_toString(d->paintType);
}


void BarcodeItem::setPaintTypeStr(const QString & paintType)
{
    bool ok;
    BarcodeItem::PaintTypes value = BarcodeItem_PaintTypes_fromString(paintType, &ok);
    if (ok)
        setPaintType(value);
}



BarcodeItem::FrameTypes BarcodeItem::frameType() const
{
    Q_D(const BarcodeItem);
    return d->frameType;
}


void BarcodeItem::setFrameType(FrameTypes frameType)
{
    Q_D(BarcodeItem);
    if(d->frameType == frameType)
        return;

    d->frameType = frameType;
    update_gui();
    emit frameTypeChanged(d->frameType);
    emit changed();
}


QString BarcodeItem::frameTypeStr() const
{
    Q_D(const BarcodeItem);
    return BarcodeItem_FrameTypes_toString(d->frameType);
}


void BarcodeItem::setFrameTypeStr(const QString &frameType)
{
    bool ok;
    BarcodeItem::FrameTypes value = BarcodeItem_FrameTypes_fromString(frameType, &ok);
    if (ok)
        setFrameType(value);
}


int BarcodeItem::barcodeHeight() const
{
    Q_D(const BarcodeItem);
    return d->barcodeHeight;
}


void BarcodeItem::setBarcodeHeight(int barcodeHeight)
{
    Q_D(BarcodeItem);
    if (d->barcodeHeight == barcodeHeight)
        return;

    d->barcodeHeight = barcodeHeight;
    update_gui();
    emit barcodeHeightChanged(d->barcodeHeight);
    emit changed();
}


int BarcodeItem::barcodeWhitespaceWidth() const
{
    Q_D(const BarcodeItem);
    return d->barcodeWhitespaceWidth;
}


void BarcodeItem::setBarcodeWhitespaceWidth(int barcodeWhitespaceWidth)
{
    Q_D(BarcodeItem);
    if (d->barcodeWhitespaceWidth == barcodeWhitespaceWidth)
        return;

    d->barcodeWhitespaceWidth = barcodeWhitespaceWidth;
    update_gui();
    emit barcodeWhitespaceWidthChanged(d->barcodeWhitespaceWidth);
    emit changed();
}


int BarcodeItem::barcodeBoderWidth() const
{
    Q_D(const BarcodeItem);
    return d->barcodeBoderWidth;
}


void BarcodeItem::setBarcodeBoderWidth(int barcodeBoderWidth)
{
    Q_D(BarcodeItem);
    if (d->barcodeBoderWidth == barcodeBoderWidth)
        return;

    d->barcodeBoderWidth = barcodeBoderWidth;
    update_gui();
    emit barcodeWidthChanged(d->barcodeWidth);
    emit changed();
}


QColor BarcodeItem::barcodeColor() const
{
    Q_D(const BarcodeItem);
    return d->barcodeColor;
}


void BarcodeItem::setBarcodeColor(QColor barcodeColor)
{
    Q_D(BarcodeItem);
    if (d->barcodeColor == barcodeColor)
        return;

    d->barcodeColor = barcodeColor;
    update_gui();
    emit barcodeColorChanged(d->barcodeColor);
    emit changed();
}


//QColor BarcodeItem::barcodeBackgroundColor() const
//{
//    Q_D(const BarcodeItem);
//    return d->barcodeBackgroundColor;
//}


//void BarcodeItem::setBarcodeBackgroundColor(QColor barcodeBackgroundColor)
//{
//    Q_D(BarcodeItem);
//    if (d->barcodeBackgroundColor == barcodeBackgroundColor)
//        return;
//    d->barcodeBackgroundColor = barcodeBackgroundColor;
//    update_gui();
//    emit barcodeBackgroundColorChanged(d->barcodeBackgroundColor);
//}


int BarcodeItem::barcodeWidth() const
{
    Q_D(const BarcodeItem);
    return d->barcodeWidth;
}


void BarcodeItem::setBarcodeWidth(int barcodeWidth)
{
    Q_D(BarcodeItem);
    if (d->barcodeWidth == barcodeWidth)
        return;

    d->barcodeWidth = barcodeWidth;

    if (d->barcodeWidth<0)
        d->barcodeWidth=0;
    if (d->barcodeWidth>100)
        d->barcodeWidth=100;
    update_gui();
    emit barcodeWidthChanged(d->barcodeWidth);
    emit changed();
}


int BarcodeItem::barcodeSecurityLevel() const
{
    Q_D(const BarcodeItem);
    return d->barcodeSecurityLevel;
}


void BarcodeItem::setBarcodeSecurityLevel(int barcodeSecurityLevel)
{
    Q_D(BarcodeItem);
    if (d->barcodeSecurityLevel == barcodeSecurityLevel)
        return;

    d->barcodeSecurityLevel = barcodeSecurityLevel;
    update_gui();
    emit barcodeSecurityLevelChanged(d->barcodeSecurityLevel);
    emit changed();
}


int BarcodeItem::pdf417Max() const
{
    Q_D(const BarcodeItem);
    return d->pdf417Max;
}


void BarcodeItem::setPdf417Max(int pdf417Max)
{
    Q_D(BarcodeItem);
    if (d->pdf417Max == pdf417Max)
        return;

    d->pdf417Max = pdf417Max;

//	if (d->pdf417Max<928)
//		d->pdf417Max=928;
//	if (d->pdf417Max>1900)
//		d->pdf417Max=1900;
    update_gui();
    emit pdf417MaxChanged(d->pdf417Max);
    emit changed();
}


BarcodeItem::MsiPlessey BarcodeItem::msiPlessey() const
{
    Q_D(const BarcodeItem);
    return d->msiPlessey;
}


void BarcodeItem::setMsiPlessey(MsiPlessey msiPlessey)
{
    Q_D(BarcodeItem);
    if (d->msiPlessey == msiPlessey)
        return;

    d->msiPlessey = msiPlessey;
    update_gui();
    emit msiPlesseyChanged(d->msiPlessey);
    emit msiPlesseyChanged(BarcodeItem_MsiPlessey_toString(d->msiPlessey));
    emit changed();
}


QString BarcodeItem::msiPlesseyStr() const
{
    Q_D(const BarcodeItem);
    return BarcodeItem_MsiPlessey_toString(d->msiPlessey);
}


void BarcodeItem::setMsiPlesseyStr(const QString &msiPlessey)
{
    bool ok;
    BarcodeItem::MsiPlessey value = BarcodeItem_MsiPlessey_fromString(msiPlessey, &ok);
    if (ok)
        setMsiPlessey(value);
}


bool BarcodeItem::exCode39_43Extension() const
{
    Q_D(const BarcodeItem);
    return d->exCode39_43Extension;
}


void BarcodeItem::setExCode39_43Extension(bool exCode39_43Extension)
{
    Q_D(BarcodeItem);
    if (d->exCode39_43Extension == exCode39_43Extension)
        return;

    d->exCode39_43Extension = exCode39_43Extension;
    update_gui();
    emit exCode39_43ExtensionChanged(d->exCode39_43Extension);
    emit changed();
}


BarcodeItem::DrawTextTypes BarcodeItem::drawTextType() const
{
    Q_D(const BarcodeItem);
    return d->drawTextType;
}


void BarcodeItem::setDrawTextType(DrawTextTypes drawTextType)
{
    Q_D(BarcodeItem);
    if (d->drawTextType == drawTextType)
        return;
    d->drawTextType = drawTextType;
    update_gui();
    emit drawTextTypeChanged(d->drawTextType);
    emit drawTextTypeChanged(d->drawTextType);
    emit changed();
}


QString BarcodeItem::drawTextTypeStr() const
{
    Q_D(const BarcodeItem);
    return BarcodeItem_DrawTextTypes_toString(d->drawTextType);
}


void BarcodeItem::setDrawTextTypeStr(const QString &msiPlessey)
{
    bool ok;
    BarcodeItem::DrawTextTypes value = BarcodeItem_DrawTextTypes_fromString(msiPlessey, &ok);
    if (ok)
        setDrawTextType(value);
}


const QString & BarcodeItem::primaryMessage() const
{
    Q_D(const BarcodeItem);
    return d->primaryMessage;
}


void BarcodeItem::setPrimaryMessage(const QString &primaryMessage)
{
    Q_D(BarcodeItem);
    if (d->primaryMessage == primaryMessage)
        return;

    d->primaryMessage = primaryMessage;
    update_gui();
    emit primaryMessageChanged(d->primaryMessage);
    emit changed();
}


QString BarcodeItem::script() const
{
    Q_D(const BarcodeItem);
    return d->script;
}


void BarcodeItem::setScript(const QString &script)
{
    Q_D(BarcodeItem);
    if (d->script == script)
        return;

    d->script = script;
    update_gui();
    emit scriptChanged(d->script);
    emit changed();
}


QString BarcodeItem::testText() const
{
    Q_D(const BarcodeItem);
    return d->testText;
}


void BarcodeItem::setTestText(const QString & testText)
{
    Q_D(BarcodeItem);
    if (d->testText == testText)
        return;

    d->testText = testText;
    update_gui();
    emit testTextChanged(d->testText);
    emit changed();
}


QStringList BarcodeItem::scriptingStrings()
{
    Q_D(BarcodeItem);
    return QStringList() << d->script << d->primaryMessage;
}


void BarcodeItem::initScript(QScriptEngine *scriptEngine)
{
    registerBarcodeItemScriptClass(scriptEngine);
}


QStringList BarcodeItem::_paintType_variants() const
{
    return BarcodeItem_PaintTypes_variants();
}


QStringList BarcodeItem::_frameType_variants() const
{
    return BarcodeItem_FrameTypes_variants();
}


QStringList BarcodeItem::_barcodeType_variants() const
{
    return BarcodeItem_BarcodeTypes_variants();
}


QStringList BarcodeItem::_msiPlessey_variants() const
{
    return BarcodeItem_MsiPlessey_variants();
}


QStringList BarcodeItem::_drawTextType_variants() const
{
    return BarcodeItem_DrawTextTypes_variants();
}


QIcon BarcodeItem::itemIcon() const
{
	return QIcon(":/barcode.png");
}


QString BarcodeItem::moduleShortName() const
{
	return tr("Barcode");
}


QString BarcodeItem::itemGroup() const
{
	return tr("Shapes");
}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Barcode, BarcodeItem)
#endif
