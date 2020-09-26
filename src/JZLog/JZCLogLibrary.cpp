#include "JZCLogLibrary.h"
#ifdef ERROR
#define GLOG_NO_ABBREVIATED_SEVERITIES
#endif
#include "logging.h"
#include <map>

using std::map;
using std::lock_guard;

using namespace google;

//日志管理类
class LogManager {
public:
	LogManager();
	~LogManager();
	
	bool AddLog(char* arg);
	void DelLog(ILogLibrary* pLogLibrary);
	ILogLibrary* GetLog(char* arg);
	
private:
	map<string, ILogLibrary*> m_mapLogs;
	bool m_bInit;
	mutex m_mutex;
};

LogManager::LogManager()
:m_bInit(false)
{
	
}

LogManager::~LogManager()
{
	lock_guard<mutex> lock(m_mutex);
	for(auto it = m_mapLogs.begin();it != m_mapLogs.end(); )
	{
		delete it->second;
		m_mapLogs.erase(it++);
	}
	
	if(m_bInit)
	{
		ShutdownGoogleLogging();
		m_bInit = false;
	}
}

bool LogManager::AddLog(char* arg)
{
	bool bRet = true;
	lock_guard<mutex> lock(m_mutex);
	if(!m_bInit)
	{
		InitGoogleLogging("");
		FLAGS_logbufsecs = 0;       //立马将日志写到文件
		FLAGS_stderrthreshold = 0;  //默认输出到屏幕的等级
		m_bInit = true;
	}
	auto it = m_mapLogs.find(arg);
	if(it == m_mapLogs.end())
	{
		m_mapLogs[arg] = new CLogLibrary(arg);
	}
	else
	{
		bRet = false;
	}
	return bRet;
}

void LogManager::DelLog(ILogLibrary* pLogLibrary)
{
	lock_guard<mutex> lock(m_mutex);
	for(auto it = m_mapLogs.begin();it != m_mapLogs.end();it++)
	{
		if(it->second == pLogLibrary)
		{
			delete it->second;
			m_mapLogs.erase(it);
			break;
		}
	}
	if(m_bInit)
	{
		ShutdownGoogleLogging();
		m_bInit = false;
	}
}

ILogLibrary* LogManager::GetLog(char* arg)
{
	lock_guard<mutex> lock(m_mutex);
	auto it = m_mapLogs.find(arg);
	if(it != m_mapLogs.end())
		return it->second;
	
	return NULL;
}

//全局日志管理类
static LogManager g_logmanager;

CLogLibrary::CLogLibrary(const char* pszModuleName):ILogLibrary(pszModuleName),m_bInit(false)
{
	m_strBaseName = "MyLog";
	if(pszModuleName)
	{
		string strTemp = pszModuleName;
		if(strTemp != "")
			m_strBaseName = strTemp;
	}
	
	m_strLogDir = "./";
}

CLogLibrary::~CLogLibrary()
{
	if(m_bInit)
	{
		UnInit();
	}
}

void CLogLibrary::UnInit()
{
	
}

int CLogLibrary::SetLogDir(const char* pszLogDir)
{
	if(pszLogDir == NULL)
		return -1;
	if(strlen(pszLogDir) > 256)
		return -1;
	lock_guard<mutex> lock(m_mutex);
	m_strLogDir = pszLogDir;
	m_bInit = false;
	return 0;
}

int CLogLibrary::SetLogProperty(ECreateFileType eFileType, int nFileSize)
{
	if(nFileSize < 1 || nFileSize > 1000)
		return -1;
	lock_guard<mutex> lock(m_mutex);
	if(eFileType == eFileRewind)
		m_bCreateNewLog = false;
	else
		m_bCreateNewLog = true;
	
	m_nMaxLogSize = nFileSize;
	return 0;
}

int CLogLibrary::SetLogLevel(ELogOutputType eOutputType, int nLevel)
{
	lock_guard<mutex> lock(m_mutex);
	if(eOutputType == eConsole)
		m_nLogScreenLevel = nLevel-1;
	else
		m_nLogFileLevel = nLevel -1;
	m_bInit = false;
	return 0;
}

int CLogLibrary::Log(int nLevel, const char* message)
{
	int nRet = 0;
	{
		lock_guard<mutex> lock(m_mutex);
		if(!m_bInit)
		{
			if(m_strLogDir == "")
				return -1;
			
			AddLogDestination(m_strBaseName.c_str(), m_strLogDir.c_str());
			ChangeLogDestination(m_strBaseName.c_str(), m_bCreateNewLog, m_nMaxLogSize, m_nLogFileLevel, m_nLogScreenLevel);
			m_bInit = true;
		}
	}
	
	switch(nLevel-1)
	{
		case GLOG_DEBUG:
			MYLOG(m_strBaseName.c_str(), DEBUG) << message;
			break;
		case GLOG_INFO:
			MYLOG(m_strBaseName.c_str(), INFO) << message;
			break;
		case GLOG_WARNING:
			MYLOG(m_strBaseName.c_str(), WARNING) << message;
			break;
		case GLOG_ERROR:
			MYLOG(m_strBaseName.c_str(), ERROR) << message;
			break;
		case GLOG_FATAL:
			MYLOG(m_strBaseName.c_str(), FATAL) << message;
			break;
		default:
			nRet = -1;
			break;
	}
	
	return nRet;
}


//创建日志
ILogLibrary* CreateLog(char* arg)
{
	if(g_logmanager.AddLog(arg))
		return g_logmanager.GetLog(arg);
	
	return NULL;
}

//获取日志接口
ILogLibrary* GetLog(char* arg)
{
	return g_logmanager.GetLog(arg);
}

//清理退出
void CleanupLog(ILogLibrary* pLogLibrary)
{
	g_logmanager.DelLog(pLogLibrary);
	return;
}
