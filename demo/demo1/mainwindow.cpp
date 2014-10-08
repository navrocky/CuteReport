#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cutereport.h"
#include "reportpreview.h"
#include "stdstoragedialog.h"
#include "reportinterface.h"

#include <QtGui>
#include <QSettings>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_reportTemplate(0)
{
    ui->setupUi(this);

    m_settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "CuteReport", "Cute Report");
    m_report = new CuteReport::Report(this);

    ui->view->setReportCore(m_report->core());
    ui->view->setShowFlags(CuteReport::ReportPreview::ShowAllTools | CuteReport::ReportPreview::ShowBorderless
                           /*| CuteReport::PreviewContainer::ShowFitted*/ );

    connect(ui->actionOpen, SIGNAL(activated()), this, SLOT(loadReport()));
    connect(ui->actionOpen_and_Run, SIGNAL(activated()), this, SLOT(loadAndRunReport()));
}


MainWindow::~MainWindow()
{
    delete m_settings;
    delete ui;
}


void MainWindow::loadReport()
{
    /** deleting previous report instance if exists */
    delete m_reportTemplate;

    CuteReport::StdStorageDialog d(m_report->core(), this, "Open Report");
    if (d.exec() == QDialog::Accepted) {

        /** creating new report instance */
        CuteReport::ReportInterface * m_reportTemplate = m_report->loadReport(d.currentObjectUrl());
        if (!m_reportTemplate)
            return;

        m_reportTemplate->setParent(this);

        /** join report to preview widget
            report can be rendered by 2 ways: "run" button on PreviewContainer
            or by calling m_report->process(m_reportTemplate);
        */
        ui->view->connectReport(m_reportTemplate);
    }
}


void MainWindow::loadAndRunReport()
{
    /** deleting previous report instance if exists */
    delete m_reportTemplate;

    CuteReport::StdStorageDialog d(m_report->core(), this, "Open Report");
    if (d.exec() == QDialog::Accepted) {

        /** creating new report instance */
        CuteReport::ReportInterface * m_reportTemplate = m_report->loadReport(d.currentObjectUrl());

        if (!m_reportTemplate)
            return;

        m_reportTemplate->setParent(this);

        /** join report to preview widget
            report can be rendered by 2 ways: "run" button on PreviewContainer
            or by calling m_report->process(m_reportTemplate);
        */
        ui->view->connectReport(m_reportTemplate);
        m_report->process(m_reportTemplate);
    }
}
