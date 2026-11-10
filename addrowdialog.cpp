#include "addrowdialog.h"
#include "ui_addrowdialog.h"

AddRowDialog::AddRowDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddRowDialog)
{
    ui->setupUi(this);
}

AddRowDialog::~AddRowDialog()
{
    delete ui;
}

void AddRowDialog::on_buttonBox_accepted()
{
    fromRow = ui->spinBox->value();
    rows = ui->spinBox_2->value();
}


void AddRowDialog::on_buttonBox_rejected()
{
    reject();
}

