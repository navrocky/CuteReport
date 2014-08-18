#include "emptydialog.h"
#include <QAbstractButton>
#include <QKeyEvent>
#include <QDebug>
#include "ui_emptydialog.h"

EmptyDialog::EmptyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmptyDialog)
{
    ui->setupUi(this);
    foreach (QAbstractButton * b, ui->buttonBox->buttons())
        b->setFocusPolicy(Qt::NoFocus);
}


EmptyDialog::~EmptyDialog()
{
    delete ui;
}


void EmptyDialog::setWidget(QWidget * widget)
{
    ui->mainLayout->addWidget(widget);
}


void EmptyDialog::keyPressEvent(QKeyEvent * event)
{
    if (event->modifiers() == Qt::ControlModifier && (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) )
        accept();

    QDialog::keyPressEvent(event);
}

