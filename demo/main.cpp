
#include "JZLog.h"

int main()
{
	ILogLibrary* pLog = CreateLog("test");
	pLog->SetLogDir("./");
	pLog->SetLogProperty(eFileCreate, 100);
	pLog->SetLogLevel(eConsole, JZ_INFO);
	pLog->SetLogLevel(eFile, JZ_INFO);

	pLog->Log(JZ_INFO, "this is info message.");
	pLog->Log(JZ_WARN, "this is warn message.");
	pLog->Log(JZ_ERROR, "this is error message.");
	
	return 0;

}
