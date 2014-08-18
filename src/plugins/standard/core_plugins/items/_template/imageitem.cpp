#include "imageitem.h"
#include "imageitem_p.h"
#include "item_common/simpleitemview.h"

using namespace Report;

ImageItem::ImageItem(PageInterface * page, ItemInterface *parentItem) :
    ItemInterface(page, parentItem)
{
}

ImageItem::ImageItem(ImageItemPrivate &dd, PageInterface * page, ItemInterface *parentItem)
    :ItemInterface(dd, page, parentItem)
{
}


Report::ItemInterface * ImageItem::clone()
{
    Q_D(ImageItem);
    return new ImageItem(*d, m_page, m_parentItem);
}


QByteArray & ImageItem::serialize()
{
    Q_D(ImageItem);
    QByteArray ba;
    QDataStream s( &ba, QIODevice::ReadWrite );
    s << *d;
    return ba;
}


void ImageItem::deserialize(QByteArray &data)
{
    Q_D(ImageItem);
    QDataStream s( &data, QIODevice::ReadWrite );
    s >> *d;
}


QDataStream &operator<<(QDataStream &s, const ImageItemPrivate &p) {
    qDebug() << "ImageItem <<";
    s << static_cast<const ItemInterfacePrivate&>(p);
//    s << p.order;
    return s;
}


QDataStream &operator>>(QDataStream &s, ImageItemPrivate &p) {
    qDebug() << "ImageItem >>";
    s >> static_cast<ItemInterfacePrivate&>(p);
//    s >> p.order; s >> p.margins; s >> p.group;
    return s;
}


Report::ItemInterface * ImageItem::createInstance(Report::PageInterface *page, Report::ItemInterface *parentItem)
{
    return new ImageItem(page, parentItem);
}


void ImageItem::initGUI()
{
    if (gui)
        return;
    gui = new SimpleItemView(this);
}


bool ImageItem::canContain(QObject * object)
{
    return (dynamic_cast<Report::ItemInterface*>(object));
}


QIcon ImageItem::itemIcon()
{
    return QIcon(":/image.png");
}


QString ImageItem::itemName()
{
    return tr("Image");
}


QString ImageItem::itemGroup()
{
    return tr("Shapes");
}




Report::RenderedItemInterface * ImageItem::render(int customDPI)
{
    return 0;
}



Q_EXPORT_PLUGIN2(image, ImageItem)
