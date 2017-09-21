#include<opencv2/opencv.hpp>
#include<stdio.h>
#include <mysql.h> 
#include <winsock.h>
#include <iostream> 
#include"registration.h"
#include"train.h"
#include"face_recognise.h"
#include"testsql.h"
#include"addinfo.h"
#include"recordquery.h"

using namespace std;
using namespace cv;

int main()
{
        testsql();
	int key = 0;
	while (1)
	{
                printf("选择choose:\n\t1. registration\n\t2. training\n\t3. recognization\n\t4. recordquery\n\t5. quit\n");
		scanf_s("%d", &key);
		switch (key)
		{
		case 1:
			registration();
			break;

		case 2:
			train();
			break;

		case 3:
			face_recognise();
			break;

		case 4:
			recordquery();
			break;
		case 5:
			exit(0);
			break;

		default:
			break;
		}
	}

	return 0;
}
