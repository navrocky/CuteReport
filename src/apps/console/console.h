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
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/

#ifndef CONSOLE_H
#define CONSOLE_H

#include <QObject>
#include "types.h"

namespace CuteReport {
class ReportCore;
class ReportPreview;
}

class Console : public QObject
{
    Q_OBJECT
public:
    enum Output {Unknown, File, Preview, Printer};
    explicit Console(QObject *parent = 0);
    ~Console();

    void run(int argc, char *argv[]);

private slots:
    void slotExportDone(const QString &url, bool success);
    void slotPrintDone(const QString &url, bool success);
    void slotPreviewClosed();

private:
    void parseArguments(int argc, char *argv[]);
    void checkReport();
    QStringList modulesList(CuteReport::ModuleType type);
    QStringList moduleOptions(CuteReport::ModuleType type, const QString &moduleName);
    void showModuleHelp(CuteReport::ModuleType type, const QString &moduleName);

    CuteReport::ReportCore * m_reportCore;
    QStringList m_inputFiles;
    QString m_outputDir;
    QString m_logFile;
    Output m_output;
    QString m_format;
    QString m_storageOptions;
    QString m_printerOptions;
    QString m_rendererOptions;
    QString m_exportOptions;
    bool m_timeElapsedReport;
    bool m_timeElapsedTotal;
    int m_threads;
    int m_counter;
    CuteReport::ReportPreview * m_preview;
};

#endif // CONSOLE_H
