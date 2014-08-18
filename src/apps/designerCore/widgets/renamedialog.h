#ifndef RENAMEDIALOG_H
#define RENAMEDIALOG_H

#include <QDialog>
#include "designer_globals.h"


namespace Ui {
class RenameDialog;
}


namespace CuteReport {
class ReportCore;
class ReportInterface;
}


class DESIGNER_EXPORTS RenameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RenameDialog(QObject * object, CuteReport::ReportInterface * report, QWidget *parent = 0);
    ~RenameDialog();

    QString newName();

private slots:
    void textEdited(QString text);

private:
    Ui::RenameDialog *ui;
    QObject * m_object;
    CuteReport::ReportInterface * m_report;
};

#endif // RENAMEDIALOG_H
