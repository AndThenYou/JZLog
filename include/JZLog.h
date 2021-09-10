#ifndef _JZLOG_H_
#define _JZLOG_H_

#include <sstream>
#include <string>

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


#define MAX_CONTENT_LEN 4096

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


//----------方便使用----------------
class JZLogStream 
{
public:
	JZLogStream(ILogLibrary* log, int level) 
		: m_log(log), m_message(""), m_level(level)
	{
	}
	~JZLogStream()
	{
		if (m_log)
			m_log->Log(m_level, m_message.c_str());
	}

	JZLogStream& write_buffer(const char* src, int src_len)
	{
		if (m_log)
		{
			if (src && src_len > 0)
			{
				src_len = src_len > (MAX_CONTENT_LEN - m_message.size()) ? (MAX_CONTENT_LEN - m_message.size()) : src_len;
				m_message.insert(m_message.size(), src, src_len);
			}
			else
			{
				m_message += "null";
			}
		}
		return *this;
	}

	JZLogStream& operator << (const char* msg) { return write_buffer(msg, strlen(msg)); }
	JZLogStream& operator << (char ch) { return write_buffer(&ch, 1); }
	JZLogStream & operator << (unsigned char ch) { return (*this << (unsigned long long)ch); }
	JZLogStream& operator << (bool val) { return (val ? write_buffer("true", 4) : write_buffer("false", 5)); }

	JZLogStream & operator << (short val) { return (*this << (long long)val); }
	JZLogStream & operator << (unsigned short val) { return (*this << (unsigned long long)val); }
	JZLogStream & operator << (int val) { return (*this << (long long)val); }
	JZLogStream & operator << (unsigned int val) { return (*this << (unsigned long long)val); }
	JZLogStream & operator << (long val) { return (*this << (long long)val); }
	JZLogStream & operator << (unsigned long val) { return (*this << (unsigned long long)val); }

	JZLogStream& operator << (long long integer) {
		std::stringstream sstr;
		sstr << integer;
		if (!m_log || m_message.size() > (MAX_CONTENT_LEN - sstr.str().size()))
		{
			return *this;
		}
		m_message += sstr.str();
		return *this;
	}

	JZLogStream& operator << (unsigned long long integer) {
		std::stringstream sstr;
		sstr << integer;
		if (!m_log || m_message.size() > (MAX_CONTENT_LEN - sstr.str().size()))
		{
			return *this;
		}
		m_message += sstr.str();
		return *this;
	}

	JZLogStream& operator << (float f)
	{
		std::stringstream sstr;
		sstr << f;
		if (!m_log || m_message.size() > (MAX_CONTENT_LEN - sstr.str().size()))
		{
			return *this;
		}
		m_message += sstr.str();
		return *this;
	}
	JZLogStream& operator << (double df)
	{
		std::stringstream sstr;
		sstr << df;
		if (!m_log || m_message.size() > (MAX_CONTENT_LEN - sstr.str().size()))
		{
			return *this;
		}
		m_message += sstr.str();
		return *this;
	}

private:
	ILogLibrary * m_log;
	int m_level;
	std::string m_message;
};

#define LOG_STREAM(log, level) JZLogStream(log, level) << " [" << __FILE__ <<":"<< __FUNCTION__ << "] "

#define LogDebug(log) LOG_STREAM(log, JZ_DEBUG)
#define LogInfo(log) LOG_STREAM(log, JZ_INFO)
#define LogWarn(log) LOG_STREAM(log, JZ_WARN)
#define LogError(log) LOG_STREAM(log, JZ_ERROR)
#define LogFatal(log) LOG_STREAM(log, JZ_FATAL)


JZLOG_API ILogLibrary* CreateLog(char* arg);
JZLOG_API ILogLibrary* GetLog(const char* arg);
JZLOG_API void CleanupLog(ILogLibrary* pLogLibrary);

#endif