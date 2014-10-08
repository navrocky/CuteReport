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
 ****************************************************************************/
#ifndef PAGEINTERFACE_H
#define PAGEINTERFACE_H

#include "reportplugininterface.h"
#include "types.h"

#include <QPointF>
#include <QRectF>
#include <QMap>
#include <QIcon>
#include <QString>
#include <QGraphicsRectItem>
#include <QWidget>


//static int RenderedPageInterfaceType = QGraphicsItem::UserType + 12789;
//static int PageViewInterfaceType = QGraphicsItem::UserType + 123754;

class QGraphicsView;
class QGraphicsScene;
class QAction;
class QLabel;

namespace CuteReport
{

class BaseItemInterface;
class PageManipulatorInterface;
class ReportCore;
class RenderedPageInterface;
class PageViewInterface;


class CUTEREPORT_EXPORTS PageAction  {
public:
    QAction * action;
    QString name;
    QString parentName;
};

class CUTEREPORT_EXPORTS PageInterface : public ReportPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(CuteReport::ReportPluginInterface)

    Q_ENUMS(Unit)
    Q_ENUMS(Orientation)

    Q_PROPERTY(Orientation orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)
    Q_PROPERTY(QString format READ format WRITE setFormat/* NOTIFY formatChanged*/)
    Q_PROPERTY(QSizeF paperSize READ paperSize WRITE setPaperSize NOTIFY paperSizeChanged)
    Q_PROPERTY(QString unit READ unitStr WRITE setUnitStr NOTIFY unitChanged)
    Q_PROPERTY(int dpi READ dpi WRITE setDpi NOTIFY dpiChanged)
    Q_PROPERTY(qreal marginLeft READ marginLeft WRITE setMarginLeft NOTIFY marginLeftChanged)
    Q_PROPERTY(qreal marginTop READ marginTop WRITE setMarginTop NOTIFY marginTopChanged)
    Q_PROPERTY(qreal marginRight READ marginRight WRITE setMarginRight NOTIFY marginRightChanged)
    Q_PROPERTY(qreal marginBottom READ marginBottom WRITE setMarginBottom NOTIFY marginBottomChanged)
    Q_PROPERTY(QColor background READ background WRITE setBackground NOTIFY backgroundChanged)

    Q_PROPERTY(int _current_property READ _currentProperty WRITE _setCurrentProperty DESIGNABLE false)
    Q_PROPERTY(QString _current_property_description READ _current_property_description DESIGNABLE false)
    Q_PROPERTY(int _current_property_precision READ _current_property_precision DESIGNABLE false)

public:
    enum Orientation { Portrait = 0, Landscape = 1 };

    explicit PageInterface(QObject *parent = 0);
    virtual ~PageInterface();

    virtual QIcon icon() = 0;

    virtual void init(){} // work after restoring
    
    virtual QList <BaseItemInterface*> items() const = 0;
    virtual BaseItemInterface* item(const QString & objectName) const = 0;

    virtual QWidget * createHelper() = 0;
    virtual PageInterface * createInstance( QObject * parent=0) const = 0;
    virtual PageInterface * clone(bool withChildren = true, bool init = true) const;

    virtual bool canContain(const QObject * object) = 0;

    // properties
    virtual Orientation orientation() const = 0;
    virtual void setOrientation(const Orientation & orientation) = 0;
    virtual QString format() const =0;
    virtual void setFormat(const QString & formatName)=0;
    virtual QSizeF paperSize(Unit unit = UnitNotDefined) const = 0; // if NotDefined = use current page format
    virtual void setPaperSize(const QSizeF & size, CuteReport::Unit unit = CuteReport::UnitNotDefined) = 0;   // for custom formats
    virtual QRectF pageRect(CuteReport::Unit unit = UnitNotDefined) = 0;
    virtual qreal marginLeft(CuteReport::Unit unit = CuteReport::UnitNotDefined) const = 0;
    virtual qreal marginTop(CuteReport::Unit unit = CuteReport::UnitNotDefined) const = 0;
    virtual qreal marginRight(CuteReport::Unit unit = CuteReport::UnitNotDefined) const = 0;
    virtual qreal marginBottom(CuteReport::Unit unit = CuteReport::UnitNotDefined) const = 0;
    virtual void setMarginLeft(qreal margin, CuteReport::Unit unit = CuteReport::UnitNotDefined) = 0;
    virtual void setMarginTop(qreal margin, CuteReport::Unit unit = CuteReport::UnitNotDefined) = 0;
    virtual void setMarginRight(qreal margin, CuteReport::Unit unit = CuteReport::UnitNotDefined) = 0;
    virtual void setMarginBottom(qreal margin, CuteReport::Unit unit = CuteReport::UnitNotDefined) = 0;
//    virtual CuteReport::Margins margins(CuteReport::Unit unit = CuteReport::UnitNotDefined) const = 0;
    Q_SCRIPTABLE virtual void setMargins(qreal left, qreal top, qreal right, qreal bottom, CuteReport::Unit unit = CuteReport::UnitNotDefined) = 0;
    virtual CuteReport::Unit unit() const = 0;
    virtual void setUnit(const CuteReport::Unit &) = 0;
    virtual QString unitStr() const = 0;
    virtual void setUnitStr(const QString &) = 0;
    virtual int dpi() const = 0;
    virtual void setDpi(int dpi) = 0;
    virtual QColor background() const = 0;
    virtual void setBackground(const QColor & background) = 0;

    virtual bool addItem(BaseItemInterface * item, QPointF pagePos, QString * error = 0) = 0;
    virtual bool addItem(BaseItemInterface * item) = 0;
    virtual bool addItem(const QString & className, QPointF pagePos, QString * error = 0) = 0;
    virtual void deleteItem(BaseItemInterface * item) = 0;
    virtual void deleteItem(const QString & itemName) = 0;

    /// Renderer
    virtual void renderInit(){}
    virtual CuteReport::RenderedPageInterface * render(int customDPI = 0) = 0;
    virtual void render(QPainter * /*painter*/, QPointF /*translate*/, const QRectF & /*clipRect*/){}
    virtual void renderPageCompleted() {}
    virtual void renderReset(){}


    virtual BaseItemInterface * itemAt(QPointF pos) = 0;
    virtual QList<BaseItemInterface *> itemsAt(QPointF pos) = 0;

    // GUI
    virtual PageManipulatorInterface * createManupulator(QObject * parent = 0) = 0;
    virtual int pageManupilatorID() = 0;
    virtual QList<BaseItemInterface *> selectedItems() = 0;
    virtual void setSelectedItems(QList<BaseItemInterface *> selected) = 0;
    virtual BaseItemInterface * currentItem() = 0;
    virtual void setCurrentItem(BaseItemInterface * item) = 0;
    virtual QList <PageViewInterface *> views() = 0;
    virtual PageViewInterface * createView(QWidget * parent = 0) = 0;
    virtual CuteReport::PageViewInterface * createSimpleView(QWidget * parent = 0) = 0; // the simplest view without any additional elements like shadow, rulers, etc.
    virtual  QGraphicsItem * pageItem() = 0;

    // mapping units
//    virtual QRectF mapFromPixel(QRectF rect, int customDPI = 0) = 0;
//    virtual QPointF mapFromPixel(QPointF point, int customDPI = 0) = 0;
//    virtual QRect mapToPixel(QRectF rect, int customDPI = 0) = 0;
//    virtual QPoint mapToPixel(QPointF point, int customDPI = 0) = 0;

    virtual void _setCurrentProperty(int num) {m_currentProperty = num;}
    virtual int _currentProperty() { return m_currentProperty;}
    virtual QString _current_property_description() const;
    virtual int _current_property_precision() const;

signals:
    //GUI methods
    void activeObjectChanged(QObject * object);
    void beforeNewItemAdded(CuteReport::BaseItemInterface * item, bool * cancel);
    void afterNewItemAdded(CuteReport::BaseItemInterface * item);
    void beforeItemRemoved(CuteReport::BaseItemInterface* item, bool * cancel);
    void afterItemRemoved(CuteReport::BaseItemInterface* item, QString itemName, bool directDeletion);
    void itemGeometryChanged(BaseItemInterface * item, QRectF oldGeometry, QRectF newGeometry);
    void viewCreated(PageViewInterface *);

    //properties
    void orientationChanged(Orientation newOrientation);
    void formatChanged(QString newFormat);
    void paperSizeChanged(QSizeF newFormatSize);
    void pageRectChanged(QRectF newFormatSize);
    void unitChanged(CuteReport::Unit newUnit);
    void unitChanged(QString newUnit);
    void dpiChanged(int newDpi);
    void marginLeftChanged(qreal margin);
    void marginTopChanged(qreal margin);
    void marginRightChanged(qreal margin);
    void marginBottomChanged(qreal margin);
    void marginChanged();
    void backgroundChanged(QColor);

protected:
    PageInterface(const PageInterface &dd, QObject * parent);
    virtual PageInterface * objectClone() const = 0;

    int m_currentProperty;

    friend class ReportCore;
};


/******************************************************************
 *
 *  PageManipulatorInterface
 *
 ******************************************************************/

class CUTEREPORT_EXPORTS PageManipulatorInterface : public QObject
{
    Q_OBJECT
public:
    explicit PageManipulatorInterface(QObject *parent = 0);

    virtual void setActivePage(PageInterface * page) = 0;
    virtual PageInterface * activePage() = 0;
    virtual int pageManupilatorId() = 0;

    virtual QList<PageAction*> actions() = 0;
    virtual QList<QLabel*> statusBarLabels() = 0;
};


class CUTEREPORT_EXPORTS RenderedPageInterface: public QObject, public QGraphicsRectItem
{
public:
    enum {Type = QGraphicsItem::UserType + 12789};
    enum ShowFlag { ShowNothing =          0x00,
                    ShowBackground =       0x01,
                    ShowPaperBorder =      0x04,
                    ShowPageBorder =       0x08,
                    ShowDefaultPreview = ShowBackground | ShowPaperBorder | ShowPageBorder,
                    ShowDefaultExport = ShowPaperBorder,
                    ShowDefaultPrint = ShowNothing
                  };
    Q_DECLARE_FLAGS(ShowFlags, ShowFlag)

    explicit RenderedPageInterface ( QGraphicsItem * parent = 0 ) : QGraphicsRectItem(parent), m_showFlags(ShowDefaultPreview){}

    virtual void setShowFlags(ShowFlags f) {m_showFlags = f;}
    virtual ShowFlags showFlags() {return m_showFlags;}

    virtual CuteReport::PageInterface::Orientation orientation() const = 0;
    //virtual CuteReport::Margins margins(CuteReport::Unit unit) const = 0;
    virtual QRectF paperRect(CuteReport::Unit unit) const = 0;
    virtual QSizeF paperSize(CuteReport::Unit unit) const = 0;
    virtual QRectF pageRect(CuteReport::Unit unit) const = 0;
    virtual QSizeF pageSize(CuteReport::Unit unit) const = 0;
    virtual int dpi() const = 0;
    virtual void setDpi(int dpi) = 0;

    virtual void redraw() = 0;

    virtual int type() const { return Type; }

protected:
    ShowFlags m_showFlags;
};

class CUTEREPORT_EXPORTS PageViewInterface: public QWidget
{
public:
    explicit PageViewInterface(QWidget * parent = 0, Qt::WindowFlags f = 0 ){ Q_UNUSED(parent); Q_UNUSED(f)}
    virtual void fit() = 0;

    //virtual int type() const { return PageViewInterfaceType; }
};

} //namespace

Q_DECLARE_OPERATORS_FOR_FLAGS( CuteReport::RenderedPageInterface::ShowFlags )
Q_DECLARE_METATYPE(CuteReport::PageAction)
Q_DECLARE_INTERFACE(CuteReport::PageInterface, "CuteReport.PageInterface/1.0")
Q_DECLARE_INTERFACE(CuteReport::RenderedPageInterface, "CuteReport.RenderedPageInterface/1.0")
Q_DECLARE_INTERFACE(CuteReport::PageViewInterface, "CuteReport.PageViewInterface/1.0")

#endif // PAGEINTERFACE_H
