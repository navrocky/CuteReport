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
#include "testmodel.h"

#include <QtXml>

TestModel::TestModel(QObject *parent) :
    QStandardItemModel(parent)
{
}

void TestModel::load(const QByteArray data)
{
    if (data.isEmpty()) {
        return;
    }

    clear();

    QByteArray d(QByteArray::fromBase64(data));
    QDomDocument domDoc;

    QString error;
    int errLine = -1;

    if(domDoc.setContent(d, &error, &errLine)) {
        QDomElement domElement= domDoc.documentElement();
        traverseNode(domElement);
    } else {
        qDebug() << "Error while QDomDocument::setContent :" << error << "line: " << errLine;
    }
}

QByteArray TestModel::save()
{
    QDomDocument doc;
    QDomProcessingInstruction pi = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf8\"");
    doc.appendChild(pi);

    QDomElement  cellsEl = doc.createElement("cells");
    cellsEl.setAttribute("rows", rowCount());
    cellsEl.setAttribute("cols", columnCount());
    doc.appendChild(cellsEl);


    for (int row = 0; row < rowCount(); row++) {
        QDomElement itemRow = doc.createElement(QString("row%1").arg(row));

        for (int col = 0; col < columnCount(); col++ ) {

            //TODO save images
            if (index(row, col).data().toString().simplified().isEmpty()) {
                continue;
            }

            QDomElement itemCol = doc.createElement(QString("col%1").arg(col));
            itemCol.setAttribute("type", QVariant::typeToName(columnType(col)));

            QDomText text = doc.createTextNode(index(row, col).data().toByteArray());
            itemCol.appendChild(text);
            itemRow.appendChild(itemCol);
        }

        if (itemRow.childNodes().count() > 0)
            cellsEl.appendChild(itemRow);
    }

//    QFile file("c:/test.xml");
//    if(file.open(QIODevice::WriteOnly)) {
//        QTextStream(&file) << doc.toString();
//        file.close();
//    }

    return doc.toByteArray().toBase64();
}

void TestModel::traverseNode(const QDomNode &node)
{
    QDomElement cells = node.toElement();
    int rows = cells.attribute("rows").toInt();
    int cols = cells.attribute("cols").toInt();

    insertRows(0, rows);
    insertColumns(0, cols);

    if (rows == 0 || cols == 0) {
        return;
    }

    QDomNode rowNode = cells.firstChild();
    while (!rowNode.isNull()) {
        int row = rowNode.nodeName().remove(0, 3).toInt();

         QDomNode colNode = rowNode.firstChild();
         while (!colNode.isNull()) {
            int col = colNode.nodeName().remove(0, 3).toInt();

            QDomElement colEl = colNode.toElement();
            setColumnType(col, QVariant::nameToType(colEl.attribute("type").toAscii()));

            setItem(row, col, new QStandardItem(colEl.text()));

            colNode = colNode.nextSibling();
         }

        rowNode = rowNode.nextSibling();
    }

    // Fill empty indexes
    for (int row = 0; row < rowCount(); row++) {

        for (int col = 0; col < columnCount(); col++) {

            if(!item(row, col)) {
                setItem(row, col, new QStandardItem(QString()));
            }
        }
    }
}

void TestModel::setColumnType(const int col, const QVariant::Type type)
{
    m_typeByColumn.insert(col, type);
}

QVariant::Type TestModel::columnType(int col)
{
    return m_typeByColumn.value(col, QVariant::String);
}


