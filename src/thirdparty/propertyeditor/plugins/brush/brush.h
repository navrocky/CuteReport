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

#ifndef BRUSH_H
#define BRUSH_H

#include <QBrush>
#include <propertyinterface.h>

using namespace PropertyEditor;

class Brush : public PropertyInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "eu.licentia.PropertyEditor.PropetyInterface/1.0")
#endif
    Q_INTERFACES(PropertyEditor::PropertyInterface)

    Q_ENUMS(BrushStyle)

    Q_PROPERTY(BrushStyle style READ style WRITE setStyle USER true)
    Q_PROPERTY(QColor color READ color WRITE setColor USER true)
    Q_PROPERTY(QPixmap texture READ texture WRITE setTexture USER true)

public:

    enum BrushStyle   // brush style
    {
        NoBrush,
        SolidPattern,
        Dense1Pattern,
        Dense2Pattern,
        Dense3Pattern,
        Dense4Pattern,
        Dense5Pattern,
        Dense6Pattern,
        Dense7Pattern,
        HorPattern,
        VerPattern,
        CrossPattern,
        BDiagPattern,
        FDiagPattern,
        DiagCrossPattern,
        LinearGradientPattern,
        RadialGradientPattern,
        ConicalGradientPattern,
        TexturePattern = 24
#if defined(QT3_SUPPORT) && !defined(Q_MOC_RUN)
        , CustomPattern = TexturePattern
#endif
    };

public:
    Brush(QObject* parent = 0, QObject* object = 0, int property = -1, const PropertyModel * propertyModel = 0);

    QWidget* createEditor(QWidget * parent, const QModelIndex & index);

    QVariant data(const QModelIndex & index);
    bool setData(QVariant data, const QModelIndex & index);
    void setEditorData(QWidget * /*editor*/, const QModelIndex & /*index*/){};

    bool canHandle(QObject * object, int property)const;
    Qt::ItemFlags flags(const QModelIndex &index);

    Brush::BrushStyle style();
    void setStyle(Brush::BrushStyle style);

    QColor color();
    void setColor(QColor color);

    QPixmap texture();
    void setTexture(const QPixmap & texture);

    PropertyInterface* createInstance(QObject * object, int property, const PropertyModel * propertyModel) const;


};

#endif
