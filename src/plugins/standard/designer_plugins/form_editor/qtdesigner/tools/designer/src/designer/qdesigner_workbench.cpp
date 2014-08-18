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

#include "qdesigner_workbench.h"
#include "qdesigner_actions.h"
#include "qdesigner_settings.h"
#include "qdesigner_formwindow.h"

#include <QtDesigner/private/qdesigner_integration_p.h>
#include <QtDesigner/private/pluginmanager_p.h>
#include <QtDesigner/private/formwindowbase_p.h>
#include <qdesigner_formwindowmanager_p.h>
#include <previewmanager_p.h>

#include <QtDesigner/QDesignerComponents>
#include <QtDesigner>
#include <QtCore>
#include <QtGui>

#include "designerrootwidget.h"

QT_BEGIN_NAMESPACE

typedef QList<QAction *> ActionList;

static QMdiSubWindow *mdiSubWindowOf(const QWidget *w)
{
    QMdiSubWindow *rc = qobject_cast<QMdiSubWindow *>(w->parentWidget());
    Q_ASSERT(rc);
    return rc;
}

static QDockWidget *dockWidgetOf(const QWidget *w)
{
    for (QWidget *parentWidget = w->parentWidget(); parentWidget ; parentWidget = parentWidget->parentWidget()) {
        if (QDockWidget *dw = qobject_cast<QDockWidget *>(parentWidget)) {
            return dw;
        }
    }
    Q_ASSERT("Dock widget not found");
    return 0;
}

// ------------ QDesignerWorkbench::Position
QDesignerWorkbench::Position::Position(const QMdiSubWindow *mdiSubWindow, const QPoint &mdiAreaOffset) :
    m_minimized(mdiSubWindow->isShaded()),
    m_position(mdiSubWindow->pos() + mdiAreaOffset)
{
}

QDesignerWorkbench::Position::Position(const QDockWidget *dockWidget) :
    m_minimized(dockWidget->isMinimized()),
    m_position(dockWidget->pos())
{
}


void QDesignerWorkbench::Position::applyTo(QMdiSubWindow *mdiSubWindow,
                                           const QPoint &mdiAreaOffset) const
{
    // QMdiSubWindow attempts to resize its children to sizeHint() when switching user interface modes.
    // Restore old size
    const QPoint mdiAreaPos =  QPoint(qMax(0, m_position.x() - mdiAreaOffset.x()),
                                      qMax(0, m_position.y() - mdiAreaOffset.y()));
    mdiSubWindow->move(mdiAreaPos);
    const QSize decorationSize = mdiSubWindow->size() - mdiSubWindow->contentsRect().size();
    mdiSubWindow->resize(mdiSubWindow->widget()->size() + decorationSize);
    mdiSubWindow->show();
    if (m_minimized) {
        mdiSubWindow->showShaded();
    }
}


void QDesignerWorkbench::Position::applyTo(QDockWidget *dockWidget) const
{
    dockWidget->widget()->setVisible(true);
    dockWidget->setVisible(!m_minimized);
}

static inline void addActionsToMenu(QMenu *m, const ActionList &al)
{
    const ActionList::const_iterator cend = al.constEnd();
    for (ActionList::const_iterator it = al.constBegin(); it != cend; ++it)
        m->addAction(*it);
}

static inline QMenu *addMenu(QMenuBar *mb, const QString &title, const ActionList &al)
{
    QMenu *rc = mb->addMenu(title);
    addActionsToMenu(rc, al);
    return rc;
}

// -------- QDesignerWorkbench

QDesignerWorkbench::QDesignerWorkbench(QObject *parent, QSettings *s)  :
    QObject(parent),
    m_windowActions(new QActionGroup(this)),
    m_globalMenuBar(0),
    m_mode(NeutralMode),
    m_windowMenu(0),
    m_toolbarMenu(0),
    m_state(StateInitializing),
    m_rootWidget(new DesignerRootWidget(0)),
    m_appSettings(s)
{
    m_core = QDesignerComponents::createFormEditor(this);
    QDesignerComponents::initializeResources();
    m_core = QDesignerComponents::createFormEditor(this);
//    QDesignerSettings settings(m_core);

    (void) QDesignerComponents::createTaskMenu(core(), this);

    initializeCorePlugins();
    QDesignerComponents::initializePlugins(core());
    m_actionManager = new QDesignerActions(this); // accesses plugin components

    m_windowActions->setExclusive(true);
    connect(m_windowActions, SIGNAL(triggered(QAction*)), this, SLOT(formWindowActionTriggered(QAction*)));

    initTools();

//    createGlobalMenu();

    // Integration
    m_integration = new qdesigner_internal::QDesignerIntegration(m_core, this);
    connect(m_integration, SIGNAL(helpRequested(QString,QString)), m_actionManager, SLOT(helpRequested(QString,QString)));

    emit initialized();

    connect(m_core->formWindowManager(), SIGNAL(activeFormWindowChanged(QDesignerFormWindowInterface*)),
                this, SLOT(updateWindowMenu(QDesignerFormWindowInterface*)));

    switchToDockedMode();

    m_rootWidget->addWidgetBar(m_toolWindows.at(QDesignerWorkbench::WidgetBox));
    m_rootWidget->addObjectInspector(m_toolWindows.at(QDesignerWorkbench::ObjectInspector));
    m_rootWidget->addPropertyEditor(m_toolWindows.at(QDesignerWorkbench::PropertyEditor));
    m_rootWidget->addActionEditor(m_toolWindows.at(QDesignerWorkbench::ActionEditor));
    m_rootWidget->addSignalSlotEditor(m_toolWindows.at(QDesignerWorkbench::SignalSlotEditor));
    m_rootWidget->addResourceEditor(m_toolWindows.at(QDesignerWorkbench::ResourceEditor));

//    showNewFormOnStartupChecking();    reloadSettings();

    qdesigner_internal::QDesignerFormWindowManager *ifwm = qobject_cast<qdesigner_internal::QDesignerFormWindowManager *>(m_core->formWindowManager());
    Q_ASSERT(ifwm);
    m_previewManager = ifwm->previewManager();

    m_state = StateUp;
}


QDesignerWorkbench::~QDesignerWorkbench()
{
    qDeleteAll(m_toolWindows);
    delete m_rootWidget;
}

void QDesignerWorkbench::initTools()
{
    QDesignerWidgetBoxInterface *widgetBox = QDesignerComponents::createWidgetBox(m_core, m_rootWidget);
    m_toolWindows.push_back(widgetBox);
    m_core->setWidgetBox(widgetBox);

    QDesignerObjectInspectorInterface *objectInspector = QDesignerComponents::createObjectInspector(m_core, m_rootWidget);
    m_toolWindows.push_back(objectInspector);
    m_core->setObjectInspector(objectInspector);

    QDesignerPropertyEditorInterface *pe = QDesignerComponents::createPropertyEditor(m_core, m_rootWidget);
    m_toolWindows.push_back(pe);
    m_core->setPropertyEditor(pe);

    QWidget * wd = QDesignerComponents::createResourceEditor(m_core,  m_rootWidget);
    m_toolWindows.push_back(wd);

    QDesignerActionEditorInterface *ae = QDesignerComponents::createActionEditor(m_core, m_rootWidget);
    m_toolWindows.push_back(ae);
    m_core->setActionEditor(ae);

    QWidget * sse = QDesignerComponents::createSignalSlotEditor(m_core, m_rootWidget);
    m_toolWindows.push_back(sse);
}


QWidget * QDesignerWorkbench::widget() const
{
    return m_rootWidget;
}


void QDesignerWorkbench::showNewFormOnStartupChecking()
{
    m_suppressNewFormShow = readInBackup();

    QStringList files;
    if (!files.empty()) {
        const QStringList::const_iterator cend = files.constEnd();
        for (QStringList::const_iterator it = files.constBegin(); it != cend; ++it) {
            // Ensure absolute paths for recent file list to be unique
            QString fileName = *it;
            const QFileInfo fi(fileName);
            if (fi.exists() && fi.isRelative())
                fileName = fi.absoluteFilePath();
            readInForm(fileName);
        }
    }
    if ( formWindowCount())
        m_suppressNewFormShow = true;

    if (!m_suppressNewFormShow && QDesignerSettings(core()).showNewFormOnStartup())
        QTimer::singleShot(100, this, SLOT(callCreateForm())); // won't show anything if suppressed
}


//void QDesignerWorkbench::callCreateForm()
//{
//    if (!m_suppressNewFormShow)
//        actionManager()->createForm();
//}


void QDesignerWorkbench::createGlobalMenu()
{
    m_globalMenuBar = new QMenuBar;

    // Build main menu bar
    addMenu(m_globalMenuBar, tr("&File"), m_actionManager->fileActions()->actions());

    QMenu *editMenu = addMenu(m_globalMenuBar, tr("&Edit"), m_actionManager->editActions()->actions());
    editMenu->addSeparator();
    addActionsToMenu(editMenu, m_actionManager->toolActions()->actions());

    QMenu *formMenu = addMenu(m_globalMenuBar, tr("F&orm"),  m_actionManager->formActions()->actions());
    QMenu *previewSubMenu = new QMenu(tr("Preview in"), formMenu);
    formMenu->insertMenu(m_actionManager->previewFormAction(), previewSubMenu);
    addActionsToMenu(previewSubMenu, m_actionManager->styleActions()->actions());

    QMenu *viewMenu = m_globalMenuBar->addMenu(tr("&View"));

    addMenu(m_globalMenuBar, tr("&Settings"), m_actionManager->settingsActions()->actions());

    m_windowMenu = addMenu(m_globalMenuBar, tr("&Window"), m_actionManager->windowActions()->actions());

    addMenu(m_globalMenuBar, tr("&Help"), m_actionManager->helpActions()->actions());

    //  Add the tools in view menu order
    QActionGroup *viewActions = new QActionGroup(this);
    viewActions->setExclusive(false);

//    for (int i = 0; i < QDesignerToolWindow::StandardToolWindowCount; i++) {
//        if (QAction *action = m_toolWindows.at(i)->action()) {
//            viewMenu->addAction(action);
//            viewActions->addAction(action);
//        }
//        // The widget box becomes the main window in top level mode
//        if (i == QDesignerToolWindow::WidgetBox)
//            connect(m_toolWindows.at(i), SIGNAL(closeEventReceived(QCloseEvent*)), this, SLOT(handleCloseEvent(QCloseEvent*)));
//    }

    // remaining view options (config toolbars)
    viewMenu->addSeparator();
    m_toolbarMenu = viewMenu->addMenu(tr("Toolbars"));
}

void QDesignerWorkbench::saveGeometriesForModeChange()
{
    m_Positions.clear();
    switch (m_mode) {
    case NeutralMode:
        break;
    case DockedMode: {
        const QPoint mdiAreaOffset = m_rootWidget->mdiArea()->pos();
        foreach (QWidget *tw, m_toolWindows) {
            m_Positions.insert(tw, Position(dockWidgetOf(tw)));
        }
        foreach (QDesignerFormWindow *fw, m_formWindows) {
            m_Positions.insert(fw, Position(mdiSubWindowOf(fw), mdiAreaOffset));
        }
    }
        break;
    }
}



UIMode QDesignerWorkbench::mode() const
{
    return m_mode;
}

void QDesignerWorkbench::addFormWindow(QDesignerFormWindow *formWindow)
{
    // ### Q_ASSERT(formWindow->windowTitle().isEmpty() == false);

    m_formWindows.append(formWindow);


    m_actionManager->setWindowListSeparatorVisible(true);

    if (QAction *action = formWindow->action()) {
        if (m_windowActions)
            m_windowActions->addAction(action);
        if (m_windowMenu)
            m_windowMenu->addAction(action);
        action->setChecked(true);
    }

    m_actionManager->minimizeAction()->setEnabled(true);
    m_actionManager->minimizeAction()->setChecked(false);
    connect(formWindow, SIGNAL(minimizationStateChanged(QDesignerFormWindowInterface*,bool)),
            this, SLOT(minimizationStateChanged(QDesignerFormWindowInterface*,bool)));

    m_actionManager->editWidgets()->trigger();
}

Qt::WindowFlags QDesignerWorkbench::magicalWindowFlags(const QWidget * /*widgetForFlags*/) const
{
    switch (m_mode) {
        case DockedMode:
            return Qt::Window | Qt::WindowShadeButtonHint | Qt::WindowSystemMenuHint | Qt::WindowTitleHint;
        case NeutralMode:
            return Qt::Window;
        default:
            Q_ASSERT(0);
            return 0;
    }
}

QWidget *QDesignerWorkbench::magicalParent(const QWidget * /*w*/) const
{
    switch (m_mode) {
        case DockedMode:
            return m_rootWidget->mdiArea();
        case NeutralMode:
            return 0;
        default:
            Q_ASSERT(0);
            return 0;
    }
}

void QDesignerWorkbench::switchToNeutralMode()
{
    QDesignerSettings settings(m_core);
//    saveGeometries(settings);
    saveGeometriesForModeChange();

    m_mode = NeutralMode;

//    foreach (QWidget *tw, m_toolWindows) {
//        tw->setCloseEventPolicy(MainWindowBase::AcceptCloseEvents);
//        tw->setParent(0);
//    }

    foreach (QDesignerFormWindow *fw, m_formWindows) {
        fw->setParent(0);
        fw->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    }

#ifndef Q_WS_MAC
    if (m_globalMenuBar)
        m_globalMenuBar->setParent(0);
#endif

    m_core->setTopLevel(0);
//    m_designer->setMainWindow(0);

//    delete m_dockedMainWindow;
//    m_dockedMainWindow = 0;
}

void QDesignerWorkbench::switchToDockedMode()
{
    if (m_mode == DockedMode)
        return;

    switchToNeutralMode();

#ifdef Q_WS_X11
//    QApplication::setAttribute(Qt::AA_DontUseNativeMenuBar, false);
#endif
#ifndef Q_WS_MAC
//    QWidget *widgetBoxWrapper = widgetBoxToolWindow();
//    widgetBoxWrapper->action()->setVisible(true);
//    widgetBoxWrapper->setWindowTitle(tr("Widget Box"));
#endif

    m_mode = DockedMode;
    const QDesignerSettings settings(m_core);
//    m_dockedMainWindow = new DockedMainWindow(this, m_toolbarMenu, m_toolWindows);
//    m_dockedMainWindow->setParent(m_rootWidget);
//    m_rootWidget->layout()->addWidget(m_dockedMainWindow);
//    m_dockedMainWindow->setUnifiedTitleAndToolBarOnMac(true);
//    m_dockedMainWindow->setCloseEventPolicy(MainWindowBase::EmitCloseEventSignal);
//    connect(m_dockedMainWindow, SIGNAL(closeEventReceived(QCloseEvent*)), this, SLOT(handleCloseEvent(QCloseEvent*)));
//    connect(m_dockedMainWindow, SIGNAL(fileDropped(QString)), this, SLOT(slotFileDropped(QString)));
//    connect(m_dockedMainWindow, SIGNAL(formWindowActivated(QDesignerFormWindow*)), this, SLOT(slotFormWindowActivated(QDesignerFormWindow*)));
//    m_dockedMainWindow->restoreSettings(settings, m_dockedMainWindow->addToolWindows(m_toolWindows), desktopGeometry());

//    m_core->setTopLevel(m_dockedMainWindow);

    m_core->setTopLevel(m_rootWidget);

#ifndef Q_WS_MAC
    if (m_globalMenuBar) {
        m_rootWidget->setMenuBar(m_globalMenuBar);
        m_globalMenuBar->show();
    }
#endif
//    m_designer->setMainWindow(m_dockedMainWindow);

    foreach (QDesignerFormWindow *fw, m_formWindows) {
        QMdiSubWindow *subwin = m_rootWidget->createMdiSubWindow(fw, magicalWindowFlags(fw),
                                                                       m_actionManager->closeFormAction()->shortcut());
        subwin->hide();
        if (QWidget *mainContainer = fw->editor()->mainContainer())
            resizeForm(fw, mainContainer);
    }

//    m_actionManager->setBringAllToFrontVisible(false);
    m_rootWidget->show();
    // Trigger adjustMDIFormPositions() delayed as viewport size is not yet known.

    if (m_state != StateInitializing)
        QMetaObject::invokeMethod(this, "adjustMDIFormPositions", Qt::QueuedConnection);
}

void QDesignerWorkbench::adjustMDIFormPositions()
{
    const QPoint mdiAreaOffset = m_rootWidget->mdiArea()->pos();

    foreach (QDesignerFormWindow *fw, m_formWindows) {
        const PositionMap::const_iterator pit = m_Positions.constFind(fw);
        if (pit != m_Positions.constEnd())
            pit->applyTo(mdiSubWindowOf(fw), mdiAreaOffset);
    }
}


QDesignerFormWindowManagerInterface *QDesignerWorkbench::formWindowManager() const
{
    return m_core->formWindowManager();
}

QDesignerFormEditorInterface *QDesignerWorkbench::core() const
{
    return m_core;
}

int QDesignerWorkbench::toolWindowCount() const
{
    return m_toolWindows.count();
}

QWidget *QDesignerWorkbench::toolWindow(int index) const
{
    return m_toolWindows.at(index);
}

int QDesignerWorkbench::formWindowCount() const
{
    //return m_formWindows.count();
    return core()->formWindowManager()->formWindowCount();
}

QDesignerFormWindow *QDesignerWorkbench::formWindow(int index) const
{
//    return m_formWindows.at(index);
    return findFormWindow(core()->formWindowManager()->formWindow(index));
}

//QRect QDesignerWorkbench::desktopGeometry() const
//{
//    // Return geometry of the desktop designer is running in.
//    QWidget *widget = 0;
//    switch (m_mode) {
//    case DockedMode:
//        widget = m_rootWidget;
//        break;
//    case NeutralMode:
//        break;
//    }
//    const QDesktopWidget *desktop = qApp->desktop();
//    const int screenNumber = widget ? desktop->screenNumber(widget) : 0;
//    return desktop->availableGeometry(screenNumber);
//}

//QRect QDesignerWorkbench::availableGeometry() const
//{
//    if (m_mode == DockedMode)
//        return m_rootWidget->mdiArea()->geometry();

//    const QDesktopWidget *desktop = QApplication::desktop();
//    return desktop->availableGeometry(desktop->screenNumber(widgetBoxToolWindow()));
//}

int QDesignerWorkbench::marginHint() const
{    return 20;
}

void QDesignerWorkbench::slotFormWindowActivated(QDesignerFormWindow* fw)
{
    core()->formWindowManager()->setActiveFormWindow(fw->editor());
}

void QDesignerWorkbench::removeFormWindow(QDesignerFormWindow *formWindow)
{
    QDesignerFormWindowInterface *editor = formWindow->editor();
//    const bool loadOk = editor->mainContainer();
    updateBackup(editor);
    const int index = m_formWindows.indexOf(formWindow);
    if (index != -1) {
        m_formWindows.removeAt(index);
    }

    if (QAction *action = formWindow->action()) {
        if (m_windowActions)
            m_windowActions->removeAction(action);
        if (m_windowMenu)
            m_windowMenu->removeAction(action);
    }

    if (m_formWindows.empty()) {
        m_actionManager->setWindowListSeparatorVisible(false);
        // Show up new form dialog unless closing
//        if (loadOk && m_state == StateUp
//            && QDesignerSettings(m_core).showNewFormOnStartup()) {
//            QTimer::singleShot(200, m_actionManager, SLOT(createForm()));
//        }
    }
}


void QDesignerWorkbench::closePreview()
{
    m_previewManager->closeAllPreviews();
}


void QDesignerWorkbench::initializeCorePlugins()
{
    QList<QObject*> plugins = QPluginLoader::staticInstances();
    plugins += core()->pluginManager()->instances();

    foreach (QObject *plugin, plugins) {
        if (QDesignerFormEditorPluginInterface *formEditorPlugin = qobject_cast<QDesignerFormEditorPluginInterface*>(plugin)) {
            if (!formEditorPlugin->isInitialized())
                formEditorPlugin->initialize(core());
        }
    }
}

void QDesignerWorkbench::saveSettings() const
{
    QDesignerSettings settings(m_core);
    settings.clearBackup();
    saveGeometries();
}

void QDesignerWorkbench::saveGeometries() const
{
    m_rootWidget->saveSettings(m_appSettings);
}

void QDesignerWorkbench::slotFileDropped(const QString &f)
{
    readInForm(f);
}

bool QDesignerWorkbench::readInForm(const QString &fileName) const
{
    return m_actionManager->readInForm(fileName);
}

bool QDesignerWorkbench::writeOutForm(QDesignerFormWindowInterface *formWindow, const QString &fileName) const
{
    return m_actionManager->writeOutForm(formWindow, fileName);
}

bool QDesignerWorkbench::saveForm(QDesignerFormWindowInterface *frm)
{
    return m_actionManager->saveForm(frm);
}

QDesignerFormWindow *QDesignerWorkbench::findFormWindow(QWidget *widget) const
{
    foreach (QDesignerFormWindow *formWindow, m_formWindows) {
        if (formWindow->editor() == widget)
            return formWindow;
    }

    return 0;
}

QDesignerFormWindow * QDesignerWorkbench::activeFormWindow() const
{
    return findFormWindow(core()->formWindowManager()->activeFormWindow());
}


void QDesignerWorkbench::closeCurrentForm()
{
    if (m_previewManager->previewCount()) {
        closePreview();
        return;
    }

    bool closed = false;
    QDesignerFormWindowInterface *fw = 0;

    if ((fw = core()->formWindowManager()->activeFormWindow())) {
        if (QWidget *parent = fw->parentWidget()) {
            if (QMdiSubWindow *mdiSubWindow = qobject_cast<QMdiSubWindow *>(parent->parentWidget())) {
                closed = mdiSubWindow->close();
            } else {
                closed = parent->close();
            }
        }
    }
}


bool QDesignerWorkbench::handleClose()
{
    m_state = StateClosing;
    QList<QDesignerFormWindow *> dirtyForms;
    foreach (QDesignerFormWindow *w, m_formWindows) {
        if (w->editor()->isDirty())
            dirtyForms << w;
    }

    if (dirtyForms.size()) {
        if (dirtyForms.size() == 1) {
            if (!dirtyForms.at(0)->close()) {
                m_state = StateUp;
                return false;
            }
        } else {
            int count = dirtyForms.size();
            QMessageBox box(QMessageBox::Warning, tr("Save Forms?"),
                    tr("There are %n forms with unsaved changes."
                        " Do you want to review these changes before quitting?", "", count),
                    QMessageBox::Cancel | QMessageBox::Discard | QMessageBox::Save);
            box.setInformativeText(tr("If you do not review your documents, all your changes will be lost."));
            box.button(QMessageBox::Discard)->setText(tr("Discard Changes"));
            QPushButton *save = static_cast<QPushButton *>(box.button(QMessageBox::Save));
            save->setText(tr("Review Changes"));
            box.setDefaultButton(save);
            switch (box.exec()) {
            case QMessageBox::Cancel:
                m_state = StateUp;
                return false;
            case QMessageBox::Save:
               foreach (QDesignerFormWindow *fw, dirtyForms) {
                   fw->show();
                   fw->raise();
                   if (!fw->close()) {
                       m_state = StateUp;
                       return false;
                   }
               }
               break;
            case QMessageBox::Discard:
              foreach (QDesignerFormWindow *fw, dirtyForms) {
                  fw->editor()->setDirty(false);
                  fw->setWindowModified(false);
              }
              break;
            }
        }
    }

    foreach (QDesignerFormWindow *fw, m_formWindows)
        fw->close();

    saveSettings();
    return true;
}

QDesignerActions *QDesignerWorkbench::actionManager() const
{
    return m_actionManager;
}

void QDesignerWorkbench::updateWindowMenu(QDesignerFormWindowInterface *fwi)
{
    bool minimizeChecked = false;
    bool minimizeEnabled = false;
    QDesignerFormWindow *activeFormWindow = 0;
    do {
        if (!fwi)
        break;
        activeFormWindow = qobject_cast<QDesignerFormWindow *>(fwi->parentWidget());
        if (!activeFormWindow)
            break;

        minimizeEnabled = true;
        minimizeChecked = isFormWindowMinimized(activeFormWindow);
    } while (false) ;

    m_actionManager->minimizeAction()->setEnabled(minimizeEnabled);
    m_actionManager->minimizeAction()->setChecked(minimizeChecked);

    if (!m_formWindows.empty()) {
        const QList<QDesignerFormWindow*>::const_iterator cend = m_formWindows.constEnd();
        for (QList<QDesignerFormWindow*>::const_iterator it = m_formWindows.constBegin(); it != cend; ++it)
            (*it)->action()->setChecked(*it == activeFormWindow);
    }
}

void QDesignerWorkbench::formWindowActionTriggered(QAction *a)
{
    QDesignerFormWindow *fw = qobject_cast<QDesignerFormWindow *>(a->parentWidget());
    Q_ASSERT(fw);

    if (isFormWindowMinimized(fw))
        setFormWindowMinimized(fw, false);

    if (m_mode == DockedMode) {
        if (QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *>(fw->parent())) {
            m_rootWidget->mdiArea()->setActiveSubWindow(subWindow);
        }
    } else {
        fw->activateWindow();
        fw->raise();
    }
}


bool QDesignerWorkbench::readInBackup()
{
    const QMap<QString, QString> backupFileMap = QDesignerSettings(m_core).backup();
    if (backupFileMap.isEmpty())
        return false;

    const  QMessageBox::StandardButton answer =
        QMessageBox::question(0, tr("Backup Information"),
                                 tr("The last session of Designer was not terminated correctly. "
                                       "Backup files were left behind. Do you want to load them?"),
                                    QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);
    if (answer == QMessageBox::No)
        return false;

    const QString modifiedPlaceHolder = QLatin1String("[*]");
    QMapIterator<QString, QString> it(backupFileMap);
    while(it.hasNext()) {
        it.next();

        QString fileName = it.key();
        fileName.remove(modifiedPlaceHolder);

        if(m_actionManager->readInForm(it.value()))
            formWindowManager()->activeFormWindow()->setFileName(fileName);

    }
    return true;
}

void QDesignerWorkbench::updateBackup(QDesignerFormWindowInterface* fwi)
{
    QString fwn = QDir::convertSeparators(fwi->fileName());
    if (fwn.isEmpty())
        fwn = fwi->parentWidget()->windowTitle();

    QDesignerSettings settings(m_core);
    QMap<QString, QString> map = settings.backup();
    map.remove(fwn);
    settings.setBackup(map);
}


// Resize a form window taking MDI decorations into account
// Apply maximum size as there is no layout connection between
// the form's main container and the integration's outer
// container due to the tool widget stack.

void QDesignerWorkbench::resizeForm(QDesignerFormWindow *fw, const QWidget *mainContainer) const
{
    const QSize containerSize = mainContainer->size();
    const QSize containerMaximumSize = mainContainer->maximumSize();
    if (m_mode != DockedMode) {
        fw->resize(containerSize);
        fw->setMaximumSize(containerMaximumSize);
        return;
    }
    // get decorations and resize MDI
    QMdiSubWindow *mdiSubWindow = qobject_cast<QMdiSubWindow *>(fw->parent());
    Q_ASSERT(mdiSubWindow);
    const QSize decorationSize = mdiSubWindow->geometry().size() - mdiSubWindow->contentsRect().size();
    mdiSubWindow->resize(containerSize + decorationSize);
    // In Qt::RightToLeft mode, the window can grow to be partially hidden by the right border.
    const int mdiAreaWidth = m_rootWidget->mdiArea()->width();
    if (qApp->layoutDirection() == Qt::RightToLeft && mdiSubWindow->geometry().right() >= mdiAreaWidth)
        mdiSubWindow->move(mdiAreaWidth - mdiSubWindow->width(), mdiSubWindow->pos().y());

    if (containerMaximumSize == QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX)) {
        mdiSubWindow->setMaximumSize(containerMaximumSize);
    } else {
        mdiSubWindow->setMaximumSize(containerMaximumSize + decorationSize);
    }
}


// Load a form or return 0 and do cleanup. file name and editor file
// name in case of loading a template file.

QDesignerFormWindow * QDesignerWorkbench::loadForm(const QString &fileName,
                                                   QIODevice *iodevice,
                                                   bool detectLineTermiantorMode,
                                                   bool *uic3Converted,
                                                   QString *errorMessage)
{
    qdesigner_internal::FormWindowBase::LineTerminatorMode mode = qdesigner_internal::FormWindowBase::NativeLineTerminator;

    if (detectLineTermiantorMode) {
        if (iodevice->open(QIODevice::ReadOnly)) {
            const QString text = QString::fromUtf8(iodevice->readLine());
            iodevice->close();

            const int lf = text.indexOf(QLatin1Char('\n'));
            if (lf > 0 && text.at(lf-1) == QLatin1Char('\r')) {
                mode = qdesigner_internal::FormWindowBase::CRLFLineTerminator;
            } else if (lf >= 0) {
                mode = qdesigner_internal::FormWindowBase::LFLineTerminator;
            }
        }
    }

    if (!iodevice->open(QIODevice::ReadOnly|QIODevice::Text)) {
        *errorMessage = tr("The file <b>%1</b> could not be opened.").arg(fileName);
        return 0;
    }


    // Create a form
    QDesignerFormWindowManagerInterface *formWindowManager = m_core->formWindowManager();

    QDesignerFormWindow *formWindow = new QDesignerFormWindow(/*formWindow=*/ 0, this);
    addFormWindow(formWindow);
    QDesignerFormWindowInterface *editor = formWindow->editor();
    Q_ASSERT(editor);

    // Temporarily set the file name. It is needed when converting a UIC 3 file.
    // In this case, the file name will we be cleared on return to force a save box.
    editor->setFileName(fileName);
    editor->setContents(iodevice);

    if (qdesigner_internal::FormWindowBase *fwb = qobject_cast<qdesigner_internal::FormWindowBase *>(editor))
        fwb->setLineTerminatorMode(mode);

    switch (m_mode) {
    case DockedMode: {
        // below code must be after above call to setContents(), because setContents() may popup warning dialogs which would cause
        // mdi sub window activation (because of dialogs internal call to  processEvent or such)
        // That activation could have worse consequences, e.g. NULL resource set for active form) before the form is loaded
        QMdiSubWindow *subWin = m_rootWidget->createMdiSubWindow(formWindow, magicalWindowFlags(formWindow), m_actionManager->closeFormAction()->shortcut());
        m_rootWidget->mdiArea()->setActiveSubWindow(subWin);
    }
        break;
    case NeutralMode:
        break;
    }

    if (!editor->mainContainer()) {
        removeFormWindow(formWindow);
        formWindowManager->removeFormWindow(editor);
        m_core->metaDataBase()->remove(editor);
        *errorMessage = tr("The file <b>%1</b> is not a valid Designer UI file.").arg(fileName);
        return 0;
    }
    *uic3Converted = editor->fileName().isEmpty();
    // Did user specify another (missing) resource path -> set dirty.
    const bool dirty = editor->property("_q_resourcepathchanged").toBool();
    editor->setDirty(dirty);
    resizeForm(formWindow, editor->mainContainer());
    formWindowManager->setActiveFormWindow(editor);
    return formWindow;
}


QDesignerFormWindow * QDesignerWorkbench::openForm(const QString &fileName, const QString &data, QString *errorMessage)
{
    QBuffer buffer;
    buffer.setData(data.toUtf8());

    bool uic3Converted;
    QDesignerFormWindow *rc =loadForm(fileName, &buffer, true, &uic3Converted, errorMessage);
    if (!rc)
        return 0;

    if (!uic3Converted)
        rc->editor()->setFileName(fileName);
    rc->firstShow();
    return rc;
}


QDesignerFormWindow *QDesignerWorkbench::openForm(const QString &fileName, QIODevice *dev, QString *errorMessage)
{
    bool uic3Converted;
    QDesignerFormWindow *rc =loadForm(fileName, dev, true, &uic3Converted, errorMessage);
    if (!rc)
        return 0;

    if (!uic3Converted)
        rc->editor()->setFileName(fileName);
    rc->firstShow();
    return rc;
}

QDesignerFormWindow *QDesignerWorkbench::openForm(const QString &fileName, QString *errorMessage)
{
    QFile file(fileName);
    bool uic3Converted;
    QDesignerFormWindow *rc =loadForm(fileName, &file, true, &uic3Converted, errorMessage);
    if (!rc)
        return 0;

    if (!uic3Converted)
        rc->editor()->setFileName(fileName);
    rc->firstShow();
    return rc;
}


QDesignerFormWindow * QDesignerWorkbench::openTemplate(const QString &templateFileName,
                                                       const QString &editorFileName,
                                                       QString *errorMessage)
{
    QFile file(templateFileName);
    bool uic3Converted;
    QDesignerFormWindow *rc =loadForm(templateFileName, &file, false, &uic3Converted, errorMessage);
    if (!rc)
        return 0;

    if (!uic3Converted)
        rc->editor()->setFileName(editorFileName);

    rc->firstShow();
    return rc;
}

void QDesignerWorkbench::minimizationStateChanged(QDesignerFormWindowInterface *formWindow, bool minimized)
{
    // refresh the minimize action state
    if (core()->formWindowManager()->activeFormWindow() == formWindow) {
        m_actionManager->minimizeAction()->setChecked(minimized);
    }
}

void QDesignerWorkbench::toggleFormMinimizationState()
{
    QDesignerFormWindowInterface *fwi = core()->formWindowManager()->activeFormWindow();
    if (!fwi || m_mode == NeutralMode)
        return;
    QDesignerFormWindow *fw = qobject_cast<QDesignerFormWindow *>(fwi->parentWidget());
    Q_ASSERT(fw);
    setFormWindowMinimized(fw, !isFormWindowMinimized(fw));
}

// reimplementing statusbar messages
void  QDesignerWorkbench::emitMessage(const QObject* /*object*/, const QString & /*message*/, int /*showTime*/)
{

}

bool QDesignerWorkbench::isFormWindowMinimized(const QDesignerFormWindow *fw)
{
    switch (m_mode) {
    case DockedMode:
        return mdiSubWindowOf(fw)->isShaded();
    default:
        break;
    }
    return fw->isMinimized();
}

void QDesignerWorkbench::setFormWindowMinimized(QDesignerFormWindow *fw, bool minimized)
{
    switch (m_mode) {
    case DockedMode: {
        QMdiSubWindow *mdiSubWindow = mdiSubWindowOf(fw);
        if (minimized) {
            mdiSubWindow->showShaded();
        } else {
            mdiSubWindow->setWindowState(mdiSubWindow->windowState() & ~Qt::WindowMinimized);
        }
    }
        break;
    default:
        break;
    }
}

void QDesignerWorkbench::reloadSettings()
{    
    m_rootWidget->reloadSettings(m_appSettings);
}


QWidget *QDesignerWorkbench::widgetBoxToolWindow() const
{
    return m_toolWindows.at(QDesignerWorkbench::WidgetBox);
}

QT_END_NAMESPACE
