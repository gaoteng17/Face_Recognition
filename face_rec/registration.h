#ifndef TAKE_PHOTO_H
#define TAKE_PHOTO_H

#endif // TAKE_PHOTO_H

#include<opencv2/opencv.hpp>
#include <direct.h>
#include <mysql.h>
#include "addinfo.h"


using namespace std;
//using namespace cv;

int  registration()
{
	cv::CascadeClassifier cascade;
	cascade.load("lbpcascade_frontalface.xml");
	cv::VideoCapture cap;
	cap.open(0);
	cv::Mat frame;
	int pic_num = 1;

	_mkdir("photo");  //创建photo文件夹
	
	string username, userid, userdir;
	cout << "please write down your name:" << endl;
	cin >> username; //用户姓名
	cout << "please write down your id:" << endl;
	cin >> userid; //用户id

	userdir = "photo\\\\" + userid;  //用户路径

	if (!addinfo(userid, username, userdir)) return 0;
	else {_mkdir(userdir.c_str()); };//以下计划存入数据库，格式：用户id；用户姓名；用户图片路径
	ofstream DBfile("DataBase.txt", ios::app);
	if (!DBfile) {
		cout << "Open the file failure...\n";
		exit(0);
	}
	DBfile << userid << ";" << username << ";" << userdir << ";" << endl;
	DBfile.close();

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

	while (1)
	{
		cap >> frame;

		std::vector<Rect> faces;
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
		cv::imshow("frame", frame);
		cv::waitKey(100);
	}

	cv::destroyWindow("frame");

	//cout << " registration success！" << endl;

	return 0;
}
