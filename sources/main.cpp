#include "net_balance_getter.h"
#include "notification_icon.h"
#include "account_settings_dialog.h"
#include "full_settings_dialog.h"
#include "settings_save_load_manager.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	SettingsManager mngr;
	mngr.load_settings();

	if(mngr.is_account_good() == false){
		AccountDialog dialog;
		dialog.get_info_or_exit();
		mngr.account_number = dialog.acc_num;
		mngr.account_password = dialog.acc_pass;
		mngr.save_settings();
	}

	NotificationIcon icon;
	NetBalanceGetter getter;
	QObject::connect(&icon, SIGNAL(new_settings()), &icon, SLOT(reload_settings()));
	QObject::connect(&icon, SIGNAL(new_settings()), &getter, SLOT(reload_settings()));
	QObject::connect(&getter, SIGNAL(new_balance(double)), &icon, SLOT(change_info(double)));
	QObject::connect(&getter, SIGNAL(failed()), &icon, SLOT(connection_issues()));
	getter.start();
	QApplication::setQuitOnLastWindowClosed(false);

	return app.exec();


	/*
	QApplication a(argc, argv);
	AccountDialog t;
	NetBalanceGetter getter;
	NotificationIcon w;
	FullSettingsDialog test;
	
	QObject::connect(&t, SIGNAL(account_number(int)), &getter, SLOT(set_account(int)));
	QObject::connect(&t, SIGNAL(account_password(const QString &)), &getter, SLOT(set_password(const QString &)));

	
	if(t.exec() == QDialog::Rejected)
		return 0;
	getter.start();
	
	QApplication::setQuitOnLastWindowClosed(false);
	return a.exec();*/
}
