#include "net_balance_getter.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>
#include <QUrl>
#include <Windows.h>

NetBalanceGetter::NetBalanceGetter(): QThread(), timeout_ms(5000){
	ApplicationSettings mngr;
	mngr.restoreSaved();
	account =  mngr.userId;
	password = mngr.userPassword;
	wait_time = mngr.balanceUpdatePeriod;
	manager = NULL;
}
NetBalanceGetter::~NetBalanceGetter(){
}

void NetBalanceGetter::run(){
	QNetworkAccessManager mng;
	manager = &mng;
	while(true){
		QThread::sleep(wait_time);
		if(!login_cabinet())	continue;
		if(!get_cabinet_page())	continue;
		if(!get_timeleft())	continue;
		if(!unlogin_cabinet())	continue;
	}
}

QNetworkReply* NetBalanceGetter::post_req(){
	QUrl url = QUrl(QString("http://cabinet.telecom.mipt.ru/"));
	
	QUrl data;
	data.addQueryItem("login", QString::number(account));
	data.addQueryItem("password",password);

	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	return manager->post(request, data.encodedQuery());
}

bool NetBalanceGetter::login_cabinet(){
	QTimer timeout;
	timeout.setSingleShot(true);
	
	QEventLoop waitLoop;
	
	QObject::connect(&timeout, SIGNAL(timeout()), &waitLoop, SLOT(quit()) );
	QNetworkReply* reply;


	timeout.start(timeout_ms);
	reply = post_req();
	QObject::connect(reply, SIGNAL(finished()), &waitLoop, SLOT(quit()));
	waitLoop.exec();
	
	if( timeout.isActive() && (reply->error() == QNetworkReply::NoError) ) {
		timeout.stop();
		return true;
	}else
	{
		emit failed();
		return false;
	}
}
bool NetBalanceGetter::get_cabinet_page(){
	QNetworkReply* reply;

	QTimer timeout;
	QEventLoop waitLoop;
	QObject::connect(&timeout, SIGNAL(timeout()), &waitLoop, SLOT(quit()) );

	timeout.setSingleShot(true);
	timeout.start(timeout_ms);
	reply = manager->get(QNetworkRequest(QUrl("http://cabinet.telecom.mipt.ru/")));
	QObject::connect(reply, SIGNAL(finished()), &waitLoop, SLOT(quit()));
	waitLoop.exec();
	
	if( timeout.isActive() && (reply->error() == QNetworkReply::NoError) ) {
		timeout.stop();

		QString page_body = reply->readAll();
		const QString separator = "&nbsp;&mdash;";
		QString balance;
		
		int index = 0;
		int pos = page_body.indexOf(separator) + QString(separator).length();
		if(pos < 0){
			emit failed();
			return false;
		};

		do{
			if( (pos + index) >= page_body.size()){
				emit failed();
				return false;
			}
			if( QString("&nbsp;").indexOf(page_body[pos+index]) < 0 )
				balance.append(page_body[pos+index]);
			index++;
		}while(page_body[pos+index] != '.');
		balance.append(page_body[pos+index]);
		balance.append(page_body[pos+index+1]);
		balance.append(page_body[pos+index+2]);
		bool ok = false;
		balance.toDouble(&ok);		
		if(!ok){
			emit failed();
			return false;
		}
		emit new_balance(balance.toDouble());
		return true;
	}else
	{
		emit failed();
		return false;
	}
}




bool NetBalanceGetter::get_timeleft(){
	QNetworkReply* reply;

	QTimer timeout;
	QEventLoop waitLoop;
	QObject::connect(&timeout, SIGNAL(timeout()), &waitLoop, SLOT(quit()) );

	timeout.setSingleShot(true);
	timeout.start(timeout_ms);
	reply = manager->get(QNetworkRequest(QUrl("http://cabinet.telecom.mipt.ru/stats/?from=11.05.2012&to=11.05.2013")));
	QObject::connect(reply, SIGNAL(finished()), &waitLoop, SLOT(quit()));
	waitLoop.exec();
	
	if( timeout.isActive() && (reply->error() == QNetworkReply::NoError) ) {
		timeout.stop();

		QString page_body = reply->readAll();
		const QString separator = "<i>";
		QString stat;
		
		int index = 0;
		int pos = page_body.indexOf(separator) + QString(separator).length();
		pos = page_body.indexOf(separator,pos) + QString(separator).length();
		if(pos < 0){
			//emit failed();
			MessageBox(NULL,NULL,NULL,2);
			return false;
		};

		do{
			if( (pos + index) >= page_body.size()){
				emit failed();
				return false;
			}
			stat.append(page_body[pos+index]);
			index++;
		}while(page_body[pos+index] != '<');
		/*stat.append(".");
		stat.append(page_body[pos+index+1]);
		stat.append(page_body[pos+index+2]);
		stat.append(page_body[pos+index+2]);*/
		bool ok = false;
		stat.toDouble(&ok);		
		if(!ok){
			emit failed();
			MessageBox(NULL,stat.toStdWString().data(),NULL,2);
			return false;
		}
		emit new_timeleft(stat.toDouble());
		return true;
	}else
	{
		//emit failed();
		return false;
	}
}














bool NetBalanceGetter::unlogin_cabinet(){
	
	QTimer timeout;
	QEventLoop waitLoop;
	QObject::connect(&timeout, SIGNAL(timeout()), &waitLoop, SLOT(quit()) );
	
	timeout.setSingleShot(true);
	timeout.start(timeout_ms);
	
	QNetworkReply* reply;
	reply = manager->get(QNetworkRequest(QUrl("http://cabinet.telecom.mipt.ru/exit/")));
	QObject::connect(reply, SIGNAL(finished()), &waitLoop, SLOT(quit()));

	waitLoop.exec();
	
	if( timeout.isActive() &&(reply->error() == QNetworkReply::NoError)) {
		timeout.stop();
		return true;
	}else
	{
		emit failed();
		return false;
	}
}

void NetBalanceGetter::set_password(const QString &new_pass){
	password = new_pass;
}
void NetBalanceGetter::set_wait_time(int new_time){
	wait_time = new_time;
}
void NetBalanceGetter::set_account(int new_acc){
	account = new_acc;
}



void NetBalanceGetter::reload_settings(){
	ApplicationSettings mngr;
	mngr.restoreSaved();
	account =  mngr.userId;
	password = mngr.userPassword;
	wait_time = mngr.balanceUpdatePeriod;
}