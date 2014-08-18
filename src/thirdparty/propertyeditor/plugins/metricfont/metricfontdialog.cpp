/***************************************************************************
 *   This file is part of the propertyEditor project                       *
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *   bog_dan_ro@yahoo.com                                                  *
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

#include <QCompleter>
#include <QItemSelectionModel>
#include <QGraphicsScene>
#include <QDesktopWidget>

#include "metricfontdialog.h"
#define screen_heightMM (((double)QDesktopWidget().screen()->height() /(double)QDesktopWidget().screen()->physicalDpiY() )*25.4)

QFont MetricFontDialog::getFont(const QFont & initial, QWidget * parent, const QString & caption )
{
	MetricFontDialog mfd(parent);
	mfd.setFont(initial);
	mfd.setWindowTitle(caption);
	return (mfd.exec()==QDialog::Accepted)?mfd.font():initial;
}

#if QT_VERSION < 0x050000
MetricFontDialog::MetricFontDialog(QWidget* parent, Qt::WFlags fl)
		: QDialog(parent, fl), Ui::metricFontDialog()
#else
MetricFontDialog::MetricFontDialog(QWidget* parent, Qt::WindowFlags fl)
        : QDialog(parent, fl), Ui::metricFontDialog()
#endif
{
	setupUi(this);

	sampleGraphicsView->setScene( new QGraphicsScene());

	m_writingSystem=QFontDatabase::Any;
	m_writingSystems.push_back(tr("Any"));
	foreach(QFontDatabase::WritingSystem ws, m_fontDatabase.writingSystems())
		m_writingSystems.push_back(QFontDatabase::writingSystemName(ws));

	m_writingSystemsModel.setStringList(m_writingSystems);
	writingSystemsComboBox->setModel(&m_writingSystemsModel);
	connect(writingSystemsComboBox, SIGNAL(currentIndexChanged(int)),SLOT(writingSystemChanged(int)));

	writingSystemComboBox->setModel(&m_writingSystemModel);
	connect(writingSystemComboBox, SIGNAL(currentIndexChanged(int)),SLOT(fontWritingSystemChanged(int)));

	m_styleSelectionModel = new QItemSelectionModel(&m_fontStyleModel,this);
	styleListView->setModel(&m_fontStyleModel);
	styleListView->setSelectionModel(m_styleSelectionModel);
	connect(m_styleSelectionModel, SIGNAL(currentChanged( const QModelIndex&, const QModelIndex& )), SLOT(styleChanged(const QModelIndex&, const QModelIndex& )));

	m_fontFamiliesModel.setStringList(m_fontDatabase.families(m_writingSystem));
	setFontFamily(m_fontDatabase.families(m_writingSystem)[0]);
	m_familiesSelectionModel= new QItemSelectionModel(&m_fontFamiliesModel,this);
	familiesListView->setModel(&m_fontFamiliesModel);
	familiesListView->setSelectionModel(m_familiesSelectionModel);
	connect(m_familiesSelectionModel, SIGNAL(currentChanged( const QModelIndex&, const QModelIndex& )), SLOT(familyChanged(const QModelIndex&, const QModelIndex& )));
	m_familiesSelectionModel->setCurrentIndex(m_fontFamiliesModel.index(0,0), QItemSelectionModel::SelectCurrent);

    m_sizeList<<"6"<<"7"<<"8"<<"9"<<"10"<<"10.5"<<"11"<<"12"<<"13"<<"14"<<"15"<<"16"<<"17"<<"18.0"<<"20.0"<<"22.0"<<"24"<<"26"<<"28"<<"32"<<"36"<<"40"<<"44"<<"48"<<"54"<<"60"<<"66"<<"72"<<"80";
	m_fontSizeModel.setStringList(m_sizeList);
	m_sizeSelectionModel = new QItemSelectionModel(&m_fontSizeModel,this);
	connect(m_sizeSelectionModel, SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), SLOT(sizeChanged(const QModelIndex&, const QModelIndex& )));
	connect(sizeDoubleSpinBox, SIGNAL(valueChanged(double)),SLOT(sizeChanged(double)));
	sizeListView->setModel(&m_fontSizeModel);
	sizeListView->setSelectionModel(m_sizeSelectionModel);
	m_sizeSelectionModel->setCurrentIndex(m_fontSizeModel.index(3,0), QItemSelectionModel::SelectCurrent);

	connect(strikeoutCheckBox, SIGNAL(toggled(bool)),SLOT(strikeoutChanged(bool)));
	connect(underlineCheckBox, SIGNAL(toggled(bool)),SLOT(underlineChanged(bool)));
}

MetricFontDialog::~MetricFontDialog()
{

}

void MetricFontDialog::updateLists()
{
	m_fontStyleModel.setStringList(m_fontDatabase.styles(m_font.family()));
	m_styleSelectionModel->clearSelection();
	if (m_fontDatabase.styles(m_font.family()).size())
		m_styleSelectionModel->setCurrentIndex(m_fontStyleModel.index(m_fontDatabase.styles(m_font.family()).indexOf(m_fontDatabase.styleString(m_font)),0), QItemSelectionModel::SelectCurrent);
	if (!m_styleSelectionModel->currentIndex().isValid())
		m_styleSelectionModel->setCurrentIndex(m_fontStyleModel.index(0,0), QItemSelectionModel::SelectCurrent);
	QStringList wsl;
	foreach(QFontDatabase::WritingSystem ws, m_fontDatabase.writingSystems(m_font.family()))
		wsl.push_back(QFontDatabase::writingSystemName(ws));
	m_writingSystemModel.setStringList(wsl);

	if (wsl.size())
		writingSystemComboBox->setCurrentIndex(0);
}

void MetricFontDialog::setFont(const QFont & font)
{
	m_font = font;
	writingSystemsComboBox->setCurrentIndex(0);
	sizeDoubleSpinBox->setValue(m_font.pointSizeF());
	sizeChanged(m_font.pointSizeF());
	m_familiesSelectionModel->clearSelection();
	if (-1!=m_fontFamiliesModel.stringList().indexOf(m_font.family()))
		m_familiesSelectionModel->setCurrentIndex(m_fontFamiliesModel.index(m_fontFamiliesModel.stringList().indexOf(m_font.family()),0), QItemSelectionModel::SelectCurrent);
	updateLists();
	updateSample();
}

QFont MetricFontDialog::font()
{
	return m_font;
}


void MetricFontDialog::setWritingSystem(QFontDatabase::WritingSystem ws)
{
	m_writingSystem=ws;
	m_fontFamiliesModel.setStringList(m_fontDatabase.families(ws));
	if (m_fontDatabase.families(ws).size())
		m_familiesSelectionModel->setCurrentIndex(m_fontFamiliesModel.index(0,0), QItemSelectionModel::SelectCurrent);
	updateSample();
}

void MetricFontDialog::writingSystemChanged( int row )
{
	if (!row)
		setWritingSystem(QFontDatabase::Any);
	else
		setWritingSystem(m_fontDatabase.writingSystems()[row-1]);
}

void MetricFontDialog::setFontFamily(const QString & family)
{
	m_font.setFamily(family);
	updateLists();
	updateSample();
}

void MetricFontDialog::familyChanged(const QModelIndex & current, const QModelIndex & /*previous*/ )
{
	setFontFamily(m_fontDatabase.families(m_writingSystem)[current.row()]);
}

void MetricFontDialog::sizeChanged(const QModelIndex & current, const QModelIndex & /*previous*/ )
{
	sizeDoubleSpinBox->setValue(m_sizeList[current.row()].toDouble());
	m_font.setPointSizeF(m_sizeList[current.row()].toDouble());
	updateSample();
}

void MetricFontDialog::sizeChanged(double value )
{
	QString val;
	val.setNum(value,'f',1);
	m_font.setPointSizeF(value);
	m_sizeSelectionModel->clearSelection();
	if (-1!=m_sizeList.indexOf(val))
		m_sizeSelectionModel->setCurrentIndex(m_fontSizeModel.index(m_sizeList.indexOf(val),0),QItemSelectionModel::SelectCurrent);
	updateSample();
}

void MetricFontDialog::styleChanged(const QModelIndex & current, const QModelIndex & /*previous */)
{
	QFont f=m_fontDatabase.font(m_font.family(), m_fontDatabase.styles(m_font.family())[current.row()],10);
	m_font.setStyle(f.style());
	m_font.setBold(f.bold());
	m_font.setItalic(f.italic());
	updateSample();
}

void MetricFontDialog::strikeoutChanged(bool checked )
{
	m_font.setStrikeOut(checked);
	updateSample();
}

void MetricFontDialog::underlineChanged(bool checked )
{
	m_font.setUnderline(checked);
	updateSample();
}

void MetricFontDialog::fontWritingSystemChanged(int)
{
	updateSample();
}

void MetricFontDialog::updateSample()
{
	sampleGraphicsView->scene()->clear();
    QFont f=m_font;
//	double scale=(double)screen_heightMM/QDesktopWidget().screen()->height();
//	f.setPixelSize(m_font.pointSizeF()/scale);
	sampleGraphicsView->scene()->setFont(f);
	sampleGraphicsView->scene()->addText(m_fontDatabase.writingSystemSample(m_fontDatabase.writingSystems(f.family()).size()?m_fontDatabase.writingSystems(f.family())[writingSystemComboBox->currentIndex()>-1 && m_fontDatabase.writingSystems(f.family()).size()>writingSystemComboBox->currentIndex() ? writingSystemComboBox->currentIndex(): 0]:QFontDatabase::Any),f);
}
