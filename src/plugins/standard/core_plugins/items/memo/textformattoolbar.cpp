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
#include "textformattoolbar.h"

#include <QAction>
#include <QToolBar>
#include <QApplication>
#include <QTextEdit>
#include <QColorDialog>
#include <QFontDialog>
#include <QLayout>
#include <QComboBox>
#include <QLineEdit>

TextFormatToolBar::TextFormatToolBar(QWidget *parent) :
    QToolBar(parent),
    m_editor(0)
{
    QString rsrcPath = ":/images";

    setIconSize(QSize(20,20));

    m_acTextBold = new QAction(QIcon(rsrcPath + "/text_bold.png"),
                               tr("&Bold"), this);
    m_acTextBold->setShortcut(Qt::CTRL + Qt::Key_B);

    QFont bold;
    bold.setBold(true);
    m_acTextBold->setFont(bold);
    connect(m_acTextBold, SIGNAL(triggered()), this, SLOT(textBold()));
    m_acTextBold->setCheckable(true);
    addAction(m_acTextBold);

    m_acTextItalic = new QAction(QIcon(rsrcPath + "/text_italic.png"),
                                 tr("&Italic"), this);
    m_acTextItalic->setPriority(QAction::LowPriority);
    m_acTextItalic->setShortcut(Qt::CTRL + Qt::Key_I);
    QFont italic;
    italic.setItalic(true);
    m_acTextItalic->setFont(italic);
    connect(m_acTextItalic, SIGNAL(triggered()), this, SLOT(textItalic()));
    addAction(m_acTextItalic);
    m_acTextItalic->setCheckable(true);

    m_acTextUnderline = new QAction(QIcon(rsrcPath + "/text_underline.png"),
                                    tr("&Underline"), this);
    m_acTextUnderline->setShortcut(Qt::CTRL + Qt::Key_U);
    m_acTextUnderline->setPriority(QAction::LowPriority);
    QFont underline;
    underline.setUnderline(true);
    m_acTextUnderline->setFont(underline);
    connect(m_acTextUnderline, SIGNAL(triggered()), this, SLOT(textUnderline()));
    addAction(m_acTextUnderline);
    m_acTextUnderline->setCheckable(true);

    addSeparator();

    QActionGroup *grp = new QActionGroup(this);
    connect(grp, SIGNAL(triggered(QAction*)), this, SLOT(textAlign(QAction*)));

    if (QApplication::isLeftToRight()) {
        m_acAlignLeft = new QAction(QIcon(rsrcPath + "/text_align_left.png"),
                                    tr("&Left"), grp);
        m_acAlignCenter = new QAction(QIcon(rsrcPath + "/text_align_center.png"), tr("C&enter"), grp);
        m_acAlignRight = new QAction(QIcon(rsrcPath + "/text_align_right.png"), tr("&Right"), grp);
    } else {
        m_acAlignRight = new QAction(QIcon(rsrcPath + "/text_align_right.png"), tr("&Right"), grp);
        m_acAlignCenter = new QAction(QIcon(rsrcPath + "/text_align_center.png"), tr("C&enter"), grp);
        m_acAlignLeft = new QAction(QIcon(rsrcPath + "/text_align_left.png"), tr("&Left"), grp);
    }
    m_acAlignJustify = new QAction(QIcon(rsrcPath + "/text_align_justify.png"), tr("&Justify"), grp);

    m_acAlignLeft->setShortcut(Qt::CTRL + Qt::Key_L);
    m_acAlignLeft->setCheckable(true);
    m_acAlignLeft->setPriority(QAction::LowPriority);
    m_acAlignCenter->setShortcut(Qt::CTRL + Qt::Key_E);
    m_acAlignCenter->setCheckable(true);
    m_acAlignCenter->setPriority(QAction::LowPriority);
    m_acAlignRight->setShortcut(Qt::CTRL + Qt::Key_R);
    m_acAlignRight->setCheckable(true);
    m_acAlignRight->setPriority(QAction::LowPriority);
    m_acAlignJustify->setShortcut(Qt::CTRL + Qt::Key_J);
    m_acAlignJustify->setCheckable(true);
    m_acAlignJustify->setPriority(QAction::LowPriority);

    addActions(grp->actions());

    addSeparator();

    m_cmbSize = new QComboBox(this);
    m_cmbSize->setEditable(true);
    m_cmbSize->addItem(QString("6"));
    m_cmbSize->addItem(QString("7"));
    m_cmbSize->addItem(QString("8"));
    m_cmbSize->addItem(QString("9"));
    m_cmbSize->addItem(QString("10"));
    m_cmbSize->addItem(QString("12"));
    m_cmbSize->addItem(QString("14"));
    m_cmbSize->addItem(QString("16"));
    m_cmbSize->addItem(QString("20"));
    m_cmbSize->addItem(QString("22"));
    m_cmbSize->addItem(QString("26"));
    m_cmbSize->addItem(QString("30"));
    m_cmbSize->addItem(QString("42"));
    m_cmbSize->addItem(QString("72"));

    addWidget(m_cmbSize);
    addSeparator();

    QPixmap pix(16, 16);
    pix.fill(Qt::black);
    m_acTextColor = new QAction(pix, tr("&Color..."), this);
    connect(m_acTextColor, SIGNAL(triggered()), this, SLOT(textColor()));
    addAction(m_acTextColor);


    m_acTextFont = new QAction(QIcon(rsrcPath + "/style.png"),
                               tr("&Style"), this);
    connect(m_acTextFont,SIGNAL(triggered()),SLOT(textFont()));
    connect(m_cmbSize, SIGNAL(editTextChanged(QString)), SLOT(textSize(QString)));
    addAction(m_acTextFont);
}

void TextFormatToolBar::setTextEdit(QTextEdit *editor)
{
    if (m_editor) {
        m_editor->disconnect();
    }

    if (m_editor == editor) {
        return;
    }

    m_editor = editor;

    connect(m_editor,SIGNAL(currentCharFormatChanged(QTextCharFormat)),
            SLOT(currentCharFormatChanged(QTextCharFormat)));
    connect(m_editor,SIGNAL(cursorPositionChanged()),
            SLOT(cursorPositionChanged()));
    connect(m_editor, SIGNAL(textChanged()), SLOT(cursorPositionChanged()));
}

QTextEdit *TextFormatToolBar::textEdit() const
{
    return m_editor;
}

void TextFormatToolBar::currentCharFormatChanged(const QTextCharFormat &format)
{
    fontChanged(format.font());
    colorChanged(format.foreground().color());
}

void TextFormatToolBar::cursorPositionChanged()
{
    alignmentChanged(m_editor->alignment());
}

void TextFormatToolBar::textBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(m_acTextBold->isChecked() ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
}

void TextFormatToolBar::textUnderline()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(m_acTextUnderline->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void TextFormatToolBar::textItalic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(m_acTextItalic->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void TextFormatToolBar::textFamily(const QString &f)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    mergeFormatOnWordOrSelection(fmt);
}

void TextFormatToolBar::textSize(const QString &p)
{    
    qreal pointSize = p.toFloat();
    if (p.toFloat() > 0) {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        mergeFormatOnWordOrSelection(fmt);
    }
}

void TextFormatToolBar::textColor()
{
    QColor col = QColorDialog::getColor(m_editor->textColor(), this);
    if (!col.isValid())
        return;
    QTextCharFormat fmt;
    fmt.setForeground(col);
    mergeFormatOnWordOrSelection(fmt);
    colorChanged(col);
}

void TextFormatToolBar::textAlign(QAction *a)
{
    if (a == m_acAlignLeft)
        m_editor->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
    else if (a == m_acAlignCenter)
        m_editor->setAlignment(Qt::AlignHCenter);
    else if (a == m_acAlignRight)
        m_editor->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
    else if (a == m_acAlignJustify)
        m_editor->setAlignment(Qt::AlignJustify);
}

void TextFormatToolBar::textFont()
{
    bool ok;

    QFont font = QFontDialog::getFont(
                &ok, m_currentFont, this);
    if (ok) {
        QTextCharFormat fmt;
        fmt.setFont(font);
        mergeFormatOnWordOrSelection(fmt);
    }
}

void TextFormatToolBar::comboSizeChanged()
{
    textSize(m_cmbSize->currentText());
}

void TextFormatToolBar::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = m_editor->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    m_editor->mergeCurrentCharFormat(format);
}

void TextFormatToolBar::fontChanged(const QFont &f)
{
    m_currentFont = f;
    m_acTextBold->setChecked(f.bold());
    m_acTextItalic->setChecked(f.italic());
    m_acTextUnderline->setChecked(f.underline());
    m_cmbSize->blockSignals(true);
    m_cmbSize->lineEdit()->setText(QString::number(f.pointSize()));
    m_cmbSize->blockSignals(false);
}

void TextFormatToolBar::colorChanged(const QColor &c)
{
    QPixmap pix(16, 16);
    pix.fill(c);
    m_acTextColor->setIcon(pix);
}

void TextFormatToolBar::alignmentChanged(Qt::Alignment a)
{
    if (a & Qt::AlignLeft) {
        m_acAlignLeft->setChecked(true);
    } else if (a & Qt::AlignHCenter) {
        m_acAlignCenter->setChecked(true);
    } else if (a & Qt::AlignRight) {
        m_acAlignRight->setChecked(true);
    } else if (a & Qt::AlignJustify) {
        m_acAlignJustify->setChecked(true);
    }
}
