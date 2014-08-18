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
#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include "iteminterface.h"
#include "iteminterfaceview.h"
#include "renderediteminterface.h"
#include "globals.h"

class ImageItemPrivate;

class ImageItem : public CuteReport::ItemInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteReport.ItemInterface/1.0")
#endif
    Q_INTERFACES(CuteReport::ItemInterface)

    Q_ENUMS(SourceType PaintType TextPosition )

    Q_PROPERTY(QString sourceType READ sourceTypeStr WRITE setSourceTypeStr NOTIFY sourceTypeChanged())
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(QString paintType READ paintTypeStr WRITE setPaintTypeStr NOTIFY paintTypeChanged)
    Q_PROPERTY(QString textPosition READ textPositionStr WRITE setTextPositionStr NOTIFY textPositionChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY fontChanged)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY textColorChanged)

    Q_PROPERTY(PaintType paintType READ paintType WRITE setPaintType NOTIFY paintTypeChanged DESIGNABLE false)
    Q_PROPERTY(SourceType sourceType READ sourceType WRITE setSourceType NOTIFY sourceTypeChanged() DESIGNABLE false)
    Q_PROPERTY(TextPosition textPosition READ textPosition WRITE setTextPosition NOTIFY textPositionChanged DESIGNABLE false)

    Q_PROPERTY(QStringList _sourceType_variants READ _sourceType_variants DESIGNABLE false)
    Q_PROPERTY(QStringList _paintType_variants READ _paintType_variants DESIGNABLE false)
    Q_PROPERTY(QStringList _textPosition_variants READ _textPosition_variants DESIGNABLE false)

public:
    enum SourceType { Script, Dataset, Static };

    enum PaintType {
        PaintTypeNotDefined = 0,
        ResizeItemToImage = 1,
        CenterImageToItem = 2,
        ResizeImageToItem_keepAspectRatio = 3,
        ResizeImageToItem_ignoreAspectRatio = 4
    };

//    enum FrameType    {
//        NO_FRAME = 0,
//        IMAGE_BIND = 1,
//        IMAGE_BOX = 2
//    };

    enum TextPosition    {
        NoText = 0,
        TextTop = 1,
        TextBottom = 2,
        TextAbove = 3,
        TextBelow = 4
    };


    explicit ImageItem(QObject * parent = 0);

    virtual ~ImageItem();

    virtual void moduleInit();

    virtual void init_gui();

    virtual CuteReport::BaseItemInterface * createInstance(QObject * parent) const;
    virtual BaseItemInterface * clone();
    virtual QByteArray serialize();
    virtual void deserialize(QByteArray &data);
    virtual bool canContain(QObject * object);

    QIcon itemIcon() const;
    QString moduleName() const;
    QString itemGroup() const;

    SourceType sourceType() const;
    void setSourceType(SourceType sourceType);
    QString sourceTypeStr() const;
    void setSourceTypeStr(const QString & sourceType);

    QImage image() const;
    void setImage(const QImage & image);

    QString source() const;
    void setSource(const QString & source);

    PaintType paintType() const;
    void setPaintType(PaintType paintType);
    QString paintTypeStr() const;
    void setPaintTypeStr(const QString & paintType);

    TextPosition textPosition() const;
    void setTextPosition(TextPosition textPosition);
    QString textPositionStr() const;
    void setTextPositionStr(const QString & textPosition);

    QString text() const;
    void setText(const QString & text);

    QFont font() const;
    void setFont(const QFont & font);

    QColor textColor() const;
    void setTextColor(const QColor & color);

    virtual void renderInit(CuteReport::RendererPublicInterface * renderer);
    virtual void renderReset();
    virtual CuteReport::RenderedItemInterface * render(int customDPI = 0);
    static void paint(QPainter * painter, const QStyleOptionGraphicsItem *option, const CuteReport::BaseItemInterfacePrivate * data, const QRectF &boundingRect = QRectF(), CuteReport::RenderingType type = CuteReport::RenderingTemplate);
    
    static QString sourceTypeToString(SourceType value);
    static SourceType sourceTypeFromString(const QString & value, bool * ok = 0);

    static QString paintTypeToString(PaintType value);
    static PaintType paintTypeFromString(const QString & value, bool * ok = 0);

    static QString textPositionToString(TextPosition value);
    static TextPosition textPositionFromString(const QString & value, bool * ok = 0);

    virtual QStringList scriptingStrings();
    virtual void initScript(QScriptEngine * scriptEngine);

    /** propertyeditor hints   */
    QStringList _sourceType_variants() const;
    QStringList _paintType_variants() const;
    QStringList _textPosition_variants() const;

    virtual QString  _current_property_description() const;

signals:
    void imageChanged(const QImage &);
    void paintTypeChanged(const PaintType &);
    void paintTypeChanged(const QString &);
    void textPositionChanged(const TextPosition &);
    void textPositionChanged(const QString &);
    void textChanged(const QString &);
    void fontChanged(const QFont & );
    void textColorChanged(const QColor &);
    void sourceChanged(const QString &);
    void sourceTypeChanged(ImageItem::SourceType);
    void sourceTypeChanged(const QString &);

protected:
    ImageItem(ImageItemPrivate &dd, QObject * parent);

    CuteReport::RendererPublicInterface * m_renderer;

private:
    Q_DECLARE_PRIVATE(ImageItem)

friend class ImageItemView;
};


class ImageItemView : public CuteReport::ItemInterfaceView
{
    Q_INTERFACES(CuteReport::ItemInterfaceView)
public:
    ImageItemView(CuteReport::BaseItemInterface * item):
        CuteReport::ItemInterfaceView(item){}

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)
    {
        Q_UNUSED(widget)
        ImageItem::paint(painter, option, ptr(), boundingRect(), CuteReport::RenderingTemplate);
    }
};


class RenderedImageItem : public CuteReport::RenderedItemInterface
{
public:
    explicit RenderedImageItem(CuteReport::BaseItemInterface * item, CuteReport::BaseItemInterfacePrivate *itemPrivateData, CuteReport::RendererPublicInterface * renderer);

    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

};

Q_DECLARE_METATYPE(ImageItem::SourceType)
Q_DECLARE_METATYPE(ImageItem::PaintType)
Q_DECLARE_METATYPE(ImageItem::TextPosition)

#endif // IMAGEITEM_H


