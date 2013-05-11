#ifndef _ACCOUNT_SETTINGS_DIALOG_H_
#define _ACCOUNT_SETTINGS_DIALOG_H_

#include <QIntValidator>
#include <QPushButton>
#include <QGridLayout>
#include <QSettings>
#include <QLineEdit>
#include <QDialog>
#include <QWidget>
#include <QLabel>

class AccountDialog : public QDialog
{
Q_OBJECT
private:
	QLabel*			p_label_number;
	QLabel*			p_label_password;
	QLineEdit*		p_number;
	QIntValidator*	p_val;
	QLineEdit*		p_password;
	QPushButton*	p_exit;
	QPushButton*	p_ok;
	QGridLayout*	p_layout;
public:
	int		acc_num;
	QString acc_pass;
	AccountDialog();
	~AccountDialog();
	void get_info_or_exit();
private slots:
	void set_info();
	void ok_acc();
};
#endif