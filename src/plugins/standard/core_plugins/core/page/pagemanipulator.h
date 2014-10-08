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
#ifndef PAGEMANIPULATOR_H
#define PAGEMANIPULATOR_H

#include "page.h"

class QLabel;

SUIT_BEGIN_NAMESPACE

class PageManipulator : public CuteReport::PageManipulatorInterface
{
    Q_OBJECT
public:
    explicit PageManipulator(QObject *parent = 0);
    ~PageManipulator();

    virtual void setActivePage(CuteReport::PageInterface * page);
    virtual CuteReport::PageInterface * activePage();
    virtual int pageManupilatorId();

    virtual QList<CuteReport::PageAction*> actions();
    virtual QList<QLabel*> statusBarLabels();

private slots:
    void slotZoomIn();
    void slotZoomOut();
    void slotZoomOrig();
    void slotZoomFit();

    void slotMagnetTop(bool b);
    void slotMagnetRight(bool b);
    void slotMagnetBottom(bool b);
    void slotMagnetLeft(bool b);

    void slotBringForward();
    void slotBringBackward();

private:
    void initActions();
    void setMagnetButtonsState();

private:
    Page * m_activePage;
    QList<CuteReport::PageAction*> m_actions;
    CuteReport::PageAction * magnetLeft;
    CuteReport::PageAction * magnetTop;
    CuteReport::PageAction * magnetRight;
    CuteReport::PageAction * magnetBottom;
};

SUIT_END_NAMESPACE


#endif // PAGEMANIPULATOR_H
