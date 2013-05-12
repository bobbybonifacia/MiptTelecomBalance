#include "ApplicationSettings.h"

#include <QCoreApplication>
#include <QSettings>
#include <QString>
#include <limits> 

const int     defaultId            = std::numeric_limits<int>::min();
const QString defaultPassword      = QString();
const bool    defaultAlert         = true;
const int     defaultAlertBoundary = 10;
const int     defaultUpdatePeriod  = 5;
const int     defaulCostPerGb      = 25;


ApplicationSettings::ApplicationSettings(){
	userId              = defaultId;
	userPassword        = defaultPassword;

	useLimitAlert       = defaultAlert;
	boundaryLimitAlert  = defaultAlertBoundary;
	
    balanceUpdatePeriod = defaultUpdatePeriod;
	
    costPerGigabyte     = defaulCostPerGb;
}

void ApplicationSettings::restoreSaved(){
	QCoreApplication::setOrganizationName("Mipt");
	QCoreApplication::setApplicationName("TelecomBalance");
	
	QSettings sys;
	userId              = sys.value("/USER_ID"       , defaultId).toInt();
	userPassword        = sys.value("/USER_PASSWORD" , defaultPassword).toString();
	useLimitAlert       = sys.value("/USE_ALERT"     , defaultAlert).toBool();
	boundaryLimitAlert  = sys.value("/ALERT_BOUNDARY", defaultAlertBoundary).toInt();
	balanceUpdatePeriod = sys.value("/UPDATE_PERIOD" , defaultUpdatePeriod).toInt();
	costPerGigabyte     = sys.value("/COST_PER_UNIT" , defaulCostPerGb).toInt();
}


bool ApplicationSettings::userInfoExist(){
	if ((userId < 0) ||( userPassword==defaultPassword)) return false; else
		return true;
	
}

void ApplicationSettings::saveActual(){
	QCoreApplication::setOrganizationName("Mipt");
	QCoreApplication::setApplicationName("TelecomBalance");
	
	QSettings sys;
	sys.setValue("/USER_ID"       , userId);
	sys.setValue("/USER_PASSWORD" , userPassword);
	sys.setValue("/USE_ALERT"     , useLimitAlert);
	sys.setValue("/ALERT_BOUNDARY", boundaryLimitAlert);
	sys.setValue("/UPDATE_PERIOD" , balanceUpdatePeriod);
	sys.setValue("/COST_PER_UNIT" , costPerGigabyte);
}
