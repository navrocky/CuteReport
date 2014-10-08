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
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/
#ifndef DATASETCONTAINER_H
#define DATASETCONTAINER_H

#include <QWidget>
#include <QPointer>

namespace Ui {
class DatasetContainer;
}

namespace CuteReport {
class DatasetInterface;
}

namespace CuteDesigner {
class Core;
}

class QAbstractItemModel;
class DatasetEditor;

class DatasetContainer : public QWidget
{
    Q_OBJECT
    
public:
    explicit DatasetContainer(DatasetEditor * datasetEditor, QWidget *parent = 0);
    ~DatasetContainer();

    void reloadSettings();
    void saveSettings();

    void addDatasetPlugins(QList<CuteReport::DatasetInterface*> datasets);
    void addTab(QWidget *const widget, const QIcon &icon, const QString &name);
    void deleteTab(QString name);
    void deleteAllTabs();
    void changeTabText(QString oldText, QString newText);

    void addPropertyEditor(QWidget * widget);
    void setModel(QAbstractItemModel * model);
    void setError(QString error);
    void setCurrentTab(QString name);

signals:
    void requestForCreateDataset(QString name);
    void requestForDeleteDataset();
    void currentTabChanged(QString name);
    void requestForRename(QString name);
    void requestForPopulateDataset();

private slots:
    void slotNewDataset(int index);
    void slotCurrentDatasetChanged(int index);
    void slotTabDoubleClicked(int index);
    void slotPropertyButtonToggled(bool b);
    void slotShowDataPage();
    void slotShowOptionsPage();
    void populateClicked();

private:
    Ui::DatasetContainer *ui;
    DatasetEditor * m_datasetEditor;
    QWidget * m_propertyEditor;
};

#endif // DATASETCONTAINER_H
