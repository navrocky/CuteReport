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
    Q_PROPERTY(Frames frame READ frame WRITE setFrame NOTIFY frameChanged)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation NOTIFY rotationChanged)
    Q_PROPERTY(QPen borderPen READ borderPen WRITE setBorderPen NOTIFY borderPenChanged)
    Q_PROPERTY(QBrush backgroundBrush READ backgroundBrush WRITE setBackgroundBrush NOTIFY backgroundBrushChanged)
    Q_PROPERTY(int order READ order WRITE setOrder NOTIFY orderChanged)

    Q_PROPERTY(int _current_property READ _currentProperty WRITE _setCurrentProperty DESIGNABLE false)
    Q_PROPERTY(QString _current_property_description READ _current_property_description DESIGNABLE false)

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

    virtual PageInterface * page() const;
    virtual void setPage(PageInterface * page);
    virtual BaseItemInterface * parentItem() const;
    virtual void setParentItem(BaseItemInterface *parentItem);
    CuteReport::ReportInterface * reportObject();
    virtual int childLevel();

    virtual BaseItemInterface * topmostChildAt(const QPointF & localPos);
    virtual QList<BaseItemInterface *> childrenAt(const QPointF & localPos);
    QList<BaseItemInterface *> allChildrenAt(const QPointF & localPos);

    virtual BaseItemHelperInterface * helper() { return 0; }
    virtual BaseItemInterface * clone() = 0;
    virtual QByteArray serialize();
    virtual void deserialize(QByteArray &data);

    virtual bool canContain(QObject * object);
    virtual ItemInterfaceView * view();
    virtual void updateView();

    virtual void saveState();
    virtual void restoreState();

    /** method called when renderer starts its work, nothing rendered yet */
    virtual void renderInit(RendererPublicInterface * renderer);
    /** method called when renderer has completed its work, all is already rendered */
    virtual void renderReset();

    /** method called while script engine initing for export any custom types to the engine
    * Method should be called once per engine
    */
    virtual void initScript(QScriptEngine * scriptEngine) {Q_UNUSED(scriptEngine);}

    /** method called before items on the same level (have the same parent) will be rendered */
    virtual void beforeSiblingsRendering() {}
    virtual RenderedItemInterface * render(int customDPI = 0) = 0;
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

    virtual QStringList scriptingStrings(){return QStringList();}

    // common painter
    static void paintBegin(QPainter * painter, const QStyleOptionGraphicsItem *option, const BaseItemInterfacePrivate * data, const QRectF &boundingRect = QRectF(), RenderingType type = RenderingTemplate);
    static void paintEnd(QPainter * painter, const QStyleOptionGraphicsItem *option, const BaseItemInterfacePrivate * data, const QRectF &boundingRect = QRectF(), RenderingType type = RenderingTemplate);

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

    static CuteReport::PageInterface * itemPage(CuteReport::BaseItemInterface * item);
    static QFont fontPrepared(const QFont & font, qint16 dpi);
    static QPen penPrepared(const QPen & pen, qint16 dpi);

    virtual void _setCurrentProperty(int num) {m_currentProperty = num;}
    virtual int _currentProperty() { return m_currentProperty;}
    virtual QString  _current_property_description() const;

signals:
    void changed();
    void unitChanged(Unit);
    void geometryChanged(QRectF newGeometry);
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
    void renderingInit();

    /** signal emited when renderReset() method is called */
    void renderingReset();

    /** signal emited when render() method processing started */
    void renderingBefore();

    /** signal emited in render() method when item's private data repleced by itemView's private data
     *  So any method will work direct with itemView's data and not affect private item's data itself
     *  Signal emited before any item's string precessing */
    void rendering();

    /** signal emited when itemView already generated and all preprocessing done */
    void rendered(CuteReport::RenderedItemInterface *itemView);

    /** signal emited when itemView already generated and all preprocessing done *
     * The same as rendered(...), but without exposing itemView */
    void renderingAfter();
    
public slots:
    void updateViewIfExists();

protected:
    virtual BaseItemInterface * createInstance(QObject * parent) const = 0;
    BaseItemInterface(BaseItemInterfacePrivate &dd, QObject * parent);

    virtual void childAdded(BaseItemInterface * item) { Q_UNUSED(item); }
    virtual void childRemoved(BaseItemInterface * item) { Q_UNUSED(item); }

    void setResizeFlags(int resizeFlags);
    static const QRectF adjustRect(QRectF & rect, const QPen & pen);
    virtual void adjustOrder();
    virtual void updateMeassure();

protected:
    BaseItemInterfacePrivate * d_ptr;
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
};


Q_DECLARE_OPERATORS_FOR_FLAGS(BaseItemInterface::Frames)
} // namespace

Q_DECLARE_INTERFACE(CuteReport::BaseItemInterface, "CuteReport.BaseItemInterface/1.0")
Q_DECLARE_METATYPE(CuteReport::BaseItemInterface*)
Q_DECLARE_METATYPE(CuteReport::BaseItemInterface::Frame)
Q_DECLARE_METATYPE(CuteReport::BaseItemInterface::ResizeFlags)

#endif // BASEITEMINTERFACE_H
