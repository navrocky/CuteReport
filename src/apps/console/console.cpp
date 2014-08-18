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

#include "console.h"
#include "stdio.h"
#include "reportcore.h"
#include "log.h"
#include "reportplugininterface.h"
#include "reportpreview.h"
#include "iostream"

static const char* help = "Usage: cutereport -i filename[,filename]...\n\n"
        "  -i, --input-file <filename[,filename...]> \n"
        "                                load report file\n"
        "  -u, --list-file <filename>    load report URLs found in local file\n"
        "  -d  --directory <dirname>     directory name for output files\n"
        "  -l, --log                     write log to specified file\n"
        "  -o, --output <DESTINATION>    output destination. See below.\n"
        "  -f  --format <format>         export to specified format. Default: pdf\n"
        //                          "  -s, --storage-options <NAME:opt1=value[,opt2=value2]>\n"
        //                          "                                set storage options for storage module NAME\n"
        //                          "  -p, --printer-options <NAME:opt1=value[,opt2=value2]>\n"
        //                          "                                set printer options for printer module NAME\n"
        //                          "  -r, --renderer-options <NAME:opt1=value[,opt2=value2]> \n"
        //                          "                                set renderer options for renderer module NAME\n"
        "  -e, --export-options <NAME:opt1=value[,opt2=value2]> \n"
        "                                set options for export module NAME\n"
        "  -n  --number-treads <num>     set the number of threads for batch reports\n"
        "                                rendering. Default:4\n"
        //                          "  -t  --time [report][,total]   elapsed time per report and/or total.\n"
        "  -v, --version                 display the version of CuteReport and exit\n"
        "  -h, --help [-s [NAME]] [-p [NAME]] [-r [NAME]]\n"
        "                                display this help and exit.\n"
        "                                If other options follow it then show accessible\n"
        "                                modules list. If NAME is specified then show \n"
        "                                options for named module\n"
        "             [-f]               List of export formats.\n"
        "  DESTINATION: it can be: file, printer, preview.\n"
        "    Use \'file\' if you want to render and export report to specific format file.\n"
        "    Use \'printer\' if you want print rendered report.\n"
        "    Use \'preview\' if you want to render report and watch it in preview window.\n";


Console::Console(QObject *parent) :
    QObject(parent),
    m_reportCore(0),
    m_output(Unknown),
    m_timeElapsedReport(false),
    m_timeElapsedTotal(false),
    m_threads(4),
    m_counter(0),
    m_preview(0)
{
}


Console::~Console()
{
    delete m_preview;
    delete m_reportCore;
}


void Console::run(int argc, char *argv[])
{
    parseArguments(argc, argv);
    delete m_reportCore;

    m_reportCore =  new CuteReport::ReportCore(0 ,0, false, !m_logFile.isEmpty());
    connect(m_reportCore, SIGNAL(exportDone(QString,bool)), this, SLOT(slotExportDone(QString,bool)));
    connect(m_reportCore, SIGNAL(printingDone(QString,bool)), this, SLOT(slotPrintDone(QString,bool)));

    if (!m_logFile.isEmpty()) {
        CuteReport::Log::instance()->setLogFileName(m_logFile);
    }

    m_reportCore->setMaxRenderingThreads(m_threads);

    foreach (const QString & fileName, m_inputFiles) {
        QUrl url(fileName);
        if (url.scheme().isEmpty())
            url.setScheme("file");

        QString urlStr = url.toString();

        switch (m_output) {
        case File: {
            QString output;
            if (m_outputDir.isEmpty()) {
                output = urlStr.section(".",0, -2) + "." + m_format;
            } else {
                output = urlStr.section('/', -2);
                output = output.section(".",0, -2) + "." + m_format;
            }
            m_reportCore->exportTo(url.toString(), m_format, output, m_exportOptions.split(",", QString::SkipEmptyParts));
            ++m_counter;
        }
            break;
        case Printer:
            m_reportCore->print(urlStr);
            ++m_counter;
            break;
        case Unknown:
        case Preview: {
            CuteReport::ReportInterface * report = m_reportCore->loadReport(urlStr);
            if (report) {
                m_preview = new CuteReport::ReportPreview;
                m_preview->setReportCore(m_reportCore);
                m_preview->connectReport(report);
                m_preview->show();
                m_preview->run();
                connect(m_preview, SIGNAL(closed()), this, SLOT(slotPreviewClosed()));
            } else {
                std::cout << QString("Error: Not correct input file: \'%1\'\n").arg(urlStr).toStdString();
                exit(1);
            }

            ++m_counter;
        }
            break;

        }
    }
}



void Console::slotExportDone(const QString & url, bool success)
{
    QString fileName = url.section('/', -1);
    QString result = QString("%1: %2").arg(fileName).arg(success ? "Success" : "Failed");
    std::cout << result.toStdString() << std::endl;
    --m_counter;
    if (m_counter <= 0)
        exit(0);
}


void Console::slotPrintDone(const QString &url, bool success)
{
    QString fileName = url.section('/', -1);
    QString result = QString("%1: %2").arg(fileName).arg(success ? "Success" : "Failed");
    std::cout << result.toStdString() << std::endl;
    --m_counter;
    if (m_counter <= 0)
        exit(0);
}


void Console::slotPreviewClosed()
{
    Q_ASSERT(m_preview);
    delete m_preview;
    exit(0);
}


void Console::checkReport()
{
    if (m_reportCore)
        return;

    m_reportCore =  new CuteReport::ReportCore(0,0,false,false);
}


QStringList Console::modulesList(CuteReport::ModuleType type)
{
    checkReport();
    return m_reportCore->moduleNames(type);
}


QStringList Console::moduleOptions(CuteReport::ModuleType type, const QString &moduleName)
{
    checkReport();
    const CuteReport::ReportPluginInterface * module = m_reportCore->module(type, moduleName );

    if (!module)
        return QStringList();

    QStringList list;

    for (int i = 2; i < module->metaObject()->propertyCount();i++) {
        QString propertyName = module->metaObject()->property(i).name();
        if (module->metaObject()->property(i).isDesignable() && module->metaObject()->property(i).isValid() &&
                module->metaObject()->property(i).isWritable() && module->metaObject()->property(i).isReadable() &&
                !propertyName.startsWith("_")  ) {
            list << QString("%1 (%2)").arg(propertyName).arg(module->metaObject()->property(i).typeName());
        }
    }

    return list;
}


void Console::showModuleHelp(CuteReport::ModuleType type, const QString &moduleName)
{
    bool showModuleList = true;
    if (!moduleName.isEmpty()) {
        QStringList list(moduleOptions(type, moduleName));
        if (!list.isEmpty()) {
            std::cout << QString("Options for module \'%1\':\n").arg(moduleName).toStdString();
            std::cout << QString("   " + list.join("\n   ")).toStdString();
            showModuleList = false;
        } else {
            std::cout << QString("Module name \'%1\' not found.\n").arg(moduleName).toStdString();
        }
    }

    if (showModuleList) {
        std::cout << "Accessible Storage Modules:\n";
        std::cout << "   " << modulesList(CuteReport::StorageModule).join("\n   ").toStdString() << std::endl;
    }

}


void Console::parseArguments(int argc, char *argv[])
{
    int i=1;
    int steps = 1;
    QString arg;
    QString nextArg;

    while (i<argc) {

        arg = argv[i];
        nextArg = (i+1<argc) ? argv[i+1] : QString();

        steps = 1;

        if (arg == "--version" || arg == "-v") {
            printf("cutereport version: %s\n", REPORT_VERSION);
            exit(0);
        } else if (arg == "--help" || arg == "-h") {
            bool extendedHelp = false;
            QString nextNextArg = (i+2<argc ? argv[i+2] : "");

            if (nextArg == "-s") {
                showModuleHelp(CuteReport::StorageModule, nextNextArg);
                extendedHelp = true;
            } else if (nextArg == "-p") {
                showModuleHelp(CuteReport::PrinterModule, nextNextArg);
                extendedHelp = true;
            } else if (nextArg == "-r") {
                showModuleHelp(CuteReport::RendererModule, nextNextArg);
                extendedHelp = true;
            }

            if (!nextArg.isEmpty())
                ++steps;
            if (!nextNextArg.isEmpty())
                ++steps;

            if (!extendedHelp)
                std::cout << help;

            exit(0);
        } else if (arg == "--input-file" || arg == "-i") {
            if (nextArg.startsWith("-")) {
                std::cout << QString("Error: Not correct input file: \'%1\'\n").arg(nextArg).toStdString();
                exit(1);
            }

            m_inputFiles = nextArg.split(',');
        } else if (arg == "--list-file" || arg == "-u") {
            if (nextArg.startsWith("-")) {
                std::cout << QString("Error: Not correct input file: \'%1\'\n").arg(nextArg).toStdString();
                exit(1);
            }

            QFile file(nextArg);

            if (!file.exists()) {
                std::cout << QString("Error: List file name is not exists: \'%1\'\n").arg(nextArg).toStdString();
                exit(1);
            }

            QTextStream stream(&file);
            QString line;
            do {
                m_inputFiles << stream.readLine();
            } while (!line.isNull());
        } else if (arg == "--directory" || arg == "-d") {
            if (!nextArg.startsWith("-")) {
                std::cout << QString("Error: Not correct output directory: \'%1\'\n").arg(nextArg).toStdString();
                exit(1);
            }
            m_outputDir = nextArg;
            QDir dir (m_outputDir);

            if (!dir.exists()) {
                std::cout << QString("Error: Output directory is not exists: \'%1\'\n").arg(nextArg).toStdString();
                exit(1);
            }
        } else if (arg == "--log" || arg == "-l") {
            if (nextArg.startsWith("-")) {
                std::cout << QString("Error: Not correct Log file: \'%1\'\n").arg(nextArg).toStdString();
                exit(1);
            }
            m_logFile = nextArg;
            QFileInfo fi(m_logFile);
            if (!fi.absoluteDir().exists()) {
                std::cout << QString("Error: Path for log file is not exists: \'%1\'\n").arg(nextArg).toStdString();
                exit(1);
            }
        } else if (arg == "--output" || arg == "-o") {
            if (nextArg == "printer")
                m_output = Printer;
            else if (nextArg == "file")
                m_output = File;
            else if (nextArg == "preview")
                m_output = Preview;
            else {
                std::cout << QString("Error: Not correct output type: \'%1\'\n").arg(nextArg).toStdString();
                exit(1);
            }
        } else if (arg == "--format" || arg == "-f") {
            if (nextArg.startsWith("-")) {
                std::cout << QString("Error: Format is not correct: \'%1\'\n").arg(nextArg).toStdString();
                exit(1);
            }
            m_format = nextArg;
        } else if (arg == "--storage-options" || arg == "-s") {
            if (nextArg.startsWith("-")) {
                std::cout << QString("Error: Storage options are not correct: \'%1\'\n").arg(nextArg).toStdString();
                exit(1);
            }
            m_storageOptions = nextArg;
        }  else if (arg == "--printer-options" || arg == "-p") {
            if (nextArg.startsWith("-")) {
                std::cout << QString("Error: Printer options are not correct: \'%1\'\n").arg(nextArg).toStdString();
                exit(1);
            }
            m_printerOptions = nextArg;
        } else if (arg == "--renderer-options" || arg == "-r") {
            if (nextArg.startsWith("-")) {
                std::cout << QString("Error: Renderer options is not correct: \'%1\'\n").arg(nextArg).toStdString();
                exit(1);
            }
            m_rendererOptions = nextArg;
        } else if (arg == "--export-options" || arg == "-e") {
            if (nextArg.startsWith("-")) {
                std::cout << QString("Error: Export options is not correct: \'%1\'\n").arg(nextArg).toStdString();
                exit(1);
            }
            m_exportOptions = nextArg;
        } else if (arg == "--number-treads" || arg == "-n") {
            bool ok;
            m_threads = nextArg.toInt(&ok);
            if (!ok) {
                std::cout << QString("Error: Threads number is not correct: \'%1\'\n").arg(nextArg).toStdString();
                exit(1);
            }
        } else if (arg == "--time" || arg == "-t") {
            QStringList list = nextArg.split(',');
            foreach(const QString &time, list) {
                if (time == "report")
                    m_timeElapsedReport = true;
                else if (time == "total")
                    m_timeElapsedTotal = true;
                else {
                    std::cout << QString("Error: Renderer options are not correct: \'%1\'\n").arg(nextArg).toStdString();
                    exit(1);
                }
            }
        }


        i += steps;
    }

    bool error = false;

    if (m_inputFiles.isEmpty()) {
        std::cout << "Error: input files are not specified.\n";
        error = true;
    }

    if (m_output == Unknown) {
        std::cout << "Error: Output type is not specified.\n";
        error = true;
    }

    if (m_output == File && m_format.isEmpty()) {
        std::cout << "Error: Export format is not specified.\n";
        error = true;
    }

    if (m_output == Preview && m_inputFiles.count() > 1) {
        std::cout << "Error: Only one defined file is allowed for preview. There are " << m_inputFiles.count() << std::endl;
        error = true;
    }


    if (error) {
        std::cout << "Use -h option for help.\n";
        exit(1);
    }
}

