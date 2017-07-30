#include "PersistSystem.h"
#include "GameServer.h"
#include "PlayerData.h"

using namespace Lynx;

PersistSystem::PersistSystem()
{

}

PersistSystem::~PersistSystem()
{

}

bool 
PersistSystem::initial()
{
    REGISTER_THREAD_MSG(mThreadMsgHandler, PersistLoadPlayerDataResp, PersistSystem::onPersistLoadPlayerDataResp);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistFindSimilarPowerResp, PersistSystem::onPersistFindSimilarPowerResp);
  
	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistManagerSetDirtyResp, PersistSystem::onPersistSetDirtyResp);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistNotifyWorkerSaveDbResp, PersistSystem::onPersistNotifyWorkerSaveDbResp);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistCreatePlayerResp, PersistSystem::onPersistCreatePlayerResp);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistLoadOfflineDataResp, PersistSystem::onPersistLoadOfflineDataResp);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistLoadPlayerByAccountResp, PersistSystem::onPersistLoadPlayerByAccountResp);

	REGISTER_THREAD_MSG(mThreadMsgHandler, PersistInsertRobotResp, PersistSystem::onPersistInsertRobotResp);

    if (!loadDatabaseData())
    {
        LOG_WARN("Failed to load database data.");
        return false;
    }

    for (UInt32 i = 0; i < PERSIST_THREAD_WORKER_COUNT; ++i)
    {
        if (!mWorkerArray[i].initial(i))
        {
            LOG_WARN("Failed to initial persist thread worker [%u]", i);
            return false;
        }
    }

	m_nLastSyncTime = 0;
// 	mWorkerArray[0].textDb();
	 
	

	LOG_INFO("Initial PersistSystem");

	if (mWorkerArray[0].loadStageRangking() != LynxErrno::None)
	{
		LOG_ERROR("Failed to initial StageRangking");
		return false;	
	}
	if (mWorkerArray[0].loadServerData() != LynxErrno::None)
	{
		LOG_ERROR("Failed to initial loadServerData");
		return false;	
	}

	if (mWorkerArray[0].loadServerDailyData() != LynxErrno::None)
	{
		LOG_ERROR("Failed to initial loadServerDailyData");
		return false;	
	}
	
	
	if (mWorkerArray[0].loadResetStages() != LynxErrno::None)
	{
		LOG_ERROR("Failed to initial ResetStages");
		return false;	
	}
	
	PersistResetAllLeaveTime leavetime;
	leavetime.m_nLeaveTime = time(0) *1000;
	PersistSystem::getSingleton().postThreadMsg(leavetime,1);
	return true;
}

void 
PersistSystem::release()
{
    for (UInt32 i = 0; i < PERSIST_THREAD_WORKER_COUNT; ++i)
    {
        mWorkerArray[i].release();
    }

	m_mapUid2PersistInfo.clear();

    DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistLoadPlayerDataResp);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistFindSimilarPowerResp);
   
  
   
	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistManagerSetDirtyResp);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistNotifyWorkerSaveDbResp);
	
	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistCreatePlayerResp);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistLoadPlayerByAccountResp);
	
	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistInsertRobotResp);

	LOG_INFO("Release PersistSystem");
}

void 
PersistSystem::update(const UInt64& accTime)
{
	for (UInt32 i = 0; i < PERSIST_THREAD_WORKER_COUNT; ++i)
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

	//60000ms测试
	if(accTime - m_nLastSyncTime >= 60000)
	{
		//上次同步时间更新
		m_nLastSyncTime = accTime;
		checkDirtyToSave();
		
	}
}


 void PersistSystem::SaveAllDirty()
 {
	 PersistSystem::checkDirtyToSave();
 }

void PersistSystem::checkDirtyToSave()
{
	Map<UInt64,PersistInfo>::Iter * iter = m_mapUid2PersistInfo.begin();
	while(iter)
	{
		//如果该玩家存盘操作进行中
		if(iter->mValue.mProcessState == PROCESSBUSY)
		{
			iter = m_mapUid2PersistInfo.next(iter);
			continue;
		}

		//如果检查次数超过6次
		if(iter->mValue.mCheckTimes >= 6)
		{
			iter = m_mapUid2PersistInfo.erase(iter);
			continue;
		}

		Player * player = LogicSystem::getSingleton().getPlayerByGuid(iter->mKey);
		if(!player)
		{
			iter = m_mapUid2PersistInfo.next(iter);
			continue;
		}

		bool needDel = player->isToDel();

		if(!iter->mValue.mDirtyBit && !needDel)
		{
			iter = m_mapUid2PersistInfo.next(iter);
			continue;
		}

		iter->mValue.mCheckTimes++;
		iter->mValue.mProcessState = PROCESSBUSY;

		PersistNotifyWorkerSaveDbReq saveDbReq;
		saveDbReq.mPlayerUid = iter->mValue.mPlayerUid;
		saveDbReq.mDirtyBit = saveDbReq.mDirtyBit | iter->mValue.mDirtyBit;
		postThreadMsg(saveDbReq,(UInt16)saveDbReq.mPlayerUid);
		//发送给worker后，该标记清零，方便新的数据设置
		iter->mValue.mDirtyBit = 0;
		iter = m_mapUid2PersistInfo.next(iter);
	}

}


bool 
PersistSystem::loadDatabaseData()
{
    String ip = ConfigParser::getSingleton().getMainDbIp();
    UInt16 port = ConfigParser::getSingleton().getMainDbPort();
    String username = ConfigParser::getSingleton().getMainDbUsername();
    String password = ConfigParser::getSingleton().getMainDbPassword();
    String dbname = ConfigParser::getSingleton().getMainDbName();

    DBInterface dbInterface;

    if (!dbInterface.initial(ip.c_str(), port, username.c_str(),
        password.c_str(), dbname.c_str()))
    {
        LOG_WARN("Failed to connect mysql server. [%s %s %s %s]",
            ip.c_str(), username.c_str(), password.c_str(), dbname.c_str());
        return false;
    }

    if (!loadGuidData(dbInterface))
    {
        LOG_WARN("Failed to load guid table data.");
        return false;
    }
	if (!loadSqlTime(dbInterface))
	{
		LOG_WARN("sqltime localtime  over 60s");
// 		return false;

	}

    dbInterface.release();
    return true;
}

bool 
PersistSystem::loadSqlTime(DBInterface& dbInterface)
{
	char sql[2048];
	snprintf(sql, sizeof(sql), "select UNIX_TIMESTAMP(NOW())");
	bool result = dbInterface.execSql(sql);
	MYSQL_RES* rs = dbInterface.storeResult();
	if(!result)
	{  
		LOG_WARN("Failed to load baseGuid!");
		return false;
	}
	MYSQL_ROW row = dbInterface.fetchRow(rs);
	if(row)
	{
		UInt64 sqlTime = lynxAtoi<UInt64>(row[0]);	

		UInt64 nowTime = TimeUtil::getTimeSec();
		if	(( nowTime -sqlTime) >60 || (nowTime - sqlTime) <-60)
		{
			return false;
		}
		
		return true;
	}
	dbInterface.freeResult(&rs);

	return true;
}

bool 
PersistSystem::loadGuidData(DBInterface& dbInterface)
{
    char sql[2048];
    snprintf(sql, sizeof(sql), "call BaseGuid_Load()");
    bool result = dbInterface.execSql(sql);
    MYSQL_RES* rs = dbInterface.storeResult();
    if(!result)
    {  
        LOG_WARN("Failed to load baseGuid!");
        return false;
    }
    MYSQL_ROW row = dbInterface.fetchRow(rs);
    if(row)
    {
        Guid playerGuid = lynxAtoi<Guid>(row[0]);
		//测试县注释掉，一会该回来
		Guid itemGuid = lynxAtoi<Guid>(row[1]);		
		Guid lansquenetBaseGuid = lynxAtoi<Guid>(row[2]); 
		Guid guildBaseGuid = lynxAtoi<Guid>(row[3]);
		Guid emailBaseGuid = lynxAtoi<Guid>(row[4]);
		Guid materialBaseGuid = lynxAtoi<Guid>(row[5]);
		Guid charactorBaseGuid = lynxAtoi<Guid>(row[6]);
		Guid consortBaseGuid = lynxAtoi<Guid>(row[7]);
		Guid consortTicketGuid = lynxAtoi<Guid>(row[8]);

        LogicSystem::getSingleton().setPlayerGuid(playerGuid);
		LogicSystem::getSingleton().setItemGuid(itemGuid);
		LogicSystem::getSingleton().setLansquenetGuid(lansquenetBaseGuid);
		LogicSystem::getSingleton().setGuildGuid(guildBaseGuid);
		LogicSystem::getSingleton().setEmailGuid(emailBaseGuid);
		LogicSystem::getSingleton().setMaterialGuid(materialBaseGuid);
		LogicSystem::getSingleton().setCharactorGuid(charactorBaseGuid);
		LogicSystem::getSingleton().setConsortGuid(consortBaseGuid);
		LogicSystem::getSingleton().setTicketGuid(consortTicketGuid);

        dbInterface.freeResult(&rs);
        return true;
    }
    dbInterface.freeResult(&rs);

	LogicSystem::getSingleton().setPlayerGuid(10000);
	LogicSystem::getSingleton().setItemGuid(10000);
	LogicSystem::getSingleton().setLansquenetGuid(10000);
	LogicSystem::getSingleton().setGuildGuid(10000);
	LogicSystem::getSingleton().setMaterialGuid(10000);
	LogicSystem::getSingleton().setCharactorGuid(10000);
	LogicSystem::getSingleton().setConsortGuid(10000);
	LogicSystem::getSingleton().setTicketGuid(10000);
    return true;
}


void 
PersistSystem::dispatchThreadMsg(ThreadMsg& msg)
{
	mThreadMsgHandler.onDispatchMsg(msg);
}

void 
PersistSystem::onPersistLoadPlayerDataResp(PersistLoadPlayerDataResp& msg)
{
    if (msg.merrorId == LynxErrno::None)
    {
        LogicSystem::getSingleton().loadPlayerDataSuccess(msg.mConnId, msg.mPlayerData);
    }
    else
    {
        LogicSystem::getSingleton().loadPlayerDataFail(msg.mConnId, msg.mPlayerGuid);
    }
}

void 
PersistSystem::onPersistFindSimilarPowerResp(PersistFindSimilarPowerResp& msg)
{
	Guid otherPlayerID = 0;
	String playerUidStr;
	char dest[64] = {};

	for(UInt32 i =0;i<msg.keyValueList.size();i++)
	{
		otherPlayerID =  LogicSystem::getSingleton().listRandOne(msg.playerID,msg.keyValueList,msg.times,0);
		sprintf(dest,"%llu",otherPlayerID);
		playerUidStr = (String)dest;	
		if(LogicSystem::getSingleton().getDetailInfo(playerUidStr.c_str()) != "")
		{
			break;
		}
	}

	LOG_INFO("onPersistFindSimilarPowerResp otherPlayerID = %llu  ",otherPlayerID);
	if (msg.times == 10001)
	{
		PVPSystem::getSingleton().matchingRoomResp(msg.playerID,otherPlayerID);
	}
	else
	{
		LogicSystem::getSingleton().dealSimililarPower(msg.playerID,otherPlayerID,msg.times);
	}

	
}


void 
PersistSystem::onPersistLoadOfflineDataResp(PersistLoadOfflineDataResp& msg)
{
// 	if (msg.mBaseData != NULL)
	{

	}
}

void PersistSystem::onPersistLoadPlayerByAccountResp(PersistLoadPlayerByAccountResp &msg)
{
		GCAccountLoginResp accountLoginResp;

		accountLoginResp.mPacketID = BOC_ACCOUNT_LOGIN_RESP;
		Json::Value root;
		root["errorId"] = msg.mErrorId;
		root["accountId"] = msg.mAccountId;
		root["playeruid"] = msg.mPlayerUid;
		Json::StyledWriter writer;
		accountLoginResp.mRespStr = writer.write(root);
		LOG_INFO("onPersistLoadPlayerByAccountResp = %s",accountLoginResp.mRespStr.c_str());
		NetworkSystem::getSingleton().sendMsg(accountLoginResp,msg.mConnId);
		
}


void 
PersistSystem::onPersistSetDirtyResp(PersistManagerSetDirtyResp & msg)
{
	Map<UInt64,PersistInfo>::Iter *findIter = m_mapUid2PersistInfo.find(msg.mPlayerUid);

	if(findIter)
	{
		findIter->mValue.mDirtyBit = findIter->mValue.mDirtyBit | msg.mDirtyBit;

	}
	else
	{
		PersistInfo persistInfo;
		persistInfo.mPlayerUid = msg.mPlayerUid;
		persistInfo.mDirtyBit = persistInfo.mDirtyBit | msg.mDirtyBit;
		persistInfo.mCheckTimes = 0;
		persistInfo.mProcessState = PROCESSFREE;

		m_mapUid2PersistInfo.insert(persistInfo.mPlayerUid, persistInfo);
		
	}
	
}


void PersistSystem::onPersistNotifyWorkerSaveDbResp(PersistNotifyWorkerSaveDbResp & msg)
{
	errorId errorId = msg.merrorId;
	UInt64 playerUid = msg.mPlayerUid;
	UInt32 dirtyBit = msg.mDirtyBit;
	
	if(errorId == LynxErrno::None)
	{
		//persistworker 处理成功的情况下
		Map<UInt64,PersistInfo>::Iter * iterFind = m_mapUid2PersistInfo.find(playerUid);
		iterFind->mValue.mCheckTimes = 0;
		iterFind->mValue.mProcessState = PROCESSFREE;
	}
	else
	{
		//persistworker 处理失败的情况下
		Map<UInt64,PersistInfo>::Iter * iterFind = m_mapUid2PersistInfo.find(playerUid);
		iterFind->mValue.mDirtyBit = iterFind->mValue.mDirtyBit | msg.mDirtyBit;
		iterFind->mValue.mProcessState = PROCESSFREE;

	}
	
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(msg.mPlayerUid);
	if(player)
	{
		bool needDel = player->isToDel();
		if(needDel)
		{
			LogicSystem::getSingleton().destroyPlayerByGuid(msg.mPlayerUid);
		}
	}
	
	
}


void PersistSystem::onPersistCreatePlayerResp(PersistCreatePlayerResp & msg)
{
	GCGameCreatePlayerResp createPlayerResp;
	createPlayerResp.mPacketID = BOC_GAME_CREATEPLAYER_RESP;
	Json::Value root;
	root["errorId"] = msg.mRes;
	root["uuid"] = msg.mPlayerUid;

	Json::FastWriter writer;
	std::string respStr = writer.write(root);

	createPlayerResp.mRespStr = respStr;

	LogicSystem::getSingleton().setPlayerGuid(msg.mPlayerUid);

	NetworkSystem::getSingleton().sendMsg(createPlayerResp,msg.mConnId);

	LOG_INFO("createend %llu !",TimeUtil::getTimeMilliSec());

	if (msg.mRes == LynxErrno::None)
	{
		LogicSystem::getSingleton().initRoleInfoReq(msg.mPlayerUid,msg.mName.c_str(),msg.mPlayerModelId);

		char dest[1024]={0};
		snprintf(dest,sizeof(dest),"%llu",msg.mPlayerUid);

		LogicSystem::getSingleton().write_server_log(LogType51,dest,LogInfo);


	}
	else
	{
		LogicSystem::getSingleton().eraseCreateSet(msg.mName.c_str());
	}
	

	

}


void PersistSystem::onPersistInsertRobotResp(PersistInsertRobotResp & msg)
{	
	GameServer::getSingleton().gameExit();
}