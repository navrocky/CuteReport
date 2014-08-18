/***************************************************************************
 *   This file is part of the CuteReport project                           *
 *   Copyright (C) 2012-2013 by Alexander Mikhalov                              *
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

ItemsToolWidget::ItemsToolWidget(QWidget * parent)
    :QToolBar( parent )
    , m_menu(0)
{
    this->setObjectName("ItemsToolWidget");
    this->setWindowTitle(tr ("Items Tool Widget"));
//    QString category;
//    bool allIconsExists = true;

//    if (!allIconsExists)
//		setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

}

void ItemsToolWidget::addItem(const QIcon & icon, const QString & name, const QString & className, const QString & group)
{
    QAction * newItem = new QAction(icon, name, this );
    newItem->setData(className);

    if (group.isEmpty()) {
        addAction( newItem );
    } else {
        Menu * menu;
        if (menus.contains(group))
            menu = menus.value(group);
        else {
            QIcon groupIcon(QString(REPORT_IMAGES_PATH) + "/group_" + group + ".png");
            QAction * groupAction = new QAction(groupIcon, group, this );
            addAction(groupAction);
            menu = new Menu( this );
            menus[group] = menu;
            connect ( groupAction, SIGNAL(hovered()), this, SLOT(showItems()) );
            connect ( groupAction, SIGNAL(triggered()), this, SLOT(showItems()) );
        }
        menu->addAction( newItem );
    }

}


void ItemsToolWidget::showItems()
{
    if (m_menu)
	m_menu->hide();
    m_menu = menus[((QAction*)sender())->text()];

    if (orientation () == Qt::Vertical)
		m_menu->popup(  mapToGlobal( QPoint(this->geometry().width() , widgetForAction((QAction*)sender())->pos().y()) ) );
    else
		m_menu->popup(  mapToGlobal( QPoint(widgetForAction((QAction*)sender())->pos().x(), this->geometry().height()) ) );
}

void ItemsToolWidget::leaveEvent ( QEvent * event )
{
    QToolBar::leaveEvent(event);
    QTimer::singleShot(100,this,SLOT(tryHideMenu()));
}


//Qt::Orientation ItemsToolWidget::orientation ()
//{
//    return geometry.height() > geometry().width() ? Qt::Vertical : Qt::Horizontal;
//}


void ItemsToolWidget::tryHideMenu()
{
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

        mimeData->setText(QString("CuteReport::%1").arg(action->data().toString()));
//        mimeData->setData("pluginPointer");
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
