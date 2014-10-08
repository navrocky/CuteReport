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
#ifndef REPORTCORE_H
#define REPORTCORE_H

#include <QObject>
#include <QMap>
#include "globals.h"
#include <types.h>

//#define gCore CuteReport::ReportCore::instance()

//#define log_debug(message, ...)  CuteReport::ReportCore::instance()->log(LogDebug, (message), ##__VA_ARGS__)
//#define log_info(message, ...)  CuteReport::ReportCore::instance()->log(LogInfo, (message), ##__VA_ARGS__)
//#define log_error(message, ...) CuteReport::ReportCore::instance()->log(LogError, (message), ##__VA_ARGS__)
//#define log_warning(message, ...)  CuteReport::ReportCore::instance()->log(LogWarning, (message), ##__VA_ARGS__)
//#define log_critical(message, ...)  CuteReport::ReportCore::instance()->log(LogCritical, (message), ##__VA_ARGS__)


namespace CuteReport
{

class DatasetInterface;
class PageInterface;
class BaseItemInterface;
class StorageInterface;
class StorageOptionsInterface;
class RendererInterface;
class RenderedPageInterface;
class PrinterInterface;
class FormInterface;
class SerializerInterface;
class ImportInterface;
class ExportInterface;
class Log;
class ReportInterface;
class RenderedReportInterface;
class ReportPluginInterface;
class ScriptExtensionInterface;
class DesignerItemInterface;

enum BaseType {BasedOnTemplate, BasedOnReport};
enum RenderingDestination { RenderToPreview, RenderToExport, RenderToPrinter};

struct QueueReport {
    explicit QueueReport();
    virtual ~QueueReport();
    ReportInterface * report;
    QString url;
    RenderingDestination destination;
    bool success;
};


struct QueueReportExport: public QueueReport {
    QString format;
    QString outputUrl;
    QStringList options;
};


struct QueueReportPrint: public QueueReport {
};


class CUTEREPORT_EXPORTS ReportCore : public QObject
{
    Q_OBJECT

public:
    explicit ReportCore(QObject *parent = 0, QSettings *settings = 0, bool interactive = false, bool initLogSytem = true);
    explicit ReportCore(QSettings *settings, bool interactive = false);
    ~ReportCore();

    int versionMajor() const;
    int versionMinor() const;

    /** shows can report be changed or no */
    bool interactive() const;

    /*! syncing all storages and other data with remote or local systems
     *  emits finished() signal
     * do not operate with ReportCore while you have no finished() signal
     * read meassage() for have information;
    */
    void finish();

    /** reportCore takes ownership of defaultStorage */
    void setDefaultStorage(const QString &storageName);
    bool setDefaultStorageByScheme(const QString & scheme);
    CuteReport::StorageInterface * defaultStorage() const;

    void setDefaultPrinter(const QString moduleName);
    CuteReport::PrinterInterface * defaultPrinter() const;

    void setDefaultRenderer(const QString &moduleName);
    CuteReport::RendererInterface * defaultRenderer() const;

    /** Root widget where all dialogs will be centered on
     *  usually it's a main window
     **/
    void setRootWidget(QWidget * widget);
    QWidget * rootWidget();

    QStringList moduleNames(ModuleType moduleType) const;
    const ReportPluginInterface *module(ModuleType moduleType, const QString &moduleName) const;

    QStringList moduleOptions(ReportPluginInterface * module);
    void setModuleOptions(ReportPluginInterface * module, const QStringList & options);
    QString moduleOptionsStr(ReportPluginInterface * module, const QString & delimiter = ",");
    void setModuleOptionsStr(ReportPluginInterface * module, const QString & options, const QString & delimiter = ",");

    const QList<BaseItemInterface *> &                   itemModules() const;
    const QList<CuteReport::PageInterface*> &            pageModules() const;
    const QList<CuteReport::DatasetInterface*> &         datasetModules() const;
    const QList<CuteReport::StorageInterface*> &         storageModules() const;
    const QList<CuteReport::RendererInterface*> &        rendererModules() const;
    const QList<CuteReport::PrinterInterface*> &         printerModules() const;
    const QList<CuteReport::FormInterface*> &            formModules() const;
    const QList<CuteReport::ImportInterface*> &          importModules() const;
    const QList<CuteReport::ExportInterface*> &          exportModules() const;
    const QList<CuteReport::ScriptExtensionInterface*> & scriptExtensionModules() const;

    QList<StorageInterface *> storageObjectList(CuteReport::ReportInterface * report = 0) const;

    const BaseItemInterface*                itemModule(const QString & moduleName  = QString()) const;
    const CuteReport::PageInterface*        pageModule(const QString & moduleName  = QString()) const;
    const CuteReport::DatasetInterface*     datasetModule(const QString & moduleName  = QString()) const;
    CuteReport::SerializerInterface*        serializerModule(const QString & moduleName  = QString()) const;
    CuteReport::StorageInterface*           storageModule(const QString & moduleName = QString()) const;
    CuteReport::StorageInterface*           storageModuleByScheme(const QString & scheme) const;
    CuteReport::RendererInterface*          rendererModule(const QString & moduleName = QString()) const; //default if empty
    CuteReport::PrinterInterface*           printerModule(const QString & moduleName = QString()) const; //default if empty
    CuteReport::FormInterface*              formModule(const QString & moduleName = QString()) const;
    const CuteReport::ImportInterface*      importModule(const QString & moduleName = QString()) const;
    CuteReport::ExportInterface*            exportModule(const QString & moduleName = QString()) const;

    PageInterface *     pageByName(const QString & pageName, CuteReport::ReportInterface * report) const;
    BaseItemInterface * itemByName(const QString & itemName, CuteReport::PageInterface * page = 0) const; //if page is 0, look at all pages
    BaseItemInterface * itemByName(const QString & itemName, const QString & pageName, CuteReport::ReportInterface * report) const;
    DatasetInterface *  datasetByName(const QString & datasetName, CuteReport::ReportInterface * report) const;
    FormInterface *     formByName(const QString & formName, CuteReport::ReportInterface * report) const;

    const QList<ReportInterface *> &reports() const;
    CuteReport::ReportInterface *   createReport();
    void                            deleteReport(CuteReport::ReportInterface * report);

    CuteReport::PageInterface *     createPageObject(CuteReport::ReportInterface *report, const QString & moduleName = QString());
    CuteReport::DatasetInterface *  createDatasetObject(CuteReport::ReportInterface *report, const QString & moduleName = QString());
    BaseItemInterface *             createItemObject(CuteReport::ReportInterface *report, const QString & moduleName, QObject * parent = 0);
    CuteReport::PrinterInterface *  createPrinterObject(CuteReport::ReportInterface *report, const QString & moduleName = QString());
    CuteReport::StorageInterface *  createStorageObject(CuteReport::ReportInterface *report, const QString & moduleName = QString());
    CuteReport::RendererInterface * createRendererObject(CuteReport::ReportInterface *report, const QString & moduleName = QString());
    CuteReport::FormInterface *     createFormObject(CuteReport::ReportInterface *report, const QString & moduleName = QString());
    CuteReport::ExportInterface *   createExportObject(CuteReport::ReportInterface *report, const QString & moduleName = QString());

    QByteArray  serialize(const QObject * object, bool *ok = 0, QString * error = 0, const QString & moduleName = QString());
    QObject *   deserialize(const QByteArray &data, bool *ok = 0, QString * error = 0, const QString & moduleName = QString());

    /*! Storage methods */
    CuteReport::StorageInterface * getStorageByUrl(const QString & urlString, CuteReport::ReportInterface * report) const;
    CuteReport::StorageInterface * getStorageByName(const QString & storageName, CuteReport::ReportInterface * report) const;

    bool saveReport(const QString & urlString, CuteReport::ReportInterface * report, QString * errorText = 0);

    CuteReport::ReportInterface * loadReport(const QString & urlString,
                                             QString * errorText = 0);

    bool        saveObject(const QString & urlString,
                            CuteReport::ReportInterface * report,
                            const QVariant & objectData,
                            QString * errorText = 0);

    QVariant    loadObject(const QString & urlString,
                              CuteReport::ReportInterface * report,
                              QString * errorText = 0);

    QString localCachedFileName(const QString & url, CuteReport::ReportInterface * report);

    /*! Renderer methods */
    const QList<QString> renderers();
    bool render(ReportInterface* report);
    bool render(const QString & reportUrl);
    void stopRendering(ReportInterface *report);
    int rendererTotalPages(ReportInterface *report) const;
    RenderedPageInterface * rendererGetPage(ReportInterface *report, int number) const;
    void renderDataClear(ReportInterface * report);
    void renderDataclearAll();

    /*! Printer methods */
    void print(ReportInterface* report);
    void print(const QString url);

    /*! Import methods */
    QStringList importExtensions() const;
    bool canImport(const QString &reportUrl) const;
    QStringList importModulesForFile(const QString &reportUrl) const;
    ReportInterface *import(const QString &reportUrl, const QString & moduleName = QString()) const;

    /*! Export methods
    options is in format parameter=value;parameter2=value2
    space symbol is allowed */
    void exportTo(ReportInterface* report, const QString & format, const QString & outputUrl = QString(), const QStringList &options = QStringList());
    void exportTo(const QString &reportUrl, const QString & format, const QString & outputUrl = QString(), const QStringList & options = QStringList());

    static void log(LogLevel level, const QString & sender, const QString & message);
    static void log(LogLevel level, const QString & sender, const QString & shortMessage, const QString & fullMessage);

    static bool isNameUnique(QObject *object, const QString &name, QObject * rootObject);

    int maxRenderingThreads() const;
    void setMaxRenderingThreads(int maxRenderingThreads);

    //deleting of the interface object is designer's responcibility
    void registerDesignerInterface(CuteReport::DesignerItemInterface * _interface);
    CuteReport::DesignerItemInterface * designerInterface() const;


    CuteReport::StorageInterface * getStorageModule(const QString & moduleName, QString * errorText = 0) const;
    CuteReport::RendererInterface * getRenderer(const ReportInterface *report) const;

    QSettings * settings();

    /** always return path without ending slash */
    QString resourcesPath() const;
    QString imagesPath() const;
    QString pluginsPath() const;

    static QString uniqueName(QObject * object, const QString &proposedName, QObject *rootObject);

public slots:
    void sendMetric(CuteReport::MetricType type, const QVariant &value);

signals:
    void finished(bool);
    void syncMessage(const QString & moduleName, const QString & message);

    void rendererStarted(CuteReport::ReportInterface * report);
    void rendererDone(CuteReport::ReportInterface * report, bool success);
    void rendererDone(const QString & reportUrl, bool success);
    void rendererMessage(CuteReport::ReportInterface * report, int logLevel, QString message);
    void rendererProcessingPage(CuteReport::ReportInterface * report, int page, int total);
    void rendererProcessedPage(CuteReport::ReportInterface * report, CuteReport::RenderedPageInterface * page, int pageNumber);

    void printingDone(CuteReport::ReportInterface * report, bool successfully);
    void printingDone(const QString & reportUrl, bool successfully);

    void exportDone(CuteReport::ReportInterface * report, bool successfully);
    void exportDone(const QString & reportUrl, bool successfully);

    void reportObjectCreated(CuteReport::ReportInterface * report);
    void reportObjectDestroyed(CuteReport::ReportInterface * report);
    void metricUpdated(CuteReport::MetricType type, const QVariant &value);

private slots:
    void _rendererStarted();
    void _rendererDone(bool successful);
    void _rendererMessage(int logLevel, QString message);
    void _rendererProcessingPage(int page, int total);

    void _reportObjectCreated(CuteReport::ReportInterface * report);

private:
    static bool loadPlugins(QSettings *settings);

    void init(QSettings * settings, bool initLogSystem);
    inline bool checkReportPointer(CuteReport::ReportInterface * report, QString * errorText = 0) const;
    bool _render(QueueReport *queueReport);
    void _renderDone(QueueReport *queueReport);
    void _export(QueueReport *queueReport);
    void _exportDone(QueueReport *queueReport);
    void _print(QueueReport *queueReport);
    void _printDone(QueueReport *queueReport);

private:
    static int m_refCount;

    // TODO: v2 - change  each list to work with CuteReport::ReportPluginInterface *
    // and make methods simplier
    static QList<BaseItemInterface*> * m_itemPlugins;
    static QList<PageInterface*> * m_pagePlugins;
    static QList<DatasetInterface*> * m_datasetPlugins;
    static QList<StorageInterface*> * m_storagePlugins;
    static QList<SerializerInterface*> * m_serializePlugins;
    static QList<RendererInterface*> * m_rendererPlugins;
    static QList<PrinterInterface*> * m_printerPlugins;
    static QList<FormInterface*> * m_formPlugins;
    static QList<ImportInterface*> * m_importPlugins;
    static QList<ExportInterface*> * m_exportPlugins;
    static QList<ScriptExtensionInterface*> * m_scriptExtensionPlugins;

    SerializerInterface*    m_defaultSerializer;
    StorageInterface*       m_defaultStorage;
    PrinterInterface*       m_defaultPrinter;
    RendererInterface*      m_defaultRenderer;

    QWidget* m_rootWidget;

    QList<SerializerInterface*> m_serializers;
    QList<StorageInterface*>    m_storages;
    QList<PrinterInterface*>    m_printers;
    QList<RendererInterface*>   m_renderers;
    QList<ExportInterface*>     m_exporters;

    QSettings * m_settings;
    bool m_interactive;

    QList<QueueReport*> m_waitingQueue;
    QHash<RendererInterface*, QueueReport*> m_renderingQueue;
    int m_maxRenderingThreads;

    DesignerItemInterface * m_designerInterface;
};

}
#endif // REPORTCORE_H
