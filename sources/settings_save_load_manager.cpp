#include "settings_save_load_manager.h"
#include <QCoreApplication>
#include <QIntValidator>
#include <QPushButton>
#include <QGridLayout>
#include <QCheckBox>
#include <QSettings>
#include <QLineEdit>
#include <QSlider>
#include <QDialog>
#include <QWidget>
#include <QLabel>

const int def_num = -1;
const QString def_pwrd = "";
const bool def_alerts = true;
const int def_alerts_boundary = 20;
const int def_timeout = 5;
const int def_price = 20;

SettingsManager::SettingsManager(){
	account_number = def_num;
	account_password = def_pwrd;
	use_alerts = def_alerts;
	alerts_boundary = def_alerts_boundary;
	timeup = def_timeout;
	price = def_price;
}

void SettingsManager::load_settings(){
	QCoreApplication::setOrganizationName("mipt");
	QCoreApplication::setApplicationName("balance");
	
	QSettings settings;
	account_number = settings.value("/Settings/account_number", def_num).toInt();
	account_password = settings.value("/Settings/account_password",def_pwrd).toString();
	use_alerts = settings.value("/Settings/use_alerts", def_alerts).toBool();
	alerts_boundary = settings.value("/Settings/alerts_boundary", def_alerts_boundary).toInt();
	timeup = settings.value("/Settings/timeup", def_timeout).toInt();
	price = settings.value("/Settings/price", def_price).toInt();
}


bool SettingsManager::is_account_good(){
	if ((account_number==def_num) ||( account_password==def_pwrd)) return false; else
		return true;
	
}

void SettingsManager::save_settings(){
		QCoreApplication::setOrganizationName("mipt");
		QCoreApplication::setApplicationName("balance");
	
		QSettings settings;
		settings.setValue("/Settings/account_number", account_number);
		settings.setValue("/Settings/account_password",account_password);
		settings.setValue("/Settings/use_alerts", use_alerts);
		settings.setValue("/Settings/alerts_boundary", alerts_boundary);
		settings.setValue("/Settings/timeup", timeup);
		settings.setValue("/Settings/price", price);
}
