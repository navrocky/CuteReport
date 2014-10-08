/***************************************************************************
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *   bogdan@licentia.eu                                                    *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include <QScriptEngine>
#include <QtCore>
#include <QtGui>
#include <QToolBar>

#include "reportinterface.h"
#include "scripteditor.h"
#include "qscriptsyntaxhighlighter_p.h"
#include "qscriptedit_p.h"
#include "core.h"

inline void initMyResource() { Q_INIT_RESOURCE(script_editor); }

ScriptEditor::ScriptEditor(QObject *parent)
    : ModuleInterface(parent)
    ,m_gui(0)
{

}


void ScriptEditor::init(CuteDesigner::Core *core)
{
    initMyResource();
    ModuleInterface::init(core);

    m_gui = new QWidget();
    gridLayout = new QGridLayout(m_gui);
//	QSplitter *splitter = new QSplitter(Qt::Vertical, this);

    textEdit = new QScriptEdit( m_gui );
    QToolBar * tb = new QToolBar(m_gui);
//	varsWidget = new QTableWidget();
//	varsWidget->setColumnCount(2);
//	QStringList h;
//	h << tr("Variable") << tr ("Value");
//	varsWidget->setHorizontalHeaderLabels(h);

//	textEdit->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::MinimumExpanding );
//	varsWidget->setSizePolicy( QSizePolicy::Preferred ,QSizePolicy::Minimum)  ;

//	splitter->addWidget(textEdit);
//	splitter->addWidget(varsWidget);
//	splitter->setStretchFactor(0, 1);

    gridLayout->addWidget(tb, 0, 0, 1, 1);
    gridLayout->addWidget(textEdit, 1, 0, 1, 1);

    QMetaObject::connectSlotsByName(this);

    m_copyAction = tb->addAction(QIcon(":/images/editcopy.png"), tr("Copy"));
    m_cutAction = tb->addAction(QIcon(":/images/editcut.png"), tr("Cut"));
    m_pasteAction = tb->addAction(QIcon(":/images/editpaste.png"), tr("Paste"));
    tb->addSeparator();
    m_undoAction = tb->addAction(QIcon(":/images/editundo.png"), tr("Undo"));
    m_redoAction = tb->addAction(QIcon(":/images/editredo.png"), tr("Redo"));
    tb->addSeparator();
    m_validateAction = tb->addAction(QIcon(":/images/validate.png"), tr("Validate"));

    m_copyAction->setEnabled(false);
    m_cutAction->setEnabled(false);
    m_undoAction->setEnabled(false);
    m_redoAction->setEnabled(false);
    m_pasteAction->setEnabled(false/*textEdit ->canPaste()*/);

    connect(textEdit , SIGNAL(copyAvailable(bool)) , m_copyAction, SLOT(setEnabled(bool)));
    connect(textEdit , SIGNAL(copyAvailable(bool)) , m_cutAction, SLOT(setEnabled(bool)));

    connect(m_validateAction, SIGNAL(triggered()), this, SLOT(validate()));

    connect(m_copyAction, SIGNAL(triggered()), textEdit , SLOT(copy()));
    connect(m_cutAction, SIGNAL(triggered()), textEdit , SLOT(cut()));
    connect(m_pasteAction, SIGNAL(triggered()), textEdit , SLOT(paste()));

    connect(textEdit , SIGNAL(undoAvailable(bool)) , m_undoAction, SLOT(setEnabled(bool)));
    connect(textEdit , SIGNAL(redoAvailable(bool)) , m_redoAction, SLOT(setEnabled(bool)));

    connect(m_undoAction, SIGNAL(triggered()), textEdit , SLOT(undo()));
    connect(m_redoAction, SIGNAL(triggered()), textEdit , SLOT(redo()));

    m_gui->setEnabled(core->currentReport());
//	varsWidget->hide();

    connect(core, SIGNAL(currentReportChanged(CuteReport::ReportInterface*)), this, SLOT(slotCurrentReportChanged(CuteReport::ReportInterface*)));
}


void ScriptEditor::reloadSettings()
{

}

void ScriptEditor::saveSettings()
{

}

void ScriptEditor::activate()
{
    if (core()->currentReport())
        textEdit->setPlainText(core()->currentReport()->script());
}

void ScriptEditor::deactivate()
{
    sync();
}


void ScriptEditor::sync()
{
    if (core()->currentReport())
        core()->currentReport()->setScript(textEdit->toPlainText());
}


QWidget * ScriptEditor::view()
{
    return m_gui;
}


QIcon ScriptEditor::icon()
{
    return QIcon(":images/script_48.png");
}


QString ScriptEditor::name()
{
    return QString("Script");
}


QString ScriptEditor::text()
{
    return textEdit->toPlainText();
}

void ScriptEditor::setText(const QString & string)
{
    textEdit->setPlainText(string);
}

bool ScriptEditor::isValid()
{
    QScriptEngine se(this);
    return se.canEvaluate(textEdit->toPlainText());
}

void ScriptEditor::validate()
{

    if (isValid()){};
    /*
    if (isValid())
        Message::instance()->show(tr("the script it's ok"));
    else
        Message::instance()->show(tr("the script is invalid"));
*/
    emit refreshVariables();
}

ScriptEditor::~ScriptEditor()
{
//	delete m_syntax;
    delete m_gui;
}




QStringList ScriptEditor::variables()
{
    QStringList list;

    QRegExp rx("(\\$+)(\\w{1,30})");
    QString str = textEdit->toPlainText();
    int pos = 0;

    while ((pos = rx.indexIn(str, pos)) != -1) {
        if (rx.cap(1).count()%2==1 && !list.contains(rx.cap(2)) )
            list.append(rx.cap(2));
	pos += rx.matchedLength();
    }

    return list;
}


void ScriptEditor::slotCurrentReportChanged(CuteReport::ReportInterface* report)
{
    m_gui->setEnabled(core()->currentReport());
    textEdit->setPlainText(report ? report->script() : QString());
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(ScriptEditor, ScriptEditor)
#endif
