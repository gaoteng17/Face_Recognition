# win32 + opencv3.3 + VS2015

**注：此目录文件暂时不再更改，提交更改请选择 /windows 目录**

these *.dll* files are needed:
* opencv_core330d.dll
* opencv_face330d.dll
* opencv_highgui330d.dll
* opencv_imgcodecs330d.dll
* opencv_imgproc330d.dll
* opencv_objdetect330d.dll
* opencv_videoio330d.dll

------17/09/17------

1. 解决opencv与sql头文件cv命名空间的冲突

2. 将sql与qt窗口整合（工程代码在 /windows 目录下）

------17/09/14------

1. 数据库测试程序testsql.h中加入了if not exist，新建"信息表"、"考勤表"

2. 注册程序已修改，若输入学号有误，则不创建对应文件夹（否则将影响训练函数）

------17/09/13------

1. 添加了函数face_recoqery() 函数用来显示识别成功后的学生信息，并在recognise_record（考勤记录表）中新建一行数据

2. 添加了函数addrecord 函数用来向recognise_record（考勤记录表）中添加数据

3. 优化了一些细节

------17/09/08------

by qiu

1. 添加了数据库连接测试程序

2. 添加了学生信息注册程序

3. 添加了考勤信息查询程序

4. 添加主程序考勤信息查询

------17/09/07------

1. 添加主程序退出部分

2. 添加人脸识别条件（连续5次相同）

------17/09/06------

1. 删掉py文件写标签部分，改用cpp

2. 合并写标签部分到拍照模块，并改称注册模块

3. 子函数写入头文件中，减少工程文件数目

