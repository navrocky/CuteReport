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

#include "newformdialog.h"
#include "ui_newformdialog.h"
#include "core.h"
#include "reportcore.h"
#include "storageinterface.h"
#include <QtGui>
#include <QFormBuilder>

const QString templatePath = QLatin1String(":/templates");
const QString templateStoragePrefix = QLatin1String("templates");
const QString uiExtension = QLatin1String("ui");

enum NewFormDialog_CustomRole {
    // File name (templates from resources, paths)
    TemplateNameRole = Qt::UserRole + 100,
    // Class name (widgets from Widget data base)
    ClassNameRole = Qt::UserRole + 101
};

NewFormDialog::NewFormDialog(CuteDesigner::Core * core, QDesignerWorkbench * /*workbench*/, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewFormDialog),
    m_core(core)
{
    ui->setupUi(this);
    setWindowTitle("Open Template");

    ui->rbInternal->setChecked(true);
    fillFromSource(true);

    connect(ui->list, SIGNAL(currentItemChanged ( QListWidgetItem* , QListWidgetItem*)), this,
            SLOT(currentItemChanged(QListWidgetItem*,QListWidgetItem*)));
    connect(ui->rbStorage, SIGNAL(toggled(bool)), this, SLOT(fillFromSource(bool)));

}


NewFormDialog::~NewFormDialog()
{
    delete ui;
}


const QString NewFormDialog::getForm()
{
    QListWidgetItem * item = ui->list->currentItem();
    const QVariant fileName = item->data(TemplateNameRole);
    QString str;

    if (ui->rbStorage->isChecked()) {
        QString error;
        QByteArray ba = m_core->reportCore()->loadObject(QString(templateStoragePrefix + "/" + item->data(TemplateNameRole).toString()),
                                                         m_core->currentReport(), &error).toByteArray();
        str = ba;
    } else {
        QFile file(fileName.toString());
        file.open(QFile::ReadOnly);
        str = file.readAll();
        file.close();
    }
    return str;
}


void NewFormDialog::currentItemChanged ( QListWidgetItem * current, QListWidgetItem * previous )
{
    Q_UNUSED(previous)
    showCurrentItemPixmap(current);
}


void NewFormDialog::loadFrom(const QString &path, bool isResource, const QString &uiExtension)
{
    Q_UNUSED(path);
    Q_UNUSED(isResource);
    Q_UNUSED(uiExtension);
    qDebug() << Q_FUNC_INFO;

//    const QChar underscore = QLatin1Char('_');
//    const QChar blank = QLatin1Char(' ');
//    ui->list->clear();
//    m_itemPixmapCache.clear();

//    if (isResource) {
//        const QDir dir(path);
//        if (!dir.exists())
//            return;

//        const QFileInfoList list = dir.entryInfoList(QStringList(QLatin1String("*.") + uiExtension), QDir::Files);
//        if (list.isEmpty())
//            return;

//        const QFileInfoList::const_iterator lcend = list.constEnd();
//        for (QFileInfoList::const_iterator it = list.constBegin(); it != lcend; ++it) {
//            if (!it->isFile())
//                continue;
//            const QString text = it->baseName().replace(underscore, blank);
//            QListWidgetItem * item = new QListWidgetItem(text, ui->list);
//            item->setData(TemplateNameRole, it->absoluteFilePath());
//        }
//    }
//    else {
//        QString error;
//        QStringList list = m_core->reportCore()->objectsList(m_core->currentReport(), path, &error);
//        qDebug() << list;

//        foreach (QString string, list ) {
//            QFileInfo fileInfo(string);
//            const QString text = fileInfo.baseName().replace(underscore, blank);
//            QListWidgetItem * item = new QListWidgetItem(text, ui->list);
//            item->setData(TemplateNameRole, fileInfo.fileName());
//        }
//    }
}


bool NewFormDialog::showCurrentItemPixmap(QListWidgetItem * item)
{
    bool result = false;
    if (item) {
        const QPixmap pixmap = formPreviewPixmap(item);
        if (pixmap.isNull()) {
            ui->preview->setText(tr("Error loading form"));
        } else {
            ui->preview->setPixmap(pixmap);
            result = true;
        }
    }
    return result;
}


QPixmap NewFormDialog::formPreviewPixmap(const QListWidgetItem *item)
{
    ItemPixmapCache::iterator it = m_itemPixmapCache.find(item);
    if (it == m_itemPixmapCache.end()) {
        QFormBuilder builder;
        QWidget *widget;
        if (ui->rbStorage->isChecked()) {
            QString error;
            QByteArray ba = m_core->reportCore()->loadObject(QString(templateStoragePrefix + "/" + item->data(TemplateNameRole).toString()),
                                              m_core->currentReport(), &error).toByteArray();
            QBuffer buffer(&ba);
            buffer.open(QFile::ReadOnly);
            widget = builder.load(&buffer, this);
            buffer.close();
        } else {
            const QVariant fileName = item->data(TemplateNameRole);
            QFile file(fileName.toString());
            file.open(QFile::ReadOnly);
            widget = builder.load(&file, this);
            file.close();
        }
        QPixmap pixmap;
        if (!widget)
            return pixmap;
        pixmap = QPixmap::grabWidget(widget);
        widget->deleteLater();

        if (pixmap.isNull())
            return pixmap;
        it = m_itemPixmapCache.insert(item, pixmap);
    }

    const int margin = 7;
    const int shadow = 7;
    const int previewHeight = 300;
    const int previewWidth = 400;
    QImage image = QImage(it.value().toImage().scaled(previewWidth - margin * 2, previewHeight - margin * 2,
                                             Qt::KeepAspectRatio,
                                             Qt::SmoothTransformation));

    QImage dest(previewWidth, previewHeight, QImage::Format_ARGB32_Premultiplied);
    dest.fill(0);

    QPainter p(&dest);
    p.drawImage(margin, margin, image);

    p.setPen(QPen(palette().brush(QPalette::WindowText), 0));

    p.drawRect(margin-1, margin-1, image.width() + 1, image.height() + 1);

    const QColor dark(Qt::darkGray);
    const QColor light(Qt::transparent);

    // right shadow
    {
        const QRect rect(margin + image.width() + 1, margin + shadow, shadow, image.height() - shadow + 1);
        QLinearGradient lg(rect.topLeft(), rect.topRight());
        lg.setColorAt(0, dark);
        lg.setColorAt(1, light);
        p.fillRect(rect, lg);
    }

    // bottom shadow
    {
        const QRect rect(margin + shadow, margin + image.height() + 1, image.width() - shadow + 1, shadow);
        QLinearGradient lg(rect.topLeft(), rect.bottomLeft());
        lg.setColorAt(0, dark);
        lg.setColorAt(1, light);
        p.fillRect(rect, lg);
    }

    // bottom/right corner shadow
    {
        const QRect rect(margin + image.width() + 1, margin + image.height() + 1, shadow, shadow);
        QRadialGradient g(rect.topLeft(), shadow);
        g.setColorAt(0, dark);
        g.setColorAt(1, light);
        p.fillRect(rect, g);
    }

    // top/right corner
    {
        const QRect rect(margin + image.width() + 1, margin, shadow, shadow);
        QRadialGradient g(rect.bottomLeft(), shadow);
        g.setColorAt(0, dark);
        g.setColorAt(1, light);
        p.fillRect(rect, g);
    }

    // bottom/left corner
    {
        const QRect rect(margin, margin + image.height() + 1, shadow, shadow);
        QRadialGradient g(rect.topRight(), shadow);
        g.setColorAt(0, dark);
        g.setColorAt(1, light);
        p.fillRect(rect, g);
    }

    p.end();

    return QPixmap::fromImage(dest);
}


void NewFormDialog::fillFromSource(bool isResources)
{
    Q_UNUSED(isResources)

    if (ui->rbStorage->isChecked()) {
        loadFrom(templateStoragePrefix, false, uiExtension);
    } else {
        loadFrom(templatePath, true, uiExtension);
    }
}
