/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the Qt Designer of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QStringListModel>
#include <QFileDialog>
#include <QTextStream>

#include "stringlisteditor.h"

StringListEditor::StringListEditor(QWidget *parent)
		: QDialog(parent), Ui::stringListDialog(), m_model(new QStringListModel(this))
{
	listView=0;
	setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	listView->setModel(m_model);

	connect(listView->selectionModel(),
	        SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
	        this, SLOT(currentIndexChanged(const QModelIndex &, const QModelIndex &)));

	connect(listView->itemDelegate(),
	        SIGNAL(closeEditor(QWidget *, QAbstractItemDelegate::EndEditHint)),
	        this, SLOT(currentValueChanged()));

	upButton->setIcon(QIcon(":/propertyEditor/stringList/images/up.png"));
	downButton->setIcon(QIcon(":/propertyEditor/stringList/images/down.png"));
	newButton->setIcon(QIcon(":/propertyEditor/stringList/images/plus.png"));
	deleteButton->setIcon(QIcon(":/propertyEditor/stringList/images/minus.png"));
	updateUi();
}

StringListEditor::~StringListEditor()
{
}

QStringList StringListEditor::getStringList(QWidget *parent, const QStringList &init, int *result)
{
	StringListEditor dlg(parent);
	dlg.setStringList(init);
	int res = dlg.exec();
	if (result)
		*result = res;
	return (res == QDialog::Accepted) ? dlg.stringList() : init;
}

void StringListEditor::setStringList(const QStringList &stringList)
{
	m_model->setStringList(stringList);
	updateUi();
}

QStringList StringListEditor::stringList() const
{
	return m_model->stringList();
}

void StringListEditor::currentIndexChanged(const QModelIndex &current, const QModelIndex &previous)
{
	Q_UNUSED(previous);
	setCurrentIndex(current.row());
	updateUi();
}

void StringListEditor::currentValueChanged()
{
	setCurrentIndex(currentIndex());
	updateUi();
}

void StringListEditor::on_upButton_clicked()
{
	int from = currentIndex();
	int to = currentIndex() - 1;
	QString value = stringAt(from);
	removeString(from);
	insertString(to, value);
	setCurrentIndex(to);
	updateUi();
}

void StringListEditor::on_downButton_clicked()
{
	int from = currentIndex();
	int to = currentIndex() + 1;
	QString value = stringAt(from);
	removeString(from);
	insertString(to, value);
	setCurrentIndex(to);
	updateUi();
}

void StringListEditor::on_newButton_clicked()
{
	int to = currentIndex();
	if (to == -1)
		to = count() - 1;
	++to;
	insertString(to, QString());
	setCurrentIndex(to);
	updateUi();
	editString(to);
}

void StringListEditor::on_deleteButton_clicked()
{
	removeString(currentIndex());
	setCurrentIndex(currentIndex());
	updateUi();
}

void StringListEditor::on_fileButton_clicked()
{
 	QString fileName = QFileDialog::getOpenFileName(this, tr("Choose a data file"),
                                                 "", "*.cht *.txt");

   if (!fileName.isEmpty()) {
        QFile file(fileName);

        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream stream(&file);
            QString line;

            m_model->removeRows(0, m_model->rowCount(QModelIndex()), QModelIndex());

            int row = 0;
            do {
                line = stream.readLine();
                if (!line.isEmpty()) {

                    m_model->insertRows(row, 1, QModelIndex());

                    m_model->setData(m_model->index(row, 0, QModelIndex()), line);

                    row++;
                }
            } while (!line.isEmpty());

            file.close();
        }
    }
}

void StringListEditor::on_clearButton_clicked()
{
     m_model->removeRows(0, m_model->rowCount(QModelIndex()), QModelIndex());
}

void StringListEditor::on_valueEdit_textEdited(const QString &text)
{
	setStringAt(currentIndex(), text);
}

void StringListEditor::updateUi()
{
	upButton->setEnabled((count() > 1) && (currentIndex() > 0));
	downButton->setEnabled((count() > 1) && (currentIndex() >= 0) && (currentIndex() < (count() - 1)));
	deleteButton->setEnabled(currentIndex() != -1);
	valueEdit->setEnabled(currentIndex() != -1);
}

int StringListEditor::currentIndex() const
{
	return listView->currentIndex().row();
}

void StringListEditor::setCurrentIndex(int index)
{
	QModelIndex modelIndex = m_model->index(index, 0);
	if (listView->currentIndex() != modelIndex)
		listView->setCurrentIndex(modelIndex);
	valueEdit->setText(stringAt(index));
}

int StringListEditor::count() const
{
	return m_model->rowCount();
}

QString StringListEditor::stringAt(int index) const
{
	return qvariant_cast<QString>(m_model->data(m_model->index(index, 0), Qt::DisplayRole));
}

void StringListEditor::setStringAt(int index, const QString &value)
{
	m_model->setData(m_model->index(index, 0), value);
}

void StringListEditor::removeString(int index)
{
	m_model->removeRows(index, 1);
}

void StringListEditor::insertString(int index, const QString &value)
{
	m_model->insertRows(index, 1);
	m_model->setData(m_model->index(index, 0), value);
}

void StringListEditor::editString(int index)
{
	listView->edit(m_model->index(index, 0));
}
