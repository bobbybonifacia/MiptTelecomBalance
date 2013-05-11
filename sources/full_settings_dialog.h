#ifndef _FULL_SETTINGS_DIALOG_H_
#define _FULL_SETTINGS_DIALOG_H_

#include <QIntValidator>
#include <QPushButton>
#include <QGridLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QSlider>
#include <QDialog>
#include <QWidget>
#include <QLabel>

class FullSettingsDialog : public QDialog
{
Q_OBJECT
private:
	QLabel*			p_label_number;
	QLabel*			p_label_password;
	QLineEdit*		p_number;
	QLineEdit*		p_password;
	
	QCheckBox*		p_use_alerts;
	QLineEdit*		p_alert_lim;
	

	QLabel*			p_lbl_uptime;
	QSlider*		p_uptime;

	QPushButton*	p_cancel;
	QPushButton*	p_exit;
	QPushButton*	p_ok;

	QGridLayout*	p_layout;
	QIntValidator*	p_val;
public:
	FullSettingsDialog();
	~FullSettingsDialog();
signals:
	void new_settings();
private slots:
	void alert_lock(int code);
	void lbl_uptime(int timeup);
	void dialog_accept();
};
#endif;