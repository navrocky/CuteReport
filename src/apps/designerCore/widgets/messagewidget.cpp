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
#include "messagewidget.h"
#include "messagelist.h"

#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>

MessageWidget::MessageWidget(QWidget *parent) :
    QFrame(parent),
    m_animation(0)
{
    delete this->layout();

    QHBoxLayout * layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    m_button = new QToolButton(this);
    m_button->setIcon(QIcon(":images/arrow_up_6_green.png"));
    m_button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    m_button->setAutoRaise(true);
    m_button->setAutoFillBackground(true);
    m_label = new QLabel(this);
    layout->addWidget(m_button);
    layout->addWidget(m_label);

    this->setLayout(layout);

    m_view = new MessageList(m_button);
    m_view->setWindowFlags(m_view->windowFlags() | Qt::Popup);

    connect(m_button, SIGNAL(clicked()), this, SLOT(buttonClicked()));
}


void MessageWidget::pushMessage(CuteReport::LogLevel level, const QString & sender, const QString & shortMessage, const QString & fullMessage)
{
    if (level != CuteReport::LogDebug)
        m_label->setText(shortMessage);
    if (level == CuteReport::LogError) {
        m_button->setIcon(QIcon(":images/arrow_up_6_red.png"));
        if (!m_animation || m_animation->state() != QPropertyAnimation::Running)
            startAnimation();
    }

    m_view->addMessage(level, sender, shortMessage, fullMessage);
}


void MessageWidget::clear()
{
    m_view->clear();
    m_button->setIcon(QIcon(":images/arrow_up_6_green.png"));
    if (m_animation)
        m_animation->stop();
}


void MessageWidget::buttonClicked()
{
    QRect geometry = m_view->geometry();
    geometry.moveBottomLeft(this->mapToGlobal(m_button->geometry().topLeft()));
    geometry.setWidth(this->width());
    m_view->setGeometry(geometry);
    m_view->show();
}


void MessageWidget::startAnimation()
{
    if (!m_animation) {
        m_animation = new QPropertyAnimation(this, "highlightColor");
        m_animation->setKeyValueAt(0, palette().color(QPalette::Button));
        m_animation->setKeyValueAt(0.5, QColor(255,0,0));
        m_animation->setKeyValueAt(1, palette().color(QPalette::Button));
        m_animation->setDuration(800);
        m_animation->setLoopCount(8);
        m_animation->setEasingCurve(QEasingCurve::Linear);

        m_animationColor = QColor(palette().color(QPalette::Button));
    }

    m_animation->start();
}


QColor MessageWidget::highlightColor() const
{
    return m_animationColor;
}


void MessageWidget::setHighlightColor(const QColor & new_color)
{
    m_animationColor = new_color;
    QPalette pal(palette());
    pal.setColor(QPalette::Button, new_color);

    m_button->setPalette(pal);
}
