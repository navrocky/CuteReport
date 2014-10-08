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
#ifndef REPORTPAGE_H
#define REPORTPAGE_H

#include "pageinterface.h"
#include "globals.h"
#include "types.h"
#include "plugins_common.h"

namespace CuteReport {
    class BaseItemInterface;
    class BandInterface;
    class RenderedPageInterface;
}

SUIT_BEGIN_NAMESPACE
class PageGUI;
class LayoutManager;
class PageFormat;
class Page;
struct PagePrivateData;
SUIT_END_NAMESPACE

USING_SUIT_NAMESPACE

SUIT_BEGIN_NAMESPACE

class PageFormat {
public:
    QString name;
    QString info;
    QSizeF sizeMM;
};

class Page: public CuteReport::PageInterface
{
    Q_OBJECT
    Q_INTERFACES(CuteReport::PageInterface)
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteReport.PageInterface/1.0")
#endif

//    Q_PROPERTY(qreal bandsIndention READ bandsIndention WRITE setBandsIndention NOTIFY bandsIndentionsChanged)
    Q_PROPERTY(bool useGrid READ useGrid WRITE setUseGrid NOTIFY useGridChanged)
    Q_PROPERTY(qreal gridStep READ gridStep WRITE setGridStep NOTIFY gridStepChanged)
    Q_PROPERTY(int magnetValue READ magnetValue WRITE setMagnetValue DESIGNABLE false NOTIFY magnetValueChanged)
    Q_PROPERTY(int magnetRate READ magnetRate WRITE setMagnetRate NOTIFY magnetRateChanged)
    Q_PROPERTY(QString gridSteps READ gridSteps WRITE setGridSteps DESIGNABLE false)

    Q_PROPERTY(QStringList _format_variants READ _format_variants DESIGNABLE false)
    Q_PROPERTY(QStringList _unit_variants READ _unit_variants DESIGNABLE false)
    Q_PROPERTY(int _current_property READ _currentProperty WRITE _setCurrentProperty DESIGNABLE false)
    Q_PROPERTY(QString _current_property_description READ _current_property_description DESIGNABLE false)

public:
    explicit Page(QObject *parent = 0);
    virtual ~Page();

    virtual void moduleInit();

    virtual QIcon icon();
    virtual QString moduleShortName() const {return QString("Page");}
    virtual QString suitName() const { return SUIT_NAMESPACE_STR; }

    virtual void init();  // work after restoring

    virtual CuteReport::PageInterface * createInstance(QObject * parent = 0) const;
    virtual QWidget * createHelper();

    virtual bool canContain(const QObject * object);
    virtual bool canContainAt(const QObject *object, const QPointF &pos); // position in current page Unit

    /** Page Properties */
    virtual Orientation orientation() const;
    virtual void setOrientation(const Orientation & orientation);
    virtual QString format() const;
    virtual void setFormat(const QString & formatName);
    virtual QSizeF paperSize(CuteReport::Unit unit = CuteReport::UnitNotDefined) const;
    virtual void setPaperSize(const QSizeF & size, CuteReport::Unit unit = CuteReport::UnitNotDefined);   // for custom formats
    virtual QRectF pageRect(CuteReport::Unit unit = CuteReport::UnitNotDefined);
    virtual qreal marginLeft(CuteReport::Unit unit = CuteReport::UnitNotDefined) const;
    virtual qreal marginTop(CuteReport::Unit unit = CuteReport::UnitNotDefined) const;
    virtual qreal marginRight(CuteReport::Unit unit = CuteReport::UnitNotDefined) const;
    virtual qreal marginBottom(CuteReport::Unit unit = CuteReport::UnitNotDefined) const;
    virtual void setMarginLeft(qreal margin, CuteReport::Unit unit = CuteReport::UnitNotDefined);
    virtual void setMarginTop(qreal margin, CuteReport::Unit unit = CuteReport::UnitNotDefined);
    virtual void setMarginRight(qreal margin, CuteReport::Unit unit = CuteReport::UnitNotDefined);
    virtual void setMarginBottom(qreal margin, CuteReport::Unit unit = CuteReport::UnitNotDefined);
    virtual void setMargins(qreal left, qreal top, qreal right, qreal bottom, CuteReport::Unit unit = CuteReport::UnitNotDefined);
    virtual CuteReport::Unit unit() const;
    virtual void setUnit(const CuteReport::Unit & unit);
    virtual QString unitStr() const;
    virtual void setUnitStr(const QString & unit);
    virtual int dpi() const;
    virtual void setDpi(int dpi);
    virtual CuteReport::Units units() const;
    virtual void setUnits(const CuteReport::Units &units);
    virtual QColor background() const;
    virtual void setBackground(const QColor &background);

    virtual bool addItem(CuteReport::BaseItemInterface *item, QPointF pagePos, QString * error = 0);
    virtual bool addItem(CuteReport::BaseItemInterface * item);
    virtual bool addItem(const QString & moduleName, QPointF pagePos, QString * error = 0);
    virtual void deleteItem(CuteReport::BaseItemInterface * item);
    virtual void deleteItem(const QString & itemName);

    /** methods */
    virtual QList <CuteReport::BaseItemInterface*> items() const;
    virtual CuteReport::BaseItemInterface* item(const QString & objectName) const;
    virtual CuteReport::BaseItemInterface *itemAt(QPointF pos);
    virtual QList<CuteReport::BaseItemInterface *> itemsAt(QPointF pos);

    /** Rendering */
    virtual void renderInit();
    virtual CuteReport::RenderedPageInterface * render(int /*customDPI*/ = 0);
    virtual void renderPageCompleted();
    virtual void renderReset();

    /** GUI methhods */
    virtual CuteReport::PageManipulatorInterface * createManupulator(QObject * parent = 0);
    virtual int pageManupilatorID();
    virtual QList<CuteReport::BaseItemInterface *> selectedItems();
    virtual void setSelectedItems(QList<CuteReport::BaseItemInterface *> selected);
    virtual CuteReport::BaseItemInterface * currentItem();
    virtual void setCurrentItem(CuteReport::BaseItemInterface * item);
    virtual QList<CuteReport::PageViewInterface *> views();
    virtual CuteReport::PageViewInterface * createView(QWidget * parent = 0);
    virtual CuteReport::PageViewInterface * createSimpleView(QWidget * parent = 0);
    QGraphicsItem *pageItem();

    /** local GUI properties (not in PageInterface)    */
    virtual void setBandsIndention(qreal value);
    virtual qreal bandsIndention() const;
    int magnetValue() const;
    void setMagnetValue(int value);
    int magnetRate() const;
    void setMagnetRate(int rate);
    void bringCurrentItemForward();
    void bringCurrentItemBackward();
    bool useGrid() const;
    void setUseGrid(bool b);
    qreal gridStep(CuteReport::Unit unit = CuteReport::UnitNotDefined);
    void setGridStep(qreal value, CuteReport::Unit unit = CuteReport::UnitNotDefined);
    QString gridSteps();
    void setGridSteps(QString values);

    /** propertyeditor hints   */
    QStringList _format_variants() const;
    QStringList _unit_variants() const;
    void _setCurrentProperty(int num) {m_currentProperty = num;}
    int _currentProperty() { return m_currentProperty;}
    QString _current_property_description() const;

signals:
    void useGridChanged(bool);
    void gridStepChanged(qreal, CuteReport::Unit unit = CuteReport::UnitNotDefined);
    void bandsIndentionsChanged();
    void magnetRateChanged(int);
    void magnetValueChanged(int);

private:
    Page(const Page & dd, QObject * parent);
    virtual PageInterface * objectClone() const;
    static void initFormats();
    void initGUI();
    void checkGUI() {if (!m_gui) this->initGUI();}
    void updateMeassure();
    void prepareNewItem(CuteReport::BaseItemInterface *item, bool doLayout = false, bool withChildren = true);
    void _deleteItem(CuteReport::BaseItemInterface * item, bool emitSignals = true, bool directDeletion = true);
    int layerLevel(CuteReport::BaseItemInterface *item);
    void afterGeometryChanged();
    void _setMargin(qreal & currentValue, const qreal & newValue, CuteReport::Unit unit, bool processGeometry = true);


private slots:
    void slotItemGeometryChanged(QRectF newGeometry);
    void slotItemOrderChanged(int order);
    void slotItemDestroyed(QObject *object);

private:
    QSharedDataPointer<PagePrivateData> d;
    bool m_inited;
    static QMap<QString, PageFormat> m_formats;
    static QStringList m_formatVariants;
    PageGUI * m_gui;
    bool m_itemsConnected;  // for temporary disable items processing while layout changing
    int m_currentProperty;

friend class PageManipulator;
friend class PageGUI;
friend class LayoutManager;
friend class RenderedPage;
};


struct PagePrivateData : public QSharedData
{
    CuteReport::Unit unit;
    PageFormat format;
    CuteReport::PageInterface::Orientation orientation;
    int dpi;
//    CuteReport::Margins margins;
    qreal marginLeft;
    qreal marginTop;
    qreal marginRight;
    qreal marginBottom;
    QRectF pageRect;
    CuteReport::Units units;
    qreal bandsIndention;
    int magnetValue;
    int magnetRate;
    QColor background;
    bool useGrid;
    QHash<CuteReport::Unit,qreal> gridSteps;
    bool renderingStage;
};


class RenderedPage: public CuteReport::RenderedPageInterface
{
public:
    explicit RenderedPage(Page * page);
    virtual void paintBegin ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

    virtual CuteReport::PageInterface::Orientation orientation() const {return m_orientation;}
//    virtual CuteReport::Margins margins(CuteReport::Unit unit) const;
    virtual QRectF paperRect(CuteReport::Unit unit) const;
    virtual QSizeF paperSize(CuteReport::Unit unit) const;
    virtual QRectF pageRect(CuteReport::Unit unit) const;
    virtual QSizeF pageSize(CuteReport::Unit unit) const;
    virtual int dpi() const;
    virtual void setDpi(int dpi);

    virtual void redraw();
private:

    QSizeF m_paperSizeMM;
    qreal m_marginLeft;
    qreal m_marginRight;
    qreal m_marginTop;
    qreal m_marginBottom;
    int m_dpi;
    QRectF m_rect;
    QRectF m_border;
    CuteReport::PageInterface::Orientation m_orientation;

friend class Page;
};

SUIT_END_NAMESPACE

#endif
