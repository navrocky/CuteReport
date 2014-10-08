#ifndef VIEW_H
#define VIEW_H

#include "page.h"

#include <QGraphicsView>

SUIT_BEGIN_NAMESPACE

class View : public QGraphicsView
{
    Q_OBJECT
public:
    explicit View(Page * page, PageGUI * pageGui);

signals:
    void dropItem(QString classname, QPointF pos);

protected:
    virtual void dragMoveEvent ( QDragMoveEvent * event );
    virtual void dropEvent(QDropEvent *event);
    virtual void mouseMoveEvent ( QMouseEvent * event );
    virtual void wheelEvent(QWheelEvent * event);

private:
    Page * m_page;
    PageGUI * m_pageGui;
};

SUIT_END_NAMESPACE

#endif // VIEW_H
