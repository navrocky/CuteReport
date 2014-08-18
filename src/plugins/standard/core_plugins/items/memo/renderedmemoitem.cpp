#include "renderedmemoitem.h"
#include "memo.h"
#include "memo_p.h"
#include "rendererpublicinterface.h"


RenderedMemoItem::RenderedMemoItem(CuteReport::BaseItemInterface * item, CuteReport::BaseItemInterfacePrivate *itemPrivateData)
    :RenderedItemInterface(item, itemPrivateData)
{
    MemoItemPrivate * const d = static_cast<MemoItemPrivate*>(itemPrivateData);
    MemoItem* memo = reinterpret_cast<MemoItem*>(item);
    d->text = memo->m_renderer->processString(d->text, d->delimiters.section(",",0,0).simplified(), d->delimiters.section(",",1,1).simplified(), memo);
    //MemoItem::adjust(d, memo->m_renderer->currentBandDelta());
}


void RenderedMemoItem::paint(QPainter * painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    MemoItem::paint(painter, option, d_ptr, boundingRect(), CuteReport::RenderingReport);
}


void RenderedMemoItem::redraw(bool withChildren)
{
    MemoItem::adjust(reinterpret_cast<MemoItemPrivate*>(d_ptr), QPointF());
    RenderedItemInterface::redraw(withChildren);
}
