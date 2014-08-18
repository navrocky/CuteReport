#ifndef RENDEREDMEMOITEM_H
#define RENDEREDMEMOITEM_H

#include "renderediteminterface.h"

class  RenderedMemoItem : public CuteReport::RenderedItemInterface
{
public:
    explicit RenderedMemoItem(CuteReport::BaseItemInterface * item, CuteReport::BaseItemInterfacePrivate *itemPrivateData);

    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual void redraw(bool withChildren = true);
};

#endif // RENDEREDMEMOITEM_H
