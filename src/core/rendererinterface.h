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
#ifndef RENDERERINTERFACE_H
#define RENDERERINTERFACE_H

#include "reportplugininterface.h"
#include <QObject>
#include <QGraphicsItem>
#include <QtScript>
#include <QtCore>

#include <qscriptengine.h>

namespace CuteReport
{

class ReportInterface;
class RenderedPageInterface;
class RenderedReportInterface;
class RenderedReportInterface;

class CUTEREPORT_EXPORTS RendererInterface : public ReportPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(CuteReport::ReportPluginInterface)

    Q_PROPERTY(int dpi READ dpi WRITE setDpi NOTIFY dpiChanged)

    Q_PROPERTY(int _current_property READ _currentProperty WRITE _setCurrentProperty DESIGNABLE false)
    Q_PROPERTY(QString _current_property_description READ _current_property_description DESIGNABLE false)

public:
    explicit RendererInterface(QObject *parent = 0);
    virtual ~RendererInterface();

    virtual RendererInterface * createInstance(QObject * parent = 0) const = 0;
    virtual RendererInterface * clone() const = 0;

    virtual void run(ReportInterface* report) = 0;
    virtual void stop() = 0;
    virtual bool isRunning() = 0;

    virtual ReportInterface * report() = 0;

    virtual void setDpi(int dpi) = 0;
    virtual int dpi() const = 0;

    virtual CuteReport::RenderedReportInterface * takeRenderedReport() = 0;

    virtual void _setCurrentProperty(int num) { m_currentProperty = num; }
    virtual int _currentProperty() { return m_currentProperty; }
    virtual QString _current_property_description() const;

    virtual QAbstractItemModel * functionsModel() {return 0;}
    virtual QAbstractItemModel * variablesModel() {return 0;}

signals:
    void started();
    void done(bool errorsFound);
    void cancelled();
    void processingPage(int page, int total);
    void dpiChanged(int);

protected:
    explicit RendererInterface(const RendererInterface &dd, QObject * parent);

    friend class RenderedReportInterface;

    int m_currentProperty;
};

} //namespace

Q_DECLARE_INTERFACE(CuteReport::RendererInterface, "CuteReport.RendererInterface/1.0")

/// ------------------  template for additional metatypes
template <typename Tp>
QScriptValue qScriptValueFromQObject(QScriptEngine *engine, Tp const &qobject)
{
    return engine->newQObject(qobject);
}

template <typename Tp>
void qScriptValueToQObject(const QScriptValue &value, Tp &qobject)
{
    qobject = qobject_cast<Tp>(value.toQObject());
}

template <typename Tp>
int qScriptRegisterQObjectMetaType( QScriptEngine *engine, const QScriptValue &prototype = QScriptValue()
#ifndef qdoc
    , Tp * /* dummy */ = 0
#endif
    )
{
    return qScriptRegisterMetaType<Tp>(engine, qScriptValueFromQObject,
                                       qScriptValueToQObject, prototype);
}
/// --------------------------




#endif // RENDERERINTERFACE_H
