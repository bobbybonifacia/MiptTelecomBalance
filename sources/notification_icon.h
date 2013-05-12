#ifndef _NOTIFICATTION_ICON_HPP_
#define _NOTIFICATTION_ICON_HPP_

#include "ApplicationSettings.hpp"
#include "full_settings_dialog.h"
#include <QSystemTrayIcon>
#include <QMenu>

enum State{OK, FAILED, STARTING};

class NotificationIcon: public QObject
{
Q_OBJECT
private:
	ApplicationSettings *p_settingsData;
    FullSettingsDialog  *p_settingsDialog;
	QSystemTrayIcon     *p_trayIcon; 	
	QMenu               *p_trayMenu;
    QPixmap             *p_iconImage;
    QString             *p_toolTipText;
    State                currentState;
    bool                 alertWasShowed;
    double               curentBalance;
    long int             consumptionTraffic;

    bool showAlarm(double balance);

public:
	NotificationIcon();
signals:
	void new_settings();
private slots:
	void SettingsDialog();
	void about_dialog();
	void ApplicationExit();
public slots:
	void ReloadSettings();
	void ShowBalance(double balance);
	void FailedGetBalance();
	void UpdateTrafficStatistic(double consumptionPerMonth);
};
#endif