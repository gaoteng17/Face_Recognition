#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include "infodialog.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_regBtn_clicked();
    void getInfo(QString,QString);
    void nextFrame();
	void nextFrame2();
    void on_logBtn_clicked();
	void showthiswindow();

private:
    Ui::MainWindow *ui;
    cv::Mat frame;
    cv::VideoCapture capture;
    QImage  image;
    QTimer *timer;
	QTimer *timer2;
    cv::String username;
    cv::String userid;
    InfoDialog *infodlg;
	int result, cnt, predictPCA,pic_num;
	cv::CascadeClassifier cascade;



};




#endif // MAINWINDOW_H
