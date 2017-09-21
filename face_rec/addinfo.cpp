#include <winsock.h>
#include <iostream>  
#include <string>  
#include <mysql.h> 
using namespace std;

#pragma comment(lib, "ws2_32.lib")  
#pragma comment(lib, "libmysql.lib")  

//单步执行，不想单步执行就注释掉  
#define STEPBYSTEP  


//addinfo 函数用来向表information（个人信息表）中添加数据
bool addinfo(string userid, string username, string userdir) {
	cout << "****************************************" << endl;

 

	//必备的一个数据结构  
	MYSQL mydata;

	//初始化数据库  
	if (0 == mysql_library_init(0, NULL, NULL)) {
		cout << "mysql_library_init() succeed" << endl;
	}
	else {
		cout << "mysql_library_init() failed" << endl;
		system("pause");
		return false;
	}



	//初始化数据结构  
	if (NULL != mysql_init(&mydata)) {
		cout << "mysql_init() succeed" << endl;
	}
	else {
		cout << "mysql_init() failed" << endl;
		system("pause");
		return false;
	}

	//在连接数据库之前，设置额外的连接选项  
	//可以设置的选项很多，这里设置字符集，否则无法处理中文  
	if (0 == mysql_options(&mydata, MYSQL_SET_CHARSET_NAME, "gbk")) {
		cout << "mysql_options() succeed" << endl;
	}
	else {
		cout << "mysql_options() failed" << endl;
		system("pause");
		return false;
	}


	//连接数据库  
	if (NULL
		!= mysql_real_connect(&mydata, "localhost", "root", "1995", "face_recogition", 3306, NULL, 0))
		//这里的地址，用户名，密码，端口可以根据自己本地的情况更改  
	{
		cout << "mysql_real_connect() succeed" << endl;
	}
	else {
		cout << "mysql_real_connect() failed" << endl;
		system("pause");
		return false;
	}

	//sql字符串  
	string sqlstr;
	//向表中插入数据  
	sqlstr =
		"INSERT INTO `face_recogition`.`information` (`student_id`, `stuName`,`userdir`) VALUES ('";
	sqlstr += userid;
	sqlstr += "', '";
	sqlstr += username;
	sqlstr += "', '";
	sqlstr += userdir;
	sqlstr += "');";
	if (0 == mysql_query(&mydata, sqlstr.c_str())) {
		cout << "mysql_query() insert data succeed" << endl;
	}
	else {
		cout << "输入数据有误" << endl;
		system("pause");
		mysql_close(&mydata);
		return false;
	}


	//显示刚才插入的数据  
	sqlstr = "SELECT student_id,stuName,userdir FROM information";
	MYSQL_RES *result = NULL;
	if (0 == mysql_query(&mydata, sqlstr.c_str())) {
		cout << "mysql_query() select data succeed" << endl;

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
				cout << row[i] << "\t\t";
			}
			cout << endl;
			row = mysql_fetch_row(result);
		}

	}
	else {
		cout << "mysql_query() select data failed" << endl;
		mysql_close(&mydata);
		return false;
	}
	mysql_close(&mydata);
	mysql_server_end();
	system("pause");
	return true;
}
//addrecord 函数用来向recognise_record（考勤记录表）中添加数据
bool addrecord(string userid, string name) {
	//userid 来自face_recognise.h 识别出来的学号predictPCA，name 来自face_recoquery()中数据库查询到的predictPCA对应的学生姓名result_a
	cout << "****************************************" << endl;



	//必备的一个数据结构  
	MYSQL mydata;

	//初始化数据库  
	if (0 != mysql_library_init(0, NULL, NULL)) {
		cout << "mysql_library_init() failed" << endl;
		system("pause");
		return false;
	}



	//初始化数据结构  
	if (NULL == mysql_init(&mydata)) {
		cout << "mysql_init() failed" << endl;
		system("pause");
		return false;
	}

	//在连接数据库之前，设置额外的连接选项  
	//可以设置的选项很多，这里设置字符集，否则无法处理中文  
	if (0 != mysql_options(&mydata, MYSQL_SET_CHARSET_NAME, "gbk")) {
		cout << "mysql_options() failed" << endl;
		system("pause");
		return false;
	}


	//连接数据库  
	if (NULL
		!= mysql_real_connect(&mydata, "localhost", "root", "1995", "face_recogition", 3306, NULL, 0))
		//这里的地址，用户名，密码，端口可以根据自己本地的情况更改  
	{}
	else {
		cout << "mysql_real_connect() failed" << endl;
		system("pause");
		return false;
	}

	//sql字符串  
	string sqlstr;
	//向表中插入数据  
	sqlstr =
		"INSERT INTO `face_recogition`.`recognise_record` (`studentID`, `stuName`) VALUES ('";
	sqlstr += userid;
	sqlstr += "', '";
	sqlstr += name;
	sqlstr += "');";
	if (0 == mysql_query(&mydata, sqlstr.c_str())) {
		cout << "新的考勤数据已经添加" << endl;
	}
	else {
		cout << "考勤数据添加失败" << endl;
		system("pause");
		mysql_close(&mydata);
		return false;
	}
#ifdef STEPBYSTEP  
	system("pause");
#endif  
	
	mysql_close(&mydata);
	mysql_server_end();
	system("pause");
	return true;
}
