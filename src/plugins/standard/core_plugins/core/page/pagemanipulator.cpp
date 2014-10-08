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
#include "pagemanipulator.h"
#include "pagegui.h"
#include "magnets.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QAction>
#include <QtGui>

#define MANIPULATOR_ID 17322121

using namespace CuteReport;

SUIT_BEGIN_NAMESPACE

PageManipulator::PageManipulator(QObject *parent)
    : PageManipulatorInterface(parent),
      m_activePage(0)
{
    initActions();
}


PageManipulator::~PageManipulator()
{
    qDeleteAll(m_actions);
}


void PageManipulator::setActivePage(PageInterface * page)
{
    if (!page) {
        m_activePage = 0;
        return;
    }

    if (page->pageManupilatorID() == MANIPULATOR_ID) {
        m_activePage = qobject_cast<Page*>(page);
        setMagnetButtonsState();
    }
}


PageInterface * PageManipulator::activePage()
{
    return m_activePage;
}


int PageManipulator::pageManupilatorId()
{
    return MANIPULATOR_ID;
}


QList<PageAction *> PageManipulator::actions()
{
    return m_actions;
}


QList<QLabel*> PageManipulator::statusBarLabels()
{
    if (!m_activePage)
        return QList<QLabel*>();

    if (!m_activePage->m_gui)
        m_activePage->initGUI();

    return m_activePage->m_gui->statusBarLabels();
}

void PageManipulator::initActions()
{
    PageAction * zoomIn = new PageAction();
    zoomIn->action = new QAction(QIcon(":/images/zoom-in.png"), tr("Zoom in"), this);
    zoomIn->action->setStatusTip(tr("Zoom current page in"));
    connect(zoomIn->action, SIGNAL(triggered()), this, SLOT(slotZoomIn()));
    m_actions.append(zoomIn);

    PageAction * zoomOut = new PageAction();
    zoomOut->action = new QAction(QIcon(":/images/zoom-out.png"), tr("Zoom out"), this);
    zoomOut->action->setStatusTip(tr("Zoom current page out"));
    connect(zoomOut->action, SIGNAL(triggered()), this, SLOT(slotZoomOut()));
    m_actions.append(zoomOut);

    PageAction * zoomOrig = new PageAction();
    zoomOrig->action = new QAction(QIcon(":/images/zoom-original.png"), tr("Zoom Original size"), this);
    zoomOrig->action->setStatusTip(tr("Zoom current page to original size"));
    connect(zoomOrig->action, SIGNAL(triggered()), this, SLOT(slotZoomOrig()));
    m_actions.append(zoomOrig);

//    PageAction * zoomFit = new PageAction();
//    zoomFit->action = new QAction(QIcon(":/images/zoom-fit-best.png"), tr("Zoom To best fit"), this);
//    zoomFit->action->setStatusTip(tr("Zoom current page to best fit"));
//    connect(zoomFit->action, SIGNAL(triggered()), this, SLOT(slotZoomFit()));
//    m_actions.append(zoomFit);

    magnetLeft = new PageAction();
    magnetLeft->action = new QAction(QIcon(":/images/magnet_left.png"), tr("Left magnet on/off"), this);
    magnetLeft->action->setCheckable(true);
    magnetLeft->action->setStatusTip(tr("Leftside magnet action on/off"));
    connect(magnetLeft->action, SIGNAL(triggered(bool)), this, SLOT(slotMagnetLeft(bool)));
    m_actions.append(magnetLeft);

    magnetTop = new PageAction();
    magnetTop->action = new QAction(QIcon(":/images/magnet_top.png"), tr("Top magnet on/off"), this);
    magnetTop->action->setCheckable(true);
    magnetTop->action->setStatusTip(tr("Topside magnet action on/off"));
    connect(magnetTop->action, SIGNAL(triggered(bool)), this, SLOT(slotMagnetTop(bool)));
    m_actions.append(magnetTop);

    magnetRight = new PageAction();
    magnetRight->action = new QAction(QIcon(":/images/magnet_right.png"), tr("Right magnet on/off"), this);
    magnetRight->action->setCheckable(true);
    magnetRight->action->setStatusTip(tr("Rightside magnet action onn/off"));
    connect(magnetRight->action, SIGNAL(triggered(bool)), this, SLOT(slotMagnetRight(bool)));
    m_actions.append(magnetRight);

    magnetBottom = new PageAction();
    magnetBottom->action = new QAction(QIcon(":/images/magnet_bottom.png"), tr("Bottom magnet on/off"), this);
    magnetBottom->action->setCheckable(true);
    magnetBottom->action->setStatusTip(tr("Bottomside magnet action on/off"));
    connect(magnetBottom->action, SIGNAL(triggered(bool)), this, SLOT(slotMagnetBottom(bool)));
    m_actions.append(magnetBottom);

    PageAction * bringForward = new PageAction();
    bringForward->action = new QAction(QIcon(":/images/bring-forward.png"), tr("Bring Item forward"), this);
    bringForward->action->setStatusTip(tr("Raise Item to top"));
    connect(bringForward->action, SIGNAL(triggered()), this, SLOT(slotBringForward()));
    m_actions.append(bringForward);

    PageAction * bringBackward = new PageAction();
    bringBackward->action = new QAction(QIcon(":/images/bring-backward.png"), tr("Bring Item backward"), this);
    bringBackward->action->setStatusTip(tr("Push item to background"));
    connect(bringBackward->action, SIGNAL(triggered()), this, SLOT(slotBringBackward()));
    m_actions.append(bringBackward);

}


void PageManipulator::slotZoomIn()
{
    if (!m_activePage)
        return;

    m_activePage->setDpi(m_activePage->dpi() + 5);
}


void PageManipulator::slotZoomOut()
{
    if (!m_activePage)
        return;

    m_activePage->setDpi(m_activePage->dpi() - 5);
}


void PageManipulator::slotZoomOrig()
{
    if (!m_activePage)
        return;

    m_activePage->setDpi(QApplication::desktop()->physicalDpiX());
}


void PageManipulator::slotZoomFit()
{
//    QRect viewGeometry = ui->view->viewport()->geometry();
//    qreal sw, sh;

//    for (int i = 0; i < 2; ++i) {   // FIXME: dirty hack - it doesn't work precise with single iteration

//        switch (m_zoomStyle) {
//        case ZoomDPI: {
//            sw = (viewGeometry.width() * m_viewDpi) / currentSceneGeometry().width();
//            sh = (viewGeometry.height() * m_viewDpi) / currentSceneGeometry().height();
//            qreal scale = qMin(sh,sw);
//            m_viewDpi = scale;
//            showPage(m_currentPageIndex);
//            break;
//        }
//        case ZoomScale: {
//            sw = (viewGeometry.width()) / currentSceneGeometry().width();
//            sh = (viewGeometry.height()) / currentSceneGeometry().height();
//            qreal scale = qMin(sh,sw);
//            ui->view->resetMatrix();
//            ui->view->scale(scale, scale);
//            m_viewScale = scale;
//            updateZoomLabel();
//            updateScrollBars();
//            break;
//        }
//        }

//    }
}


void PageManipulator::setMagnetButtonsState()
{
    magnetLeft->action->setChecked( m_activePage->magnetValue()&Magnets::Left );
    magnetTop->action->setChecked( m_activePage->magnetValue()&Magnets::Top );
    magnetRight->action->setChecked( m_activePage->magnetValue()&Magnets::Right );
    magnetBottom->action->setChecked( m_activePage->magnetValue()&Magnets::Bottom );
}


void PageManipulator::slotMagnetTop(bool b)
{
    m_activePage->m_gui->magnets()->setTopMagnet(b);
}


void PageManipulator::slotMagnetRight(bool b)
{
    m_activePage->m_gui->magnets()->setRightMagnet(b);
}


void PageManipulator::slotMagnetBottom(bool b)
{
    m_activePage->m_gui->magnets()->setBottomMagnet(b);
}


void PageManipulator::slotMagnetLeft(bool b)
{
    m_activePage->m_gui->magnets()->setLeftMagnet(b);
}


void PageManipulator::slotBringForward()
{
    m_activePage->bringCurrentItemForward();
}


void PageManipulator::slotBringBackward()
{
    m_activePage->bringCurrentItemBackward();
}

SUIT_END_NAMESPACE
