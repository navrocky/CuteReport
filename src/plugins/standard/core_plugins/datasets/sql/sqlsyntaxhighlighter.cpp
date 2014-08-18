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

#include <QFile>
#include "sqlsyntaxhighlighter.h"

SQLSyntaxHighlighter::SQLSyntaxHighlighter(QTextDocument *parent)
		: QSyntaxHighlighter(parent)
{
	HighlightingRule rule;
	rule.format.setForeground(Qt::darkBlue);
	rule.format.setFontWeight(QFont::Bold);
	QFile file(":/syntax/sql.txt");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		while (!file.atEnd())
		{
			QString pattern = file.readLine();
			if (pattern.trimmed().length())
			{
				rule.pattern = QRegExp("\\b" + pattern.trimmed() + "\\b");
				highlightingRules.append(rule);
			}
		}
		file.close();
	}

	rule.format.setForeground(Qt::blue);
	rule.pattern = QRegExp("(\\b[0-9]+\\b)");
	highlightingRules.append(rule);

	rule.format.setForeground(Qt::black);
	rule.pattern = QRegExp("(\\b::\\b|\\B::\\B|\\#|\\(|\\)|\\[|\\]|\\||\\<|\\>|\\/|\\?|\\!|\\*|\\$|\\%|\\^|\\.|\\&|\\-|\\+)");
	highlightingRules.append(rule);

    rule.format.setForeground(QColor("#FF9393"));
    rule.pattern = QRegExp("\\$\\{.+\\}");
    highlightingRules.append(rule);

	rule.format.setFontWeight(QFont::Normal);
	rule.format.setForeground(Qt::darkGreen);

	rule.pattern = QRegExp("\'.*\'");
	highlightingRules.append(rule);

	multiLineCommentFormat.setForeground(Qt::darkGray);
	rule.format = multiLineCommentFormat;

	rule.pattern = QRegExp("--[^\n]*");
	highlightingRules.append(rule);

	commentStartExpression = QRegExp("/\\*");
	commentEndExpression = QRegExp("\\*/");
}

void SQLSyntaxHighlighter::highlightBlock(const QString & text)
{
	foreach(HighlightingRule rule, highlightingRules)
	{
		QRegExp expression(rule.pattern);
		int index = text.toLower().indexOf(expression);
		while (index >= 0)
		{
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = text.toLower().indexOf(expression, index + length);
		}
	}

	setCurrentBlockState(0);

	int startIndex = 0;
	if (previousBlockState() != 1)
		startIndex = text.indexOf(commentStartExpression);

	while (startIndex >= 0)
	{
		int endIndex = text.indexOf(commentEndExpression, startIndex);
		int commentLength;
		if (endIndex == -1)
		{
			setCurrentBlockState(1);
			commentLength = text.length() - startIndex;
		}
		else
		{
			commentLength = endIndex - startIndex
			                + commentEndExpression.matchedLength();
		}
		setFormat(startIndex, commentLength, multiLineCommentFormat);
		startIndex = text.indexOf(commentStartExpression,
		                          startIndex + commentLength);
	}
}

