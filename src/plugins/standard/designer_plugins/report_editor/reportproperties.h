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
#ifndef REPORTPROPERTIES_H
#define REPORTPROPERTIES_H

#include <QWidget>
#include <QPointer>
#include "core.h"

namespace Ui {
class ReportProperties;
}

namespace PropertyEditor{
    class EditorWidget;
}

namespace CuteReport {
class RendererInterface;
class PrinterInterface;
class StorageInterface;
class StorageHelperInterface;
}

class QTreeWidgetItem;
class QTableWidgetItem;

namespace PropertyEditor{

class ReportProperties : public QWidget
{
    Q_OBJECT    
public:
    explicit ReportProperties(CuteDesigner::Core * core, QWidget *parent = 0);
    ~ReportProperties();

    void reloadSettings();
    void saveSettings();

    void addRendererPropertyEditor(PropertyEditor::EditorWidget * propertyEditor);
    void addPrinterPropertyEditor(PropertyEditor::EditorWidget * propertyEditor);

public slots:
    void saveAll();
    void connectReport(CuteReport::ReportInterface * report);
    void disconnectReport();

//    void syncData();

signals:
    void requestForNewRenderer( const CuteReport::RendererInterface*);
    void requestForNewPrinter( const CuteReport::PrinterInterface*);
    void requestForNewStorage( const CuteReport::StorageInterface*);
    void requestForDeleteStorage( const QString & name );
    void requestForDefaultStorage( const QString & name );
    void requestForDeletePrinter();
    void requestForDeleteRenderer();

private slots:
    void setGUIReportFilePath(const QString & url);
    void setGUIReportName(const QString & reportName);
    void setGUIReportAuthor(const QString & reportAuthor);
    void setGUIReportDescription(const QString & reportDescription);
    void setGUIReportPrinter(CuteReport::PrinterInterface* printer);
    void setGUIReportRenderer(CuteReport::RendererInterface* renderer);
    void addGUIReportStorage(CuteReport::StorageInterface* storage);
    void removeGUIReportStorage(CuteReport::StorageInterface* storage);
    void setGUIDefaultStorage(const QString & storageName);
    void updateGUIvariables();

    void setNewRendererModule();
    void setNewPrinterModule();
    void setNewStorageModule();
    void deleteCurrentStorage();
    void setDefaultStorage();
    void clearDefaultStorage();

    void storagesListIndexChanged(QTreeWidgetItem* current,QTreeWidgetItem*previous);
    void variableItemChanged(QTableWidgetItem*item);

private:
    void updateLayout();

    Ui::ReportProperties *ui;
    CuteDesigner::Core * m_core;
    QPointer<CuteReport::ReportInterface> m_report;
    QPointer<CuteReport::StorageHelperInterface> m_currentStorageHelper;
    QPointer<EditorWidget> m_rendererPropertyEditor;
    QPointer<EditorWidget> m_printerPropertyEditor;
};

}
#endif // REPORTPROPERTIES_H
