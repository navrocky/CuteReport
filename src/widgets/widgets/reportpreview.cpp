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
#include "reportpreview.h"
#include "ui_reportpreview.h"
#include "reportcore.h"
#include "reportinterface.h"
#include "rendererinterface.h"
#include "pageinterface.h"
#include "stdstoragedialog.h"
#include "exportinterface.h"
#include "renderedreportinterface.h"

#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <QProgressDialog>
#include <QGraphicsDropShadowEffect>

#define MODULENAME "ReportPreview"

inline void initMyResource() { Q_INIT_RESOURCE(reportpreview); }

namespace CuteReport
{


ReportPreview::ReportPreview(QWidget * parent, PreviewFlags f) :
    QWidget(parent),
    ui(new Ui::ReportPreview),
    m_reportCore(0)
{
    initMe(f);
    setShowFlags(f);
}

ReportPreview::ReportPreview(CuteReport::ReportCore *core, QWidget *parent, PreviewFlags f) :
    QWidget(parent),
    ui(new Ui::ReportPreview),
    m_reportCore(core)
{
    initMe(f);
}


ReportPreview::~ReportPreview()
{
    clearPage();
    delete ui;
}


void ReportPreview::initMe(PreviewFlags f)
{
    initMyResource();
    ui->setupUi(this);

    setShowFlags(f);
    m_cancelled = false;
    m_zoomStyle = ZoomDPI; //use ZoomDPI because it draws correct border thickness

    m_viewScale = 1.0;
    m_viewDpi = 0;
    m_origDpi = 0;
    m_progressDialog = 0,
            m_pageEffect = 0;
    m_currentPageIndex = -1;
    m_reportCoreInited = false;

    m_scene = new QGraphicsScene(this);
    m_scene->setBackgroundBrush(QBrush(QColor(Qt::gray)));
    ui->view->setScene(m_scene);

    ui->hScrollBar->setVisible(false);
    ui->vScrollBar->setVisible(false);

    ui->zoomPercent->setEditText("100%");

    connectCore();

    connect(ui->bRun, SIGNAL(clicked()), this, SLOT(run()));
    connect(ui->buttonNext, SIGNAL(clicked()), this, SLOT(slotPageNext()));
    connect(ui->buttonPrevious, SIGNAL(clicked()), this, SLOT(slotPagePrevious()));
    connect(ui->buttonFirst, SIGNAL(clicked()), this, SLOT(slotPageFirst()));
    connect(ui->buttonLast, SIGNAL(clicked()), this, SLOT(slotPageLast()));
    connect(ui->print, SIGNAL(clicked()), this, SLOT(slotPrint()));
    connect(ui->buttonFitWidth, SIGNAL(clicked()), this, SLOT(slotFitWidth()));
    connect(ui->buttonFitPage, SIGNAL(clicked()), this, SLOT(slotFitPage()));
    connect(ui->zoomPercent->lineEdit(), SIGNAL(textChanged(QString)), this, SLOT(slotZoomEdited(QString)));
    connect(ui->zoomPercent->lineEdit(), SIGNAL(cursorPositionChanged(int,int)), this, SLOT(slotZoomCursorPositionChanged(int,int)));
    connect(ui->zoomPercent->lineEdit(), SIGNAL(editingFinished()), this, SLOT(slotZoomChanged()));
    connect(ui->zoomPercent, SIGNAL(currentIndexChanged(int)), this, SLOT(slotZoomChanged()));
    connect(ui->buttonZoomOut, SIGNAL(clicked()), this, SLOT(slotZoomOut()));
    connect(ui->buttonZoomIn, SIGNAL(clicked()), this, SLOT(slotZoomIn()));
    connect(ui->buttonZoomOrig, SIGNAL(clicked()), this, SLOT(slotZoomOrig()));
    connect(ui->buttonExport, SIGNAL(clicked()), this, SLOT(slotExport()));

    connect(ui->view->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(viewVerticalScrollBarChanged(int)));
    connect(ui->view->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(viewHorizontalScrollBarChanged(int)));
    connect(ui->vScrollBar, SIGNAL(sliderMoved(int)), ui->view->verticalScrollBar(), SLOT(setValue(int)));
    connect(ui->hScrollBar, SIGNAL(sliderMoved(int)), ui->view->horizontalScrollBar(), SLOT(setValue(int)));

    setEnabled(false);
}


void ReportPreview::connectCore()
{
    if (m_reportCore && !m_reportCoreInited) {
        connect(m_reportCore, SIGNAL(rendererStarted(CuteReport::ReportInterface*)), this, SLOT(slotRenderingStarted(CuteReport::ReportInterface*)));
        connect(m_reportCore, SIGNAL(rendererDone(CuteReport::ReportInterface*,bool)), this, SLOT(slotRendererDone(CuteReport::ReportInterface*,bool)));
        connect(m_reportCore, SIGNAL(rendererProcessingPage(CuteReport::ReportInterface*,int,int)), this, SLOT(slotRendererProcessingPage(CuteReport::ReportInterface*,int,int)));
        m_reportCoreInited = true;
    }
}

void ReportPreview::setShowFlags(PreviewFlags f)
{
    ui->bRun->setVisible(f & ShowRun);
    ui->buttonFitWidth->setVisible(f & ShowFit && !(f & ShowFitted));
    ui->buttonFitPage->setVisible(f & ShowFit && !(f & ShowFitted));
    ui->zoomPercent->setVisible(f & ShowFit || f & ShowZoom);
    ui->buttonZoomIn->setVisible(f & ShowZoom && !(f & ShowFitted));
    ui->buttonZoomOut->setVisible(f & ShowZoom && !(f & ShowFitted));
    ui->buttonZoomOrig->setVisible(f & ShowZoom && !(f & ShowFitted));
    ui->buttonFirst->setVisible(f & ShowNavigation);
    ui->buttonPrevious->setVisible(f & ShowNavigation);
    ui->pageNumber->setVisible(f & ShowNavigation);
    ui->totalPages->setVisible(f & ShowNavigation);
    ui->buttonNext->setVisible(f & ShowNavigation);
    ui->buttonLast->setVisible(f & ShowNavigation);
    ui->print->setVisible(f & ShowPrint);

    ui->zoomPercent->setEnabled(!(f & ShowFitted));

    ui->line->setVisible(f & ShowRun);

    m_flags = f;
}


void ReportPreview::connectReport(ReportInterface * report)
{
    m_report = report;
    setEnabled(m_report);
    connectCore();
}


void ReportPreview::run()
{
    if (!m_report || !m_reportCore)
        return;

    m_cancelled = false;
    m_reportCore->log(CuteReport::LogDebug, MODULENAME, "run");
    m_reportCore->render(m_report);
}


void ReportPreview::clear()
{
    clearPage();
    m_currentPageIndex = -1;
}


void ReportPreview::updateView()
{
    ui->view->centerOn( 0, 0 );
    updateZoomLabel();
    updateScrollBars();
}


void ReportPreview::resetView()
{
    setPageCounter(0,0);
    m_viewScale = 1.0;
    m_viewDpi = m_origDpi;
    updateZoomLabel();
}


void ReportPreview::setPageCounter(int current, int total)
{
    ui->pageNumber->setText(QString::number(current));
    ui->totalPages->setText("/" +QString::number(total));
}


void ReportPreview::slotFitPage()
{
    if (m_currentPageIndex < 0)
        return;

    ui->hScrollBar->setVisible(false);
    ui->vScrollBar->setVisible(false);

    QRect viewGeometry = ui->view->viewport()->geometry();
    qreal sw, sh;

    for (int i = 0; i < 2; ++i) {   // FIXME: dirty hack - it doesn't work precise with single iteration

        switch (m_zoomStyle) {
        case ZoomDPI: {
            sw = (viewGeometry.width() * m_viewDpi) / currentSceneGeometry().width();
            sh = (viewGeometry.height() * m_viewDpi) / currentSceneGeometry().height();
            qreal scale = qMin(sh,sw);
            m_viewDpi = scale;
            showPage(m_currentPageIndex);
            break;
        }
        case ZoomScale: {
            sw = (viewGeometry.width()) / currentSceneGeometry().width();
            sh = (viewGeometry.height()) / currentSceneGeometry().height();
            qreal scale = qMin(sh,sw);
            ui->view->resetMatrix();
            ui->view->scale(scale, scale);
            m_viewScale = scale;
            updateZoomLabel();
            updateScrollBars();
            break;
        }
        }

    }

}


void ReportPreview::slotFitWidth()
{
    if (m_currentPageIndex < 0)
        return;

    QRect viewGeometry = ui->view->viewport()->geometry();
    qreal sw0 = viewGeometry.width() / currentSceneGeometry().width();
    qreal sh0 = viewGeometry.height() / currentSceneGeometry().height();

    bool vScrollBarVisible = sw0 > sh0;
    ui->hScrollBar->setVisible(false);
    ui->vScrollBar->setVisible(vScrollBarVisible);

    qreal sw = (ui->view->viewport()->geometry().width() /*- scrollBarSize*/) / currentSceneGeometry().width();
    switch (m_zoomStyle) {
    case ZoomDPI:
        m_viewDpi = m_viewDpi * sw;
        showPage(m_currentPageIndex);
        break;
    case ZoomScale:
        ui->view->resetMatrix();
        ui->view->scale(sw, sw);
        m_viewScale = sw;
        updateScrollBars();
        updateZoomLabel();
        break;
    }
}


void ReportPreview::slotZoomEdited(QString text)
{
    if (m_currentPageIndex < 0)
        return;

    int cursorPos = ui->zoomPercent->lineEdit()->cursorPosition();
    if (text.at(text.count()-1) != '%') {
        ui->zoomPercent->lineEdit()->setText(text + "%");
        ui->zoomPercent->lineEdit()->setCursorPosition(cursorPos);
    }
}


void ReportPreview::slotZoomCursorPositionChanged(int oldPos, int newPos)
{
    Q_UNUSED(oldPos)

    QString text = ui->zoomPercent->lineEdit()->text();
    if (text.at(text.count()-1) == '%') {
        if (newPos > text.count()-2){
            QString selectedText =  ui->zoomPercent->lineEdit()->selectedText();
            ui->zoomPercent->lineEdit()->setCursorPosition(text.count() - 1);
            if (!selectedText.isEmpty()) {
                if (selectedText.at(selectedText.count()-1) == '%')
                    selectedText.remove(selectedText.count()-1,1);
                QRegExp rx(selectedText);
                int pos=0;
                if (rx.indexIn(text, pos) != -1) {
                    ui->zoomPercent->lineEdit()->setSelection ( pos, selectedText.size());
                }
            }
        }
    }
}


void ReportPreview::slotZoomChanged()
{
    if (m_currentPageIndex < 0)
        return;

    QString text = ui->zoomPercent->lineEdit()->text();
    if (text.at(text.count()-1) == '%') {
        switch (m_zoomStyle) {
        case ZoomDPI:
            m_viewDpi = text.remove(text.count()-1,1).toDouble() / 100.0 * m_origDpi;
            showPage(m_currentPageIndex);
            break;
        case ZoomScale:
            ui->view->resetMatrix();
            m_viewScale = text.remove(text.count()-1,1).toInt()/100.0;
            if (m_viewScale != 0)
                ui->view->scale(m_viewScale, m_viewScale);
            updateScrollBars();
            break;
        }
    }
}


void ReportPreview::slotZoomIn()
{
    if (m_currentPageIndex < 0)
        return;

    switch (m_zoomStyle) {
    case ZoomDPI:
        m_viewDpi += 5;
        showPage(m_currentPageIndex);
        break;
    case ZoomScale:
        m_viewScale += 0.1;
        if (m_viewScale > 5.0)
            m_viewScale = 5.0;

        ui->view->resetMatrix();
        ui->view->scale(m_viewScale, m_viewScale);
        updateScrollBars();
        updateZoomLabel();
    }
}


void ReportPreview::slotZoomOut()
{
    if (m_currentPageIndex < 0)
        return;

    switch (m_zoomStyle) {
    case ZoomDPI:
        m_viewDpi -= 5;
        if (m_viewDpi < 5) m_viewDpi = 5;
        showPage(m_currentPageIndex);
        break;
    case ZoomScale:
        m_viewScale -= 0.1;
        if (m_viewScale < 0.2) m_viewScale = 0.2;
        ui->view->resetMatrix();
        ui->view->scale(m_viewScale, m_viewScale);
        updateScrollBars();
        updateZoomLabel();
        break;
    }
}


void ReportPreview::slotZoomOrig()
{
    if (m_currentPageIndex < 0)
        return;

    switch (m_zoomStyle) {
    case ZoomDPI:
        m_viewDpi = m_origDpi;
        showPage(m_currentPageIndex);
        break;
    case ZoomScale:
        m_viewScale = 1.0;
        ui->view->resetMatrix();
        updateScrollBars();
        updateZoomLabel();
        break;
    }
}


void ReportPreview::updateScrollBars()
{
    QRect viewGeometry = ui->view->viewport()->geometry();

    switch (m_zoomStyle) {
    case ZoomDPI:
        ui->hScrollBar->setVisible( currentSceneGeometry().width()  > viewGeometry.width() );
        ui->vScrollBar->setVisible( currentSceneGeometry().height()  > viewGeometry.height() );
        break;
    case ZoomScale:
        ui->hScrollBar->setVisible( currentSceneGeometry().width() * m_viewScale > viewGeometry.width() );
        ui->vScrollBar->setVisible( currentSceneGeometry().height() * m_viewScale > viewGeometry.height() );
        break;
    }

    if (ui->hScrollBar->isVisible()) {
        ui->hScrollBar->setRange(ui->view->horizontalScrollBar()->minimum(), ui->view->horizontalScrollBar()->maximum());
        ui->hScrollBar->setSingleStep(ui->view->horizontalScrollBar()->singleStep());
        ui->hScrollBar->setPageStep(ui->view->horizontalScrollBar()->pageStep());
    }
    if (ui->vScrollBar->isVisible()) {
        ui->vScrollBar->setRange(ui->view->verticalScrollBar()->minimum(), ui->view->verticalScrollBar()->maximum());
        ui->vScrollBar->setSingleStep(ui->view->verticalScrollBar()->singleStep());
        ui->vScrollBar->setPageStep(ui->view->verticalScrollBar()->pageStep());
    }

    //ui->gridLayout->update();
    //ui->view->updateGeometry();
    //ui->view->update();
}


void ReportPreview::slotPageNext()
{
    if (!m_reportCore)
        return;

    int pageCount = m_reportCore->rendererTotalPages(m_report);
    if (m_currentPageIndex +1 >= pageCount)
        return;
    clearPage();
    m_currentPageIndex++;
    if (m_currentPageIndex >= pageCount)
        m_currentPageIndex = pageCount -1;

    showPage(m_currentPageIndex);
    emit pageNext();
}


void ReportPreview::slotPagePrevious()
{
    if (!m_reportCore)
        return;

    int pageCount = m_reportCore->rendererTotalPages(m_report);
    if (m_currentPageIndex > pageCount)
        return;
    clearPage();
    m_currentPageIndex--;
    if (m_currentPageIndex < 0 )
        m_currentPageIndex = 0;

    showPage(m_currentPageIndex);
    emit pagePrevious();
}


void ReportPreview::slotPageFirst()
{
    if (!m_reportCore)
        return;

    int pageCount = m_reportCore->rendererTotalPages(m_report);
    if (!pageCount)
        return;
    clearPage();
    m_currentPageIndex = 0;

    showPage(m_currentPageIndex);
    emit pageFirst();
}

void ReportPreview::slotPageLast()
{
    if (!m_reportCore)
        return;

    int pageCount = m_reportCore->rendererTotalPages(m_report);
    if (!pageCount)
        return;
    clearPage();
    m_currentPageIndex = pageCount - 1;

    showPage(m_currentPageIndex);
    emit pageLast();
}


void ReportPreview::slotPrint()
{
    if (!m_reportCore)
        return;

    if (reportCore()->rendererTotalPages(m_report) == 0) {
        int ret = QMessageBox::warning(this, tr("CuteReport"),
                                       tr("Report template is not rendered yet and cannot be printed.\n"
                                          "Do you want to render it?"),
                                       QMessageBox::Yes | QMessageBox::No,
                                       QMessageBox::Yes);
        if (ret == QMessageBox::No)
            return;

        run();
    }

    m_reportCore->print(m_report);

    emit print();
}


void ReportPreview::slotExport()
{
    if (!m_reportCore)
        return;

    if (reportCore()->rendererTotalPages(m_report) == 0) {
        int ret = QMessageBox::warning(this, tr("CuteReport"),
                                       tr("Report template is not rendered yet and cannot be exported.\n"
                                          "Do you want to render it?"),
                                       QMessageBox::Yes | QMessageBox::No,
                                       QMessageBox::Yes);
        if (ret == QMessageBox::No)
            return;

        run();
    }

    QStringList filters;
    foreach (ExportInterface * m, m_reportCore->exportModules()) {
        filters << QString("%1 (*.%2)").arg(m->moduleFullName()).arg(m->format().toLower());
    }

    StdStorageDialog d(m_reportCore, m_report, m_reportCore->rootWidget(), "Export to file");
    d.setNameFilters(filters, false);

    if( d.exec() != QDialog::Accepted)
        return;

    QString moduleName;
    QString filter = d.currentNameFilter();
    QRegExp rx("(\\w+)\\s+\\(");
    if (rx.indexIn(filter) != -1)
        moduleName = rx.cap(1).trimmed();

    ExportInterface * exportModule = 0;
    foreach (ExportInterface * m, m_reportCore->exportModules()) {
        if (m->moduleFullName().toLower() == moduleName.toLower()) {
            exportModule = m;
            break;
        }
    }

    if (!exportModule)
        return;

    if (d.currentObjectExists()) {
        int ret = QMessageBox::warning(this, tr("CuteReport"),
                                       tr("File with selected filename already exists.\n"
                                          "Do you want to overwrite it?"),
                                       QMessageBox::Yes | QMessageBox::No,
                                       QMessageBox::No);
        if (ret == QMessageBox::No)
            return;
    }

    exportModule->processWithDialog(m_report, d.currentObjectUrl());
    emit exported();
}


void ReportPreview::showProgressDialog(const QString & labelText, const QString & cancelButtonText, int minimum, int maximum)
{
    Q_UNUSED(minimum)
    Q_UNUSED(maximum)

    hideProgressDialog();
    m_progressDialog = new QProgressDialog(labelText, cancelButtonText, 0, 100, m_reportCore->rootWidget() ? m_reportCore->rootWidget() : this);
    m_progressDialog->setWindowTitle("Rendering...");
    m_progressDialog->show();
    connect(m_progressDialog, SIGNAL(canceled()), this, SLOT(slotDialogCancelled()));
}


void ReportPreview::hideProgressDialog()
{
    if(m_progressDialog) {
        delete m_progressDialog;
        m_progressDialog = 0;
    }
}


void ReportPreview::updateProgressDialog(const QString & labelText, int value, int maximum)
{
    if (m_progressDialog && !m_cancelled){
        m_progressDialog->show();
        m_progressDialog->setValue(value);
        m_progressDialog->setMaximum(maximum);
        m_progressDialog->setLabelText(labelText);
    }
}


void ReportPreview::slotRenderingStarted(CuteReport::ReportInterface * report)
{
    if (m_report && report != m_report)
        return;

    m_cancelled = false;
    setPageCounter(0, 0);
    showProgressDialog("", "Cancel", 0, 100);
    clear();
}


void ReportPreview::slotRendererDone(CuteReport::ReportInterface *report, bool)
{
    if (m_report && report != m_report)
        return;

    int pageCount = m_reportCore->rendererTotalPages(m_report);
    m_origDpi = report->renderedReport()->dpi();
    m_viewDpi = m_origDpi;

    if (pageCount)
        m_currentPageIndex = 0;

    hideProgressDialog();
    showPage(m_currentPageIndex); //first view init
    slotFitPage();
}


void ReportPreview::slotRendererProcessingPage(CuteReport::ReportInterface *report,int value, int total)
{
    if (m_report && report != m_report)
        return;

    if (total > 0)
        updateProgressDialog(QString("Processing page %1 of %2").arg(value).arg(total), value, total);
    else
        updateProgressDialog(QString("Processing page %1").arg(value), value, total);
}


void ReportPreview::slotDialogCancelled()
{
    m_reportCore->log(CuteReport::LogDebug, MODULENAME, "stopRender()");
    m_cancelled = true;
    m_reportCore->stopRendering(m_report.data());
}


void ReportPreview::clearPage()
{
    if (m_currentPageIndex < 0)
        return;

    deletePageEffect();
    // cleaning scene up without deletion
    foreach (QGraphicsItem * item, m_scene->items()) {
        if (!item->parentItem()) {
            m_scene->removeItem(item);
        }
    }
}


void ReportPreview::showPage(int index)
{
    int pageCount = m_reportCore->rendererTotalPages(m_report);

    if (index >= pageCount || index<0)
        return;

    switch (m_zoomStyle) {
    case ZoomDPI:
        clearPage();
        break;
    case ZoomScale:
        break;
    }

    CuteReport::RenderedPageInterface * page = m_reportCore->rendererGetPage(m_report, index);
    m_scene->addItem(page);

    switch (m_zoomStyle) {
    case ZoomDPI:
        if (m_viewDpi > 0 && page->dpi() != m_viewDpi) {
            page->setDpi(m_viewDpi);
        }
        break;
    case ZoomScale:
        break;
    }

    page->redraw();

    int border = (m_flags & ShowBorderless) ? 0 : 20;
    page->setPos(border, border);
    m_scene->setSceneRect(0,0, page->boundingRect().width() + border*2, page->boundingRect().height() + border*2);
    setPageEffect(page);

    setPageCounter(m_currentPageIndex + 1, pageCount);
    updateView();
}


void ReportPreview::setPageEffect(CuteReport::RenderedPageInterface * page)
{
    Q_UNUSED(page)

    //    if (!m_pageEffect) {
    //        m_pageEffect = new QGraphicsDropShadowEffect(this);
    //        m_pageEffect->setOffset(10,10);
    //        m_pageEffect->setBlurRadius(10);
    //    }
    //    page->setGraphicsEffect(m_pageEffect);
}


void ReportPreview::deletePageEffect()
{
    if (m_pageEffect)
        delete m_pageEffect;
}


QRectF ReportPreview::currentPageGeometry()
{
    CuteReport::RenderedPageInterface * page = m_reportCore->rendererGetPage(m_report, m_currentPageIndex);
    return page ? QRectF(page->pos(), page->boundingRect().size()) : QRectF();
}


QRectF ReportPreview::currentSceneGeometry()
{
    return m_scene->sceneRect();
}


void ReportPreview::resizeEvent ( QResizeEvent * event )
{
    QWidget::resizeEvent(event);
    if (m_flags & ShowFitted)
        slotFitPage();
    else
        updateScrollBars();
}


void ReportPreview::closeEvent(QCloseEvent *event)
{
    emit closed();
    QWidget::closeEvent(event);
}


void ReportPreview::updateZoomLabel()
{
    switch (m_zoomStyle) {
    case ZoomDPI:
        if (m_origDpi != 0)
            ui->zoomPercent->lineEdit()->setText(QString("%1%").arg(int((qreal)m_viewDpi/(qreal)m_origDpi*100)));
        else
            ui->zoomPercent->lineEdit()->setText(QString("0%"));
        break;
    case ZoomScale:
        ui->zoomPercent->lineEdit()->setText(QString("%1%").arg(int(m_viewScale*100)));
        break;
    }
}


void ReportPreview::viewVerticalScrollBarChanged(int value)
{
    ui->vScrollBar->blockSignals(true);
    ui->vScrollBar->setValue(value);
    ui->vScrollBar->blockSignals(false);
}


void ReportPreview::viewHorizontalScrollBarChanged(int value)
{
    ui->hScrollBar->blockSignals(true);
    ui->hScrollBar->setValue(value);
    ui->hScrollBar->blockSignals(false);
}


} // namespace
