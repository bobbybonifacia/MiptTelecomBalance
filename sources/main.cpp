#include "net_balance_getter.h"
#include "notification_icon.h"
#include "account_settings_dialog.h"
#include "full_settings_dialog.h"
#include "ApplicationSettings.hpp"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	ApplicationSettings mngr;
	mngr.restoreSaved();

	if(mngr.userInfoExist() == false){
		AccountDialog dialog;
		dialog.get_info_or_exit();
		mngr.userId = dialog.acc_num;
		mngr.userPassword = dialog.acc_pass;
		mngr.saveActual();
	}

	NotificationIcon icon;
	NetBalanceGetter getter;
	QObject::connect(&icon, SIGNAL(new_settings()), &icon, SLOT(ReloadSettings()));
	QObject::connect(&icon, SIGNAL(new_settings()), &getter, SLOT(reload_settings()));
	QObject::connect(&getter, SIGNAL(new_balance(double)), &icon, SLOT(ShowBalance(double)));
	QObject::connect(&getter, SIGNAL(new_timeleft(double)), &icon, SLOT(UpdateTrafficStatistic(double)));
	QObject::connect(&getter, SIGNAL(failed()), &icon, SLOT(FailedGetBalance()));
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
	QObject::connect(&t, SIGNAL(userPassword(const QString &)), &getter, SLOT(set_password(const QString &)));

	
	if(t.exec() == QDialog::Rejected)
		return 0;
	getter.start();
	
	QApplication::setQuitOnLastWindowClosed(false);
	return a.exec();*/
}
