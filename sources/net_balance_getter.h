#ifndef _NET_BALANCE_GETTER_H_
#define _NET_BALANCE_GETTER_H_


#include "ApplicationSettings.hpp"
#include <QThread>
#include <QString>
#include <QNetworkRequest>
#include <QNetworkReply>

class NetBalanceGetter : public QThread
{
Q_OBJECT
private:
	int account;
	QString password;
	int wait_time;
	const int timeout_ms;

	QNetworkAccessManager *manager;
	QNetworkReply* post_req();
	bool login_cabinet();
	bool get_cabinet_page();
	bool get_timeleft();
	bool unlogin_cabinet();
	void run();
public:
	NetBalanceGetter();
	~NetBalanceGetter();
signals:
	void new_balance(double);
	void new_timeleft(double);
	void failed();
public slots:
	void set_account(int);
	void set_wait_time(int);
	void set_password(const QString &);
	void reload_settings();
};
#endif