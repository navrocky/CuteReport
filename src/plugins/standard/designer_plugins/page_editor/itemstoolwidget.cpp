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
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/
#include "itemstoolwidget.h"

#include <QTimer>
#include <QMouseEvent>
#include <QDrag>
#include <QPushButton>
#include <QLayout>

ItemsToolWidget::ItemsToolWidget(QWidget * parent, const QString & imagePath)
    :QWidget( parent )
    ,m_menu(0)
    ,m_imagePath(imagePath)
{
    this->setObjectName("ItemsToolWidget");
    this->setWindowTitle(tr ("Items Tool Widget"));

    m_hideDelay.setInterval(500);
    m_hideDelay.setSingleShot(true);
    connect(&m_hideDelay, SIGNAL(timeout()), this, SLOT(tryHideMenu()));
}


void ItemsToolWidget::setImagesPath(const QString &path)
{
    m_imagePath = path;
}


void ItemsToolWidget::addItem(const QIcon & icon, const QString & name, const QString &suiteName, const QString & group)
{
    if (!layout()) {
        this->setLayout(new QHBoxLayout(this));
        this->layout()->setContentsMargins ( 1, 1, 1, 1 );
    }

    ToolButton * groupButton;

    if (m_groups.contains(group)) {
        groupButton = m_groups.value(group, 0);
    } else {
        QString filePath = m_imagePath + "/group_" + group + ".png";
        QIcon groupIcon(filePath);
        groupButton = new ToolButton(groupIcon, "", this);
        this->layout()->addWidget(groupButton);
        m_groups.insert(group, groupButton);
    }

    if (group.isEmpty()) {
        ToolButton * itemButton = new ToolButton(icon, name, this);
        this->layout()->addWidget(itemButton);
    } else {
        Menu * menu;
        if (m_menus.contains(group))
            menu = m_menus.value(group);
        else {
            menu = new Menu( this );
            groupButton->setMenu(menu);
            m_menus.insert(group, menu);
        }
        QString actionName = (suiteName.isEmpty() || (suiteName == "Standard")) ? name : QString("%1 (%2)").arg(name, suiteName);
        QAction * newItem = new QAction(icon, actionName, this );
        newItem->setData(suiteName.isEmpty() ? name : suiteName + "::" + name);
        menu->addAction( newItem );
    }


    /*
    QAction * newItem = new QAction(icon, name, this );
    //    newItem->setData(className);

    if (group.isEmpty()) {
        addAction( newItem );
    } else {
        Menu * menu;
        if (menus.contains(group))
            menu = menus.value(group);
        else {
            QIcon groupIcon( m_imagePath + "/group_" + group + ".png");
            QAction * groupAction = new QAction(groupIcon, group, this );
            addAction(groupAction);
            menu = new Menu( this );
            menus[group] = menu;
            connect ( groupAction, SIGNAL(hovered()), this, SLOT(showItems()) );
            connect ( groupAction, SIGNAL(triggered()), this, SLOT(showItems()) );
        }
        menu->addAction( newItem );
    }
*/
}


void ItemsToolWidget::showItems()
{
//    qDebug() << "showItems";
//    if (m_menu) {
//        qDebug() << "test1";
//        m_menu->hide();
//        m_hideDelay.stop();
//    }
//    qDebug() << "test2";
//    m_menu = menus[((QAction*)sender())->text()];

//    if (isEnabled()) {
//        m_menu->popup(  mapToGlobal( QPoint(widgetForAction((QAction*)sender())->pos().x(), this->geometry().height()) ) );
//    }

    //    if (isEnabled()) {
    //        if (orientation () == Qt::Vertical) {
    //            qDebug() << "test3";
    //            m_menu->popup(  mapToGlobal( QPoint(this->geometry().width() , widgetForAction((QAction*)sender())->pos().y()) ) );
    //        }
    //        else {
    //            qDebug() << "test4";
    //            m_menu->popup(  mapToGlobal( QPoint(widgetForAction((QAction*)sender())->pos().x(), this->geometry().height()) ) );
    //        }
    //    }
}

void ItemsToolWidget::leaveEvent ( QEvent * event )
{
    qDebug() << "leaveEvent";
    QWidget::leaveEvent(event);
    m_hideDelay.start();
}


//Qt::Orientation ItemsToolWidget::orientation ()
//{
//    return geometry.height() > geometry().width() ? Qt::Vertical : Qt::Horizontal;
//}


void ItemsToolWidget::tryHideMenu()
{
    qDebug() << "tryHideMenu";
    if ((m_menu) && (!m_menu->underMouse()))
        m_menu->hide();
}



//-------------Menu
Menu::Menu(QWidget * parent )
    :QMenu(parent)
{
    this->setAcceptDrops( true );
}


void Menu::leaveEvent ( QEvent * event )
{
    qDebug() << "Menu::leaveEvent";
    Q_UNUSED(event)
    hide ();
}


void Menu::mousePressEvent ( QMouseEvent * event )
{
    QWidget::mousePressEvent( event );
    QAction * action = actionAt ( event->pos());
    if (!action)
        return;

    if (event->button() == Qt::LeftButton)
    {
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;

        mimeData->setText(QString("CuteReport::BaseItemInterface::%1").arg(action->data().toString()));
        drag->setMimeData(mimeData);
        drag->setPixmap(action->icon().pixmap(100,100));

        drag->exec();
    }
}

void Menu::dragEnterEvent(QDragEnterEvent *event)
{
    QMenu::dragEnterEvent( event );
    event->acceptProposedAction();
}

void Menu::dragLeaveEvent ( QDragLeaveEvent * /*event*/ )
{
    hide();
}





ToolButton::ToolButton(const QIcon &icon, const QString &text, QWidget *parent)
    :QPushButton(icon, text, parent)
{
    //connect(this, SIGNAL(pressed())
}


void ToolButton::enterEvent(QEvent *event)
{
//    qDebug() << "ToolButton::enterEvent";
//    click();
    QPushButton::enterEvent(event);
}


void ToolButton::leaveEvent(QEvent *event)
{
    //emit hideMenu();
    QPushButton::leaveEvent(event);
}
