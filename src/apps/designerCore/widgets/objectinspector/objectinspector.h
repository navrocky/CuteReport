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
#ifndef OBJECTINSPECTOR_H
#define OBJECTINSPECTOR_H

#include <QTreeView>
#include "designer_globals.h"

class ObjectModel;

class DESIGNER_EXPORTS ObjectInspector : public QTreeView
{
    Q_OBJECT
public:
    explicit ObjectInspector(QWidget *parent = 0);
	~ObjectInspector();
    QObjectList selectedObjects();

signals:
    void objectChanged( QObject * object);
    void selectionChanged();

public slots:
	void setRootObject( QObject * rootObject);
	void selectObject( QObject * object, QItemSelectionModel::SelectionFlag selFlag = QItemSelectionModel::ClearAndSelect);

private slots:
    void slotObjectChanged(const QModelIndex & index, const QModelIndex & );
    void slotSelectionChanged(QItemSelection, QItemSelection);

private:
	bool selectObject(QObject * object, QModelIndex index, QItemSelectionModel::SelectionFlag selFlag = QItemSelectionModel::ClearAndSelect);

private:
	ObjectModel * m_objectModel;
	QItemSelectionModel *m_selectionModel;
};

#endif // OBJECTINSPECTOR_H
