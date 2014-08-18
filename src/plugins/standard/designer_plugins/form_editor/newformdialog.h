/***************************************************************************
 *   This file is part of the CuteReport project                           *
 *   Copyright (C) 2012-2013 by Alexander Mikhalov                              *
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
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/

#ifndef NEWFORMDIALOG_H
#define NEWFORMDIALOG_H

#include <QtGui>

namespace Ui {
class NewFormDialog;
}

class QDesignerWorkbench;
class QListWidgetItem;

namespace CuteDesigner {
    class Core;
}


class NewFormDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewFormDialog(CuteDesigner::Core * core, QDesignerWorkbench *workbench, QWidget *parent = 0);
    ~NewFormDialog();

    void loadFrom(const QString &path, bool isResource, const QString &uiExtension);
    const QString getForm();
    

private slots:
    void currentItemChanged ( QListWidgetItem * current, QListWidgetItem * previous );
    bool showCurrentItemPixmap(QListWidgetItem * item);
    void fillFromSource(bool isResources);

private:
    QPixmap formPreviewPixmap(const QListWidgetItem *item);
    QPixmap formPreviewPixmap(const QString &fileName) const;

private:
    Ui::NewFormDialog *ui;

    CuteDesigner::Core * m_core;
    typedef QMap<const QListWidgetItem *, QPixmap> ItemPixmapCache;
    ItemPixmapCache m_itemPixmapCache;
};

#endif // NEWFORMDIALOG_H
