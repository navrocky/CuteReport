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

    Q_ENUMS(SourceType ScaleType TextPosition )

    Q_PROPERTY(QString sourceType READ sourceTypeStr WRITE setSourceTypeStr NOTIFY sourceTypeChanged())
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(bool keepAspectRatio READ keepAspectRatio WRITE setKeepAspectRatio NOTIFY keepAspectRatioChanged)
    Q_PROPERTY(QString scaleType READ scaleTypeStr WRITE setScaleTypeStr NOTIFY scaleTypeChanged)
    Q_PROPERTY(bool center READ center WRITE setCenter NOTIFY centerChanged)

    Q_PROPERTY(ScaleType scaleType READ scaleType WRITE setScaleType NOTIFY scaleTypeChanged DESIGNABLE false)
    Q_PROPERTY(SourceType sourceType READ sourceType WRITE setSourceType NOTIFY sourceTypeChanged() DESIGNABLE false)

    Q_PROPERTY(QStringList _sourceType_variants READ _sourceType_variants DESIGNABLE false)
    Q_PROPERTY(QStringList _scaleType_variants READ _scaleType_variants DESIGNABLE false)

public:
    enum SourceType { Static, Storage, Dataset};

    enum ScaleType {
        NoScale = 0,
        ScaleImage = 1,
        ScaleFrame = 2,
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
//    virtual void update_gui();

    virtual CuteReport::BaseItemInterface * createInstance(QObject * parent) const;
    virtual QByteArray serialize();
    virtual void deserialize(QByteArray &data);
    virtual bool canContain(QObject * object);

    QIcon itemIcon() const;
    QString moduleShortName() const;
    virtual QString suitName() const { return "Standard"; }
    QString itemGroup() const;

    SourceType sourceType() const;
    void setSourceType(SourceType sourceType);
    QString sourceTypeStr() const;
    void setSourceTypeStr(const QString & sourceType);

    QImage image() const;
    void setImage(const QImage & image);

    QString source() const;
    void setSource(const QString & source);

    ScaleType scaleType() const;
    void setScaleType(ScaleType scaleType);
    QString scaleTypeStr() const;
    void setScaleTypeStr(const QString & scaleType);

    bool keepAspectRatio() const;
    void setKeepAspectRatio(bool keepAspectRatio);

    bool center() const;
    void setCenter(bool center);

    virtual void renderInit(CuteReport::RendererPublicInterface * renderer);
    virtual void renderReset();
    virtual bool renderPrepare();
    virtual CuteReport::RenderedItemInterface * renderView();

    static void paint(QPainter * painter, const QStyleOptionGraphicsItem *option, const CuteReport::BaseItemInterfacePrivate * data, const QRectF &boundingRect = QRectF(), CuteReport::RenderingType type = CuteReport::RenderingTemplate);
    
    static QString sourceTypeToString(SourceType value);
    static SourceType sourceTypeFromString(const QString & value, bool * ok = 0);

    static QString scaleTypeToString(ScaleType value);
    static ScaleType scaleTypeFromString(const QString & value, bool * ok = 0);

    virtual QStringList scriptingStrings();
    virtual void initScript(QScriptEngine * scriptEngine);

    /** propertyeditor hints   */
    QStringList _sourceType_variants() const;
    QStringList _scaleType_variants() const;

    virtual QString  _current_property_description() const;

signals:
    void imageChanged(const QImage &);
    void scaleTypeChanged(const ScaleType &);
    void scaleTypeChanged(const QString &);
    void textPositionChanged(const TextPosition &);
    void textPositionChanged(const QString &);
    void textChanged(const QString &);
    void fontChanged(const QFont & );
    void textColorChanged(const QColor &);
    void sourceChanged(const QString &);
    void sourceTypeChanged(ImageItem::SourceType);
    void sourceTypeChanged(const QString &);
    void keepAspectRatioChanged(bool);
    void centerChanged(bool);

private:
    Q_DECLARE_PRIVATE(ImageItem)
    explicit ImageItem(ImageItemPrivate *dd, QObject * parent);
    virtual BaseItemInterface * itemClone() const;

    CuteReport::RendererPublicInterface * m_renderer;
    QImage img;

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
Q_DECLARE_METATYPE(ImageItem::ScaleType)
Q_DECLARE_METATYPE(ImageItem::TextPosition)

#endif // IMAGEITEM_H


