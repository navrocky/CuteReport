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

#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QtCore>
#include "types.h"
#include "globals.h"

namespace CuteReport {

class LogProcessor;
class LogMouseHandler;

class CUTEREPORT_EXPORTS Log : public QObject
{
    Q_OBJECT
public:
    struct Destination {
        bool console;
        bool file;
        bool needContext;
        bool needLogfileTimestamp;
        Destination() {
            console = file = needContext = needLogfileTimestamp = false;
        }
    };

    struct Filter {
        QRegExp re;
        QString replace;
    };

    static QHash<LogLevel, QString> LogLevelNames;

    struct Module {
        QList <LogLevel> levels;
        QMap <int, Destination> destinations;
    };


    static void createInstance(QSettings * settings);
    static Log * instance();
    static void refCounterInc();
    static void refCounterDec();

    void push(LogLevel logLevel, const QString &sender, const QString &message);
    void push(LogLevel logLevel, const QString &sender, const QString &shortMessage, const QString &fullMessage);

    bool enabled() const;
    void setEnabled(bool enabled);

    QString logFileName() const;
    void setLogFileName(const QString &logFileName);

signals:
    void logMessage(CuteReport::LogLevel logLevel, const QString &sender, const QString &shortMessage, const QString &fullMessage);

private slots:
    void initMe();
    void initMouseHandler();

private:
    explicit Log(QObject *parent, QSettings *settings);
    ~Log();

#if QT_VERSION < 0x050000
    static void customMessageHandler(QtMsgType type, const char *msg);
#else
    static void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
#endif

    static inline void log_stderr(QString message);

    void setDefaultConfig();
    void startCustomHandler();
    void stopCustomHandler();
    void logRotate(QString filename = QString());
    QString infoPreprocess(QString str);
    QList<Filter> getFilters();
    LogLevel levelNameToEnum(QString name);

    Destination settingLineToDestination(QString level, QString line, bool listExists = false, QStringList allowedDestinations = QStringList());
    Module getGeneralLevelsSettings();
    QHash<QString, Log::Module> getModulesLevelsSettings();

private:
    static Log * m_log;
    static int m_refCounter;

    bool m_isInited;

    QThread * m_processorThread;
    LogProcessor * m_logProcessor;

    LogMouseHandler * m_mouseHandler;

    QSettings * m_settings;
    bool m_interceptQtMessages;
    bool m_interceptMouseEvents;
    qint64 m_logRotateFileSize;
    int m_logRotateCounter;
    bool m_sync;
    bool m_enabled;

};

}

#endif // LOG_H
