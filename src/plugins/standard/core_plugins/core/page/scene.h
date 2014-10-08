#ifndef SCENE_H
#define SCENE_H

#include "plugins_common.h"

#include <QGraphicsScene>

SUIT_BEGIN_NAMESPACE
class PageGUI;
class Page;
class Scene;
SUIT_END_NAMESPACE

USING_SUIT_NAMESPACE

SUIT_BEGIN_NAMESPACE

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(Page * page, PageGUI *parent = 0);

signals:
    void dropItem(QString classname, QPointF pos);
    void mouseReleased(QGraphicsSceneMouseEvent * event);
    void mousePressed (QGraphicsSceneMouseEvent * event);
    void mouseDoubleClicked (QGraphicsSceneMouseEvent * event);

protected:
    virtual bool event(QEvent * event);
    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
    virtual void mouseReleaseEvent (QGraphicsSceneMouseEvent *event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    virtual void keyPressEvent ( QKeyEvent * event );

private:
    Page * m_page;
    PageGUI * m_pageGui;
    bool m_ignoreMouse;
};

SUIT_END_NAMESPACE

#endif // SCENE_H
