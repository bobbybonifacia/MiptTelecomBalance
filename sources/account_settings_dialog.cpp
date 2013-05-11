#include "account_settings_dialog.h"
#include <QCoreApplication>
#include <QIntValidator>
#include <QPushButton>
#include <QGridLayout>
#include <QValidator>
#include <QSettings>
#include <QLineEdit>
#include <QDialog>
#include <QString>
#include <QWidget>
#include <QLabel>
#include <QFile>  

AccountDialog::AccountDialog()
	:QDialog(0, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{
	p_label_number = new QLabel("Number:");
	p_label_password = new QLabel("Password:");
	
	p_number = new QLineEdit();
	p_val = new QIntValidator(1,9999999,this);
	p_number->setValidator(p_val);

	p_password = new QLineEdit();
	p_password->setEchoMode(QLineEdit::Password);

	p_label_number->setBuddy(p_number);
	p_label_password->setBuddy(p_password);

	p_ok = new QPushButton("Ok");
	p_exit = new QPushButton("Exit");

	QObject::connect(p_ok, SIGNAL(clicked()), this, SLOT(ok_acc()));
	QObject::connect(p_exit, SIGNAL(clicked()), this, SLOT(reject()));

	p_layout = new QGridLayout;
	p_layout ->addWidget(p_label_number, 0, 0);
	p_layout ->addWidget(p_label_password, 1, 0);
	p_layout ->addWidget(p_number, 0, 1);
	p_layout ->addWidget(p_password , 1, 1);
	p_layout ->addWidget(p_ok, 2, 0);
	p_layout ->addWidget(p_exit, 2, 1);
	setLayout(p_layout);
	setWindowTitle("Account setting");
	setFixedSize(250,100);
	setModal(true);
}

AccountDialog::~AccountDialog(){
	delete	p_val;
	delete 	p_label_number;
	delete	p_label_password;
	delete	p_number;
	delete	p_password;
	delete	p_exit;
	delete	p_ok;
	delete	p_layout;
}


void AccountDialog::set_info(){
	int num = p_number->text().toInt();
	QString pwrd = p_password->text();
	acc_num = num;
	acc_pass = pwrd;
}

void AccountDialog::get_info_or_exit(){
	show();
	if(this->exec() == QDialog::Rejected)
		qApp->quit();

}


void AccountDialog::ok_acc(){
	int cur;
	QString str = p_number->text();
	if(p_val->validate(str,cur) == QValidator::Acceptable){
		set_info();
		accept();
	}
}