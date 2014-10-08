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
#include "memoitemview.h"

using namespace CuteReport;

inline void initMyResource() { Q_INIT_RESOURCE(memo); }

SUIT_BEGIN_NAMESPACE

MemoItem::MemoItem(QObject * parent) :
    ItemInterface(new MemoItemPrivate, parent),
    m_renderer(0)
{
    Q_D(MemoItem);
    setResizeFlags( ResizeLeft | ResizeRight | ResizeTop | ResizeBottom);
    d->rect = QRectF(0,0,30,8);
    d->frame = 0;
    d->text = "Memo";
    setTextFlags(AlignLeft | AlignVCenter);
}


MemoItem::MemoItem(MemoItemPrivate *dd, QObject * parent)
    :ItemInterface(dd, parent),
      m_renderer(0)
{
}


MemoItem::~MemoItem()
{
}


void MemoItem::moduleInit()
{
    initMyResource();
}

void MemoItem::init()
{
    Q_D(MemoItem);
    ItemInterface::init();
    adjust();
}


BaseItemInterface *MemoItem::itemClone() const
{
    Q_D(const MemoItem);
    MemoItemPrivate * newD = new MemoItemPrivate(*d);
    return new MemoItem(newD, parent());
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
      << p.stretchFont << p.allowHtml << p.allowExpressions << p.originalHeight << p.showStretchability;
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
    s >> p.stretchFont; s >> p.allowHtml; s >> p.allowExpressions >> p.originalHeight; s >> p.showStretchability;

    delete p.textDocument;
    p.textDocument = 0;

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
    m_gui = new SUIT_NAMESPACE::MemoItemView(this);
}


void MemoItem::update_gui()
{
    if (!m_gui )
        return;

    ItemInterface::update_gui();

    Q_D(MemoItem);
    QRectF currentRect = d->rect;
    adjust();
    if (currentRect != d->rect) {
        emit geometryChanged(d->rect);
        emit changed();
    }
    m_gui->update();
}


void MemoItem::adjust()
{
    Q_D(MemoItem);

    d->absoluteRect = absoluteGeometry(Millimeter);

    if (!page())
        return;
    adjust(d, absoluteGeometry(Millimeter).topLeft());

    emit adjusted();
}


void MemoItem::adjust(MemoItemPrivate * d, const QPointF &posDeltaMM)
{
    d->posDeltaMM = posDeltaMM;
    QRectF qrect = d->rect.translated(posDeltaMM);
    QRectF absRectPix = convertUnit(d->absoluteRect, Millimeter, Pixel, d->dpi);
    qreal marginPixel = convertUnit(d->textMargin, Millimeter, Pixel, d->dpi);
    qreal bottomMargin = marginPixel;
    qreal shift = marginPixel;
    d->sw = (d->dpi == 0) ? 1 : (qreal)d->dpi / (qreal)qApp->desktop()->logicalDpiY();
    qreal textWidth = d->textDocument ? d->textDocument->textWidth() : -1;

    qreal x = marginPixel;
    qreal y = shift;

    QTextDocument * oldDoc = d->textDocument ? d->textDocument->clone() : 0;
    delete d->textDocument;
    if (oldDoc) {
        d->textDocument = oldDoc;
    } else {
        d->textDocument = new QTextDocument();
        if (d->allowHtml)
            d->textDocument->setHtml(d->text);
        else {
            d->textDocument->setPlainText(d->text);
        }
        d->textDocument->setDocumentMargin(0);
    }
    d->textDocument->setDefaultFont(d->font);
    d->textDocument->setDefaultTextOption(QTextOption(Qt::Alignment((int)d->textFlags)));

    if (d->textDocument) {
        d->textDocument->setTextWidth( (textWidth == -1) ? (convertUnit(qrect, Millimeter, Pixel, d->dpi).width() - marginPixel*2) / d->sw : textWidth);

        if (d->stretchFont && d->stretchMode == DontStretch) {
            y = 0;
            QFont font = d->font;
            font.setPixelSize((absRectPix.height() - 2*marginPixel)/ d->sw * 0.85);
            d->textDocument->setDefaultFont(font);
        }

        QSizeF docSize = d->textDocument->size();

        if (d->textFlags.testFlag(AlignHCenter))
            x = qMax(((qreal)absRectPix.width() - docSize.width() * d->sw )/2, x);
        if (d->textFlags.testFlag(AlignVCenter))
            y = qMax(((qreal)absRectPix.height() - docSize.height() * d->sw )/2, y);

        d->textPos = QPointF(x,y);
        // FIXME: +2 unknown gap at the bottom sometimes
        d->textClipRect = QRectF(0, 0, qMin(docSize.width(), (absRectPix.width() - bottomMargin) / d->sw),
                                 qMax((absRectPix.height() - bottomMargin - y) / d->sw, (absRectPix.height() - bottomMargin - y) / d->sw) +2);
    }

    if ((d->stretchMode == ActualHeight && d->renderingType == RenderingReport)
            || (d->stretchMode == ActualHeight && d->renderingType == RenderingTemplate && d->showStretchability)) {
        if (textWidth == -1) {
            qreal topMargin = 0;
            qreal textAddon = 0;
            topMargin = marginPixel;
            qreal mainText = (d->textDocument && !d->textDocument->toPlainText().isEmpty()) ? d->textDocument->size().height() * d->sw : 0;
            qreal textHeightPix = topMargin + textAddon + mainText + bottomMargin;
            if (textHeightPix >= absRectPix.height()) {
                qreal textHeightMM = convertUnit(QRectF(QPointF(0,0), QSizeF(0, textHeightPix)), Pixel, Millimeter, d->dpi).height();
                if (d->originalHeight == -1) {// not stretched state
                    d->originalHeight = d->rect.height();
                    d->rect.setHeight(textHeightMM);
                }
            }
        }
    }

    if (d->renderingType == RenderingTemplate && (d->stretchMode == DontStretch || !d->showStretchability) && d->originalHeight != -1) { // restoring original height
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
    return QIcon(":/images/memo.png");
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
    resetDocumentWidth();
}


void MemoItem::setHeight(qreal height, Unit unit)
{
    Q_D(MemoItem);
    ItemInterface::setHeight(height, unit);
    d->originalHeight = -1;     // means not stretching, but real geometry
    resetDocumentWidth();
}


MemoItem::StretchMode MemoItem::stretchMode() const
{
    Q_D(const MemoItem);
    return d->stretchMode;
}


void MemoItem::setStretchMode(StretchMode sizePolicy)
{
    Q_D(MemoItem);
    if (sizePolicy == d->stretchMode)
        return;

    d->stretchMode = sizePolicy;

    update_gui();

    emit stretchModeChanged(d->stretchMode);
    emit changed();
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


bool MemoItem::allowExpressions() const
{
    Q_D(const MemoItem);
    return d->allowExpressions;
}


void MemoItem::setAllowExpressions(bool value)
{
    Q_D(MemoItem);
    if (d->allowExpressions == value)
        return;
    d->allowExpressions = value;
    update_gui();
    emit allowExpressionsChanged(d->allowExpressions);
    emit changed();
    emit scriptingStringsChanged();
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
    if (d->textMargin < 0)
        d->textMargin = 0;

    resetDocumentWidth();
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
    if (d->textFlags == textFlags)
        return;
    d->textFlags = textFlags;
    update_gui();
    emit textFlagsChanged(d->textFlags);
    emit changed();
}


QString MemoItem::text() const
{
    Q_D(const MemoItem);
    return d->text;
}


void MemoItem::setText(const QString &text)
{
    Q_D(MemoItem);
    if (d->text == text)
        return;
    d->text = text;
    delete d->textDocument;
    d->textDocument =  0;
    update_gui();
    emit textChanged(text);
    emit scriptingStringsChanged();
    emit changed();
}


QFont MemoItem::font() const
{
    Q_D(const MemoItem);
    return d->font;
}


void MemoItem::setFont(const QFont & font)
{
    Q_D(MemoItem);
    if (d->font == font)
        return;
    d->font = font;
    update_gui();
    emit fontChanged(d->font);
    emit changed();
}


QColor MemoItem::textColor() const
{
    Q_D(const MemoItem);
    return d->textColor;
}


void MemoItem::setTextColor(const QColor & color)
{
    Q_D(MemoItem);
    if (d->textColor == color)
        return;
    d->textColor = color;
    update_gui();
    emit textColorChaged(d->textColor);
    emit changed();
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
    if (d->delimiters == delimiters)
        return;

    d->delimiters = delimiters;
    update_gui();
    emit delimitersChanged(d->delimiters);
    emit changed();
}


void MemoItem::renderInit(RendererPublicInterface * renderer)
{
    Q_D(const MemoItem);

    m_renderer = renderer;
    if (d->allowExpressions)
        m_renderer->registerEvaluationString(d->text, d->delimiters.section(",",0,0).trimmed(), d->delimiters.section(",",1,1).trimmed(),  this);
}


void MemoItem::renderReset()
{
    //Q_D(MemoItem);
    m_renderer = 0;
}


bool MemoItem::renderPrepare()
{
    emit printBefore();
    setRenderingPointer(new MemoItemPrivate(*(reinterpret_cast<MemoItemPrivate*>(d_ptr))));

    Q_D(MemoItem);
    emit printDataBefore();

    if (d->allowExpressions)
        d->text = m_renderer->processString(d->text, d->delimiters.section(",",0,0).trimmed(), d->delimiters.section(",",1,1).trimmed(), this);
    adjust();

    emit printDataAfter();

    return d->enabled;
}


RenderedItemInterface *MemoItem::renderView()
{
    Q_D(MemoItem);
    RenderedMemoItem * view = new RenderedMemoItem(this, new MemoItemPrivate(*(reinterpret_cast<MemoItemPrivate*>(d))));
    return view;
}


void MemoItem::paint(QPainter * painter, const QStyleOptionGraphicsItem *option, const BaseItemInterfacePrivate * data, const QRectF &boundingRect, RenderingType type)
{
    Q_UNUSED(option)
    Q_UNUSED(type)

    ItemInterface::paintBegin(painter, option, data, boundingRect, type);

    const MemoItemPrivate * const d = static_cast<const MemoItemPrivate*>(data);

    painter->setClipRect( boundingRect );
    painter->translate( boundingRect.toRect().topLeft() );

    if (d->textDocument) {
        painter->save();
        painter->translate(d->textPos.x(), d->textPos.y());
        painter->scale(d->sw, d->sw);
        d->textDocument->drawContents(painter, d->textClipRect);
        painter->restore();
    }

    ItemInterface::paintEnd(painter, option, data, boundingRect, type);
}


BaseItemHelperInterface *MemoItem::helper()
{
    if (!m_helper)
        m_helper = new MemoHelper(this);

    return m_helper;
}


void MemoItem::resetDocumentWidth()
{
    Q_D(MemoItem);
    if (d->textDocument) {
        d->textDocument->setTextWidth(-1);
        adjust();
        emit textWidthWasReset();
    }
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


SUIT_END_NAMESPACE

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Memo, SUIT_NAMESPACE::MemoItem)
#endif
