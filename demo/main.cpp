
#include "JZLog.h"

int main()
{
	ILogLibrary* pLog = CreateLog("test");
	pLog->SetLogDir("./");
	pLog->SetLogProperty(eFileCreate, 100);
	pLog->SetLogLevel(eConsole, JZ_DEBUG);
	pLog->SetLogLevel(eFile, JZ_INFO);

	LogDebug(pLog) << "this is debug message.";
	LogDebug(pLog) << "this is debug ¸ãÉ¶×Ó£¿£¡@#£¤.";
	LogDebug(pLog) << "this is debug message.";
	LogInfo(pLog) << "this is info message." << "yyyy" << 111 << " f:" << 12.369;
	pLog->Log(JZ_WARN, "this is warn message.");
	pLog->Log(JZ_ERROR, "this is error message.");
	
	getchar();
	return 0;

}
