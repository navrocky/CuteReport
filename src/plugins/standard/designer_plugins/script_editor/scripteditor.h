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
#ifndef DESIGNERSCRIPTWIDGET_H
#define DESIGNERSCRIPTWIDGET_H

#include "moduleinterface.h"
#include <QDialog>
#include <QAction>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QStringList>

namespace CuteReport {
class ReportInterface;
}

class ScriptSyntaxHighlighter;
class QScriptEdit;
class QTableWidget;


class ScriptEditor : public CuteDesigner::ModuleInterface
{
	Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteDesigner.ModuleInterface/1.0")
#endif
    Q_INTERFACES(CuteDesigner::ModuleInterface)

public:
    ScriptEditor(QObject* parent = 0);
    ~ScriptEditor();

    virtual void init(CuteDesigner::Core *core);

    virtual void reloadSettings();
    virtual void saveSettings();
    virtual void activate();
    virtual void deactivate();
    virtual void sync();
    virtual QWidget * view();
    virtual QIcon icon();
    virtual QString name();
    virtual qint16 priority(){return 300;}


signals:
//	void removeVar(QString var);
//	void addVar(QString var);
	void refreshVariables();

private slots:
	void validate();
    void slotCurrentReportChanged(CuteReport::ReportInterface*report);

private:
    QString text();
    void setText(const QString & string);

    bool isValid();
//	void refreshVariables();
    QStringList variables();

private:
    QPointer <QWidget> m_gui;
	QGridLayout *gridLayout;
    QScriptEdit *textEdit;
	QHBoxLayout *horizontalLayout;
	QSpacerItem *horizontalSpacer;
	QPushButton *okButton;
	QPushButton *cancelButton;

	QAction * m_validateAction;

	QAction * m_undoAction;
	QAction * m_redoAction;

	QAction * m_copyAction;
	QAction * m_pasteAction;
	QAction * m_cutAction;
//	ScriptSyntaxHighlighter * m_syntax;
	QStringList varList;
};


#endif
