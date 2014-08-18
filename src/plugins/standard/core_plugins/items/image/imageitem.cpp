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
#include "imageitem.h"
#include "imageitem_p.h"
#include "item_common/simpleitemview.h"
#include "item_common/simplerendereditem.h"
#include "pageinterface.h"
#include "rendererpublicinterface.h"
#include "datasetinterface.h"
#include "imageitemscript.h"
#include "reportcore.h"

#define MODULENAME "ImageItem"

using namespace CuteReport;

ImageItem::ImageItem(QObject * parent) :
    ItemInterface(*new ImageItemPrivate, parent),
    m_renderer(0)
{
    Q_D(ImageItem);
    setResizeFlags( ResizeLeft | ResizeRight | ResizeTop | ResizeBottom);
    d->rect = QRectF(0,0,30,30);
}


ImageItem::ImageItem(ImageItemPrivate &dd, QObject * parent)
    :ItemInterface(dd, parent),
      m_renderer(0)
{
    //    emptyBrush = QBrush(QImage(":/empty.png"));
}


ImageItem::~ImageItem()
{
}


void ImageItem::moduleInit()
{
}


CuteReport::BaseItemInterface * ImageItem::clone()
{
    Q_D(ImageItem);
    return new ImageItem(*d, parent());
}


QByteArray ImageItem::serialize()
{
    Q_D(ImageItem);
    QByteArray ba;
    QDataStream s( &ba, QIODevice::ReadWrite );
    s << *d;
    return ba;
}


void ImageItem::deserialize(QByteArray &data)
{
    Q_D(ImageItem);
    QDataStream s( &data, QIODevice::ReadWrite );
    s >> *d;
}


QDataStream &operator<<(QDataStream &s, const ImageItemPrivate &p) {
    s << static_cast<const ItemInterfacePrivate&>(p);
    s << p.image << p.source << p.comment << p.font << (qint8)p.paintType << (qint8)p.textPosition << p.textColor;
    s << (qint8)p.sourceType;
    return s;
}


QDataStream &operator>>(QDataStream &s, ImageItemPrivate &p) {
    qint8 paintType, textPosition, sourceType;
    s >> static_cast<ItemInterfacePrivate&>(p);
    s >> p.image; s >> p.source; s >> p.comment; s >> p.font; s >> paintType; s >> textPosition; s >> p.textColor;
    s >> sourceType;

    p.paintType = (ImageItem::PaintType)paintType;
    p.textPosition = (ImageItem::TextPosition)textPosition;
    p.sourceType = (ImageItem::SourceType)sourceType;

    return s;
}


BaseItemInterface *ImageItem::createInstance(QObject * parent) const
{
    return new ImageItem(parent);
}


void ImageItem::init_gui()
{
    if (m_gui)
        return;
    m_gui = new ImageItemView(this);
}


bool ImageItem::canContain(QObject * object)
{
    return (dynamic_cast<CuteReport::BaseItemInterface*>(object));
}


QIcon ImageItem::itemIcon() const
{
    return QIcon(":/image.png");
}


QString ImageItem::moduleName() const
{
    return tr("Image");
}


QString ImageItem::itemGroup() const
{
    return tr("Shapes");
}


ImageItem::SourceType ImageItem::sourceType() const
{
    Q_D(const ImageItem);
    return d->sourceType;
}




void ImageItem::setSourceType(ImageItem::SourceType sourceType)
{
    Q_D(ImageItem);
    if (sourceType == d->sourceType)
        return;
    d->sourceType = sourceType;
    emit sourceTypeChanged(d->sourceType);
    emit sourceTypeChanged(sourceTypeToString(d->sourceType));
    emit changed();
}


QString ImageItem::sourceTypeStr() const
{
    Q_D(const ImageItem);
    return sourceTypeToString(d->sourceType);
}


void ImageItem::setSourceTypeStr(const QString & sourceType)
{
    bool ok;
    ImageItem::SourceType value = sourceTypeFromString(sourceType, &ok);
    if (ok)
        setSourceType(value);
}


QImage ImageItem::image() const
{
    Q_D(const ImageItem);
    return d->image;
}


void ImageItem::setImage(const QImage & image)
{
    Q_D(ImageItem);
    if (d->image == image)
        return;

    d->image = image;

    update_gui();
    emit imageChanged(d->image);

    if (m_inited)
        setSourceType(Static);
}


QString ImageItem::source() const
{
    Q_D(const ImageItem);
    return d->source;
}


void ImageItem::setSource(const QString & source)
{
    Q_D(ImageItem);
    if (d->source != source) {
        d->source = source;
        QImage image(source);
        //        if (!image.isNull())
        setImage(image);
        emit sourceChanged(d->source);
        emit scriptingStringsChanged();
        emit changed();
    }
}


ImageItem::PaintType ImageItem::paintType() const
{
    Q_D(const ImageItem);
    return d->paintType;
}


void ImageItem::setPaintType(ImageItem::PaintType paintType)
{
    Q_D(ImageItem);
    if (d->paintType != paintType) {
        d->paintType = paintType;
        update_gui();
        emit paintTypeChanged(d->paintType);
        emit paintTypeChanged(paintTypeToString(d->paintType));
        emit changed();
    }
}


QString ImageItem::paintTypeStr() const
{
    Q_D(const ImageItem);
    return paintTypeToString(d->paintType);
}


void ImageItem::setPaintTypeStr(const QString &paintType)
{
    bool ok;
    ImageItem::PaintType value = paintTypeFromString(paintType, &ok);
    if (ok)
        setPaintType(value);
}


ImageItem::TextPosition ImageItem::textPosition() const
{
    Q_D(const ImageItem);
    return d->textPosition;
}


void ImageItem::setTextPosition(TextPosition textPosition)
{
    Q_D(ImageItem);
    if (d->textPosition != textPosition) {
        d->textPosition = textPosition;
        update_gui();
        emit textPositionChanged(d->textPosition);
        emit textPositionChanged(textPositionToString(d->textPosition));
        emit changed();
    }
}


QString ImageItem::textPositionStr() const
{
    Q_D(const ImageItem);
    return textPositionToString(d->textPosition);
}


void ImageItem::setTextPositionStr(const QString &textPosition)
{
    bool ok;
    ImageItem::TextPosition value = textPositionFromString(textPosition, &ok);
    if (ok)
        setTextPosition(value);
}


QString ImageItem::text() const
{
    Q_D(const ImageItem);
    return d->comment;
}


void ImageItem::setText(const QString & comment)
{
    Q_D(ImageItem);
    if (d->comment != comment) {
        d->comment = comment;
        update_gui();
        emit textChanged(d->comment);
        emit changed();
    }
}


QFont ImageItem::font() const
{
    Q_D(const ImageItem);
    return d->font;
}


void ImageItem::setFont(const QFont & font)
{
    Q_D(ImageItem);
    if (d->font != font) {
        d->font = font;
        update_gui();
        emit fontChanged(d->font);
        emit changed();
    }
}


QColor ImageItem::textColor() const
{
    Q_D(const ImageItem);
    return d->textColor;
}


void ImageItem::setTextColor(const QColor & color)
{
    Q_D(ImageItem);
    if (d->textColor != color) {
        d->textColor = color;
        update_gui();
        emit textColorChanged(d->textColor);
        emit changed();
    }
}


void ImageItem::renderInit(RendererPublicInterface *renderer)
{
    Q_D(ImageItem);
    m_renderer = renderer;
    m_renderer->registerEvaluationString(d->source, this);
    m_renderer->registerEvaluationString(d->comment, this);
}

void ImageItem::renderReset()
{
    m_renderer = 0;
}


CuteReport::RenderedItemInterface * ImageItem::render(int customDPI)
{
    Q_UNUSED(customDPI);
    Q_D(ImageItem);

    emit renderingBefore();

    ImageItemPrivate * pCurrent = d;
    ImageItemPrivate * pNew = new ImageItemPrivate(*d);

    d_ptr = pNew;
    emit rendering();
    d_ptr = pCurrent;

    RenderedImageItem * view = new RenderedImageItem(this, pNew, m_renderer);

    emit rendered(view);
    emit renderingAfter();

    return view;
}


void ImageItem::paint(QPainter * painter, const QStyleOptionGraphicsItem *option, const BaseItemInterfacePrivate * data, const QRectF &boundingRect, RenderingType type )
{
    Q_UNUSED(option)

    ItemInterface::paintBegin(painter, option, data, boundingRect, type);

    const ImageItemPrivate * const d = static_cast<const ImageItemPrivate*>(data);

    qreal textH  = 0;
    qreal sw=0;
    qreal sh=0;

    QRect rect = boundingRect.toRect();

    painter->setFont(fontPrepared(d->font, d->dpi));

    if (d->textPosition  == TextAbove || d->textPosition == TextBelow)
        textH = QFontMetricsF(painter->font()).height();

    if (d->image.isNull()) {
        if (type == RenderingTemplate) {
            static QBrush emptyBrush(QImage(":/empty.png"));
            painter->setBrush(emptyBrush);
            qreal opacity = painter->opacity();
            painter->setOpacity(0.3);
            painter->drawRect(rect);
            painter->setOpacity(opacity);
        } else
            painter->drawText(rect, Qt::AlignCenter, "No Image");
    } else {
        QImage img(0,0/*,QImage::Format_ARGB32_Premultiplied*/);

        QPointF point = rect.topLeft();
        int cx = 0, cy = 0, cw = d->image.width(), ch = d->image.height();

        switch (d->paintType) {

            case ResizeImageToItem_keepAspectRatio:
                img = d->image.scaled(rect.width(), rect.height() - textH , Qt::KeepAspectRatio, Qt::SmoothTransformation);
                break;

            case ResizeImageToItem_ignoreAspectRatio:
                img = d->image.scaled(rect.width(), rect.height() - textH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                break;

            case CenterImageToItem:
                point.setX(point.x()+(rect.width()-d->image.width())/2);
                point.setY(point.y()+(rect.height()-d->image.height()-textH)/2);

                if (point.x()<0) {
                    cx = abs(point.x());
                    cw -= 2*cx;
                    point.setX(0);
                }

                if (point.y()<0) {
                    cy = abs(point.y());
                    ch -= 2*cy;
                    point.setY(0);
                }

                img = d->image.copy(cx,cy,cw,ch);

                break;

            case ResizeItemToImage:
                img =d ->image;
                sw = img.width() - rect.width();
                sh = img.height() - (rect.height()-textH);
                break;
            default:
                break;
        }

        if (d->textPosition == TextAbove)
            point.setY(point.y() + textH);

        painter->drawImage(point, img);

    }

    painter->setPen(d->textColor);

    switch (d->textPosition)  {
        case TextTop:
            painter->drawText(rect, Qt::AlignTop| Qt::AlignHCenter, d->comment);
            break;

        case TextBottom:
            painter->drawText(rect, Qt::AlignBottom| Qt::AlignHCenter, d->comment);
            break;

        case TextAbove:
            painter->drawText(rect, Qt::AlignTop| Qt::AlignHCenter, d->comment);
            break;

        case TextBelow:
            painter->drawText(rect, Qt::AlignBottom| Qt::AlignHCenter, d->comment);
            break;

        default:
            break;
    }

    if (sw || sh) {
        //        setWidth( boundingRect.width() + sw);
        //        setHeight( boundingRect.height() + sh);
    }

    ItemInterface::paintEnd(painter, option, data, boundingRect, type);
}


QStringList ImageItem::scriptingStrings()
{
    Q_D(ImageItem);
    return QStringList() << d->source << d->comment;
}


void ImageItem::initScript(QScriptEngine *scriptEngine)
{
    Q_UNUSED(scriptEngine)
    registerImageItemScriptClass(scriptEngine);
}


QString ImageItem::sourceTypeToString(ImageItem::SourceType value)
{
    int idx = -1;
    for (int i = 0; i < ImageItem_SourceType_num; ++i) {
        if (qtscript_ImageItem_SourceType_values[i] == value) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        CuteReport::ReportCore::log(LogError, MODULENAME, "sourceTypeToString", QString("Value \\'%1\\' is not within range").arg((int)value));
        return QString();
    }

    return QString(qtscript_ImageItem_SourceType_keys[idx]);
}


ImageItem::SourceType ImageItem::sourceTypeFromString(const QString &value, bool *ok)
{
    QString strIn = value.toLower();
    int idx = -1;
    for (int i = 0; i < ImageItem_SourceType_num; ++i) {
        QString str(qtscript_ImageItem_SourceType_keys[i]);
        if (strIn == str.toLower()) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        CuteReport::ReportCore::log(LogError, MODULENAME, "sourceTypeFromString", QString("Value \\'%1\\' is not within range").arg(value));
        if (ok)
            *ok = false;
    }

    return ImageItem::SourceType(qtscript_ImageItem_SourceType_values[idx]);
}


QString ImageItem::paintTypeToString(ImageItem::PaintType value)
{
    int idx = -1;
    for (int i = 0; i < ImageItem_PaintType_num; ++i) {
        if (qtscript_ImageItem_PaintType_values[i] == value) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        CuteReport::ReportCore::log(LogError, MODULENAME, "paintTypeToString", QString("Value \\'%1\\' is not within range").arg((int)value));
        return QString();
    }

    return QString(qtscript_ImageItem_PaintType_keys[idx]);
}


ImageItem::PaintType ImageItem::paintTypeFromString(const QString &value, bool *ok)
{
    QString strIn = value.toLower();
    int idx = -1;
    for (int i = 0; i < ImageItem_PaintType_num; ++i) {
        QString str(qtscript_ImageItem_PaintType_keys[i]);
        if (strIn == str.toLower()) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        CuteReport::ReportCore::log(LogError, MODULENAME, "paintTypeFromString", QString("Value \\'%1\\' is not within range").arg(value));
        if (ok)
            *ok = false;
    }

    return ImageItem::PaintType(qtscript_ImageItem_PaintType_values[idx]);
}


QString ImageItem::textPositionToString(ImageItem::TextPosition value)
{
    int idx = -1;
    for (int i = 0; i < ImageItem_TextPosition_num; ++i) {
        if (qtscript_ImageItem_TextPosition_values[i] == value) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        CuteReport::ReportCore::log(LogError, MODULENAME, "textPositionToString", QString("Value \\'%1\\' is not within range").arg((int)value));
        return QString();
    }

    return QString(qtscript_ImageItem_TextPosition_keys[idx]);
}


ImageItem::TextPosition ImageItem::textPositionFromString(const QString &value, bool *ok)
{
    QString strIn = value.toLower();
    int idx = -1;
    *ok = true;
    for (int i = 0; i < ImageItem_TextPosition_num; ++i) {
        QString str(qtscript_ImageItem_TextPosition_keys[i]);
        if (strIn == str.toLower()) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        CuteReport::ReportCore::log(LogError, MODULENAME, "textPositionFromString", QString("Value \\'%1\\' is not within range").arg(value));
        if (ok)
            *ok = false;
    }

    return ImageItem::TextPosition(qtscript_ImageItem_TextPosition_values[idx]);
}


QStringList ImageItem::_sourceType_variants() const
{
    return ImageItem_SourceType_variants();
}


QStringList ImageItem::_paintType_variants() const
{
    return ImageItem_PaintType_variants();
}


QStringList ImageItem::_textPosition_variants() const
{
    return ImageItem_TextPosition_variants();
}


QString ImageItem::_current_property_description() const
{
    QString propertyName = metaObject()->property(m_currentProperty).name();

    if (propertyName == "sourceType")
        return tr("Defines the source of the image data.\n"
                  "Script: you can define \'source\' field in format:\n"
                  "file://[data.\"name\"] to take file with runtime name\n"
                  "res://objectname.png to take file from resource\n"
                  "Dataset: [data.\"name\"]\nStatic: use \"image\" to load blob");
    else if (propertyName == "source")
        return tr("Set source for sourceTypes: Script and Dataset");
    else if (propertyName == "image")
        return tr("Set image blob for sourceTypes: Static");
    else if (propertyName == "paintType")
        return tr("Type of Image layout and scale in a frame");
    else if (propertyName == "textPosition")
        return tr("Postion of text commment");
    else if (propertyName == "text")
        return tr("Text comment");
    else if (propertyName == "font")
        return tr("Text comment font");
    else if (propertyName == "textColor")
        return tr("Text comment color");
    else
        return ItemInterface::_current_property_description();
}


//====================================================================================================================

RenderedImageItem::RenderedImageItem(CuteReport::BaseItemInterface * item, CuteReport::BaseItemInterfacePrivate *itemPrivateData, CuteReport::RendererPublicInterface * renderer)
    :RenderedItemInterface(item, itemPrivateData)
{
    ImageItemPrivate * const d = static_cast<ImageItemPrivate*>(itemPrivateData);
    ImageItem* image = reinterpret_cast<ImageItem*>(item);
    switch (d->sourceType) {
        case ImageItem::Script:
            d->source = renderer->processString(d->source, "[","]" , image);
            d->image.loadFromData( renderer->getStorageObject(d->source).toByteArray() );
            break;
        case ImageItem::Dataset: {
            QString str = d->source.remove('[').remove(']').remove('"');
            CuteReport::DatasetInterface * dataset = renderer->dataset(str.section('.', 0, 0));
            if (dataset)
                d->image.loadFromData( dataset->value(str.section('.', 1, 1)).toByteArray() );
            else
                d->image = QImage();
            }
            break;
        case ImageItem::Static :
            break;
    }
    d->comment = renderer->processString(d->comment, "[","]" , image);
}


void RenderedImageItem::paint(QPainter * painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    ImageItem::paint(painter, option, d_ptr, boundingRect(), CuteReport::RenderingReport);
}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(image, ImageItem)
#endif
