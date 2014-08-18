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
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/
#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QFrame>
#include "types.h"

class QToolButton;
class QLabel;
class MessageList;

class MessageWidget : public QFrame
{
    Q_OBJECT

    Q_PROPERTY(QColor highlightColor READ highlightColor WRITE setHighlightColor)
public:
    explicit MessageWidget(QWidget *parent = 0);

    void startAnimation();
    void setHighlightColor(const QColor & new_color);
    QColor highlightColor() const;

public slots:
    void pushMessage(CuteReport::LogLevel level, const QString & sender, const QString & shortMessage, const QString & fullMessage);
    void clear();
    
signals:
    
private slots:
    void buttonClicked();

private:
    QToolButton * m_button;
    QLabel * m_label;
    MessageList * m_view;
    QPropertyAnimation * m_animation;
    QColor m_animationColor;
};

#endif // MESSAGEWIDGET_H
