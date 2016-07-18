#include "PassportWorker.h"
#include "ConfigParser.h"

using namespace Lynx;

PassportWorker::PassportWorker()
{

}

PassportWorker::~PassportWorker()
{

}

bool 
PassportWorker::initial(UInt32 index)
{
    REGISTER_THREAD_MSG(mThreadMsgHandler, PassportAuthReq, PassportWorker::onPassportAuthReq);
//	REGISTER_THREAD_MSG(mThreadMsgHandler, PassportGMAuthReq, PassportWorker::onPassportGMAuthReq);
	
	REGISTER_THREAD_MSG(mThreadMsgHandler, PassportCodeReq, PassportWorker::onPassportCodeReq);



    String ip = ConfigParser::getSingleton().getAccountDbIp();
    UInt16 port = ConfigParser::getSingleton().getAccountDbPort();
    String username = ConfigParser::getSingleton().getAccountDbUsername();
    String password = ConfigParser::getSingleton().getAccountDbPassword();
    String dbname = ConfigParser::getSingleton().getAccountDbName();

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
PassportWorker::release()
{
    shutdown();
    join();

    DEREGISTER_THREAD_MSG(mThreadMsgHandler, PassportAuthReq);
//	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PassportGMAuthReq);

    LOG_INFO("Shutdown PassportWorker [%u]", mWorkerIndex);
}

void 
PassportWorker::onThreadStarted()
{
	
}

void 
PassportWorker::onThreadShutdown()
{
	
}

void 
PassportWorker::onPreUpdate()
{

}

void 
PassportWorker::onPostUpdate()
{

}

void 
PassportWorker::onDispatchMsg(ThreadMsg& msg)
{
	mThreadMsgHandler.onDispatchMsg(msg);
}

void 
PassportWorker::onPassportAuthReq(PassportAuthReq& msg)
{
    String accountName = msg.mAccountName;
    String password = msg.mPassword;
    ConnId connId = msg.mConnId;

    PassportAuthResp passportAuthResp;
    passportAuthResp.mConnId = msg.mConnId;
    passportAuthResp.mAccountName = accountName;
    passportAuthResp.mFromIp = msg.mFromIp;
    passportAuthResp.mFromPort = msg.mFromPort;

    // 数据库校验
    char sql[2048];
    snprintf(sql, sizeof(sql), "call Account_LoadInfo('%s')", accountName.c_str());
    LOG_DEBUG("Sql:%s", sql);
    bool result = mDBInterface.execSql(sql);
    MYSQL_RES* rs = mDBInterface.storeResult();
    if(!result)
    {
        mDBInterface.freeResult(&rs);
        passportAuthResp.merrorId = LynxErrno::SqlExecuteFail;     
        postMsgToOutputQueue(passportAuthResp, 0);
        return;
    }
    MYSQL_ROW row = mDBInterface.fetchRow(rs);
    if(row)
    {
        passportAuthResp.mAccountId = lynxAtoi<Guid>(row[0]);
        std::string realPassword = row[1];
        if(realPassword == password)
        {
            mDBInterface.freeResult(&rs);

            passportAuthResp.merrorId = LynxErrno::None;
            postMsgToOutputQueue(passportAuthResp, 0);            
            return;
        }
        else
        {
            mDBInterface.freeResult(&rs);

            passportAuthResp.merrorId = LynxErrno::AccountAuthFailed;
            postMsgToOutputQueue(passportAuthResp, 0);
            return;
        }
    }
    else
    {
        mDBInterface.freeResult(&rs);

        passportAuthResp.merrorId = LynxErrno::AccountNotExist;
        postMsgToOutputQueue(passportAuthResp, 0);
        return;
    }
}


void 
PassportWorker::onPassportCodeReq(PassportCodeReq& msg)
{
	PassporCodeResp resp;

	// 数据库校验
	char sql[2048];

	if(msg.isSave == 1)
	{
		snprintf(sql, sizeof(sql), "REPLACE INTO exchange_code(code,gift_type,player_uuid)VALUES('%s',%u,%llu)", msg.code.c_str(),msg.reqType,msg.playerID);
		LOG_DEBUG("Sql:%s", sql);
		bool result = mDBInterface.execSql(sql);
		MYSQL_RES* rs = mDBInterface.storeResult();
		if(!result)
		{
			LOG_DEBUG("error onPassportCodeReq");		
		}
			
		mDBInterface.freeResult(&rs);
		return;
	}

	
	snprintf(sql, sizeof(sql), "SELECT * FROM exchange_code WHERE code = '%s' AND gift_type = %d", msg.code.c_str(),msg.reqType);
	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!result)
	{
		mDBInterface.freeResult(&rs);
		resp.errorId = LynxErrno::SqlExecuteFail;     
		postMsgToOutputQueue(resp, 0);
		return;
	}
	MYSQL_ROW row = mDBInterface.fetchRow(rs);
	resp.PlayerID = msg.playerID;
	if(row)
	{
		Guid playerID = 0;

		resp.code = row[0];
		playerID = lynxAtoi<Guid>(row[1]);
		resp.reqType = lynxAtoi<Guid>(row[2]);
		resp.beginTime = lynxAtoi<Guid>(row[3]);
		resp.endTime = lynxAtoi<Guid>(row[4]);
		resp.awardID = lynxAtoi<Guid>(row[5]);

		if (playerID != 0)
		{
			resp.errorId = LynxErrno::Used;    
		}		

		mDBInterface.freeResult(&rs);		
	}
	else
	{
		mDBInterface.freeResult(&rs);

		resp.errorId = LynxErrno::NotFound;		
	}
	postMsgToOutputQueue(resp, 0);
	return;
}
//void 
//PassportWorker::onPassportGMAuthReq(PassportGMAuthReq& msg)
//{
//    String accountName = msg.mAccountName;
//    String password = msg.mAccountPassword;
//    ConnId connId = msg.mConnId;
//
//    PassportGMAuthResp passportAuthResp;
//    passportAuthResp.mConnId = msg.mConnId;
//    passportAuthResp.mGMData.mAccountName = msg.mAccountName;
//    passportAuthResp.mFromIp = msg.mFromIp;
//    passportAuthResp.mFromPort = msg.mFromPort;
//
//    // 数据库校验
//    char sql[2048];
//    snprintf(sql, sizeof(sql), "call GM_Account_LoadInfo('%s')", accountName.c_str());
//    LOG_DEBUG("Sql:%s", sql);
//    bool result = mDBInterface.execSql(sql);
//    MYSQL_RES* rs = mDBInterface.storeResult();
//    if(!result)
//    {
//        mDBInterface.freeResult(&rs);
//        passportAuthResp.merrorId = LynxErrno::SqlExecuteFail;     
//        postMsgToOutputQueue(passportAuthResp, 0);
//        return;
//    }
//    MYSQL_ROW row = mDBInterface.fetchRow(rs);
//    if(row)
//    {
//        std::string realPassword = row[1];
//        if(realPassword == password)
//        {
//            passportAuthResp.mGMData.mLevel = lynxAtoi<UInt8>(row[2]);
//            passportAuthResp.mGMData.mAccountPassword = password;
//            mDBInterface.freeResult(&rs);
//            passportAuthResp.merrorId = LynxErrno::None;
//            postMsgToOutputQueue(passportAuthResp, 0);            
//            return;
//        }
//        else
//        {
//            mDBInterface.freeResult(&rs);
//            passportAuthResp.merrorId = LynxErrno::AccountAuthFailed;
//            postMsgToOutputQueue(passportAuthResp, 0);
//            return;
//        }
//    }
//    else
//    {
//        mDBInterface.freeResult(&rs);
//        passportAuthResp.merrorId = LynxErrno::AccountNotExist;
//        postMsgToOutputQueue(passportAuthResp, 0);
//        return;
//    }
//}

DBInterface PassportWorker::getDBInterface()
{
	return mDBInterface;
}
