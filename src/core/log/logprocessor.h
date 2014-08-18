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
#ifndef LOGPROCESSOR_H
#define LOGPROCESSOR_H

#include <QObject>
#include <QFileInfoList>
#include "log.h"

namespace CuteReport {

class LogProcessor : public QObject
{
    Q_OBJECT
public:
    enum MatrixResult {MatrixResultOk, MatrixResultNotFound, MarixResultDIsabled };
    explicit LogProcessor(QObject *parent = 0);
    ~LogProcessor();
    void initServices();
    void setMouseEventFile(QString file);
    void setContextSize(int size);
    void setHeartbeatInterval(int heartbeatDelay);
    void setFilters(QList <Log::Filter> filters);
    void setGeneralMatrix(Log::Module settings);
    Log::Module generalMatrix() const;
    void setModulesMatrix(QHash<QString, Log::Module> matrix);
    
    QString logFileName() const;
    void setLogFileName(const QString &logFileName);

signals:
    void internalMessage(int logLevel, const QString &_message);
    
public slots:
    void push(CuteReport::LogLevel logLevel, const QString &_sender, const QString &_message, const QString &fullMessage);

private slots:
    void internalMessageSlot(int level, QString message);

private:
    MatrixResult checkMatrixFor(int logLevel, QString module, Log::Destination * dest);
    inline void log_stderr(QString message);

private:
    Log::Destination debug;
    Log::Destination info;
    Log::Destination warning;
    Log::Destination error;
    Log::Destination critical;
    Log::Destination metric;

    qint64 m_context_size;
    QTimer * m_heartbeat;

    QString m_mouseEventFile;
    bool m_mouseEventSeparated;
    int m_heartbeatDelay;
    QList <Log::Filter> m_filters;
    bool m_useFilters;
    bool m_keepZip;

    Log::Module m_generalSettings;
    QHash<QString, Log::Module> m_modulesSettings;

    QString m_logFileName;

    QMutex m_mutex;
};

}

#endif // LOGPROCESSOR_H
