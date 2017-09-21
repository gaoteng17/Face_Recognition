#include <QApplication>
#include "mainwindow.h"
#include "logindialog.h"
#include <opencv2/opencv.hpp>
#include<opencv2/core.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    LoginDialog dlg;
    if (dlg.exec() == QDialog::Accepted)
    {
       w.show();
       return a.exec();
    }
    else return 0;
}
