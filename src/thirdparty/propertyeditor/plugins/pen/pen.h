/***************************************************************************
 *   This file is part of the propertyEditor project                       *
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *   bog_dan_ro@yahoo.com                                                  *
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
 ****************************************************************************/

#ifndef PEN_H
#define PEN_H

#include <propertyinterface.h>
#include <QBrush>
#include <QPen>

using namespace PropertyEditor;


class Pen : public PropertyInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "eu.licentia.PropertyEditor.PropetyInterface/1.0")
#endif
    Q_INTERFACES(PropertyEditor::PropertyInterface)

    Q_ENUMS(PenStyle PenCapStyle PenJoinStyle)

    Q_PROPERTY(QBrush brush READ brush WRITE setBrush USER true)
    Q_PROPERTY(PenCapStyle capStyle READ capStyle WRITE setCapStyle USER true)
    Q_PROPERTY(QColor color READ color WRITE setColor USER true)
    Q_PROPERTY(PenJoinStyle joinStyle READ joinStyle WRITE setJoinStyle USER true)
    Q_PROPERTY(qreal miterLimit READ miterLimit WRITE setMiterLimit USER true)
    Q_PROPERTY(PenStyle style READ style WRITE setStyle USER true)
    Q_PROPERTY(qreal width READ width WRITE setWidth USER true)

public:

    enum PenStyle   // pen style
    {
        NoPen,
        SolidLine,
        DashLine,
        DotLine,
        DashDotLine,
        DashDotDotLine,
        CustomDashLine
#ifndef Q_MOC_RUN
        , MPenStyle = 0x0f
#endif
    };

    enum PenCapStyle   // line endcap style
    {
        FlatCap = 0x00,
        SquareCap = 0x10,
        RoundCap = 0x20,
        MPenCapStyle = 0x30
    };

    enum PenJoinStyle   // line join style
    {
        MiterJoin = 0x00,
        BevelJoin = 0x40,
        RoundJoin = 0x80,
        SvgMiterJoin = 0x100,
        MPenJoinStyle = 0x1c0
    };


public:
    Pen(QObject* parent = 0, QObject* object = 0, int property = -1, const PropertyModel * propertyModel = 0);

    QWidget* createEditor(QWidget * parent, const QModelIndex & index);

    QVariant data(const QModelIndex & index);
    bool setData(QVariant data, const QModelIndex & index);
    void setEditorData(QWidget * /*editor*/, const QModelIndex & /*index*/){}

    bool canHandle(QObject * object, int property)const;
    Qt::ItemFlags flags(const QModelIndex &index);

    QBrush brush();
    void setBrush(const QBrush & brush);

    PenCapStyle capStyle();
    void setCapStyle(PenCapStyle capStyle);

    QColor color();
    void setColor(const QColor & color);

    PenJoinStyle joinStyle();
    void setJoinStyle(PenJoinStyle joinStyle);

    qreal miterLimit();
    void setMiterLimit(qreal miterLimit);

    PenStyle style();
    void setStyle(PenStyle style);

    qreal width();
    void setWidth(qreal width);

    PropertyInterface* createInstance(QObject * object, int property, const PropertyModel * propertyModel) const;
};

#endif
