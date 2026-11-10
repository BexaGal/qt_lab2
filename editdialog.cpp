#include "editdialog.h"
#include "ui_editdialog.h"

EditDialog::EditDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditDialog)
{
    ui->setupUi(this);
}

void EditDialog::TransferData(QVariant var1){
    ui->textEdit->setText(var1.toString());
}

EditDialog::~EditDialog()
{
    delete ui;
}

void EditDialog::on_buttonBox_accepted()
{
    containedData = ui->textEdit->toPlainText();
    EditDialog::close();
}


void EditDialog::on_buttonBox_rejected()
{
    EditDialog::close();
}

