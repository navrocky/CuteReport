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
#include "magnets.h"
#include "pagegui.h"
#include "page.h"
#include "iteminterface.h"
#include "iteminterfaceview.h"
#include "limits"
#include "scene.h"


using namespace CuteReport;


SUIT_BEGIN_NAMESPACE

Magnets::Magnets(PageGUI *pageGUI) :
    QObject(pageGUI),
    m_page(pageGUI->page())
{
    setMagnetRate(m_page->magnetRate());
    setMagnetsValue(m_page->magnetValue());

    connect(m_page, SIGNAL(magnetRateChanged(int)), this, SLOT(setMagnetRate(int)));
}


Magnets::~Magnets()
{
    qDeleteAll(m_gideLines);
}


int Magnets::magnetsValue()
{
    return m_page->magnetValue();
}


void Magnets::setMagnetsValue(int value)
{
    m_page->setMagnetValue(value);
}


int Magnets::magnetRate() const
{
    return m_page->magnetRate();
}


void Magnets::setMagnetRate(int rate)
{
    m_page->setMagnetRate(rate);
    m_magnetRangeMM = rate * m_page->gridStep();
}


qreal Magnets::magnetRangeMM() const
{
    return m_magnetRangeMM;
}


void Magnets::setLeftMagnet(bool magneted)
{
    if (magneted)
        m_page->setMagnetValue( m_page->magnetValue() | Left);
    else {
        int a = 0xffff;
        a ^= Left;
        m_page->setMagnetValue( m_page->magnetValue() & a);
    }
}


void Magnets::setRightMagnet(bool magneted)
{
    if (magneted)
        m_page->setMagnetValue( m_page->magnetValue() | Right);
    else {
        int a = 0xffff;
        a ^= Right;
        m_page->setMagnetValue( m_page->magnetValue() & a);
    }
}


void Magnets::setTopMagnet(bool magneted)
{
    if (magneted)
        m_page->setMagnetValue( m_page->magnetValue() | Top);
    else {
        int a = 0xffff;
        a ^= Top;
        m_page->setMagnetValue( m_page->magnetValue() & a);
    }
}

void Magnets::setBottomMagnet(bool magneted)
{
    if (magneted)
        m_page->setMagnetValue( m_page->magnetValue() | Bottom);
    else {
        int a = 0xffff;
        a ^= Bottom;
        m_page->setMagnetValue( m_page->magnetValue() & a);
    }
}


void Magnets::setHorizontalMagnet(bool magneted)
{
    if (magneted)
        m_page->setMagnetValue( m_page->magnetValue() | Horizontal);
    else
    {
        int a = 0xffff;
        a ^= Horizontal;
        m_page->setMagnetValue( m_page->magnetValue() & a);
    }
}

void Magnets::setVerticalMagnet(bool magneted)
{
    if (magneted)
        m_page->setMagnetValue( m_page->magnetValue() | Vertical);
    else
    {
        int a = 0xffff;
        a ^= Vertical;
        m_page->setMagnetValue( m_page->magnetValue() & a);
    }
}


void Magnets::clear()
{
    qDeleteAll(m_gideLines);
    m_gideLines.clear();
}

// TODO: speed optimization
QPointF Magnets::delta(const QList<QPointF> & pointListMM, const QList<CuteReport::BaseItemInterface*> & checkingItems, MagnetDirection direction)
{
    PageGUI * page = static_cast<PageGUI*>(parent());
    Scene * scene = page->m_scene;

    qreal nearestX = std::numeric_limits<qreal>::max();
    qreal nearestY = std::numeric_limits<qreal>::max();
    qreal nearestXtemp = std::numeric_limits<qreal>::max();
    qreal nearestYtemp = std::numeric_limits<qreal>::max();

    QGraphicsItem * tempHGuideLine = 0;
    QGraphicsItem * tempVGuideLine = 0;

    clear();

    QPen p;
    p.setWidth(3);
    p.setColor(Qt::red);
    p.setStyle(Qt::DotLine);

    foreach(BaseItemInterface * item, checkingItems)  {

        QRectF itemGeometry = item->absoluteGeometry(Millimeter);

        foreach (const QPointF & point, pointListMM) {

            nearestXtemp = std::numeric_limits<qreal>::max();
            nearestYtemp = std::numeric_limits<qreal>::max();

            if (direction == HDirection || direction == HVDirection) {
                if (m_page->magnetValue()&Left && fabs( (nearestXtemp = itemGeometry.left() - point.x()) ) <= m_magnetRangeMM && fabs(nearestXtemp) < fabs(nearestX)) {
                    nearestX = nearestXtemp;
                    QRectF r = item->view()->mapRectToScene(item->view()->boundingRect());
                    delete tempHGuideLine;
                    tempHGuideLine = scene->addLine(r.left(), r.top(), r.left(), r.bottom(), p);
                } else if (m_page->magnetValue()&Right && fabs( (nearestXtemp = itemGeometry.right() - point.x()) ) <= m_magnetRangeMM && fabs(nearestXtemp) < fabs(nearestX)) {
                    nearestX = nearestXtemp;
                    QRectF r = item->view()->mapRectToScene(item->view()->boundingRect());
                    delete tempHGuideLine;
                    tempHGuideLine = scene->addLine(r.right(), r.top(), r.right(), r.bottom(), p);
                }
            }

            if (direction == VDirection || direction == HVDirection) {
                if (m_page->magnetValue()&Top && fabs( (nearestYtemp = itemGeometry.top() - point.y()) ) <= m_magnetRangeMM && fabs(nearestYtemp) < fabs(nearestY)) {
                    nearestY = nearestYtemp;
                    QRectF r = item->view()->mapRectToScene(item->view()->boundingRect());
                    delete tempVGuideLine;
                    tempVGuideLine = scene->addLine(r.left(), r.top(), r.right(), r.top(), p);
                } else if (m_page->magnetValue()&Bottom && fabs( (nearestYtemp = itemGeometry.bottom() - point.y()) ) <= m_magnetRangeMM && nearestYtemp < nearestY) {
                    nearestY = nearestYtemp;
                    QRectF r = item->view()->mapRectToScene(item->view()->boundingRect());
                    delete tempVGuideLine;
                    tempVGuideLine = scene->addLine(r.left(), r.bottom(), r.right(), r.bottom(), p);
                }
            }
        }
    }

    m_gideLines << tempVGuideLine << tempHGuideLine;

    QPointF result( nearestX == std::numeric_limits<qreal>::max() ? 0 : nearestX,
                    nearestY == std::numeric_limits<qreal>::max() ? 0 : nearestY);

    return result;
}

SUIT_END_NAMESPACE

