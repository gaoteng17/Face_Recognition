#ifndef FACE_RECOGNISE_H
#define FACE_RECOGNISE_H

#endif // FACE_RECOGNISE_H

#include<opencv2/opencv.hpp>
#include<opencv2/face.hpp>
#include<iostream>
#include "addinfo.h"
#include"recordquery.h"

using namespace std;
//using namespace cv;
using namespace cv::face;

int face_recognise()
{
	cv::VideoCapture cap(0);    //打开默认摄像头
	if (!cap.isOpened())
	{
		return -1;
	}
	cv::Mat frame;
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
		cap >> frame;

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

				cv::rectangle(frame, faces[i], Scalar(255, 0, 0), 1, 8, 0);
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

		cout << predictPCA << endl;

		cv::putText(frame, to_string(predictPCA), text_lb, cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(0, 0, 255));

		cv::imshow("face", frame);
		cv::waitKey(200);

		if (cnt == 5 && predictPCA)
		{
			cout << "predict result:" << predictPCA << endl;
			//predictPCA为识别出来的学号
			stringstream predictPCA_a;
			predictPCA_a << predictPCA;
			string predictPCA_b = predictPCA_a.str();
			if (!face_recoquery(predictPCA_b))  return 0 ;
			//face_recoqery() 函数以学号为索引用来显示识别成功后的学生信息，并在recognise_record表中新建一行数据
			break;
		}
	}

	return 0;
}
