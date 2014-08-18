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

#ifndef METRICFONTDIALOG_H
#define METRICFONTDIALOG_H

#include <QDialog>
#include "ui_metricfontdialog.h"
#include <QFontDatabase>
#include <QStringListModel>

class MetricFontDialog : public QDialog, private Ui::metricFontDialog
{
    Q_OBJECT

public:

#if QT_VERSION < 0x050000
    MetricFontDialog(QWidget* parent = 0, Qt::WFlags fl = 0);
#else
    MetricFontDialog(QWidget* parent = 0, Qt::WindowFlags fl = 0);
#endif

    ~MetricFontDialog();
    void setWritingSystem(QFontDatabase::WritingSystem ws);
    void setFont(const QFont & font);
    QFont font();
    static QFont getFont(const QFont & initial, QWidget * parent=0, const QString & caption=tr("Choose a font") );

private:
    void updateSample();
    void setFontFamily(const QString & family);
    void updateLists();

private:
    QFontDatabase m_fontDatabase;
    QStringList m_writingSystems;
    QStringList m_sizeList;
    QStringListModel m_writingSystemsModel;
    QStringListModel m_fontFamiliesModel;
    QStringListModel m_fontStyleModel;
    QStringListModel m_fontSizeModel;
    QStringListModel m_writingSystemModel;

    QItemSelectionModel * m_familiesSelectionModel;
    QItemSelectionModel * m_styleSelectionModel;
    QItemSelectionModel * m_sizeSelectionModel;

    QFontDatabase::WritingSystem m_writingSystem;
    QFont m_font;


private slots:
    void writingSystemChanged( int row );
    void familyChanged(const QModelIndex & current, const QModelIndex & previous );
    void sizeChanged(const QModelIndex & current, const QModelIndex & previous );
    void sizeChanged(double value );
    void styleChanged(const QModelIndex & current, const QModelIndex & previous );
    void strikeoutChanged(bool checked );
    void underlineChanged(bool checked );
    void fontWritingSystemChanged( int row );

};

#endif

