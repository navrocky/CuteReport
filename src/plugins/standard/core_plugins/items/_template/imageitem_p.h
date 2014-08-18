#ifndef IMAGEITEM_P_H
#define IMAGEITEM_P_H

#include "iteminterface_p.h"

using namespace Report;

class ImageItemPrivate : public ItemInterfacePrivate
{
    ImageItemPrivate(): ItemInterfacePrivate(){}
    virtual ~ImageItemPrivate(){}

};

QDataStream &operator<<(QDataStream &s, const ImageItemPrivate &p);
QDataStream &operator>>(QDataStream &s, ImageItemPrivate &p);

#endif // IMAGEITEM_P_H
