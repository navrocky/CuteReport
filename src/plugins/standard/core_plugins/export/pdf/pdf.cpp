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
#include "pdf.h"
#include "reportcore.h"
#include "pageinterface.h"
#include <QtCore>
#include <QPrinter>
#include <QGraphicsScene>
#include "objectfactory.h"

using namespace CuteReport;

ExportPDF::ExportPDF(QObject *parent) :
    ExportInterface(parent)
{
}


ExportPDF::ExportPDF(const ExportPDF &dd, QObject *parent)
    : ExportInterface(dd, parent)
    ,m_lastError(dd.m_lastError)
{

}


ExportPDF::~ExportPDF()
{

}

QIcon ExportPDF::icon() const
{
    return QIcon(":/pdf_icon.png");
}


ExportInterface *ExportPDF::clone() const
{
    return new ExportPDF(*this, parent());
}


ExportInterface * ExportPDF::createInstance( QObject * parent) const
{
    return new ExportPDF(parent);
}


ExportHelperInterface *ExportPDF::helper()
{
    return 0;
}


QString ExportPDF::lastError()
{
    return m_lastError;
}


bool ExportPDF::process(CuteReport::ReportInterface * report, const QString & outputUrl)
{
    return process_(report, outputUrl, false);
}


bool ExportPDF::processWithDialog(CuteReport::ReportInterface * report, const QString & outputUrl)
{
    return process_(report, outputUrl, true);
}


bool ExportPDF::process_(CuteReport::ReportInterface * report, const QString & outputUrl, bool forceShowDialog)
{
    Q_UNUSED(forceShowDialog)
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

    printer.setOrientation((QPrinter::Orientation)page->orientation());
    printer.setDocName(report->name().isEmpty() ? report->objectName() : report->name());
    printer.setFullPage(true);
    printer.setPageMargins(0,0,0,0,QPrinter::Millimeter);
    printer.setPaperSize(page->paperSize(), (QPrinter::Unit)page->unit());

    printer.setOutputFileName(reportCore()->localCachedFileName(outputUrl, report));
    printer.setOutputFormat(QPrinter::PdfFormat);

    int firstPage = 0;
    int lastPage = totalPageNumber -1;

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

        qreal sw = 1.0;
        qreal sh = 1.0;
        QSizeF paperSizePixels = printer.paperSize(QPrinter::DevicePixel);

        sw = paperSizePixels.width() / p->boundingRect().width();
        sh = paperSizePixels.height() / p->boundingRect().height();

        qreal s = qMin(sw, sh);
        sw = sh = s;

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


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(ExportPDF, ExportPDF)
#endif
