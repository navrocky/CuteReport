#include "exportdialog.h"
#include "ui_exportdialog.h"

ExportDialog::ExportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog)
{
    ui->setupUi(this);
    connect(ui->bExport, SIGNAL(clicked()), this, SLOT(exportClicked()));
    connect(ui->bCancel, SIGNAL(clicked()), this, SLOT(reject()));
}


ExportDialog::~ExportDialog()
{
    delete ui;
}


void ExportDialog::exportClicked()
{

}
