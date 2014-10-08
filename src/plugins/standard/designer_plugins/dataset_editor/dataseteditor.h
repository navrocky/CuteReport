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
#ifndef DATASETEDITOR_H
#define DATASETEDITOR_H

#include "moduleinterface.h"
#include <QPointer>
#include <QIcon>
#include <QHash>

class DatasetContainer;

namespace PropertyEditor{
class EditorWidget;
}

namespace CuteReport {
class DatasetInterface;
class ReportInterface;
}

class DatasetEditor : public CuteDesigner::ModuleInterface
{
    Q_OBJECT
    Q_INTERFACES(CuteDesigner::ModuleInterface)
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteDesigner.ModuleInterface/1.0")
#endif
public:
    explicit DatasetEditor(QObject *parent = 0);
    virtual ~DatasetEditor();

    virtual void init(CuteDesigner::Core *core);

    virtual void reloadSettings();
    virtual void saveSettings();
    virtual void activate();
    virtual void deactivate();
    virtual void sync();
    virtual QWidget * view();
    virtual QIcon icon();
    virtual QString name();
    virtual qint16 priority() {return 400;}

private slots:
    void slotRequestForCreateDataset(QString name);
    void slotRequestForRemoveDataset();
    void slotCurrentTabChanged(QString name);
    void slotRequestForPopulatedataset();
    void slotCurrentReportChanged(CuteReport::ReportInterface * report);
    void datasetNameChanged(QString name);
    void slotRequestForRename(QString currentName);

private:
    void preprocessDataset(CuteReport::DatasetInterface *dataset);
    void postprocessDataset(CuteReport::DatasetInterface *);

private:
    QPointer<DatasetContainer> ui;
    PropertyEditor::EditorWidget *m_propertyEditor;
    CuteReport::DatasetInterface * m_currentDataset;
    QHash<CuteReport::DatasetInterface*, QString> m_datasets;
    QPointer<CuteReport::ReportInterface> m_currentReport;
};

#endif // PREVIEW_H
