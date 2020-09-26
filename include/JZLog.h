#ifndef _JZLOG_H_
#define _JZLOG_H_

#include <sstream>

#ifdef WIN32
#include <Windows.h>

#ifndef JZLOG_API
#ifdef _WIN32
#ifdef JZ_EXPORT
#define JZLOG_API extern "C" __declspec(dllexport)
#else
#define JZLOG_API extern "C" __declspec(dllimport)
#endif
#else
#define JZLOG_API
#endif
#endif

#else
#define JZLOG_API	 extern "C"
#endif

enum ECreateFileType
{
	eFileRewind = 0,  //回卷方式创建
	eFileCreate = 1,  //新建方式创建
};

enum ELogOutputType
{
	eConsole = 0,   //控制台输出
	eFile = 1, 	//文件输出
};

#define JZ_DEBUG 1
#define JZ_INFO 2
#define JZ_WARN 3
#define JZ_ERROR 4
#define JZ_FATAL 5


/*使用抽象类而不是导出类：1.减少导出内容； 2.产生的dll能够通用*/
class ILogLibrary {
public:
	ILogLibrary(const char* pszMoudleName){};
	virtual ~ILogLibrary(){};
	
	virtual int SetLogDir(const char* pszLogDir) = 0;  //设置日志路径
	virtual int SetLogProperty(ECreateFileType eFileType, int nFileSize) = 0;  //设置日志文件属性
	virtual int SetLogLevel(ELogOutputType eOutputType, int nLevel) = 0; //设置日志输出等级
	virtual int Log(int nLevel, const char* message) = 0;
};

JZLOG_API ILogLibrary* CreateLog(char* arg);
JZLOG_API ILogLibrary* GetLog(const char* arg);
JZLOG_API void CleanupLog(ILogLibrary* pLogLibrary);

#endif