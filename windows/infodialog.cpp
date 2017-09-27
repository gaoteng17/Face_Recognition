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
	if (name.isEmpty())
	{
		QMessageBox::warning(this, tr("Waring"),
			tr("Please input your name!"),
			QMessageBox::Yes);
		ui->usrLineEdit->setFocus();
	}
	else if (id.isEmpty()) {

		QMessageBox::warning(this, tr("Waring"),
			tr("Please input your ID!"),
			QMessageBox::Yes);
		ui->pwdLineEdit->setFocus();

	}
	else
	{
		emit sendInfo(name, id);
		ui->pwdLineEdit->clear();
		ui->usrLineEdit->clear();
		this->close();
	}

}

void InfoDialog::on_exitBtn_clicked()
{
    emit sendsignal();
	ui->pwdLineEdit->clear();
	ui->usrLineEdit->clear();
    this->close();
}
