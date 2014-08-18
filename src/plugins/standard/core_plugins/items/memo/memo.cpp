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
#include "memo.h"
#include "memohelper.h"
#include "memo_p.h"
#include "pageinterface.h"
#include "rendererpublicinterface.h"
#include "renderedmemoitem.h"
#include "memoitemscript.h"

using namespace CuteReport;

MemoItem::MemoItem(QObject * parent) :
    ItemInterface(*new MemoItemPrivate, parent),
    m_renderer(0)
{
    Q_D(MemoItem);
    setResizeFlags( ResizeLeft | ResizeRight | ResizeTop | ResizeBottom);
    d->rect = QRectF(0,0,30,8);
    d->frame = 0;
    d->text = "Memo";
    setTextFlags(AlignLeft | AlignVCenter);
}


MemoItem::MemoItem(MemoItemPrivate &dd, QObject * parent)
    :ItemInterface(dd, parent),
      m_renderer(0)
{
}


MemoItem::~MemoItem()
{
}


void MemoItem::moduleInit()
{
    //    qRegisterMetaType<MemoItem::StretchMode>("MemoItem::StretchMode");
    //    qRegisterMetaTypeStreamOperators<int>("MemoItem::StretchMode");
}


BaseItemInterface *MemoItem::clone()
{
    Q_D(MemoItem);
    return new MemoItem(*new MemoItemPrivate(*d), parent());
}


QByteArray MemoItem::serialize()
{
    Q_D(MemoItem);
    QByteArray ba;
    QDataStream s( &ba, QIODevice::ReadWrite );
    s << *d;
    return ba;
}


void MemoItem::deserialize(QByteArray &data)
{
    Q_D(MemoItem);
    QDataStream s( &data, QIODevice::ReadWrite );
    s >> *d;
}


QDataStream &operator<<(QDataStream &s, const MemoItemPrivate &p) {
    s << static_cast<const ItemInterfacePrivate&>(p);
    s << (qint8)p.stretchMode << (qint32)p.textFlags << p.text << p.font << p.textColor << p.delimiters
      << p.stretchFont << p.allowHtml << p.originalHeight << p.showStretchability;
    return s;
}


QDataStream &operator>>(QDataStream &s, MemoItemPrivate &p) {
    s >> static_cast<ItemInterfacePrivate&>(p);
    qint8 sizePolicy;
    qint32 textFlags;
    s >> sizePolicy; s >> textFlags;
    p.stretchMode = (MemoItem::StretchMode)sizePolicy;
    p.textFlags = (MemoItem::TextFlags)textFlags;
    s >> p.text; s >> p.font; s >> p.textColor; s >> p.delimiters;
    s >> p.stretchFont; s >> p.allowHtml; s >> p.originalHeight; s >> p.showStretchability;
    return s;
}


BaseItemInterface *MemoItem::createInstance(QObject * parent) const
{
    return new MemoItem(parent);
}


void MemoItem::init_gui()
{
    if (m_gui)
        return;
    m_gui = new MemoItemView(this);
}


void MemoItem::update_gui()
{
    if (!m_gui )
        return;

    ItemInterface::update_gui();

    Q_D(MemoItem);
    QRectF currentRect = d->rect;
    adjust(d, absoluteGeometry(Millimeter).topLeft());
    if (currentRect != d->rect) {
        emit geometryChanged(d->rect);
        emit changed();
    }
    m_gui->update();
}


void MemoItem::adjust(MemoItemPrivate * d, const QPointF &posDeltaMM, RenderingType type)
{
    Q_UNUSED(type)

    QRectF qrect = d->rect.translated(posDeltaMM);

    if (d->stretchFont && d->stretchMode == DontStretch) {
        //pix = f.pointSizeF()/72*dpi;
        //qreal marginInch = convertUnit(d->textMargin, Millimeter, Inch, d->dpi);
        //qreal margin = convertUnit(d->textMargin, d->unit, Inch, d->dpi);
        QRectF fontFrame = qrect;
        //fontFrame.setTop(marginInch);
        d->adjustedFontPointSize = convertUnit( fontFrame, d->unit, Pixel, d->dpi).height() * 0.90;
    } else {
        d->adjustedFontPointSize = d->font.pointSizeF() / 72 * d->dpi;
    }

    if (d->stretchMode != DontStretch || d->allowHtml) {
        if (!d->textDocument)
            d->textDocument = new QTextDocument();
        int marginPixel = convertUnit(d->textMargin, Millimeter, Pixel, d->dpi);
        QFont font = fontPrepared(d->font, d->dpi);
        d->textDocument->setDocumentMargin(marginPixel);
        d->textDocument->setDefaultFont(font);
        d->textDocument->setTextWidth(convertUnit(qrect, Millimeter, Pixel, d->dpi).width());
        if (d->allowHtml)
            d->textDocument->setHtml(d->text);
        else
            d->textDocument->setPlainText(d->text);

        if (d->stretchMode == ActualHeight && d->showStretchability) {
            //        if (d->stretchMode == ActualHeight) {
            qreal textHeight = convertUnit(d->textDocument->size().height(), Pixel, Millimeter, d->dpi);
            if (textHeight > d->rect.height()) {
                if (d->originalHeight == -1) // not stretched state
                    d->originalHeight = d->rect.height();
                d->rect.setHeight(textHeight);
            }
        }

    } else {
        delete d->textDocument;
        d->textDocument = 0;
    }

    if ((d->stretchMode == DontStretch || !d->showStretchability) && d->originalHeight != -1) { // restoring original height
        //     if (d->stretchMode == DontStretch  && d->originalHeight != -1) { // restoring original height
        d->rect.setHeight(d->originalHeight);
        d->originalHeight = -1;
    }
}


bool MemoItem::canContain(QObject * object)
{
    return (dynamic_cast<CuteReport::BaseItemInterface*>(object));
}


QIcon MemoItem::itemIcon() const
{
    return QIcon(":/memo.png");
}


QString MemoItem::moduleName() const
{
    return tr("Memo");
}


QString MemoItem::itemGroup() const
{
    return QString("Shapes");
}


void MemoItem::setGeometry(const QRectF & rect, Unit unit)
{
    Q_D(MemoItem);
    ItemInterface::setGeometry(rect, unit);
    d->originalHeight = -1;     // means not stretching, but real geometry
}


void MemoItem::setHeight(qreal height, Unit unit)
{
    Q_D(MemoItem);
    ItemInterface::setHeight(height, unit);
    d->originalHeight = -1;     // means not stretching, but real geometry
}


MemoItem::StretchMode MemoItem::stretchMode() const
{
    Q_D(const MemoItem);
    return d->stretchMode;
}


void MemoItem::setStretchMode(StretchMode sizePolicy)
{
    Q_D(MemoItem);
    if (sizePolicy != d->stretchMode) {
        d->stretchMode = sizePolicy;
        update_gui();
        emit stretchModeChanged(d->stretchMode);
        emit changed();
    }
}


QString MemoItem::stretchModeStr() const
{
    Q_D(const MemoItem);
    return MemoItem_StretchMode_toString(d->stretchMode);
}


void MemoItem::setStretchModeStr(const QString &stretchMode)
{
    bool ok;
    MemoItem::StretchMode value = MemoItem_StretchMode_fromString(stretchMode, &ok);
    qDebug() << stretchMode << value << ok;
    if (ok)
        setStretchMode(value);
}


bool MemoItem::showStretchability() const
{
    Q_D(const MemoItem);
    return d->showStretchability;
}


void MemoItem::setShowStretchability(bool value)
{
    Q_D(MemoItem);
    if (d->showStretchability == value)
        return;
    d->showStretchability = value;

    if (!m_inited)
        return;

    update_gui();

    emit showStretchabilityChanged(d->showStretchability);
    emit changed();
}



bool MemoItem::allowHTML() const
{
    Q_D(const MemoItem);
    return d->allowHtml;
}


void MemoItem::setAllowHTML(bool value)
{
    Q_D(MemoItem);
    if (d->allowHtml == value)
        return;
    d->allowHtml = value;
    update_gui();
    emit allowHTMLChanged(d->allowHtml);
    emit changed();
}


qreal MemoItem::textMargin() const
{
    Q_D(const MemoItem);
    return convertUnit(d->textMargin, Millimeter, d->unit, d->dpi);
}


void MemoItem::setTextMargin(qreal value)
{
    Q_D(MemoItem);

    qreal newValue = convertUnit(value, d->unit, Millimeter, d->dpi);
    if (d->textMargin == newValue)
        return;

    d->textMargin = newValue;
    update_gui();

    emit textMarginChaged(d->textMargin);
    emit changed();
}


MemoItem::TextFlags MemoItem::textFlags() const
{
    Q_D(const MemoItem);
    return d->textFlags;
}


void MemoItem::setTextFlags(MemoItem::TextFlags textFlags)
{
    Q_D(MemoItem);
    if (d->textFlags != textFlags) {
        d->textFlags = textFlags;
        update_gui();
        emit textFlagsChanged(d->textFlags);
        emit changed();
    }
}


QString MemoItem::text() const
{
    Q_D(const MemoItem);
    return d->text;
}


void MemoItem::setText(const QString &text)
{
    Q_D(MemoItem);
    if (d->text != text) {
        d->text = text;
        update_gui();
        emit textChanged(text);
        emit scriptingStringsChanged();
        emit changed();
    }
}


QFont MemoItem::font() const
{
    Q_D(const MemoItem);
    return d->font;
}


void MemoItem::setFont(const QFont & font)
{
    Q_D(MemoItem);
    if (d->font != font) {
        d->font = font;
        update_gui();
        emit fontChanged(d->font);
        emit changed();
    }
}


QColor MemoItem::textColor() const
{
    Q_D(const MemoItem);
    return d->textColor;
}


void MemoItem::setTextColor(const QColor & color)
{
    Q_D(MemoItem);
    if (d->textColor != color) {
        d->textColor = color;
        update_gui();
        emit textColorChaged(d->textColor);
        emit changed();
    }
}


bool MemoItem::stretchFont() const
{
    Q_D(const MemoItem);
    return d->stretchFont;
}


void MemoItem::setStretchFont(bool b)
{
    Q_D(MemoItem);
    if (d->stretchFont == b)
        return;

    d->stretchFont = b;
    update_gui();
    emit stretchFontChanged(b);
    emit changed();
}


QString MemoItem::delimiters() const
{
    Q_D(const MemoItem);
    return d->delimiters;
}


void MemoItem::setDelimiters(const QString & delimiters)
{
    Q_D(MemoItem);
    if (d->delimiters != delimiters) {
        d->delimiters = delimiters;
        update_gui();
        emit delimitersChanged(d->delimiters);
        emit changed();
    }
}


void MemoItem::renderInit(RendererPublicInterface * renderer)
{
    Q_D(const MemoItem);

    m_renderer = renderer;
    m_renderer->registerEvaluationString(d->text, this);
}


void MemoItem::renderReset()
{
    m_renderer = 0;
}


CuteReport::RenderedItemInterface * MemoItem::render(int customDPI)
{
    Q_UNUSED(customDPI);
    Q_D(MemoItem);

    emit renderingBefore();

    MemoItemPrivate * pCurrent = d;
    MemoItemPrivate * pNew = new MemoItemPrivate(*d);

    d_ptr = pNew;
    emit rendering();
    d_ptr = pCurrent;

    RenderedMemoItem * view = new RenderedMemoItem(this, pNew);

    emit rendered(view);
    emit renderingAfter();

    return view;
}


void MemoItem::paint(QPainter * painter, const QStyleOptionGraphicsItem *option, const BaseItemInterfacePrivate * data, const QRectF &boundingRect, RenderingType type)
{
    Q_UNUSED(option)
    Q_UNUSED(type)

    ItemInterface::paintBegin(painter, option, data, boundingRect, type);

    const MemoItemPrivate * const d = static_cast<const MemoItemPrivate*>(data);
    painter->setPen(d->textColor);

    QRect rect = boundingRect.toRect();

    painter->setClipRect( rect );
    painter->translate( rect.topLeft() );

    if (!d->textDocument) {
        if (d->adjustedFontPointSize > 3) {
            QFont font = d->font;
            font.setPixelSize(d->adjustedFontPointSize);
            painter->setFont(font);
            int margin = 0;
            painter->drawText(margin, margin, rect.width() - margin*2, rect.height() - margin*2, d->textFlags, d->text);
        }
    } else {
        d->textDocument->drawContents(painter, QRectF(QPointF(0,0), rect.size()));
    }

    ItemInterface::paintEnd(painter, option, data, boundingRect, type);
}


BaseItemHelperInterface *MemoItem::helper()
{
    if (!m_helper)
        m_helper = new MemoHelper(this);

    return m_helper;
}


QStringList MemoItem::scriptingStrings()
{
    Q_D(MemoItem);
    return QStringList() << d->text;
}


void MemoItem::initScript(QScriptEngine *scriptEngine)
{
    registerMemoItemScriptClass(scriptEngine);
}


QStringList MemoItem::_stretchMode_variants() const
{
    return MemoItem_StretchMode_variants();
}


QString MemoItem::_current_property_description() const
{
    QString propertyName = metaObject()->property(m_currentProperty).name();

    if (propertyName == "textFlags")
        return tr("text arrangement and look flags");
    else if (propertyName == "stretchMode")
        return tr("Set item stretchability mode");
    else if (propertyName == "showStretchability")
        return tr("Should Page editor show item stretchable (if set) or no");
    else if (propertyName == "allowHTML")
        return tr("If yes item will recognize HTML tags");
    else if (propertyName == "text")
        return tr("Actual item's text. Text set between expression delimiters will be processed in script engine");
    else if (propertyName == "textColor")
        return tr("Color of the text");
    else if (propertyName == "textMargin")
        return tr("Margin beetween text and border in current unit");
    else if (propertyName == "font")
        return tr("Text font");
    else if (propertyName == "stretchFont")
        return tr("If Yes, font will change its size to fit item border");
    else if (propertyName == "expDelimiters")
        return tr("Expression delimiters");

    return ItemInterface::_current_property_description();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(memo, MemoItem)
#endif
