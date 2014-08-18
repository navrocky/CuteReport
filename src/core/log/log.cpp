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

#include "log.h"
#include "reportcore.h"
#include "logprocessor.h"
#include "types.h"
#include "logmousehandler.h"

#include <iostream>
#include <QtGui>

#define QTHANDLER_IDENTIFIER  "Qt"
#define LOGFILENAME "log.txt"

namespace CuteReport {

QHash<LogLevel, QString> Log::LogLevelNames;

Log * Log::m_log = 0;
int Log::m_refCounter = 0;


void Log::createInstance(QSettings * settings)
{
    if (m_log)
        return;
    m_log = new Log(0, settings);
}


Log *Log::instance()
{
    return m_log;
}


void Log::refCounterInc()
{
    ++m_refCounter;
}


void Log::refCounterDec()
{
    --m_refCounter;
    if (!m_refCounter)
        delete Log::instance();
}


Log::Log(QObject *parent, QSettings * settings) :
    QObject(parent)
  , m_isInited (false)
  , m_processorThread (0)
  , m_logProcessor(0)
  , m_mouseHandler(0)
  , m_settings(settings)
  , m_sync(false)
  , m_enabled(true)
{
    LogLevelNames.insert(LogError, "error");
    LogLevelNames.insert(LogWarning, "warning");
    LogLevelNames.insert(LogInfo, "info");
    LogLevelNames.insert(LogDebug, "debug");
    LogLevelNames.insert(LogCritical, "critical");
    LogLevelNames.insert(LogMetric, "metric");

    initMe();
}


Log::~Log()
{
    stopCustomHandler();
//    delete m_mouseHandler;
    delete m_logProcessor;
    if (m_processorThread) {
//        m_processorThread->terminate();
        m_processorThread->quit();
        m_processorThread->wait();
        delete m_processorThread;
    }
    m_log = 0;

    qDebug() << "Log DTOR";
}


void Log::initMe()
{
    setDefaultConfig();

    m_interceptMouseEvents = m_settings ? m_settings->value("Log/mouseEvents", false).toBool() : false;
    m_interceptQtMessages = m_settings ? m_settings->value("Log/messagesQt", true).toBool() : false;
    m_logRotateCounter = m_settings ? m_settings->value("Log/logRotateCounter", 5).toLongLong() : 5;
    QString logRotateFileSize = m_settings ? m_settings->value("Log/logRotateFileSize", "50mb").toString() : "50mb";
    if (logRotateFileSize.contains("kb", Qt::CaseInsensitive))
        m_logRotateFileSize = logRotateFileSize.remove("kb", Qt::CaseInsensitive).toInt() * 1024;
    else if (logRotateFileSize.contains("mb", Qt::CaseInsensitive))
        m_logRotateFileSize = logRotateFileSize.remove("mb", Qt::CaseInsensitive).toInt() * 1024000;
    else
        m_logRotateFileSize = logRotateFileSize.toInt();

    m_sync = m_settings ? m_settings->value("Log/synchronously", true).toBool() : true;
    m_enabled = m_settings ? m_settings->value("Log/enabled", true).toBool() : true;

    m_isInited = true;

    /// do logrotate if needed
    logRotate();

    /// process it after exit from Core::init because gCore->scene() is not inited yet
    if (m_interceptMouseEvents)
        QTimer::singleShot(0, this, SLOT(initMouseHandler()));


    /// Setting up the qDebug handler only after m_isInited = true ensures that
    /// qDebug statements within the logger initialization don't cause stack (as
    /// warnings need to be logged if a log call is made during logger init.)
    if (m_interceptQtMessages)
        startCustomHandler();

    if (m_enabled) {
        QString context_size = m_settings ? m_settings->value("Log/contextMaxSize", "1kb").toString() : "1kb";
        int s = 0;
        if (context_size.contains("kb", Qt::CaseInsensitive))
            s = context_size.remove("kb", Qt::CaseInsensitive).toInt() * 1024;
        else if (context_size.contains("mb", Qt::CaseInsensitive))
            s = context_size.remove("mb", Qt::CaseInsensitive).toInt() * 1024000;
        else
            s = logRotateFileSize.toInt();

        m_logProcessor = new LogProcessor();
        m_logProcessor->setContextSize(s);
        m_logProcessor->setMouseEventFile(m_settings ? m_settings->value("Log/mouseLogFilename", "").toString() : "");
        m_logProcessor->setFilters(this->getFilters());
        m_logProcessor->setGeneralMatrix( this->getGeneralLevelsSettings());
        m_logProcessor->setModulesMatrix( this->getModulesLevelsSettings());

        /// synchronous or asynchronous
        ///
        if (m_sync) {
            log_stderr("Log is synchronous");
            connect(this, SIGNAL(logMessage(CuteReport::LogLevel,QString,QString,QString)), m_logProcessor, SLOT(push(CuteReport::LogLevel,QString,QString,QString)), Qt::DirectConnection);
        } else {
            log_stderr("Log is asynchronous");
            m_processorThread = new QThread(this);
            m_logProcessor->moveToThread(m_processorThread);
            connect(this, SIGNAL(logMessage(CuteReport::LogLevel,QString,QString,QString)), m_logProcessor, SLOT(push(CuteReport::LogLevel,QString,QString,QString)), Qt::QueuedConnection);
            m_processorThread->start(QThread::LowestPriority);

            log_stderr(QString("Log current thread pointer is %1").arg(qint64(this->thread())));
            log_stderr(QString("Log processor thread pointer is %1").arg(qint64(m_logProcessor->thread())));
        }
    }

}


void Log::push(LogLevel logLevel, const QString &sender, const QString &message)
{
    emit logMessage(logLevel, sender, message, "");
}


void Log::push(LogLevel logLevel, const QString &sender, const QString &shortMessage, const QString &fullMessage)
{
    emit logMessage(logLevel, sender, shortMessage, fullMessage);
}


void Log::log_stderr(QString message)
{
    std::cerr <<  message.toStdString() << std::endl;
}


void Log::startCustomHandler()
{
#if QT_VERSION < 0x050000
    qInstallMsgHandler(customMessageHandler);
#else
    qInstallMessageHandler(customMessageHandler);
#endif
}


void Log::stopCustomHandler()
{
#if QT_VERSION < 0x050000
    qInstallMsgHandler(0);
#else
    qInstallMessageHandler(0);
#endif
}


void Log::setDefaultConfig()
{
    if (!m_settings)
        return;

    if (m_settings->value("Log/logFileTimestampFor").toString().isEmpty())
        m_settings->setValue("Log/logFileTimestampFor","debug|warning|error|critical|info|metric");

    if (m_settings->value("Log/debugTo").toString().isEmpty())
        m_settings->setValue("Log/debugTo","console|file");

    if (m_settings->value("Log/warningTo").toString().isEmpty())
        m_settings->setValue("Log/warningTo","console|file");

    if (m_settings->value("Log/errorTo").toString().isEmpty())
        m_settings->setValue("Log/errorTo","console|file");

    if (m_settings->value("Log/criticalTo").toString().isEmpty())
        m_settings->setValue("Log/criticalTo","console|file");
}


Log::Destination Log::settingLineToDestination(QString level, QString line, bool listExists, QStringList allowedDestinations )
{
    Destination dest;

    if (m_settings) {
        QString valueStr = m_settings->value(line).toString();
        QStringList values = valueStr.split("|", QString::SkipEmptyParts);

        dest.needContext = m_settings->value("Log/contextFor").toString().contains(level, Qt::CaseInsensitive) ? true : false;
        dest.needLogfileTimestamp = m_settings->value("Log/logFileTimestampFor").toString().contains(level, Qt::CaseInsensitive) ? true : false;

        foreach (QString value, values) {
            QString str = value.trimmed();
            if (str == "console") {
                if (listExists && !allowedDestinations.contains(str))
                    continue;
                else
                    dest.console = true;
            } else if (str == "file") {
                if (listExists && !allowedDestinations.contains(str))
                    continue;
                else
                    dest.file = true;
            }
        }
    } else {
        dest.needContext = false;
        dest.needLogfileTimestamp = false;
        dest.console = true;
        dest.file = false;
    }

    return dest;
}


Log::Module Log::getGeneralLevelsSettings()
{
    Log::Module module;
    for (int i = 1; i<LOGLEVELMAXVALUE; ++i) {

        QString param = QString("Log/%1To").arg(LogLevelNames.value(LogLevel(i)));
        QString value = m_settings ? m_settings->value(param).toString() : "console";

        Destination dest = settingLineToDestination(LogLevelNames.value(LogLevel(i)), param);
        module.destinations.insert(i, dest);
        if (!value.isEmpty()) {
//            log_console(QString("value \'%1\'").arg(value));
            module.levels.append(LogLevel(i));
        }
    }

    //debug
    log_stderr("======= Log general settings =========");
    foreach (LogLevel id, module.levels) {
        log_stderr(QString("level \'%1\'(%2)").arg(LogLevelNames.value(id)).arg(id));
    }
    for (int i = 1; i<LOGLEVELMAXVALUE; ++i) {
        Destination dest = module.destinations.value(i);
        log_stderr(QString("Log level \'%1\'(%2):  Destinations: console:%3 file:%4").arg(LogLevelNames.value(LogLevel(i))).arg(i)
                    .arg(dest.console).arg(dest.file));
    }

    return module;
}


LogLevel Log::levelNameToEnum(QString name)
{
    for (int i=0; i<LOGLEVELMAXVALUE; ++i) {
        if (LogLevelNames.value(LogLevel(i)) == name)
            return LogLevel(i);
    }
    return LogUndefined;
}


QHash<QString, Log::Module> Log::getModulesLevelsSettings()
{
    QHash<QString, Log::Module> modules;

    if (!m_settings)
        return modules;

    m_settings->beginGroup("Log");
    QStringList keys = m_settings->allKeys();
    m_settings->endGroup();

    /// detecting defined modules name

    QStringList modulesNames;
    QStringList submodulesNames;

    foreach (QString key, keys) {
        QString moduleName;
        if (key.leftRef(13) == QString("moduleLevels_"))
            moduleName = key.midRef(13).toString();
        else if (key.leftRef(11) == QString("moduleDest_")) {
            /// detecting subkey moduleDest_debuglevel
            for (int i=1; i<LOGLEVELMAXVALUE; ++i) {
                QString searchKey =  QString("moduleDest_%1").arg(LogLevelNames.value(LogLevel(i)));
                if (key.leftRef(searchKey.size()) == searchKey) {
                    moduleName = key.midRef(searchKey.size()+1).toString();
                    break;
                }
            }

            if (moduleName.isEmpty()) {
                /// continue without specific debug level
                moduleName = key.midRef(11).toString();
            }
        }

        if (!moduleName.isEmpty()) {
            if (moduleName.contains("_")) {
                /// submodule
                if ((!moduleName.isEmpty()) && !submodulesNames.contains(moduleName))
                    submodulesNames.append(moduleName);
            } else {
                /// module
                if ((!moduleName.isEmpty()) && !modulesNames.contains(moduleName))
                    modulesNames.append(moduleName);
            }
        }
    }

    /// reading specific settings for defined modules
    foreach (QString moduleName, modulesNames) {
        Log::Module module;
        QStringList allowedDestinations;
        bool listExists = keys.contains(QString("moduleDest_%1").arg(moduleName));
        if (listExists) {
            allowedDestinations = m_settings->value(QString("Log/moduleDest_%1")
                                                      .arg(moduleName)).toString()
                                                      .split("|", QString::SkipEmptyParts);
        }

        /// parsing levels information
        if (keys.contains(QString("moduleLevels_%1").arg(moduleName))) {
//            module.levelsDefined = true;
            QStringList moduleLevels = m_settings->value(QString("Log/moduleLevels_%1")
                                                          .arg(moduleName)).toString()
                                                          .split("|", QString::SkipEmptyParts);
            foreach (QString level, moduleLevels) {
                LogLevel levelID = levelNameToEnum(level);
                if (levelID != -1)
                    module.levels.append(levelID);
                else
                    log_stderr(QString("Wrong level \'%1\' defined for module \'%2\'").arg(level).arg(moduleName));
            }
        } else {
            /// not defined, so will take general
            for (int i = 1; i<LOGLEVELMAXVALUE; ++i) {
                if (!m_settings->value(QString("%1To").arg(LogLevelNames.value(LogLevel(i)))).toString().isEmpty())
                    module.levels.append(LogLevel(i));
            }
        }


        /// checking special destinations for defined loglevels
        for (int i=1; i<LOGLEVELMAXVALUE; ++i) {
            QString levelKey = QString("moduleDest_%1_%2").arg(LogLevelNames.value(LogLevel(i))).arg(moduleName);

            if (keys.contains(levelKey)) {
                QString destForCurrentLevel = QString("Log/"+levelKey);
                Destination dest = settingLineToDestination(LogLevelNames.value(LogLevel(i)), destForCurrentLevel, listExists, allowedDestinations);
                module.destinations.insert(i, dest);
            } else {
                /// not defined - use general
                QString param = QString("Log/%1To").arg(LogLevelNames.value(LogLevel(i)));
                Destination dest = settingLineToDestination(LogLevelNames.value(LogLevel(i)), param, listExists, allowedDestinations);
                module.destinations.insert(i, dest);
            }
        }

        modules.insert(moduleName, module);

        /// debug
        log_stderr("Module name = " + moduleName);
        foreach (LogLevel id, module.levels) {
            log_stderr(QString("level \'%1\'(%2)").arg(LogLevelNames.value(id)).arg(id));
        }
        for (int i=1; i<LOGLEVELMAXVALUE; ++i) {
            Destination dest = module.destinations.value(i);
            log_stderr(QString("Log level \'%1\'(%2):  Destinations: console:%3 file:%4").arg(LogLevelNames.value(LogLevel(i))).arg(i)
                        .arg(dest.console).arg(dest.file));
        }
    }

    /// reading specific settings for defined submodules
    foreach (QString moduleName, submodulesNames) {
        Log::Module module;
        QString parentModuleName = moduleName.section("_",0,0);
        QStringList allowedDestinations;
        bool listExists = keys.contains(QString("moduleDest_%1").arg(moduleName));
        if (listExists) {
            allowedDestinations = m_settings->value(QString("Log/moduleDest_%1")
                                                      .arg(moduleName)).toString()
                    .split("|", QString::SkipEmptyParts);
        } else {
            listExists = keys.contains(QString("moduleDest_%1").arg(parentModuleName));
            if (listExists) {
                allowedDestinations = m_settings->value(QString("Log/moduleDest_%1")
                                                          .arg(parentModuleName)).toString()
                        .split("|", QString::SkipEmptyParts);
            }
        }

        /// parsing levels information
        if (keys.contains(QString("moduleLevels_%1").arg(moduleName))) {
            QStringList moduleLevels = m_settings->value(QString("Log/moduleLevels_%1")
                                                          .arg(moduleName)).toString()
                                                          .split("|", QString::SkipEmptyParts);
            foreach (QString level, moduleLevels) {
                LogLevel levelID = levelNameToEnum(level);
                if (levelID != -1)
                    module.levels.append(levelID);
                else
                    log_stderr(QString("Wrong level \'%1\' defined for module \'%2\'").arg(level).arg(moduleName));
            }
        } else {
            /// not defined, so will try parent
            if (modules.contains(parentModuleName)) {
                module.levels = modules.value(parentModuleName).levels;
            } else {
                /// then use general
                for (int i=1; i<LOGLEVELMAXVALUE; ++i) {
                    if (!m_settings->value(QString("%1To").arg(LogLevelNames.value(LogLevel(i)))).toString().isEmpty())
                        module.levels.append(LogLevel(i));
                }
            }
        }

        /// checking special destinations for defined loglevels
        for (int i=1; i<LOGLEVELMAXVALUE; ++i) {
            QString levelKey = QString("moduleDest_%1_%2").arg(LogLevelNames.value(LogLevel(i))).arg(moduleName);

            if (keys.contains(levelKey)) {
                QString destForCurrentLevel = QString("Log/"+levelKey);
                Destination dest = settingLineToDestination(LogLevelNames.value(LogLevel(i)), destForCurrentLevel, listExists, allowedDestinations);
                module.destinations.insert(i, dest);
            } else {
                /// not defined - try parent
                if (modules.contains(parentModuleName)) {
                    module.destinations.insert(i, modules.value(parentModuleName).destinations.value(i));
                } else {
                    // then use general
                    QString param = QString("Log/%1To").arg(LogLevelNames.value(LogLevel(i)));
                    Destination dest = settingLineToDestination(LogLevelNames.value(LogLevel(i)), param, listExists, allowedDestinations);
                    module.destinations.insert(i, dest);
                }
            }
        }

        modules.insert(moduleName, module);

        /// debug

        log_stderr("subModule name = " + moduleName);
        foreach (LogLevel id, module.levels) {
            log_stderr(QString("level \'%1\'(%2)").arg(LogLevelNames.value(LogLevel(id))).arg(id));
        }
        for (int i=1; i<LOGLEVELMAXVALUE; ++i) {
            Destination dest = module.destinations.value(i);
            log_stderr(QString("Log level \'%1\'(%2):").arg(LogLevelNames.value(LogLevel(i))).arg(i));
            log_stderr(QString("     Destinations: console: %1").arg(dest.console) + QString(" file: %1").arg(dest.file));
        }
    }

    return modules;
}


bool Log::enabled() const
{
    return m_enabled;
}


void Log::setEnabled(bool enabled)
{
    m_enabled = enabled;
}


QString Log::logFileName() const
{
    return m_logProcessor->logFileName();
}


void Log::setLogFileName(const QString &logFileName)
{
    if (logFileName != m_logProcessor->logFileName())
        logRotate(logFileName);

    m_logProcessor->setLogFileName(logFileName);
}


void Log::logRotate(QString filename)
{
    QFile log( filename.isEmpty() ? LOGFILENAME : filename);
    if (filename.isEmpty()) {     // default filec - first iteration
        if (log.size() > m_logRotateFileSize)
            logRotate(log.fileName());
    } else {                        // just renaming file
        QFileInfo fileinfo(log.fileName());
        QString suffix = fileinfo.suffix();
        int newsuffix = suffix.toInt() + 1;
        if (newsuffix <= m_logRotateCounter) {
            QString newfilename;
            if (suffix.toInt() == 0)        // still have no numeric additional suffix
                newfilename = fileinfo.dir().dirName() + "/" + fileinfo.fileName() + "." + QString::number(newsuffix);
            else
                newfilename = fileinfo.dir().dirName() + "/" + fileinfo.completeBaseName() + "." + QString::number(newsuffix);
            if (QFile::exists(newfilename))
                logRotate(newfilename);
            QFile::rename(log.fileName(), newfilename);
        } else {
            QFile::remove(log.fileName());
        }
    }
}


#if QT_VERSION < 0x050000
void Log::customMessageHandler(QtMsgType type, const char *msg)
{
    if (!m_log)
        log_stderr(msg);

    switch (type) {
    case QtDebugMsg:
        m_log->push(LogDebug, QString(QTHANDLER_IDENTIFIER) + "::Debug", QString(msg));
        break;
    case QtWarningMsg:
        m_log->push(LogWarning, QString(QTHANDLER_IDENTIFIER) + "::Warning", QString(msg));
        break;
    case QtCriticalMsg:
        m_log->push(LogCritical, QString(QTHANDLER_IDENTIFIER) + "::Critical", QString(msg));
        break;
    case QtFatalMsg:
        m_log->push(LogCritical, QString(QTHANDLER_IDENTIFIER) + "::Fatal", QString(msg));
        break;
    default:
        m_log->push(LogDebug, QString(QTHANDLER_IDENTIFIER) + "::Debug", QString(msg));
        break;
    }
}
#else
void Log::customMessageHandler(QtMsgType type,  const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context)

    if (!m_log)
        log_stderr(msg);

    QByteArray localMsg = msg.toLocal8Bit();
    //fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);

    switch (type) {
    case QtDebugMsg:
        m_log->push(LogDebug, QString(QTHANDLER_IDENTIFIER) + "::Debug", localMsg.constData());
        break;
    case QtWarningMsg:
        m_log->push(LogWarning, QString(QTHANDLER_IDENTIFIER) + "::Warning", localMsg.constData());
        break;
    case QtCriticalMsg:
        m_log->push(LogCritical, QString(QTHANDLER_IDENTIFIER) + "::Critical", localMsg.constData());
        break;
    case QtFatalMsg:
        m_log->push(LogCritical, QString(QTHANDLER_IDENTIFIER) + "::Fatal", localMsg.constData());
        break;
    default:
        m_log->push(LogDebug, QString(QTHANDLER_IDENTIFIER) + "::Debug", localMsg.constData());
        break;
    }
}
#endif

void Log::initMouseHandler()
{
//    m_mouseHandler = new LogMouseHandler(this);
//    gCore->scene()->installEventFilter(m_mouseHandler);
}


QList <Log::Filter> Log::getFilters()
{
    QList <Filter> filters;

    if (!m_settings)
        return filters;

    m_settings->beginGroup("Log");
    QStringList keys = m_settings->allKeys();
    m_settings->endGroup();

    foreach (QString key, keys) {
        if (key.leftRef(13) == QString("filterRegexp_")) {
            Filter f;
            QString filterName = key.midRef(13).toString();
            qDebug() << key;
            QString regExp = m_settings->value("Log/"+key).toString();
            QString replace = m_settings->value(QString("Log/filterAction_%1").arg(filterName)).toString();
            f.re = QRegExp(regExp);
            f.replace = replace;
            filters.append(f);
        }
    }
    return filters;
}



}
