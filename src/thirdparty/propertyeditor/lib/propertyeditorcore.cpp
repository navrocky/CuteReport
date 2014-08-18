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

PropertyEditorCore * PropertyEditorCore::m_instance = 0;


PropertyEditorCore *PropertyEditorCore::createInstance()
{
    if (!m_instance) {
        m_instance = new PropertyEditorCore();
    }

    return m_instance;
}


PropertyEditorCore * PropertyEditorCore::instance()
{
    if (!m_instance) {
        m_instance = new PropertyEditorCore();
        m_instance->loadPlugins();
    }

    return m_instance;
}


PropertyEditorCore::PropertyEditorCore(QObject *parent) :
    QObject(parent),
    m_counter(0)
{
    qDebug() << "PropertyEditorCore CTOR";
}


PropertyEditorCore::~PropertyEditorCore()
{
    qDebug() << "PropertyEditorCore DTOR";
    qDeleteAll(m_plugins);
}


void PropertyEditorCore::init()
{
    loadPlugins();
}


const QList<PropertyInterface*> & PropertyEditorCore::plugins() const
{
    return m_plugins;
}


void PropertyEditorCore::inc()
{
    ++m_counter;
}


void PropertyEditorCore::dec()
{
    --m_counter;
    if (!m_counter)
        delete this;
}


void PropertyEditorCore::loadPlugins()
{
    QFileInfoList files;
    QStringList dirs;
    dirs << PROPERTYEDITOR_PLUGINS_PATH;
    dirs << "propertyeditor_plugins";

    foreach (const QString & dirName, dirs) {
        QDir pluginsDir(dirName);
        files += pluginsDir.entryInfoList(QDir::Files);
    }

    /*
    // looking for static plugins
    foreach (QObject *plugin, QPluginLoader::staticInstances()) {
        if (plugin && qobject_cast<PropertyInterface*>(plugin))
            m_plugins.push_back(qobject_cast<PropertyInterface*>(plugin));
    }
*/

    // looking for dynamic plugins
    foreach(const QFileInfo & fileName, files)
    {
        emit log((int)LogDebug, MODULENAME, QString("Loading plugin: %1").arg(fileName.fileName()), "");
        QPluginLoader loader;
        loader.setLoadHints(QLibrary::ResolveAllSymbolsHint|QLibrary::ExportExternalSymbolsHint);
        loader.setFileName( fileName.absoluteFilePath());
        if (!loader.load())
        {
            qCritical() << loader.errorString();
            continue;
        }
        QObject *plugin = loader.instance();
        if (plugin && qobject_cast<PropertyInterface*>(plugin))
            m_plugins.push_back(qobject_cast<PropertyInterface*>(plugin));
        else
            emit log((int)LogWarning, MODULENAME, QString("It\'s not a PropertyEditor plugin: %1").arg(fileName.fileName()), "");
    }
}

}
