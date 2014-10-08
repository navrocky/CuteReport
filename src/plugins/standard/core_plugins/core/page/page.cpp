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
 ***************************************************************************/
#include "page.h"
#include "pagegui.h"
#include "iteminterface.h"
#include "bandinterface.h"
#include "renderediteminterface.h"
#include "reportcore.h"
#include "reportinterface.h"
#include "layoutmanager.h"
#include "pagemanipulator.h"
#include "types.h"

#include "QtCore"
#include "QtGui"
#include <QApplication>
#include <QDesktopWidget>

#define MANIPULATOR_ID 17322121
#define MODULENAME "Page"

inline void initMyResource() { Q_INIT_RESOURCE(page); }

using namespace CuteReport;

SUIT_BEGIN_NAMESPACE

QMap<QString, PageFormat> Page::m_formats;
QStringList Page::m_formatVariants;

Page::Page(QObject * parent):
    PageInterface(parent),
    m_inited(false),
    m_gui(0),
    m_itemsConnected(true),
    m_currentProperty(-1)
{
    d = new PagePrivateData;
    d->unit = Millimeter;
    d->orientation = Portrait;
    d->dpi = QApplication::desktop()->physicalDpiX();
    d->magnetRate = 6;
    d->magnetValue = 0;
    d->background = QColor(Qt::white);
    d->renderingStage = false;

    //    d->grid = new Grid();
}


Page::Page(const Page & dd, QObject * parent)
    :PageInterface(*reinterpret_cast<const PageInterface*>(&dd), parent),
      d(new PagePrivateData(*(dd.d.data()))),
      m_inited(false),
      m_gui(0),
      m_itemsConnected(dd.m_itemsConnected),
      m_currentProperty(dd.m_currentProperty)
{

}


Page::~Page()
{
    delete m_gui;
}


void Page::moduleInit()
{
    initMyResource();
    if (m_formats.isEmpty())
        initFormats();
    else
        ReportCore::log(LogDebug, MODULENAME, "Formats are already preloaded");

    setFormat("A4");
    setMargins(10, 10, 10, 10);
}


QIcon Page::icon()
{
    QIcon icon(":/images/page_icon.png");
    return icon;
}


void Page::init()
{
    if (m_inited)
        return;

    updateMeassure();

    foreach (BaseItemInterface* item, findChildren<BaseItemInterface*>()) {
        if (item->parent() == this)
            prepareNewItem(item, false, true);
    }

    m_inited = true;
}


PageInterface * Page::createInstance(QObject *parent) const
{
    PageInterface * newPage = new Page(parent);
    newPage->setFormat("A4");
    newPage->setMargins(10, 10, 10, 10);
    return newPage;
}


PageInterface *Page::objectClone() const
{
    return new Page(*this, parent());
}


QWidget * Page::createHelper()
{
    return 0;
}


bool Page::canContain(const QObject *object)
{
    return (bool)(qobject_cast<BandInterface*>(object));
}


bool Page::canContainAt(const QObject * object, const QPointF & pos)
{
    BaseItemInterface* item = qobject_cast<BaseItemInterface*>(object);
    BandInterface* band = qobject_cast<BandInterface*>(object);
    BaseItemInterface* parentItem = itemAt(pos);
    BandInterface* parentBand =  qobject_cast<BandInterface*>(parentItem);

    /// Band can't be placed on Item
    if (band && parentItem && !parentBand)
        return false;

    /// Items and all Bands except LayoutFree should be located only within active area
    if (!band || band->layoutType() != BandInterface::LayoutFree) {
        if (pos.x() < d->pageRect.left() || pos.x() > d->pageRect.right() || pos.y() < d->pageRect.top() || pos.y() > d->pageRect.bottom())
            return false;
    }

    if (parentItem)
        return parentItem->canContain(item);
    else {
        /// only bands can by placed direct on page
        if (!band)
            return false;
    }

    return true;
}


QList<BaseItemInterface *> Page::items() const
{
    return findChildren<CuteReport::BaseItemInterface*>();
}


BaseItemInterface *Page::item(const QString &objectName) const
{
    return findChild<CuteReport::BaseItemInterface*>(objectName);
}


PageInterface::Orientation Page::orientation() const
{
    return d->orientation;
}


void Page::setOrientation(const Orientation &orientation)
{
    if (d->orientation == orientation)
        return;

    d->orientation = orientation;
    if (m_inited) {
        QSizeF size = d->format.sizeMM;
        d->format.sizeMM = QSizeF(size.height(), size.width());

        afterGeometryChanged();

        emit orientationChanged(d->orientation);
        emit paperSizeChanged(paperSize());
        emit changed();
    }
}


QString Page::format() const
{
    return d->format.name;
}


void Page::setFormat(const QString & formatName)
{
    if (!m_formats.contains(formatName) || d->format.name == formatName)
        return;

    d->format = m_formats.value(formatName);

    if (m_inited) {
        if (d->orientation == Landscape) {
            QSizeF size = d->format.sizeMM;
            d->format.sizeMM = QSizeF(size.height(), size.width());
        }

        afterGeometryChanged();

        emit formatChanged(d->format.name);
        emit paperSizeChanged(paperSize());
        emit changed();
    }
}


QSizeF Page::paperSize(Unit unit) const
{
    if (m_inited) {
        Unit u = (unit == UnitNotDefined) ? d->unit : unit;
        return convertUnit(d->format.sizeMM, Millimeter, u, d->dpi);
    } else {
        return d->format.sizeMM;
    }
}


void Page::setPaperSize(const QSizeF &size, CuteReport::Unit unit)
{
    if (!m_inited) {
        d->format.sizeMM = size;
        return;
    }

    Unit u = (unit == UnitNotDefined) ? d->unit : unit;
    QSizeF newSize = convertUnit(size, u, Millimeter, d->dpi);

    if (d->format.sizeMM  == newSize)
        return;

    d->format.name = "Custom";
    d->format.sizeMM = newSize;

    if (m_inited) {
        if (d->orientation == Landscape && d->format.sizeMM.width() < d->format.sizeMM.height()) {
            d->orientation = Portrait;
            emit orientationChanged(d->orientation);
        } else if (d->orientation == Portrait && d->format.sizeMM.width() > d->format.sizeMM.height()) {
            d->orientation = Landscape;
            emit orientationChanged(d->orientation);
        }

        afterGeometryChanged();

        emit formatChanged(d->format.name);
        emit paperSizeChanged(paperSize());
        emit changed();
    }
}


QStringList Page::_format_variants() const
{
    return m_formatVariants ;
}


QStringList Page::_unit_variants() const
{
    return QStringList() << unitToFullString(Millimeter) << unitToFullString(Inch) << unitToFullString(Pixel);
}


QString Page::_current_property_description() const
{
    QString propertyName = metaObject()->property(m_currentProperty).name();

    if (propertyName == "objectName")
        return tr("Page name");
    else if (propertyName == "magnetRate")
        return tr("Multiplier for calculate active magnet distance:\n currentUnit * magnetRate");
    else if (propertyName == "orientation")
        return tr("Page orientation: Landscape or Portrait");
    else if (propertyName == "format")
        return tr("Page format: standard format can be choosen or custom one");
    else if (propertyName == "formatSize")
        return tr("Page size for current standard format or custom one");
    else if (propertyName == "unit")
        return tr("Page meassure unit: mm, inch or pixel");
    else if (propertyName == "dpi")
        return tr("Page DPI (dot per inch). Affects the quality of preview rendering. Does not affect of printing or export quality");
    else if (propertyName == "margins")
        return tr("Page border size");
    else if (propertyName == "useGrid")
        return tr("if grid set, items geometry will be managed by grid step");
    else if (propertyName == "gridStep")
        return tr("grid step for current unit");
    else if (propertyName == "background")
        return tr("background color");

    return QString();
}


Unit Page::unit() const
{
    return d->unit;
}


void Page::setUnit(const CuteReport::Unit &unit)
{
    if (d->unit == unit)
        return;

    if (m_inited) {
        d->unit = unit;

        foreach (BaseItemInterface * item, findChildren<BaseItemInterface*>())
            item->setUnit( unit );

        emit unitChanged(d->unit);
        emit unitChanged(unitToFullString(d->unit));
        emit changed();
    } else {
        d->unit = unit;
    }
}


QString Page::unitStr() const
{
    QString str = unitToFullString(d->unit);
    return str;
}


void Page::setUnitStr(const QString & unit)
{
    setUnit(unitFromString(unit));
}


int Page::dpi() const
{
    return d->dpi;
}


void Page::setDpi(int dpi)
{
    if (dpi == d->dpi)
        return;

    d->dpi = dpi;

    foreach (BaseItemInterface*item, findChildren<BaseItemInterface*>())
        item->setDpi(d->dpi);

    emit dpiChanged(dpi);
    emit changed();
}


void Page::afterGeometryChanged()
{
    d->pageRect = QRectF(d->marginLeft, d->marginTop,
                         qMax(0.0, d->format.sizeMM.width() - d->marginLeft - d->marginRight),
                         qMax(0.0, d->format.sizeMM.height() - d->marginTop - d->marginBottom));

    m_itemsConnected = false;
    LayoutManager::updatePositions(this);
    m_itemsConnected = true;

    if (m_gui)
        m_gui->updateItems();
}


Units Page::units() const
{
    return d->units;
}


void Page::setUnits(const Units &units)
{
    d->units = units;
}



QColor Page::background() const
{
    return d->background;
}


void Page::setBackground(const QColor & background)
{
    if (d->background == background)
        return;

    d->background = background;
    emit backgroundChanged(d->background);
    emit changed();
}


bool Page::addItem(BaseItemInterface * item, QPointF pagePos, QString * error)
{
    bool cancel = false;
    if (cancel) {
        if (error)
            *error = "canceled";
        return false;
    }

    if (!canContainAt(item, pagePos)) {
        if (error)
            *error = "Page can't conatain the item in this position";
        return false;
    }

    BaseItemInterface* parentItem = itemAt(pagePos);
    item->setParentItem(parentItem);

    QRectF geometry(convertUnit(pagePos, d->unit, Millimeter, item->dpi()), item->geometry(Millimeter).size());
    item->setAbsoluteGeometry(geometry, Millimeter);

    emit beforeNewItemAdded(item, &cancel);

    prepareNewItem(item, true);

    if (m_gui)
        m_gui->itemAdded(item);

    emit afterNewItemAdded(item);
    emit activeObjectChanged(item);

    return true;
}


bool Page::addItem(BaseItemInterface *item)
{
    bool cancel = false;
    emit beforeNewItemAdded(item, &cancel);
    if (cancel) {
        return false;
    }

    prepareNewItem(item, true);

    if (m_gui)
        m_gui->itemAdded(item);

    emit afterNewItemAdded(item);
    emit activeObjectChanged(item);

    return true;
}


bool Page::addItem(const QString & moduleName, QPointF pagePos, QString * error)
{
    // do not set parent, because need to check it first may it be placed or not
    ReportInterface * report = dynamic_cast<CuteReport::ReportInterface *>(parent());
    BaseItemInterface * item = reportCore()->createItemObject(report, moduleName, 0);
    if (!item) {
        if (error)
            *error = QString("item by moduleName \'%1\' not found").arg(moduleName);
        return false;
    }

    if (!this->addItem(item, pagePos))
        delete item;

    return true;
}


void Page::deleteItem(BaseItemInterface *item)
{
    if (!item)
        return;

    if (m_gui && m_gui->currentItem() == item)
        emit activeObjectChanged(item->parentItem() ? (QObject*)item->parentItem() : (QObject*)this);

    bool isBand = (bool)qobject_cast<CuteReport::BandInterface *>(item);
    _deleteItem(item);
    if (isBand)
        LayoutManager::updatePositions(this);
}


void Page::deleteItem(const QString &itemName)
{
    deleteItem(item(itemName));
}


void Page::_deleteItem(BaseItemInterface *item, bool emitSignals, bool directDeletion)
{
    QString itemName = item->objectName();
    reportCore()->log(LogDebug, MODULENAME, QString("_deleteItem(%1)").arg(itemName));
    bool cancel;


    if (emitSignals)
        emit beforeItemRemoved(item,&cancel);

    disconnect(item, SIGNAL(destroyed(QObject*)), this, SLOT(slotItemDestroyed(QObject*)));

    item->deleteLater();

    if (emitSignals)
        emit afterItemRemoved(item, itemName, directDeletion);

    foreach (BaseItemInterface * child, item->findChildren<BaseItemInterface *>())
        if (child->parentItem() == item)
            _deleteItem(child, emitSignals, false);

    if (m_gui)
        m_gui->itemBeforeDestroyed(item);
}


QRectF Page::pageRect(CuteReport::Unit unit)
{
    if (m_inited) {
        Unit u = (unit == UnitNotDefined) ? d->unit : unit;
        return convertUnit(d->pageRect, Millimeter, u, d->dpi);
    } else {
        return d->pageRect;
    }
}


qreal Page::marginLeft(CuteReport::Unit unit) const
{
    if (!m_inited)
        return d->marginLeft;
    CuteReport::Unit u = (unit == UnitNotDefined) ? d->unit : unit;
    return convertUnit(d->marginLeft, Millimeter, u, d->dpi);
}


qreal Page::marginTop(CuteReport::Unit unit) const
{
    if (!m_inited)
        return d->marginTop;
    CuteReport::Unit u = (unit == UnitNotDefined) ? d->unit : unit;
    return convertUnit(d->marginTop, Millimeter, u, d->dpi);
}


qreal Page::marginRight(CuteReport::Unit unit) const
{
    if (!m_inited)
        return d->marginRight;
    CuteReport::Unit u = (unit == UnitNotDefined) ? d->unit : unit;
    return convertUnit(d->marginRight, Millimeter, u, d->dpi);
}


qreal Page::marginBottom(CuteReport::Unit unit) const
{
    if (!m_inited)
        return d->marginBottom;
    CuteReport::Unit u = (unit == UnitNotDefined) ? d->unit : unit;
    return convertUnit(d->marginBottom, Millimeter, u, d->dpi);
}


void Page::setMarginLeft(qreal margin, CuteReport::Unit unit)
{
    _setMargin(d->marginLeft, margin, unit);
    if (!m_inited)
        return;
    emit marginLeftChanged(marginLeft(d->unit));
    emit marginChanged();
    emit changed();
}


void Page::setMarginTop(qreal margin, CuteReport::Unit unit)
{
    _setMargin(d->marginTop, margin, unit);
    if (!m_inited)
        return;
    emit marginTopChanged(marginTop(d->unit));
    emit marginChanged();
    emit changed();
}


void Page::setMarginRight(qreal margin, CuteReport::Unit unit)
{
    _setMargin(d->marginRight, margin, unit);
    if (!m_inited)
        return;
    emit marginRightChanged(marginRight(d->unit));
    emit marginChanged();
    emit changed();
}


void Page::setMarginBottom(qreal margin, CuteReport::Unit unit)
{
    _setMargin(d->marginBottom, margin, unit);
    if (!m_inited)
        return;
    emit marginBottomChanged(marginBottom(d->unit));
    emit marginChanged();
    emit changed();
}


void Page::setMargins(qreal left, qreal top, qreal right, qreal bottom, CuteReport::Unit unit)
{
    _setMargin(d->marginLeft, left, unit, false);
    _setMargin(d->marginRight, right, unit, false);
    _setMargin(d->marginTop, top, unit, false);
    _setMargin(d->marginBottom, bottom, unit, false);
    afterGeometryChanged();
    emit marginChanged();
    emit changed();
}


void Page::_setMargin(qreal & currentValue, const qreal & newValue, CuteReport::Unit unit, bool processGeometry)
{
    if (!m_inited) {
        currentValue = newValue;
        return;
    }

    CuteReport::Unit u = (unit == UnitNotDefined) ? d->unit : unit;
    qreal newValueMM = convertUnit(newValue, u, Millimeter, d->dpi);

    if (currentValue == newValueMM)
        return;

    currentValue = qMax(0.0, newValueMM);

    if (processGeometry)
        afterGeometryChanged();
}


BaseItemInterface * Page::itemAt(QPointF pos)
{
    QMap<int, BaseItemInterface*> items;
    foreach (BaseItemInterface * const item, findChildren<BaseItemInterface *>()){
        QRectF itemGeometry = item->absoluteBoundingRect();
        if (itemGeometry.contains(pos))
            items.insert(item->childLevel(), item);
    }

    if (items.isEmpty())
        return 0;

    int maxLevel = (--items.end()).key();
    BaseItemInterface* topmostItem = *(--items.end());

    foreach (BaseItemInterface * const item, items.values(maxLevel))
        if (item->order() > topmostItem->order())
            topmostItem = item;

    return topmostItem;
}


QList<BaseItemInterface *> Page::itemsAt(QPointF pos)
{
    QList<BaseItemInterface *> list;
    foreach (BaseItemInterface * const item, findChildren<BaseItemInterface *>()){
        if (item->parent() != this)
            continue;
        QRectF childGeometry = item->absoluteBoundingRect();
        if (childGeometry.contains(pos)) {
            list.append(item);
            list.append(item->childrenAt(item->mapFromPage(pos)));
        }
    }

    return list;
}


int Page::layerLevel(BaseItemInterface * item)
{
    if (item->parentItem())
        return layerLevel(item->parentItem()) + 1;
    else
        return 0;
}


void Page::renderInit()
{
    d->renderingStage = true;
}


CuteReport::RenderedPageInterface * Page::render(int /*customDPI*/)
{
    RenderedPage * renderedPage = new RenderedPage(this);
    return renderedPage;
}


void Page::renderPageCompleted()
{

}


void Page::renderReset()
{
    d->renderingStage = false;
}


/// =========================== GUI ===============================

PageManipulatorInterface *Page::createManupulator(QObject *parent)
{
    return new PageManipulator(parent);
}


int Page::pageManupilatorID()
{
    return MANIPULATOR_ID;
}


QList<BaseItemInterface *> Page::selectedItems()
{
    checkGUI();
    return m_gui->selectedItems();
}


void Page::setSelectedItems(QList<BaseItemInterface *> selected)
{
    if (!m_gui)
        return;
    m_gui->setSelectedItems(selected);
}


BaseItemInterface *Page::currentItem()
{
    checkGUI();
    return m_gui->currentItem();
}


void Page::setCurrentItem(BaseItemInterface * item)
{
    if (!m_gui)
        return;
    m_gui->setCurrentItem(item);
}


QList<PageViewInterface *> Page::views()
{
    checkGUI();
    return m_gui->views();
}


PageViewInterface *Page::createView(QWidget *parent)
{
    checkGUI();
    PageViewInterface * view = m_gui->createView();
    emit viewCreated(view);
    return view;
}


PageViewInterface *Page::createSimpleView(QWidget * parent)
{
    checkGUI();
    PageViewInterface * view = m_gui->createSimpleView();
    emit viewCreated(view);
    return view;
}


void Page::setBandsIndention(qreal value)
{
    d->bandsIndention = value;
    emit bandsIndentionsChanged();
    // FIXME: update view;
}

qreal Page::bandsIndention() const
{
    return d->bandsIndention;
}


int Page::magnetValue() const
{
    return d->magnetValue;
}


void Page::setMagnetValue(int value)
{
    if (d->magnetValue == value)
        return;

    d->magnetValue = value;
    emit magnetValueChanged(d->magnetValue);
}


int Page::magnetRate() const
{
    return d->magnetRate;
}


void Page::setMagnetRate(int rate)
{
    if (d->magnetRate == rate)
        return;

    d->magnetRate = rate;
    emit magnetRateChanged(d->magnetRate);
}


void Page::bringCurrentItemForward()
{
    BaseItemInterface * item = currentItem();
    if (!item || !item->parentItem())
        return;

    item->setOrder(999);
}


void Page::bringCurrentItemBackward()
{
    BaseItemInterface * item = currentItem();
    if (!item || !item->parentItem())
        return;

    item->setOrder(0);
}


bool Page::useGrid() const
{
    return d->useGrid;
}


void Page::setUseGrid(bool b)
{
    if (d->useGrid == b)
        return;
    d->useGrid = b;
    emit useGridChanged(d->useGrid);
}


qreal Page::gridStep(CuteReport::Unit unit)
{
    CuteReport::Unit u = (unit == UnitNotDefined) ? d->unit : unit;
    QHash<CuteReport::Unit,qreal>::const_iterator it = d->gridSteps.find(u);
    if (it == d->gridSteps.end()) { // inser default step
        switch (u) {
            case Millimeter:    d->gridSteps.insert(Millimeter, 0.5); break;
            case Inch:          d->gridSteps.insert(Inch, 0.05); break;
            default:            d->gridSteps.insert(UnitNotDefined, 1.0); break;
        }
    }
    return d->gridSteps.value(u);
}


void Page::setGridStep(qreal value, CuteReport::Unit unit)
{
    if (!m_inited)
        return;
    CuteReport::Unit u = (unit == UnitNotDefined) ? d->unit : unit;
    if (d->gridSteps.value(u) == value)
        return;
    d->gridSteps.insert(u, value);
    emit gridStepChanged(value, u);
}


QString Page::gridSteps()
{
    QStringList list;
    QHash<CuteReport::Unit,qreal>::iterator it;
    for (it = d->gridSteps.begin(); it != d->gridSteps.end(); ++it) {
        list.append(QString("%1=%2").arg(it.key()).arg(it.value()));
    }
    return list.join("|");
}


void Page::setGridSteps(QString values)
{
    QStringList list = values.split("|");
    foreach (QString str, list) {
        CuteReport::Unit unit = (Unit)str.section("=", 0, 0).toInt();
        qreal value = str.section("=", 1, 1).toFloat();
        d->gridSteps.insert(unit, value);
    }
    emit gridStepChanged(d->gridSteps.value(d->unit));
}


QGraphicsItem *Page::pageItem()
{
    return m_gui ? m_gui->pageItem() : 0;
}


///========================= private ===============================


void Page::initFormats()
{
    m_formats.clear();

    QFile file(":/formats.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ReportCore::log(LogDebug, MODULENAME, "Formats are not recognized");
        return;
    }

    QTextStream in(&file);
    int linename = 0;
    while (!in.atEnd()) {
        linename++;
        QString line = in.readLine();
        if (line.left(1) == QString("#"))   // commented out
            continue;
        PageFormat f;
        f.name = line.section("|",0,0);
        f.info = line.section("|", 1,1);
        QString basicMeassure = line.section("|",2,2);
        QString size = line.section("|",3,3);
        qreal width = size.section(":",0,0).toFloat();
        qreal height = size.section(":",1,1).toFloat();

        if (basicMeassure == "mm")
            f.sizeMM = QSize(width, height);
        else if (basicMeassure == "inch")
            f.sizeMM = QSize(width*25.4, height*25.4);

        /// Error checking
        if (f.name.isEmpty()) {
            ReportCore::log(LogWarning, MODULENAME, "Wrong format in line: " + linename );
            continue;
        }
        if (f.sizeMM.width() == 0 || f.sizeMM.height() == 0) {
            ReportCore::log(LogWarning, MODULENAME, "Wrong MM size for format: " + f.name );
            continue;
        }
        m_formats.insert(f.name, f);
        m_formatVariants.append(f.name);
    }
    m_formatVariants.append("Custom");
    ReportCore::log(CuteReport::LogDebug, MODULENAME, QString("%1 format(s) are recognized").arg(m_formats.count()) );
}


void Page::initGUI()
{
    m_gui = new PageGUI(this);
    m_gui->updateItems();
    connect (m_gui, SIGNAL(activeObjectChanged(QObject*)), this, SIGNAL(activeObjectChanged(QObject*)));
}


void Page::updateMeassure()
{
    d->format.sizeMM = convertUnit(d->format.sizeMM, d->unit, Millimeter, d->dpi);
    d->marginLeft = convertUnit(d->marginLeft, d->unit, Millimeter, d->dpi);
    d->marginRight = convertUnit(d->marginRight, d->unit, Millimeter, d->dpi);
    d->marginTop = convertUnit(d->marginTop, d->unit, Millimeter, d->dpi);
    d->marginBottom = convertUnit(d->marginBottom, d->unit, Millimeter, d->dpi);
    d->pageRect = QRectF(d->marginLeft, d->marginTop,
                         d->format.sizeMM.width() - d->marginLeft - d->marginRight,
                         d->format.sizeMM.height() - d->marginTop - d->marginBottom);
}


void Page::slotItemDestroyed(QObject* object)
{
    if (!m_itemsConnected)
        return;

    qCritical() << QString("direct item \'%1\' deleting is not allowed! Use Extended::Page::itemDelete(ItemInterface*) instead.").arg(object->objectName());
    /*
    ItemInterface * item = static_cast<ItemInterface*>(object);
//    BandInterface * band = dynamic_cast<BandInterface*>(item);
    if  (items().contains(item))
        d->items.removeOne(item);

    // TODO: detect and do update only for bands
    LayoutManager::updatePositions(this);
    emit afterItemRemoved(item);
*/
}


void Page::slotItemGeometryChanged(QRectF newGeometry)
{
    Q_UNUSED(newGeometry)
    if (!m_itemsConnected || d->renderingStage)
        return;

    BaseItemInterface * item = qobject_cast<BaseItemInterface*>(sender());
    Q_ASSERT (item);
    LayoutManager::updatePositions(this);

    if (m_gui)
        m_gui->updateItems();
}


void Page::slotItemOrderChanged(int order)
{  
    Q_UNUSED(order);

    if (!m_itemsConnected || d->renderingStage)
        return;
    BaseItemInterface * item = qobject_cast<BaseItemInterface*>(sender());
    Q_ASSERT (item);
    LayoutManager::itemOrderChanged(this, item);
    if (m_gui)
        m_gui->updateItems();
}


void Page::prepareNewItem(BaseItemInterface * item, bool doLayout, bool withChildren)
{
    item->init();
    item->setPage(this);

    if (doLayout) {
        LayoutManager::itemAdded(this, item);
    }


    connect(item, SIGNAL(destroyed(QObject*)), this, SLOT(slotItemDestroyed(QObject*)));
    connect(item, SIGNAL(geometryChanged(QRectF)), this, SLOT(slotItemGeometryChanged(QRectF)));
    connect(item, SIGNAL(orderChanged(int)), this, SLOT(slotItemOrderChanged(int)));

    if (withChildren)
        foreach (BaseItemInterface* child, item->findChildren<BaseItemInterface*>()) {
            if (child->parent() == item)
                prepareNewItem(child, false, true);
        }
}

////////////////////////////////////////////////////////////////////////////////////////
////   RenderedPage
///////////////////////////////////////////////////////////////////////////////////////


RenderedPage::RenderedPage(Page *page)
{
    m_orientation = page->orientation();
    m_paperSizeMM = page->paperSize(CuteReport::Millimeter);
    m_marginLeft = page->marginLeft(CuteReport::Millimeter);
    m_marginRight = page->marginRight(CuteReport::Millimeter);
    m_marginTop = page->marginTop(CuteReport::Millimeter);
    m_marginBottom =  page->marginBottom(CuteReport::Millimeter);
    m_dpi = page->dpi();

    setBrush(QBrush(page->background()));

    redraw();
}


void RenderedPage::paintBegin ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (m_showFlags.testFlag( ShowBackground )) {
        painter->setBrush(brush());
        painter->drawRect(m_rect);
    }
    if (m_showFlags.testFlag( ShowPaperBorder )) {
        painter->setPen(QPen(QColor("#DDDDDD")));
        painter->drawRect(m_border);
    }
}


//CuteReport::Margins RenderedPage::margins(CuteReport::Unit unit) const
//{
//    QPointF p1 = convertUnit(QPointF(marginsLeft, m_marginsMM.top()), Millimeter, unit, m_dpi);
//    QPointF p2 = convertUnit(QPointF(m_marginsMM.right(), m_marginsMM.bottom()), Millimeter, unit, m_dpi);
//    return Margins(p1.x(), p1.y(), p2.x(), p2.y());
//}


QRectF RenderedPage::paperRect(CuteReport::Unit unit) const
{
    return convertUnit(QRectF(QPointF(0,0), m_paperSizeMM), CuteReport::Millimeter, unit, m_dpi);
}


QSizeF RenderedPage::paperSize(CuteReport::Unit unit) const
{
    return convertUnit(m_paperSizeMM, CuteReport::Millimeter, unit, m_dpi);
}


QRectF RenderedPage::pageRect(CuteReport::Unit unit) const
{
    QRectF rect(QPointF(0,0), m_paperSizeMM);
    rect.adjust(m_marginLeft, m_marginTop, -m_marginRight, -m_marginBottom);
    return convertUnit(rect, CuteReport::Millimeter, unit, m_dpi);
}


QSizeF RenderedPage::pageSize(Unit unit) const
{
    QRectF rect(QPointF(0,0), m_paperSizeMM);
    rect.adjust(m_marginLeft, m_marginTop, -m_marginRight, -m_marginBottom);
    return convertUnit(rect.size(), CuteReport::Millimeter, unit, m_dpi);
}


int RenderedPage::dpi() const
{
    return m_dpi;
}


void RenderedPage::setDpi(int dpi)
{
    m_dpi = dpi;

    QList<QGraphicsItem *> list = childItems();
    foreach (QGraphicsItem * child, list) {
        if (child->type() == RenderedItemInterface::Type) {
            RenderedItemInterface * item = static_cast<RenderedItemInterface *>(child);
            item->setDpi(m_dpi);
        }
    }
}


void RenderedPage::redraw()
{
    m_border = pageRect(CuteReport::Pixel);
    m_rect = paperRect(CuteReport::Pixel);

    setRect(m_rect);

    if (scene()) {
        QList<QGraphicsItem *> list = childItems();
        foreach (QGraphicsItem * child, list) {
            if (child->type() == RenderedItemInterface::Type && child->parentItem() == this) {
                RenderedItemInterface * item = static_cast<RenderedItemInterface *>(child);
                item->redraw(true);
            }
        }
    }
}

SUIT_END_NAMESPACE

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Page, SUIT_NAMESPACE::Page)
#endif

