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

#ifndef LINE_H
#define LINE_H

#include <iteminterface.h>
#include "iteminterfaceview.h"
#include "renderediteminterface.h"
#include "globals.h"

class LineItemPrivate;

class LineItem : public CuteReport::ItemInterface
{
	Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteReport.ItemInterface/1.0")
#endif
    Q_INTERFACES(CuteReport::ItemInterface)

	Q_ENUMS(LineStyle)

    Q_PROPERTY(QPen linePen READ linePen WRITE setLinePen NOTIFY linePenChanged)
    Q_PROPERTY(QString lineStyle READ lineStyleStr WRITE setLineStyleStr NOTIFY lineStyleChanged)

    Q_PROPERTY(LineStyle lineStyle READ lineStyle WRITE setLineStyle NOTIFY lineStyleChanged)

    Q_PROPERTY(QStringList _lineStyle_variants READ _lineStyle_variants DESIGNABLE false)

public:
    enum LineStyle {Vertical, Horizontal, ForwardDiagonal, BackwardDiagonal};

    explicit LineItem(QObject * parent = 0);

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

    virtual bool renderPrepare();
    virtual CuteReport::RenderedItemInterface * renderView();

    static void paint(QPainter * painter, const QStyleOptionGraphicsItem *option, const CuteReport::BaseItemInterfacePrivate * data, const QRectF &boundingRect, CuteReport::RenderingType type = CuteReport::RenderingTemplate);

    LineStyle lineStyle() const;
    void setLineStyle(LineStyle lineStyle);
    QString lineStyleStr() const;
    void setLineStyleStr(const QString & lineStyle);

    QPen linePen() const;
    void setLinePen(const QPen & pen);

    virtual void initScript(QScriptEngine * scriptEngine);

    /** propertyeditor hints   */
    QStringList _lineStyle_variants() const;

    virtual QString _current_property_description() const;

signals:
    void lineStyleChanged(LineStyle);
    void lineStyleChanged(const QString &);
    void linePenChanged(QPen);

protected:
    explicit LineItem(LineItemPrivate *dd, QObject * parent);
    virtual BaseItemInterface * itemClone() const;

private:
    Q_DECLARE_PRIVATE(LineItem)
};


class LineItemView : public CuteReport::ItemInterfaceView
{
    Q_INTERFACES(CuteReport::ItemInterfaceView)
public:
    LineItemView(CuteReport::BaseItemInterface * item):
        CuteReport::ItemInterfaceView(item){}

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)
    {
        Q_UNUSED(widget)
        LineItem::paint(painter, option, ptr(), boundingRect(), CuteReport::RenderingTemplate);
    }
};


class RenderedLineItem : public CuteReport::RenderedItemInterface
{
public:
    explicit RenderedLineItem(CuteReport::BaseItemInterface * item, CuteReport::BaseItemInterfacePrivate *itemPrivateData)
        :RenderedItemInterface(item, itemPrivateData) {}

    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)
    {
        Q_UNUSED(widget)
        LineItem::paint(painter, option, d_ptr, boundingRect(), CuteReport::RenderingReport);
    }
};

Q_DECLARE_METATYPE(LineItem::LineStyle)

#endif
