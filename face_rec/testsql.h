#ifndef TESTSQL_H
#define TESTSQL_H

#endif // TESTSQL_H

#include <winsock.h>
#include <iostream>  
#include <string>  
#include <mysql.h>  
using namespace std;

#pragma comment(lib, "ws2_32.lib")  
#pragma comment(lib, "libmysql.lib")  

//单步执行，不想单步执行就注释掉  
#define STEPBYSTEP  

int testsql() {
	cout << "****************************************" << endl;
	cout << "正在进行数据初始化测试,请耐心等待0_o\n";

 

	//必备的一个数据结构  
	MYSQL mydata;

	//初始化数据库  
	if (0 == mysql_library_init(0, NULL, NULL)) {
		cout << "mysql_library_init() succeed" << endl;
	}
	else {
		cout << "mysql_library_init() failed" << endl;
		return -1;
	}

 

	//初始化数据结构  
	if (NULL != mysql_init(&mydata)) {
		cout << "mysql_init() succeed" << endl;
	}
	else {
		cout << "mysql_init() failed" << endl;
		return -1;
	}


	//在连接数据库之前，设置额外的连接选项  
	//可以设置的选项很多，这里设置字符集，否则无法处理中文  
	if (0 == mysql_options(&mydata, MYSQL_SET_CHARSET_NAME, "gbk")) {
		cout << "mysql_options() succeed" << endl;
	}
	else {
		cout << "mysql_options() failed" << endl;
		return -1;
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
		return -1;
	}


	//sql字符串  
	string sqlstr;
	//创建一个表  
	MYSQL_RES *result = NULL;
	sqlstr = "CREATE TABLE IF NOT EXISTS information";
	sqlstr += "(";
	sqlstr += "student_id varchar(20) NOT NULL PRIMARY KEY ,";
	sqlstr += "stuName varchar(20) NOT NULL,";
	sqlstr += "userdir varchar(100) DEFAULT NULL";
	sqlstr += ")";
	if (0 == mysql_query(&mydata, sqlstr.c_str())) {
		cout << "mysql_query() recreate table succeed" << endl;
	}
	else {
		cout << "mysql_query() recreate table failed" << endl;
		mysql_close(&mydata);
		return -1;
	}
	sqlstr = "CREATE TABLE IF NOT EXISTS recognise_record";
	sqlstr += "(";
	sqlstr += "studentID varchar(45) DEFAULT NULL ,";
	sqlstr += "stuName varchar(45) DEFAULT NULL,";
	sqlstr += "reco_time timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP";
	sqlstr += ")";
	if (0 == mysql_query(&mydata, sqlstr.c_str())) {
		cout << "mysql_query() recreate table succeed" << endl;
	}
	else {
		cout << "mysql_query() recreate table failed" << endl;
		mysql_close(&mydata);
		return -1;
	}
	cout << "****************************************" << endl;
	mysql_free_result(result);
	mysql_close(&mydata);
	mysql_server_end();

	system("pause");
	return 0;
}
