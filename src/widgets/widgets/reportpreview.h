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
#ifndef REPORTPREVIEW_H
#define REPORTPREVIEW_H

#include "widgets_export.h"
#include <QWidget>
#include <QPointer>

namespace Ui {
class ReportPreview;
}

class QGraphicsScene;
class QGraphicsDropShadowEffect;
class QProgressDialog;
class Preview;

namespace CuteReport
{

class ReportCore;
class ReportInterface;
class RenderedPageInterface;

class WIDGET_EXPORTS ReportPreview : public QWidget
{
    Q_OBJECT
public:
    enum PreviewFlag {
        ShowNothing     = 0x0000,
        ShowPrint       = 0x0001,
        ShowZoom        = 0x0002,
        ShowFit         = 0x0004,
        ShowNavigation  = 0x0008,
        ShowRun         = 0x0010,
        ShowBorderless  = 0x0020,
        ShowFitted      = 0x0040,
        ShowAllTools    = ShowPrint | ShowZoom | ShowFit | ShowNavigation | ShowRun | ShowRun,
        ShowDefault     = ShowAllTools
    };
    Q_DECLARE_FLAGS(PreviewFlags, PreviewFlag)

    enum ZoomStyle {ZoomDPI, ZoomScale};

    explicit ReportPreview(QWidget * parent = 0, PreviewFlags f = ShowDefault);
    explicit ReportPreview(CuteReport::ReportCore *core, QWidget *parent = 0, PreviewFlags f = ShowDefault);
    ~ReportPreview();

    void setShowFlags(PreviewFlags f);
    void connectReport(ReportInterface *report);
    void clear();

    void updateView();
    void resetView();
    void setPageCounter(int current, int total);
    void setViewScale (qreal scale);

    void setReportCore(CuteReport::ReportCore *reportCore) {m_reportCore = reportCore;}
    CuteReport::ReportCore * reportCore() {return m_reportCore;}

public slots:
    void run();
    void slotFitPage();
    void slotFitWidth();

signals:
    void closed();
    void pagePrevious();
    void pageNext();
    void pageFirst();
    void pageLast();
    void print();
    void exported();
    void viewScalled(qreal);

private slots:
    void initMe(CuteReport::ReportPreview::PreviewFlags f);
    void slotZoomEdited(QString text);
    void slotZoomCursorPositionChanged(int oldPos, int newPos);
    void slotZoomChanged();
    void slotZoomIn();
    void slotZoomOut();
    void slotZoomOrig();

    void slotPageNext();
    void slotPagePrevious();
    void slotPageFirst();
    void slotPageLast();
    void slotPrint();
    void slotExport();

    void showProgressDialog(const QString & labelText, const QString & cancelButtonText, int minimum, int maximum);
    void hideProgressDialog();
    void updateProgressDialog(const QString & labelText, int value, int maximum);
    void slotRenderingStarted(CuteReport::ReportInterface * report);
    void slotRendererDone(CuteReport::ReportInterface * report, bool);
    void slotRendererProcessingPage(CuteReport::ReportInterface*report, int value, int total);
    void slotDialogCancelled();

    void viewVerticalScrollBarChanged(int value);
    void viewHorizontalScrollBarChanged(int value);


private:
    void connectCore();
    void clearPage();
    void showPage(int index);
    void setPageEffect(CuteReport::RenderedPageInterface * page);
    void deletePageEffect();
    QRectF currentPageGeometry();
    QRectF currentSceneGeometry();
    void updateScrollBars();
    void updateZoomLabel();
    virtual void resizeEvent ( QResizeEvent * event );
    virtual void closeEvent(QCloseEvent * event);

    
private:
    Ui::ReportPreview *ui;
    QGraphicsScene * m_scene;
    CuteReport::ReportCore * m_reportCore;
    QPointer<CuteReport::ReportInterface> m_report;
    qreal m_viewScale;
    int m_origDpi;
    int m_viewDpi;
    QProgressDialog * m_progressDialog;
    QPointer<QGraphicsDropShadowEffect> m_pageEffect;
    int m_currentPageIndex;
    bool m_reportCoreInited;
    PreviewFlags m_flags;
    bool m_cancelled;
    ZoomStyle m_zoomStyle;
};


} //namespace

Q_DECLARE_OPERATORS_FOR_FLAGS(CuteReport::ReportPreview::PreviewFlags)

#endif // REPORTPREVIEW_H
