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
#ifndef REPORTEDITOR_H
#define REPORTEDITOR_H

#include "moduleinterface.h"
#include "reportproperties.h"
#include "propertyeditor.h"
#include <QPointer>
#include <QIcon>
#include <QHash>

class ReportContainer;
class TemplateDialog;

namespace PropertyEditor{
    class ReportProperties;
    class EditorWidget;
}

namespace CuteReport {
class PrinterInterface;
class PrinterOptionsInterface;
class RendererInterface;
class RendererOptionsInterface;
class ReportInterface;
class StorageInterface;
}


struct ReportStruct
{
    explicit ReportStruct(): reportProperties(0), rendererPropertyEditor(0), printerPropertyEditor(0) {}
    CuteReport::ReportInterface * report;
    QString tabText;
    QPointer<PropertyEditor::ReportProperties> reportProperties;
    QPointer<PropertyEditor::EditorWidget> rendererPropertyEditor;
    QPointer<PropertyEditor::EditorWidget> printerPropertyEditor;
};

class ReportEditor : public CuteDesigner::ModuleInterface
{
    Q_OBJECT
    Q_INTERFACES(CuteDesigner::ModuleInterface)
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteDesigner.ModuleInterface/1.0")
#endif

public:
    explicit ReportEditor(QObject *parent = 0);
    ~ReportEditor();

    virtual void init(CuteDesigner::Core *core);

    virtual void reloadSettings();
    virtual void saveSettings();
    virtual void activate();
    virtual void deactivate();
    virtual void sync();
    virtual QWidget * view();
    virtual QIcon icon();
    virtual QString name();
    virtual qint16 priority() {return 100;}

    virtual QList<CuteDesigner::DesignerMenu*> mainMenu();

private slots:
    void loadReport(const QString & objectUrl);
    void slotRequestForTemplate();
    void slotRequestForNewReport();
    void slotRequestForOpenReport();
    void slotRequestForSaveReport();
    void slotRequestForSaveReportAs();
    void slotRequestForCloseReport();
    void slotCurrentReportChangedByCore(CuteReport::ReportInterface*report);
    void slotCurrentTabChanged(int index);
    void slotPrinterChanged(CuteReport::PrinterInterface *printer);
    void slotRendererChanged(CuteReport::RendererInterface *renderer);
    void slotRequestForNewRenderer(const CuteReport::RendererInterface* renderer);
    void slotRequestForNewPrinter(const CuteReport::PrinterInterface* printer);
    void slotRequestForNewStorage(const CuteReport::StorageInterface* storage);
    void slotRequestForDeleteStorage(const QString & storageName);
    void slotRequestForChangeDefaultStorage(const QString & storageName);
    void slotRequestForDeletePrinter();
    void slotRequestForDeleteRenderer();
    void slotReportObjectDestroyed(QObject * report);
//    void slotRequestForRenameReport(int index);
    void slotReportNameChangedOutside(QString name);
    void slotDirtynessChanged(bool isDirty);
    void slotCoreReportCreated(CuteReport::ReportInterface*report);
    void slotSetCurrentReport();
    void slotDesignerInitDone();
    void slotAppIsAboutToClose();

private:
    bool saveReport(CuteReport::ReportInterface *report, bool askFileName = true);
    void newReportPreprocess(CuteReport::ReportInterface * report);
    QString makeCorrectObjectName(CuteReport::ReportInterface * report);
    CuteReport::ReportInterface* sameReportExists(CuteReport::ReportInterface* newReport);

private:
    QPointer<ReportContainer> ui;
    TemplateDialog * m_templateDialog;
    QList<ReportStruct> m_reports;
    typedef QList<ReportStruct>::iterator ReportIterator;
    QPointer<CuteReport::ReportInterface> m_reportToSwitch;
};


//SUIT_END_NAMESPACE
#endif // REPORTEDITOR_H
