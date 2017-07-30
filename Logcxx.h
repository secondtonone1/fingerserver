#ifndef __LYNX_GAME_SERVER_LOG4CXX_SYSTEM_H__
#define __LYNX_GAME_SERVER_LOG4CXX_SYSTEM_H__

#include "MainThread.h"
#ifdef __linux__
#include<log4cxx/logger.h>
#include<log4cxx/propertyconfigurator.h>

using namespace log4cxx;

#endif

namespace Lynx
 {


	class Logcxx : public MainThread, public Singleton<Logcxx>
	{
	public:

		void initLog4cxx();

		void log_writeFile(string logStr,UInt32 logLevel);
		void log_write(char * dest,UInt32 logLevel);

		void renameFile(string oldFileName,string newFileName);

		string FileNameAddDate(string fileName);


	};
#ifdef __linux__
	static LoggerPtr rootLogger;
#endif		
} 

#endif  //__LYNX_GAME_SERVER_LOG4CXX_SYSTEM_H__

