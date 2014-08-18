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
#include "objectpreview.h"
#include "ui_objectpreview.h"
#include "reportcore.h"
#include "reportinterface.h"
#include "pageinterface.h"
#include <QtGui>

namespace CuteReport {


ObjectPreview::ObjectPreview(QWidget *parent):
    QWidget(parent),
    ui(new Ui::ObjectPreview),
    m_reportCore(0)
{
    initMe();
}


ObjectPreview::ObjectPreview(ReportCore *reporCore, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ObjectPreview)
{
    initMe();
    m_reportCore = reporCore;
}


ObjectPreview::~ObjectPreview()
{
    delete ui;
}


void ObjectPreview::initMe()
{
    ui->setupUi(this);

    m_resizeTimer = 0;
    m_item = 0;
    m_report = 0;
    m_reportView = 0;

    ui->view->setScene(new QGraphicsScene(this));
    ui->view->setRenderHint(QPainter::SmoothPixmapTransform, true);
}


void ObjectPreview::setImage(const QImage & image)
{
    clear();
    QGraphicsPixmapItem * item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    ui->view->scene()->addItem(item);
    m_item = item;
    ui->stackedWidget->setCurrentIndex(0);
    doResize();
}


void ObjectPreview::setImage(const QPixmap & pixmap)
{
    clear();
    QGraphicsPixmapItem * item = new QGraphicsPixmapItem(pixmap);
    ui->view->scene()->addItem(item);
    m_item = item;
    ui->stackedWidget->setCurrentIndex(0);
    doResize();
}


void ObjectPreview::setReport(const QString & url)
{
    if (!m_reportCore)
        return;
    setReport(m_reportCore->loadReport(url), true);
}


void ObjectPreview::setReport(CuteReport::ReportInterface * report, bool takeOwnership)
{
    clear();
    m_report = report;
    if (!report)
        return;


    if (takeOwnership)
        m_report->setParent(this);

    QList<CuteReport::PageInterface *> pages = m_report->pages();
    if (!pages.count()) {
        clearReport();
        return;
    }
    m_reportView = pages.at(0)->createSimpleView();
    m_reportView->setMinimumSize(20,20);
    m_reportView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_reportView->setEnabled(false);
    ui->layout->addWidget(m_reportView);
    ui->stackedWidget->setCurrentIndex(1);

    m_reportView->hide();   // preventing graphics gap
    delayedResize(0);
}


void ObjectPreview::clear()
{
    clearImage();
    clearReport();
}


void ObjectPreview::clearImage()
{
    if (m_item) {
        ui->view->scene()->removeItem(m_item);
        delete m_item;
        m_item = 0;
    }
}


void ObjectPreview::clearReport()
{
    if (m_report && m_report->parent() == this) {
        m_report->deleteLater();
        m_report = 0;
    }

    if (m_reportView) {
        delete m_reportView;
        m_reportView = 0;
    }
}

void ObjectPreview::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    delayedResize();
}


void ObjectPreview::delayedResize(int delay)
{
    if (!m_resizeTimer) {
        m_resizeTimer = new QTimer(this);
        m_resizeTimer->setSingleShot(true);
        connect (m_resizeTimer, SIGNAL(timeout()), this, SLOT(doResize()));
        m_resizeTimer->start(delay);
    }
}


void ObjectPreview::doResize()
{
    QRectF newGeometry = QRectF(QPointF(0,0), QSizeF(ui->view->viewport()->geometry().size()));
    ui->view->scene()->setSceneRect(newGeometry);

    if (m_item) {
        qreal xScale = ui->view->scene()->sceneRect().width() / m_item->boundingRect().width();
        qreal yScale = ui->view->scene()->sceneRect().height() / m_item->boundingRect().height();
        qreal resultScale = qMin(xScale, yScale);
        m_item->setScale(qMin(resultScale, 1.0));
        m_item->setPos(0,0);
        m_item->setPos(ui->view->scene()->sceneRect().center() - m_item->mapRectToScene(m_item->boundingRect()).center());
    }


    if (m_reportView) {
        m_reportView->show();
        m_reportView->fit();
    }

    if (m_resizeTimer) {
        m_resizeTimer->deleteLater();
        m_resizeTimer = 0;
    }
}

} //namespace
