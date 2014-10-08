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
#ifndef MEMOHELPER_H
#define MEMOHELPER_H

#include "iteminterface.h"
#include "plugins_common.h"

namespace Ui {
    class MemoHelper;
}

class Highlighter;

SUIT_BEGIN_NAMESPACE
class MemoItem;
class MemoHelper;
SUIT_END_NAMESPACE

USING_SUIT_NAMESPACE

SUIT_BEGIN_NAMESPACE
class MemoHelper : public CuteReport::BaseItemHelperInterface
{
    Q_OBJECT
public:
    enum State {Text, Expr};
    MemoHelper(MemoItem *item);
    ~MemoHelper();

    virtual void sync();
    virtual bool screenBack(bool accept = true);


private slots:
    void slotExpressionClicked();
    void slotAggregateClicked();
    void slotFormattingClicked();

    void slotCurrentTextTabChange(int index);

protected:
    void changeEvent(QEvent *e);

private:
    void setState(MemoHelper::State state);
    bool setToolWidget(const QString  & moduleName);

    Ui::MemoHelper *m_ui;
    MemoItem * m_item;
    QWidget * m_currentToolWidget;
    State m_currentState;
    Highlighter *m_highlighter;
};

SUIT_END_NAMESPACE

#endif // MEMOHELPER_H
