/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** Commercial Usage
**
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://qt.nokia.com/contact.
**
**************************************************************************/

#ifndef FANCYTABWIDGET_H
#define FANCYTABWIDGET_H

#include <QIcon>
#include <QPropertyAnimation>
#include <QProxyStyle>
#include <QTabBar>
#include <QTimer>
#include <QWidget>
#include "designer_globals.h"


class QActionGroup;
class QMenu;
class QPainter;
class QSignalMapper;
class QStackedLayout;
class QStatusBar;
class QVBoxLayout;

namespace Core {
namespace Internal {

class DESIGNER_EXPORTS FancyTabProxyStyle : public QProxyStyle {
    Q_OBJECT

public:
    void drawControl(ControlElement element, const QStyleOption* option,
                     QPainter* painter, const QWidget* widget) const;
    void polish(QWidget* widget);
    void polish(QApplication* app);
    void polish(QPalette& palette);

protected:
    bool eventFilter(QObject* o, QEvent* e);
};


class DESIGNER_EXPORTS TabBar : public QTabBar
{
   Q_OBJECT
public:
    explicit TabBar(QWidget* parent=0) : QTabBar(parent) {}

signals:
    void tabDoubleClicked(int);

private:
    virtual void mouseDoubleClickEvent ( QMouseEvent * e );
};


class DESIGNER_EXPORTS FancyTab : public QWidget{
    Q_OBJECT

    Q_PROPERTY(float fader READ fader WRITE setFader)
public:
    FancyTab(QWidget *tabbar);
    float fader() { return m_fader; }
    void setFader(float value);

    QSize sizeHint() const;

    void fadeIn();
    void fadeOut();

    QIcon icon;
    QString text;
signals:
    void doubleClicked();

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mouseDoubleClickEvent ( QMouseEvent * event );

private:
    QPropertyAnimation animator;
    QWidget *tabbar;
    float m_fader;
};

class DESIGNER_EXPORTS FancyTabBar : public QWidget
{
    Q_OBJECT

public:
    FancyTabBar(QWidget *parent = 0);
    ~FancyTabBar();

    void paintEvent(QPaintEvent *event);
    void paintTab(QPainter *painter, int tabIndex) const;
    void mousePressEvent(QMouseEvent *);
    void mouseDoubleClickEvent ( QMouseEvent * e );
    bool validIndex(int index) const { return index >= 0 && index < m_tabs.count(); }

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    void addTab(const QIcon &icon, const QString &label);
    void addSpacer(int size = 40,  QSizePolicy::Policy vPolicy = QSizePolicy::Fixed);
    void removeTab(int index) {
        FancyTab *tab = m_tabs.takeAt(index);
        delete tab;
    }
    void removeAllTabs();
    void setCurrentIndex(int index);
    int currentIndex() const { return m_currentIndex; }
    QString currentText() const;

    void setTabToolTip(int index, const QString& toolTip);
    QString tabToolTip(int index) const;

    QIcon tabIcon(int index) const {return m_tabs.at(index)->icon; }
    QString tabText(int index) const { return m_tabs.at(index)->text; }
    void setTabText(const QString & text, int index);
    int count() const {return m_tabs.count(); }
    QRect tabRect(int index) const;

signals:
    void currentChanged(int);
    void tabDoubleClicked(int);

public slots:
    void emitCurrentIndex();

private:
    static const int m_rounding;
    static const int m_textPadding;
    int m_currentIndex;
    QList<FancyTab*> m_tabs;
    QTimer m_triggerTimer;
    QSize tabSizeHint(bool minimum = false) const;

};

class DESIGNER_EXPORTS FancyTabWidget : public QWidget {
    Q_OBJECT

public:
    FancyTabWidget(QWidget* parent = 0);

    // Values are persisted - only add to the end
    enum Mode {
        Mode_None = 0,

        Mode_LargeSidebar = 1,
        Mode_SmallSidebar = 2,
        Mode_Tabs = 3,
        Mode_IconOnlyTabs = 4,
        Mode_PlainSidebar = 5
    };

    struct Item {
        Item(const QIcon& icon, const QString& label)
            : type_(Type_Tab), tab_label_(label), tab_icon_(icon), spacer_size_(0) {}
        Item(int size, QSizePolicy::Policy vPolicy) : type_(Type_Spacer), spacer_size_(size), vPolicy_(vPolicy) {}

        enum Type {
            Type_Tab,
            Type_Spacer
        };

        Type type_;
        QString tab_label_;
        QIcon tab_icon_;
        int spacer_size_;
        QSizePolicy::Policy vPolicy_;
    };

    void AddTab(QWidget *tab, const QIcon &icon, const QString &label);
    void AddSpacer(int size = 40, QSizePolicy::Policy vPolicy = QSizePolicy::Fixed);
    void SetBackgroundPixmap(const QPixmap& pixmap);

    void  removeTab(const QString &label); // releases tab widget without destroying it
    void  removeTab(int index);
    void  removeAllTabs();                 // releases tab widgets without destroying it
    void  deleteTab(const QString &label); // destroys tab widget
    void  deleteTab(int index);
    void  deleteAllTabs();                 // destroys tab widgets
    int   tabsCount();

    void AddBottomWidget(QWidget* widget);

    int current_index() const;
    QString current_text() const;
    Mode mode() const { return mode_; }

    void changeText(const QString & text, int index);
    QString tabText(int index);

public slots:
    void SetCurrentIndex(int index);
    void SetMode(Mode mode);
    void SetMode(int mode) { SetMode(Mode(mode)); }
    void updateState();

signals:
    void CurrentChanged(int index);
    void tabDoubleClicked(int);
    void ModeChanged(FancyTabWidget::Mode mode);

protected:
    void paintEvent(QPaintEvent *event);
    void contextMenuEvent(QContextMenuEvent* e);

private slots:
    void ShowWidget(int index);

private:
    void MakeTabBar(QTabBar::Shape shape, bool text, bool icons, bool fancy);
    void AddMenuItem(QSignalMapper* mapper, QActionGroup* group,
                     const QString& text, Mode mode);

    Mode mode_;
    QList<Item> items_;
    int current_index_;

    QWidget* tab_bar_;
    QStackedLayout * stack_;
    QPixmap background_pixmap_;
    QWidget* side_widget_;
    QVBoxLayout* side_layout_;
    QVBoxLayout* top_layout_;

    bool use_background_;

    QMenu* menu_;

    QScopedPointer<FancyTabProxyStyle> proxy_style_;
};


class DESIGNER_EXPORTS FancyTabContainer : public QWidget
{
    Q_OBJECT

public:
    FancyTabContainer(QWidget* parent = 0);

    // Values are persisted - only add to the end
    enum Mode {
        Mode_None = 0,

        Mode_LargeSidebar = 1,
        Mode_SmallSidebar = 2,
        Mode_Tabs = 3,
        Mode_IconOnlyTabs = 4,
        Mode_PlainSidebar = 5
    };

    struct Item {
        Item(const QIcon& icon, const QString& label)
            : type(Type_Tab), tabLabel(label), tabIcon(icon), spacerSize(0) {}
        Item(int size, QSizePolicy::Policy vPolicy) : type(Type_Spacer), spacerSize(size), vPolicy(vPolicy) {}

        enum Type {
            Type_Tab,
            Type_Spacer
        };

        Type type;
        QString tabLabel;
        QIcon tabIcon;
        int spacerSize;
        QSizePolicy::Policy vPolicy;
    };

    void setSingleWidget(QWidget * widget);

    void addTab(const QIcon& icon, const QString& label); //for singleWidgetMode
    void addTab(QWidget *tab, const QIcon &icon, const QString &label);
    void addSpacer(int size = 40, QSizePolicy::Policy vPolicy = QSizePolicy::Fixed);
    void setBackgroundPixmap(const QPixmap& pixmap);

    void  removeTab(const QString &label, bool deleteChildWidget = false);
    void  removeAllTabs(bool deleteChildrenWidgets = false);
    int   count();

    void addBottomWidget(QWidget* widget);

    int currentIndex() const;
    QString currentText() const;
    Mode mode() const { return m_mode; }

    void changeText(const QString & text, int index);
    QString tabText(int index);

public slots:
    void setCurrentIndex(int index);
    void setMode(Mode mode);
    void setMode(int mode) { setMode(Mode(mode)); }
    void updateState();

signals:
    void currentChanged(int index);
    void tabDoubleClicked(int);
    void modeChanged(FancyTabContainer::Mode mode);

protected:
    void paintEvent(QPaintEvent *event);
    void contextMenuEvent(QContextMenuEvent* e);

private slots:
    void showWidget(int index);

private:
    void makeTabBar(QTabBar::Shape shape, bool text, bool icons, bool fancy);
    void addMenuItem(QSignalMapper* mapper, QActionGroup* group,
                     const QString& text, Mode mode);

    Mode m_mode;
    QList<Item> m_items;
    int m_currentIndex;

    QWidget* m_tabBar;
    QStackedLayout * m_stack;
    QPixmap m_backgroundPixmap;
    QWidget* m_sideWidget;
    QVBoxLayout* m_sideLayout;
    QVBoxLayout* m_topLayout;

    bool m_useBackground;
    bool m_singleWidgetMode;

    QMenu* m_menu;

    QScopedPointer<FancyTabProxyStyle> m_proxyStyle;
};





} // namespace Internal
} // namespace Core

Q_DECLARE_METATYPE(QPropertyAnimation*)

using Core::Internal::FancyTab;
using Core::Internal::FancyTabBar;
using Core::Internal::FancyTabWidget;
using Core::Internal::FancyTabContainer;

#endif // FANCYTABWIDGET_H
