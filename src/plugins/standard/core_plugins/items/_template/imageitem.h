#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include "iteminterface.h"

using namespace Report;

class ImageItemPrivate;

class ImageItem : public ItemInterface
{
    Q_OBJECT
    Q_INTERFACES(Report::ItemInterface)
public:
    explicit ImageItem(PageInterface * page = 0, ItemInterface *parentItem = 0);

    virtual void initGUI();

    virtual Report::ItemInterface * createInstance(Report::PageInterface *page = 0, Report::ItemInterface *parentItem = 0);
    virtual ItemInterface * clone();
    virtual QByteArray & serialize();
    virtual void deserialize(QByteArray &data);
    virtual bool canContain(QObject * object);

    QIcon itemIcon();
    QString itemName();
    QString itemGroup();

    virtual Report::RenderedItemInterface * render(int customDPI = 0);
    
signals:
    
public slots:
protected:
    ImageItem(ImageItemPrivate &dd, PageInterface * page, ItemInterface *parentItem);

private:
    Q_DECLARE_PRIVATE(ImageItem)

    
};


#endif // IMAGEITEM_H


/*
    virtual Report::ItemInterface * createInstance(Report::PageInterface *page = 0, Report::ItemInterface *parentItem = 0);
    virtual ItemInterface * clone();
    virtual QByteArray & serialize();
    virtual void deserialize(QByteArray &data);
    virtual bool canContain(QObject * object);

    Report::BandInterface::LayoutType layoutType(){ return LayoutTop;}
    int layoutPriority(){ return 75;}
    Report::BandInterface::AccomodationType accommodationType(){ return AccomodationFirstPage;}

    QIcon itemIcon();
    QString itemName();
    QString itemGroup();


    virtual Report::RenderedItemInterface * render(int customDPI = 0);
    virtual Report::RenderedItemInterface * renderNewPage(int customDPI = 0);
    */
