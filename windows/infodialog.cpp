#include "infodialog.h"
#include "ui_infodialog.h"
#include "mainwindow.h"
#include <QMessageBox>

InfoDialog::InfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InfoDialog)
{
    ui->setupUi(this);
}

InfoDialog::~InfoDialog()
{
    delete ui;
}

void InfoDialog::on_loginBtn_clicked()
{
    name = ui->usrLineEdit->text().trimmed();
    id = ui->pwdLineEdit->text();
    emit sendInfo(name,id);
    this->close();
}

void InfoDialog::on_exitBtn_clicked()
{
    emit sendsignal();
    this->close();
}
