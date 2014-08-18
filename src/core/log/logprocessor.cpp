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
#include "logprocessor.h"
#include "log.h"
#include <QtGui>
#include <iostream>

#define QTHANDLER_IDENTIFIER  "qt"
#define LOGFILENAMEDEFAULT "log.txt"
#define LOGMODULE "Log::Logprocessor"

namespace CuteReport {

LogProcessor::LogProcessor(QObject *parent) :
    QObject(parent)
  ,m_heartbeat(0)
  ,m_useFilters(false)
  ,m_logFileName(LOGFILENAMEDEFAULT)
{
    initServices();
    connect (this, SIGNAL(internalMessage(int,QString)), this, SLOT(internalMessageSlot(int,QString)), Qt::QueuedConnection);
}


LogProcessor::~LogProcessor()
{
}


void LogProcessor::setContextSize(int size)
{
    m_context_size = size;
}


void LogProcessor::setMouseEventFile(QString file)
{
    if (!file.isEmpty()) {
        m_mouseEventFile = file;
        m_mouseEventSeparated = true;
    } else {
        m_mouseEventSeparated = false;
    }
}


void LogProcessor::setHeartbeatInterval(int heartbeatDelay)
{
    m_heartbeatDelay = heartbeatDelay;
}


void LogProcessor::initServices()
{
//    if (!m_weblogger) {
//        QString url = gCore->settingValue("Log/webServiceURL", "https://webservice.elacarte.com/").toString();
//        m_weblogger = new WebLogger(this, url);
//        connect (m_weblogger, SIGNAL(logMessage(int,QString)), this, SLOT(webLoggerMessage(int,QString)), Qt::QueuedConnection);
//    }

//    int m_heartbeatDelay = gCore->settingValue("Log/heartbeatDelay", 15).toInt() * 1000;
//    if (!m_heartbeat && m_heartbeatDelay > 0 ) {
//        m_heartbeat = new QTimer(this);
//        connect(m_heartbeat, SIGNAL(timeout()), this, SLOT(heartbeatProcessing()));
//        m_heartbeat->start(m_heartbeatDelay);
//        heartbeatProcessing();
//    }

//    m_keepZip = gCore->settingValue("Log/keepZip", false).toBool();
}


void LogProcessor::log_stderr(QString message)
{
    std::cerr <<  message.toStdString() << std::endl;
}


void LogProcessor::push(CuteReport::LogLevel logLevel, const QString &_sender, const QString &message, const QString &fullMessage)
{
    QMutexLocker locker(&m_mutex);

    QString _message(message);
    QString _module = _sender.section("::",0,0);
    QString _submodule = _sender.section("::",1,1);

    if (m_useFilters) {
        foreach (Log::Filter filter, m_filters)
            _message.replace(filter.re, filter.replace);
    }

    Log::Destination dest;

    if (m_modulesSettings.contains(_module+"_"+_submodule)) {
        if (m_modulesSettings.value(_module+"_"+_submodule).levels.contains(logLevel))
            dest = m_modulesSettings.value(_module).destinations.value(logLevel);
        else
            return;
    }
    else if (m_modulesSettings.contains(_module)) {
        if (m_modulesSettings.value(_module).levels.contains(logLevel))
            dest = m_modulesSettings.value(_module).destinations.value(logLevel);
        else
            return;
    } else {
        if (m_generalSettings.levels.contains(logLevel))
            dest = m_generalSettings.destinations.value(logLevel);
        else
            return;
    }

//    QString strLevel = Log::LogLevelNames.value(logLevel);

    if (dest.console) {
//        QString module = _submodule.isEmpty() ? _module : _module + "::" + _submodule;
        QString sender = _sender;
        if (sender.size() < 25)
            sender += QString(25 - sender.size(), ' ');
        if (fullMessage.isEmpty())
            log_stderr(QTime::currentTime().toString("HH:mm:ss  ") + sender + message  );
        else
            log_stderr(QTime::currentTime().toString("HH:mm:ss  ") + sender + message + " ^ " + fullMessage);
    }


    if (dest.file) {
        /// write mouse metric to separated file if needed
        QFile log(m_mouseEventSeparated && logLevel == LogMetric ? m_mouseEventFile : m_logFileName);
        if (log.open(QFile::WriteOnly| QFile::Append)) {
            QTextStream out(&log);
            QString date = dest.needLogfileTimestamp ? QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") : "";
            QString sender = _sender;
            if (sender.size() < 25)
                sender += QString(25 - sender.size(), ' ');
            if (fullMessage.isEmpty())
                out << QString("%1    %2 %3").arg(date).arg(sender).arg(_message) << '\n';
            else
                out << QString("%1    %2 %3 ^ %4").arg(date).arg(sender).arg(_message).arg(fullMessage) << '\n';
            log.close();
        }
    }
}


void LogProcessor::setFilters(QList <Log::Filter> filters)
{
    m_filters = filters;
    m_useFilters = m_filters.isEmpty() ? false : true;
}


void LogProcessor::setGeneralMatrix(Log::Module settings)
{
    QMutexLocker locker(&m_mutex);
    m_generalSettings = settings;
}


Log::Module LogProcessor::generalMatrix() const
{
    return m_generalSettings;
}


void LogProcessor::setModulesMatrix(QHash<QString, Log::Module> matrix)
{
    m_modulesSettings = matrix;
}


void LogProcessor::internalMessageSlot(int level, QString message)
{
    this->push(CuteReport::LogLevel(level), LOGMODULE, message, "");
}


QString LogProcessor::logFileName() const
{
    return m_logFileName;
}


void LogProcessor::setLogFileName(const QString &logFileName)
{
    QMutexLocker locker(&m_mutex);
    m_logFileName = logFileName;

    QMap <int, Log::Destination>::iterator i;
    for (i = m_generalSettings.destinations.begin(); i!=m_generalSettings.destinations.end(); ++i) {
        i.value().file = true;
    }
}



}
