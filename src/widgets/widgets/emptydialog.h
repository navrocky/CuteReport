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
    void setHelperWidget(QWidget * widget);

public slots:
    void slotAccept();
    void slotReject();

private:
    void keyPressEvent(QKeyEvent * event);

    Ui::EmptyDialog *ui;
    bool m_helperWidget;
    QWidget *m_widget;
};

#endif // EMPTYDIALOG_H
