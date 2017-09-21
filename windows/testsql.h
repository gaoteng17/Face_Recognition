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

//����ִ�У����뵥��ִ�о�ע�͵�  
//#define STEPBYSTEP  

bool testsql() {
	//cout << "****************************************" << endl;
	//cout << "���ڽ������ݳ�ʼ������,�����ĵȴ�0_o\n";



	//�ر���һ�����ݽṹ  
	MYSQL mydata;

	//��ʼ�����ݿ�  
	if (mysql_library_init(0, NULL, NULL)) {
		cout << "mysql_library_init() failed" << endl;
		return false;
	}

	//��ʼ�����ݽṹ  
	if (NULL == mysql_init(&mydata)) {
		cout << "mysql_init() failed" << endl;
		//system("pause");
		return false;
	}

	//���������ݿ�֮ǰ�����ö��������ѡ��  
	//�������õ�ѡ��ܶ࣬���������ַ����������޷���������  
	if (mysql_options(&mydata, MYSQL_SET_CHARSET_NAME, "gbk")) {
		cout << "mysql_options() failed" << endl;
		return false;
	}



	//�������ݿ�  
	if (NULL == mysql_real_connect(&mydata, "localhost", "root", "950313", "database1", 3306, NULL, 0)) {
		//����ĵ�ַ���û��������룬�˿ڿ��Ը����Լ����ص��������  
		cout << "mysql_real_connect() failed" << endl;
		return false;
	}


	//sql�ַ���  
	string sqlstr;
	//����һ����  
	MYSQL_RES *result = NULL;

	sqlstr = "CREATE TABLE IF NOT EXISTS information";
	sqlstr += "(";
	sqlstr += "student_id varchar(20) NOT NULL PRIMARY KEY ,";
	sqlstr += "stuName varchar(20) NOT NULL,";
	sqlstr += "userdir varchar(100) DEFAULT NULL";
	sqlstr += ")";
	if (mysql_query(&mydata, sqlstr.c_str())) {
		cout << "mysql_query() recreate table failed" << endl;
		mysql_close(&mydata);
		return false;
	}

	sqlstr = "CREATE TABLE IF NOT EXISTS recognise_record";
	sqlstr += "(";
	sqlstr += "studentID varchar(45) DEFAULT NULL ,";
	sqlstr += "stuName varchar(45) DEFAULT NULL,";
	sqlstr += "reco_time timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP";
	sqlstr += ")";
	if (mysql_query(&mydata, sqlstr.c_str())) {
		cout << "mysql_query() recreate table failed" << endl;
		mysql_close(&mydata);
		return false;
	}
	
	//cout << "****************************************" << endl;
	
	mysql_free_result(result);
	mysql_close(&mydata);
	mysql_server_end();

	return true;
}

