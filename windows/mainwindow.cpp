#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "infodialog.h"
#include "train.h"
#include "addinfo.h"
#include"recordquery.h"
#include <opencv2/opencv.hpp>
#include <direct.h>
#include <QTimer>
#include <QMessageBox>
#include <QInputDialog>

cv::Ptr<EigenFaceRecognizer> modelPCA;

using namespace std;
//using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->label->setScaledContents(true);  //fit video to label area
	infodlg = new InfoDialog;
	connect(infodlg, SIGNAL(sendInfo(QString, QString)), this, SLOT(getInfo(QString, QString)));
	connect(infodlg, SIGNAL(sendsignal()), this, SLOT(showthiswindow()));

}

MainWindow::~MainWindow()
{
	delete ui;
}

QImage  Mat2QImage(cv::Mat cvImg)
{
	QImage qImg;
	if (cvImg.channels() == 3)                             //3 channels color image
	{

		cv::cvtColor(cvImg, cvImg, CV_BGR2RGB);
		qImg = QImage((const unsigned char*)(cvImg.data),
			cvImg.cols, cvImg.rows,
			cvImg.cols*cvImg.channels(),
			QImage::Format_RGB888);
	}
	else if (cvImg.channels() == 1)                    //grayscale image
	{
		qImg = QImage((const unsigned char*)(cvImg.data),
			cvImg.cols, cvImg.rows,
			cvImg.cols*cvImg.channels(),
			QImage::Format_Indexed8);
	}
	else
	{
		qImg = QImage((const unsigned char*)(cvImg.data),
			cvImg.cols, cvImg.rows,
			cvImg.cols*cvImg.channels(),
			QImage::Format_RGB888);
	}

	return qImg;

}

void MainWindow::showthiswindow()
{
	this->show();
	ui->regBtn->setEnabled(true);
	ui->logBtn->setEnabled(true);
}

void MainWindow::nextFrame2()
{
	//TAKE PHOTO


	timer2->stop();
	capture >> frame;
	if (!frame.empty())
	{
		std::vector<cv::Rect> faces;
		cv::Mat frame_gray;
		cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
		cascade.detectMultiScale(frame_gray, faces, 1.1, 4, 0, cv::Size(100, 100), cv::Size(500, 500));

		for (size_t i = 0; i < faces.size(); i++)
		{
			cv::rectangle(frame, faces[i], cv::Scalar(255, 0, 0), 2, 8, 0);
		}

		if (faces.size() == 1)
		{
			cv::Mat faceROI = frame_gray(faces[0]);
			cv::Mat myFace;
			cv::resize(faceROI, myFace, cv::Size(92, 112));
			cv::putText(frame, to_string(pic_num), faces[0].tl(), 3, 1.2, (0, 0, 255), 2, cv::LINE_AA);

			string filename = cv::format("photo\\%d\\pic%d.jpg", atoi(userid.c_str()), pic_num);
			cv::imwrite(filename, myFace);
			//cv::imshow(filename, myFace);
			//cv::waitKey(500);
			//cv::destroyWindow(filename);
			pic_num++;
			
		}
	}
	image = Mat2QImage(frame);
	ui->label->setPixmap(QPixmap::fromImage(image));
	timer2->start();

	if (pic_num == 21)
	{
		timer2->stop();
		capture.release();
		ui->label->setText("registration success!");
		train();
		ui->regBtn->setEnabled(true);
		ui->logBtn->setEnabled(true);
	}
	
}

void MainWindow::getInfo(QString name, QString id)
{
	this->show();
	username = name.toStdString();
	userid = id.toStdString();
	if (capture.isOpened())
		capture.release();     //decide if capture is already opened; if so,close it
	capture.open(0);           //open the default camera

	cascade.load("lbpcascade_frontalface.xml");

	_mkdir("photo");

	pic_num = 1;

	string userdir = "photo\\\\" + userid;  //用户路径

	_mkdir(userdir.c_str());  //创建该用户图片文件夹

	if (capture.isOpened())
	{
		//rate= capture.get(CV_CAP_PROP_FPS);	//using when open a vedio file
		capture >> frame;
		if (!frame.empty())
		{
			cascade.load("lbpcascade_frontalface.xml");
			image = Mat2QImage(frame);
			ui->label->setPixmap(QPixmap::fromImage(image));
			timer2 = new QTimer(this);
			timer2->setInterval(1000 / 25);   //replace "30" by "rate",set timer match with FPS
			connect(timer2, SIGNAL(timeout()), this, SLOT(nextFrame2()));
			timer2->start();
		}
	}



	//以下计划存入数据库，格式：用户id；用户姓名；用户图片路径
	addinfo(userid, username, userdir);

}

void MainWindow::nextFrame()
{
	//识别

	if (cnt == 5)
	{
		timer->stop();
		ui->label->setText(QString::number(predictPCA));
		capture.release();
		stringstream predictPCA_a;
		predictPCA_a << predictPCA;
		cv::String infor = "Name:\t" + find_name(predictPCA_a.str()) + ";\n" + "ID:\t" + predictPCA_a.str() + ".";
		QMessageBox::StandardButton rb = QMessageBox::question(NULL, "Login Information", QString::fromStdString(infor), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		if (rb == QMessageBox::Yes)
		{
			//QMessageBox::aboutQt(NULL, "About Qt");
			face_recoquery(predictPCA_a.str());      //此处需设置异常
			ui->logBtn->setEnabled(true);
			ui->regBtn->setEnabled(true);
		}
		if (rb == QMessageBox::No)
		{
			ui->label->setText("NULL!");
			cnt = 0;
			capture.open(0);
			timer->start();
		}


	}
	capture >> frame;
	if (!frame.empty())
	{
		cv::Mat edges;
		cv::Mat gray;

		//建立用于存放人脸的向量容器
		vector<cv::Rect> faces(0);

		cv::cvtColor(frame, gray, CV_BGR2GRAY);
		//改变图像大小，使用双线性差值
		//resize(gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR);
		//变换后的图像进行直方图均值化处理
		cv::equalizeHist(gray, gray);

		cascade.detectMultiScale(gray, faces,
			1.1, 2, 0
			//|CV_HAAR_FIND_BIGGEST_OBJECT
			//|CV_HAAR_DO_ROUGH_SEARCH
			| CV_HAAR_SCALE_IMAGE,
			cv::Size(30, 30));

		cv::Mat face;
		cv::Point text_lb;

		for (size_t i = 0; i < faces.size(); i++)
		{
			if (faces[i].height > 0 && faces[i].width > 0)
			{
				face = gray(faces[i]);
				text_lb = cv::Point(faces[i].x, faces[i].y);

				cv::rectangle(frame, faces[i], cv::Scalar(255, 0, 0), 1, 8, 0);
			}
		}

		cv::Mat face_test;

		predictPCA = 0;
		if (face.rows >= 120)
		{
			cv::resize(face, face_test, cv::Size(92, 112));

		}
		//Mat face_test_gray;
		//cvtColor(face_test, face_test_gray, CV_BGR2GRAY);

		if (!face_test.empty())
		{
			//测试图像应该是灰度图
			predictPCA = modelPCA->predict(face_test);
			if (predictPCA == result && predictPCA) ++cnt;
			else
			{
				result = predictPCA;
				cnt = 1;
			}
		}

		cv::putText(frame, to_string(predictPCA), text_lb, cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(0, 0, 255));
		
		image = Mat2QImage(frame);
		ui->label->setPixmap(QPixmap::fromImage(image));


	}



}

void MainWindow::on_regBtn_clicked()
{
	ui->regBtn->setEnabled(false);
	ui->logBtn->setEnabled(false);
	infodlg->show();
	this->hide();

}

void MainWindow::on_logBtn_clicked()
{
	ui->logBtn->setEnabled(false);
	ui->regBtn->setEnabled(false);
	if (capture.isOpened())
		capture.release();     //decide if capture is already opened; if so,close it
	capture.open(0);           //open the default camera


	result = -1;
	cnt = 0;

	if (capture.isOpened())
	{
		//rate= capture.get(CV_CAP_PROP_FPS);	//using when open a vedio file
		capture >> frame;
		if (!frame.empty())
		{
			cascade.load("lbpcascade_frontalface.xml");
			//cv::Ptr<EigenFaceRecognizer> modelPCA;
			modelPCA = cv::Algorithm::load<cv::face::EigenFaceRecognizer>("MyFacePCAModel.xml");
			image = Mat2QImage(frame);
			ui->label->setPixmap(QPixmap::fromImage(image));
			timer = new QTimer(this);
			timer->setInterval(1000 / 25);   //replace "30" by "rate",set timer match with FPS
			connect(timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
			timer->start();
		}
	}


	

	
}
