/***************************************************************************
 *   This file is part of the propertyEditor project                       *
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *   bog_dan_ro@yahoo.com                                                  *
 *   Copyright (C) 2013 by Mikhalov Alexander                              *
 *   alexander.mikhalov@gmail.com                                          *
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
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ****************************************************************************/

#include "propertyeditorcore.h"
#include "propertyinterface.h"

#include <QDebug>
#include <QFileInfoList>
#include <QStringList>
#include <QDir>
#include <QPluginLoader>

#define MODULENAME "PropertyEditor"

namespace PropertyEditor
{


//PluginManager * PluginManager::instance()
//{
//    if (!m_instance) {
//        m_instance = new PluginManager();
//        m_instance->loadPlugins();
//    }

//    return m_instance;
//}


PluginManager::PluginManager(QObject *parent) :
    QObject(parent),
    m_counter(0)
{
    qDebug() << "PropertyEditorCore CTOR";
    loadPlugins();
}


PluginManager::~PluginManager()
{
//    qDebug() << "PropertyEditorCore DTOR";
//    foreach (PropertyInterface * plugin, m_plugins) {
//        qDebug() << plugin->metaObject()->className();
//        delete plugin;
//    }

    qDeleteAll(m_plugins);
    m_plugins.clear();
}


const QList<PropertyInterface*> & PluginManager::plugins() const
{
    return m_plugins;
}


void PluginManager::loadPlugins()
{
    // looking for static plugins
    foreach (QObject *pluginObject, QPluginLoader::staticInstances()) {
        PropertyInterface *plugin = qobject_cast<PropertyInterface*>(pluginObject);
        if (plugin) {
            emit log((int)LogDebug, MODULENAME, QString("Found static plugin: %1").arg(plugin->metaObject()->className()), "");
            m_plugins.push_back(qobject_cast<PropertyInterface*>(plugin));
        }
    }

    // looking for dynamic plugins
    QFileInfoList files;
    QStringList dirs;
    dirs << PROPERTYEDITOR_PLUGINS_PATH;
    dirs << "propertyeditor_plugins";

    foreach (const QString & dirName, dirs) {
        QDir pluginsDir(dirName);
        files += pluginsDir.entryInfoList(QDir::Files);
        emit log((int)LogDebug, MODULENAME, "Plugin dir: " + pluginsDir.absolutePath(), "");
    }

    QPluginLoader loader;
    loader.setLoadHints(QLibrary::ResolveAllSymbolsHint|QLibrary::ExportExternalSymbolsHint);
    foreach(const QFileInfo & fileName, files)
    {
        loader.setFileName( fileName.absoluteFilePath());
        if (!loader.load()) {
            emit log((int)LogWarning, MODULENAME, "Error while loading plugin " + fileName.fileName() + ": " + loader.errorString(), "");
            continue;
        }
        PropertyInterface *plugin = qobject_cast<PropertyInterface*>(loader.instance());
        if (plugin) {
            m_plugins.push_back(plugin);
            emit log((int)LogWarning, MODULENAME, "Loading plugin: " + fileName.baseName(), "");
        } else {
            loader.unload();
            emit log((int)LogWarning, MODULENAME, QString("It\'s not a PropertyEditor plugin: %1").arg(fileName.baseName()), "");
        }
    }
}

}
