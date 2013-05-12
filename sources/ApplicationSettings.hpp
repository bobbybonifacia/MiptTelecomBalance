#ifndef _APPLICATION_SETTINGS_HPP_
#define _APPLICATION_SETTINGS_HPP_

#include <QSettings>
#include <QString>

class ApplicationSettings
{
public:
    int     userId;
    QString userPassword;

    bool useLimitAlert;
    int  boundaryLimitAlert;

    int  balanceUpdatePeriod;

    int  costPerGigabyte;

    ApplicationSettings();
    void restoreSaved();
    void saveActual();
    bool userInfoExist();

};

#endif