#ifndef _JZCLOGLIBRARY_H_
#define _JZCLOGLIBRARY_H_

#define GOOGLE_GLOG_DLL_DECL

#include "JZLog.h"
#include <string>
#include <mutex>

using std::string;
using std::mutex;


class CLogLibrary : public ILogLibrary {

public:
	CLogLibrary(const char* pszModuleName);
	~CLogLibrary();

public:
	void UnInit(); 
	int SetLogDir(const char* pszLogDir);  //
	int SetLogProperty(ECreateFileType eFileType, int nFileSize);  //
	int SetLogLevel(ELogOutputType eOutputType, int nLevel); //
	int Log(int nLevel, const char* message);

private:
	bool m_bInit;
	bool m_bCreateNewLog;
	string m_strBaseName;
	string m_strLogDir;
	int m_nMaxLogSize;
	int m_nLogFileLevel;
	int m_nLogScreenLevel;
	mutex m_mutex;
};


#endif
