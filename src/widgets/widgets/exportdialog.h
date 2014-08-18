#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include "widgets_export.h"
#include <QDialog>

namespace Ui {
class ExportDialog;
}

class WIDGET_EXPORTS ExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(QWidget *parent = 0);
    ~ExportDialog();

public slots:
    void exportClicked();

private:
    Ui::ExportDialog *ui;
};

#endif // EXPORTDIALOG_H
