#include "notification_icon.h"

#include "ApplicationSettings.hpp"
#include "full_settings_dialog.h"
#include <QSystemTrayIcon>
#include <QApplication>
#include <QMessageBox>
#include <QString>
#include <QMenu>
#include <QTime>

const QString AttentionPath = "C:\\attention.png";
const QString PositivePath  = "C:\\good.png";
const QString NegativePath  = "C:\\bad.png";
const QString ProblemPath   = "C:\\unknown.png";

NotificationIcon::NotificationIcon()
{
    currentState = STARTING;
	
    consumptionTraffic = 0;
    alertWasShowed = false;

    p_settingsData = new ApplicationSettings();
    p_settingsData ->restoreSaved();

    p_settingsDialog = new FullSettingsDialog();
    QObject::connect(p_settingsDialog, SIGNAL(new_settings()), this, SIGNAL(new_settings()));

	QAction *showSettings	= new QAction("Settings", this);
    QAction *showAuthors	= new QAction("About"   , this);
	QAction *appExit		= new QAction("Exit"    , this);
	
	p_trayMenu = new QMenu();
	p_trayMenu ->addAction(showSettings);
	p_trayMenu ->addAction(showAuthors);
	p_trayMenu ->addAction(appExit);

	QObject::connect(showSettings, SIGNAL(triggered()), this, SLOT(SettingsDialog()));
	QObject::connect(showAuthors , SIGNAL(triggered()), this, SLOT(about_dialog())   );
	QObject::connect(appExit     , SIGNAL(triggered()), this, SLOT(ApplicationExit()));

    p_iconImage = new QPixmap;
	p_iconImage->load(AttentionPath);

    p_toolTipText  = new QString("Starting...");
    
    p_trayIcon = new QSystemTrayIcon(this);
	p_trayIcon->setContextMenu(p_trayMenu);
    p_trayIcon->setIcon( *p_iconImage );
    p_trayIcon->setToolTip( *p_toolTipText );
    p_trayIcon->show();
}

void NotificationIcon::FailedGetBalance() {
    currentState = FAILED;

    *p_toolTipText =  "Failed to get the current balance.\
                       \nPlease, check the network connection\
                       \nand check your account settings.";

    p_iconImage->load(ProblemPath);
	p_trayIcon->setToolTip( *p_toolTipText );
    p_trayIcon->setIcon( *p_iconImage );
}

bool NotificationIcon::showAlarm(double balance) {

    if(p_settingsData->useLimitAlert == false) return false;

    int limit = p_settingsData->boundaryLimitAlert;

    if( (alertWasShowed == true) && (balance >= limit)) {
	    alertWasShowed = false;
        return false;
    }

    if( (alertWasShowed == false) && (balance < limit)){
        alertWasShowed = true;
        QString title = "Balance is less than ";
		title.append(QString::number(limit));
		title.append(" rubles!");
        p_trayIcon->showMessage(title,"Please top up your balance, which is currently low",QSystemTrayIcon::Warning);
        return true;
    }
    return false;
}

void NotificationIcon::ShowBalance(double balance) {
    currentState = OK;

	if ( balance > 0 )
        p_iconImage->load(PositivePath);
	else
		p_iconImage->load(NegativePath);
    p_trayIcon->setIcon( *p_iconImage );

    showAlarm(balance);

    QString message = "MiptTelecomBalance v0.3\n";
    message.append("Balance: ");
	message.append(QString::number(balance, 'f', 2));
    if (consumptionTraffic > 0) {
        message.append("\nconsumptionMonthTraffic = ");
        message.append(QString::number(consumptionTraffic, 'f', 2));
    }
	p_trayIcon->setToolTip(message);
}

void NotificationIcon::UpdateTrafficStatistic(double consumptionPerMonth){
    consumptionTraffic = consumptionPerMonth;
}

void NotificationIcon::about_dialog(){
    QDialog aboutDialog(0, Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
    QLabel l("<h1>MiptTelecomBalance</h1>\
            \nThis program allows you to monitor\n\
            the balance of your personal account\n\
            in \"MIPT-TELECOM\"\n\
            Authors:");
    l.setWordWrap(true);
    QLabel l1, l2, l3;
    l1.setText("<h3>Lantsov Alexander</h3>");
    l2.setText("<h3>Kudinova Marina</h3>");
    l3.setText("<h3>Alekseeva Maria</h3>");
    
    QString img[4] = { AttentionPath, PositivePath, NegativePath, ProblemPath };
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    
    QLabel lp1, lp2, lp3;
    QPixmap p1(img[qrand() % 4]);
    QPixmap p2(img[qrand() % 4]);
    QPixmap p3(img[qrand() % 4]);
    lp1.setPixmap(p1);
    lp2.setPixmap(p2);
    lp3.setPixmap(p3);
    
    lp1.setFixedSize(p1.width(),p1.height());
    lp2.setFixedSize(p2.width(),p2.height());    
    lp3.setFixedSize(p3.width(),p3.height());
    



    QPushButton button("OK");

    QGridLayout layout;
    layout.addWidget(&l  , 0, 0, 1, 2);
    layout.addWidget(&lp1, 1, 0);
    layout.addWidget(&l1 , 1, 1);
    layout.addWidget(&lp2, 2, 0);
    layout.addWidget(&l2 , 2, 1);
    layout.addWidget(&lp3, 3, 0);
    layout.addWidget(&l3 , 3, 1);
    layout.addWidget(&button, 4, 0, 1, 2);
    QObject::connect(&button, SIGNAL(clicked()), &aboutDialog, SLOT(accept()));
    aboutDialog.setLayout(&layout);
	aboutDialog.setWindowTitle("About");
    //aboutDialog.setFixedSize( aboutDialog.sizeHint() );
    layout.setSizeConstraint( QLayout::SetFixedSize );
    aboutDialog.show();
    aboutDialog.exec();
}


void NotificationIcon::SettingsDialog() {
    if(!p_settingsDialog->isVisible()) {
        p_settingsDialog->show();
	    p_settingsDialog->exec();
    } else {
        qApp->setActiveWindow(p_settingsDialog);
    }
}

void NotificationIcon::ApplicationExit() {
	p_trayIcon->hide();
	qApp->quit();
}

void NotificationIcon::ReloadSettings() {
	p_settingsData->restoreSaved();
	consumptionTraffic = 0;
    alertWasShowed = false;
}

