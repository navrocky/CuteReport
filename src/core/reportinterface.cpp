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

#include "reportinterface.h"
#include "pageinterface.h"
#include "iteminterface.h"
#include "datasetinterface.h"
#include "forminterface.h"
#include "rendererinterface.h"
#include "printerinterface.h"
#include "storageinterface.h"
#include "log/log.h"
#include "globals.h"
#include "renderedreportinterface.h"

using namespace CuteReport;


ReportInterface::ReportInterface(QObject *parent)
    : QObject(parent),
//      m_reportCore(0),
      m_version(0.0),
      m_isDirty(false),
      m_isValid(true),
      m_renderedReport(0)
{
    Log::refCounterInc();
}


ReportInterface::ReportInterface(const ReportInterface & dd, QObject * parent)
    :QObject(parent),
//      m_reportCore(dd.m_reportCore),
      m_name(dd.m_name),
      m_author(dd.m_author),
      m_script(dd.m_script),
      m_version(dd.m_version),
      m_description(dd.m_description),
      m_filePath(dd.m_filePath),
      m_defaultStorageName(dd.m_defaultStorageName),
      m_variables(dd.m_variables),
      m_flags(dd.m_flags),
      m_isDirty(dd.m_isDirty),
      m_renderedReport(0)
{
    Log::refCounterInc();
}


ReportInterface::~ReportInterface()
{
    Log::refCounterDec();
}


void ReportInterface::init()
{
    QList<CuteReport::PageInterface*> pages = this->findChildren<CuteReport::PageInterface*>();
    foreach(CuteReport::PageInterface* page, pages) {
        page->init();
        connect(page, SIGNAL(destroyed(QObject*)), this, SLOT(childDestroyed(QObject*)));
        connect(page, SIGNAL(afterNewItemAdded(CuteReport::BaseItemInterface*,QPointF)),
                this, SLOT(slotItemAdded(CuteReport::BaseItemInterface*,QPointF)));
        connect(page, SIGNAL(afterItemRemoved(CuteReport::BaseItemInterface*)),
                this, SLOT(slotItemRemoved(CuteReport::BaseItemInterface*)));
    }

//    QList<CuteReport::BaseItemInterface*> items = this->findChildren<CuteReport::BaseItemInterface*>();
//    foreach(CuteReport::BaseItemInterface* item, items) {
//        item->init();
//    }

    QList<CuteReport::DatasetInterface*> datasets = this->findChildren<CuteReport::DatasetInterface*>();
    foreach(CuteReport::DatasetInterface* dataset, datasets) {
        dataset->init();
        connect(dataset, SIGNAL(destroyed(QObject*)), this, SLOT(childDestroyed(QObject*)));
    }

    QList<CuteReport::FormInterface*> forms = this->findChildren<CuteReport::FormInterface*>();
    foreach(CuteReport::FormInterface* form, forms) {
        form->init();
        connect(form, SIGNAL(destroyed(QObject*)), this, SLOT(childDestroyed(QObject*)));
    }

}


ReportInterface * ReportInterface::clone(bool withChildren)
{
    ReportInterface * report = new ReportInterface(*this, this->parent());
    if (!withChildren)
        return report;

//    foreach (QObject * child, children()) {
//        if (dynamic_cast<PageInterface*>(child)) {
//            PageInterface * page = reinterpret_cast<PageInterface*>(child)->clone(true);
//            foreach (BaseItemInterface * item, children()))
//        }
//    }


    return report;
}


QString ReportInterface::script()
{
    return m_script;
}


void ReportInterface::setScript(const QString & script)
{
    if (m_script == script)
        return;

    m_script = script;

    emit scriptChanged(m_script);
    emit changed();
}


QString ReportInterface::name()
{
    return m_name;
}


void ReportInterface::setName(const QString & name)
{
    if (m_name == name)
        return;
    m_name = name;

    emit nameChanged(m_name);
    emit changed();
}


QString ReportInterface::author()
{
    return m_author;
}


void ReportInterface::setAuthor(const QString & author)
{
    if (m_author == author)
        return;
    m_author = author;

    emit authorChanged(m_author);
    emit changed();
}


QList<PageInterface *> ReportInterface::pages()
{
    return this->findChildren<CuteReport::PageInterface*>();
}


void ReportInterface::addPage (PageInterface * page)
{
    page->setParent(this);

    connect(page, SIGNAL(destroyed(QObject*)), this, SLOT(childDestroyed(QObject*)));
    connect(page, SIGNAL(afterNewItemAdded(CuteReport::BaseItemInterface*,QPointF)),
            this, SLOT(slotItemAdded(CuteReport::BaseItemInterface*,QPointF)));
    connect(page, SIGNAL(afterItemRemoved(CuteReport::BaseItemInterface*)),
            this, SLOT(slotItemRemoved(CuteReport::BaseItemInterface*)));

    emit pageAdded(page);
    emit changed();
}


void ReportInterface::deletePage(PageInterface * page)
{
    /** disconnect for prevent triggering destroyed object postprocessing */
    disconnect(page, 0, this, 0);
    delete page;

    emit pageDeleted(page);
    emit changed();
}


QList<DatasetInterface *> ReportInterface::datasets()
{
    return this->findChildren<CuteReport::DatasetInterface*>();
}


DatasetInterface * ReportInterface::dataset(const QString & datasetName)
{
    return findChild<DatasetInterface *>(datasetName);
}


void ReportInterface::addDatasets(QList<DatasetInterface *> datasets)
{
    foreach (DatasetInterface * dataset, datasets) {
        dataset->setParent(this);
        connect(dataset, SIGNAL(destroyed(QObject*)), this, SLOT(childDestroyed(QObject*)));
        if (m_flags.testFlag(VariablesAutoUpdate))
            connect(dataset, SIGNAL(variablesChanged()), this, SLOT(slotObjectVariablesChanged()), Qt::UniqueConnection);

        emit datasetAdded(dataset);
        emit changed();
    }
}


void ReportInterface::addDataset(DatasetInterface* dataset)
{
    dataset->setParent(this);
    connect(dataset, SIGNAL(destroyed(QObject*)), this, SLOT(childDestroyed(QObject*)));
    if (m_flags.testFlag(VariablesAutoUpdate))
        connect(dataset, SIGNAL(variablesChanged()), this, SLOT(slotObjectVariablesChanged()), Qt::UniqueConnection);

    emit datasetAdded(dataset);
    emit changed();
}


void ReportInterface::deleteDataset(DatasetInterface* dataset)
{
    /** disconnect for prevent triggering destroyed object postprocessing */
    disconnect(dataset, 0, this, 0);
    delete dataset;

    emit datasetDeleted(dataset);
    emit changed();
}


QList<FormInterface*> ReportInterface::forms()
{
    return findChildren<FormInterface *>();
}


FormInterface* ReportInterface::form(const QString & formName)
{
    return findChild<FormInterface *>(formName);
}


void ReportInterface::addForm(FormInterface * form )
{
    form->setParent(this);

    connect(form, SIGNAL(destroyed(QObject*)), this, SLOT(childDestroyed(QObject*)));
    if (m_flags.testFlag(VariablesAutoUpdate))
        connect(form, SIGNAL(scriptingStringsChanged()), this, SLOT(slotScriptStringsChanged()), Qt::UniqueConnection);

    emit formAdded(form);
    emit changed();
}


void ReportInterface::deleteForm(FormInterface * form )
{
    /** disconnect for prevent triggering destroyed object postprocessing */
    disconnect(form, 0, this, 0);
    delete form;

    emit formDeleted(form);
    emit changed();
}


CuteReport::RendererInterface *ReportInterface::renderer() const
{
    return findChild<CuteReport::RendererInterface*>();
}


void ReportInterface::setRenderer(CuteReport::RendererInterface * renderer)
{
    if (!renderer)
        return;

    CuteReport::RendererInterface* currentRenderer = findChild<CuteReport::RendererInterface*>();
    if (currentRenderer == renderer)
        return;

    delete currentRenderer;
    renderer->setParent(this);

    emit rendererChanged(renderer);
    emit changed();
}


PrinterInterface * ReportInterface::printer() const
{
    return findChild<CuteReport::PrinterInterface*>();
}


void ReportInterface::setPrinter(PrinterInterface *printer)
{
    if (!printer)
        return;

    CuteReport::PrinterInterface* currentPrinter = findChild<CuteReport::PrinterInterface*>();
    if (currentPrinter == printer)
        return;

    delete currentPrinter;
    printer->setParent(this);

    emit printerChanged(printer);
    emit changed();
}


StorageInterface *ReportInterface::storage(const QString &storageName) const
{
    QList<StorageInterface *> list = findChildren<CuteReport::StorageInterface*>();
    StorageInterface * module = 0;

    foreach (StorageInterface * m, list) {
        if (m->moduleName() == storageName) {
            module = m;
            break;
        }
    }

    return module;
}


StorageInterface *ReportInterface::storageByUrl(const QString &url) const
{
    QString scheme = url.section(':', 0,0);
    return storageByScheme(scheme);
}


CuteReport::StorageInterface* ReportInterface::storageByScheme(const QString & scheme) const
{
    QList<StorageInterface *> list = findChildren<CuteReport::StorageInterface*>();
    StorageInterface * module = 0;
    foreach (StorageInterface * m, list) {
        if (m->urlScheme() == scheme) {
            module = m;
            break;
        }
    }

    return module;
}


QList<StorageInterface *> ReportInterface::storages() const
{
    return findChildren<CuteReport::StorageInterface*>();
}


QStringList ReportInterface::storagesName() const
{
    QStringList list;
    foreach (StorageInterface * storage, storages())
        list << storage->moduleName();
    return list;
}


void ReportInterface::setStorage(StorageInterface *storage)
{
    if (!storage || storage->parent() == this)
        return;

    QList<CuteReport::StorageInterface*> storages = findChildren<CuteReport::StorageInterface*>();
    CuteReport::StorageInterface* existentStorage = 0;
    foreach(CuteReport::StorageInterface* st, storages) {
        if (storage->moduleName() == st->moduleName()) {
            existentStorage = st;
            break;
        }
    }

    if (existentStorage) {
        deleteStorage(existentStorage);
    }

    storage->setParent(this);

    emit storageAdded(storage);
    emit changed();
}


void ReportInterface::deleteStorage(const QString & moduleName)
{
    QList<CuteReport::StorageInterface*> storages = findChildren<CuteReport::StorageInterface*>();
    CuteReport::StorageInterface* existentStorage = 0;
    foreach(CuteReport::StorageInterface* st, storages)
        if (moduleName == st->moduleName()) {
            existentStorage = st;
            break;
        }

    if (!existentStorage)
        return;

    if (m_defaultStorageName == existentStorage->moduleName())
        m_defaultStorageName = QString();

    existentStorage->deleteLater();

    emit storageDeleted(existentStorage);
    emit changed();
}


void ReportInterface::deleteStorage(StorageInterface * storage)
{
    QList<CuteReport::StorageInterface*> storages = findChildren<CuteReport::StorageInterface*>();
    CuteReport::StorageInterface* existentStorage = 0;
    foreach(CuteReport::StorageInterface* st, storages)
        if (storage->moduleName() == st->moduleName()) {
            existentStorage = st;
            break;
        }

    if (!existentStorage)
        return;

    if (m_defaultStorageName == existentStorage->moduleName())
        m_defaultStorageName = QString();

    existentStorage->deleteLater();

    emit storageDeleted(existentStorage);
    emit changed();
}


bool ReportInterface::hasStorage(const QString & moduleName)
{
    QList<CuteReport::StorageInterface*> storages = findChildren<CuteReport::StorageInterface*>();
    CuteReport::StorageInterface* existentStorage = 0;
    foreach(CuteReport::StorageInterface* st, storages)
        if (moduleName == st->moduleName()) {
            existentStorage = st;
            break;
        }

    return existentStorage;
}


QString ReportInterface::defaultStorageName() const
{
    return m_defaultStorageName;
}


void ReportInterface::setDefaultStorageName(const QString & name)
{
    if (m_defaultStorageName == name)
        return;

    m_defaultStorageName = name;

    emit defaultStorageNameChanged(m_defaultStorageName);
    emit changed();
}


StorageInterface * ReportInterface::defaultStorage() const
{
    StorageInterface * defaultStorage = 0;

    if (!m_defaultStorageName.isEmpty()) {
        foreach (CuteReport::StorageInterface * storage, storages())
            if (storage->moduleName() == m_defaultStorageName) {
                defaultStorage = storage;
                break;
            }
    }

    return defaultStorage;
}


// FIXMI: not imlemented report file version checking
double ReportInterface::version()
{
    return m_version;
}


void ReportInterface::setVersion(double tVersion)
{
    m_version = tVersion;
}


int ReportInterface::suitId()
{
    return 0;
}


QString ReportInterface::description()
{
    return m_description;
}


void ReportInterface::setDescription(const QString & description)
{
    if (m_description == description)
        return;

    m_description = description;

    emit descriptionChanged(m_description);
    emit changed();
}


QString ReportInterface::filePath()
{
    return m_filePath;
}


void ReportInterface::setFilePath(const QString & filePath)
{
    if (filePath == m_filePath)
        return;

    m_filePath = filePath;

    emit filePathChanged(m_filePath);
    emit changed();
}


const QVariantHash &ReportInterface::variables()
{
    return m_variables;
}


void ReportInterface::setVariables (const QVariantHash &vars)
{
    if (m_variables == vars)
        return;

    m_variables = vars;

    emit variablesChanged();
    emit changed();
}


void ReportInterface::setVariableValue(const QString & name, const QVariant & value)
{
    m_variables.insert(name, value);
}


void ReportInterface::updateVariables()
{
    QVariantHash oldVariables = m_variables;
    m_variables.clear();

    QSet<QString> set;

    set.unite( findVariables(m_script) );

    foreach (PageInterface * page, pages()) {
        foreach (BaseItemInterface * item, page->items()) {
            QStringList scriptingStrings = item->scriptingStrings();
            foreach (const QString & str, scriptingStrings) {
                set.unite( findVariables(str) );
            }
        }
    }

    foreach (DatasetInterface * ds, datasets()) {
        set.unite(ds->variables());
    }

    foreach (FormInterface * form, forms()) {
        QStringList scriptingStrings = form->scriptingStrings();
        foreach (const QString & str, scriptingStrings) {
            set.unite( findVariables(str) );
        }
    }

    foreach (const QString &key, set)
        m_variables.insert(key, oldVariables.value(key));

    variablesChanged();
}


void ReportInterface::childDestroyed(QObject * object)
{
    if (qobject_cast<CuteReport::DatasetInterface*>(object))
        emit datasetDeleted((CuteReport::DatasetInterface*) object);
    else if (qobject_cast<CuteReport::FormInterface*>(object))
        emit formDeleted((CuteReport::FormInterface*) object);
    else if (qobject_cast<CuteReport::PageInterface*>(object))
        emit pageDeleted((CuteReport::PageInterface*) object);
}


void ReportInterface::slotItemAdded(CuteReport::BaseItemInterface*item,QPointF)
{
    emit itemAdded(item);
}


void ReportInterface::slotItemRemoved(CuteReport::BaseItemInterface* item)
{
    emit itemDeleted(item);
}


void ReportInterface::setFlag(ReportFlag flag, bool enable)
{
    ReportFlags prev = m_flags;
    if (enable)
        m_flags |= flag;
    else
        m_flags &= ~flag;
    precessFlags(prev);
}


void ReportInterface::setFlags(ReportFlags flags)
{
    ReportFlags prev = m_flags;
    m_flags = flags;
    precessFlags(prev);
}


bool ReportInterface::isFlagSet(ReportFlag flag)
{
    return m_flags.testFlag(flag);
}


ReportInterface::ReportFlags ReportInterface::flags()
{
    return m_flags;
}


void ReportInterface::precessFlags(ReportFlags previousFlags)
{
    if (m_flags.testFlag(VariablesAutoUpdate) && !previousFlags.testFlag(VariablesAutoUpdate)) {

        foreach (PageInterface * page, pages()) {
            foreach (BaseItemInterface * item, page->items())
                connect(item, SIGNAL(scriptingStringsChanged()), this, SLOT(slotScriptStringsChanged()), Qt::UniqueConnection);
        }
        foreach (DatasetInterface * dataset, datasets())
            connect(dataset, SIGNAL(variablesChanged()), this, SLOT(slotObjectVariablesChanged()), Qt::UniqueConnection);
        foreach (FormInterface * form, forms())
            connect(form, SIGNAL(scriptingStringsChanged()), this, SLOT(slotScriptStringsChanged()), Qt::UniqueConnection);

        connect(this, SIGNAL(scriptChanged(QString)), this, SLOT(slotScriptStringsChanged()), Qt::UniqueConnection);
    }

    if (!m_flags.testFlag(VariablesAutoUpdate) && previousFlags.testFlag(VariablesAutoUpdate)) {

        foreach (PageInterface * page, pages()) {
            foreach (BaseItemInterface * item, page->items())
                disconnect(item, SIGNAL(scriptingStringsChanged()), this, SLOT(slotScriptStringsChanged()));
        }
        foreach (DatasetInterface * dataset, datasets())
            disconnect(dataset, SIGNAL(variablesChanged()), this, SLOT(slotObjectVariablesChanged()));
        foreach (FormInterface * form, forms())
            disconnect(form, SIGNAL(scriptingStringsChanged()), this, SLOT(slotScriptStringsChanged()));

        disconnect(this, SIGNAL(scriptChanged(QString)), this, SLOT(slotScriptStringsChanged()));
    }

    if (m_flags.testFlag(DirtynessAutoUpdate) && !previousFlags.testFlag(DirtynessAutoUpdate)) {
        foreach (PageInterface * page, pages()) {
            connect(page, SIGNAL(afterNewItemAdded(CuteReport::BaseItemInterface*,QPointF)),
                    this, SLOT(slotNewItemAdded(CuteReport::BaseItemInterface*)), Qt::UniqueConnection);
            foreach (BaseItemInterface * item, page->items())
                connect(item, SIGNAL(changed()), this, SLOT(setDirty()), Qt::UniqueConnection);
        }
        foreach (DatasetInterface * dataset, datasets())
            connect(dataset, SIGNAL(changed()), this, SLOT(setDirty()), Qt::UniqueConnection);
        foreach (FormInterface * form, forms())
            connect(form, SIGNAL(changed()), this, SLOT(setDirty()), Qt::UniqueConnection);

        connect(this, SIGNAL(changed()), this, SLOT(setDirty()), Qt::UniqueConnection);
    }

    if (!m_flags.testFlag(DirtynessAutoUpdate) && previousFlags.testFlag(DirtynessAutoUpdate)) {
        foreach (PageInterface * page, pages()) {
            disconnect(page, SIGNAL(afterNewItemAdded(CuteReport::BaseItemInterface*,QPointF)),
                    this, SLOT(slotNewItemAdded(CuteReport::BaseItemInterface*)));
            foreach (BaseItemInterface * item, page->items())
                disconnect(item, SIGNAL(changed()), this, SLOT(setDirty()));
        }
        foreach (DatasetInterface * dataset, datasets())
            disconnect(dataset, SIGNAL(changed()), this, SLOT(setDirty()));
        foreach (FormInterface * form, forms())
            disconnect(form, SIGNAL(changed()), this, SLOT(setDirty()));

        disconnect(this, SIGNAL(changed()), this, SLOT(setDirty()));
    }
}


void ReportInterface::slotScriptStringsChanged()
{
    updateVariables();

    emit variablesChanged();
    emit changed();
}


void ReportInterface::slotObjectVariablesChanged()
{
    updateVariables();

    emit variablesChanged();
    emit changed();
}


bool ReportInterface::isDirty() const
{
    return m_isDirty;
}


void ReportInterface::setDirty(bool b)
{
    if (m_isDirty == b)
        return;

    m_isDirty = b;

    emit dirtynessChanged(m_isDirty);
}


bool ReportInterface::isValid() const
{
    return m_isValid;
}


void ReportInterface::setValid(bool b)
{
    if (m_isValid == b)
        return;

    m_isValid = b;

    emit validityChanged(m_isValid);
}


void ReportInterface::setInvalid()
{
    if (!m_isValid)
        return;

    m_isValid = false;

    emit validityChanged(m_isValid);
}


void ReportInterface::slotNewItemAdded(CuteReport::BaseItemInterface *item)
{
    if (m_flags.testFlag(DirtynessAutoUpdate)) {
        connect(item, SIGNAL(changed()), this, SLOT(setDirty()), Qt::UniqueConnection);
    }
}


RenderedReportInterface *ReportInterface::renderedReport() const
{
    return m_renderedReport;
}


void ReportInterface::setRenderedReport(RenderedReportInterface *renderedReport)
{
    delete m_renderedReport;
    m_renderedReport = renderedReport;
    if (m_renderedReport)
        m_renderedReport->setParent(this);
}


void ReportInterface::clearRenderedReport()
{
    delete m_renderedReport;
    m_renderedReport = 0;
}
