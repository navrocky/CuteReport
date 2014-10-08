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
#include "resourcestoragehelper.h"
#include "ui_resourcestoragehelper.h"
#include "resource.h"
#include "reportcore.h"
#include "reportinterface.h"
#include "stdstoragedialog.h"
#include <QFileDialog>
#include <QMenu>

using namespace CuteReport;

ResourceStorageHelper::ResourceStorageHelper(StorageResource *storage, VisibleOptions visibleOptions) :
    StorageHelperInterface(storage),
    ui(new Ui::ResourceStorageHelper),
    m_storage(storage)
{
    Q_UNUSED(visibleOptions);
    ui->setupUi(this);

    CuteReport::ReportInterface * report = dynamic_cast<CuteReport::ReportInterface*>(m_storage->parent());

    QMenu *menu = new QMenu();
    foreach (StorageInterface * storage, m_storage->reportCore()->storageObjectList(report)) {
        if (storage->objectName() == m_storage->objectName() && storage->parent() == m_storage->parent())
            continue;
        QAction *action = new QAction(QString("%1 (%2)").arg(storage->objectName(), storage->moduleFullName()), this);
        action->setData(storage->objectName());
        connect(action, SIGNAL(triggered()), this, SLOT(addObject()));
        menu->addAction(action);
    }

    QMenu *menu2 = new QMenu();
    foreach (StorageInterface * storage, m_storage->reportCore()->storageObjectList(report)) {
        if (storage->objectName() == m_storage->objectName() && storage->parent() == m_storage->parent())
            continue;
        QAction *action = new QAction(QString("%1 (%2)").arg(storage->objectName(), storage->moduleFullName()), this);
        action->setData(storage->objectName());
        connect(action, SIGNAL(triggered()), this, SLOT(addObject()));
        menu2->addAction(action);
    }

    ui->bAddObject->setMenu(menu);
    ui->bAddReport->setMenu(menu2);

    /// show "objects" tab if storage is embedded"
    if (dynamic_cast<CuteReport::ReportInterface*>(m_storage->parent())) {
        ui->rbObjects->setChecked(true);
        itemTypeToggled(!ui->rbObjects->isChecked());
        ui->switchWidget->hide();
    }

    load();

    connect (ui->rbReports, SIGNAL(toggled(bool)), this, SLOT(itemTypeToggled(bool)));
    connect(ui->bAddReport, SIGNAL(clicked()), this, SLOT(addReport()));
    connect(ui->bAddPrefix, SIGNAL(clicked()), this, SLOT(addNewPrefix()));
    connect(ui->bRemovePrefix, SIGNAL(clicked()), this, SLOT(deletePrefix()));
    connect(ui->bRemoveObject, SIGNAL(clicked()), this, SLOT(deleteObject()));
    connect(ui->bRemoveReport, SIGNAL(clicked()), this, SLOT(deleteReport()));
    connect(ui->objectsList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(slotCurrentItemChanged(QListWidgetItem*,QListWidgetItem*)));
    connect(ui->objectPrefixList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(slotCurrentPrefixChanged(QListWidgetItem*,QListWidgetItem*)));
    connect(ui->reportsList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(slotCurrentReportChanged(QListWidgetItem*,QListWidgetItem*)));
    connect(ui->objectPrefixList, SIGNAL(itemChanged(QListWidgetItem*)),
            this,  SLOT(currentPrefixChanged(QListWidgetItem*)));

}


ResourceStorageHelper::~ResourceStorageHelper()
{
    delete ui;
}


void ResourceStorageHelper::load()
{
    fillReports();
    fillPrefixes();
    fillObjects();
    updatePreview();
}


void ResourceStorageHelper::save()
{
}

void ResourceStorageHelper::slotSync()
{
    m_storage->sync();
}


void ResourceStorageHelper::showSyncMessage(const QString & message)
{
    Q_UNUSED(message)
    //    ui->output->setText(message);
}


void ResourceStorageHelper::fillReports(const QString & currentReport)
{
    ui->reportsList->blockSignals(true);
    ui->reportsList->clear();
    QListWidgetItem * currentReportItem = 0;
    foreach (const QString & name, m_storage->reports().keys()) {
        QListWidgetItem * item = new QListWidgetItem(name, ui->reportsList);
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
        if (!currentReportItem)             // set first
            currentReportItem = item;
        if (currentReport == name)
            currentReportItem = item;
    }
    ui->reportsList->setCurrentItem(currentReportItem);
    m_currentReport = currentReportItem ? currentReportItem->text() : "";
    ui->reportsList->blockSignals(false);
}


void ResourceStorageHelper::fillPrefixes(const QString & currentPrefix)
{
    ui->objectPrefixList->blockSignals(true);
    ui->objectPrefixList->clear();
    QListWidgetItem * currentPrefixItem = 0;
    QStringList prefixes;
    foreach (const QString & path, m_storage->objects().keys()) {
        QString prefix = path.section("/",0, -2);
        if(prefix.isEmpty())
            prefix = "/";

        if (!prefixes.contains(prefix) ) {
            QListWidgetItem * item = new QListWidgetItem(prefix, ui->objectPrefixList);
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
            if (!currentPrefixItem)             // set first
                currentPrefixItem = item;
            if (currentPrefix == prefix)
                currentPrefixItem = item;
        }

        prefixes << prefix;
    }
    ui->objectPrefixList->setCurrentItem(currentPrefixItem);
    m_currentObjectPrefix = currentPrefixItem ? currentPrefixItem->text() : "";
    ui->objectPrefixList->blockSignals(false);
}


void ResourceStorageHelper::fillObjects(const QString & currentObject)
{
    QString currentPrefix;
    if (ui->objectPrefixList->currentItem())
        currentPrefix = ui->objectPrefixList->currentItem()->text();

    ui->objectsList->blockSignals(true);
    ui->objectsList->clear();
    m_objects.clear();

    QListWidgetItem * currentObjectItem = 0;

    foreach (const QString & path, m_storage->objects().keys()) {
        QString prefix = path.section("/",0, -2);
        QString objectName = path.section("/",-1);
        if(prefix.isEmpty())
            prefix = "/";

        if (currentPrefix != prefix)
            continue;

        QListWidgetItem * objectItem = new QListWidgetItem(objectName, ui->objectsList);
        objectItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable /*| Qt::ItemIsEditable*/);
        if (!currentObjectItem)             // set first
            currentObjectItem = objectItem;
        if (currentObject == objectName)
            currentObjectItem = objectItem;

        m_objects.insert(prefix, objectName);
    }

    ui->objectsList->setCurrentItem(currentObjectItem);
    m_currentObjectName = currentObjectItem ? currentObjectItem->text() : "";
    ui->objectsList->blockSignals(false);
}


void ResourceStorageHelper::addNewPrefix()
{
    QListWidgetItem * item = new QListWidgetItem("newPrefix", ui->objectPrefixList);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
    ui->objectPrefixList->setCurrentItem(item);
    ui->objectPrefixList->editItem(item);
}


void ResourceStorageHelper::deletePrefix()
{
    QStringList list;
    foreach (const QString & path, m_storage->objects().keys()) {
        QString prefix = path.section("/",0, -2);
        if (m_currentObjectPrefix == prefix)
            list << path;
    }

    foreach (const QString & path, list) {
        m_storage->objects().remove(path);
    }

    QListWidgetItem * item = ui->objectPrefixList->currentItem();
    delete item;

    m_currentObjectPrefix = "";
    fillObjects(m_currentObjectName);
}


void ResourceStorageHelper::addObject()
{
    QAction * action = dynamic_cast<QAction*>(sender());
    if (!action)
        return;

    CuteReport::ReportCore * core = m_storage->reportCore();
    CuteReport::ReportInterface * report = dynamic_cast<CuteReport::ReportInterface*>(m_storage->parent());
    QWidget * parentWidget = m_storage->reportCore()->rootWidget();

    CuteReport::StdStorageDialog d(core, report, parentWidget, "Load Object");
    d.setCurrentStorage(action->data().toString());
    d.setUrlHint("objects");

    if (!d.exec())
        return;

    QString selectedObjectUrl = d.currentObjectUrl();
    if (selectedObjectUrl.isEmpty())
        return;

    QVariant object = m_storage->reportCore()->loadObject(selectedObjectUrl, report);
    if (object.isNull())
        return;

    QString objectPath;
    QFileInfo fileInfo(selectedObjectUrl);
    QListWidgetItem * item = ui->objectPrefixList->currentItem();
    if (item)
        objectPath += item->text() + "/";
    objectPath += fileInfo.fileName();

    m_storage->saveObject(objectPath, object);
    if (ui->objectPrefixList->count() <= 0)          /// add default prefix is none exists
        fillPrefixes();
    fillObjects(fileInfo.fileName());
    updatePreview();
}


void ResourceStorageHelper::deleteObject()
{
    if (!ui->objectPrefixList->currentItem())
        return;
    m_storage->objects().remove(m_currentObjectPrefix + "/" + m_currentObjectName);
    fillObjects(m_currentObjectName);
    updatePreview();
}


void ResourceStorageHelper::itemTypeToggled(bool b)
{
    ui->stackedWidget->setCurrentIndex(b ? 1 : 0);
    updatePreview();
}


void ResourceStorageHelper::addReport()
{
    QAction * action = dynamic_cast<QAction*>(sender());
    if (!action)
        return;

    QWidget * parentWidget = m_storage->reportCore()->rootWidget();
    CuteReport::StdStorageDialog d(m_storage->reportCore(), parentWidget, "Load Report");
    d.setUrlHint("reports");
    if (!d.exec())
        return;

    m_currentReport = d.currentObjectUrl();
    if (m_currentReport.isEmpty())
        return;

    QVariant object = m_storage->reportCore()->loadObject(m_currentReport, 0);
    if (object.isNull())
        return;

    m_storage->addReport(m_currentReport, object);
    fillReports(m_currentReport);
    updatePreview();
}


void ResourceStorageHelper::deleteReport()
{
    if (!ui->reportsList->currentItem())
        return;
    m_storage->reports().remove(ui->reportsList->currentItem()->text());
    fillReports();
    updatePreview();
}


void ResourceStorageHelper::slotCurrentPrefixChanged(QListWidgetItem* current ,QListWidgetItem*)
{
    if (current)
        m_currentObjectPrefix = current->text();

    QString currentObject;
    if (ui->objectsList->currentItem())
        currentObject = ui->objectsList->currentItem()->text();

    fillObjects(m_currentObjectName);
    //    updatePreview();
}


void ResourceStorageHelper::slotCurrentItemChanged(QListWidgetItem* current ,QListWidgetItem*)
{
    if (current)
        m_currentObjectName = current->text();
    updatePreview();
}


void ResourceStorageHelper::slotCurrentReportChanged(QListWidgetItem* current ,QListWidgetItem*)
{
    if (current)
        m_currentReport = current->text();
    updatePreview();
}


void ResourceStorageHelper::updatePreview()
{
    if (ui->rbObjects->isChecked() || ui->rbObjects->isDown()) {
        if (!ui->objectPrefixList->currentItem() || !ui->objectsList->currentItem()) {
            ui->objectsPreview->setImage(QImage());
            return;
        }

//        CuteReport::ReportInterface * report = dynamic_cast<CuteReport::ReportInterface*>(m_storage->parent());
        QString objectPath = m_storage->urlScheme() + ":" + ui->objectPrefixList->currentItem()->text() + "/" + ui->objectsList->currentItem()->text();
        QVariant object = m_storage->loadObject(objectPath);
        QImage image = QImage::fromData(object.toByteArray());
        if (!image.isNull())
            ui->objectsPreview->setImage(image);
        else
            ui->objectsPreview->clear();
    } else if (ui->rbReports->isChecked() || ui->rbReports->isDown()){
        if (!m_currentReport.isEmpty()) {
            ui->objectsPreview->setReport(m_storage->urlScheme() + ":" + m_currentReport);
        }
        else
            ui->objectsPreview->clear();
    }
}


void ResourceStorageHelper::clearPreview()
{
    ui->objectsPreview->clear();
}


void ResourceStorageHelper::currentPrefixChanged (QListWidgetItem* item)
{
    if (!item  || item->text() == m_currentObjectPrefix)
        return;

    QStringList list;
    foreach (const QString & path, m_storage->objects().keys()) {
        QString prefix = path.section("/",0, -2);
        if (m_currentObjectPrefix == prefix)
            list << path;
    }

//    CuteReport::ReportInterface * report = dynamic_cast<CuteReport::ReportInterface*>(m_storage->parent());
    QString prefix = item->text();

    foreach (const QString & path, list) {
        QString objectName = path.section("/",-1);

        QVariant value = m_storage->objects().take(path);
        m_storage->saveObject(prefix + "/" + objectName, value);
    }

    m_currentObjectPrefix = item->text();
    fillObjects(m_currentObjectName);
}
