

#include "Logcxx.h"

#ifdef __linux__

#include<log4cxx/logger.h>
#include<log4cxx/propertyconfigurator.h>

using namespace log4cxx;

#endif

void Logcxx::renameFile(string oldFileName,string newFileName)
{

	char source[256];//文件路径
	char newname[256];

	snprintf(source, sizeof(source), oldFileName.c_str());
	snprintf(newname, sizeof(newname), newFileName.c_str());


	if(!rename(source,newname))//删除成功
	{
		cout<<source<<" 成功重命名为: "<<newname<<endl;
	}
	else//无法重命名:文件打开或无权限执行重命名
	{
		cout<<"文件无法重命名(可能原因如下):"<<endl;
		cout<<"\t"<<"1. "<<newname<<" 已存在"<<endl
			<<"\t"<<"2. "<<newname<<" 正在使用,未关闭."<<endl
			<<"\t"<<"3. "<<"你没有权限重命名此文件."<<endl;
	}

}



string Logcxx::FileNameAddDate(string fileName)
{
	// 	log4j.appender.dailyFile.DatePattern='.'yyyy_MM_dd
	// 		log4j.appender.dailyFile.File=./Logs/logfile

	char buf[32] = {0};
	int fineNameLength = 32;
	string newFileName = "";

	TimeUtil::getTimeStr(buf, fineNameLength, TimeUtil::getTimeMilliSec());
	string dateString = (string)buf;
	newFileName = fileName +"."+dateString;

	return newFileName;
}



void Logcxx::initLog4cxx()
{	


#ifdef __linux__
	string fileBaseName = "Logs/logfile";
	Logcxx::renameFile(fileBaseName,Logcxx::FileNameAddDate(fileBaseName));

	// 	 Read configure file

	// 代码里用到配置文件的"Logs/logfile";
	PropertyConfigurator::configure("Data/log4cxx.properties");

	// 	 Get root logger
	rootLogger = log4cxx::Logger::getRootLogger();

	// 	 LOG4CXX_WARN(rootLogger,"write log 21111111111111111111111111111111111111111111111111111111112222222222222222 ");
	// 
	// 	
	//  	 UInt32 logLevel = LogWarn;
	//  	 string logStr = "first log !!!";
	//  	  log_writeFile(logStr,logLevel);
#endif

}

void Logcxx::log_writeFile(string logStr,UInt32 logLevel)
{

#ifdef __linux__

	if (logLevel == LogInfo)
	{
		LOG4CXX_INFO(rootLogger,logStr.c_str());
	}
	else if (logLevel == LogWarn)
	{
		LOG4CXX_WARN(rootLogger,logStr.c_str());
	}
	else 	if (logLevel == LogError)
	{
		LOG4CXX_ERROR(rootLogger,logStr.c_str());
	}
	else 	if (logLevel == LogFatal)
	{
		LOG4CXX_FATAL(rootLogger,logStr.c_str());
	}

#endif
}

void log_write(char * dest,UInt32 logLevel)
{
	String logStr;
	logStr = (String)dest;
	string str = logStr.c_str();
	Logcxx::getSingleton().log_writeFile(str,logLevel);

}

