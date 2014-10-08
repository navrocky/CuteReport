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
#include "reportcore.h"
#include "baseiteminterface.h"
#include "pageinterface.h"
#include "datasetinterface.h"
#include "reportplugininterface.h"
#include "reportinterface.h"
#include "storageinterface.h"
#include "rendererinterface.h"
#include "printerinterface.h"
#include "forminterface.h"
#include "serializerinterface.h"
#include "importinterface.h"
#include "exportinterface.h"
#include "types.h"
#include "log/log.h"
#include "renderedreportinterface.h"
#include "scriptextensioninterface.h"
#include "designeriteminterface.h"

#include <QApplication>
#include <QtCore>
#include <QUrl>
#include <QtXml>

void * gDesigner = 0;

static const QString MODULENAME = "ReportCore";
static const int MaxRenderingThreads = 100;


template <class T>
void processModuleList(QList<T*> & modules)
{
    QStringList removeList;
    foreach (T * module, modules) {
        removeList.append(module->removesModules());
    }
    QMutableListIterator<T*> i(modules);
    while (i.hasNext()) {
        T* module = i.next();
        if (removeList.contains(module->moduleFullName()))
            i.remove();
    }
    foreach (T * module, modules)  module->moduleInit();
}

template <class T>
void processModuleList(QList<T*> * modules)
{
    QStringList removeList;
    foreach (T * module, *modules) {
        removeList.append(module->removesModules());
    }
    QMutableListIterator<T*> i(*modules);
    while (i.hasNext()) {
        T* module = i.next();
        if (removeList.contains(module->moduleFullName()))
            i.remove();
    }
    foreach (T * module, *modules)  module->moduleInit();
}


namespace CuteReport
{


int                                 ReportCore::m_refCount = 0;
QList<BaseItemInterface*> *         ReportCore::m_itemPlugins = 0;
QList<PageInterface*> *             ReportCore::m_pagePlugins;
QList<DatasetInterface*> *          ReportCore::m_datasetPlugins;
QList<StorageInterface*> *          ReportCore::m_storagePlugins;
QList<SerializerInterface*> *       ReportCore::m_serializePlugins;
QList<RendererInterface*> *         ReportCore::m_rendererPlugins;
QList<PrinterInterface*> *          ReportCore::m_printerPlugins;
QList<FormInterface*> *             ReportCore::m_formPlugins;
QList<ImportInterface*> *           ReportCore::m_importPlugins;
QList<ExportInterface*> *           ReportCore::m_exportPlugins;
QList<ScriptExtensionInterface*> *  ReportCore::m_scriptExtensionPlugins;


QueueReport::QueueReport()
    : report(0), success(true)
{

}

QueueReport::~QueueReport()
{
    if (!url.isEmpty()) report->deleteLater();
}


ReportCore::ReportCore(QObject *parent, QSettings * settings, bool interactive, bool initLogSytem) :
    QObject(parent),
    m_interactive(interactive)
{
    init(settings, initLogSytem);
}


ReportCore::ReportCore(QSettings *settings, bool interactive) :
    QObject(0),
    m_interactive(interactive)
{
    init(settings, true);
}


ReportCore::~ReportCore()
{
    --m_refCount;

    qDeleteAll(m_serializers);
    qDeleteAll(m_storages);
    qDeleteAll(m_printers);
    qDeleteAll(m_renderers);
    qDeleteAll(m_exporters);

    if (m_refCount == 0) {
        qDeleteAll(*m_itemPlugins);
        delete m_itemPlugins;
        qDeleteAll(*m_pagePlugins);
        delete m_pagePlugins;
        qDeleteAll(*m_datasetPlugins);
        delete m_datasetPlugins;
        qDeleteAll(*m_storagePlugins);
        delete m_storagePlugins;
        qDeleteAll(*m_serializePlugins);
        delete m_serializePlugins;
        qDeleteAll(*m_rendererPlugins);
        delete m_rendererPlugins;
        qDeleteAll(*m_printerPlugins);
        delete m_printerPlugins;
        qDeleteAll(*m_formPlugins);
        delete m_formPlugins;
        qDeleteAll(*m_importPlugins);
        delete m_importPlugins;
        qDeleteAll(*m_exportPlugins);
        delete m_exportPlugins;
        qDeleteAll(*m_scriptExtensionPlugins);
        delete m_scriptExtensionPlugins;
    }

    if (Log::instance())
        Log::refCounterDec();
}


int ReportCore::versionMajor() const
{
    return QString(REPORT_VERSION).section('.', 0, 0).toInt();
}


int ReportCore::versionMinor() const
{
    return QString(REPORT_VERSION).section('.', 1, 1).toInt();
}


bool ReportCore::interactive() const
{
    return m_interactive;
}


void ReportCore::finish()
{
    //TODO: implement sycing Storage modules with remote data
    emit finished(true);
}


bool ReportCore::checkReportPointer(CuteReport::ReportInterface * report, QString * errorText) const
{
    Q_UNUSED(errorText)

    try {
        report->isValid();
    }
    catch (...)
    {
        return false;
    }
    return true;
}


CuteReport::StorageInterface * ReportCore::getStorageModule(const QString & moduleName, QString * errorText) const
{
    CuteReport::StorageInterface * module = moduleName.isEmpty() ? m_defaultStorage : storageModule(moduleName);

    if (!module) {
        QString error = tr("Storage Module name \'%1\' not found").arg(module->moduleFullName());
        if (errorText)
            *errorText = error;
        log(LogWarning, MODULENAME, error);
        return 0;
    }
    return module;
}


CuteReport::RendererInterface * ReportCore::getRenderer(const CuteReport::ReportInterface * report) const
{
    CuteReport::RendererInterface * module = report->renderer();
    if (!module)
        module = m_defaultRenderer;
    return module;
}


QSettings *ReportCore::settings()
{
    return m_settings;
}


QString ReportCore::resourcesPath() const
{
    QString path = m_settings->value("CuteReport/ResourcesPath").toString();
    if (!path.isEmpty())
        return QDir::cleanPath(path);
    path = m_settings->value("CuteReport/RootPath").toString();
    return QDir::cleanPath(path.isEmpty() ? REPORT_RESOURCES_PATH : path + "/resources");
}


QString ReportCore::imagesPath() const
{
    QString path = m_settings->value("CuteReport/ImagesPath").toString();
    if (!path.isEmpty())
        return QDir::cleanPath(path);
    path = m_settings->value("CuteReport/RootPath").toString();
    return QDir::cleanPath(path.isEmpty() ? REPORT_IMAGES_PATH : path + "/images");
}


QString ReportCore::pluginsPath() const
{
    QString path = m_settings->value("CuteReport/PluginsPath").toString();
    if (!path.isEmpty())
        return QDir::cleanPath(path);
    path = m_settings->value("CuteReport/RootPath").toString();
    return QDir::cleanPath(path.isEmpty() ? REPORT_PLUGINS_PATH : path + "/plugins");
}


CuteReport::StorageInterface * ReportCore::getStorageByUrl(const QString & urlString, CuteReport::ReportInterface * report) const
{
    QUrl url(urlString);
    QString urlScheme = url.scheme();

    CuteReport::StorageInterface * module = 0;

    /// by first try report's storage
    if (report) {
        QList<CuteReport::StorageInterface *> storages = report->storages();
        if (urlScheme.isEmpty()) {
            module = report->defaultStorage();
        }

        if (!module) {
            foreach (CuteReport::StorageInterface * storage, storages)
                if (storage->objectName() == urlScheme) {
                    module = storage;
                    break;
                }
        }
    }

    /// next looking for global's default storages
    if ((!module && m_defaultStorage && ((m_defaultStorage->urlScheme() == urlScheme))) || urlScheme.isEmpty())
        module = m_defaultStorage;

    if (!module) {
        foreach (CuteReport::StorageInterface * m, *m_storagePlugins) {
            if (m->urlScheme() == urlScheme)
                return m;
        }
    }

    return module;
}


CuteReport::StorageInterface * ReportCore::getStorageByName(const QString & storageName, CuteReport::ReportInterface * report) const
{
    CuteReport::StorageInterface * module = 0;

    /// by first try report's storage
    if (report) {
        QList<CuteReport::StorageInterface *> storages = report->storages();
        if (storageName.isEmpty()) {
            module = report->defaultStorage();
        }

        if (!module) {
            foreach (CuteReport::StorageInterface * storage, storages)
                if (storage->objectName() == storageName) {
                    module = storage;
                    break;
                }
        }
    }

    /// next looking for global's default storages
    if ((!module && m_defaultStorage && ((m_defaultStorage->objectName() == storageName))) || storageName.isEmpty())
        module = m_defaultStorage;

    if (!module) {
        foreach (CuteReport::StorageInterface * m, m_storages) {
            if (m->objectName() == storageName)
                return m;
        }
    }

    return module;
}


void ReportCore::setDefaultStorage(const QString & storageName)
{
    StorageInterface * newStorage = storageModule(storageName);
    if (newStorage)
        m_defaultStorage = newStorage;
}


bool ReportCore::setDefaultStorageByScheme(const QString & scheme)
{
    CuteReport::StorageInterface* plugin = storageModuleByScheme(scheme);
    if (!plugin)
        return false;

    setDefaultStorage(plugin->moduleFullName());
    return true;
}


CuteReport::StorageInterface * ReportCore::defaultStorage() const
{
    return m_defaultStorage;
}


void ReportCore::setDefaultPrinter(const QString moduleName)
{
    PrinterInterface * newPrinter = printerModule(moduleName);
    if (newPrinter)
        m_defaultPrinter = newPrinter;
}


CuteReport::PrinterInterface * ReportCore::defaultPrinter() const
{
    return m_defaultPrinter;
}


void ReportCore::setDefaultRenderer(const QString & moduleName)
{
    RendererInterface * newRenderer = rendererModule(moduleName);
    if (newRenderer)
        m_defaultRenderer = newRenderer;
}


CuteReport::RendererInterface * ReportCore::defaultRenderer() const
{
    return m_defaultRenderer;
}


void ReportCore::setRootWidget(QWidget * widget)
{
    m_rootWidget = widget;
}


QWidget * ReportCore::rootWidget()
{
    return m_rootWidget;
}


void ReportCore::init(QSettings *settings, bool initLogSystem)
{
    m_designerInterface = 0;
    m_rootWidget = 0;
    m_maxRenderingThreads = MaxRenderingThreads;
    m_settings = settings;

    if (initLogSystem) {
        Log::createInstance(m_settings);
    }
    /** FIXME - log can be inited by other ReportCore instance. as temporary solution we still track reference counter */
    Log::refCounterInc();

    if (!m_settings) {
        QString orgName = QApplication::organizationName();
        QString appName = QApplication::applicationName();
        m_settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                                   orgName.isEmpty() ? "ExaroLogic" : orgName,
                                   appName.isEmpty() ? "CuteReport" : appName,
                                   this);
    }

    log(CuteReport::LogDebug, MODULENAME, "Version", QString("CuteReport version: %1").arg(REPORT_VERSION));

    bool pluginsResultOk = true;
    if (m_refCount == 0)
        pluginsResultOk = loadPlugins(m_settings);
    else
        log(CuteReport::LogDebug, MODULENAME, "Plugins are already preloaded!");

    if (!pluginsResultOk) {
        log(CuteReport::LogCritical, MODULENAME, "Application is exiting because of critial error in CuteReport!");
        exit(1);
    }

    foreach (SerializerInterface * m, *m_serializePlugins) {
        SerializerInterface * localModule = m->createInstance(this);
        localModule->setReportCore(this);
        m_serializers.append(localModule);

        setModuleOptionsStr(localModule,
                            m_settings->value(QString("CuteReport/Serializer_%1_options")
                                              .arg(localModule->moduleFullName()
                                                   .replace("::","_"))).toString());
    }

    foreach (StorageInterface * m, *m_storagePlugins) {
        StorageInterface * localModule = m->createInstance(this);
        localModule->setReportCore(this);
        localModule->setObjectName(localModule->urlScheme());
        m_storages.append(localModule);

        setModuleOptionsStr(localModule,
                            m_settings->value(QString("CuteReport/Storage_%1_options")
                                              .arg(localModule->moduleFullName()
                                                   .replace("::","_"))).toString());
    }

    foreach (PrinterInterface * m, *m_printerPlugins) {
        PrinterInterface * localModule = m->createInstance(this);
        localModule->setReportCore(this);
        m_printers.append(localModule);

        setModuleOptionsStr(localModule,
                            m_settings->value(QString("CuteReport/Printer_%1_options")
                                              .arg(localModule->moduleFullName()
                                                   .replace("::","_"))).toString());
    }

    foreach (RendererInterface * m, *m_rendererPlugins) {
        RendererInterface * localModule = m->createInstance(this);
        localModule->setReportCore(this);
        m_renderers.append(localModule);

        setModuleOptionsStr(localModule,
                            m_settings->value(QString("CuteReport/Renderer_%1_options")
                                              .arg(localModule->moduleFullName()
                                                   .replace("::","_"))).toString());
    }

//    foreach (ImportInterface * m, *m_importPlugins) {
//        ImportInterface * localModule = m->createInstance(this);
//        localModule->setReportCore(this);
//        m_importPlugins->append(localModule);
//    }

    foreach (ExportInterface * m, *m_exportPlugins) {
        ExportInterface * localModule = m->createInstance(this);
        localModule->setReportCore(this);
        m_exporters.append(localModule);
    }

    m_defaultSerializer = serializerModule(m_settings->value("CuteReport/PrimarySerializer").toString());
    if (!m_defaultSerializer) {
        foreach (SerializerInterface * m, m_serializers)
            if (m->moduleFullName() == "Standard::XML") {
                m_defaultSerializer = m;
                break;
            }
    }
    if (!m_defaultSerializer) {
        foreach (SerializerInterface * m, m_serializers)
            if (m->moduleShortName().toUpper() == "XML") {
                m_defaultSerializer = m;
                break;
            }
    }
    if (!m_defaultSerializer) m_defaultSerializer = m_serializers[0];

    m_defaultStorage = storageModule(m_settings->value("CuteReport/PrimaryStorage").toString());
    if (!m_defaultStorage) {
        foreach (StorageInterface * m, m_storages)
            if (m->moduleFullName().toLower() == "standard::filesystem") {
                m_defaultStorage = m;
                break;
            }
    }
    if (!m_defaultStorage) {
        foreach (StorageInterface * m, m_storages)
            if (m->moduleShortName().toLower().contains("file")) {
                m_defaultStorage = m;
                break;
            }
    }
    if (!m_defaultStorage) m_defaultStorage = m_storages[0];

    m_defaultPrinter = printerModule(m_settings->value("CuteReport/PrimaryPrinter").toString());
    if (!m_defaultPrinter) {
        foreach (PrinterInterface * m, m_printers)
            if (m->moduleFullName().toLower() == "standard::printer") {
                m_defaultPrinter = m;
                break;
            }
    }
    if (!m_defaultPrinter) {
        foreach (PrinterInterface * m, m_printers)
            if (m->moduleShortName().toLower() == "printer") {
                m_defaultPrinter = m;
                break;
            }
    }
    if (!m_defaultPrinter) m_defaultPrinter = m_printers[0];

    m_defaultRenderer = rendererModule(m_settings->value("CuteReport/PrimaryRenderer").toString());
    if (!m_defaultRenderer) {
        foreach (RendererInterface * m, m_renderers)
            if (m->moduleFullName().toLower() == "standard::renderer") {
                m_defaultRenderer = m;
                break;
            }
    }
    if (!m_defaultPrinter) {
        foreach (RendererInterface * m, m_renderers)
            if (m->moduleShortName().toLower() == "renderer") {
                m_defaultRenderer = m;
                break;
            }
    }
    if (!m_defaultPrinter) m_defaultRenderer = m_renderers[0];

    ++m_refCount;
}

bool ReportCore::loadPlugins(QSettings *settings)
{
    //    qRegisterMetaType<CuteReport::Unit>("CuteReport::Unit");
    qRegisterMetaType<CuteReport::Units>("CuteReport::Units");
    qRegisterMetaType<CuteReport::Margins>("CuteReport::Margins");
    qRegisterMetaType<QList<CuteReport::PageAction*> >("QList<CuteReport::PageAction*>");
    qRegisterMetaType<CuteReport::LogLevel>("CuteReport::LogLevel");
    qRegisterMetaType<CuteReport::RenderingType>("CuteReport::RenderingType");

    QFileInfoList files;
    QStringList dirs;
    QString iniPath = settings->value("CuteReport/PluginsPath").toString();
    if (iniPath.isEmpty())
        dirs << REPORT_PLUGINS_PATH;
    else
        dirs << iniPath;

    QList<ReportPluginInterface *> pluginList;
    foreach (QObject *plugin, QPluginLoader::staticInstances()) {
        //qDebug() << "test: " << plugin->metaObject()->className();
        if (plugin && qobject_cast<ReportPluginInterface*>(plugin)) {
            log(LogDebug, MODULENAME, QString("Found static plugin: %1").arg(plugin->metaObject()->className()), "");
            pluginList.push_back(qobject_cast<ReportPluginInterface*>(plugin));
        }
    }

    foreach (const QString & dirStr, dirs) {
        QDir dir(dirStr);
        log(CuteReport::LogDebug, MODULENAME, "Plugin dir: " + dir.absolutePath() );
        files += dir.entryInfoList(QDir::Files);
    }

    QPluginLoader loader;
    loader.setLoadHints(QLibrary::ResolveAllSymbolsHint|QLibrary::ExportExternalSymbolsHint);

    foreach(const QFileInfo & fileName, files) {
        loader.setFileName(fileName.absoluteFilePath());
        if (!loader.load()) {
            log(LogWarning, MODULENAME, "Error while loading plugin " + fileName.fileName() + ": " + loader.errorString() );
            continue;
        }

        ReportPluginInterface * plugin = dynamic_cast<ReportPluginInterface *>(loader.instance());
        if (plugin) {
            log(CuteReport::LogDebug, MODULENAME, "Loading plugin: " + fileName.baseName() );
            pluginList.append(plugin);
        } else {
            log(CuteReport::LogDebug, MODULENAME, "Plugin has not CuteReport's type: " + fileName.baseName() );
            loader.unload();
        }
    }

    m_itemPlugins = new QList<BaseItemInterface*>();
    m_pagePlugins = new QList<PageInterface*>;
    m_datasetPlugins = new QList<DatasetInterface*>;
    m_storagePlugins = new QList<StorageInterface*>;
    m_serializePlugins = new QList<SerializerInterface*>;
    m_rendererPlugins = new QList<RendererInterface*>;
    m_printerPlugins = new QList<PrinterInterface*>;
    m_formPlugins = new QList<FormInterface*>;
    m_importPlugins = new QList<ImportInterface*>;
    m_exportPlugins = new QList<ExportInterface*>;
    m_scriptExtensionPlugins = new QList<ScriptExtensionInterface*>;

    foreach (ReportPluginInterface * plugin, pluginList) {
        if (qobject_cast<CuteReport::BaseItemInterface*>(plugin))
            m_itemPlugins->append(static_cast<CuteReport::BaseItemInterface*>(plugin));

        else if (qobject_cast<CuteReport::PageInterface*>(plugin))
            m_pagePlugins->append(static_cast<CuteReport::PageInterface*>(plugin));

        else if (qobject_cast<CuteReport::DatasetInterface*>(plugin))
            m_datasetPlugins->append(static_cast<CuteReport::DatasetInterface*>(plugin));

        else if (qobject_cast<CuteReport::StorageInterface*>(plugin))
            m_storagePlugins->append(static_cast<CuteReport::StorageInterface*>(plugin));

        else if (qobject_cast<CuteReport::RendererInterface*>(plugin))
            m_rendererPlugins->append(static_cast<CuteReport::RendererInterface*>(plugin));

        else if (qobject_cast<CuteReport::PrinterInterface*>(plugin))
            m_printerPlugins->append(static_cast<CuteReport::PrinterInterface*>(plugin));

        else if (qobject_cast<CuteReport::FormInterface*>(plugin))
            m_formPlugins->append(static_cast<CuteReport::FormInterface*>(plugin));

        else if (qobject_cast<CuteReport::SerializerInterface*>(plugin))
            m_serializePlugins->append(static_cast<CuteReport::SerializerInterface*>(plugin));

        else if (qobject_cast<CuteReport::ImportInterface*>(plugin))
            m_importPlugins->append(static_cast<CuteReport::ImportInterface*>(plugin));

        else if (qobject_cast<CuteReport::ExportInterface*>(plugin))
            m_exportPlugins->append(static_cast<CuteReport::ExportInterface*>(plugin));

        else if (qobject_cast<CuteReport::ScriptExtensionInterface*>(plugin))
            m_scriptExtensionPlugins->append(static_cast<CuteReport::ScriptExtensionInterface*>(plugin));
    }

    processModuleList<BaseItemInterface>(m_itemPlugins);
    processModuleList<PageInterface>(m_pagePlugins);
    processModuleList<DatasetInterface>(m_datasetPlugins);
    processModuleList<SerializerInterface>(m_serializePlugins);
    processModuleList<StorageInterface>(m_storagePlugins);
    processModuleList<PrinterInterface>(m_printerPlugins);
    processModuleList<RendererInterface>(m_rendererPlugins);
    processModuleList<ImportInterface>(m_importPlugins);
    processModuleList<ExportInterface>(m_exportPlugins);
    processModuleList<ScriptExtensionInterface>(m_scriptExtensionPlugins);

    /// Checking critical plugins presence and default plugin settings
    bool error = false;

    if (!m_itemPlugins->size()) {
        error = true;
        log(LogCritical, MODULENAME, "Item plugins not found!" );
    }

    if (!m_pagePlugins->size()) {
        error = true;
        log(LogCritical, MODULENAME, "Page plugins not found!" );
    }

    if (!m_datasetPlugins->size()) {
        error = true;
        log(LogCritical, MODULENAME, "Dataset plugins not found!" );
    }

    if (!m_storagePlugins->size()) {
        error = true;
        log(LogCritical, MODULENAME, "Storage plugins not found!" );
    }

    if (!m_serializePlugins->size()) {
        error = true;
        log(LogCritical, MODULENAME, "Serialize plugins not found!" );
    }

    if (!m_rendererPlugins->size()) {
        error = true;
        log(LogCritical, MODULENAME, "Renderer plugins not found!" );
    }

    if (!m_printerPlugins->size()) {
        error = true;
        log(LogCritical, MODULENAME, "Printer plugins not found!" );
    }

    if (!m_formPlugins->size()) {
        log(LogCritical, MODULENAME, "Forms plugins not found!" );
    }

    if (!m_importPlugins->size()) {
        log(LogWarning, MODULENAME, "Import plugins not found!" );
    }

    if (!m_exportPlugins->size()) {
        log(LogWarning, MODULENAME, "Export plugins not found!" );
    }

    if (!m_scriptExtensionPlugins->size()) {
        log(LogWarning, MODULENAME, "Script extension plugins not found!" );
    }

    if (error) {
        log(LogCritical, MODULENAME, "CRITICAL ERRORS FOUND!" );
        return false;
    }

    return true;
}


const QList<CuteReport::BaseItemInterface*> & ReportCore::itemModules() const
{
    return *m_itemPlugins;
}


const QList<CuteReport::PageInterface*>  & ReportCore::pageModules() const
{
    return *m_pagePlugins;
}


const QList<DatasetInterface*> & ReportCore::datasetModules() const
{
    return *m_datasetPlugins;
}


const QList<CuteReport::StorageInterface*> & ReportCore::storageModules() const
{
    return m_storages;
}


const QList<CuteReport::RendererInterface*> & ReportCore::rendererModules() const
{
    return m_renderers;
}


const QList<CuteReport::PrinterInterface*> & ReportCore::printerModules() const
{
    return m_printers;
}


const QList<CuteReport::FormInterface*> & ReportCore::formModules() const
{
    return *m_formPlugins;
}


const QList<ImportInterface *> &ReportCore::importModules() const
{
    return *m_importPlugins;
}


const QList<CuteReport::ExportInterface*> & ReportCore::exportModules() const
{
    return m_exporters;
}


const QList<ScriptExtensionInterface *> &ReportCore::scriptExtensionModules() const
{
    return *m_scriptExtensionPlugins;
}


QList<StorageInterface *> ReportCore::storageObjectList(ReportInterface *report) const
{
    QList<StorageInterface *> list;
    if (report) {
        QList<CuteReport::StorageInterface *> storages = report->storages();
        list << storages;
    }

    foreach (CuteReport::StorageInterface * m, m_storages) {
        bool exists = false;
        foreach(CuteReport::StorageInterface * mExists, list) {
            if (mExists->objectName() == m->objectName()) {
                exists =  true;
                break;
            }
        }
        if (!exists)
            list << m;
    }

    return list;
}


QStringList ReportCore::moduleNames(ModuleType moduleType) const
{
    QStringList list;
    switch (moduleType) {
        case UnknownModuleType: break;
        case ItemModule:
            foreach (ReportPluginInterface * module, *m_itemPlugins)
                list.append(module->moduleFullName());
            break;
        case PageModule:
            foreach (ReportPluginInterface * module, *m_pagePlugins)
                list.append(module->moduleFullName());
            break;
        case DatasetModule:
            foreach (ReportPluginInterface * module, *m_datasetPlugins)
                list.append(module->moduleFullName());
            break;
        case StorageModule:
            foreach (ReportPluginInterface * module, m_storages)
                list.append(module->moduleFullName());
            break;
        case RendererModule:
            foreach (ReportPluginInterface * module, m_renderers)
                list.append(module->moduleFullName());
            break;
        case PrinterModule:
            foreach (ReportPluginInterface * module, m_printers)
                list.append(module->moduleFullName());
            break;
        case FormModule:
            foreach (ReportPluginInterface * module, *m_formPlugins)
                list.append(module->moduleFullName());
            break;
        case ExportModule:
            foreach (ReportPluginInterface * module, m_exporters)
                list.append(module->moduleFullName());
            break;
    }

    return list;
}


const CuteReport::ReportPluginInterface * ReportCore::module(ModuleType moduleType, const QString & moduleName) const
{
    const CuteReport::ReportPluginInterface * module = 0;

    switch (moduleType) {
        case UnknownModuleType: break;
        case ItemModule:     module = itemModule(moduleName);  break;
        case PageModule:     module = pageModule(moduleName);  break;
        case DatasetModule:  module = datasetModule(moduleName);  break;
        case StorageModule:  module = storageModule(moduleName);  break;
        case RendererModule: module = rendererModule(moduleName);  break;
        case PrinterModule:  module = printerModule(moduleName);  break;
        case FormModule:     module = formModule(moduleName);  break;
        case ExportModule:   module = exportModule(moduleName);  break;
    }

    return module;
}


QStringList ReportCore::moduleOptions(ReportPluginInterface * module )
{
    QStringList list;

    if (!module)
        return list;

#if QT_VERSION >= 0x050000
    int firstIndex = 1;
#else
    int firstIndex = 2;
#endif

    for (int i = firstIndex; i<module->metaObject()->propertyCount(); ++i) {
        QString propertyName = module->metaObject()->property(i).name();
        if (!propertyName.isEmpty() && propertyName.at(0) != '_') {
            QString str = QString("%1=%2").arg(propertyName).arg(module->metaObject()->property(i).read(module).toString());
            list << str;
        }
    }
    return list;
}


void ReportCore::setModuleOptions(ReportPluginInterface *module, const QStringList &options)
{
    if (!module || options.isEmpty())
        return;

    for (int i = 0; i<options.size(); ++i) {
        QString property = options[i].section("=",0,0);
        QString value = options[i].section("=",1,1);
        if (value.isEmpty() || property.isEmpty() || property.at(0) == '_')
            continue;
        module->setProperty(property.toLatin1(), value);
    }
}


QString ReportCore::moduleOptionsStr(ReportPluginInterface *module, const QString &delimiter)
{
    return moduleOptions(module).join(delimiter);
}


void ReportCore::setModuleOptionsStr(ReportPluginInterface * module, const QString & options, const QString & delimiter)
{
    setModuleOptions(module, options.split(delimiter, QString::SkipEmptyParts));
}

const BaseItemInterface * ReportCore::itemModule(const QString & moduleName) const
{
    BaseItemInterface * module = 0;
    if (moduleName.isEmpty() && m_itemPlugins->count())
        module = m_itemPlugins->at(0);
    else {
        foreach (BaseItemInterface * m, *m_itemPlugins) {
            if (m->moduleFullName() == moduleName) {
                module = m;
                break;
            }
        }
    }

    return module;
}


const PageInterface *ReportCore::pageModule(const QString & moduleName) const
{
    PageInterface * module = 0;
    if (moduleName.isEmpty() && m_pagePlugins->count())
        module = m_pagePlugins->at(0);
    else {
        foreach (PageInterface * m, *m_pagePlugins) {
            if (m->moduleFullName() == moduleName) {
                module = m;
                break;
            }
        }
    }

    return module;
}


const DatasetInterface *ReportCore::datasetModule(const QString & moduleName) const
{
    DatasetInterface * module = 0;
    if (moduleName.isEmpty() && m_datasetPlugins->count())
        module = m_datasetPlugins->at(0);
    else {
        foreach (DatasetInterface * m, *m_datasetPlugins) {
            if (m->moduleFullName() == moduleName) {
                module = m;
                break;
            }
        }
    }

    return module;
}


CuteReport::SerializerInterface * ReportCore::serializerModule(const QString & moduleName) const
{
    SerializerInterface * module = 0;
    if (moduleName.isEmpty() && m_serializers.count())
        module = m_serializers.at(0);
    else {
        foreach (SerializerInterface * m, m_serializers) {
            if (m->moduleFullName() == moduleName) {
                module = m;
                break;
            }
        }
    }

    return module;
}


StorageInterface *ReportCore::storageModule(const QString & moduleName) const
{
    StorageInterface * module = 0;
    if (moduleName.isEmpty() && m_storages.count())
        module = m_storages.at(0);
    else {
        foreach (StorageInterface * m, m_storages) {
            if (m->moduleFullName() == moduleName) {
                module = m;
                break;
            }
        }
    }

    return module;
}


CuteReport::StorageInterface* ReportCore::storageModuleByScheme(const QString & scheme) const
{
    StorageInterface * module = 0;
    foreach (StorageInterface * m, m_storages) {
        if (m->urlScheme() == scheme) {
            module = m;
            break;
        }
    }

    return module;
}


//ReportInterface * ReportCore::reportByName(const QString & reportName) const
//{
//    foreach (CuteReport::ReportInterface * report, m_reports)
//        if (report->objectName() == reportName)
//            return report;
//    return 0;
//}


PageInterface *  ReportCore::pageByName(const QString & pageName, CuteReport::ReportInterface * report) const
{
    return report->findChild<PageInterface *>(pageName);
}


CuteReport::PrinterInterface*  ReportCore::printerModule(const QString & moduleName) const
{
    PrinterInterface * printer = 0;
    if (moduleName.isEmpty() && m_printers.count())
        printer = m_printers.at(0);
    else {
        foreach (PrinterInterface * i, m_printers) {
            if (i->moduleFullName() == moduleName) {
                printer = i;
                break;
            }
        }
    }
    return printer;
}


CuteReport::FormInterface* ReportCore::formModule(const QString & moduleName) const
{
    FormInterface * form = 0;
    if (moduleName.isEmpty() && m_formPlugins->count())
        form = m_formPlugins->at(0);
    else {
        foreach (FormInterface * i, *m_formPlugins) {
            if (i->moduleFullName() == moduleName) {
                form = i;
                break;
            }
        }
    }
    return form;
}


const ImportInterface *ReportCore::importModule(const QString &moduleName) const
{
    ImportInterface * module = 0;

    foreach (ImportInterface * m, *m_importPlugins) {
        if (m->moduleFullName() == moduleName) {
            module = m;
            break;
        }
    }

    return module;
}


CuteReport::ExportInterface* ReportCore::exportModule(const QString & moduleName) const
{
    ExportInterface * exporter = 0;
    foreach (ExportInterface * i, m_exporters) {
        if (i->moduleFullName() == moduleName) {
            exporter = i;
            break;
        }
    }
    return exporter;
}


CuteReport::RendererInterface * ReportCore::rendererModule(const QString & moduleName) const
{
    RendererInterface * renderer = 0;
    if (moduleName.isEmpty() && m_renderers.count()) {
        renderer = m_renderers.at(0);
        QString moduleShortName = moduleName.section("::",1,1);
        foreach (RendererInterface * r, m_renderers) {
            if (r->extendsModules().contains(renderer->moduleFullName())  || r->extendsModules().contains(moduleShortName)) {
                renderer = r;
                break;
            }
        }
    } else {
        foreach (RendererInterface * r, m_renderers) {
            if (r->moduleFullName() == moduleName) {
                renderer = r;
                break;
            }
        }
    }

    return renderer;
}


BaseItemInterface * ReportCore::itemByName(const QString & itemName, CuteReport::PageInterface * page) const
{
    BaseItemInterface * resultItem = 0;
    QList <BaseItemInterface*> items = page->items();
    foreach (BaseItemInterface* item, items)
        if (item->objectName() == itemName) {
            resultItem = item;
            break;
        }

    return resultItem;
}


BaseItemInterface * ReportCore::itemByName(const QString & itemName, const QString & pageName, CuteReport::ReportInterface * report) const
{
    PageInterface * resultPage = 0;
    QList <PageInterface*> pages = report->pages();
    foreach (PageInterface* page, pages)
        if (page->objectName() == pageName) {
            resultPage = page;
            break;
        }

    if (resultPage)
        return itemByName(itemName, resultPage);
    else
        return 0;
}


DatasetInterface * ReportCore::datasetByName(const QString & datasetName, CuteReport::ReportInterface * report) const
{
    return report->dataset(datasetName);
}


FormInterface * ReportCore::formByName(const QString & formName, CuteReport::ReportInterface * report) const
{
    return report->form(formName);
}


//const QList<CuteReport::ReportInterface*> & ReportCore::reports() const
//{
//    return m_reports;
//}


ReportInterface *ReportCore::createReport()
{
    CuteReport::ReportInterface * newReport = new CuteReport::ReportInterface(this);
    _reportObjectCreated(newReport);
    return newReport;
}


void ReportCore::deleteReport(CuteReport::ReportInterface * report)
{
    delete report;
}


bool ReportCore::saveReport(const QString & urlString, CuteReport::ReportInterface * report, QString * errorText)
{
    if (urlString.isEmpty() || !report)
        return false;

    CuteReport::StorageInterface * module = 0;
    if (!checkReportPointer(report, errorText) || !(module = getStorageByUrl(urlString, report) ))
        return false;

    bool saveResultOk = module->saveObject(urlString, serialize(report));

    if (saveResultOk) {
        report->setFilePath(urlString);
        report->setDirty(false);
    } else {
        if (errorText)
            *errorText = module->lastError();
    }

    return saveResultOk;
}


CuteReport::ReportInterface * ReportCore::loadReport(const QString & urlString, QString * errorText)
{
    if (urlString.isEmpty())
        return 0;

    CuteReport::StorageInterface * module = 0;
    if (!(module = getStorageByUrl(urlString, 0)))
        return 0;

    QVariant object = module->loadObject(urlString);
    CuteReport::ReportInterface * report = dynamic_cast<CuteReport::ReportInterface*> (deserialize(object.toByteArray() /*, &ok, &error*/));

    if (report)
        report->setFilePath(urlString);

    if (!report && errorText)
        *errorText = module->lastError();

    _reportObjectCreated(report);
    return report;
}


bool ReportCore::saveObject(const QString & urlString,
                            CuteReport::ReportInterface * report,
                            const QVariant &objectData,
                            QString * errorText)
{
    CuteReport::StorageInterface * module = 0;
    if (!checkReportPointer(report, errorText) || !(module = getStorageByUrl(urlString, report)) )
        return false;

    bool saveResultOk = module->saveObject(urlString, objectData);
    if (!saveResultOk && errorText)
        *errorText = module->lastError();

    return saveResultOk;
}


QVariant ReportCore::loadObject(const QString & urlString,
                                CuteReport::ReportInterface * report,
                                QString * errorText)
{
    CuteReport::StorageInterface * module = 0;
    if ((report && !checkReportPointer(report, errorText)) || !(module = getStorageByUrl(urlString, report)) )
        return QByteArray();

    QVariant data = module->loadObject(urlString);

    if (data.isNull() && errorText)
        *errorText = module->lastError();

    return data;
}


QString ReportCore::localCachedFileName(const QString & url, CuteReport::ReportInterface * report)
{
    if (!report)
        return QString();

    CuteReport::StorageInterface * module = getStorageByUrl(url, report);
    return module ? module->localCachedFileName(url) : QString();
}


CuteReport::PageInterface * ReportCore::createPageObject(CuteReport::ReportInterface *report, const QString & moduleName)
{
    QList<CuteReport::ReportPluginInterface*> modules = this->findChildren<CuteReport::ReportPluginInterface*>();
    CuteReport::PageInterface * plugin = 0;
    if (moduleName.isEmpty() && m_pagePlugins->count())
        plugin =  m_pagePlugins->at(0);
    else
        foreach  (CuteReport::PageInterface * i, *m_pagePlugins)  {
            qDebug() << i->moduleFullName();
            if (i->moduleFullName() == moduleName) {
                plugin = i;
                break;
            }
        }

    if (!plugin) {
        QString moduleShortName = moduleName.section("::",1,1);
        foreach  (CuteReport::PageInterface * i, *m_pagePlugins)
            if (i->extendsModules().contains(moduleName) || i->extendsModules().contains(moduleShortName)) {
                plugin = i;
                break;
            }
    }

    if (plugin) {
        CuteReport::PageInterface * newObject =  plugin->createInstance(report);
        newObject->setReportCore(this);
        newObject->setObjectName(this->uniqueName(newObject, "page", report));
        newObject->setParent(report);
        qDebug() << newObject->parent();
        QList<CuteReport::ReportPluginInterface*> modules2 = this->findChildren<CuteReport::ReportPluginInterface*>();
        qDebug() << qobject_cast<CuteReport::ReportPluginInterface*>(newObject);
        qDebug() << qobject_cast<CuteReport::PageInterface*>(newObject);
        QList<QObject*> modules3 = this->findChildren<QObject*>();
        return newObject;
    } else
        return 0;
}


CuteReport::BaseItemInterface * ReportCore::createItemObject(CuteReport::ReportInterface *report, const QString & moduleName, QObject * parent)
{
    CuteReport::BaseItemInterface * plugin = 0;
    if (moduleName.isEmpty() && m_itemPlugins->count())
        plugin =  m_itemPlugins->at(0);
    else
        foreach  (CuteReport::BaseItemInterface * i, *m_itemPlugins)
            if (i->moduleFullName() == moduleName) {
                plugin = i;
                break;
            }

    if (!plugin) {
        QString moduleShortName = moduleName.section("::",1,1);
        foreach  (CuteReport::BaseItemInterface * i, *m_itemPlugins)
            if (i->extendsModules().contains(moduleName) || i->extendsModules().contains(moduleShortName)) {
                plugin = i;
                break;
            }
    }

    if (plugin) {
        CuteReport::BaseItemInterface * newObject =  plugin->createInstance(parent);
        newObject->setReportCore(this);
        newObject->setObjectName(this->uniqueName(newObject, plugin->moduleShortName().toLower(), newObject->page() ? (QObject*)newObject->page() : (QObject*)report));
        return newObject;
    } else
        return 0;
}


CuteReport::DatasetInterface * ReportCore::createDatasetObject(CuteReport::ReportInterface *report, const QString & moduleName)
{
    CuteReport::DatasetInterface * plugin = 0;
    if (moduleName.isEmpty() && m_datasetPlugins->count())
        plugin =  m_datasetPlugins->at(0);
    else
        foreach  (CuteReport::DatasetInterface * i, *m_datasetPlugins)
            if (i->moduleFullName() == moduleName) {
                plugin = i;
                break;
            }

    if (!plugin) {
        QString moduleShortName = moduleName.section("::",1,1);
        foreach  (CuteReport::DatasetInterface * i, *m_datasetPlugins)
            if (i->extendsModules().contains(moduleName) || i->extendsModules().contains(moduleShortName)) {
                plugin = i;
                break;
            }
    }

    if (plugin) {
        CuteReport::DatasetInterface * newObject =  plugin->createInstance(report);
        newObject->setReportCore(this);
        newObject->setObjectName(this->uniqueName(newObject, "data", report));
        if (report)
            report->addDataset(newObject);
        return newObject;
    } else
        return 0;
}


CuteReport::StorageInterface * ReportCore::createStorageObject(ReportInterface *report, const QString & moduleName)
{
    CuteReport::StorageInterface * plugin = 0;
    if (moduleName.isEmpty() && m_storagePlugins->count())
        plugin =  m_storagePlugins->at(0);
    else
        foreach  (CuteReport::StorageInterface * i, *m_storagePlugins)
            if (i->moduleFullName() == moduleName) {
                plugin = i;
                break;
            }

    if (!plugin) {
        QString moduleShortName = moduleName.section("::",1,1);
        foreach  (CuteReport::StorageInterface * i, *m_storagePlugins)
            if (i->extendsModules().contains(moduleName) || i->extendsModules().contains(moduleShortName)) {
                plugin = i;
                break;
            }
    }

    if (plugin) {
        /** do not set parent. ReportInterface should send signal for new storage */
        CuteReport::StorageInterface * newObject =  plugin->createInstance();
        newObject->setReportCore(this);
        newObject->setObjectName(this->uniqueName(newObject, newObject->urlScheme(), report));
        return newObject;
    } else
        return 0;
}


CuteReport::PrinterInterface * ReportCore::createPrinterObject(CuteReport::ReportInterface *report, const QString & moduleName)
{ 
    CuteReport::PrinterInterface * plugin = 0;
    if (moduleName.isEmpty() && m_printerPlugins->count())
        plugin =  m_printerPlugins->at(0);
    else
        foreach  (CuteReport::PrinterInterface * i, *m_printerPlugins)
            if (i->moduleFullName() == moduleName) {
                plugin = i;
                break;
            }

    if (!plugin) {
        QString moduleShortName = moduleName.section("::",1,1);
        foreach  (CuteReport::PrinterInterface * i, *m_printerPlugins)
            if (i->extendsModules().contains(moduleName) || i->extendsModules().contains(moduleShortName)) {
                plugin = i;
                break;
            }
    }

    if (plugin) {
        /** do not set parent. ReportInterface should send signal for new printer */
        CuteReport::PrinterInterface * newObject =  plugin->createInstance();
        newObject->setReportCore(this);
        newObject->setObjectName(this->uniqueName(newObject, "printer", report));
        return newObject;
    } else
        return 0;
}


CuteReport::RendererInterface * ReportCore::createRendererObject(CuteReport::ReportInterface *report, const QString & moduleName)
{
    CuteReport::RendererInterface * plugin = 0;
    if (moduleName.isEmpty() && m_rendererPlugins->count())
        plugin =  m_rendererPlugins->at(0);
    else
        foreach  (CuteReport::RendererInterface * i, *m_rendererPlugins)
            if (i->moduleFullName() == moduleName) {
                plugin = i;
                break;
            }

    if (!plugin) {
        QString moduleShortName = moduleName.section("::",1,1);
        foreach  (CuteReport::RendererInterface * i, *m_rendererPlugins)
            if (i->extendsModules().contains(moduleName) || i->extendsModules().contains(moduleShortName)) {
                plugin = i;
                break;
            }
    }

    if (plugin) {
        /** do not set parent. ReportInterface should send signal for new renderer */
        CuteReport::RendererInterface * newObject =  plugin->createInstance();
        newObject->setReportCore(this);
        newObject->setObjectName(this->uniqueName(newObject, "renderer", report));
        return newObject;
    } else
        return 0;
}


CuteReport::FormInterface * ReportCore::createFormObject(ReportInterface *report, const QString & moduleName)
{
    CuteReport::FormInterface * formPlugin = 0;
    if (moduleName.isEmpty() && m_formPlugins->count())
        formPlugin =  m_formPlugins->at(0);
    else
        foreach  (CuteReport::FormInterface * form, *m_formPlugins)
            if (form->moduleFullName() == moduleName) {
                formPlugin = form;
                break;
            }

    if (!formPlugin) {
        QString moduleShortName = moduleName.section("::",1,1);
        foreach  (CuteReport::FormInterface * i, *m_formPlugins)
            if (i->extendsModules().contains(moduleName) || i->extendsModules().contains(moduleShortName)) {
                formPlugin = i;
                break;
            }
    }

    if (formPlugin) {
        CuteReport::FormInterface * newObject =  formPlugin->createInstance(report);
        newObject->setReportCore(this);
        newObject->setObjectName(this->uniqueName(newObject, "form", report));
        return newObject;
    } else
        return 0;
}


CuteReport::ExportInterface * ReportCore::createExportObject(CuteReport::ReportInterface *report, const QString & moduleName)
{
    CuteReport::ExportInterface * exportPlugin = 0;

    foreach  (CuteReport::ExportInterface * m, m_exporters) {
        if (m->moduleFullName() == moduleName) {
            exportPlugin = m;
            break;
        }
    }

    if (!exportPlugin) {
        QString moduleShortName = moduleName.section("::",1,1);
        foreach  (CuteReport::ExportInterface * i, m_exporters)
            if (i->extendsModules().contains(moduleName) || i->extendsModules().contains(moduleShortName)) {
                exportPlugin = i;
                break;
            }
    }

    if (exportPlugin) {
        CuteReport::ExportInterface * newObject =  exportPlugin->createInstance(report);
        newObject->setReportCore(this);
        newObject->setObjectName(this->uniqueName(newObject, "export", report));
        return newObject;
    } else
        return 0;
}


QByteArray ReportCore::serialize(const QObject * object, bool * ok, QString *error, const QString & moduleName)
{
    QByteArray ba;

    SerializerInterface * serializer = moduleName.isEmpty() ? m_defaultSerializer : serializerModule(moduleName);

    log(LogWarning, MODULENAME, "serializer " + moduleName + ":" + (serializer ? serializer->moduleFullName() : "not found!") );

    if (!serializer && m_serializePlugins->size())
        serializer = m_serializePlugins->at(0);

    if (serializer) {
        ba = m_defaultSerializer->serialize(object, ok);
        if (ok && !(*ok)) {
            if (error)
                *error = serializer->lastError();
        }
    } else {
        if (ok)
            *ok = false;
        if (error)
            *error = QString("Serializer \'%1\' not found").arg(moduleName);
    }

    return ba;
}


QObject * ReportCore::deserialize(const QByteArray &data, bool *ok, QString *error, const QString & moduleName)
{
    QObject * object;
    SerializerInterface * serializer = moduleName.isEmpty() ? m_defaultSerializer : serializerModule(moduleName);

    if (!serializer && m_serializePlugins->size())
        serializer = m_serializePlugins->at(0);

    if (serializer) {
        object = m_defaultSerializer->deserialize(data, ok);
        if (ok && !(*ok)) {
            if (error)
                *error = serializer->lastError();
        }
    } else {
        if (ok)
            *ok = false;
        if (error)
            *error = QString("Serializer \'%1\' not found").arg(moduleName);
    }

    return object;
}


const QList<QString> ReportCore::renderers()
{
    QStringList list;
    foreach (RendererInterface * r, *m_rendererPlugins)
        list.append(r->moduleFullName());

    return list;
}


bool ReportCore::render(ReportInterface* report)
{
    log(LogWarning, MODULENAME, QString("render \'%1\'").arg((long)report) );
    QueueReport * qr = new QueueReport();
    qr->report = report;
    qr->destination = RenderToPreview;

    if (m_renderingQueue.size() > m_maxRenderingThreads) {
        log(LogWarning, MODULENAME, QString("add to queue (size is %1 max is %2)").arg(m_renderingQueue.size()).arg(m_maxRenderingThreads));
        m_waitingQueue.append(qr);
        return true;
    } else
        return _render(qr);
}


bool ReportCore::render(const QString &reportUrl)
{
    log(LogWarning, MODULENAME, QString("render \'%1\'").arg(reportUrl) );
    QueueReport * qr = new QueueReport();
    qr->url = reportUrl;
    qr->destination = RenderToPreview;

    if (m_renderingQueue.size() > m_maxRenderingThreads) {
        m_waitingQueue.append(qr);
        return true;
    } else
        return _render(qr);
}


bool ReportCore::_render(QueueReport * queueReport)
{
    log(LogWarning, MODULENAME, QString("_render"));
    if (!queueReport->report) {
        queueReport->report = loadReport(queueReport->url);
    }

    if (!queueReport->report) {
        queueReport->success = false;
        _renderDone(queueReport);
        return false;
    }

    RendererInterface * renderer = getRenderer(queueReport->report);
    if (!renderer) {
        log(LogWarning, MODULENAME, QString("Can't find renderer for report  \'%1\'").arg(queueReport->report->objectName()) );
        queueReport->success = false;
        _renderDone(queueReport);
        return false;
    }

    RendererInterface * rendererCopy = renderer->clone();
    m_renderingQueue.insert(rendererCopy, queueReport);

    connect (rendererCopy, SIGNAL(started()), this, SLOT(_rendererStarted()), Qt::UniqueConnection);
    connect (rendererCopy, SIGNAL(done(bool)), this, SLOT(_rendererDone(bool)), Qt::UniqueConnection);
    connect (rendererCopy, SIGNAL(processingPage(int,int)), this, SLOT(_rendererProcessingPage(int,int)), Qt::UniqueConnection);

    rendererCopy->run(queueReport->report);

    return true;
}


void ReportCore::_renderDone(QueueReport *queueReport)
{
    log(CuteReport::LogDebug, MODULENAME, QString("Renderer done for report: %1 %2")
        .arg(queueReport->report ? queueReport->report->objectName() : queueReport->url)
        .arg(queueReport->success ? "without errors" : "with errors" ) );

    if (!queueReport->url.isEmpty())
        emit rendererDone(queueReport->url, queueReport->success);

    emit rendererDone(queueReport->report, queueReport->success);

    switch (queueReport->destination) {
        case RenderToPreview: delete queueReport; break;
        case RenderToExport:  queueReport->success ? _export(queueReport) : _exportDone(queueReport); break;
        case RenderToPrinter: queueReport->success ? _print(queueReport)  : _printDone(queueReport);  break;
    }
}


void ReportCore::_export(QueueReport *queueReport)
{
    QueueReportExport * qr = reinterpret_cast<QueueReportExport*>(queueReport);

    ExportInterface * exportModule = 0;
    foreach (ExportInterface * m, exportModules()) {
        if (m->format().toLower() == qr->format) {
            exportModule = m;
            break;
        }
    }

    if (exportModule) {
        ExportInterface * moduleCopy =  exportModule->clone();
        if (!qr->options.isEmpty())
            setModuleOptions(moduleCopy, qr->options);
        moduleCopy->process(qr->report, qr->outputUrl);
        moduleCopy->deleteLater();
    }

    _exportDone(queueReport);
}


void ReportCore::_exportDone(QueueReport *queueReport)
{
    log(CuteReport::LogDebug, MODULENAME, QString("Export done for report: %1 %2")
        .arg(queueReport->report ? queueReport->report->objectName() : queueReport->url)
        .arg(queueReport->success ? "without errors" : "with errors" ) );

    if (!queueReport->url.isEmpty())
        emit exportDone(queueReport->url, queueReport->success);

    emit exportDone(queueReport->report, queueReport->success);

    delete queueReport;
}


void ReportCore::_print(QueueReport *queueReport)
{
    QueueReportPrint * qr = reinterpret_cast<QueueReportPrint*>(queueReport);

    PrinterInterface * printer = qr->report->printer();
    if (!printer)
        printer = m_defaultPrinter;

    if (printer) {
        printer->print(qr->report);
    } else {
        qr->success = true;
        log(LogWarning, QString("Can't find printer for print report \'%1\'").arg(qr->report->objectName()), MODULENAME);
    }

    _printDone(queueReport);
}


void ReportCore::_printDone(QueueReport *queueReport)
{
    log(CuteReport::LogDebug, MODULENAME, QString("Printing done for report: %1 %2")
        .arg(queueReport->report ? queueReport->report->objectName() : queueReport->url)
        .arg(queueReport->success ? "without errors" : "with errors" ) );

    if (!queueReport->url.isEmpty())
        emit printingDone(queueReport->url, queueReport->success);

    emit printingDone(queueReport->report, queueReport->success);

    delete queueReport;
}


void ReportCore::stopRendering(ReportInterface* report)
{
    bool exists = false;
    foreach (RendererInterface * renderer, m_renderingQueue.keys()) {
        if (renderer->report() == report) {
            renderer->stop();
            exists = true;
        }
    }

    if (!exists)
        log(LogWarning, QString("Can't find renderer for report \'%1\'").arg(report->objectName()), MODULENAME);
}


int ReportCore::rendererTotalPages(ReportInterface * report) const
{
    if (!report)
        return 0;

    RenderedReportInterface * renderedReport = report->renderedReport();
    return renderedReport ? renderedReport->pageCount() : 0;
}


RenderedPageInterface *ReportCore::rendererGetPage(ReportInterface * report, int number) const
{
    if (!report)
        return 0;

    RenderedReportInterface * renderedReport = report->renderedReport();
    return renderedReport ? renderedReport->page(number) : 0;
}


void ReportCore::renderDataClear(ReportInterface * report)
{
    if (!report)
        return;

    report->clearRenderedReport();
}


void ReportCore::print(ReportInterface* report)
{
    QueueReportPrint * qr = new QueueReportPrint();
    qr->report = report;
    qr->destination = RenderToPrinter;

    if (m_renderingQueue.size() > m_maxRenderingThreads) {
        m_waitingQueue.append(qr);
        return;
    } else
        _print(qr);
}


void ReportCore::print(const QString url)
{
    QueueReportPrint * qr = new QueueReportPrint();
    qr->url = url;
    qr->destination = RenderToPrinter;

    if (m_renderingQueue.size() > m_maxRenderingThreads) {
        m_waitingQueue.append(qr);
        return;
    } else
        _print(qr);
}


QStringList ReportCore::importExtensions() const
{
    QStringList list;
    foreach (ImportInterface * module, *m_importPlugins) {
        list << module->fileExtensions();
    }
    return list;
}


bool ReportCore::canImport(const QString &reportUrl) const
{
    foreach (ImportInterface * module, *m_importPlugins) {
        if (module->canHandle(reportUrl))
            return true;
    }
    return false;
}


QStringList ReportCore::importModulesForFile(const QString &reportUrl) const
{
    QStringList list;
    foreach (ImportInterface * module, *m_importPlugins) {
        if (module->canHandle(reportUrl))
            list << module->moduleFullName();
    }
    return list;
}


ReportInterface *ReportCore::import(const QString &reportUrl, const QString &moduleName) const
{
    const ImportInterface * module = importModule(moduleName);

    if (!module) {
        QList<ImportInterface*> list;
        foreach (ImportInterface * module, *m_importPlugins) {
            if (module->canHandle(reportUrl))
                list << module;
        }
        if (list.size() > 0)
            module = list.at(0);
    }

    ReportInterface * report = module->importReport(reportUrl);
    return report;
}


void ReportCore::exportTo(ReportInterface *report, const QString &format, const QString &outputUrl, const QStringList &options)
{
    QueueReportExport * qr = new QueueReportExport();
    qr->report = report;
    qr->destination = RenderToExport;
    qr->format = format.toLower();
    qr->outputUrl = outputUrl;
    qr->options = options;

    if (m_renderingQueue.size() > m_maxRenderingThreads) {
        m_waitingQueue.append(qr);
        return;
    } else
        _render(qr);
}


void ReportCore::exportTo(const QString &reportUrl, const QString &format, const QString &outputUrl, const QStringList &options)
{
    QueueReportExport * qr = new QueueReportExport();
    qr->url = reportUrl;
    qr->destination = RenderToExport;
    qr->format = format;
    qr->outputUrl = outputUrl;
    qr->options = options;

    if (m_renderingQueue.size() > m_maxRenderingThreads) {
        m_waitingQueue.append(qr);
        return;
    } else
        _render(qr);
}


bool ReportCore::isNameUnique(QObject * object, const QString & name, QObject * rootObject)
{
    if (!rootObject)
        return true;
    if (rootObject->objectName() == name)
        return false;

    foreach(QObject * o, rootObject->children()) {
        if (object != o && !isNameUnique(object, name, o))
            return false;
    }
    return true;
}


QString ReportCore::uniqueName(QObject * object, const QString & proposedName, QObject * rootObject)
{
    QString className = proposedName.isEmpty() ? object->metaObject()->className() : proposedName;
    QString newName = className.section("::", -1,-1).toLower();
    if (isNameUnique(object, newName, rootObject))
        return newName;

    int number = 1;

    forever{
        if (isNameUnique(object, newName + "_" + QString::number(number, 10), rootObject ))
            return newName + "_" + QString::number(number);
        number++;
    }
}


void ReportCore::_rendererStarted()
{
    RendererInterface * renderer = reinterpret_cast<RendererInterface *>(sender());
    if (!renderer)
        return;

    log(CuteReport::LogDebug, MODULENAME, "Renderer started for report: " + renderer->report()->objectName());
    emit rendererStarted(renderer->report());
}


void ReportCore::_rendererDone(bool successful)
{
    RendererInterface * renderer = reinterpret_cast<RendererInterface *>(sender());
    if (!renderer)
        return;

    CuteReport::RenderedReportInterface * renderedReport = renderer->takeRenderedReport();
    renderer->report()->setRenderedReport( renderedReport );

    QueueReport * qr = m_renderingQueue.take(renderer);
    qr->success = successful;
    _renderDone(qr);

    if (m_waitingQueue.size() != 0 && m_renderingQueue.size() < m_maxRenderingThreads) {
        QueueReport * qr = m_waitingQueue.takeFirst();
        qr->success = successful;
        _render(qr);
    }
}


void ReportCore::_rendererMessage(int logLevel, QString message)
{
    RendererInterface * renderer = reinterpret_cast<RendererInterface *>(sender());
    if (!renderer)
        return;
    log(CuteReport::LogDebug, MODULENAME, QString("Renderer message for report \'%1\' : %2").arg(renderer->report()->objectName()).arg(message) );
    emit rendererMessage(renderer->report(), logLevel, message);
}


void ReportCore::_rendererProcessingPage(int page, int total)
{
    RendererInterface * renderer = reinterpret_cast<RendererInterface *>(sender());
    if (!renderer)
        return;
    log(CuteReport::LogDebug, MODULENAME, QString("Rendering report \'%1\': page %2 of %3").arg(renderer->report()->objectName()).arg(page).arg(total) );
    emit rendererProcessingPage(renderer->report(), page, total);
}


void ReportCore::_reportObjectCreated(CuteReport::ReportInterface * report)
{
    if (!report)
        return;

    //    m_reports.append(report);
    //    report->setReportCore(this);
    report->setParent(this);
    report->setObjectName(uniqueName(report, report->objectName().isEmpty() ? "report" : report->objectName(), this));
    //    connect(report, SIGNAL(destroyed(QObject*)), this, SLOT(_reportObjectDestroyed(QObject*)), Qt::UniqueConnection);
    emit reportObjectCreated(report);
}


void ReportCore::log(LogLevel level, const QString & sender, const QString & message )
{
    if (Log::instance())
        Log::instance()->push(level, sender, message);
}


void ReportCore::log(LogLevel level, const QString & sender, const QString & shortMessage, const QString & fullMessage)
{
    if (Log::instance())
        Log::instance()->push(level, sender, shortMessage, fullMessage);
}


void ReportCore::sendMetric(MetricType type, const QVariant &value)
{
    emit this->metricUpdated(type, value);
}


int ReportCore::maxRenderingThreads() const
{
    return m_maxRenderingThreads;
}


void ReportCore::setMaxRenderingThreads(int maxRenderingThreads)
{
    m_maxRenderingThreads = maxRenderingThreads;
}

void ReportCore::registerDesignerInterface(DesignerItemInterface *_interface)
{
    //FIX: compiler mingw32  C:\Projects\cutereport\src\core\reportcore.cpp:1944: error: expected primary-expression before 'struct'
    //    m_designerInterface = interface;
    //                          ^
    delete m_designerInterface;
    m_designerInterface = _interface;
}


DesignerItemInterface *ReportCore::designerInterface() const
{
    return m_designerInterface;
}


} //namespace
