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
#ifndef BASEITEMINTERFACE_H
#define BASEITEMINTERFACE_H

#include <QObject>
#include <QtCore>
#include <QGraphicsWidget>
#include <QPen>
#include <QWidget>
#include <QPolygonF>

#include "reportplugininterface.h"
#include "types.h"
#include "functions.h"
#include "globals.h"

#define ITEMVIEWTYPE QGraphicsItem::UserType + 101

class QScriptEngine;

namespace CuteReport
{

class PageInterface;
class BaseItemHelperInterface;
class BaseItemInterfacePrivate;
class RenderedItemInterfacePrivate;
class ItemInterfaceView;
class RendererInterface;
class RenderedItemInterface;
class RendererPublicInterface;
class ReportInterface;


class CUTEREPORT_EXPORTS BaseItemInterface : public ReportPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(CuteReport::ReportPluginInterface)

    Q_FLAGS(Frame Frames)
    Q_ENUMS(Frame ResizeFlags)

    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(QRectF geometry READ geometry WRITE setGeometry NOTIFY geometryChanged)
    Q_PROPERTY(QRectF boundingRect READ boundingRect NOTIFY boundingRectChanged)
    Q_PROPERTY(Frames frame READ frame WRITE setFrame NOTIFY frameChanged)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation NOTIFY rotationChanged)
    Q_PROPERTY(QPen borderPen READ borderPen WRITE setBorderPen NOTIFY borderPenChanged)
    Q_PROPERTY(QBrush backgroundBrush READ backgroundBrush WRITE setBackgroundBrush NOTIFY backgroundBrushChanged)
    Q_PROPERTY(int order READ order WRITE setOrder NOTIFY orderChanged)

    Q_PROPERTY(int _current_property READ _currentProperty WRITE _setCurrentProperty DESIGNABLE false)
    Q_PROPERTY(QString _current_property_description READ _current_property_description DESIGNABLE false)
    Q_PROPERTY(int _current_property_precision READ _current_property_precision DESIGNABLE false)

public:
    enum Frame {DrawLeft = 1, /**< Draw left frame*/
                DrawRight = 2, /**< Draw right frame*/
                DrawTop = 4, /**< Draw top frame*/
                DrawBottom = 8 /**< Draw bottom frame*/
               };
    Q_DECLARE_FLAGS(Frames, Frame)

    enum ResizeFlags {Fixed = 0, /**< Item cant be resized*/
                      ResizeLeft = 1, /**< Item can be resized to left*/
                      ResizeRight = 2, /**< Item can be resized to right*/
                      ResizeTop = 4, /**< Item can be resized to top*/
                      ResizeBottom = 8, /**< Item can be resized to bottom*/
                      FixedPos = 16 /**< Item cant be moved*/
                     };

    explicit BaseItemInterface(QObject * parent);
    virtual ~BaseItemInterface();

    /** this method used for register metatypes, etc. It should be called only once */
    virtual void moduleInit();

    /** every new created item should be inited using this method */
    virtual void init();

    virtual void check_gui();
    virtual void update_gui();
    virtual void init_gui() = 0;

    virtual BaseItemInterface * createInstance(QObject * parent) const = 0;
    virtual BaseItemInterface * clone(bool withChildren = true, bool init = true) const;

    virtual PageInterface * page() const;
    virtual void setPage(PageInterface * page);
    virtual BaseItemInterface * parentItem() const;
    virtual void setParentItem(BaseItemInterface *parentItem);
    CuteReport::ReportInterface * reportObject();
    virtual int childLevel();
    CuteReport::BandInterface * carrierBand();

    virtual BaseItemInterface * topmostChildAt(const QPointF & localPos);
    virtual QList<BaseItemInterface *> childrenAt(const QPointF & localPos);
    QList<BaseItemInterface *> allChildrenAt(const QPointF & localPos);

    virtual BaseItemHelperInterface * helper() { return 0; }
    virtual QByteArray serialize();
    virtual void deserialize(QByteArray &data);

    virtual bool canContain(QObject * object);
    virtual ItemInterfaceView * view();
    virtual void updateView();

    virtual void saveState();
    virtual void restoreState();

    /** method called while script engine initing for export any custom types to the engine
    * Method should be called once per engine
    */
    virtual void initScript(QScriptEngine * scriptEngine) {Q_UNUSED(scriptEngine);}

    /** method called when renderer starts its work, nothing rendered yet */
    virtual void renderInit(RendererPublicInterface * renderer);

    /** method called when renderer has completed its work, all is already rendered */
    virtual void renderReset();

    /** method is called to calculate rendered item data, replace internal item's data pointer with the
     * new rendered data and return rendered itemView if needed
     * return bool: true - item is printable; false - item is not printable
     */
    virtual bool renderPrepare() = 0;

    /** render image to print */
    virtual RenderedItemInterface * renderView() = 0;

    /** method called after rendered item's geometry and position was managed and became final
     * Every item must implement this method, return back correct internal data pointer and send printAfter signal
     */
    virtual bool renderEnd();

    /** method called before items on the same level (have the same parent) will be rendered */
    virtual void beforeSiblingsRendering() {}
    /** method called after items on the same level (have the same parent) have rendered */
    virtual void afterSiblingsRendering() {}

    /** item provides its own geometry logic so renderer will net change it */
    virtual bool selfRendering();

    /** item renders its children by itself. Renderer will not touch children */
    virtual bool childrenSelfRendering();

    virtual QIcon itemIcon() const = 0;
    virtual QString itemGroup() const = 0;
    virtual int itemSortOrderInGroup() const {return 0;}

    virtual Unit unit() const;
    virtual void setUnit(Unit unit);

    virtual QRectF geometry(Unit unit = UnitNotDefined) const;
    virtual void setGeometry(const QRectF & rect, Unit unit = UnitNotDefined);
    virtual qreal height(Unit unit = UnitNotDefined) const;
    virtual void setHeight(qreal height, Unit unit = UnitNotDefined);
    virtual qreal width(Unit unit = UnitNotDefined) const;
    virtual void setWidth(qreal width, Unit unit = UnitNotDefined);
    virtual QSizeF minSize(Unit unit = UnitNotDefined) const;
    virtual QRectF boundingRect(Unit unit = UnitNotDefined) const;
    virtual QPolygonF polygon(Unit unit = UnitNotDefined) const;

    virtual qint8 frame() const;
    virtual void setFrame(Frames frame);
    virtual qreal opacity() const;
    virtual void setOpacity(qreal opacity);
    virtual qreal rotation() const;
    virtual void setRotation(qreal angle);

    virtual QPen borderPen() const;
    virtual void setBorderPen(const QPen & pen);
    virtual QBrush backgroundBrush() const;
    virtual void setBackgroundBrush(const QBrush & brush);

    int resizeFlags() const;

    virtual bool enabled() const;
    virtual void setEnabled(bool b = true);

    virtual int order() const;
    virtual void setOrder(int order);

    virtual void setDpi(int dpi);
    virtual int dpi() const;

    virtual bool isSplitAllowed() const {return false;}

    virtual QStringList scriptingStrings(){return QStringList();}

    // common painter
    static void paintBegin(QPainter * painter, const QStyleOptionGraphicsItem *option, const BaseItemInterfacePrivate * data, const QRectF &boundingRect = QRectF(), RenderingType type = RenderingTemplate);
    static void paintEnd(QPainter * painter, const QStyleOptionGraphicsItem *option, const BaseItemInterfacePrivate * data, const QRectF &boundingRect = QRectF(), RenderingType type = RenderingTemplate);

    virtual QTransform transform() const;

    // values in local unit
    // NotDefined = default item meassure
    QRectF mapFromPage(const QRectF & rect, Unit inputUnit = UnitNotDefined, Unit outputUnit = UnitNotDefined) const;
    QPointF mapFromPage(const QPointF & point, Unit inputUnit = UnitNotDefined, Unit outputUnit = UnitNotDefined) const;
    QRectF mapToPage(const QRectF & rect, Unit inputUnit = UnitNotDefined, Unit outputUnit = UnitNotDefined) const;
    QPointF mapToPage(const QPointF & point, Unit inputUnit = UnitNotDefined, Unit outputUnit = UnitNotDefined) const;
    QRectF mapToParent(const QRectF & rect, Unit inputUnit = UnitNotDefined, Unit outputUnit = UnitNotDefined) const ;
    QPointF mapToParent(const QPointF & point, Unit inputUnit = UnitNotDefined, Unit outputUnit = UnitNotDefined) const;
    QRectF mapFromParent(const QRectF & rect, Unit inputUnit = UnitNotDefined, Unit outputUnit = UnitNotDefined) const ;
    QPointF mapFromParent(const QPointF & point, Unit inputUnit = UnitNotDefined, Unit outputUnit = UnitNotDefined) const;
    QRectF absoluteGeometry(Unit outputUnit = UnitNotDefined) const;
    void setAbsoluteGeometry(const QRectF & rect, Unit inputUnit = UnitNotDefined);
    QRectF absoluteBoundingRect(Unit unit = UnitNotDefined) const;
    QPolygonF absolutePolygon(Unit unit = UnitNotDefined) const;

    static CuteReport::PageInterface * itemPage(CuteReport::BaseItemInterface * item);
    static QFont fontPrepared(const QFont & font, qint16 dpi);
    static QPen penPrepared(const QPen & pen, qint16 dpi);
    static QTransform transform(const BaseItemInterfacePrivate *d);
    static QPointF transformedPos(const BaseItemInterfacePrivate *d, const QRectF &rect);

    virtual void _setCurrentProperty(int num) {m_currentProperty = num;}
    virtual int _currentProperty() { return m_currentProperty;}
    virtual QString _current_property_description() const;
    virtual int _current_property_precision() const;

signals:
    void unitChanged(Unit);
    void geometryChanged(QRectF newGeometry);
    void boundingRectChanged(QRectF newRect);
    void opacityChanged(qreal newOpacity);
    void frameChanged(Frames);
    void rotationChanged(qreal angle);
    void borderPenChanged(QPen pen);
    void backgroundBrushChanged(QBrush brush);
    void dpiChanged(int dpi);
    void enabledChanged(bool);
    void orderChanged(int);
    void parentItemChanged(CuteReport::BaseItemInterface * parent);
    void scriptingStringsChanged();

    /** signal emited when renderInit() method is called */
    void printInit();

    /** signal emited when renderReset() method is called */
    void printReset();

    /** signal emited when render() method processing started */
    void printBefore();

    /** signal emited in render() method when item's private data replaced by itemView's private data
     * but before data processing
     *  So any method will work direct with new item's data and not original private item's data itself
     */
    void printDataBefore();

    /** signal emited in render() method when item's private data replaced by itemView's private data
     * and data processed
     *  So any method will work direct with itemView's data and not affect private original item's data itself
     */
    void printDataAfter();

    /** signal emited when itemView already generated and all preprocessing is done
     * This signal must be inited after all gemetry arrangement is done, so only rendering module can init it from outside item
    */
    void printAfter();
    
public slots:
    void updateViewIfExists();

protected:
    BaseItemInterface(BaseItemInterfacePrivate *dd, QObject * parent);
    virtual BaseItemInterface * itemClone() const = 0;
    void setRenderingPointer(BaseItemInterfacePrivate * r);
    void replaceRenderingPointer();

    virtual void childAdded(BaseItemInterface * item) { Q_UNUSED(item); }
    virtual void childRemoved(BaseItemInterface * item) { Q_UNUSED(item); }

    void setResizeFlags(int resizeFlags);
    static const QRectF adjustRect(QRectF & rect, const QPen & pen);
    virtual void adjustOrder();
    virtual void updateMeassure();

protected:
    BaseItemInterfacePrivate * d_ptr;
    BaseItemInterfacePrivate * orig_ptr;
    QList <BaseItemInterfacePrivate*>  dataStack;
    ItemInterfaceView * m_gui;
    PageInterface * m_page;
    BaseItemInterface * m_parentItem;
    bool m_inited;
    int m_currentProperty;

private:
    void initMe();
    Q_DECLARE_PRIVATE(BaseItemInterface)

    friend class ReportCore;
    friend class BaseRenderedItemInterface;
    friend class ItemInterfaceView;
};


class CUTEREPORT_EXPORTS BaseItemHelperInterface: public QWidget
{
public:
    virtual void sync(){}
    virtual bool screenBack(bool accept = true){Q_UNUSED(accept); return false;} //only one screen by default;
};


Q_DECLARE_OPERATORS_FOR_FLAGS(BaseItemInterface::Frames)
} // namespace

Q_DECLARE_INTERFACE(CuteReport::BaseItemInterface, "CuteReport.BaseItemInterface/1.0")
Q_DECLARE_METATYPE(CuteReport::BaseItemInterface*)
Q_DECLARE_METATYPE(CuteReport::BaseItemInterface::Frame)
Q_DECLARE_METATYPE(CuteReport::BaseItemInterface::ResizeFlags)

#endif // BASEITEMINTERFACE_H
