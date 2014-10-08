/***************************************************************************
 *   This file is part of the CuteReport project                           *
 *   Copyright (C) 2014 by Ivan Volkov                                     *
 *   wulff007@gmail.com                                                    *
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
#ifndef TEXTFORMATTOOLBAR_H
#define TEXTFORMATTOOLBAR_H

#include <QWidget>
#include <QTextCharFormat>
#include <QToolBar>

class QTextEdit;
class QComboBox;

class TextFormatToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit TextFormatToolBar(QWidget *parent = 0);

    void setTextEdit(QTextEdit *editor);
    QTextEdit *textEdit() const;

signals:

private slots:

    void currentCharFormatChanged(const QTextCharFormat &format);
    void cursorPositionChanged();

    void textBold();
    void textUnderline();
    void textItalic();
    void textFamily(const QString &f);
    void textSize(const QString &p);
    void textColor();
    void textAlign(QAction *a);
    void textFont();

    void comboSizeChanged();

private:
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    void fontChanged(const QFont &f);
    void colorChanged(const QColor &c);
    void alignmentChanged(Qt::Alignment a);

    QAction *m_acTextBold;
    QAction *m_acTextUnderline;
    QAction *m_acTextItalic;
    QAction *m_acTextColor;
    QAction *m_acAlignLeft;
    QAction *m_acAlignCenter;
    QAction *m_acAlignRight;
    QAction *m_acAlignJustify;
    QAction *m_acTextFont;

    QTextEdit *m_editor;
    QFont m_currentFont;
    QComboBox *m_cmbSize;



};

#endif // TEXTFORMATWIDGET_H
