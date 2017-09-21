#ifndef TRAIN_P
#define TRAIN_P

#endif // TRAIN_P

#include<opencv2/face/facerec.hpp>
#include<opencv2/core.hpp>
#include<opencv2/face.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

//using namespace cv;
using namespace cv::face;
using namespace std;

static cv::Mat norm_0_255(cv::InputArray _src) {
	cv::Mat src = _src.getMat();
	// �����ͷ���һ����һ�����ͼ�����:
	cv::Mat dst;
	switch (src.channels()) {
	case 1:
		cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC1);
		break;
	case 3:
		cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC3);
		break;
	default:
		src.copyTo(dst);
		break;
	}
	return dst;
}

//ʹ��CSV�ļ�ȥ��ͼ��ͱ�ǩ����Ҫʹ��stringstream��getline����
static void read_csv(const string& filename, vector<cv::Mat>& images, vector<int>& labels, char separator = ';') {
	std::ifstream file(filename.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	string line, path, classlabel;
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, classlabel);
		if (!path.empty() && !classlabel.empty()) {
			images.push_back(cv::imread(path, 0));
			labels.push_back(atoi(classlabel.c_str()));
		}
	}
}


int train()
{

	//��ȡ���CSV�ļ�·��.
	//string fn_csv = string(argv[1]);
	string fn_csv = "at.txt";

	// 2�����������ͼ�����ݺͶ�Ӧ�ı�ǩ
	vector<cv::Mat> images;
	vector<int> labels;
	// ��ȡ����. ����ļ����Ϸ��ͻ����
	// ������ļ����Ѿ�����.
	try
	{
		read_csv(fn_csv, images, labels);
	}
	catch (cv::Exception& e)
	{
		cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
		// �ļ������⣬����ɶҲ�������ˣ��˳���
		exit(1);
	}
	// ���û�ж�ȡ���㹻ͼƬ��Ҳ�˳�.
	if (images.size() <= 1) {
		string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
		CV_Error(CV_StsError, error_message);
	}

	// ����ļ��д�������Ǵ�������ݼ����Ƴ����һ��ͼƬ
	//[gm:��Ȼ������Ҫ�����Լ�����Ҫ�޸ģ���������˺ܶ�����]
	//Mat testSample = images[images.size() - 1];
	//int testLabel = labels[labels.size() - 1];
	images.pop_back();
	labels.pop_back();
	// ���漸�д�����һ��������ģ����������ʶ��
	// ͨ��CSV�ļ���ȡ��ͼ��ͱ�ǩѵ������
	// T������һ��������PCA�任
	//�����ֻ�뱣��10�����ɷ֣�ʹ�����´���
	//      cv::createEigenFaceRecognizer(10);
	//
	// ����㻹ϣ��ʹ�����Ŷ���ֵ����ʼ����ʹ��������䣺
	//      cv::createEigenFaceRecognizer(10, 123.0);
	//
	// �����ʹ��������������ʹ��һ����ֵ��ʹ��������䣺
	//      cv::createEigenFaceRecognizer(0, 123.0);

	cv::Ptr<EigenFaceRecognizer> model = EigenFaceRecognizer::create();
	model->train(images, labels);
	model->save("MyFacePCAModel.xml");

	cv::Ptr<FisherFaceRecognizer> model1 = FisherFaceRecognizer::create();
	model1->train(images, labels);
	model1->save("MyFaceFisherModel.xml");

	cv::Ptr<LBPHFaceRecognizer> model2 = LBPHFaceRecognizer::create();
	model2->train(images, labels);
	model2->save("MyFaceLBPHModel.xml");

	// ����Բ���ͼ�����Ԥ�⣬predictedLabel��Ԥ���ǩ���
	//int predictedLabel = model->predict(testSample);
	//int predictedLabel1 = model1->predict(testSample);
	//int predictedLabel2 = model2->predict(testSample);

	// ����һ�ֵ��÷�ʽ�����Ի�ȡ���ͬʱ�õ���ֵ:
	//      int predictedLabel = -1;
	//      double confidence = 0.0;
	//      model->predict(testSample, predictedLabel, confidence);

	//string result_message = format("Predicted class = %d / Actual class = %d.", predictedLabel, testLabel);
	//string result_message1 = format("Predicted class = %d / Actual class = %d.", predictedLabel1, testLabel);
	//string result_message2 = format("Predicted class = %d / Actual class = %d.", predictedLabel2, testLabel);
	//cout << result_message << endl;
	//cout << result_message1 << endl;
	//cout << result_message2 << endl;

	//getchar();
	//waitKey(0);
	return 0;
}
