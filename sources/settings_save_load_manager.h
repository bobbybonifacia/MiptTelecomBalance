#ifndef _SETTINGS_SAVE_LOAD_MANAGER_H_
#define _SETTINGS_SAVE_LOAD_MANAGER_H_

#include <QSettings>
#include <QString>

class SettingsManager{
public:
	int	account_number;
	QString account_password;
	bool use_alerts;
	int alerts_boundary;
	int timeup;
	int price;

	SettingsManager();
	void load_settings();
	bool is_account_good();
	void save_settings();
};
#endif