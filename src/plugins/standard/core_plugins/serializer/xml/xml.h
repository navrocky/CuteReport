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
#ifndef SAVEXML_H
#define SAVEXML_H

#include "serializerinterface.h"
#include "globals.h"
#include <QDomDocument>

class SerializerXML : public CuteReport::SerializerInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteReport.SerializerInterface/1.0")
#endif

    Q_INTERFACES(CuteReport::SerializerInterface)
public:
    typedef QPair<QString, QString> StringPair;
    explicit SerializerXML(QObject * parent = 0);
    ~SerializerXML();
    virtual SerializerXML * createInstance(QObject * parent = 0) const;

    virtual QByteArray serialize(const QObject * object, bool * ok = 0);
    virtual QObject * deserialize(const QByteArray &data, bool * ok = 0);

    virtual QString lastError();

    virtual int moduleVersion() const {return 1;}
    virtual QString moduleName() const {return QString("xml");}

private:
//    QDomDocument reportToDom(const CuteReport::ReportInterface * report);
    CuteReport::ReportInterface * reportFromDom(QDomDocument * doc);
    QDomElement objectProperties(const QObject *object, QDomDocument * doc, const QString &objectName = QString());
    QObject * objectFromDom(QObject * parent, const QDomElement & dom, CuteReport::ReportInterface *report = 0);
    void setObjectPropertiesFromDom(QObject * object, const QDomElement & dom);
    void setObjectChildrenFromDom(QObject * object, const QDomElement & dom, CuteReport::ReportInterface *report = 0);
    QString objectType(const QObject * object, QList<StringPair> &specialAttributes) const;

    QDomElement variantToDom(QDomDocument * doc, const QString & name, const QVariant & value);
    QVariant domToVariant(const QDomElement & dom);

    QDomElement propertyToDom(QDomDocument * doc, const QObject *object, int propertyNum);

    QObject * createObject(CuteReport::ReportInterface *report, const QString & classname, QObject *parent = 0) const;
    QObject * createObject(const QDomElement & dom, QObject * parent, CuteReport::ReportInterface *report) const;

    const QObject *                         pluginByClassName(const QString &name) const;
    const CuteReport::BaseItemInterface *itemPluginByClassName(const QString &name) const;
    const CuteReport::PageInterface *       pagePluginByClassName(const QString &name) const;
    const CuteReport::DatasetInterface *    datasetPluginByClassName(const QString &name) const;
    const CuteReport::PrinterInterface *    printerPluginByClassName(const QString &name) const;
    const CuteReport::RendererInterface *   rendererPluginByClassName(const QString &name) const;
    const CuteReport::StorageInterface *    storagePluginByClassName(const QString &name) const;
    const CuteReport::FormInterface *       formPluginByClassName(const QString &name) const;

private:
    QString m_lastError;
};


#endif // SAVEXML_H
