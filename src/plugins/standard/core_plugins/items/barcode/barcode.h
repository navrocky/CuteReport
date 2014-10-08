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

#ifndef BARCODEITEM_H
#define BARCODEITEM_H

#include <iteminterface.h>
#include "iteminterfaceview.h"
#include "renderediteminterface.h"
#include "globals.h"

using namespace CuteReport;

class BarcodeItemPrivate;

class BarcodeItem : public CuteReport::ItemInterface
{
	Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteReport.ItemInterface/1.0")
#endif
    Q_INTERFACES(CuteReport::ItemInterface)

    Q_ENUMS(BarcodeTypes PaintTypes FrameTypes DrawTextTypes MsiPlessey)

    Q_PROPERTY(QColor color READ barcodeColor WRITE setBarcodeColor NOTIFY barcodeColorChanged)
    Q_PROPERTY(QString paintType READ paintTypeStr WRITE setPaintTypeStr NOTIFY paintTypeChanged)
    Q_PROPERTY(QString frameType READ frameTypeStr WRITE setFrameTypeStr NOTIFY frameTypeChanged)
    Q_PROPERTY(QString barcodeType READ barcodeTypeStr WRITE setBarcodeTypeStr NOTIFY barcodeTypeChanged)
    Q_PROPERTY(int barcodeHeight READ barcodeHeight WRITE setBarcodeHeight NOTIFY barcodeHeightChanged)
    Q_PROPERTY(int barcodeWhitespaceWidth READ barcodeWhitespaceWidth WRITE setBarcodeWhitespaceWidth NOTIFY barcodeWhitespaceWidthChanged)
    Q_PROPERTY(int barcodeBoderWidth READ barcodeBoderWidth WRITE setBarcodeBoderWidth NOTIFY barcodeBoderWidthChanged)
    Q_PROPERTY(int barcodeWidth READ barcodeWidth WRITE setBarcodeWidth NOTIFY barcodeWidthChanged)
    Q_PROPERTY(int barcodeSecurityLevel READ barcodeSecurityLevel WRITE setBarcodeSecurityLevel NOTIFY barcodeSecurityLevelChanged)
    Q_PROPERTY(int pdf417Max READ pdf417Max WRITE setPdf417Max NOTIFY pdf417MaxChanged)
    Q_PROPERTY(QString msiPlessey READ msiPlesseyStr WRITE setMsiPlesseyStr NOTIFY msiPlesseyChanged)
    Q_PROPERTY(bool exCode39_43Extension READ exCode39_43Extension WRITE setExCode39_43Extension NOTIFY exCode39_43ExtensionChanged)
    Q_PROPERTY(QString drawTextType READ drawTextTypeStr WRITE setDrawTextTypeStr NOTIFY drawTextTypeChanged)
    Q_PROPERTY(QString primaryMessage READ primaryMessage WRITE setPrimaryMessage NOTIFY primaryMessageChanged)
    Q_PROPERTY(QString script READ script WRITE setScript NOTIFY scriptChanged)
    Q_PROPERTY(QString testText READ testText WRITE setTestText NOTIFY testTextChanged)

    Q_PROPERTY(PaintTypes paintType READ paintType WRITE setPaintType NOTIFY paintTypeChanged)
    Q_PROPERTY(FrameTypes frameType READ frameType WRITE setFrameType NOTIFY frameTypeChanged)
    Q_PROPERTY(BarcodeTypes barcodeType READ barcodeType WRITE setBarcodeType NOTIFY barcodeTypeChanged)
    Q_PROPERTY(MsiPlessey msiPlessey READ msiPlessey WRITE setMsiPlessey NOTIFY msiPlesseyChanged)
    Q_PROPERTY(DrawTextTypes drawTextType READ drawTextType WRITE setDrawTextType NOTIFY drawTextTypeChanged)

    Q_PROPERTY(QStringList _paintType_variants READ _paintType_variants DESIGNABLE false)
    Q_PROPERTY(QStringList _frameType_variants READ _frameType_variants DESIGNABLE false)
    Q_PROPERTY(QStringList _barcodeType_variants READ _barcodeType_variants DESIGNABLE false)
    Q_PROPERTY(QStringList _msiPlessey_variants READ _msiPlessey_variants DESIGNABLE false)
    Q_PROPERTY(QStringList _drawTextType_variants READ _drawTextType_variants DESIGNABLE false)

public:
	enum MsiPlessey
	{
		MSI_PLESSEY_ 	= 0,
		MSI_10		= 1,
		MSI_10_10	= 2,
		MSI_11		= 3,
		MSI_11_10	= 4
	};

	enum DrawTextTypes
	{
		NO_TEXT = 0,
		DRAW_TOP = 1,
		DRAW_BOTTOM = 2,
		DRAW_ABOVE = 3,
		DRAW_BELOW = 4
	};

	enum FrameTypes
	{
		NO_FRAME = 0,
        BarcodeItem_BIND = 1,
        BarcodeItem_BOX = 2
	};

    enum PaintTypes
	{
		IgnoreAspectRatio=0,
		KeepAspectRatio=1,
        CenterBarcodeItem=2
	};

    enum BarcodeTypes
	{
		CODE11		=1,
		C25MATRIX	=2,
		C25INTER	=3,
		C25IATA		=4,
		C25LOGIC	=6,
		C25IND		=7,
		CODE39		=8,
		EXCODE39	=9,
		EANX		=13,
		EAN128		=16,
		CODABAR		=18,
		CODE128		=20,
		DPLEIT		=21,
		DPIDENT		=22,
		CODE16K		=23,
		CODE93		=25,
		FLAT		=28,
		RSS14		=29,
		RSS_LTD		=30,
		RSS_EXP		=31,
		TELEPEN		=32,
		UPCA		=34,
		UPCE		=37,
		POSTNET		=40,
		MSI_PLESSEY	=47,
		FIM		=49,
		LOGMARS		=50,
		PHARMA		=51,
		PZN		=52,
		PHARMA_TWO	=53,
		PDF417		=55,
		PDF417TRUNC	=56,
		MAXICODE	=57,
		QRCODE		=58,
		CODE128B	=60,
		AUSPOST		=63,
		AUSREPLY	=66,
		AUSROUTE	=67,
		AUSREDIRECT	=68,
		ISBNX		=69,
		RM4SCC		=70,
		DATAMATRIX	=71,
		ITF14		=72,
		CODABLOCKF	=74,
		NVE18		=75,
		KOREAPOST	=77,
		RSS14STACK	=79,
		RSS14STACK_OMNI	=80,
		RSS_EXPSTACK	=81,
		PLANET		=82,
		MICROPDF417	=84,
		ONECODE		=85,
		PLESSEY		=86,
		
        /* TBarcodeItem 8 codes */
		KIX		=90,
		AZTEC		=92,
		DAFT		=93,
		ITALYPOST	=94,
		DPD		=96,
		MICROQR		=97,
		
		/* Zint specific */
		TELEPEN_NUM	=128,
		CODE32		=129,
		EANX_CC		=130,
		EAN128_CC	=131,
		RSS14_CC	=132,
		RSS_LTD_CC	=133,
		RSS_EXP_CC	=134,
		UPCA_CC		=135,
		UPCE_CC		=136,
		RSS14STACK_CC	=137,
		RSS14_OMNI_CC	=138,
		RSS_EXPSTACK_CC	=139
	};

public:
    BarcodeItem(QObject * parent = 0);

    virtual void moduleInit();
    virtual void init_gui();

    virtual CuteReport::BaseItemInterface * createInstance(QObject * parent) const;
    virtual CuteReport::BaseItemHelperInterface * helper();
    virtual QByteArray serialize();
    virtual void deserialize(QByteArray &data);
    virtual bool canContain(QObject * object);

    virtual QIcon itemIcon() const;
    virtual QString moduleShortName() const;
    virtual QString suitName() const { return "Standard"; }
    virtual QString itemGroup() const;

    virtual void renderInit(CuteReport::RendererPublicInterface * renderer);
    virtual void renderReset();
    virtual bool renderPrepare();
    virtual CuteReport::RenderedItemInterface * renderView();

    static void paint(QPainter * painter, const QStyleOptionGraphicsItem *option, const BaseItemInterfacePrivate * data, const QRectF &boundingRect, RenderingType type = RenderingTemplate);

    BarcodeTypes barcodeType() const;
    void setBarcodeType(BarcodeTypes barcodeType);
    QString barcodeTypeStr() const;
    void setBarcodeTypeStr(const QString &barcodeType);

    PaintTypes paintType() const;
	void setPaintType(PaintTypes paintType);
    QString paintTypeStr() const;
    void setPaintTypeStr(const QString &paintType);

    FrameTypes frameType() const;
	void setFrameType(FrameTypes frameType);
    QString frameTypeStr() const;
    void setFrameTypeStr(const QString &frameType);

    int barcodeHeight() const;
    void setBarcodeHeight(int BarcodeHeight);

    int barcodeWhitespaceWidth() const;
    void setBarcodeWhitespaceWidth(int barcodeWhitespaceWidth);

    int barcodeBoderWidth() const;
    void setBarcodeBoderWidth(int barcodeBoderWidth);

    QColor barcodeColor() const;
    void setBarcodeColor(QColor barcodeColor);

    int barcodeWidth() const;
    void setBarcodeWidth(int barcodeWidth);

    int barcodeSecurityLevel() const;
    void setBarcodeSecurityLevel(int barcodeSecurityLevel);

    int pdf417Max() const;
	void setPdf417Max(int pdf417Max);

    MsiPlessey msiPlessey() const;
	void setMsiPlessey(MsiPlessey msiPlessey);
    QString msiPlesseyStr() const;
    void setMsiPlesseyStr(const QString & msiPlessey);

    bool exCode39_43Extension() const;
	void setExCode39_43Extension(bool exCode39_43Extension);

    DrawTextTypes drawTextType() const;
	void setDrawTextType(DrawTextTypes drawTextType);
    QString drawTextTypeStr() const;
    void setDrawTextTypeStr(const QString & msiPlessey);

    const QString & primaryMessage() const;
	void setPrimaryMessage(const QString &primaryMessage);

    QString script() const;
	void setScript(const QString & script);

    QString testText() const;
	void setTestText(const QString & testText);

    virtual QStringList scriptingStrings();
    virtual void initScript(QScriptEngine * scriptEngine);

    /** propertyeditor hints   */
    QStringList _paintType_variants() const;
    QStringList _frameType_variants() const;
    QStringList _barcodeType_variants() const;
    QStringList _msiPlessey_variants() const;
    QStringList _drawTextType_variants() const;

signals:
    void paintTypeChanged(BarcodeItem::PaintTypes);
    void paintTypeChanged(const QString &);
    void frameTypeChanged(BarcodeItem::FrameTypes);
    void frameTypeChanged(const QString &);
    void barcodeTypeChanged(BarcodeItem::BarcodeTypes);
    void barcodeTypeChanged(const QString &);
    void barcodeHeightChanged(int);
    void barcodeWhitespaceWidthChanged(int);
    void barcodeBoderWidthChanged(int);
    void barcodeColorChanged(QColor);
    void barcodeWidthChanged(int);
    void barcodeSecurityLevelChanged(int);
    void pdf417MaxChanged(int);
    void msiPlesseyChanged(BarcodeItem::MsiPlessey);
    void msiPlesseyChanged(const QString &);
    void exCode39_43ExtensionChanged(bool);
    void drawTextTypeChanged(BarcodeItem::DrawTextTypes);
    void drawTextTypeChanged(const QString &);
    void primaryMessageChanged(QString);
    void scriptChanged(QString);
    void testTextChanged(QString);

private:
    Q_DECLARE_PRIVATE(BarcodeItem)
    BarcodeItem(BarcodeItemPrivate *dd, QObject * parent);
    virtual BaseItemInterface * itemClone() const;

    RendererPublicInterface * m_renderer;
};



class  BarcodeItemView : public ItemInterfaceView
{
    Q_INTERFACES(CuteReport::ItemInterfaceView)
public:
    BarcodeItemView(BaseItemInterface * item):
        CuteReport::ItemInterfaceView(item){}

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)
    {
        Q_UNUSED(widget)
        BarcodeItem::paint(painter, option, ptr(), boundingRect(), RenderingTemplate);
    }
};


class  RenderedBarcodeItem : public RenderedItemInterface
{
public:
    explicit RenderedBarcodeItem(BaseItemInterface * item, CuteReport::BaseItemInterfacePrivate *itemPrivateData)
        :RenderedItemInterface(item, itemPrivateData) {}

    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)
    {
        Q_UNUSED(widget)
        BarcodeItem::paint(painter, option, d_ptr, boundingRect(), RenderingReport);
    }
};

Q_DECLARE_METATYPE(BarcodeItem::MsiPlessey)
Q_DECLARE_METATYPE(BarcodeItem::DrawTextTypes)
Q_DECLARE_METATYPE(BarcodeItem::FrameTypes)
Q_DECLARE_METATYPE(BarcodeItem::BarcodeTypes)
Q_DECLARE_METATYPE(BarcodeItem::PaintTypes)


#endif
