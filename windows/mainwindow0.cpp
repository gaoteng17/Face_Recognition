#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "infodialog.h"
#include "train.h"
#include "addinfo.h"
#include"recordquery.h"
#include<opencv2/opencv.hpp>
#include <direct.h>
#include <QTimer>
#include <QMessageBox>
#include <QInputDialog>


using namespace std;
//using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->setScaledContents(true);  //fit video to label area
    infodlg = new InfoDialog;
    connect(infodlg, SIGNAL(sendInfo(QString,QString)), this, SLOT(getInfo(QString,QString)));
    connect(infodlg, SIGNAL(sendsignal()), this, SLOT(show()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

QImage  Mat2QImage(cv::Mat cvImg)
{
    QImage qImg;
    if(cvImg.channels()==3)                             //3 channels color image
    {

        cv::cvtColor(cvImg,cvImg,CV_BGR2RGB);
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols, cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_RGB888);
    }
    else if(cvImg.channels()==1)                    //grayscale image
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols,cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_Indexed8);
    }
    else
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols,cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_RGB888);
    }

    return qImg;

}



void MainWindow::getInfo(QString name,QString id)
{
    this->show();
    username = name.toStdString();
    userid = id.toStdString();
    if (capture.isOpened())
        capture.release();     //decide if capture is already opened; if so,close it
    capture.open(0);           //open the default camera

    cv::CascadeClassifier cascade;
    cascade.load("lbpcascade_frontalface.xml");

	_mkdir("photo");

    int pic_num = 1;

    string userdir = "photo\\\\" + userid;  //用户路径

    _mkdir(userdir.c_str());  //创建该用户图片文件夹

    while (1)
    {
        capture >> frame;
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
            cv::imshow(filename, myFace);
            cv::waitKey(500);
            cv::destroyWindow(filename);
            pic_num++;
            if (pic_num == 21) break;
        }
		image = Mat2QImage(frame);
		ui->label->setPixmap(QPixmap::fromImage(image));
        
    }

	//以下计划存入数据库，格式：用户id；用户姓名；用户图片路径
	addinfo(userid, username, userdir);

	//以下为csv标签文件，格式：路径；标签；
	ofstream tagfile("at.txt", ios::app);
	if (!tagfile) {
		cout << "Open the file failure...\n";
		exit(0);
	}
	for (int i = 1;i <= 20;i++)
	{
		tagfile << userdir << "\\pic" << i << ".jpg" << ";" << userid << ";" << endl;
	}
	tagfile.close();

    ui->label->setText("registration success!");
	capture.release();
    train();
}

void MainWindow::nextFrame()
{
    capture >> frame;
    if (!frame.empty())
    {
        image = Mat2QImage(frame);
        ui->label->setPixmap(QPixmap::fromImage(image));
        //this->update();
    }

}


void MainWindow::on_regBtn_clicked()
{
    infodlg->show();
    this->hide();

}

void MainWindow::on_logBtn_clicked()
{
    capture.open(0);    //打开默认摄像头

    cv::Mat edges;
    cv::Mat gray;

    cv::CascadeClassifier cascade;
    bool stop = false;
    //训练好的文件名称，放置在可执行文件同目录下
    cascade.load("lbpcascade_frontalface.xml");

    cv::Ptr<cv::face::EigenFaceRecognizer> modelPCA = cv::Algorithm::load<cv::face::EigenFaceRecognizer>("MyFacePCAModel.xml");

    int result = -1, cnt = 0;

    while (1)
    {
        capture >> frame;

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

        int predictPCA = 0;
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
        cv::imshow("faces", frame);
        cv::waitKey(500);
        cv::destroyWindow("faces");
        image = Mat2QImage(frame);
        ui->label->setPixmap(QPixmap::fromImage(image));
        cv::waitKey(200);

        if (cnt == 5 && predictPCA)
        {
            ui->label->setText(QString::number(predictPCA));
			capture.release();
			stringstream predictPCA_a;
			predictPCA_a << predictPCA;

			face_recoquery(predictPCA_a.str());      //此处需设置异常

            break;
        }
    }
}
