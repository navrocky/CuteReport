#include "scene.h"
#include "types.h"
#include "pagegui.h"
#include "functions.h"
#include "reportcore.h"
#include "baseiteminterface.h"
#include "magnets.h"

#include <QEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QDebug>
#include <QLabel>

using namespace CuteReport;


SUIT_BEGIN_NAMESPACE

Scene::Scene(Page *page, PageGUI *parent) :
    QGraphicsScene(parent)
  ,m_page(page)
  ,m_pageGui(parent)
  ,m_ignoreMouse(false)
{
}


bool Scene::event(QEvent *event)
{
    if (event->type() == PageMouseBlock) {
        qDebug() << "PageMouseBlock";
        m_ignoreMouse = true;
    } else if (event->type() == PageMouseUnblock) {
        qDebug() << "PageMouseBlock";
        m_ignoreMouse = false;
    }
    return QGraphicsScene::event(event);
}


void Scene::mousePressEvent (QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
    qDebug() << "mousePressEvent" << event->isAccepted() << event->pos();

    if (!m_ignoreMouse && !event->isAccepted())
        emit mousePressed(event);
}


void Scene::mouseReleaseEvent (QGraphicsSceneMouseEvent * event)
{
    QGraphicsScene::mouseReleaseEvent(event);
    qDebug() << "mouseReleaseEvent" << event->isAccepted() << event->pos();

    m_pageGui->magnets()->clear();

    if (!m_ignoreMouse && !event->isAccepted())
        emit mouseReleased(event);
}



void Scene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
    QGraphicsScene::mouseDoubleClickEvent(event);
    qDebug() << "mouseDoubleClickEvent" << event->isAccepted();

    if (!m_ignoreMouse && !event->isAccepted())
        emit mouseDoubleClicked(event);
}


void Scene::keyPressEvent ( QKeyEvent * event )
{
    event->ignore();
    QGraphicsScene::keyPressEvent(event);
    if (event->isAccepted())
        return;

    switch (event->key()) {
        case Qt::Key_Delete: {
                BaseItemInterface * item = m_pageGui->currentItem();
                if (item) {
                    m_page->deleteItem(item);
                }
            }
            break;
        case Qt::Key_Left: {
                BaseItemInterface * item = m_pageGui->currentItem();
                if (item && !(item->resizeFlags()&CuteReport::BaseItemInterface::FixedPos)) {
                    QRectF rect= item->geometry().translated(-m_pageGui->page()->gridStep(),0);
                    m_pageGui->setItemAjustedAbsoluteGeometryMM(item, rect);
                }
            }
            break;
        case Qt::Key_Right: {
                BaseItemInterface * item = m_pageGui->currentItem();
                if (item && !(item->resizeFlags()&CuteReport::BaseItemInterface::FixedPos)) {
                    QRectF rect= item->geometry().translated(m_pageGui->page()->gridStep(), 0);
                    m_pageGui->setItemAjustedAbsoluteGeometryMM(item, rect);
                }
            }
            break;
        case Qt::Key_Up: {
                BaseItemInterface * item = m_pageGui->currentItem();
                if (item && !(item->resizeFlags()&CuteReport::BaseItemInterface::FixedPos)) {
                    QRectF rect= item->geometry().translated(0, -m_pageGui->page()->gridStep());
                    m_pageGui->setItemAjustedAbsoluteGeometryMM(item, rect);
                }
            }
            break;
        case Qt::Key_Down: {
                BaseItemInterface * item = m_pageGui->currentItem();
                if (item && !(item->resizeFlags()&CuteReport::BaseItemInterface::FixedPos)) {
                    QRectF rect= item->geometry().translated(0, m_pageGui->page()->gridStep());
                    m_pageGui->setItemAjustedAbsoluteGeometryMM(item, rect);
                }
            }
            break;
    }

}

SUIT_END_NAMESPACE
