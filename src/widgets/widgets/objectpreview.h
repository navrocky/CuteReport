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
#ifndef OBJECTPREVIEW_H
#define OBJECTPREVIEW_H

#include "widgets_export.h"
#include <QWidget>
#include <QGraphicsView>

namespace Ui {
class ObjectPreview;
}


namespace CuteReport {


class ReportInterface;
class PageViewInterface;
class ReportCore;


class WIDGET_EXPORTS ObjectPreview : public QWidget
{
    Q_OBJECT   
public:
    explicit ObjectPreview(QWidget *parent = 0 );
    explicit ObjectPreview(CuteReport::ReportCore *reporCore, QWidget *parent = 0 );
    ~ObjectPreview();

    void setReportCore(CuteReport::ReportCore *reporCore) {m_reportCore = reporCore;}
    CuteReport::ReportCore * reportCore() {return m_reportCore;}

    void clear();
    void setImage(const QImage & image);
    void setImage(const QPixmap & image);
    void setReport(const QString & url);
    void setReport(CuteReport::ReportInterface * report, bool takeOwnership = true);

private:
    void initMe();
    void resizeEvent(QResizeEvent *event);

private slots:
    void delayedResize(int delay = 500);
    void doResize();

private:
    void clearImage();
    void clearReport();

private:
    Ui::ObjectPreview *ui;
    CuteReport::ReportCore * m_reportCore;
    QTimer * m_resizeTimer;
    QGraphicsItem * m_item;
    CuteReport::ReportInterface * m_report;
    CuteReport::PageViewInterface * m_reportView;
};


} //namespace
#endif // OBJECTPREVIEW_H
