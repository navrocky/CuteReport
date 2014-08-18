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

#include <QtCore>
#include <QBrush>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QFontMetricsF>

#include "image.h"

inline void initMyResource()
{
	Q_INIT_RESOURCE(picture);
}

Image::Image(QGraphicsItem* parent, QObject* parentObject) : ItemInterface(parent, parentObject)
{
    m_paintType = RESIZE_IMAGE_TO_ITEM_KEEP_ASPECT_RATIO;
	m_frameType = NO_FRAME;
	m_comment = "picture";
	m_textColor = Qt::black;
	m_borderColor = Qt::black;
	m_borderWidth = 0;
	m_drawTextType = NO_TEXT;
	initMyResource();
	setWidth(20/UNIT); //20 mm
	setHeight(20/UNIT); // 20 mm
	m_emptyBrush = QBrush(QImage(":/empty.png"));
	m_font=QFont("Serif");
	m_font.setPointSizeF(3.5);
	m_font.setStyleStrategy(QFont::PreferMatch);
	m_font.setStyleStrategy(QFont::ForceOutline);
}


bool Image::prData()
{ 
    qDebug("================Picture::prData()");
    if (stringIsField(m_datasetField))
    {
	qDebug("isField");
	QString dataset;
	QString field;
	stringToField (m_datasetField, &dataset, &field);
	qDebug("dataset = %s,   field=%s", qPrintable(dataset), qPrintable(field));
	if (!m_image.loadFromData(ItemInterface::datasetField(dataset,field).toByteArray()))
	    qDebug("ERROR loading image from field!!!");

    }
    else qDebug("no field found");
    return true;
}

bool Image::prReset()
{
    if (stringIsField(m_datasetField))
	m_image = QImage();
    return true;
}

Image::PaintTypes Picture::paintType()
{
	return m_paintType;
}

void Image::setPaintType(PaintTypes paintType)
{
	m_paintType = paintType;
	update();
}

Image::FrameTypes Picture::frameType()
{
	return m_frameType;
}

void Image::setFrameType(FrameTypes frameType)
{
	m_frameType = frameType;
	update();
}

Image::DrawTextTypes Picture::drawTextType()
{
	return m_drawTextType;
}

void Image::setDrawTextType(DrawTextTypes drawTextType)
{
	m_drawTextType = drawTextType;
	update();
}


QString Image::comment()
{
	return m_comment;
}


void Image::setComment(const QString & comment)
{
	m_comment = comment;
	update();
}


QRectF Image::boundingRect() const
{
	return QRectF(0, 0, width(), height());
}

void Image::_paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QRectF & rect,  QWidget * /*widget*/)
{
	QString m_text;
	m_text=m_comment;

	painter->setFont(fontConvert(m_font));

	qreal textH  = 0;
	qreal sw=0;
	qreal sh=0;

	if (m_drawTextType == DRAW_ABOVE || m_drawTextType == DRAW_BELOW)
		textH = QFontMetricsF(painter->font()).height();

	if (m_image.isNull())
	{
		painter->setBrush(m_emptyBrush);
		(option->type == QStyleOption::SO_GraphicsItem)  ?
		painter->drawRect(rect) : 	painter->drawText(rect, Qt::AlignCenter, "No Picture");
	}
	else
	{
		QImage img(0,0/*,QImage::Format_ARGB32_Premultiplied*/);

		QPointF point = rect.topLeft();
		int cx=0,cy=0,cw=m_image.width(),ch=m_image.height();

		switch (m_paintType)
		{

			case RESIZE_PICTURE_TO_ITEM_KEEP_ASPECT_RATIO:
				img = m_image.scaled(rect.width(), rect.height() - textH , Qt::KeepAspectRatio, Qt::SmoothTransformation);
				break;

			case RESIZE_PICTURE_TO_ITEM_IGNORE_ASPECT_RATIO:
				img = m_image.scaled(rect.width(), rect.height() - textH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
				break;

			case CENTER_PICTURE_TO_ITEM:
				point.setX(point.x()+(rect.width()-m_image.width())/2);
				point.setY(point.y()+(rect.height()-m_image.height()-textH)/2);

				if (point.x()<0)
				{
					cx=abs(point.x());
					cw-=2*cx;
					point.setX(0);
				}

				if (point.y()<0)
				{
					cy=abs(point.y());
					ch-=2*cy;
					point.setY(0);
				}

				img=m_image.copy(cx,cy,cw,ch);

				break;

			case RESIZE_ITEM_TO_PICTURE:
				img=m_image;
				sw=img.width()-width();
				sh=img.height()-(height()-textH);
				break;
		}

		if (m_drawTextType == DRAW_ABOVE)
			point.setY(point.y()+textH);;

		painter->drawImage(point, img);
	}

	painter->setPen(m_textColor);

	switch (m_drawTextType)
	{

		case DRAW_TOP:
			painter->drawText(rect, Qt::AlignTop| Qt::AlignHCenter, m_text);
			break;

		case DRAW_BOTTOM:
			painter->drawText(rect, Qt::AlignBottom| Qt::AlignHCenter, m_text);
			break;

		case DRAW_ABOVE:
			painter->drawText(rect, Qt::AlignTop| Qt::AlignHCenter, m_text);
			break;

		case DRAW_BELOW:
			painter->drawText(rect, Qt::AlignBottom| Qt::AlignHCenter, m_text);
			break;

		default:
			break;
	}

	if (sw || sh)
	{
		setWidth(width()+sw);
		setHeight(height()+sh);
	}

//	if (m_borderWidth > 0)
//	{
//		QPen pen;
//		pen.setWidth(m_borderWidth);
//		pen.setColor(m_borderColor);
//		pen.setJoinStyle(Qt::MiterJoin);
//		painter->setPen(pen);
//		painter->setBrush(Qt::NoBrush);
//		painter->drawRect(rect.x() + m_borderWidth/2, rect.y() + m_borderWidth/2,
//		                  rect.width() - m_borderWidth, rect.height() - m_borderWidth);
//	}


//	if (option->type == QStyleOption::SO_GraphicsItem)
//	    drawSelection(painter, rect);
//
//	if (option->type != QStyleOption::SO_GraphicsItem)
//		emit afterPrint(this);
}


QIcon Image::toolBoxIcon()
{
	return QIcon(":/picture.png");
};

QString Image::toolBoxText()
{
	return tr("Picture");
}

QString Image::toolBoxGroup()
{
	return tr("Shapes");
}


QObject * Image::createInstance(QGraphicsItem* parent, QObject* parentObject)
{
	return new Picture(parent, parentObject);
}


QPixmap Image::pixmap()
{
	QPixmap pixmap;
	return pixmap.fromImage(m_image) ;
}

void Image::setPixmap(const QPixmap & pixmap)
{
	m_image = QImage(pixmap.toImage()) ;
	update();
}

QColor Image::textColor()
{
	return m_textColor;
}

void Image::setTextColor(const QColor & color)
{
	m_textColor = color;
	update();
}

int Image::borderWidth()
{
	return m_borderWidth;
}

void Image::setBorderWidth(int bWidth)
{
	if (bWidth < 0)
		m_borderWidth = 0;
	else
		if (bWidth > 5)
			m_borderWidth = 5;
		else
			m_borderWidth = bWidth;

	update();
}

QColor Image::borderColor()
{
	return m_borderColor;
}

void Image::setBorderColor(const QColor & color)
{
	m_borderColor = color;
	update();
}

QFont Image::font()
{
	return m_font;
}

void Image::setFont(const QFont & font)
{
	m_font = font;
	update();
}

QString Image::datasetField()
{
    return m_datasetField;
}

void Image::setDatasetField(QString str)
{
    m_datasetField = str;
}

#ifndef STATIC_SUITE
Q_EXPORT_PLUGIN2(image, Image)
#endif
