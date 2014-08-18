#include "renamedialog.h"
#include "reportinterface.h"
#include "reportcore.h"
#include "ui_renamedialog.h"

#include <QDialogButtonBox>
#include <QPushButton>


using namespace CuteReport;

RenameDialog::RenameDialog(QObject * object, CuteReport::ReportInterface * report, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RenameDialog),
    m_object(object),
    m_report(report)
{
    ui->setupUi(this);
    ui->validationText->clear();
    ui->name->setText(object->objectName());
    connect(ui->name, SIGNAL(textEdited(QString)), this, SLOT(textEdited(QString)));
}


RenameDialog::~RenameDialog()
{
    delete ui;
}

QString RenameDialog::newName()
{
    return ui->name->text();
}


void RenameDialog::textEdited(QString text)
{
    if (text.isEmpty()) {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        ui->validationText->setText("");
        return;
    }

    bool unique = ReportCore::isNameUnique(m_object, text, m_report);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(unique);
    ui->validationText->setText(unique ? "Ok" : "Object with this name already exists!");
}
