#ifndef _NOTIFICATTION_ICON_H_
#define _NOTIFICATTION_ICON_H_

#include <QSystemTrayIcon>
#include <QMenu>

class NotificationIcon: public QObject{
Q_OBJECT
private:
	bool few_money; 
	bool use_alerts;
	int few_money_boundary;
	
	QSystemTrayIcon*	p_icon; 	
	QMenu*				p_menu;


public:
	NotificationIcon();
signals:
	void new_settings();
public slots:
	void reload_settings();
	void change_info(double new_balance);
	void connection_issues();
	void settings_dialog();
	void about_dialog();
	void app_exit();
};
#endif