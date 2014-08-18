#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

namespace CuteReport {
class Report;
class ReportInterface;
}

class QSettings;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
public slots:
    void loadReport();
    void loadAndRunReport();

private:
    Ui::MainWindow *ui;
    QSettings * m_settings;
    CuteReport::Report * m_report;
    CuteReport::ReportInterface * m_reportTemplate;
};

#endif // MAINWINDOW_H
