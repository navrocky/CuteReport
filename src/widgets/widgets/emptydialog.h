#ifndef EMPTYDIALOG_H
#define EMPTYDIALOG_H

#include "widgets_export.h"
#include <QDialog>

namespace Ui {
class EmptyDialog;
}

class WIDGET_EXPORTS EmptyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmptyDialog(QWidget *parent = 0);
    ~EmptyDialog();

    void setWidget(QWidget * widget);

private:
    void keyPressEvent(QKeyEvent * event);

    Ui::EmptyDialog *ui;
};

#endif // EMPTYDIALOG_H
