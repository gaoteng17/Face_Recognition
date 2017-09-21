#ifndef INFODIALOG_H
#define INFODIALOG_H

#include <QDialog>

namespace Ui {
class InfoDialog;
}

class InfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InfoDialog(QWidget *parent = 0);
    ~InfoDialog();
    QString name,id;

private slots:
    void on_loginBtn_clicked();
    void on_exitBtn_clicked();

signals:
    void sendInfo(QString,QString);
    void sendsignal();

private:
    Ui::InfoDialog *ui;
};

#endif // INFODIALOG_H
