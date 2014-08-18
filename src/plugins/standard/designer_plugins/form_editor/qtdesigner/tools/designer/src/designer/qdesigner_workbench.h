/***************************************************************************
 *   This file is part of the CuteReport project                           *
 *   Copyright (C) 2012 by Alexander Mikhalov                              *
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

/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Designer of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QDESIGNER_WORKBENCH_H
#define QDESIGNER_WORKBENCH_H

#include "designer_enums.h"

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QSet>
#include <QtCore/QList>
#include <QtCore/QRect>
#include <QPointer>

QT_BEGIN_NAMESPACE

class QDesignerActions;
class QDesignerToolWindow;
class QDesignerFormWindow;
class DockedMainWindow;
class QDesignerSettings;

class QAction;
class QActionGroup;
class QDockWidget;
class QMenu;
class QMenuBar;
class QMainWindow;
class QToolBar;
class QMdiArea;
class QMdiSubWindow;
class QCloseEvent;
class QFont;
class QtToolBarManager;
class ToolBarManager;

class QDesignerFormEditorInterface;
class QDesignerFormWindowInterface;
class QDesignerFormWindowManagerInterface;

namespace qdesigner_internal {
class QDesignerIntegration;
class PreviewManager;
}

class QDesigner;
class MainWindowBase;
class DesignerRootWidget;
class QSettings;

class QDesignerWorkbench: public QObject
{
    Q_OBJECT

public:
    enum StandardToolWindow { WidgetBox,  ObjectInspector, PropertyEditor,
                              ResourceEditor, ActionEditor, SignalSlotEditor,
                              StandardToolWindowCount };
    QDesignerWorkbench(QObject * parent, QSettings * s);
    virtual ~QDesignerWorkbench();

    QWidget * widget() const;

    UIMode mode() const;

    QDesignerFormEditorInterface *core() const;
    QDesignerFormWindow *findFormWindow(QWidget *widget) const;

    QDesignerFormWindow *openForm(const QString &fileName, const QString &data, QString *errorMessage);
    QDesignerFormWindow *openForm(const QString &fileName, QIODevice *dev, QString *errorMessage);
    QDesignerFormWindow *openForm(const QString &fileName, QString *errorMessage);

    QDesignerFormWindow *openTemplate(const QString &templateFileName,
                                      const QString &editorFileName,
                                      QString *errorMessage);

    int toolWindowCount() const;
    QWidget *toolWindow(int index) const;

    int formWindowCount() const;
    QDesignerFormWindow *formWindow(int index) const;
    QDesignerFormWindow *activeFormWindow() const;

    QDesignerActions *actionManager() const;

//    QActionGroup *modeActionGroup() const;

//    QRect availableGeometry() const;
//    QRect desktopGeometry() const;

    int marginHint() const;

    // open form
    bool readInForm(const QString &fileName) const;
    bool writeOutForm(QDesignerFormWindowInterface *formWindow, const QString &fileName) const;
    bool saveForm(QDesignerFormWindowInterface *fw);
    bool handleClose();
    bool readInBackup();
    void updateBackup(QDesignerFormWindowInterface* fwi);
    void closeCurrentForm();

    void reloadSettings();
    void saveSettings() const;

    void closePreview();

signals:
    void modeChanged(UIMode mode);
    void initialized();

public slots:
    void addFormWindow(QDesignerFormWindow *formWindow);
    void removeFormWindow(QDesignerFormWindow *formWindow);
    void toggleFormMinimizationState();
    void emitMessage(const QObject *object, const QString & message, int showTime);

private slots:
    void showNewFormOnStartupChecking();
    void switchToNeutralMode();
    void switchToDockedMode();
    void initializeCorePlugins();
    void slotFormWindowActivated(QDesignerFormWindow* fw);
    void updateWindowMenu(QDesignerFormWindowInterface *fw);
    void formWindowActionTriggered(QAction *a);
    void adjustMDIFormPositions();
    void minimizationStateChanged(QDesignerFormWindowInterface *formWindow, bool minimized);

    void slotFileDropped(const QString &f);
//    void callCreateForm();

private:
    void initTools();
    void createGlobalMenu();
    QWidget *magicalParent(const QWidget *w) const;
    Qt::WindowFlags magicalWindowFlags(const QWidget *widgetForFlags) const;
    QDesignerFormWindowManagerInterface *formWindowManager() const;
//    void closeAllToolWindows();
    QWidget *widgetBoxToolWindow() const;
    QDesignerFormWindow *loadForm(const QString &fileName, QIODevice *iodevice, bool detectLineTermiantorMode, bool *uic3Converted, QString *errorMessage);
    void resizeForm(QDesignerFormWindow *fw,  const QWidget *mainContainer) const;
    void saveGeometriesForModeChange();
    void saveGeometries() const;

    bool isFormWindowMinimized(const QDesignerFormWindow *fw);
    void setFormWindowMinimized(QDesignerFormWindow *fw, bool minimized);

    QDesignerFormEditorInterface *m_core;
    qdesigner_internal::QDesignerIntegration *m_integration;

    QDesignerActions *m_actionManager;
    QActionGroup *m_windowActions;  // switching between windows if many

    QMenu *m_windowMenu;

    QMenuBar *m_globalMenuBar;

    struct TopLevelData {
        ToolBarManager *toolbarManager;
        QList<QToolBar *> toolbars;
    };
    TopLevelData m_topLevelData;

    UIMode m_mode;

    QList<QPointer<QWidget> > m_toolWindows;
    QList<QDesignerFormWindow*> m_formWindows;

    QMenu *m_toolbarMenu;  // menu action for configure all main windows toolbar

    // Helper class to remember the position of a window while switching user
    // interface modes.
    class Position {
    public:
        Position(const QDockWidget *dockWidget);
        Position(const QMdiSubWindow *mdiSubWindow, const QPoint &mdiAreaOffset);

        void applyTo(QMdiSubWindow *mdiSubWindow, const QPoint &mdiAreaOffset) const;
        void applyTo(QDockWidget *dockWidget) const;

        QPoint position() const { return m_position; }
    private:
        bool m_minimized;
        // Position referring to top-left corner (desktop in top-level mode or
        // main window in MDI mode)
        QPoint m_position;
    };
    typedef  QHash<QWidget*, Position> PositionMap;
    PositionMap m_Positions;

    enum State { StateInitializing, StateUp, StateClosing };
    State m_state;

    QPointer<DesignerRootWidget> m_rootWidget;
    bool m_suppressNewFormShow;
    qdesigner_internal::PreviewManager * m_previewManager;
    QSettings * m_appSettings;
};

QT_END_NAMESPACE

#endif // QDESIGNER_WORKBENCH_H
