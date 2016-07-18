#include "LoggerSystem.h"
// #include "../TableExport/HelpHandler.h"

using namespace Lynx;

LoggerSystem::LoggerSystem()
{

}

LoggerSystem::~LoggerSystem()
{

}

bool 
LoggerSystem::initial()
{
    for (UInt32 i = 0; i < LOGGER_THREAD_WORKER_COUNT; ++i)
    {
        if (!mWorkerArray[i].initial(i))
        {
            LOG_WARN("Failed to initial logger thread worker [%u]", i);
            return false;
        }
    }
	LOG_INFO("Initial LoggerSystem");
	return true;
}

void 
LoggerSystem::release()
{
    for (UInt32 i = 0; i < LOGGER_THREAD_WORKER_COUNT; ++i)
    {
        mWorkerArray[i].release();
    }
	LOG_INFO("Release LoggerSystem");
}

void 
LoggerSystem::update(const UInt64& accTime)
{
	for (UInt32 i = 0; i < LOGGER_THREAD_WORKER_COUNT; ++i)
	{
		StreamBuffer& threadMsgStream = mWorkerArray[i].getOutputMsgQueue().getMsgs();
		UInt32 msgCount = threadMsgStream.length() / sizeof(void*);
		void* threadMsgPointer;
		ThreadMsg* threadMsg;
		for (UInt32 j = 0; j < msgCount; ++j)
		{
			threadMsgStream >> threadMsgPointer;
			threadMsg = (ThreadMsg*)threadMsgPointer;
			if (threadMsg)
			{
				dispatchThreadMsg(*threadMsg);
				deallocThreadMsg(threadMsg);
			}
		}
	}
}

void 
LoggerSystem::dispatchThreadMsg(ThreadMsg& msg)
{
	mThreadMsgHandler.onDispatchMsg(msg);
}

void LoggerSystem::writeLog(String fileName,Guid playerID,string playerName,string str)
{
	struct tm tim;
	char dest[64] = {};	
	char buf[409600];	
	TimeUtil::localTime(tim,-1);

	sprintf(buf, "%4i:%2.2i:%2.2i %2.2i:%2.2i:%2.2i playerID:%llu,playerName:%s, %s \n", 
		tim.tm_year + 1900, tim.tm_mon + 1, tim.tm_mday, tim.tm_hour,
		tim.tm_min, tim.tm_sec,playerID, playerName.c_str(),str);

	writeFile(fileName,buf);
}

void LoggerSystem::stringNameLog(string fileName,Guid playerID,string playerName,string str)
{
	struct tm tim;
	String file_Name ;
	char dest[64] = {};	
	char buf[409600];	
	TimeUtil::localTime(tim,-1);

	sprintf(buf, "%4i:%2.2i:%2.2i %2.2i:%2.2i:%2.2i playerID:%llu,playerName:%s, %s \n", 
		tim.tm_year + 1900, tim.tm_mon + 1, tim.tm_mday, tim.tm_hour,
		tim.tm_min, tim.tm_sec,playerID, playerName.c_str(),str);
	sprintf(dest,"%s",fileName.c_str());


	file_Name = (String)dest;

	if(!FileUtil::saveFile(file_Name, buf, strlen(buf)))
	{
		FileUtil::openFile(file_Name);
		FileUtil::saveFile(file_Name, buf, strlen(buf));
	}
}


 
void LoggerSystem::writeFile(String fileName,char buf[409600])
{
	FILE* fFile;
	struct tm tim;
	TimeUtil::localTime(tim,-1);

	fFile= fopen(fileName.c_str(), "a+");
	if (fFile == NULL)
	{
		return;
	}
	
	fprintf(fFile, buf); 
	fclose(fFile);
}

/*
void LoggerSystem::writeErrorLog(Guid playerID,string playerName,string str)
{
	struct tm *tim = localtime(-1);
	fprintf(mOutputFile, "%4i:%2.2i:%2.2i %2.2i:%2.2i:%2.2i %s [%s:%i] ", 
		tim->tm_year + 1900, tim->tm_mon + 1, tim->tm_mday, tim->tm_hour,
		tim->tm_min, tim->tm_sec, subsys, fileName.c_str(), line);

	sprintf(buf, "%s}\r\n\r\n", buf);
	FileUtil::saveFile("ErrorLog.log", buf, strlen(buf));
}
*/


