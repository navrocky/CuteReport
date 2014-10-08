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
 ***************************************************************************/
#include "printer.h"
#include "reportcore.h"
#include "pageinterface.h"
#include <QtCore>
#include <QPrintDialog>
#include <QPrinter>
#include <QGraphicsScene>
#include "objectfactory.h"

using namespace CuteReport;


Printer::Printer(QObject *parent) :
    PrinterInterface(parent),
    m_showDialog(true),
    m_scaleToFit(true),
    m_keepAspectRatio(false),
    m_doubleSided(false),
    m_useDuplexer(false),
    m_duplexerMargins(Margins(0,0,0,15))
{
}


Printer::Printer(const Printer &dd, QObject *parent)
    : PrinterInterface(dd, parent),
    m_showDialog(dd.m_showDialog),
    m_scaleToFit(dd.m_scaleToFit),
    m_keepAspectRatio(dd.m_keepAspectRatio),
    m_doubleSided(dd.m_doubleSided),
    m_useDuplexer(dd.m_useDuplexer),
    m_duplexerMargins(dd.m_duplexerMargins)
{
}


Printer::~Printer()
{

}


CuteReport::PrinterHelperInterface * Printer::helper()
{
    return 0;
}


PrinterInterface * Printer::createInstance( QObject * parent) const
{
    return new Printer(parent);
}


PrinterInterface *Printer::clone() const
{
    return new Printer(*this, 0);
}


QString Printer::lastError()
{
    return m_lastError;
}


bool Printer::print(CuteReport::ReportInterface * report)
{
    return process_(report, true);
}


bool Printer::process_(CuteReport::ReportInterface * report, bool forceShowDialog)
{
    if (!report)
        return false;

    int totalPageNumber = reportCore()->rendererTotalPages(report);

    if (!reportCore()->rendererTotalPages(report) || !report->pages().count()) {
        m_lastError = "There are no rendered pages";
        return false;
    }

    if (!report->pages().count()) {
        m_lastError = "There are no pages in report template";
        return false;
    }

    CuteReport::PageInterface * page = report->pages()[0];

    QPrinter printer;
    QPainter painter;

    if (!m_printerName.isEmpty())
        printer.setPrinterName(m_printerName);

    printer.setOrientation((QPrinter::Orientation)page->orientation());
    printer.setDocName(report->name().isEmpty() ? report->objectName() : report->name());
    printer.setFullPage(true);
    printer.setPageMargins(0,0,0,0,QPrinter::Millimeter);
    printer.setDoubleSidedPrinting (false);
    printer.setDuplex(QPrinter::DuplexNone);

    //TODO
    //printer.setPageMargins(m_duplexerMargins.left(), m_duplexerMargins.top(), m_duplexerMargins.right(), m_duplexerMargins.bottom(), QPrinter::Millimeter);

    int firstPage = 0;
    int lastPage = totalPageNumber -1;

    if (forceShowDialog || m_showDialog) {
        QPrintDialog printDialog(&printer);
        if (printDialog.exec() != QDialog::Accepted) {
            m_lastError = "Printing cancelled by user";
            return false;
        } else {
            if (printer.fromPage() != 0)
                firstPage = qMax (0, printer.fromPage()-1);
            if (printer.toPage() != 0)
                lastPage = qMin(printer.toPage()-1, lastPage);
        }
    }

    /** if print to file we set papersize (there is wierd behaviour with pixel page size calculation if not set)
    * generaly we use paperSize set in dialog or defaul for the current printer
    * qt 4.8.4 */
    if (!printer.outputFileName().isEmpty() && printer.outputFormat() != QPrinter::NativeFormat)
        printer.setPaperSize(page->paperSize(), (QPrinter::Unit)page->unit());
    else
        printer.setPaperSize(printer.paperSize(QPrinter::Millimeter), QPrinter::Millimeter);

    qreal left, right, top, bottom;
    printer.getPageMargins(&left, &top, &right, &bottom, QPrinter::Millimeter);
    CuteReport::Margins dialogMargins(left,top,right,bottom);

    if (firstPage > lastPage) {
        m_lastError = "First page number is bigger then last";
        return false;
    }

    painter.begin(&printer);
    QGraphicsScene scene;

    for (int i = firstPage; i <= lastPage; i++) {

        if (i > firstPage)
            printer.newPage();

        printer.setFullPage(true);
        painter.resetTransform();

        CuteReport::RenderedPageInterface * p = reportCore()->rendererGetPage(report,i);
        CuteReport::RenderedPageInterface::ShowFlags flags = p->showFlags();
        p->setShowFlags(CuteReport::RenderedPageInterface::ShowDefaultPrint);


//        qreal left, right, top, bottom;
//        printer.getPageMargins(&left, &top, &right, &bottom, QPrinter::Millimeter);

        qreal sw = 1.0;
        qreal sh = 1.0;
        QSizeF paperSizePixels;

        if (m_scaleToFit) {     // fit to current format even if rendered page has different one
            paperSizePixels = printer.paperSize(QPrinter::DevicePixel);
        } else {                // print without obey current printer page size and do as rendered page promotes
            QPrinter pr;
            pr.setPrinterName(printer.printerName());
            pr.setPaperSize(p->paperSize(CuteReport::Millimeter), QPrinter::Millimeter);
            paperSizePixels = pr.paperSize(QPrinter::DevicePixel);
        }

        sw = paperSizePixels.width() / p->boundingRect().width();
        sh = paperSizePixels.height() / p->boundingRect().height();

        if (m_keepAspectRatio) {
            qreal s = qMin(sw, sh);
            sw = sh = s;
        }

        if (sw != 1.0 || sh != 1.0)
            painter.scale(sw, sh);

        QPointF pos = p->pos();
        QGraphicsScene * oldScene = p->scene();

        p->setPos(0,0);
        scene.setSceneRect(0, 0, p->boundingRect().width(), p->boundingRect().height());
        scene.addItem(p);
        scene.render(&painter, QRectF(0, 0, p->boundingRect().width(), p->boundingRect().height()));
        scene.removeItem(p);

        /** restoring */
        p->setPos(pos);
        p->setShowFlags(flags);
        if (oldScene)
            oldScene->addItem(p);
    }

    painter.end();

    return true;
}


bool Printer::isShowDialog() const
{
    return m_showDialog;
}


void Printer::setShowDialog(bool b)
{
    if (b == m_showDialog)
        return;

    m_showDialog = b;

    emit showDialogChanged(m_showDialog);
    emit changed();
}


bool Printer::scaleToFit()
{
    return m_scaleToFit;
}


void Printer::setScaleToFit(bool b)
{
    //TODO
    return;

    if (m_scaleToFit == b)
        return;
    m_scaleToFit = b;

    emit scaleToFitChanged(m_scaleToFit);
    emit changed();
}


bool Printer::keepAspectRatio()
{
    return m_keepAspectRatio;
}


void Printer::setKeepAspectRatio(bool b)
{
    //TODO
    return;

    if (m_keepAspectRatio == b)
        return;
    m_keepAspectRatio = b;

    emit keepAspectRatioChanged(m_keepAspectRatio);
    emit changed();
}


bool Printer::doubleSided()
{
    return m_doubleSided;
}


void Printer::setDoubleSided(bool b)
{
    //TODO
    return;

    if (b == m_doubleSided)
        return;
    m_doubleSided = b;

    emit doubleSidedChanged(m_doubleSided);
    emit changed();
}


bool Printer::useDuplexer()
{
    return m_useDuplexer;
}


void Printer::setUseDuplexer(bool b)
{
    //TODO
    return;

    if (b == m_useDuplexer)
        return;
    m_useDuplexer = b;

    emit useDuplexerChanged(m_useDuplexer);
    emit changed();
}


Margins Printer::duplexerMargins()
{
    return m_duplexerMargins;
}


void Printer::setDuplexerMargins(Margins margins)
{
    //TODO
    return;

    if (margins == m_duplexerMargins)
        return;
    m_duplexerMargins = margins;

    emit duplexerMarginsChanged(m_duplexerMargins);
    emit changed();
}


QString Printer::_current_property_description() const
{
    QString propertyName = metaObject()->property(m_currentProperty).name();

    if (propertyName == "showDialog")
        return tr("Force to show printer dialog when print");
    else if (propertyName == "scaleToFit")
        return tr("Scale page to fit paper size");
    else if (propertyName == "keepAspectRatio")
        return tr("Keep aspect ratio while scalling");
    else if (propertyName == "doubleSided")
        return tr("Double side printing");
    else if (propertyName == "useDuplexerIfExists")
        return tr("Use duplexer if exists or so it by software");

    return PrinterInterface::_current_property_description();
}


// centering
//QPainter painter;
//painter.begin(&printer);
//double xscale = printer.pageRect().width()/double(myWidget->width());
//double yscale = printer.pageRect().height()/double(myWidget->height());
//double scale = qMin(xscale, yscale);
//painter.translate(printer.paperRect().x() + printer.pageRect().width()/2,
//                   printer.paperRect().y() + printer.pageRect().height()/2);
//painter.scale(scale, scale);
//painter.translate(-width()/2, -height()/2);

//myWidget->render(&painter);


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Printer, Printer)
#endif
