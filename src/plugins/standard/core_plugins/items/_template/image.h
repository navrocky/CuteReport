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

#ifndef IMAGE_H
#define IMAGE_H

#include <iteminterface.h>


class Image : public Report::ItemInterface
{
	Q_OBJECT
    Q_INTERFACES(Report::ItemInterface)

//	Q_ENUMS(ImageTypes PaintTypes FrameTypes DrawTextTypes);
    Q_PROPERTY(PaintTypes howToPaintImage READ paintType WRITE setPaintType)
	Q_PROPERTY(DrawTextTypes drawTextType READ drawTextType WRITE setDrawTextType)
	Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap)
	Q_PROPERTY(QString datasetField READ datasetField WRITE setDatasetField)
	Q_PROPERTY(QString text READ comment WRITE setComment)
	Q_PROPERTY(QFont font READ font WRITE setFont)
	Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
	Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth)
	Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)

public:
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
        IMAGE_BIND = 1,
        IMAGE_BOX = 2
	};

	enum PaintTypes
	{
        RESIZE_ITEM_TO_Image = 1,
        CENTER_IMAGE_TO_ITEM = 2,
        RESIZE_IMAGE_TO_ITEM_KEEP_ASPECT_RATIO = 3,
        RESIZE_IMAGE_TO_ITEM_IGNORE_ASPECT_RATIO = 4
	};


public:
    Image(QGraphicsItem* parent = 0, QObject* parentObject = 0);

//	bool prePaint(QPainter * painter, Report::PaintInterface::PrintMode pMode = Report::PaintInterface::pmNormal);
//	bool postPaint();
	bool prData();
	bool prReset();

	QRectF boundingRect() const;
	void _paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QRectF & rect, QWidget * widget = 0);

	QIcon toolBoxIcon();
	QString toolBoxText();
	QString toolBoxGroup();


	PaintTypes paintType();
	void setPaintType(PaintTypes paintType);

	FrameTypes frameType();
	void setFrameType(FrameTypes frameType);

	DrawTextTypes drawTextType();
	void setDrawTextType(DrawTextTypes drawTextType);

	QPixmap pixmap();
	void setPixmap(const QPixmap & pixmap);

	QString comment();
	void setComment(const QString & comment);

	QColor textColor();
	void setTextColor(const QColor & color);

	int borderWidth();
	void setBorderWidth(int bWidth);

	QColor borderColor();
	void setBorderColor(const QColor & color);

	QObject * createInstance(QGraphicsItem* parent = 0, QObject* parentObject = 0);

	QFont font();
	void setFont(const QFont & font);

	QString datasetField();
	void setDatasetField(QString str);

private:
	PaintTypes m_paintType;
	FrameTypes m_frameType;
	DrawTextTypes m_drawTextType;
	QString m_comment;
	QString m_primaryMessage;
	QString m_script;
	QImage  m_image;
	QColor m_textColor;
	QColor m_borderColor;
	int m_borderWidth;
	QBrush m_emptyBrush;
	QFont m_font;
	QString m_datasetField;
};


#endif
