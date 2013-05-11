#include "notification_icon.h"

#include "settings_save_load_manager.h"
#include "full_settings_dialog.h"
#include <QSystemTrayIcon>
#include <QApplication>
#include <QMessageBox>
#include <QMenu>


NotificationIcon::NotificationIcon()
{
	SettingsManager mngr;
	mngr.load_settings();
	use_alerts = mngr.use_alerts;
	few_money = false;
	few_money_boundary = mngr.alerts_boundary;

	QAction *show_settings	= new QAction("Settings", this);
	QAction *show_authors	= new QAction("About", this);
	QAction *app_exit		= new QAction("Exit", this);
	
	p_menu = new QMenu();
	p_menu->addAction(show_settings);
	p_menu->addAction(show_authors);
	p_menu->addAction(app_exit);

	QObject::connect(show_settings, SIGNAL(triggered()), this, SLOT(settings_dialog()));
	QObject::connect(show_authors, SIGNAL(triggered()), this, SLOT(about_dialog()));
	QObject::connect(app_exit, SIGNAL(triggered()), this, SLOT(app_exit()));


	p_icon = new QSystemTrayIcon(this);
	p_icon->setContextMenu(p_menu);


	connection_issues();
	p_icon->setToolTip("Starting...");
	p_icon->show();
}



void NotificationIcon::connection_issues(){
	p_icon->setToolTip("Failed to get the current balance.\nPlease, check the network connection\nand check your account settings.");
	p_icon->setIcon(QPixmap("unknown.png"));
}


void NotificationIcon::change_info(double new_balance){

	QString str_balance = QString::number(new_balance,'g',6);
	
	if (new_balance >=0 )
		p_icon->setIcon(QPixmap("good.png"));
	else
		p_icon->setIcon(QPixmap("bad.png"));

	if(use_alerts == true){
		if( (few_money == true) && (new_balance >= few_money_boundary))
			few_money = false;
	
		if( (few_money == false) && (new_balance < few_money_boundary)){
			few_money = true;
			QString title = "Balance is less than ";
			title.append(QString::number(few_money_boundary));
			title.append(" rubles!");
			p_icon->showMessage(title,"Please, update your balance.",QSystemTrayIcon::Warning);
		}
	}
	QString message = "Balance: ";
	message.append(str_balance);
	p_icon->setToolTip(message);
}


void NotificationIcon::about_dialog(){
	QMessageBox msgBox;
	msgBox.setWindowTitle("About");
	msgBox.setText("Mipt-telecom balance checker v0.3\nby:\n\tLantsov Alexander,\n\tKudinova Marina,\n\tAlekseeva Masha\n\n7 may 2013");
	msgBox.exec();
}


void NotificationIcon::settings_dialog(){
	FullSettingsDialog dialog;
	QObject::connect(&dialog, SIGNAL(new_settings()), this, SIGNAL(new_settings()));
	dialog.exec();


}



void NotificationIcon::app_exit(){
	p_icon->hide();
	qApp->quit();
}



void NotificationIcon::reload_settings(){
	SettingsManager mngr;
	mngr.load_settings();
	use_alerts = mngr.use_alerts;
	few_money = false;
	few_money_boundary = mngr.alerts_boundary;
}

