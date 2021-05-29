# JZLog日志库

采用google glog开源日志库，并修改了源码重新进行封装，更易于使用，修改了以下功能：
* 增加了DEBUG级别日志，有DEBUG、INFO、WARNING、ERROR、FATAL五种日志级别；
* 增加了按日期存放日志文件，增加日期更改判断，自动生成新的日期文件夹和日志文件
* 修改了所有日志等级的日志都将写到同一个日志文件，而不是分开写
* 修改了日志输出格式

## 编译说明
### Linux编译：
* cd src
* chmod +x ./make-glog.sh
* ./make-glog.sh 
* cd ..
* mkdir build 
* cd build
* cmake ..
* make
* 在output目录下会生成demo的可执行文件和JZLog.so
### Windows编译:
* 在JZLog根目录
* mkdir src/glog/buildglog
* cd src/glog/buildglog
* cmake ..
* 在buildglog可以看到glog.sln工程，vs打开并生成静态库
* 在buildglog找到生成后的glogd.lib或者glog.lib，copy到src/lib
* cd回到JZLog的根目录
* mkdir build
* cd build
* 生成debug: cmake -DCMAKE_BUILD_TYPE=Debug .. (生成release: cmake -DCMAKE_BUILD_TYPE=Release ..)
* 在build目录下可以找到JZLog.sln工程，vs打开生成JZLog.dll和demo.exe
* 在output/Debug 或 output/Release目录下可以找到JZLog.dll和demo.exe

## 使用说明
依赖头文件JZLog.h和动态库
* 初始化:  ILogLibrary *pLog = CreateLog("propject name");
* 设置日志路径: pLog->SetLogDir("./");
* 设置文件创建方式和文件大小:  pLog->SetLogProperty(eFileCreate, 100);
* 设置控制台输出等级最低DEBUG： pLog->SetLogLevel(eConsole, JZ_DEBUG);
* 设置文件输出等级最低INFO: pLog->SetLogLevel(eFile, JZ_INFO);
* 写日志： 
	方式一: pLog->Log(JZ_ERROR, "this is error message");
	
	方式二：LogError(pLog) << "this is error message." << " can write int:" << 123 << " can write double：" << 123.456;
	
	
* 反初始化: CleanupLog(pLog);
* 以上接口可多次调用

