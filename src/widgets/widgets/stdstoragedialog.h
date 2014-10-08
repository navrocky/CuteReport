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
#ifndef STDSTORAGEDIALOG_H
#define STDSTORAGEDIALOG_H

#include "widgets_export.h"
#include "cutereport.h"
#include <QDialog>
#include <QPointer>
#include <QTreeWidgetItem>


namespace Ui {
class StdStorageDialog;
}

//class QTreeWidgetItem;

namespace CuteReport {

class StorageInterface;
class ReportInterface;
class ReportCore;


class WIDGET_EXPORTS StdStorageDialog : public QDialog
{
    Q_OBJECT
    
public:
    enum ViewOption {
        ShowNothing             = 0x00,
        ShowStorageSelect       = 0x01,
        ShowAll             = 0xff
    };
    Q_DECLARE_FLAGS(ViewOptions, ViewOption)

    explicit StdStorageDialog(ReportCore * reportCore, QWidget *parent, const QString &windowTitle = QString());
    explicit StdStorageDialog(ReportCore * reportCore, ReportInterface * report, QWidget *parent = 0, const QString &windowTitle = QString());
    ~StdStorageDialog();

    void setViewOptions(ViewOptions f);
    void setExtensionAutocomplete(bool b);

    void setCurrentStorage(const QString & storageName, bool exclusive = false);
//    void setCurrentStorageByName(const QString & storageScheme, bool exclusive = false);
    QString currentStorageName();

    /** set default path(s) if it found */
    void setUrlHint(const QString & urlHint);
    void setUrlHints(const QStringList & urlHints); // first has more priority

    /** highlights this file if found */
    void setObjectHint(const QString & objectHint);

    void setNameFilters(const QStringList & nameFilters, bool addAllFilesOption = true);
    QString currentNameFilter() const;

    void setRootDir(const QString & dir, bool canGoOutRoot = false);
    QString rootDir() const;
//    void setDirFilters(QDir::Filters filters);
//    QDir::Filters dirFilters() const;
    QString currentFolderUrl() const;
    QString currentObjectUrl() const;
    bool currentObjectExists() const;

    void populate(const QString &_url, const QString &urlIfEmpyTryThis = QString());

private slots:
    void currentStoragetIndexChanged(int index);
    void currentFultertIndexChanged(int index);
    void currentItemChanged ( QTreeWidgetItem * current, QTreeWidgetItem * previous );
    void itemDoubleClicked ( QTreeWidgetItem * current, int);
    void itemActivated ( QTreeWidgetItem * item, int column );
    void toRootDir();
    void dirUp();
    void showEvent ( QShowEvent * event );
    void filenameEdited(const QString & filename);
    void filenameCursorPositionChanged(int oldPos, int newPos);

private:
    void initMe();
    QPixmap formPreviewPixmap(const QTreeWidgetItem *item);

    Ui::StdStorageDialog *ui;
    CuteReport::ReportCore * m_reportCore;
    QPointer<StorageInterface> m_storage;
    QPointer<ReportInterface> m_report;
    QStringList m_urlHints;
    QString m_objectHint;
    QString m_rootDir;
    bool m_canGoOutRoot;
    QString m_currentUrl;
    QString m_previousUrl;
    typedef QMap<const QTreeWidgetItem *, QPixmap> FormPixmapCache;
    FormPixmapCache m_formPixmapCache;
    bool m_useAllFilesFilter;
    QString m_extensionAutocomplete;
    bool m_currentFileNameIsManuallyEntered;
};

} //namespace

Q_DECLARE_OPERATORS_FOR_FLAGS(CuteReport::StdStorageDialog::ViewOptions)

#endif // STDSTORAGEDIALOG_H
