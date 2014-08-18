/***************************************************************************
 *   This file is part of the CuteReport project                           *
 *   Copyright (C) 2012-2014 by Alexander Mikhalov                         *
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

#ifndef SQLDATASETHELPER_H
#define SQLDATASETHELPER_H

#include "sqldataset.h"
#include <QTextEdit>
#include <QFocusEvent>

namespace Ui {
    class SqlDatasetEditor;
}

class SqlDatasetHelper : public CuteReport::DatasetHelperInterface
{
    Q_OBJECT
public:
    explicit SqlDatasetHelper(SqlDataset * dataset);
    virtual ~SqlDatasetHelper();

    virtual void save();
    virtual void load();
private slots:
    void slotDbhostChanged(QString host);
    void slotDbnameChanged(QString name);
    void slotDbuserChanged(QString user);
    void slotDbpasswdChanged(QString passwd);
    void slotDriverChanged(QString driver);
    void slotHostEdited();
    void slotDatabaseEdited();
    void slotUserEdited();
    void slotpasswordEdited();
    void slotDriverEdited();
    void slotQueryEdited();
    void slotCopyDriverName();
    void slotExtendedOptionsToggled(bool b);
    void slotLoadFile();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::SqlDatasetEditor *m_ui;
    SqlDataset * m_dataset;
};


class TextEdit: public QTextEdit
{
    Q_OBJECT
public:
    explicit TextEdit(QWidget *parent = 0):QTextEdit(parent){}
signals:
    void editingFinished();
private:
    virtual void focusOutEvent ( QFocusEvent * e ) {
        if (e->lostFocus())
            emit editingFinished();
        QTextEdit::focusOutEvent(e);
    }

};


#endif // SQLDATASETEDITOR_H
