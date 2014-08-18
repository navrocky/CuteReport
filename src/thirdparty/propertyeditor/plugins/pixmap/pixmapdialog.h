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

#ifndef PIXMAPDIALOG_H
#define PIXMAPDIALOG_H

#include <QDialog>
#include <QGraphicsView>

#include "ui_pixmapDialog.h"

class PixmapDialog : public QDialog, private Ui::pixmapDialog
{
    Q_OBJECT

public:
#if QT_VERSION < 0x050000
    PixmapDialog(QWidget* parent = 0, Qt::WFlags fl = 0);
#else
    PixmapDialog(QWidget* parent = 0, Qt::WindowFlags fl = 0);
#endif
    void setPixmap(const QPixmap & pixmap);
    QPixmap pixmap();

private:
    QPixmap m_pixmap;

public slots:
    void on_chooseButton_clicked();
    void on_clearButton_clicked();
};

#endif

