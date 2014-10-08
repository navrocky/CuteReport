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
#ifndef MEMOITEM_H
#define MEMOITEM_H

#include "iteminterface.h"
#include "iteminterfaceview.h"
#include "renderediteminterface.h"
#include "globals.h"
#include "types.h"
#include "plugins_common.h"

SUIT_BEGIN_NAMESPACE
class MemoItemPrivate;
class MemoItem;
SUIT_END_NAMESPACE

USING_SUIT_NAMESPACE

SUIT_BEGIN_NAMESPACE
class MemoItem : public CuteReport::ItemInterface
{
	Q_OBJECT
    Q_INTERFACES(CuteReport::ItemInterface)
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteReport.ItemInterface/1.0")
#endif

    Q_FLAGS(TextFlags)
    Q_ENUMS(StretchMode TextFlag)

    Q_PROPERTY(TextFlags textFlags READ textFlags WRITE setTextFlags NOTIFY textFlagsChanged)
    Q_PROPERTY(QString stretchMode READ stretchModeStr WRITE setStretchModeStr NOTIFY stretchModeChanged)
    Q_PROPERTY(bool showStretchability READ showStretchability WRITE setShowStretchability NOTIFY showStretchabilityChanged)
    Q_PROPERTY(bool allowHTML READ allowHTML WRITE setAllowHTML NOTIFY allowHTMLChanged)
    Q_PROPERTY(bool allowExpressions READ allowExpressions WRITE setAllowExpressions NOTIFY allowExpressionsChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY textColorChaged)
    Q_PROPERTY(qreal textMargin READ textMargin WRITE setTextMargin NOTIFY textMarginChaged)
    Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY fontChanged)
    Q_PROPERTY(bool stretchFont READ stretchFont WRITE setStretchFont NOTIFY stretchFontChanged)
    Q_PROPERTY(QString expDelimiters READ delimiters WRITE setDelimiters NOTIFY delimitersChanged)

    Q_PROPERTY(StretchMode stretchMode READ stretchMode WRITE setStretchMode NOTIFY stretchModeChanged DESIGNABLE false)

    //TODO: TextFlags
    Q_PROPERTY(QStringList _stretchMode_variants READ _stretchMode_variants)

public:

	enum TextFlag 
	{
		AlignLeft = Qt::AlignLeft,
		AlignRight = Qt::AlignRight,
		AlignHCenter = Qt::AlignHCenter,
		AlignJustify = Qt::AlignJustify,
		AlignTop = Qt::AlignTop,
		AlignBottom = Qt::AlignBottom,
		AlignVCenter = Qt::AlignVCenter,
		AlignCenter = Qt::AlignCenter,
		TextDontClip = Qt::TextDontClip,
		TextSingleLine = Qt::TextSingleLine,
		TextExpandTabs = Qt::TextExpandTabs,
		TextShowMnemonic = Qt::TextShowMnemonic,
		TextWordWrap = Qt::TextWordWrap
	};
    Q_DECLARE_FLAGS(TextFlags, TextFlag)

    enum StretchMode
	{
        DontStretch = 0,
        ActualHeight = 1,
        MaxHeight = 2
	};

public:
    explicit MemoItem(QObject * parent = 0);
    virtual ~MemoItem();

    virtual void moduleInit();
    virtual void init();

    virtual void init_gui();
    virtual void update_gui();

    virtual CuteReport::BaseItemInterface * createInstance(QObject * parent) const;
    virtual CuteReport::BaseItemHelperInterface * helper();
    virtual QByteArray serialize();
    virtual void deserialize(QByteArray &data);
    virtual bool canContain(QObject * object);

    virtual QIcon itemIcon() const;
    virtual QString moduleShortName() const { return "Memo"; }
    virtual QString suitName() const { return SUIT_NAMESPACE_STR; }
    virtual QString itemGroup() const;

    virtual void setGeometry(const QRectF & rect, CuteReport::Unit unit = CuteReport::UnitNotDefined);
    virtual void setHeight(qreal height, CuteReport::Unit unit = CuteReport::UnitNotDefined);

    StretchMode stretchMode() const;
    void setStretchMode(StretchMode stretchMode);
    QString stretchModeStr() const;
    void setStretchModeStr(const QString & stretchMode);

    bool showStretchability() const;
    void setShowStretchability(bool value);

    bool allowHTML() const;
    void setAllowHTML(bool value);

    bool allowExpressions() const;
    void setAllowExpressions(bool value);

    qreal textMargin() const;
    void setTextMargin(qreal value);

    TextFlags textFlags() const;
    void setTextFlags(TextFlags textFlags);

    QString text() const;
    void setText(const QString &text);

    virtual QFont font() const;
    virtual void setFont(const QFont & font);

    QColor textColor() const;
    void setTextColor(const QColor & color);

    bool stretchFont() const;
    void setStretchFont(bool b);

    QString delimiters() const;
    void setDelimiters(const QString & delimiters);

    QString flowTo() const;
    void setFlowTo(const QString & name);

    virtual void renderInit(CuteReport::RendererPublicInterface * renderer);
    void renderReset();
    virtual bool renderPrepare();
    CuteReport::RenderedItemInterface * renderView();

    static void paint(QPainter * painter, const QStyleOptionGraphicsItem *option, const CuteReport::BaseItemInterfacePrivate * data, const QRectF &boundingRect, CuteReport::RenderingType type = CuteReport::RenderingTemplate);

    virtual QStringList scriptingStrings();
    virtual void initScript(QScriptEngine * scriptEngine);

    /** propertyeditor hints   */
    QStringList _stretchMode_variants() const;

    virtual QString  _current_property_description() const;

signals:
    void stretchModeChanged(StretchMode);
    void stretchModeChanged(const QString &);
    void textFlagsChanged(TextFlags);
    void fontChanged(QFont);
    void textChanged(QString);
    void textColorChaged(QColor);
    void delimitersChanged(QString);
    void stretchFontChanged(bool);
    void allowHTMLChanged(bool);
    void allowExpressionsChanged(bool);
    void textMarginChaged(int);
    void showStretchabilityChanged(bool);
    void textWidthWasReset();
    void adjusted();

private:
    Q_DECLARE_PRIVATE(MemoItem)
    Q_DISABLE_COPY(MemoItem)
    explicit MemoItem(MemoItemPrivate *dd, QObject * parent);
    virtual BaseItemInterface * itemClone() const;
    void adjust();
    static void adjust(MemoItemPrivate * d, const QPointF & posDeltaMM);
    void resetDocumentWidth();

    QPointer<CuteReport::BaseItemHelperInterface> m_helper;
    CuteReport::RendererPublicInterface * m_renderer;

    friend class RenderedMemoItem;
};


SUIT_END_NAMESPACE

Q_DECLARE_METATYPE(SUIT_NAMESPACE::MemoItem::TextFlag)
Q_DECLARE_METATYPE(SUIT_NAMESPACE::MemoItem::StretchMode)
Q_DECLARE_OPERATORS_FOR_FLAGS(SUIT_NAMESPACE::MemoItem::TextFlags)

#endif
