#include <winsock.h>
#include <iostream>  
#include <string> 
#include <mysql.h> 
#include"addinfo.h"

using namespace std;

#pragma comment(lib, "ws2_32.lib")  
#pragma comment(lib, "libmysql.lib")  

//单步执行，不想单步执行就注释掉  
#define STEPBYSTEP  

//recordquery 用来以学号查询考勤记录
bool recordquery() {

	//必备的一个数据结构  
	MYSQL mydata;

	//初始化数据库  
	if (mysql_library_init(0, NULL, NULL)) {
		cout << "mysql_library_init() failed" << endl;
		return false;
	}

	//初始化数据结构  
	if (NULL == mysql_init(&mydata)) {
		cout << "mysql_init() failed" << endl;
		//system("pause");
		return false;
	}

	//在连接数据库之前，设置额外的连接选项  
	//可以设置的选项很多，这里设置字符集，否则无法处理中文  
	if (mysql_options(&mydata, MYSQL_SET_CHARSET_NAME, "gbk")) {
		cout << "mysql_options() failed" << endl;
		return false;
	}



	//连接数据库  
	if (NULL == mysql_real_connect(&mydata, "localhost", "root", "950313", "database1", 3306, NULL, 0)) {
		//这里的地址，用户名，密码，端口可以根据自己本地的情况更改  
		cout << "mysql_real_connect() failed" << endl;
		return false;
	}


	//mysql查询
	string queryselect;
	string studentID;
	cout << "输入学号" << "\n";
	cin >> studentID;
	//创建一个给mysql_query使用的字符串,按学号查询并按时间排序
	queryselect = "select * from recognise_record where studentID=";
	queryselect += studentID;
	if (mysql_query(&mydata, queryselect.c_str())) {
		cout << "查询失败"<<endl;
		return false;

	}
	std::cout << "查询完毕" << std::endl;
	MYSQL_RES *result;
	//取得数据库结果集
	result = mysql_store_result(&mydata);
	if (result) {
		int row_num, col_num;
		row_num = mysql_num_rows(result);
		col_num = mysql_num_fields(result);	
		if (row_num ==0)
		{
			cout << "查无此人"<<"\n";
			system("pause");
			return false;
		}
		cout << "共有" << row_num << "数据以下为其详细内容" << endl;
		MYSQL_FIELD *fd;
		while (fd = mysql_fetch_field(result)) {
			std::cout << fd->name << "\t";

		}
		std::cout << std::endl;
		MYSQL_ROW sql_row;
		while (sql_row = mysql_fetch_row(result)) {
			for (int i = 0; i < col_num; i++) {
				if (sql_row[i] == NULL) std::cout << "NULL\t";
				else std::cout << sql_row[i] << "\t";

			}
			std::cout << std::endl;

		}

	}
	if (result != NULL)
		mysql_free_result(result);
	mysql_close(&mydata);
	mysql_server_end();
	return true;
}
//face_recoqery() 函数用来显示识别成功后的学生信息，并在recognise_record（考勤记录表）中新建一行数据
bool face_recoquery(string student_id) {

	//必备的一个数据结构  
	MYSQL mydata;

	//初始化数据库  
	if (mysql_library_init(0, NULL, NULL)) {
		cout << "mysql_library_init() failed" << endl;
		return false;
	}

	//初始化数据结构  
	if (NULL == mysql_init(&mydata)) {
		cout << "mysql_init() failed" << endl;
		//system("pause");
		return false;
	}

	//在连接数据库之前，设置额外的连接选项  
	//可以设置的选项很多，这里设置字符集，否则无法处理中文  
	if (mysql_options(&mydata, MYSQL_SET_CHARSET_NAME, "gbk")) {
		cout << "mysql_options() failed" << endl;
		return false;
	}



	//连接数据库  
	if (NULL == mysql_real_connect(&mydata, "localhost", "root", "950313", "database1", 3306, NULL, 0)) {
		//这里的地址，用户名，密码，端口可以根据自己本地的情况更改  
		cout << "mysql_real_connect() failed" << endl;
		return false;
	}


	string queryselect;
	queryselect = "select * from information where student_id=";
	queryselect += student_id;
	if (mysql_query(&mydata, queryselect.c_str())) {
		cout << "查询失败";
		return false;

	}
	std::cout << "考勤完毕" << std::endl;
	MYSQL_RES *result;
	result = mysql_store_result(&mydata);
	if (result) {
		int row_num, col_num;
		row_num = mysql_num_rows(result);
		col_num = mysql_num_fields(result);
		std::cout << "您的个人信息如下" << std::endl;
		MYSQL_FIELD *fd;
		while (fd = mysql_fetch_field(result)) {
			std::cout << fd->name << "\t";

		}
		std::cout << std::endl;
		MYSQL_ROW sql_row;
		while (sql_row = mysql_fetch_row(result)) {
			for (int i = 0; i < 2; i++) {
				if (sql_row[i] == NULL) std::cout << "NULL\t";
				else std::cout << sql_row[i] << "\t";
			}
			std::cout << std::endl;
		}
	}
	if (result != NULL)
		mysql_free_result(result);
	string sqlstr;
	string result_a;
	sqlstr = "select stuName from information where student_id=";//通过收到的学号找到相应的名字
	sqlstr += student_id;
	result = NULL;
	if (0 == mysql_query(&mydata, sqlstr.c_str())) {
		cout << "学号对应姓名已经找到" << endl;

		//一次性取得数据集  
		result = mysql_store_result(&mydata);
		//取得并打印行数  
		int rowcount = mysql_num_rows(result);
		cout << "row count: " << rowcount << endl;

		//取得并打印各字段的名称  
		unsigned int fieldcount = mysql_num_fields(result);
		MYSQL_FIELD *field = NULL;
		for (unsigned int i = 0; i < fieldcount; i++) {
			field = mysql_fetch_field_direct(result, i);
			cout << field->name << "\t\t";
		}
		cout << endl;

		//打印各行  
		MYSQL_ROW row = NULL;
		row = mysql_fetch_row(result);
		while (NULL != row) {
			for (int i = 0; i < fieldcount; i++) {
				result_a = row[0];// 将相应名字以字符串形式赋给result_a 
				cout << result_a << "\t\t";
			}
			cout << endl;
			row = mysql_fetch_row(result);
		}

	}

	else {
		cout << "学号对应姓名寻找失败" << endl;
		mysql_close(&mydata);
		return -1;
	}
#ifdef STEPBYSTEP  
	system("pause");
#endif  
	mysql_close(&mydata);
	mysql_server_end();
	cout << student_id, "\n", result_a;
	if (!addrecord(student_id, result_a)) return 0;
	return true;
}
