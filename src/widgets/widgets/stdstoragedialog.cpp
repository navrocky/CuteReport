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
 ****************************************************************************/
#include "stdstoragedialog.h"
#include "ui_stdstoragedialog.h"
#include "reportcore.h"
#include "storageinterface.h"
#include "reportinterface.h"

#include <QFormBuilder>

static const int fileType    = QTreeWidgetItem::Type + 1;
static const int folderType  = QTreeWidgetItem::Type + 2;
static const int urlRole     = Qt::UserRole + 1;

static const QString defaultTitle("Storage Dialog");
static const QString filterAllFiles("All files (*.*)");

namespace CuteReport {


StdStorageDialog::StdStorageDialog(ReportCore * reportCore, ReportInterface * report, QWidget *parent, const QString &windowTitle) :
    QDialog(parent),
    ui(new Ui::StdStorageDialog)
{
    m_reportCore = reportCore;
    m_report = report;
    initMe();
    setViewOptions(ShowAll);
    setWindowTitle(windowTitle.isEmpty() ? defaultTitle : windowTitle);
}


StdStorageDialog::StdStorageDialog(ReportCore * reportCore, QWidget *parent, const QString & windowTitle) :
    QDialog(parent),
    ui(new Ui::StdStorageDialog),
    m_report(0)
{
    m_reportCore = reportCore;
    initMe();
    setViewOptions(ShowAll);
    setWindowTitle(windowTitle.isEmpty() ? defaultTitle : windowTitle);
}


StdStorageDialog::~StdStorageDialog()
{
    delete ui;
}


void StdStorageDialog::initMe()
{
    ui->setupUi(this);

    ui->preview->setReportCore(m_reportCore);

    m_canGoOutRoot = false;
    m_useAllFilesFilter = false;
    m_currentFileNameIsManuallyEntered = false;
#if QT_VERSION < 0x050000
    ui->list->header()->setResizeMode(0, QHeaderView::ResizeToContents);
    ui->list->header()->setResizeMode(1, QHeaderView::ResizeToContents);
    ui->list->header()->setResizeMode(2, QHeaderView::ResizeToContents);
#else
    ui->list->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->list->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->list->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
#endif
    int width = geometry().width() - 12;   // 12 is left + rigth layout spacing
    ui->splitter->setSizes(QList<int>() << width*0.6 << width*0.4);

    if (m_report) {
        foreach (StorageInterface * storage, m_report->storages())
            ui->cbStorage->addItem(QString ("%1 (%2)").arg(storage->objectName(), storage->moduleFullName()), storage->objectName());
    }

    foreach (StorageInterface * storage, m_reportCore->storageModules()) {
        if (ui->cbStorage->findData(storage->objectName()) != -1)
            continue;
        ui->cbStorage->addItem(QString ("%1 (%2)").arg(storage->objectName(), storage->moduleFullName()), storage->objectName());
    }

//    ui->cbFilter->addItem(filterAllFiles);
    ui->labelNameFilter->hide();
    ui->cbFilter->hide();

    //    m_dir.setFilter(QDir::NoDotAndDotDot|QDir::Dirs|QDir::Files);

    connect(ui->list, SIGNAL(itemActivated(QTreeWidgetItem*,int)), this, SLOT(itemActivated(QTreeWidgetItem*, int)));
    connect(ui->list, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
    connect(ui->list, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(itemDoubleClicked(QTreeWidgetItem*, int)));
    connect(ui->bRootDir, SIGNAL(clicked()), this, SLOT(toRootDir()));
    connect(ui->bUpDir, SIGNAL(clicked()), this, SLOT(dirUp()));
    connect(ui->cbStorage, SIGNAL(currentIndexChanged(int)), this, SLOT(currentStoragetIndexChanged(int)));
    connect(ui->cbFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(currentFultertIndexChanged(int)));
    connect(ui->fileName, SIGNAL(textEdited(QString)), this, SLOT(filenameEdited(QString)));
    connect(ui->fileName, SIGNAL(cursorPositionChanged(int,int)), SLOT(filenameCursorPositionChanged(int,int)));


    if (m_report)
        m_storage = m_reportCore->getStorageByName("",m_report);
    else
        m_storage = m_reportCore->defaultStorage();
    if (m_storage)
        ui->cbStorage->setCurrentIndex( ui->cbStorage->findData(m_storage->objectName() ) );
}


void StdStorageDialog::setCurrentStorage(const QString & storageName, bool exclusive)
{
//    if (m_report)
        m_storage = m_reportCore->getStorageByName(storageName, m_report);
//    else
//        m_storage = m_reportCore->storageModule(moduleName);

    if (m_storage) {
        ui->cbStorage->setCurrentIndex( ui->cbStorage->findData(m_storage->objectName() ) );
        ui->cbStorage->setDisabled(exclusive);
    }
}


//void StdStorageDialog::setCurrentStorageByName(const QString & storageScheme, bool exclusive)
//{
//    if (m_report)
//        m_storage = m_reportCore->getStorageByScheme(storageScheme, m_report);
//    else
//        m_storage = m_reportCore->storageModuleByScheme(storageScheme);

//    if (m_storage) {
//        ui->cbStorage->setCurrentIndex( ui->cbStorage->findText(m_storage->moduleFullName() ) );
//        ui->cbStorage->setDisabled(exclusive);
//    }
//}


QString StdStorageDialog::currentStorageName()
{
    return m_storage ? m_storage->moduleFullName() : "";
}


void StdStorageDialog::setViewOptions(ViewOptions f)
{
    ui->storageFrame->setVisible( f & ShowStorageSelect);
}


void StdStorageDialog::setExtensionAutocomplete(bool b)
{
    if (!b)
        m_extensionAutocomplete = QString();
}


void StdStorageDialog::setUrlHint(const QString & urlHint)
{
    m_urlHints.clear();
    m_urlHints.append(urlHint);
}


void StdStorageDialog::setUrlHints(const QStringList & urlHints)
{
    m_urlHints = urlHints;
}


void StdStorageDialog::setObjectHint(const QString & objectHint)
{
    m_objectHint = objectHint.section("/", -1, -1);
    m_extensionAutocomplete = m_objectHint.section(".", -1, -1);
}


void StdStorageDialog::setNameFilters(const QStringList & nameFilters, bool addAllFilesOption)
{
    m_useAllFilesFilter = addAllFilesOption;
    ui->cbFilter->clear();
    if (m_useAllFilesFilter)
        ui->cbFilter->addItem(filterAllFiles);
    ui->cbFilter->addItems(nameFilters);

    ui->labelNameFilter->setVisible(!nameFilters.isEmpty());
    ui->cbFilter->setVisible(!nameFilters.isEmpty());
}


QString StdStorageDialog::currentNameFilter() const
{
    return (m_useAllFilesFilter && ui->cbFilter->currentIndex() == 0) ? QString() :  ui->cbFilter->currentText();
}


void StdStorageDialog::setRootDir(const QString & dir, bool canGoOutRoot)
{
    QDir tempDir(dir);
    m_rootDir =  tempDir.absolutePath();
    m_canGoOutRoot = canGoOutRoot;
}


QString StdStorageDialog::rootDir() const
{
    return m_rootDir;
}


//void StdStorageDialog::setDirFilters(QDir::Filters filters)
//{
//    m_dir.setFilter(filters);
//}


//QDir::Filters StdStorageDialog::dirFilters() const
//{
//    return m_dir.filter();
//}


//void StdStorageDialog::setCurrentDir(const QString & currentDir)
//{
//    QString oldPath = m_dir.absolutePath();
//    m_dir.setPath(currentDir);
//    if (!m_dir.absolutePath().contains(m_rootDir))
//        m_dir.setPath(oldPath);
//}


//QString StdStorageDialog::currentDir() const
//{
//    return m_dir.absolutePath();
//}


void StdStorageDialog::populate(const QString &_url, const QString &urlIfEmpyTryThis)
{
    if (!m_storage)
        return;

    ui->list->clear();

    QString url = _url;

    bool ok = true;
    static const QDir::Filters dirFilters = QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot;
    static const QDir::Filters fileFilters = QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot;

    QStringList nameFilters;
    if (ui->cbFilter->currentIndex() > (m_useAllFilesFilter ? 0 : -1) ) {
        QRegExp rx("\\((.*)\\)");
        if (rx.indexIn(ui->cbFilter->currentText()) != -1)
            nameFilters << rx.cap(1);
    }

    // dirs
    QList<StorageObjectInfo> fileInfoList = m_storage->objectsList(url, QStringList(), dirFilters, QDir::Name, &ok);
    if (!ok) {
        url = urlIfEmpyTryThis;
        fileInfoList = m_storage->objectsList(url, QStringList(), dirFilters, QDir::Name, &ok);
    }

    // files
    if (ok)
        fileInfoList << m_storage->objectsList(url, nameFilters, fileFilters, QDir::Name, &ok);

    QTreeWidgetItem * defaultObjectItem = 0;
    qint64 totalSize = 0;
    int totalDirs = 0;
    int totalFiles = 0;
    foreach(const StorageObjectInfo & info, fileInfoList) {
        QStringList fields;
        QString sizeStr;
        int type = (info.type == FileDir) ? folderType : fileType;

        if (info.type == FileDir)
            sizeStr = "<DIR>";
        else if (info.size > 1024000000)
            sizeStr = QString::number(info.size/1024000000) + "Tb";
        else if (info.size > 1024000)
            sizeStr = QString::number(info.size/1024000) + "Mb";
        else if (info.size > 1024)
            sizeStr = QString::number(info.size/1024) + "Kb";
        else
            sizeStr =QString::number(info.size) + "B";

        QUrl u (info.url);
        QFileInfo i(u.path());

        fields << i.baseName() << i.completeSuffix() << sizeStr;

        QTreeWidgetItem * objectItem = new QTreeWidgetItem(ui->list, fields, type );
        static const QIcon folderIcon(":folder-blue.png");
        static const QIcon fileIcon(":page-simple.png");
        objectItem->setIcon(0, type == folderType ? folderIcon : fileIcon);
        objectItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable /*| Qt::ItemIsEditable*/);
        objectItem->setData(0, urlRole, info.url);

        if (i.fileName() == m_objectHint)
            defaultObjectItem = objectItem;

        totalSize += info.size;
        if (type == folderType)
            ++totalDirs;
        else if (type == fileType)
            ++totalFiles;
    }


    m_previousUrl = m_currentUrl;
    m_currentUrl = url;

    QUrl curUrl(m_currentUrl);

    ui->dirInfo->setText(QString("%1 files / %2 dir(s)").arg(totalFiles).arg(totalDirs));
    ui->currentDir->setText(curUrl.path());
//    if (ui->currentDir->text().isEmpty())  # no need to do this since it translated to wron path then
//        ui->currentDir->setText("/");
    ui->preview->clear();
    if (!m_currentFileNameIsManuallyEntered)
        ui->fileName->clear();

    if (url == m_storage->rootUrl()) {
        ui->bUpDir->setEnabled(false);
        ui->bRootDir->setEnabled(false);
    } else {
        ui->bUpDir->setEnabled(true);
        ui->bRootDir->setEnabled(true);
    }

    if (defaultObjectItem) {
        ui->list->setCurrentItem(defaultObjectItem);
        currentItemChanged(defaultObjectItem, 0);
    }

    m_formPixmapCache.clear();
}


void StdStorageDialog::currentItemChanged ( QTreeWidgetItem * current, QTreeWidgetItem * previous )
{
    Q_UNUSED(previous);
    if (!current)
        return;

    if (current->type() == fileType) {
        QString name = current->text(0);
        QString ext = current->text(1);
        ui->fileName->setText(name + "." + ext);
        m_currentFileNameIsManuallyEntered = false;
        if (ext == "qtrp")
            ui->preview->setReport(currentObjectUrl());
        else if (ext == "jpg" || ext == "jpeg" || ext == "png")
            ui->preview->setImage(QImage::fromData(m_storage->loadObject(currentObjectUrl()).toByteArray()));
        else if (ext == "ui")
            ui->preview->setImage(formPreviewPixmap(current));
        else
            ui->preview->clear();
    }
}


void StdStorageDialog::currentStoragetIndexChanged(int index)
{
    Q_UNUSED(index);
    m_storage = m_reportCore->getStorageByName(ui->cbStorage->itemData(ui->cbStorage->currentIndex()).toString(), m_report);
    populate(m_currentUrl);
}


void StdStorageDialog::currentFultertIndexChanged(int index)
{
    Q_UNUSED(index);
    //is objectHint is set, we use its extension enstead of filter;
    if (m_objectHint.isEmpty()) {
        if (ui->cbFilter->currentIndex() > (m_useAllFilesFilter ? 0 : -1) ) {
            QRegExp rx("\\((.*)\\)");
            if (rx.indexIn(ui->cbFilter->currentText()) != -1)
                m_extensionAutocomplete = rx.cap(1).section(".", 1, 1);
        }
    }

    populate(m_currentUrl);
}


void StdStorageDialog::itemDoubleClicked (QTreeWidgetItem * current , int)
{
    if (!current)
        return;

    if (current->type() == fileType) {
        QString name = current->text(0);
        QString ext = current->text(1);
        ui->fileName->setText(name + "." + ext);
        m_currentFileNameIsManuallyEntered = false;
        accept();
    }
}


void StdStorageDialog::itemActivated (QTreeWidgetItem * item, int column )
{
    Q_UNUSED(column);
    if (!item)
        return;

    if (item->type() == folderType) {
        populate(item->data(0, urlRole).toString());
    }
}


void StdStorageDialog::toRootDir()
{
    if (!m_storage)
        return;
    populate(m_storage->rootUrl());
}


void StdStorageDialog::dirUp()
{
    if (ui->currentDir->text().isEmpty())
        return;
    QString currDir = ui->currentDir->text();
    currDir = currDir.section("/", 0, -2);

    populate(currDir);
}


QString StdStorageDialog::currentFolderUrl() const
{
    return m_currentUrl;
}


QString StdStorageDialog::currentObjectUrl() const
{
    QUrl curUrl(m_currentUrl);
    QString path = curUrl.path();
    return m_currentUrl + (path.isEmpty() ? "" : "/") + ui->fileName->text();
}


bool StdStorageDialog::currentObjectExists() const
{
    if (!m_storage)
        return false;
    return m_storage->objectExists(currentObjectUrl());
}


QPixmap StdStorageDialog::formPreviewPixmap(const QTreeWidgetItem *item)
{
    FormPixmapCache::iterator it = m_formPixmapCache.find(item);
    if (it == m_formPixmapCache.end()) {
        QFormBuilder builder;
        QWidget *widget;
        QByteArray ba = m_storage->loadObject(currentObjectUrl()).toByteArray();
        QBuffer buffer(&ba);
        buffer.open(QFile::ReadOnly);
        widget = builder.load(&buffer, this);
        buffer.close();
        QPixmap pixmap;
        if (!widget)
            return pixmap;
        pixmap = QPixmap::grabWidget(widget);
        widget->deleteLater();

        if (pixmap.isNull())
            return pixmap;
        it = m_formPixmapCache.insert(item, pixmap);
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


void StdStorageDialog::filenameEdited(const QString & filename)
{
    m_currentFileNameIsManuallyEntered = true;
    if (filename.contains("."))
        return;

    /// FIXME: show extension hint
//    if (!m_extensionAutocomplete.isEmpty()) {
//        ui->fileName->setText(filename + "." + m_extensionAutocomplete);
//        ui->fileName->setSelection(ui->fileName->text().length(), -m_extensionAutocomplete.length() -1);
//    }
}


void StdStorageDialog::filenameCursorPositionChanged(int oldPos, int newPos)
{
    Q_UNUSED(oldPos)
    Q_UNUSED(newPos)
//    if (!m_extensionAutocomplete.isEmpty() && ui->fileName->text().endsWith(m_extensionAutocomplete)) {
//        ui->fileName->setText(filename + "." + m_extensionAutocomplete);
//        ui->fileName->setSelection(ui->fileName->text().length(), -m_extensionAutocomplete.length() -1);
//    }
}


void StdStorageDialog::showEvent ( QShowEvent * event )
{
    if (m_storage) {
        QString hint;
        foreach (const QString & str, m_urlHints) {
            hint = m_storage->urlHint(str);
            if (!hint.isEmpty())
                break;
        }
        populate(hint.isEmpty() ? m_storage->rootUrl() : hint, hint.isEmpty() ? "" : m_storage->rootUrl() );
    }
    QDialog::showEvent(event);
}


} //namespace
