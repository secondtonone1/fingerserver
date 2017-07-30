#include "LoggerWorker.h"
#include "ConfigParser.h"
#include "ServerData.h"

using namespace Lynx;

LoggerWorker::LoggerWorker()
{

}

LoggerWorker::~LoggerWorker()
{

}

bool 
LoggerWorker::initial(UInt32 index)
{


	REGISTER_THREAD_MSG(mThreadMsgHandler, LoggerPlayerTypeLogReq, LoggerWorker::onLoggerPlayerTypeLogReq);

	REGISTER_THREAD_MSG(mThreadMsgHandler, LoggerPlayerLogoutReq, LoggerWorker::onLoggerPlayerLogoutReq);


	String ip = ConfigParser::getSingleton().getLogDbIp();
	UInt16 port = ConfigParser::getSingleton().getLogDbPort();
	String username = ConfigParser::getSingleton().getLogDbUsername();
	String password = ConfigParser::getSingleton().getLogDbPassword();
	String dbname = ConfigParser::getSingleton().getLogDbName();

	if (!mDBInterface.initial(ip.c_str(), port, username.c_str(),
		password.c_str(), dbname.c_str()))
	{
		LOG_WARN("Failed to connect mysql server. [%s %s %s %s]",
			ip.c_str(), username.c_str(), password.c_str(), dbname.c_str());
		return false;
	}

	mWorkerIndex = index + 1;
	startup();

	LOG_INFO("Start PassportWorker [%u]", mWorkerIndex);
	return true;
}

void 
LoggerWorker::release()
{
    shutdown();
    join();

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, LoggerPlayerTypeLogReq);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, LoggerPlayerLogoutReq);


	mDBInterface.release();
	LOG_INFO("Shutdown LoggerWorker [%u]", mWorkerIndex);
}


void 
LoggerWorker::onThreadStarted()
{
	
}

void 
LoggerWorker::onThreadShutdown()
{
	
}

void 
LoggerWorker::onPreUpdate()
{

}

void 
LoggerWorker::onPostUpdate()
{

}

void 
LoggerWorker::onDispatchMsg(ThreadMsg& threadMsg)
{
	mThreadMsgHandler.onDispatchMsg(threadMsg);
}





void LoggerWorker::onLoggerPlayerTypeLogReq( LoggerPlayerTypeLogReq &msg)
{

// 	ServerPublicData  serverPublicData = ServerData::getSingleton().getServerPublicData();
// 	char sql[2048];
// 
// 	snprintf(sql, sizeof(sql), "call player_log(%llu,'%s',%u,'%s',%u,%llu,'%s')",msg.playerID,msg.name.c_str(),msg.type,msg.explain.c_str(),msg.serverID,msg.accountID,msg.tableName.c_str());
// 
// 	LOG_DEBUG("Sql:%s", sql);
// 
// 	bool result = mDBInterface.execSql(sql);
// 
// 	if (!result)
// 	{
// 		LOG_INFO("deal player_log fail playerUid: %llu",msg.playerID);
// 	}
// 	else
// 	{
// 		LOG_INFO("deal player_log success playerUid: %llu",msg.playerID);
// 	}
// 
// 	MYSQL_RES* rs = mDBInterface.storeResult();
// 
// 	mDBInterface.freeResult(&rs);
// 	return;

}

void LoggerWorker::onLoggerPlayerLogoutReq( LoggerPlayerLogoutReq &msg)
{

	ServerPublicData  serverPublicData = ServerData::getSingleton().getServerPublicData();
	char sql[2048];

	snprintf(sql, sizeof(sql), "call player_logout_update(%llu,'%s',%u,%llu,'%s')",msg.playerID,msg.name.c_str(),msg.serverID,msg.accountID,msg.tableName.c_str());


	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);

	if (!result)
	{
		LOG_INFO("deal player_logout_update fail playerUid: %llu",msg.playerID);
	}
	else
	{
		LOG_INFO("deal player_logout_update success playerUid: %llu",msg.playerID);
	}

	MYSQL_RES* rs = mDBInterface.storeResult();

	mDBInterface.freeResult(&rs);
	return;

}