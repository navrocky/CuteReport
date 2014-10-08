#include "view.h"
#include "pagegui.h"
#include "types.h"
#include "functions.h"
#include "reportcore.h"
#include "baseiteminterface.h"

#include <QGridLayout>
#include <QLabel>

using namespace CuteReport;


SUIT_BEGIN_NAMESPACE

View::View(Page *page, PageGUI *pageGui):
    m_page(page),
    m_pageGui(pageGui)
{
    setAcceptDrops(true);
}


void View::dragMoveEvent ( QDragMoveEvent * event )
{
    QString moduleName = event->mimeData()->text().section("::", 2);
    if (event->mimeData()->hasFormat("text/plain")) {
        QPointF pagePos = convertUnit( m_pageGui->m_pageItem->mapFromScene(mapToScene(event->pos())), Pixel, m_page->unit(), m_page->dpi());

        const BaseItemInterface * item = m_page->reportCore()->itemModule(moduleName);
        if (m_page->canContainAt(item, pagePos)) {
            event->acceptProposedAction();
        } else {
            event->ignore();
        }
        if (m_pageGui->m_posLabel) {
            m_pageGui->m_posLabel->setText(QString("pos(%1, %2)").arg(pagePos.x(),4,'f',2,'0').arg(pagePos.y(),4,'f',2,'0'));
            m_pageGui->m_posLabel->setMinimumSize(m_pageGui->m_posLabel->geometry().size());
        }
    } else {
        event->ignore();
    }

}


void View::dropEvent(QDropEvent *event)
{
    QString moduleName = event->mimeData()->text().section("::", 2);
    QPointF pagePos = convertUnit(m_pageGui->m_pageItem->mapFromScene(mapToScene(event->pos())), Pixel, m_page->unit(), m_page->dpi());

    if (m_page->pageRect().contains(pagePos))
        emit dropItem(moduleName, pagePos);

    event->acceptProposedAction();
}


void View::mouseMoveEvent ( QMouseEvent * event )
{
    QGraphicsView::mouseMoveEvent(event);

    QPointF pos = convertUnit(m_pageGui->m_pageItem->mapFromScene(mapToScene(event->pos())), Pixel, m_page->unit(), m_page->dpi());
    pos.setX(int(pos.x() / m_page->gridStep()) * m_page->gridStep());
    pos.setY(int(pos.y() / m_page->gridStep()) * m_page->gridStep());

    if (m_pageGui->m_posLabel) {
        m_pageGui->m_posLabel->setText(QString("pos(%1, %2)").arg(pos.x(),4,'f',2,'0').arg(pos.y(),4,'f',2,'0'));
        m_pageGui->m_posLabel->setMinimumSize(m_pageGui->m_posLabel->geometry().size());
    }
}


void View::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::CTRL) {
        int d = event->delta() > 0 ? 5 : - 5;
        m_page->setDpi(m_page->dpi() + d);
        return;
    }

    QGraphicsView::wheelEvent(event);
}


SUIT_END_NAMESPACE
