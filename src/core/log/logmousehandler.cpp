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
 ****************************************************************************/
#include "logmousehandler.h"
#include <QtGui>
#include <QGraphicsSceneMouseEvent>
#include "core.h"
#include "buttonitem.h"
#include "menugriditem.h"
#include "optionitem.h"
#include "log.h"

LogMouseHandler::LogMouseHandler(Log *logObject):
    QObject(logObject)
  , m_lastPressPos(-1,-1)
{

}

bool LogMouseHandler::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::GraphicsSceneMouseDoubleClick) {
        QGraphicsSceneMouseEvent *e = (QGraphicsSceneMouseEvent *)event;
        QString widgetName = detectObject(e);
        if (!widgetName.isEmpty())
            log_metric( QString("pos:%1,%2").arg(e->pos().x()).arg(e->pos().y()), widgetName, "mouse_doubleclick");
    }

    if (event->type() == QEvent::GraphicsSceneMousePress) {
        QGraphicsSceneMouseEvent *e = (QGraphicsSceneMouseEvent *)event;
        m_lastPressPos = e->scenePos();
        m_lastPressObject = detectObject(e);
    }

    if (event->type() == QEvent::GraphicsSceneMouseRelease) {
        QGraphicsSceneMouseEvent *e = (QGraphicsSceneMouseEvent *)event;
        QString widgetName = detectObject(e);
        if (e->scenePos() == m_lastPressPos)
            log_metric(QString("pos:%1,%2").arg(e->scenePos().x()).arg(e->scenePos().y()), widgetName, "mouse_click" );
        else {
            if (widgetName == m_lastPressObject)
                log_metric( QString("from pos:(%1,%2) to:(%3,%4)").arg(m_lastPressPos.x()).arg(m_lastPressPos.y())
                           .arg(e->scenePos().x()).arg(e->scenePos().y()),  widgetName, "mouse_move");
            else
                log_metric( QString("from pos:(%1,%2) to:(%3,%4)").arg(m_lastPressPos.x()).arg(m_lastPressPos.y())
                           .arg(e->scenePos().x()).arg(e->scenePos().y()), QString("from object: \\'%1\\' to: \\'%2\\'").arg(m_lastPressObject).arg(widgetName),
                            "mouse_move");
        }

    }
    return false;
}

QString LogMouseHandler::detectObject(QGraphicsSceneMouseEvent *mouseEvent)
{
    bool detected;
    QString name;
    QPointF pos = mouseEvent->pos();


    foreach (QGraphicsItem * item, gCore->scene()->items(pos, Qt::ContainsItemBoundingRect, Qt::DescendingOrder)) {
        detected = false;

        switch (item->type()) {
        case ButtomItemType:
            {
            ButtonItem * button = dynamic_cast<ButtonItem*>(item);
            if (!button)
                break;
            name =  button->objectName();
            detected = true;
            }
            break;
        case OptionItemType:
            {
            OptionItem * oItem = dynamic_cast<OptionItem*>(item);
            if (!oItem)
                break;
            name =  oItem->modifier()->itemname;
            detected = true;
            }
            break;
        case MenuGridItemType:
            {
            MenuGridItem * mItem = dynamic_cast<MenuGridItem*>(item);
            if (!mItem)
                break;
            name =  mItem->objectName();
            detected = true;
            }
            break;
        }
        if (detected)
            break;
    }

    return name;
}

    /*
    bool detected = false;
    QString name;
    QPointF pos = event->pos();

    foreach (QGraphicsItem * item, gCore->scene()->items(pos, Qt::ContainsItemBoundingRect, Qt::DescendingOrder)) {
        //int grItem = gCore->scene()->itemsAt(pos)->type();
        detected = false;
        //printf("type = %i \n",item->type());

        switch (item->type()) {
        case ButtomItemType:
            //printf("-     Button \n");
            {
            ButtonItem * button = dynamic_cast<ButtonItem*>(item);
            if (!button)
                break;
            name =  button->objectName();
            detected = true;
            }
            break;
        case OptionItemType:
            //printf("-     OptionItem \n");
            {
            OptionItem * oItem = dynamic_cast<OptionItem*>(item);
            if (!oItem)
                break;
            name =  oItem->modifier()->itemname;
            detected = true;
            }
            break;
        case MenuGridItemType:
            //printf("-     MenuGridItem \n");
            {
            MenuGridItem * mItem = dynamic_cast<MenuGridItem*>(item);
            if (!mItem)
                break;
            name =  mItem->objectName();
            detected = true;
            }
            break;
        }
        if (detected)
            break;
    }

    if (detected) {
        if (m_metricEnabled)
            log_metric("button_pressed", name, QString("pos:%1,%2").arg(pos.x()).arg(pos.y()));
        if (m_logEnabled) {
            QFile log("events.log");
            if (log.open(QFile::WriteOnly| QFile::Append)) {
                QTextStream out(&log);
                out << QDateTime::currentDateTime().toMSecsSinceEpoch() << ":" << pos.x() << "," << pos.y() << ":button:" << name << "\n";
                log.close();
            }
        }
    }
    */
