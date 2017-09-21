#include <winsock.h>
#include <iostream>  
#include <string>  
#include <mysql.h> 
using namespace std;

#pragma comment(lib, "ws2_32.lib")  
#pragma comment(lib, "libmysql.lib")  

//单步执行，不想单步执行就注释掉  
//#define STEPBYSTEP  


//addinfo 函数用来向表information（个人信息表）中添加数据
bool addinfo(string userid, string username, string userdir) {

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
	if (NULL == mysql_real_connect(&mydata, "localhost", "root", "950313", "database1", 3306, NULL, 0)){
		//这里的地址，用户名，密码，端口可以根据自己本地的情况更改  
		cout << "mysql_real_connect() failed" << endl;
		return false;
	}

	//sql字符串  
	string sqlstr = "INSERT INTO `information` (`student_id`, `stuName`,`userdir`) VALUES ('" + userid + "', '" + username + "', '" + userdir + "');";


	if (mysql_query(&mydata, sqlstr.c_str())) {
		cout << "addinfo() insert data failed" << endl;
		mysql_close(&mydata);
		return false;
	}

	return true;
}



//addrecord 函数用来向recognise_record（考勤记录表）中添加数据
bool addrecord(string userid, string name) {

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

	//sql字符串  
	string sqlstr = "INSERT INTO `recognise_record` (`studentID`, `stuName`) VALUES ('" + userid + "', '" + name + "');";

	if (mysql_query(&mydata, sqlstr.c_str())) {
		cout << "addrecord() insert data failed" << endl;
		mysql_close(&mydata);
		return false;
	}
	
	mysql_close(&mydata);
	mysql_server_end();
	return true;
}
