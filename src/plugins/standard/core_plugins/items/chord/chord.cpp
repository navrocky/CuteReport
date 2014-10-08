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

#include "chord.h"
#include "chord_p.h"
#include "chordscripting.h"

#include <QtCore>
#include <QtGui>

using namespace CuteReport;

inline void initResource() { Q_INIT_RESOURCE(chord); }

ChordItem::ChordItem(QObject *parent)
    :ItemInterface(new ChordItemPrivate, parent)
{
    Q_INIT_RESOURCE(chord);
    Q_D(ChordItem);
    setResizeFlags( ResizeLeft | ResizeRight | ResizeTop | ResizeBottom);
    d->rect = QRectF(0,0,30,30);
    d->frame = 0;
}


void ChordItem::moduleInit()
{
    initResource();
}


ChordItem::ChordItem(ChordItemPrivate *dd, QObject * parent)
    :ItemInterface(dd, parent)
{
    Q_INIT_RESOURCE(chord);
}


BaseItemInterface *ChordItem::itemClone() const
{
    Q_D(const ChordItem);
    return new ChordItem(new ChordItemPrivate(*d), parent());
}


QByteArray ChordItem::serialize()
{
    Q_D(ChordItem);
    QByteArray ba;
    QDataStream s( &ba, QIODevice::ReadWrite );
    s << *d;
    return ba;
}


void ChordItem::deserialize(QByteArray &data)
{
    Q_D(ChordItem);
    QDataStream s( &data, QIODevice::ReadWrite );
    s >> *d;
}


QDataStream &operator<<(QDataStream &s, const ChordItemPrivate &p) {
    s << static_cast<const ItemInterfacePrivate&>(p);
    s << p.pen << (qint16)p.startAngle << (qint16)p.spanAngle;
    return s;
}


QDataStream &operator>>(QDataStream &s, ChordItemPrivate &p) {
    s >> static_cast<ItemInterfacePrivate&>(p);
    qint16 startAngle, spanAngle;

    s >> p.pen; s >> startAngle; s >> spanAngle;
    p.spanAngle = startAngle;
    p.spanAngle = spanAngle;

    return s;
}


BaseItemInterface *ChordItem::createInstance(QObject * parent) const
{
    return new ChordItem(parent);
}


BaseItemHelperInterface *ChordItem::helper()
{
    return 0;
}


void ChordItem::init_gui()
{
    if (m_gui)
        return;
    m_gui = new ChordItemView(this);
}


bool ChordItem::canContain(QObject * object)
{
    return (dynamic_cast<CuteReport::BaseItemInterface*>(object));
}


QIcon ChordItem::itemIcon() const
{
    return QIcon(":/chord.png");
}


QString ChordItem::moduleShortName() const
{
    return tr("Chord");
}


QString ChordItem::itemGroup() const
{
    return QString("Shapes");
}


bool ChordItem::renderPrepare()
{
    emit printBefore();
    setRenderingPointer(new ChordItemPrivate(*(reinterpret_cast<ChordItemPrivate*>(d_ptr))));
    emit printDataBefore();
    emit printDataAfter();
    return true;
}


RenderedItemInterface *ChordItem::renderView()
{
    Q_D(ChordItem);
    RenderedChordItem * view = new RenderedChordItem(this, new ChordItemPrivate(*d));
    return view;
}


void ChordItem::paint(QPainter * painter, const QStyleOptionGraphicsItem *option, const BaseItemInterfacePrivate * data, const QRectF &boundingRect, RenderingType type)
{
    Q_UNUSED(option)
    Q_UNUSED(type)
    ItemInterface::paintBegin(painter, option, data, boundingRect, type);

    const ChordItemPrivate * const d = static_cast<const ChordItemPrivate*>(data);

    painter->setPen(d->pen);
    painter->setBrush(d->brush);
    painter->drawChord(boundingRect, d->startAngle*16, d->spanAngle*16);

    ItemInterface::paintEnd(painter, option, data, boundingRect, type);
}


QPen ChordItem::pen() const
{
    Q_D(const ChordItem);
    return d->pen;
}


void ChordItem::setPen(const QPen & pen)
{
    Q_D(ChordItem);
    if (d->pen != pen) {
        d->pen = pen;
        update_gui();
        emit penChanged(d->pen);
        emit changed();
    }
}


QBrush ChordItem::brush() const
{
    Q_D(const ChordItem);
    return d->brush;
}


void ChordItem::setBrush(const QBrush & brush)
{
    Q_D(ChordItem);
    if (d->brush != brush) {
        d->brush = brush;
        update_gui();
        emit brushChanged(d->brush);
        emit changed();
    }
}

int ChordItem::spanAngle() const
{
    Q_D(const ChordItem);
    return d->spanAngle;
}


void ChordItem::setSpanAngle(int span)
{
    Q_D(ChordItem);
    if (span == d->spanAngle)
        return;

    d->spanAngle = span;
    update_gui();
    emit spanAngleChanged(d->spanAngle);
    emit changed();
}


int ChordItem::startAngle() const
{
    Q_D(const ChordItem);
    return d->startAngle;
}


void ChordItem::setStartAngle(int start)
{
    Q_D(ChordItem);
    if (d->startAngle == start)
        return;
    d->startAngle = start;
    update_gui();
    emit startAngleChanged(d->startAngle);
    emit changed();
}


void ChordItem::initScript(QScriptEngine *scriptEngine)
{
    registerChordItemScriptClass(scriptEngine);
}


QString ChordItem::_current_property_description() const
{
    QString propertyName = metaObject()->property(m_currentProperty).name();

    if (propertyName == "pen")
        return tr("Pen parameters");
    else if (propertyName == "brush")
        return tr("Brush options for fill chord");
    else if (propertyName == "startAngle")
        return tr("A number of degrees where chord started");
    else if (propertyName == "spanAngle")
        return tr("A number of degrees from startAngle");
    else
        return ItemInterface::_current_property_description();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Chord, ChordItem)
#endif
