#include "full_settings_dialog.h"
#include "ApplicationSettings.h"
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

FullSettingsDialog::FullSettingsDialog()
	:QDialog(0, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{
	ApplicationSettings mngr;
	mngr.restoreSaved();
	p_label_number		= new QLabel("Account number:");
	p_label_password	= new QLabel("Account password:");
	
	p_number	= new QLineEdit(QString::number(mngr.userId));
	p_val		= new QIntValidator(1,9999999,this);
	p_number->setValidator(p_val);
	
	p_password	= new QLineEdit(mngr.userPassword);
	p_password->setEchoMode(QLineEdit::Password);

	p_label_number->setBuddy(p_number);
	p_label_password->setBuddy(p_password);

	p_use_alerts = new QCheckBox("Show alert if balance is less then");
	p_use_alerts->setChecked(mngr.useLimitAlert);
	p_alert_lim = new QLineEdit(QString::number(mngr.boundaryLimitAlert));
	p_alert_lim->setValidator(p_val);

	p_lbl_uptime = new QLabel("Time up is  sec.");
	p_uptime = new QSlider(Qt::Horizontal);
	p_uptime->setRange(5, 60);
	p_uptime->setPageStep(5);
	p_uptime->setValue(mngr.balanceUpdatePeriod);
	lbl_uptime(mngr.balanceUpdatePeriod);

	p_ok = new QPushButton("Ok");
	p_cancel = new QPushButton("Cancel");
	p_exit = new QPushButton("Exit");
		
	QObject::connect(p_use_alerts, SIGNAL(stateChanged(int)), this, SLOT(alert_lock(int)));
	QObject::connect(p_ok, SIGNAL(clicked()), this, SLOT(dialog_accept()));
	QObject::connect(p_cancel, SIGNAL(clicked()), this, SLOT(reject()));
	QObject::connect(p_exit, SIGNAL(clicked()), qApp, SLOT(quit()));
	QObject::connect(p_uptime, SIGNAL(valueChanged(int)), this, SLOT(lbl_uptime(int)));

	p_layout = new QGridLayout;
	p_layout->addWidget(p_label_number	, 0, 0);
	p_layout->addWidget(p_label_password, 1, 0);
	p_layout->addWidget(p_number		, 0, 1, 1, 2);
	p_layout->addWidget(p_password		, 1, 1, 1, 2);
	p_layout->addWidget(p_use_alerts	, 2, 0, 1, 2);
	p_layout->addWidget(p_alert_lim		, 2, 2);
	p_layout->addWidget(p_lbl_uptime	, 3, 0, 1, 3);
	p_layout->addWidget(p_uptime		, 4 ,0, 1, 3);
	p_layout->addWidget(p_ok			, 5, 0);
	p_layout->addWidget(p_cancel		, 5, 1);
	p_layout->addWidget(p_exit			, 5, 2);
	setLayout(p_layout);

	setWindowTitle("Application settings");
	setFixedSize(350,200);
	setModal(false);
}

FullSettingsDialog::~FullSettingsDialog(){
	delete p_val;
	delete p_label_number;
	delete p_label_password;
	delete p_number;
	delete p_password;
	delete p_use_alerts;
	delete p_alert_lim;
	delete p_lbl_uptime;
	delete p_uptime;
	delete p_cancel;
	delete p_exit;
	delete p_ok;
	delete p_layout;

}


void FullSettingsDialog::alert_lock(int code){
	p_alert_lim->setReadOnly(!((bool)code));
}


void FullSettingsDialog::lbl_uptime(int balanceUpdatePeriod){
	QString text = "Time up is ";
	text.append(QString::number(balanceUpdatePeriod));
	text.append(" sec.");
	p_lbl_uptime->setText(text);
}


void FullSettingsDialog::dialog_accept(){
	ApplicationSettings mngr;
	mngr.userId = p_number->text().toInt();
	mngr.userPassword = p_password->text();
	mngr.boundaryLimitAlert = p_alert_lim->text().toInt();
	mngr.useLimitAlert = p_use_alerts->isChecked();
	mngr.balanceUpdatePeriod = p_uptime->value();
	mngr.saveActual();
	emit new_settings();
	accept();
}





