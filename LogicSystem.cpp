#include "LogicSystem.h"
#include "GameServer.h"
#include "RedisManager.h"
#include "LoginMsgHandler.h"
#include "RankSystem.h"
#include "PKSystem/PVPSystemWC.h"
#include "FireConfirm/Shop.h"
using namespace Lynx;

LogicSystem::LogicSystem() : mPlayerGuidBase(0), mServerId(0),mLastSyncGuidBaseTime(0),mItemGuidBase(0),
mLansquenetGuid(0),mGuildGuid(0),mEmailGuid(0),mMaterialGuid(0), mConsortGuid(0),mTicketGuid(0),
mLastRestGuidBaseTime(0),mForbidLogin(0),mLastCheckOnlineWelfareTime(0)
{

}

LogicSystem::~LogicSystem()
{

}

bool 
LogicSystem::initial()
{
	mServerId = ConfigParser::getSingleton().getServerId();

	REGISTER_EVENT(EVENT_CLIENT_CONNECTION_CLOSE, &LogicSystem::onClientConnectionClosed);
	REGISTER_EVENT(EVENT_CLIENT_CONNECTION_NOT_EXIST, &LogicSystem::onClientConnectionNotExist);
	initAllAchieve();
	initAllDailyTask();
	LOG_INFO("Initial LogicSystem");
   
	


	return true;
}

void LogicSystem::initAllAchieve()
{
	const FirstAchievementMap& firstAchMap = ACHIEVEMENT_TABLE().getFirstAchMap();
	addAchieveVec(firstAchMap,BARRIER);
	addAchieveVec(firstAchMap,ELITEBARRIER);
	addAchieveVec(firstAchMap,RHYMEENHANCELV);
	addAchieveVec(firstAchMap,RHYMESKILLACTIVE);
	addAchieveVec(firstAchMap,HONORSTONEACTIVE);
	addAchieveVec(firstAchMap,GETSERVANT);
	addAchieveVec(firstAchMap,PLAYERLV);
	addAchieveVec(firstAchMap,VIPLV);
	addAchieveVec(firstAchMap,POWER);
	addAchieveVec(firstAchMap,OWN4STARSERVANT);
	addAchieveVec(firstAchMap,OWNPURPULESERVANT);
	addAchieveVec(firstAchMap,UNLOCKSERVANTASSIS);
	addAchieveVec(firstAchMap,OWNGREENASSIST);
	
	addAchieveVec(firstAchMap,QUALIFYDANLV);
	addAchieveVec(firstAchMap,QUALIFYSUCCESS);
	addAchieveVec(firstAchMap,ARIEASUCCESS);

	addAchieveVec(firstAchMap,BUYCHARACTOR);
	addAchieveVec(firstAchMap,COURAGESUCCESS);
	addAchieveVec(firstAchMap,WXCHALLENGESUC);
	addAchieveVec(firstAchMap,USERHYMESKILL);
	addAchieveVec(firstAchMap,USERHYMESPEED);
	addAchieveVec(firstAchMap,CALLSERVANTCNT);
	addAchieveVec(firstAchMap,FRIENDCNT);

	addAchieveVec(firstAchMap,MAXGEMLV);
	addAchieveVec(firstAchMap,LINGRENCALLSERVANT);
	addAchieveVec(firstAchMap,XIBANCALLSERVANT);
	addAchieveVec(firstAchMap,MINGLINGCALLSERVANT);
	addAchieveVec(firstAchMap,OWNBLUEASSIST);
	addAchieveVec(firstAchMap,OWNPURPLEASSIST);

}

void LogicSystem::initAllDailyTask()
{
	DailyTaskTempMap* dailyTaskMap = DAILYTASK_TABLE().getDailyTaskMap();
	addDailyTaskVec(dailyTaskMap,DLYBARRIERCNT );
	addDailyTaskVec(dailyTaskMap,DLYELITEBARRIRECNT );
	addDailyTaskVec(dailyTaskMap,DLYLINGRENCALLCNT );
	addDailyTaskVec(dailyTaskMap,DLYXIBANCALLCNT );

	addDailyTaskVec(dailyTaskMap,DLYMULTYSUCCESS );
	addDailyTaskVec(dailyTaskMap,DLYTELVESUCCESS );
	addDailyTaskVec(dailyTaskMap,DLYDOMARKETCNT );

	addDailyTaskVec(dailyTaskMap,DLYDOCOURAGECNT );
	addDailyTaskVec(dailyTaskMap,DLYDOWXCHALLENGE );
	addDailyTaskVec(dailyTaskMap,DLYDOQUALIFYCNT );
		
	addDailyTaskVec(dailyTaskMap,DLYDOARIEACNT );
	addDailyTaskVec(dailyTaskMap,DLYRHYMEENHANCECNT );
	addDailyTaskVec(dailyTaskMap,DLYHORNORSTONELVUPCNT );	

	addDailyTaskVec(dailyTaskMap,DLYBAICAISHEN );
	addDailyTaskVec(dailyTaskMap,DLYBUYENERGE );
	addDailyTaskVec(dailyTaskMap,DLYFOODCOOK );	

		     
	addDailyTaskVec(dailyTaskMap,DLYCONSORTSIGN );
	
}

const Vector<AchieveData>& LogicSystem::getAchieveVec()
{
	return mAllAchiveInit;
}

const Vector<DailyTaskData> & LogicSystem::getDailyTaskVec()
{
	return mAllDailyTaskInit;
}

void LogicSystem::addAchieveVec(const FirstAchievementMap& firstAchMap, UInt64 eventType)
{
	AchieveData achdata;
	achdata.m_nEventId = eventType;
	Map<UInt64,  AchievementTemplate>::Iter * findIter = firstAchMap.find(eventType);
	if(findIter)
	{
		achdata.m_nAchieveId = findIter->mValue.mId;
		mAllAchiveInit.push_back(achdata);
	}
	else
	{
		achdata.m_nEventId = 0;
		achdata.m_nAchieveId = 0;
		mAllAchiveInit.push_back(achdata);
	}
	
}

void LogicSystem::addDailyTaskVec(DailyTaskTempMap* dailyTaskMap, UInt64 eventType)
{
			DailyTaskData dailyTaskData;
			dailyTaskData.m_nEventId = eventType;
			for(DailyTaskTempMap::Iter * dailyIter = dailyTaskMap->begin();   
				dailyIter != NULL; dailyIter = dailyTaskMap->next(dailyIter) )
			{
				if(dailyIter->mValue.mEvent == eventType)
				{
					dailyTaskData.m_nTempId = dailyIter->mValue.mId;
				}
			}

		mAllDailyTaskInit.push_back(dailyTaskData);
}


void
LogicSystem::release()
{
	DEREGISTER_EVENT(EVENT_CLIENT_CONNECTION_CLOSE, &LogicSystem::onClientConnectionClosed);
	DEREGISTER_EVENT(EVENT_CLIENT_CONNECTION_NOT_EXIST, &LogicSystem::onClientConnectionNotExist);

	for (Map<Guid, Player*>::Iter* iter = mPlayerGuidMap.begin();
		iter != NULL; iter = mPlayerGuidMap.next(iter))
	{
		Player* player = iter->mValue;
		if (player)
		{
			player->release();
			XDELETE(player);
		}
	}
	mPlayerConnectionMap.clear();
	mPlayerGuidMap.clear();
	mPlayerNameMap.clear();
	mAccountGuidMap.clear();
	mAccountNameMap.clear();

	//mArenaRobotMap.clear();
	//mArenaPlayerInfoMap.clear();
	//XSAFE_DELETE(mArenaPlayerArray);
	//XSAFE_DELETE(mArenaRobotData);

	LOG_INFO("Release LogicSystem");
}

//验证账号密码.验证名称，更新数据库，load，send
// void LogicSystem::onCreateNewPlayer(string playerName))
// {
// 
// }

void LogicSystem::dailyTaskRefresh(const UInt64& accTime)
{
	//检测在线玩家每日任务成就重置刷新情况,对于要刷新的玩家发送刷新通知
	for(Map<ConnId, Player*>::Iter * playerIter = mPlayerConnectionMap.begin(); playerIter != NULL;  
		playerIter = mPlayerConnectionMap.next(playerIter))
	{

		bool needNotify = checkReset(accTime/1000,playerIter->mValue);
		if(needNotify)
		{
			GCAchieveDailyListResp achieveDailyListResp;
			achieveDailyListResp.mPacketID = BOC_ACHIEVEDAILYLIST_RESP;
			achieveDailyListResp.mRespJsonStr = playerIter->mValue->getAchieveManager().convertAchieveDataToJson();
			//cout << achieveDailyListResp.mRespJsonStr;
			NetworkSystem::getSingleton().sendMsg(achieveDailyListResp, playerIter->mKey);
		}
	}
}





void 
LogicSystem::update(const UInt64& accTime)
{

	// 更新Player
	Vector<Guid> destroyPlayerList;
	for (Map<Guid, Player*>::Iter* iter = mPlayerGuidMap.begin();
		iter != NULL; iter = mPlayerGuidMap.next(iter))
	{
		Player* player = iter->mValue;
		if (player)
		{
			player->update(accTime);
			UInt64 connectCloseTime = player->getConnectCloseTime();

			if (connectCloseTime)
			{
				if (accTime - connectCloseTime > OFFLINESAVETIME * 1000)
				{
					destroyPlayerList.push_back(player->getPlayerGuid());										
				}
			}
		}
	}

	for (Vector<Guid>::iterator iterVec = destroyPlayerList.begin();
		iterVec != destroyPlayerList.end(); ++iterVec)
	{
		//下线前存盘

		Player* player = getPlayerByGuid(*iterVec);
		if (!player)
		{
			LOG_WARN("Failed to destroy player [%llu] for player isn't exist.", *iterVec);
			continue;
		}

		//为了节约资源，避免重复投递消息，因为消息如果没有被及时处理，下次update还会从map中找到该玩家
		UInt16 postState = player->getLastPostState();
		if(postState)
		{
			continue;
		}

		onPersistOffLineSaveReq(*iterVec);
		
		
	}


	// 同步GUIDBase
	if (accTime - mLastSyncGuidBaseTime >= SAVE_INV_TIME)//这个5000下面也要用到
	{

		mLastSyncGuidBaseTime = accTime;
		baseUidPersist();

		dailyRefresh(accTime);

		ClimbTowerManager::getSingleton().updateClimTower(0);

		RankGameManager::getSingleton().resetRankGameRewardFlag();

		ServerData::getSingleton().saveOnlinePlayerGuid( mPlayerGuidMap);



	}	

	if (accTime - mLastSyncGuidBaseTime >= SAVE_ONLINE_NUM_TIME)//在线人数
	{
		mLastSyncGuidBaseTime = accTime;

		char dest2[1024];
		UInt32 onlineNum = mPlayerGuidMap.size();;

		snprintf(dest2,sizeof(dest2),"%d",onlineNum);
		LogicSystem::getSingleton().write_server_log(LogType59,dest2,LogInfo);
	}

	if(accTime - mLastRestGuidBaseTime > RESET_INV_TIME )
	{
		mLastRestGuidBaseTime = accTime;
		//成就系统每日活动检测刷新通知
		dailyTaskRefresh(accTime);
	}


	//判断GM消息
	for(List<GMMsg>::Iter * gmIter = mGMMsgList.begin();
		gmIter != NULL; )
	{
		//过期了，就移除
		if((gmIter->mValue.m_nEndTime ) *1000<=  accTime)
		{
			gmIter = mGMMsgList.erase(gmIter);
		}
		else
		{
			//超过时间间隔，那么发送消息
			if(accTime/1000 - gmIter->mValue.m_nLastTime  > gmIter->mValue.m_nTerminal)
			{
				//更新最后一次发送时间
				gmIter->mValue.m_nLastTime =  accTime/1000;
				//发送消息

				const Map<ConnId, Player*> connectionMap = LogicSystem::getSingleton().getPlayerConnectionMap();

				for( Map<ConnId, Player*>::Iter* mapIter = connectionMap.begin(); mapIter != NULL; 
					mapIter = connectionMap.next(mapIter))
				{

					GCClientChatResp clientChatResp;
					clientChatResp.mPacketID = BOC_CLIENTCHAT_RESP;

					Json::Value root;
					root["channelType"] = SYSTEMMSG;
					root["fromPlayer"] = 0;
					root["chatMsg"] = gmIter->mValue.m_strGMMsg;
					root["modelid"] =0;
					root["name"] = "";
					root["chattime"] = TimeUtil::getTimeSec();
					root["msgType"] = 1;

					Json::StyledWriter writer;

					clientChatResp.mRespJsonStr = writer.write(root);

					cout << clientChatResp.mRespJsonStr << endl;

					NetworkSystem::getSingleton().sendMsg(clientChatResp, mapIter->mKey);


				}


			}


			gmIter = mGMMsgList.next(gmIter);
		}
	}
	
	if(accTime - mLastCheckOnlineWelfareTime > CHECK_ONLEWELFARE_TIME )
	{
		mLastCheckOnlineWelfareTime = accTime;
		for (Map<Guid, Player*>::Iter* iter = mPlayerGuidMap.begin();	iter != NULL; iter = mPlayerGuidMap.next(iter))
		{
			InlineActivityManager::getSingleton().checkOnlineWelfareLastTime(iter->mValue);
		}
	}
}

void LogicSystem::baseUidPersist()
{
	PersistSyncBaseGuidNotify notify;
	notify.mPlayerGuid = mPlayerGuidBase;
	notify.mItemGuid = mItemGuidBase;
	notify.mLansquenetGuid = mLansquenetGuid;
	notify.mGuildGuid = mGuildGuid;
	notify.mEmailGuid = mEmailGuid;
	notify.mMaterialGuid = mMaterialGuid;
	notify.mCharactorGuid = mCharactorGuid;
	notify.mConsortGuid = mConsortGuid;
	notify.mTicketGuid = mTicketGuid;
	PersistSystem::getSingleton().postThreadMsg(notify, 0);
}

UInt32  LogicSystem::getRefreshTime(UInt32 systemID)
{
	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();
	for(List<UInt32>::Iter *iter = globalValue.uSystemRefreshTime.begin();iter!= NULL;iter = globalValue.uSystemRefreshTime.next(iter))
	{	
		iter = globalValue.uSystemRefreshTime.next(iter);
		if (iter->mValue == systemID)
		{		
			iter = globalValue.uSystemRefreshTime.prev(iter);
			return iter->mValue;
		}	
	}
	return 0;
}

void  LogicSystem::getDailyRefreshList(UInt32 refreshTime,List<UInt32> &refreshList)
{
	
	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();
	for(List<UInt32>::Iter *iter = globalValue.uSystemRefreshTime.begin();iter!= NULL;iter = globalValue.uSystemRefreshTime.next(iter))
	{
		if (iter->mValue == refreshTime)
		{
			iter = globalValue.uSystemRefreshTime.next(iter);
			refreshList.insertTail(iter->mValue);
		}
		else
		{
			iter = globalValue.uSystemRefreshTime.next(iter);
		}
	}

	
}


//每天0时4时
void 
LogicSystem::dailyRefresh(const UInt64& accTime)
{	
	UInt32 nowSec = TimeUtil::getSecOfToday();
	UInt32 flag = 10000;
	UInt32 refreshTime =0;
	List<UInt32>refreshList;

	flag = nowSec/3600;

	if (nowSec >= flag*3600 && nowSec < (flag*3600 + SAVE_INV_TIME/1000))//每天整点
	{
		refreshTime = flag*3600;
		getDailyRefreshList(refreshTime,refreshList);
	}
	else
	{
		return;
	}

	if (refreshList.size() == 0)
	{
		return;
	}

	DailyResetResp resp;
	resp.refreshList = refreshList;
	resp.serverTime = TimeUtil::getTimeSec();
	resp.mRespJsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().broadcastMsg(resp);



	for(Map<Guid, Player*>::Iter* iter = mPlayerGuidMap.begin();iter != NULL; iter = mPlayerGuidMap.next(iter))
	{
		if (iter->mValue != NULL )
		{
			LogicSystem::getSingleton().updateDailyResetData(iter->mValue,flag,refreshList);
		}			
	}


}
bool 
LogicSystem::reloadTable(const String& tableName)
{
	if (!TableManager::getSingleton().reload(tableName))
	{
		LOG_WARN("Failed to reload template table %s", tableName.c_str());
		return false;
	}
	return true;
}

bool 
LogicSystem::reloadAllTable()
{
	if (!TableManager::getSingleton().reloadAll())
	{
		LOG_WARN("Failed to reload all template tables.");
		return false;
	}
	return true;
}

Player* 
LogicSystem::createPlayer(const ConnId& connId, const PlayerData& playerData, 
						  const UInt64& loginToken)
{

	Player* player = getPlayerByConnId(connId);
	if (player)
	{
		LOG_WARN("Failed to create player for player connid [%llu] has exist.", connId);
		return NULL;
	}

	player = getPlayerByGuid(playerData.mBaseData.m_nPlayerID);
	if (player)
	{
		LOG_WARN("Failed to create player for player guid [%llu] has exist.", 
			playerData.mBaseData.m_nPlayerID);
		return NULL;
	}

	player = getPlayerByName(playerData.mBaseData.m_strPlayerName.c_str());
	if (player)
	{
		LOG_WARN("Failed to create player for player name [%s] has exist.", 
			playerData.mBaseData.m_strPlayerName.c_str());
		return NULL;
	}

	//sec 目前没有关于account的需求 先注释
	/*player = getPlayerByAccountName(playerData.mBaseData.mAccountName.c_str());
	if (player)
	{
	LOG_WARN("Failed to create player for player account name [%s] has exist.", 
	playerData.mBaseData.mAccountName.c_str());
	return NULL;
	}

	player = getPlayerByAccountGuid(playerData.mBaseData.mAccountGuid);
	if (player)
	{
	LOG_WARN("Failed to create player for player account guid [%llu] has exist.", 
	playerData.mBaseData.mAccountGuid);
	return NULL;
	}*/

	player = XNEW(Player)();
	if (!player->initial(connId, playerData, loginToken))
	{
		LOG_WARN("Failed to create player for initial failed.");
		XDELETE(player);
		return NULL;
	}

	mPlayerConnectionMap.insert(connId, player);
	mPlayerGuidMap.insert(player->getPlayerGuid(), player);
	mPlayerNameMap.insert(player->getPlayerName().c_str(), player);
	//mAccountGuidMap.insert(player->getAccountGuid(), player);
	//mAccountNameMap.insert(player->getAccountName().c_str(), player);

	return player;


}

void 
LogicSystem::destroyPlayerByGuid(const Guid& guid)
{

	Player* player = getPlayerByGuid(guid);
	if (!player)
	{
		LOG_WARN("Failed to destroy player [%llu] for player isn't exist.", guid);
		return;
	}

	//新添加的，考虑到某人掉线过了一个小时，经过最后存盘后加入到删除列表，在删除之前又上线了，不要误删此玩家，直接返回。
	if(!player->getConnectCloseTime())
	{
		return;
	}

	ConnId connId = player->getConnId();
	if (connId)
	{
		pushEvent(EVENT_CLIENT_CONNECTION_CLOSE, connId);
		LOG_INFO(" save player and destroyPlayerByGuid connID = %d playerID =[%llu]  ", guid);
	}

	mPlayerGuidMap.erase(player->getPlayerGuid());
	mPlayerConnectionMap.erase(player->getConnId());
	mPlayerNameMap.erase(player->getPlayerName().c_str());
	/*  mAccountGuidMap.erase(player->getAccountGuid());
	mAccountNameMap.erase(player->getAccountName().c_str());*/
	player->release();
	XDELETE(player);

}

//测试功能，其他人不要使用 
//王戊辰

void LogicSystem::debugTestDestroyPlayer(const Guid& guid)
{
	Player* player = getPlayerByGuid(guid);
	if (!player)
	{
		LOG_WARN("Failed to destroy player [%llu] for player isn't exist.", guid);
		return;
	}

	mPlayerGuidMap.erase(player->getPlayerGuid());
	mPlayerConnectionMap.erase(player->getConnId());
	mPlayerNameMap.erase(player->getPlayerName().c_str());
	/*  mAccountGuidMap.erase(player->getAccountGuid());
	mAccountNameMap.erase(player->getAccountName().c_str());*/
	player->release();
	XDELETE(player);
}


void 
LogicSystem::destroyPlayerByConnId(const ConnId& connId)
{

	Player* player = getPlayerByConnId(connId);
	if (!player)
	{
		LOG_WARN("Failed to destroy player [%llu] for player isn't exist.", connId);
		return;
	}

	mPlayerGuidMap.erase(player->getPlayerGuid());
	mPlayerConnectionMap.erase(player->getConnId());
	mPlayerNameMap.erase(player->getPlayerName().c_str());
	/*  mAccountGuidMap.erase(player->getAccountGuid());
	mAccountNameMap.erase(player->getAccountName().c_str());*/
	player->release();
	XDELETE(player);

}

void 
LogicSystem::destroyPlayerByName(const String& playerName)
{

	Player* player = getPlayerByName(playerName);
	if (!player)
	{
		LOG_WARN("Failed to destroy player [%s] for player isn't exist.", playerName.c_str());
		return;
	}

	mPlayerGuidMap.erase(player->getPlayerGuid());
	mPlayerConnectionMap.erase(player->getConnId());
	mPlayerNameMap.erase(player->getPlayerName().c_str());
	/*  mAccountGuidMap.erase(player->getAccountGuid());
	mAccountNameMap.erase(player->getAccountName().c_str());*/
	player->release();
	XDELETE(player);

}

void 
LogicSystem::destroyPlayerByAccountId(const Guid& accountId)
{

	Player* player = getPlayerByAccountGuid(accountId);
	if (!player)
	{
		LOG_WARN("Failed to destroy player account id [%llu] for player isn't exist.", accountId);
		return;
	}

	mPlayerGuidMap.erase(player->getPlayerGuid());
	mPlayerConnectionMap.erase(player->getConnId());
	mPlayerNameMap.erase(player->getPlayerName().c_str());
	/*  mAccountGuidMap.erase(player->getAccountGuid());
	mAccountNameMap.erase(player->getAccountName().c_str());*/
	player->release();
	XDELETE(player);

}

void 
LogicSystem::destroyPlayerByAccountName(const String& accountName)
{

	Player* player = getPlayerByAccountName(accountName);
	if (!player)
	{
		LOG_WARN("Failed to destroy player account name [%s] for player isn't exist.", accountName.c_str());
		return;
	}

	mPlayerGuidMap.erase(player->getPlayerGuid());
	mPlayerConnectionMap.erase(player->getConnId());
	mPlayerNameMap.erase(player->getPlayerName().c_str());
	/*  mAccountGuidMap.erase(player->getAccountGuid());
	mAccountNameMap.erase(player->getAccountName().c_str());*/
	player->release();
	XDELETE(player);

}

Player* 
LogicSystem::getPlayerByConnId(const ConnId& connId)
{
	Map<ConnId, Player*>::Iter* iter = mPlayerConnectionMap.find(connId);
	if (!iter)
	{
		return NULL;
	}
	return iter->mValue;
}

Player* 
LogicSystem::getPlayerByGuid(const Guid& guid)
{
	Map<Guid, Player*>::Iter* iter = mPlayerGuidMap.find(guid);
	if (!iter)
	{
		return NULL;
	}
	return iter->mValue;
}

Player* 
LogicSystem::getPlayerByName(const String& playerName)
{
	StringMap<Player*>::Iter* iter = mPlayerNameMap.find(playerName.c_str());
	if (!iter)
	{
		return NULL;
	}
	return iter->mValue;
}

Player* 
LogicSystem::getPlayerByAccountGuid(const Guid& guid)
{
	Map<Guid, Player*>::Iter* iter = mAccountGuidMap.find(guid);
	if (!iter)
	{
		return NULL;
	}
	return iter->mValue;
}

Player* 
LogicSystem::getPlayerByAccountName(const String& accountName)
{
	StringMap<Player*>::Iter* iter = mAccountNameMap.find(accountName.c_str());
	if (!iter)
	{
		return NULL;
	}
	return iter->mValue;
}

void 
LogicSystem::onClientConnectionClosed(const Event& ev)
{
	if (ev.mEventArgs.empty())
	{
		LOG_WARN("Event EVENT_CLIENT_CONNECTION_CLOSE args is NULL");
		return;
	}
	ConnId connId = lynxAtoi<ConnId>(ev.mEventArgs[0].c_str());

	Player* player = getPlayerByConnId(connId);
	if (!player)
	{
		//LOG_DEBUG("Failed to get player by connId: %llu", connId);
		return;
	}

	mPlayerConnectionMap.erase(connId);
	player->onClientConnectClosed();
	//添加pvp掉线处理

	PVPSystemWC::getSingleton().dealConClose(player->getPlayerGuid());
}

void 
LogicSystem::onClientConnectionNotExist(const Event& ev)
{

}


void 
LogicSystem::closeConnection(Player* player)
{

	if (player->getConnId() == 0)
	{
		return;
	}
	const ConnId& connId =player->getConnId();
	LOG_INFO("connId is = %llu",connId);
	//mPlayerConnectionMap.erase(connId);
	//player->onClientConnectClosed();
	AccountWasLoginNotify notify;
	Json::Value root;
	root["type"] = 1;
	Json::StyledWriter writer;
	std::string jstr = writer.write(root);
	notify.mRespJsonStr = jstr;
	NetworkSystem::getSingleton().sendMsg(notify, connId);
	LOG_INFO("JSON IS = %s",notify.mRespJsonStr.c_str());
	//pushEvent(EVENT_CLIENT_CONNECTION_CLOSE, connId);

}

void LogicSystem::insertCreateSet(String name)
{
	
	mCreatePlayerNameSet.insert(name);
}

bool LogicSystem::isInCreateSet(String name)
{
	Set<String>::Iter * findIterName = mCreatePlayerNameSet.find(name);
	if(findIterName)
	{
		return true;
	}

	return false;
}

void LogicSystem::eraseCreateSet(String name)
{
	
	mCreatePlayerNameSet.erase(name);
}


void LogicSystem::updateBaseInfo(const PlayerBaseData &baseData)
{
	Map<UInt64, BaseInfoData>::Iter * baseDataIter = mPlayerBaseInfoMap.find(baseData.m_nPlayerID);
	if(baseDataIter)
	{
		baseDataIter->mValue.name = baseData.m_strPlayerName.c_str();
		baseDataIter->mValue.leaveTime = 0;
		baseDataIter->mValue.playerUid = baseData.m_nPlayerID;
		baseDataIter->mValue.power = baseData.m_nPower;
		baseDataIter->mValue.level = baseData.m_nLevel;
		baseDataIter->mValue.vipLv = baseData.m_nVipLevel;
		baseDataIter->mValue.modelId = baseData.m_nModelID;
	}
	else
	{
		BaseInfoData baseDataInfo;
		baseDataInfo.name = baseData.m_strPlayerName.c_str();
		baseDataInfo.leaveTime = 0;
		baseDataInfo.playerUid = baseData.m_nPlayerID;
		baseDataInfo.power = baseData.m_nPower;

		baseDataInfo.level = baseData.m_nLevel;
		baseDataInfo.vipLv = baseData.m_nVipLevel;

		baseDataInfo.modelId = baseData.m_nModelID;

		mPlayerBaseInfoMap.insert(baseDataInfo.playerUid, baseDataInfo);
	}
}

void LogicSystem::updateBaseInfo(const BaseInfoData & baseInfoData)
{
	Map<UInt64, BaseInfoData>::Iter * baseDataIter = mPlayerBaseInfoMap.find(baseInfoData.playerUid);
	if(baseDataIter)
	{
		baseDataIter->mValue.name = baseInfoData.name.c_str();
		baseDataIter->mValue.leaveTime = baseInfoData.leaveTime;
		baseDataIter->mValue.playerUid = baseInfoData.playerUid;
		baseDataIter->mValue.power = baseInfoData.power;
		baseDataIter->mValue.level = baseInfoData.level;
		baseDataIter->mValue.vipLv = baseInfoData.vipLv;
		baseDataIter->mValue.modelId = baseInfoData.modelId;
	}
	else
	{
		mPlayerBaseInfoMap.insert(baseInfoData.playerUid, baseInfoData);

		LogicSystem::getSingleton().insertCreateSet( baseInfoData.name.c_str());
	}
}

void LogicSystem::updateConsortInfo(UInt64 playerUid, const PlayerConsortData & playerConsortData)
{

	Map<UInt64, ConsortInfoData>::Iter * findIter = mPlayerConsortInfoMap.find(playerUid);

	if(findIter)
	{
		findIter->mValue.playerUid = playerUid;
		findIter->mValue.consortId = playerConsortData.m_nConsortId;
		findIter->mValue.curContribute = playerConsortData.m_nCurContribute;
		findIter->mValue.totalContribute = playerConsortData.m_nTotalContribute;
		findIter->mValue.consortJob = playerConsortData.m_nConsortJob;
			findIter->mValue.leavetime = playerConsortData.m_nLeaveTime;
	}
	else
	{
		ConsortInfoData  consortInfoData;
		consortInfoData.consortId = playerConsortData.m_nConsortId;
		consortInfoData.curContribute = playerConsortData.m_nCurContribute;
		consortInfoData.consortJob = playerConsortData.m_nConsortJob;
		consortInfoData.totalContribute = playerConsortData.m_nTotalContribute;
		consortInfoData.playerUid = playerUid;

			consortInfoData.leavetime = playerConsortData.m_nLeaveTime;
		mPlayerConsortInfoMap.insert(playerUid, consortInfoData);
	}

}

void LogicSystem::updateConsortInfo(UInt64 playerUid, const ConsortInfoData& consortInfoData)
{
	Map<UInt64, ConsortInfoData>::Iter * findIter = mPlayerConsortInfoMap.find(playerUid);

	if(findIter)
	{
		findIter->mValue.playerUid = playerUid;
		findIter->mValue.consortId = consortInfoData.consortId;
		findIter->mValue.curContribute = consortInfoData.curContribute;
		findIter->mValue.totalContribute = consortInfoData.totalContribute;
		findIter->mValue.consortJob = consortInfoData.consortJob;
		findIter->mValue.leavetime = consortInfoData.leavetime;
	}
	else
	{

		mPlayerConsortInfoMap.insert(playerUid, consortInfoData);
	}
}

ConsortInfoData * LogicSystem::getConsortInfo(UInt64 playerUid)
{
	Map<UInt64, ConsortInfoData>::Iter * findIter = mPlayerConsortInfoMap.find(playerUid);
	if(findIter == mPlayerConsortInfoMap.end())
	{
		return NULL;
	}

	return &(findIter->mValue);
}

//处理第二天刷新逻辑是这样的，判断当前时间和上次打点时间是否是同一天，不是同一天判断则需要打点
//复杂的情况是一天的不同时期，0,4,5等等，只需判断当前时间是否大于0,4,5，大于则打点并且更新打点时间
//王戊辰

bool LogicSystem::checkReset(UInt64 curTime, Player * player)
{
	PlayerDailyTaskData & dailytaskData = player->getDailyTaskData();


	bool isToday = TimeManager::timeIsOneDay(curTime, dailytaskData.lastResetTime);

	//当前秒数转化为当天第几点
		UInt64 hours =  TimeUtil::convertSecToTodayHour(curTime);

	UInt32 systemRefreshHour =  LogicSystem::getSingleton().getRefreshTime(TIME_REFRESH_ID03) /3600;
	//不是今天，那么需要重置数据
	if(!isToday && hours >= systemRefreshHour)
	{
		//处理日常任务重置
		for(List<DailyTaskData>::Iter * dailyIter = dailytaskData.dailytaskList.begin(); dailyIter !=NULL; 
			dailyIter =  dailytaskData.dailytaskList.next(dailyIter))
		{
			dailyIter->mValue.m_nArg = 0;
			dailyIter->mValue.m_nFlag = 0;
		}

		dailytaskData.lastResetTime = curTime;

		player->setDailyTaskData(dailytaskData);

		//处理每日活跃度重置
		PlayerDailyActiveData & dailyAcvData = player->getDailyActiveData();
		for(List<UInt32>::Iter * acvIter = dailyAcvData.flags.begin();  acvIter != NULL; acvIter = dailyAcvData.flags.next(acvIter))
		{
			acvIter->mValue = 0;
		}

		dailyAcvData.lastRestTime = curTime;
		dailyAcvData.curActive = 0;

		player->setDailyActiveData(dailyAcvData);
		//通知存盘
		PersistDailyLastTimeUpdateMsg resetMsg;
		resetMsg.playerUid = player->getPlayerGuid();
		resetMsg.time = curTime;
		PersistSystem::getSingleton().postThreadMsg(resetMsg,  resetMsg.playerUid);

	}

	return !isToday;
}

void 
LogicSystem::loadPlayerDataFromMemery(const ConnId& connId, Player* player)
{

	UInt64 loginToken = NetworkSystem::getSingleton().createLoginToken(connId);
	if (loginToken == 0)
	{
		LOG_WARN("Failed to get login token by connId %llu", connId);
		GCGameLoginResp loginFailResp;

		MsgLoginRespToJson loginRespToJson;

		loginRespToJson.result = LynxErrno::LoginTokenFailed;
		loginRespToJson.loginToken = loginToken;
		loginRespToJson.errMsg ="Failed to get login token by connId";
		loginRespToJson.loginMainKey = "";
		loginRespToJson.systemTime = TimeUtil::getTimeSec();

		std::string jsonStr = loginRespToJson.convertDataToJson();
		loginFailResp.mRespJsonStr = jsonStr;
		loginFailResp.mPacketID = BOC_GAME_LOGIN_RESP;

		NetworkSystem::getSingleton().sendMsg(loginFailResp, connId);
		pushEvent(EVENT_CLIENT_CONNECTION_CLOSE, connId);
		return;
	}

	ConnId oldConnId = player->getConnId();
	if (oldConnId)
	{
		mPlayerConnectionMap.erase(oldConnId);
		player->onClientConnectClosed();
		AccountWasLoginNotify notify;
		NetworkSystem::getSingleton().sendMsg(notify, oldConnId);
		pushEvent(EVENT_CLIENT_CONNECTION_CLOSE, oldConnId);
	}

	player->onClientConnectReconnected(connId, loginToken, true);
	mPlayerConnectionMap.insert(connId, player);



	//日常任务和活跃点刷新
	UInt64 curTime = time(0);
	checkReset(curTime, player);

	 PlayerBaseData playerBaseData = player->getPlayerBaseData();




	//禁止登陆的时间大于当前时间，那么禁止玩家登陆
		if(playerBaseData.m_nForbidLoginTime > time(0) && time(0) >=  playerBaseData.m_nForbidBeginTime)
		{
			LOG_WARN("Forbid player login %llu", playerBaseData.m_nPlayerID);

			GCGameLoginResp loginFailResp;

			MsgLoginRespToJson loginRespToJson;
			loginRespToJson.result = LynxErrno::ForbidLogin;
			loginRespToJson.loginToken = loginToken;
			loginRespToJson.errMsg ="forbid login";
			loginRespToJson.loginMainKey = "";
			loginRespToJson.systemTime = TimeUtil::getTimeSec();

			std::string jsonStr = loginRespToJson.convertDataToJson();
			loginFailResp.mRespJsonStr = jsonStr;
			loginFailResp.mPacketID = BOC_GAME_LOGIN_RESP;
			NetworkSystem::getSingleton().sendMsg(loginFailResp, connId);

			//pushEvent(EVENT_CLIENT_CONNECTION_CLOSE, connId);

			return;
		}

		InlineActivityManager::getSingleton().checkLogin(player->getPlayerGuid());//顺序不能错，在线奖励用到


	//新协议

	GCGameLoginResp loginSuccessResp;

	MsgLoginRespToJson loginRespToJson;
	loginRespToJson.result = LynxErrno::None;
	loginRespToJson.loginToken = loginToken;
	loginRespToJson.errMsg ="";
	loginRespToJson.loginMainKey = "";
	loginRespToJson.systemTime = TimeUtil::getTimeSec();

	std::string jsonStr = loginRespToJson.convertDataToJson();
	loginSuccessResp.mRespJsonStr = jsonStr;
	loginSuccessResp.mPacketID = BOC_GAME_LOGIN_RESP;
	NetworkSystem::getSingleton().sendMsg(loginSuccessResp, connId);

	GCPlayerDetailResp detailInfoResp;
	detailInfoResp.mPacketID = BOC_PLAYER_DETAIL_RESP;
	detailInfoResp.mRespJsonStr = player->getPlayerInfoManager().convertDetailInfoToJson();

	NetworkSystem::getSingleton().sendMsg(detailInfoResp, connId);

	GCSkillListResp skillListResp;
	skillListResp.mPacketID = BOC_SKILL_LIST_RESP;
	skillListResp.mRespJsonStr = player->getSkillManager().convertSkillDataToJson();
	NetworkSystem::getSingleton().sendMsg(skillListResp, connId);

	GCBuffListResp buffListResp;
	buffListResp.mPacketID = BOC_BUFF_LIST_RESP;
	buffListResp.mRespJsonStr = player->getBuffManager().convertBuffDataToJson();
	NetworkSystem::getSingleton().sendMsg(buffListResp, connId);

	ItemListResp itemListResp;
	itemListResp.mPacketID = ITME_LIST_RESP;
	itemListResp.mRespJsonStr = player->getItemManager().convertItemDataToJson();
	NetworkSystem::getSingleton().sendMsg(itemListResp,connId);

	GCPlayerEquipStateResp equipStateResp;
	equipStateResp.mPacketID = BOC_PLAYER_EQUIP_STATE_RESP;
	equipStateResp.mRespJsonStr = player->getEquipManager().convertEquipStateToJson();

	NetworkSystem::getSingleton().sendMsg(equipStateResp,connId);



	StageManager::getSingleton().sendStageData(player->getPlayerGuid(), player->getStageManager().getMapIDToChapter(),true);

	GCPlayerFashionListResp fashionListResp;
	fashionListResp.mPacketID = BOC_PLAYER_FASHION_LIST_RESP;
	fashionListResp.mRespJsonStr = player->getFashionManager().convertFashionDataToJson();
	NetworkSystem::getSingleton().sendMsg(fashionListResp,connId);

	// 	FireConfirmManager::getSingleton().getChapterCounter(connId);

	//客户端没有写对应的处理包逻辑，目前先注释，以后打开
	GCRhymeResp rhymeResp;
	rhymeResp.mPacketID = BOC_RHYME_RESP;
	rhymeResp.mRespJsonStr = player->getRhymeManager().convertRhymeDataToJson();
	NetworkSystem::getSingleton().sendMsg(rhymeResp,connId);

	GCPlayerHoarStoneListResp hoarStoneListResp;
	hoarStoneListResp.mPacketID = BOC_PLAYER_HOARSTONE_LIST_RESP;
	hoarStoneListResp.mRespJsonStr = player->getHoarStoneManager().convertDataToJson();
	NetworkSystem::getSingleton().sendMsg(hoarStoneListResp,connId);

	GCHeroEquipListResp heroEquipListResp;
	heroEquipListResp.mPacketID = BOC_HERO_EQUIPLIST_RESP;
	heroEquipListResp.mRespJsonStr = player->getHeroEquipManager().convertDataToJson();
	NetworkSystem::getSingleton().sendMsg(heroEquipListResp,connId);

	MsgServerTimeRespToJson ServerTimeresp;		
	ServerTimeresp.mClientTime = 0;
	ServerTimeresp.mServerTime = TimeUtil::getTimeSec();
	jsonStr = ServerTimeresp.convertDataToJson();	
	NetworkSystem::getSingleton().sender( connId,SYNC_TIME_RESP,jsonStr);


	GCGemListResp gemListResp;
	gemListResp.mPacketID = BOC_GEM_LIST_RESP;
	gemListResp.mRespJsonStr = player->getGemManager().convertDataToJson();
	NetworkSystem::getSingleton().sendMsg(gemListResp,connId);


	TowerReq  req;
	req.reqtype = 1;	
	ClimbTowerManager::getSingleton().onTowerReq(connId,req);

	CGFoods foodsmsg;
	foodsmsg.reqType =1;
	FireConfirmManager::onFoodsReq(connId ,foodsmsg);


	CGStrength strengthmsg;
	strengthmsg.reqType =1;
	FireConfirmManager::onStrengthReq(connId ,strengthmsg);

	GCJewelryListResp jewelryListResp;
	jewelryListResp.mPacketID = BOC_JEWELRY_LIST_RESP;
	jewelryListResp.mRespJsonStr = player->getJewelryManager().convertDataToJson();
	NetworkSystem::getSingleton().sendMsg(jewelryListResp,connId);
	//cout << jewelryListResp.mRespJsonStr;

	GCMaterialListResp materialListResp;
	materialListResp.mPacketID = BOC_MATERIAL_LIST_RESP;
	materialListResp.mRespJsonStr = player->getMaterialManager().convertDataToJson();
	NetworkSystem::getSingleton().sendMsg(materialListResp, connId);

	GCServantDataListResp servantDataListResp;
	servantDataListResp.mPacketID = BOC_SERVANTDATA_LIST_RESP;
	servantDataListResp.mRespJsonStr = player->getServantManager().convertDataToJson();
	NetworkSystem::getSingleton().sendMsg(servantDataListResp, connId);
	//cout << servantDataListResp.mRespJsonStr;

	GCChatDataListResp chatListResp;
	chatListResp.mPacketID = BOC_CHATDATALIST_RESP;
	chatListResp.mRespJsonStr = player->getChatManager().convertDataToJson();
	NetworkSystem::getSingleton().sendMsg(chatListResp,connId);

	GCFriendBlackListResp friendblackListResp;
	friendblackListResp.mPacketID = BOC_FRIENDBLACKLIST_RESP;
	friendblackListResp.mRespJsonStr = player->getFriendBlackManager().convertDataToJson();
	NetworkSystem::getSingleton().sendMsg(friendblackListResp, connId);

	EmailListResp emailListResp;
	emailListResp.mPacketID = EMAIL_LIST_RESP;
	emailListResp.mRespJsonStr = player->getEmailManager().convertDataToJson();
	//cout << emailListResp.mRespJsonStr;
	NetworkSystem::getSingleton().sendMsg(emailListResp, connId);

	GCCharactorListResp charactorList;
	charactorList.mPacketID = BOC_CHARACTORLIST_RESP;
	charactorList.mRespJsonStr = player->getCharactorManager().convertCharactorDataToJson();
	NetworkSystem::getSingleton().sendMsg(charactorList, connId);

	RewardLotteryMsgReq rewardLotteryMsgReq;
	GiftManager::getSingleton().onRewardLotteryMsgReq(connId,rewardLotteryMsgReq);

	CourageChallengeReq courageChallengeReq;
	courageChallengeReq.reqType =0;
	courageChallengeReq.needSend = true;
	CourageChallengeManager::getSingleton().onCourageChallengeReq(connId,courageChallengeReq);



	InlineActivityManager::getSingleton().sendActivitys(connId );

	GCAchieveDailyListResp achieveDailyListResp;
	achieveDailyListResp.mPacketID = BOC_ACHIEVEDAILYLIST_RESP;
	achieveDailyListResp.mRespJsonStr = player->getAchieveManager().convertAchieveDataToJson();
	//cout << achieveDailyListResp.mRespJsonStr;
	NetworkSystem::getSingleton().sendMsg(achieveDailyListResp, connId);


	GCConsortDataResp consortDataResp;
	consortDataResp.mPacketID = BOC_CONSORTDATA_RESP;
	consortDataResp.mRespJsonStr = player->getConsortManager().convertDataToJson();
	NetworkSystem::getSingleton().sendMsg(consortDataResp, connId);

	RankGameReq rankGameReq;
	rankGameReq.needSend = true;
	rankGameReq.isLogin = true;
	RankGameManager::getSingleton().onRankGameReq(connId,rankGameReq);





	LogicSystem::getSingleton().logPlayerLogin(player->getPlayerBaseData());


}



void LogicSystem::loadPlayerData(const ConnId& connId,const Guid& playerGuid)
{
	//系统禁止登陆
	if(mForbidLogin)
	{
		LOG_WARN("frobidlogin by connId %llu", connId);
		GCGameLoginResp loginFailResp;

		MsgLoginRespToJson loginRespToJson;

		loginRespToJson.result = LynxErrno::ForbidLogin;
		loginRespToJson.loginToken = 0;
		loginRespToJson.errMsg ="Failed to get login token by connId";
		loginRespToJson.loginMainKey = "";
		loginRespToJson.systemTime = TimeUtil::getTimeSec();

		std::string jsonStr = loginRespToJson.convertDataToJson();
		loginFailResp.mRespJsonStr = jsonStr;
		loginFailResp.mPacketID = BOC_GAME_LOGIN_RESP;

		NetworkSystem::getSingleton().sendMsg(loginFailResp, connId);
		//pushEvent(EVENT_CLIENT_CONNECTION_CLOSE, connId);
		return;

	}

	Player* player = LogicSystem::getSingleton().getPlayerByGuid(playerGuid);
	if (player)
	{
		loadPlayerDataFromMemery(connId, player);
	}
	else
	{
		// 加载玩家数据从DB
		PersistLoadPlayerDataReq req;
		req.mConnId = connId;
		req.mPlayerGuid = playerGuid;
		PersistSystem::getSingleton().postThreadMsg(req, 0);

	}
}


void
LogicSystem::passportAuthSuccess(const ConnId& connId, const Guid& accountId, const String& accountName)
{
	// 账户验证成功,检查是否创建角色
	PersistLoadPlayerGuidReq req;
	req.mAccountId = accountId;
	req.mConnId = connId;
	req.mAccountName = accountName;
	PersistSystem::getSingleton().postThreadMsg(req, 0);
}

void 
LogicSystem::passportAuthFail(const ConnId& connId, const String& accountName)
{
	/* LOG_WARN("Failed to auth account %s", accountName.c_str());
	LoginFailResp loginFailResp;
	loginFailResp.mErrno = LynxErrno::AccountAuthFailed;
	NetworkSystem::getSingleton().sendMsg(loginFailResp, connId);
	pushEvent(EVENT_CLIENT_CONNECTION_CLOSE, connId);*/
}

void 
LogicSystem::loadPlayerDataSuccess(const ConnId& connId, const PlayerData& playerData)
{

	Player* player = getPlayerByGuid(playerData.mBaseData.m_nPlayerID);
	if (player)
	{
		loadPlayerDataFromMemery(connId, player);
	}
	else
	{
		UInt64 loginToken = NetworkSystem::getSingleton().createLoginToken(connId);

		if (loginToken == 0)
		{
			LOG_WARN("Failed to get login token by connId %llu", connId);


			GCGameLoginResp loginFailResp;

			MsgLoginRespToJson loginRespToJson;
			loginRespToJson.result = LynxErrno::LoginTokenFailed;
			loginRespToJson.loginToken = loginToken;
			loginRespToJson.errMsg ="Failed to get login token by connId";
			loginRespToJson.loginMainKey = "";
			loginRespToJson.systemTime = TimeUtil::getTimeSec();

			std::string jsonStr = loginRespToJson.convertDataToJson();
			loginFailResp.mRespJsonStr = jsonStr;
			loginFailResp.mPacketID = BOC_GAME_LOGIN_RESP;
			NetworkSystem::getSingleton().sendMsg(loginFailResp, connId);

			pushEvent(EVENT_CLIENT_CONNECTION_CLOSE, connId);
			return;
		}

		//禁止登陆的时间大于当前时间，那么禁止玩家登陆
		if(playerData.mBaseData.m_nForbidLoginTime > time(0) && time(0) >= playerData.mBaseData.m_nForbidBeginTime)
		{
			LOG_WARN("Forbid player login %llu", playerData.mBaseData.m_nPlayerID);

			GCGameLoginResp loginFailResp;

			MsgLoginRespToJson loginRespToJson;
			loginRespToJson.result = LynxErrno::ForbidLogin;
			loginRespToJson.loginToken = loginToken;
			loginRespToJson.errMsg ="forbid login";
			loginRespToJson.loginMainKey = "";
			loginRespToJson.systemTime = TimeUtil::getTimeSec();

			std::string jsonStr = loginRespToJson.convertDataToJson();
			loginFailResp.mRespJsonStr = jsonStr;
			loginFailResp.mPacketID = BOC_GAME_LOGIN_RESP;
			NetworkSystem::getSingleton().sendMsg(loginFailResp, connId);

			//pushEvent(EVENT_CLIENT_CONNECTION_CLOSE, connId);

			return;
		}

		player = createPlayer(connId, playerData, loginToken);
		if (!player)
		{
			LOG_WARN("Failed to create player %llu", playerData.mBaseData.m_nPlayerID);

			GCGameLoginResp loginFailResp;

			MsgLoginRespToJson loginRespToJson;
			loginRespToJson.result = LynxErrno::CreatePlayerFailed;
			loginRespToJson.loginToken = loginToken;
			loginRespToJson.errMsg ="Failed to create player";
			loginRespToJson.loginMainKey = "";
			loginRespToJson.systemTime = TimeUtil::getTimeSec();

			std::string jsonStr = loginRespToJson.convertDataToJson();
			loginFailResp.mRespJsonStr = jsonStr;
			loginFailResp.mPacketID = BOC_GAME_LOGIN_RESP;
			NetworkSystem::getSingleton().sendMsg(loginFailResp, connId);

			pushEvent(EVENT_CLIENT_CONNECTION_CLOSE, connId);
			return;
		}

		//由于玩家初始化登录会initial该部分信息，所以此处注释 wwc
		//updateBaseInfo(playerData.mBaseData);
		//updateConsortInfo(playerData.mBaseData.m_nPlayerID, playerData.mConsortData);

		InlineActivityManager::getSingleton().checkLogin(playerData.mBaseData.m_nPlayerID);//顺序不能错，在线奖励用到
		StageManager::getSingleton().refreshDailyresetData(player->getPlayerGuid());//顺序不能错

		player->checkLvActiveConditon();

		player->checkGuidInit();
		UInt32 modelId = player->getPlayerModelID();
		player->getCharactorManager().syncCharactor(modelId);

		GCGameLoginResp loginSuccessResp;

		MsgLoginRespToJson loginRespToJson;
		loginRespToJson.result = LynxErrno::None;
		loginRespToJson.loginToken = loginToken;
		loginRespToJson.errMsg ="";
		loginRespToJson.loginMainKey = "";
		loginRespToJson.systemTime = TimeUtil::getTimeSec();

		std::string jsonStr = loginRespToJson.convertDataToJson();
		loginSuccessResp.mRespJsonStr = jsonStr;
		loginSuccessResp.mPacketID = BOC_GAME_LOGIN_RESP;
		NetworkSystem::getSingleton().sendMsg(loginSuccessResp, connId);

		GCPlayerDetailResp detailInfoResp;
		detailInfoResp.mPacketID = BOC_PLAYER_DETAIL_RESP;
		detailInfoResp.mRespJsonStr = player->getPlayerInfoManager().convertDetailInfoToJson();
 
		NetworkSystem::getSingleton().sendMsg(detailInfoResp, connId);
		//LOG_INFO("data resp is : %s", detailInfoResp.mRespJsonStr.c_str() );

		GCSkillListResp skillListResp;
		skillListResp.mPacketID = BOC_SKILL_LIST_RESP;
		skillListResp.mRespJsonStr = player->getSkillManager().convertSkillDataToJson();
		NetworkSystem::getSingleton().sendMsg(skillListResp, connId);

		//LOG_INFO("data resp is : %s", skillListResp.mRespJsonStr.c_str() );

		GCBuffListResp buffListResp;
		buffListResp.mPacketID = BOC_BUFF_LIST_RESP;
		buffListResp.mRespJsonStr = player->getBuffManager().convertBuffDataToJson();
		NetworkSystem::getSingleton().sendMsg(buffListResp, connId);

	   // LOG_INFO("data resp is : %s", buffListResp.mRespJsonStr.c_str() );

		ItemListResp itemListResp;
		itemListResp.mPacketID = ITME_LIST_RESP;
		itemListResp.mRespJsonStr = player->getItemManager().convertItemDataToJson();
		NetworkSystem::getSingleton().sendMsg(itemListResp,connId);

		// LOG_INFO("data resp is : %s", itemListResp.mRespJsonStr.c_str() );
	
		
		StageManager::getSingleton().sendStageData( player->getPlayerGuid(), player->getStageManager().getMapIDToChapter(),true);

		GCPlayerFashionListResp fashionListResp;
		fashionListResp.mPacketID = BOC_PLAYER_FASHION_LIST_RESP;
		fashionListResp.mRespJsonStr = player->getFashionManager().convertFashionDataToJson();
		NetworkSystem::getSingleton().sendMsg(fashionListResp,connId);

		 //LOG_INFO("data resp is : %s", fashionListResp.mRespJsonStr.c_str() );

		GCPlayerEquipStateResp equipStateResp;
		equipStateResp.mPacketID = BOC_PLAYER_EQUIP_STATE_RESP;
		equipStateResp.mRespJsonStr = player->getEquipManager().convertEquipStateToJson();
		NetworkSystem::getSingleton().sendMsg(equipStateResp,connId);

		// LOG_INFO("data resp is : %s", equipStateResp.mRespJsonStr.c_str() );

		//客户端没有写包处理逻辑，目前先注释，以后打开
		GCRhymeResp rhymeResp;
		rhymeResp.mPacketID = BOC_RHYME_RESP;
		rhymeResp.mRespJsonStr = player->getRhymeManager().convertRhymeDataToJson();
		NetworkSystem::getSingleton().sendMsg(rhymeResp,connId);

		 //LOG_INFO("data resp is : %s", rhymeResp.mRespJsonStr.c_str() );

		GCPlayerHoarStoneListResp hoarStoneListResp;
		hoarStoneListResp.mPacketID = BOC_PLAYER_HOARSTONE_LIST_RESP;
		hoarStoneListResp.mRespJsonStr = player->getHoarStoneManager().convertDataToJson();
		NetworkSystem::getSingleton().sendMsg(hoarStoneListResp,connId);

		//LOG_INFO("data resp is : %s", hoarStoneListResp.mRespJsonStr.c_str() );

		GCHeroEquipListResp heroEquipListResp;
		heroEquipListResp.mPacketID = BOC_HERO_EQUIPLIST_RESP;
		heroEquipListResp.mRespJsonStr = player->getHeroEquipManager().convertDataToJson();
		NetworkSystem::getSingleton().sendMsg(heroEquipListResp,connId);

		//LOG_INFO("data resp is : %s", heroEquipListResp.mRespJsonStr.c_str() );

		MsgServerTimeRespToJson ServerTimeresp;		
		ServerTimeresp.mClientTime = 0;
		ServerTimeresp.mServerTime = TimeUtil::getTimeSec();
		jsonStr = ServerTimeresp.convertDataToJson();	
		NetworkSystem::getSingleton().sender( connId,SYNC_TIME_RESP,jsonStr);

		//LOG_INFO("data resp is : %s", jsonStr.c_str() );

		GCGemListResp gemListResp;
		gemListResp.mPacketID = BOC_GEM_LIST_RESP;
		gemListResp.mRespJsonStr = player->getGemManager().convertDataToJson();
		NetworkSystem::getSingleton().sendMsg(gemListResp,connId);

		//LOG_INFO("data resp is : %s", gemListResp.mRespJsonStr.c_str() );

		CGFoods foodsmsg;
		foodsmsg.reqType =1;
		FireConfirmManager::onFoodsReq(connId ,foodsmsg);

		CGStrength strengthmsg;
		strengthmsg.reqType =1;
		FireConfirmManager::onStrengthReq(connId ,strengthmsg);

		TowerReq  req;
		req.reqtype = 1;	
		ClimbTowerManager::getSingleton().onTowerReq(connId,req);


		GCJewelryListResp jewelryListResp;
		jewelryListResp.mPacketID = BOC_JEWELRY_LIST_RESP;
		jewelryListResp.mRespJsonStr = player->getJewelryManager().convertDataToJson();
		NetworkSystem::getSingleton().sendMsg(jewelryListResp,connId);
		//cout << jewelryListResp.mRespJsonStr;
		//LOG_INFO("jewelrylistresp is: %s", jewelryListResp.mRespJsonStr.c_str());

		GCMaterialListResp materialListResp;
		materialListResp.mPacketID = BOC_MATERIAL_LIST_RESP;
		materialListResp.mRespJsonStr = player->getMaterialManager().convertDataToJson();
		NetworkSystem::getSingleton().sendMsg(materialListResp, connId);
		//cout << materialListResp.mRespJsonStr;
		//LOG_INFO("data resp is : %s", materialListResp.mRespJsonStr.c_str() );

		GCServantDataListResp servantDataListResp;
		servantDataListResp.mPacketID = BOC_SERVANTDATA_LIST_RESP;
		servantDataListResp.mRespJsonStr = player->getServantManager().convertDataToJson();
		NetworkSystem::getSingleton().sendMsg(servantDataListResp, connId);
		//cout << servantDataListResp.mRespJsonStr;
			//LOG_INFO("data resp is : %s", servantDataListResp.mRespJsonStr.c_str() );


		GCChatDataListResp chatListResp;
		chatListResp.mPacketID = BOC_CHATDATALIST_RESP;
		chatListResp.mRespJsonStr = player->getChatManager().convertDataToJson();
		NetworkSystem::getSingleton().sendMsg(chatListResp,connId);

			//LOG_INFO("data resp is : %s", chatListResp.mRespJsonStr.c_str() );

		GCFriendBlackListResp friendblackListResp;
		friendblackListResp.mPacketID = BOC_FRIENDBLACKLIST_RESP;
		friendblackListResp.mRespJsonStr = player->getFriendBlackManager().convertDataToJson();
		//cout << friendblackListResp.mRespJsonStr;
		NetworkSystem::getSingleton().sendMsg(friendblackListResp, connId);

		//LOG_INFO("data resp is : %s", friendblackListResp.mRespJsonStr.c_str() );

		EmailListResp emailListResp;
		emailListResp.mPacketID = EMAIL_LIST_RESP;
		emailListResp.mRespJsonStr = player->getEmailManager().convertDataToJson();
		NetworkSystem::getSingleton().sendMsg(emailListResp, connId);

		//LOG_INFO("data resp is : %s", emailListResp.mRespJsonStr.c_str() );

		GCCharactorListResp charactorList;
		charactorList.mPacketID = BOC_CHARACTORLIST_RESP;
		charactorList.mRespJsonStr = player->getCharactorManager().convertCharactorDataToJson();
		NetworkSystem::getSingleton().sendMsg(charactorList, connId);

		//LOG_INFO("data resp is : %s", charactorList.mRespJsonStr.c_str() );


		RewardLotteryMsgReq rewardLotteryMsgReq;
		GiftManager::getSingleton().onRewardLotteryMsgReq(connId,rewardLotteryMsgReq);

		CourageChallengeReq courageChallengeReq;
		courageChallengeReq.reqType =0;
		courageChallengeReq.needSend = true;
		CourageChallengeManager::getSingleton().onCourageChallengeReq(connId,courageChallengeReq);

	

		InlineActivityManager::getSingleton().sendActivitys(connId );

		GCAchieveDailyListResp achieveDailyListResp;
		achieveDailyListResp.mPacketID = BOC_ACHIEVEDAILYLIST_RESP;
		achieveDailyListResp.mRespJsonStr = player->getAchieveManager().convertAchieveDataToJson();
		//cout << achieveDailyListResp.mRespJsonStr;
		NetworkSystem::getSingleton().sendMsg(achieveDailyListResp, connId);
		// 		OnlineWelFareReq  req42;
		// 		req42.reqType = 0;		
		// 		InlineActivityManager::getSingleton().onOnlineWelFareReq( connId,req42);

		//	LOG_INFO("data resp is : %s", achieveDailyListResp.mRespJsonStr.c_str() );

		GCConsortDataResp consortDataResp;
		consortDataResp.mPacketID = BOC_CONSORTDATA_RESP;
		consortDataResp.mRespJsonStr = player->getConsortManager().convertDataToJson();
		NetworkSystem::getSingleton().sendMsg(consortDataResp, connId);

		RankSystem::getSingleton().initPlayerPowerRank(player->getPlayerGuid());

			//LOG_INFO("data resp is : %s", consortDataResp.mRespJsonStr.c_str() );

		RankGameReq rankGameReq;
		rankGameReq.needSend = true;
		rankGameReq.isLogin = true;
		RankGameManager::getSingleton().onRankGameReq(connId,rankGameReq);


		//要放在最后
		LogicSystem::getSingleton().logPlayerLogin(playerData.mBaseData);
		
		
		LOG_INFO("end dddddd %llu !",TimeUtil::getTimeMilliSec());

		//天假测试材料
		
		/*ReturnItemEle rtItemEle;
		player->getAllItemManager().addAwardMaterial(AWARD_JEWELRY, 2552 ,3,rtItemEle,MiniLog144);
		player->getAllItemManager().addAwardMaterial(AWARD_JEWELRY, 2553  ,1,rtItemEle,MiniLog144);
		player->getAllItemManager().addAwardMaterial(AWARD_JEWELRY, 2554  ,2,rtItemEle,MiniLog144);
		player->getAllItemManager().addAwardMaterial(AWARD_JEWELRY, 3022  ,1,rtItemEle,MiniLog144);
		player->getAllItemManager().addAwardMaterial(AWARD_JEWELRY, 3023  ,3,rtItemEle,MiniLog144);
		player->getAllItemManager().addAwardMaterial(AWARD_JEWELRY, 4021   ,1,rtItemEle,MiniLog144);
		player->getAllItemManager().addAwardMaterial(AWARD_JEWELRY, 4022  ,2,rtItemEle,MiniLog144);
	    player->getAllItemManager().addAwardMaterial(AWARD_JEWELRY, 4023    ,3,rtItemEle,MiniLog144);
		   player->getAllItemManager().addAwardMaterial(AWARD_JEWELRY, 5033     ,1,rtItemEle,MiniLog144);
		      player->getAllItemManager().addAwardMaterial(AWARD_JEWELRY, 5035     ,2,rtItemEle,MiniLog144);*/
		/*player->getMaterialManager().addMaterial(1001 , 11);
		player->getMaterialManager().addMaterial(1002 , 11);
		player->getMaterialManager().addMaterial(1 , 99999);
		player->getMaterialManager().addMaterial(2001 , 26);
		player->getMaterialManager().addMaterial(2002 , 26);
		player->getMaterialManager().addMaterial(3001 , 51);
		player->getMaterialManager().addMaterial(3002 , 51);
		player->getMaterialManager().addMaterial(4002 , 86);
		player->getMaterialManager().addMaterial(4001 , 86);
		player->getMaterialManager().addMaterial(5002 , 131);
		player->getMaterialManager().addMaterial(5001 , 131);
		player->getMaterialManager().addMaterial(6002 , 186);
		player->getMaterialManager().addMaterial(6001 , 186);
		player->getMaterialManager().addMaterial(7002 , 251);
		player->getMaterialManager().addMaterial(7001 , 251);*/

		//cout << emailListResp.mRespJsonStr;

		//cout << "ssssss" <<TimeUtil::getTimeMilliSec() << endl;

		///////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////

		/*PersistAddEmail addEmail;
		addEmail.mEmailData.m_nPlayerUid = playerData.mBaseData.m_nPlayerID;
		addEmail.mEmailData.m_nTempId = 2;
		addEmail.mEmailData.m_nEmailUid = generateEmailGuid();
		addEmail.mEmailData.m_nGetState = 1;
		addEmail.mEmailData.m_nOpenState = 1;
		addEmail.mEmailData.m_strContent = "ni mei a !";
		addEmail.mEmailData.m_strDes = "wo mei a!";

		PersistSystem::getSingleton().postThreadMsg(addEmail, connId);*/

		/*PersistUpdateEmail updateEmail;
		updateEmail.mEmailData.m_nPlayerUid = 1099;
		updateEmail.mEmailData.m_nTempId = 3;
		updateEmail.mEmailData.m_nEmailUid = 1;
		updateEmail.mEmailData.m_nGetState = 5;
		updateEmail.mEmailData.m_nOpenState = 6;
		updateEmail.mEmailData.m_strContent = "dfdfni mei a !";
		updateEmail.mEmailData.m_strDes = "fdfdfwo mei a!";

		PersistSystem::getSingleton().postThreadMsg(updateEmail, connId);*/

		/*PersistDelEmail delEmail;
		delEmail.mEmailUid = 1;


		PersistSystem::getSingleton().postThreadMsg(delEmail, connId);*/
		/*PersistCreatePlayerReq createPlayerReq;
		createPlayerReq.mAccountId = 3;
		createPlayerReq.mConnId = connId;
		createPlayerReq.mName = "sfdf";
		createPlayerReq.mPlayerModelId = 2;
		PersistSystem::getSingleton().postThreadMsg(createPlayerReq, connId);*/
		//cout << RankSystem::getSingleton().sendPowerRankJson(playerData.mBaseData.m_nPlayerID);

		//player->getAchieveManager().updateAchieveData(PLAYERLV,55);
		//player->getAchieveManager().finishAchieve(PLAYERLV,17001);
		//	player->getAchieveManager().dealAddDailyReq(10001);
		//player->getAchieveManager().updateDailyTaskData(1001,100);
		//player->getPlayerInfoManager().rename("iloveu");

		//cout << gAchievementTable <<endl;
		//cout << gAchievementTableCopy <<endl;
		////cout << &(ITEM_TABLE()) << endl;
		////Lynx::Map<std::string,  Lynx::UInt32>::Iter * findIter = m_mapGlobleFlag.find("item");
		////cout << findIter->mValue << endl;
		////findIter->mValue = 1;
		////cout <<&( ITEM_TABLE()) <<endl;
		//cout <<  ACHIEVEMENT_TABLE().get(11001)->mArg << endl;
		//player->getGMManager().dealDbcReload("achievement");
		//cout << ACHIEVEMENT_TABLE().get(11001)->mArg << endl;

		//player->getConsortManager().syncRankConsortList();
		//player->getConsortManager().findConsortById(3);
		//player->getConsortManager().quickJoinConsort();
		//player->getConsortManager().joinConsort(1);
		//player->getConsortManager().createConsort("nice to meet you!");
		//player->getConsortManager().quickJoinConsort();
		//player->getConsortManager().checkApply(2);
		//player->getConsortManager().quickJoinConsort();
		//	player->getConsortManager().dealApply(2, 1231, 0);
		//player->getConsortManager().joinConsort(2);
		//player->getConsortManager().dealApply(2, 1233, 1);
		//player->getConsortManager().dealApplyClear(2);
		//player->getConsortManager().dealApplyClear(2);
		//	player->getConsortManager().setConsortInfo(2,1,1000);
		//player->getConsortManager().kickMember(2, 1233);
		// 		player->getConsortManager().setViceLeader(1235, 1);
		//player->getConsortManager().createConsort("new consort");
		//player->getConsortManager().consortSign(2);

		//player->getConsortManager().consortSignAward(1);
		/*List<UInt32> taskList;
		player->getConsortManager().generateCatTask(4, taskList);*/

		//player->getConsortManager().initCatTask();
		//player->getConsortManager().beginKitchenQue();
		//player->getConsortManager().endKitchenQue(1005,1);


		//pushEvent(EVENT_CLIENT_CONNECTION_CLOSE, connId);
		//player->getConsortManager().ticketJoin(1,0,23);
		//player->getConsortManager().ticketSupport(1,4);
       //player->getConsortManager().ticketGetAward();
	}	
}

void 
LogicSystem::loadPlayerDataFail(const ConnId& connId, const Guid& playerGuid)
{
	LOG_WARN("Failed to load player %llu", playerGuid);
	UInt64 loginToken = NetworkSystem::getSingleton().createLoginToken(connId);

	GCGameLoginResp loginFailResp;

	MsgLoginRespToJson loginRespToJson;
	loginRespToJson.result = LynxErrno::LoadPlayerFailed;
	loginRespToJson.loginToken = loginToken;
	loginRespToJson.errMsg ="Failed to load player";
	loginRespToJson.loginMainKey = "";
	loginRespToJson.systemTime = TimeUtil::getTimeSec();

	std::string jsonStr = loginRespToJson.convertDataToJson();
	loginFailResp.mRespJsonStr = jsonStr;
	loginFailResp.mPacketID = BOC_GAME_LOGIN_RESP;
	NetworkSystem::getSingleton().sendMsg(loginFailResp, connId);

	pushEvent(EVENT_CLIENT_CONNECTION_CLOSE, connId);
}

void 
LogicSystem::reconnect(const ConnId& connId, const Guid& playerGuid, const UInt64& loginToken)
{

	Player* player = getPlayerByGuid(playerGuid);
	if (!player)
	{
		ReconnectFailResp resp;
		resp.mErrno = LynxErrno::PlayerNotExist;
		NetworkSystem::getSingleton().sendMsg(resp, connId);
		pushEvent(EVENT_CLIENT_CONNECTION_CLOSE, connId);
		LOG_INFO(" reconnect  fail PlayerNotExist playerID =[%llu]  ", playerGuid);
		return;
	}

	if (player->getLoginToken() != loginToken)
	{
		ReconnectFailResp resp;
		resp.mErrno = LynxErrno::InvalidPermission;
		NetworkSystem::getSingleton().sendMsg(resp, connId);
		pushEvent(EVENT_CLIENT_CONNECTION_CLOSE, connId);

		LOG_INFO(" reconnect fail InvalidPermission playerID =[%llu]  ", playerGuid);
		return;
	}

	UInt64 newLoginToken = NetworkSystem::getSingleton().createLoginToken(connId);
	if (newLoginToken == 0)
	{
		LOG_WARN("Failed to get login token by connId %llu", connId);
		ReconnectFailResp resp;
		resp.mErrno = LynxErrno::LoginTokenFailed;
		NetworkSystem::getSingleton().sendMsg(resp, connId);
		pushEvent(EVENT_CLIENT_CONNECTION_CLOSE, connId);
		return;
	}

	ConnId oldConnId = player->getConnId();
	if (oldConnId)
	{
		mPlayerConnectionMap.erase(oldConnId);
		player->onClientConnectClosed();
		AccountWasLoginNotify notify;
		Json::Value root;
		root["type"] = 0;
		Json::StyledWriter writer;
		std::string jstr = writer.write(root);
		notify.mRespJsonStr = jstr;
		NetworkSystem::getSingleton().sendMsg(notify, oldConnId);
		pushEvent(EVENT_CLIENT_CONNECTION_CLOSE, oldConnId);

		LOG_INFO(" reconnect close old connect playerID =[%llu]  ", playerGuid);

	}

	player->onClientConnectReconnected(connId, newLoginToken, false);
	mPlayerConnectionMap.insert(connId, player);

	ReconnectSuccessResp resp;
	resp.mLoginToken = newLoginToken;
	NetworkSystem::getSingleton().sendMsg(resp, connId);

}


void 
LogicSystem::broadcaseSystemNotice(const String& contents, UInt8 priority)
{
	SystemNoticeNotify notify;
	notify.mPriority = priority;
	notify.mContents = contents;

	NetworkSystem::getSingleton().broadcastMsg(notify);
}

void 
LogicSystem::sendSystemNoticeToPlayer(const Guid& playerGuid, const String& contents, UInt8 priority)
{
	Player* player = getPlayerByGuid(playerGuid);
	if (!player)
	{
		LOG_WARN("Failed to get player by guid %llu", playerGuid);
		return;
	}

	SystemNoticeNotify notify;
	notify.mPriority = priority;
	notify.mContents = contents;

	player->sendMsg(notify);
}

void LogicSystem::GMEmailSend(UInt64 tempid, UInt64 toPlayer, std::string des, std::string contant, std::string title,
							  std::string from)
{
	UInt64 times = time(0);
	//判断是否为发送给单个人
	if(toPlayer)
	{
		PersistAddEmail addEmail;
		EmailData emailData;

		emailData.m_nEmailUid = LogicSystem::getSingleton().generateEmailGuid();
		emailData.m_nGetState = 0;
		emailData.m_nOpenState = 0;
		emailData.m_nPlayerUid = toPlayer;
		emailData.m_nTempId = tempid;
		emailData.m_nTime = times;
		emailData.m_strContent = contant.c_str();
		emailData.m_strDes = des.c_str();
		emailData.m_strTittle = title.c_str();
		emailData.m_strFrom = from.c_str();
		if(contant.empty())
		{
			emailData.m_nGetState = 1;
		}

		addEmail.mEmailData = emailData;
		PersistSystem::getSingleton().postThreadMsg(addEmail, toPlayer);
		Player * sendToPlayer = LogicSystem::getSingleton().getPlayerByGuid(toPlayer);

		if(sendToPlayer)
		{
			ConnId sendToCon = sendToPlayer->getConnId();
			sendToPlayer->getEmailManager().addEmail(emailData);

			if(sendToCon)
			{
				//发送消息给玩家
				NewEmailNotify emailNotify;
				emailNotify.mPacketID = EMAIL_NOTIFY;
				Json::Value emailRoot;
				emailRoot["emailuid"] = addEmail.mEmailData.m_nEmailUid;
				emailRoot["tempid"] = tempid;
				emailRoot["openState"] = 0;
				emailRoot["getstate"] = addEmail.mEmailData.m_nGetState;
				emailRoot["des"] = des;
				emailRoot["contant"] = contant;
				
				emailRoot["time"] = times;
				emailRoot["title"] = title.c_str();
				emailRoot["from"] = from.c_str();
				
				Json::StyledWriter writer;

				emailNotify.mRespJsonStr = writer.write(emailRoot);
				LOG_INFO("emailNotify.mRespJsonStr = %s",emailNotify.mRespJsonStr.c_str());
				NetworkSystem::getSingleton().sendMsg(emailNotify, sendToCon);
			}

		}


		return;
	}

	UInt64 playerGuid = LogicSystem::getSingleton().getPlayerGuid();
	for(UInt32 i = 1000; i < playerGuid; i++)
	{
		PersistAddEmail addEmail;
		EmailData emailData;

		emailData.m_nEmailUid = LogicSystem::getSingleton().generateEmailGuid();
		emailData.m_nGetState = 0;
		emailData.m_nOpenState = 0;
		emailData.m_nPlayerUid = i;
		emailData.m_nTempId = tempid;
		emailData.m_nTime = times;
		emailData.m_strContent = contant.c_str();
		emailData.m_strDes = des.c_str();
			emailData.m_strTittle = title.c_str();
			emailData.m_strFrom = from.c_str();
		if(contant.empty())
		{
			emailData.m_nGetState = 1;
		}
		addEmail.mEmailData = emailData;

		PersistSystem::getSingleton().postThreadMsg(addEmail, addEmail.mEmailData.m_nPlayerUid);
		Player * sendToPlayer = LogicSystem::getSingleton().getPlayerByGuid(addEmail.mEmailData.m_nPlayerUid);

		//判断玩家是否在线
		if(sendToPlayer)
		{
			ConnId sendToCon = sendToPlayer->getConnId();
			sendToPlayer->getEmailManager().addEmail(emailData);

			if(sendToCon)
			{
				//发送消息给玩家
				NewEmailNotify emailNotify;
				emailNotify.mPacketID = EMAIL_NOTIFY;
				Json::Value emailRoot;
				emailRoot["emailuid"] = addEmail.mEmailData.m_nEmailUid;
				emailRoot["tempid"] = tempid;
				emailRoot["openState"] = 0;
				emailRoot["getstate"] = addEmail.mEmailData.m_nGetState;
				emailRoot["des"] = des;
				emailRoot["contant"] = contant;
				emailRoot["time"] = times;
				emailRoot["title"] = title.c_str();
				emailRoot["from"] = from.c_str();
				Json::StyledWriter writer;

				emailNotify.mRespJsonStr = writer.write(emailRoot);

				NetworkSystem::getSingleton().sendMsg(emailNotify, sendToCon);
			}
		}
	}
}




Guild* 
LogicSystem::createGuild(const Guid& playerGuid, const String& guildName)
{
	return NULL;
}

void 
LogicSystem::destroyGuildByGuid(const Guid& guildGuid)
{

}

void 
LogicSystem::destroyGuildByName(const String& guildName)
{

}

Guild* 
LogicSystem::getGuildByGuid(const Guid& guildGuid)
{
	return NULL;
}

Guild* 
LogicSystem::getGuildByName(const String& guildName)
{
	return NULL;
}

List<UInt32> LogicSystem::getRandomList(int randomCount, int randomMax)
{
	List<UInt32> randomList;
	for(UInt32 i = 0; i < randomCount; i++)
	{
		UInt32 randomValue = rand()%randomMax;
		randomList.insertTail(randomValue);
	}

	return randomList;
}
void LogicSystem::updateDailyResetData(Player *player,UInt32 flag,List<UInt32> &refreshIDs)
{
	PlayerDailyResetData dailyResetData = player->getPlayerDailyResetData();
	GlobalValue *globalValue = GlobalValueManager::getSingleton().getGlobalValueData();
	
	for(List<UInt32>::Iter*iter = refreshIDs.begin();iter!=NULL;iter = refreshIDs.next(iter))
	{
		switch(iter->mValue)
		{
		case TIME_REFRESH_ID01:
			{
				player->getStageManager().stageReset(player,true,STAGE_NORMAL);
				player->getStageManager().stageReset(player,true,STAGE_ELITE);
			}
			break;
		case TIME_REFRESH_ID02:
			{
				player->getStageManager().stageReset(player,true,STAGE_MASTAR);
			}
			break;
		case TIME_REFRESH_ID03:
			{
				//done
			}
			break;
		case TIME_REFRESH_ID04:
			{
				//onStrengthReq
			}
			break;
		case TIME_REFRESH_ID05:
			{
				dailyResetData.m_nCoinBuyTimes = 0;
				dailyResetData.m_nCoinFreeBuyTimes = 0;
			}
			break;
		case TIME_REFRESH_ID06:
			{
				//monthSingnReq
			}
			break;
		case TIME_REFRESH_ID07:
			{
				//done
			}
			break;
		case TIME_REFRESH_ID08:
			{
				dailyResetData.m_nDailyChaiCount = 0;
				dailyResetData.m_nDailyMiCount = 0;
				dailyResetData.m_nDailyYouCount = 0;
				dailyResetData.m_nDailyYanCount = 0;
			}
			break;
		case TIME_REFRESH_ID09:
			{
				dailyResetData.m_nDailyMultipleCopyCount = 0;
				player->getStageManager().resetMatchingPlayers();
			}
			break;
		case TIME_REFRESH_ID10:
			{
				if (globalValue->uRankGameTimes < dailyResetData.m_nRankGameLeftTimes)
				{
					dailyResetData.m_nRankGameLeftTimes = dailyResetData.m_nRankGameLeftTimes;
				}
				else
				{
					dailyResetData.m_nRankGameLeftTimes = globalValue->uRankGameTimes;
				}


			}
			break;
		case TIME_REFRESH_ID11:
			{
				dailyResetData.m_nRankGameBuyTimes = 0;
			}
			break;

		case TIME_REFRESH_ID12:
			{
				dailyResetData.m_nTowerBuyTimes = 0;
				if (dailyResetData.m_nTowerBuyTimes > globalValue->uTPresetlowerlimit )
				{
					dailyResetData.m_nClimbTowerTimes = dailyResetData.m_nTowerBuyTimes;

				}
				else
				{
					dailyResetData.m_nClimbTowerTimes = globalValue->uTPresetlowerlimit;			
				}
			}
			break;
		case TIME_REFRESH_ID13:
			{
				//done
			}
			break;
		case TIME_REFRESH_ID14:
			{
				//done
			}
			break;
		case TIME_REFRESH_ID15:
			{
				//done
			}
			break;
		case TIME_REFRESH_ID16:
			{
				//done
			}
			break;
		case TIME_REFRESH_ID17:
			{
				if (dailyResetData.m_nTwelvePalaceUnlockCount > globalValue->uTPresetlowerlimit)
				{
				}
				else
				{
					dailyResetData.m_nTwelvePalaceUnlockCount = globalValue->uTPresetlowerlimit;			
				}
				dailyResetData.m_nTwelvePalaceBuyTimes = 0;

				player->getStageManager().resetMatchingPlayersTwelvePalace();
				player->getStageManager().resetChapterUnlock();//顺序不能乱
				player->getStageManager().stageReset(player,true,STAGE_TWELVEPALACE);

			}
			break;
		case TIME_REFRESH_ID18:
			{				
				dailyResetData.m_RewardLotteryDailyOnceFreeCount = 0;
			}
			break;
				
		default:
			{

			}
			break;

		}
	}

	if (flag == 4)	
	{
		// 		dailyResetData.m_nStrengthBuyTimes = 0;
	}
	else if (flag == 5)
	{
		dailyResetData.notUsed7 = 0;
	}
	else if (flag == 6)
	{	
	}
	else if (flag == 0)
	{
		//如果不是同一天，说明数据需要刷新，那么更新数据
		dailyResetData.m_nArenaEnterCount = 0;
		dailyResetData.m_nAmphitheaterEnterCount = 0;
		dailyResetData.m_nAmphitheaterWinCount = 0;
		dailyResetData.m_nAmphitheaterLoseCount = 0;

		dailyResetData.m_nTwelvepalaceEnterCount = 0;

		dailyResetData.m_strTwelvepalaceData = "";


		dailyResetData.m_nCouragetrialDailyPoint = 10;

		dailyResetData.m_nActivepoint = 0;
		//m_nDailyMultipleCopyCount
		//m_nTwelvePalaceUnlockCount	
		// 		dailyResetData.m_nTwelvePalaceBuyTimes = 0;
		//先加载周更新的数据，之后根据判断进行


		dailyResetData.m_strCourageData = dailyResetData.m_strCourageData;


		dailyResetData.m_nFishEatTimes = globalValue->uFISHEATmaxtimes;

	}

	dailyResetData.m_nLastUpdateTime = TimeUtil::getTimeSec();

	player->setPlayerDailyResetData(dailyResetData);
	player->getPersistManager().setDirtyBit(DAILYRESETBIT);


}

void LogicSystem::checkPlayerDataFromDB(Player& player)
{

	Lynx::Map<UInt64, PlayerExpTemplate>::Iter * iter =	gPlayerExpTable->mMap.getMax();
	UInt64 maxLevel = iter->mKey;
	if (player.getPlayerLeval() > maxLevel)
	{
		player.setPlayerLeval(maxLevel);
		LOG_WARN("player level over table set level");
	}

	// 	PlayerVIPTemplateMap::Iter * iter1 =gVipTable->mapVip.getMax();
	// 	UInt64 maxvipLevel = iter1->mKey;
	// 	if (player.getVipLevel() > maxvipLevel)
	// 	{
	// 		player.setVipLevel(maxvipLevel);
	// 		LOG_WARN("player viplevel over table set viplevel");
	// 	}

}

bool LogicSystem::combinSendData(Guid playerID,UInt32 resType,Json::Value jvalue)
{
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	if (player == NULL)
	{
		return false;
	}

	PlayerFireConfirmData mFireConfirmData = player->GetFireConfirmData();

	if (mFireConfirmData.m_AddSendGoods.resourcestype == resType)
	{

		Json::Value son;   

		son["resourcestype"] = Json::Value(mFireConfirmData.m_AddSendGoods.resourcestype );
		son["subtype"] = Json::Value(mFireConfirmData.m_AddSendGoods.subtype );
		son["num"] = Json::Value(mFireConfirmData.m_AddSendGoods.num );
		son["attr"] = Json::Value(jvalue);

		mFireConfirmData.m_AddSendjs.append(son);

		mFireConfirmData.m_AddSendGoods.resourcestype = 0;
		mFireConfirmData.m_AddSendGoods.subtype = 0;
		mFireConfirmData.m_AddSendGoods.num = 0;
		player->SetFireConfirmData(mFireConfirmData);
		return true;
	}



	return false;

}

bool LogicSystem::combinSendDataTest(Guid playerID,UInt32 resType,const Json::Value &jvalue )
{
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	if (player == NULL)
	{
		return false;
	}

	PlayerFireConfirmData &mFireConfirmData = player->GetFireConfirmDataTest();

	if (mFireConfirmData.m_AddSendGoods.resourcestype == resType)
	{

		Json::Value son;   

		son["resourcestype"] = mFireConfirmData.m_AddSendGoods.resourcestype ;
		son["subtype"] = mFireConfirmData.m_AddSendGoods.subtype ;
		son["num"] = mFireConfirmData.m_AddSendGoods.num ;
		son["attr"].append(jvalue);
		//std::string str+= ;

		mFireConfirmData.m_AddSendjs.append(son);


		mFireConfirmData.m_AddSendGoods.resourcestype = 0;
		mFireConfirmData.m_AddSendGoods.subtype = 0;
		mFireConfirmData.m_AddSendGoods.num = 0;
		player->SetFireConfirmDataTest(mFireConfirmData);
		return true;
	}

	return false;
}

std::string LogicSystem::getDetailInfo(std::string playerUidStr)
{
	std::string detailInfo = "";	
#ifdef __linux__
	detailInfo =  RedisManager::getSingleton().get(playerUidStr);
#endif

	return detailInfo;
}

void  LogicSystem::setDetailInfo(UInt64 playerID, std::string detailInfo)
{
	String playerUidStr;
	char dest[64] = {};

	sprintf(dest,"%llu",playerID);
	playerUidStr = (String)dest;	

#ifdef __linux__

	RedisManager::getSingleton().set(playerUidStr, detailInfo);

#endif
}


bool LogicSystem::getBaseInfo(UInt64 playerUid, BaseInfoData & baseInfoData)
{
	Map<UInt64, BaseInfoData>::Iter * baseInfoIter = mPlayerBaseInfoMap.find(playerUid);
	if(baseInfoIter)
	{
		baseInfoData.playerUid = baseInfoIter->mValue.playerUid;
		baseInfoData.power = baseInfoIter->mValue.power;
		baseInfoData.name = baseInfoIter->mValue.name;
		baseInfoData.level = baseInfoIter->mValue.level;
		baseInfoData.vipLv = baseInfoIter->mValue.vipLv;
		baseInfoData.leaveTime = baseInfoIter->mValue.leaveTime;
		baseInfoData.modelId = baseInfoIter->mValue.modelId;
		return true;
	}
	else
	{
		baseInfoData.playerUid = 0;
		baseInfoData.power = 0;
		baseInfoData.name = "";
		baseInfoData.level = 0;
		baseInfoData.vipLv = 0;
		baseInfoData.leaveTime = 0;
		baseInfoData.modelId = 0;
		return false;
	}
}


const Map<UInt64, BaseInfoData> &LogicSystem::getBaseInfoMap()
{
	return mPlayerBaseInfoMap;
}

void LogicSystem::insertBaseInfo(const BaseInfoData & baseInfoData)
{
	BaseInfoData infoData = baseInfoData;
	mPlayerBaseInfoMap.insert(baseInfoData.playerUid, baseInfoData);

}

//已经判断用过的了sql里
Guid LogicSystem::listRandOne(Guid playerID,List<GuidValue> playerIDList,UInt32 flag,UInt32 initialValue)
{
	UInt32 count = 0;
	UInt32 i = 0;
	UInt32 getIt = 0;
	List<Guid> robotIDList;
	List<Guid> friendIDListLow;
	List<Guid> friendIDListHigh;
	List<Guid> IDListLow;
	List<Guid> IDListHigh;
	List<Guid> matchingPlayers;

	if(playerIDList.size() == 0)//空的时候有可能无限循环
	{
		return 0;
	}

	Player* player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	if (player == NULL )
	{
		return 0;
	}
	PlayerFireConfirmData mFireConfirmData =  player->GetFireConfirmData();
	mFireConfirmData.m_OtherPlayerID = 0;

	int low2level = player->getPlayerLeval() - 2 ;
	int high2level = player->getPlayerLeval() + 2 ;

	int low5level = player->getPlayerLeval() - 5 ;
	int high5level = player->getPlayerLeval() + 5 ;


	if (flag == 10001)//多人副本
	{
		matchingPlayers = player->getStageManager().getMatchingPlayers();
		for (List<GuidValue>::Iter * iter = playerIDList.begin();iter != NULL;iter = playerIDList.next(iter))
		{	
			getIt = 0;//去掉匹配过的
			Guid tmpPlayerID = iter->mValue.guid;
			for(List<Guid>::Iter *it =  matchingPlayers.begin();it!= NULL;it = matchingPlayers.next(it))
			{
				if (it->mValue == tmpPlayerID)
				{
					getIt = 1;
				}
			}
			if (getIt == 1)
			{
				continue;
			}


			if (iter->mValue.guid < RobotMaxRoleID && iter->mValue.guid > RobotMinRoleID)
			{
				robotIDList.insertTail(tmpPlayerID);
				continue;
			}	
			if (player->getFriendBlackManager().judgeFriend(iter->mValue.guid) == true)
			{	
				if(iter->mValue.value <= high2level && iter->mValue.value >= low2level)
				{
					friendIDListLow.insertTail(tmpPlayerID);
					continue;
				}
				else
				{
					friendIDListHigh.insertTail(tmpPlayerID);
					continue;
				}

			}
			else
			{
				if(iter->mValue.value <= high2level && iter->mValue.value >= low2level)
				{
					IDListLow.insertTail(tmpPlayerID);
					continue;
				}
				else
				{
					IDListHigh.insertTail(tmpPlayerID);
					continue;
				}	
			}

		}

		//顺序不能错
		if (friendIDListLow.size() != 0)
		{
			count = rand()% friendIDListLow.size();
			i = 0;
			for (List<Guid>::Iter * iter = friendIDListLow.begin();iter != NULL;iter = friendIDListLow.next(iter))
			{		
				if (i == count)
				{
					mFireConfirmData.m_OtherPlayerID = iter->mValue;
					player->SetFireConfirmData(mFireConfirmData);
					player->getStageManager().addMatchingPlayers(iter->mValue);
					return iter->mValue;
				}
				i++;		
			}		
		}
		if (IDListLow.size() != 0)
		{
			count = rand()% IDListLow.size();
			i = 0;
			for (List<Guid>::Iter * iter = IDListLow.begin();iter != NULL;iter = IDListLow.next(iter))
			{		
				if (i == count)
				{
					mFireConfirmData.m_OtherPlayerID = iter->mValue;
					player->SetFireConfirmData(mFireConfirmData);
					player->getStageManager().addMatchingPlayers(iter->mValue);
					return iter->mValue;
				}
				i++;		
			}		
		}

		if (friendIDListHigh.size() != 0)
		{
			count = rand()% friendIDListHigh.size();
			i = 0;
			for (List<Guid>::Iter * iter = friendIDListHigh.begin();iter != NULL;iter = friendIDListHigh.next(iter))
			{		
				if (i == count)
				{
					mFireConfirmData.m_OtherPlayerID = iter->mValue;
					player->SetFireConfirmData(mFireConfirmData);
					player->getStageManager().addMatchingPlayers(iter->mValue);
					return iter->mValue;
				}
				i++;		
			}		
		}


		if (IDListHigh.size() != 0)
		{
			count = rand()% IDListHigh.size();
			i = 0;
			for (List<Guid>::Iter * iter = IDListHigh.begin();iter != NULL;iter = IDListHigh.next(iter))
			{		
				if (i == count)
				{
					mFireConfirmData.m_OtherPlayerID = iter->mValue;
					player->SetFireConfirmData(mFireConfirmData);
					player->getStageManager().addMatchingPlayers(iter->mValue);
					return iter->mValue;
				}
				i++;		
			}		
		}

		if (robotIDList.size() != 0)
		{
			count = rand()% robotIDList.size();
			i = 0;
			for (List<Guid>::Iter * iter = robotIDList.begin();iter != NULL;iter = robotIDList.next(iter))
			{		
				if (i == count)
				{
					mFireConfirmData.m_OtherPlayerID = iter->mValue;
					player->SetFireConfirmData(mFireConfirmData);
					player->getStageManager().addMatchingPlayers(iter->mValue);
					return iter->mValue;
				}
				i++;		
			}		
		}
		player->getStageManager().resetMatchingPlayers();
		return  LogicSystem::getSingleton().listRandOne(playerID,playerIDList,flag,initialValue);
	}
	else 
	{//筛选范围为玩家战斗力+-3%的玩家数据，如筛选不到则扩大筛选范围为+-5%范围 	筛选范围最大为+-5%以内，如还筛选不到玩家的话，从符合条件的机器人内挑选数据，筛选范围同上


		matchingPlayers = player->getStageManager().getMatchingPlayersTwelvePalace();

		for (List<GuidValue>::Iter * iter = playerIDList.begin();iter != NULL;iter = playerIDList.next(iter))
		{	
			Guid tmpPlayerID = iter->mValue.guid;
			getIt = 0;

			for(List<Guid>::Iter *it =  matchingPlayers.begin();it!= NULL;it = matchingPlayers.next(it))
			{
				if (it->mValue == tmpPlayerID)
				{
					getIt = 1;
				}
			}
			if (getIt == 1)
			{
				continue;
			}

			if (iter->mValue.guid < RobotMaxRoleID && iter->mValue.guid > RobotMinRoleID)
			{
				robotIDList.insertTail(tmpPlayerID);
				continue;
			}	

			if(iter->mValue.value <= ( player->getPlayerPower() *(100 + 3) /100 ) && iter->mValue.value >= ( player->getPlayerPower() *(100 - 3) /100 ) )
			{
				IDListLow.insertTail(tmpPlayerID);
				continue;
			}
			else
			{
				IDListHigh.insertTail(tmpPlayerID);
				continue;
			}
		}

		//顺序不能错


		if (IDListLow.size() != 0)
		{
			count = rand()% IDListLow.size();
			i = 0;
			for (List<Guid>::Iter * iter = IDListLow.begin();iter != NULL;iter = IDListLow.next(iter))
			{		
				if (i == count)
				{
					mFireConfirmData.m_OtherPlayerID = iter->mValue;
					player->SetFireConfirmData(mFireConfirmData);
					player->getStageManager().addMatchingPlayersTwelvePalace(iter->mValue);
					return iter->mValue;
				}
				i++;		
			}		
		}

		if (IDListHigh.size() != 0)
		{
			count = rand()% IDListHigh.size();
			i = 0;
			for (List<Guid>::Iter * iter = IDListHigh.begin();iter != NULL;iter = IDListHigh.next(iter))
			{		
				if (i == count)
				{
					mFireConfirmData.m_OtherPlayerID = iter->mValue;
					player->SetFireConfirmData(mFireConfirmData);
					player->getStageManager().addMatchingPlayersTwelvePalace(iter->mValue);
					return iter->mValue;
				}
				i++;		
			}		
		}

		if (robotIDList.size() != 0)
		{
			count = rand()% robotIDList.size();
			i = 0;
			for (List<Guid>::Iter * iter = robotIDList.begin();iter != NULL;iter = robotIDList.next(iter))
			{		
				if (i == count)
				{
					mFireConfirmData.m_OtherPlayerID = iter->mValue;
					player->SetFireConfirmData(mFireConfirmData);
					player->getStageManager().addMatchingPlayersTwelvePalace(iter->mValue);
					return iter->mValue;
				}
				i++;		
			}		
		}

		//都找不着
		player->getStageManager().resetMatchingPlayersTwelvePalace();
		return  LogicSystem::getSingleton().listRandOne(playerID,playerIDList,flag,initialValue);

	}

	return 0;
}

void LogicSystem::dealSimililarPower(Guid playerID,Guid otherPlayerID,UInt32 times)
{

	std::string str;
	String playerUidStr;
	char dest[64] = {};
	TwelvePalaceStartResp resp;
	PlayerFireConfirmData fireConfirmData;

	Player* player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	if (player == NULL)
	{
		return;
	}

	if (otherPlayerID == 0&&times == 1)//匹配不到用机器人
	{
		PersistFindSimilarPowerReq req;
		req.playerGuid = player->getPlayerGuid();
		int low =  player->getPlayerLeval() - 5;
		if (low < 0)
		{
			low = 0;
		}
		int high = player->getPlayerLeval() + 5;

		req.low = low;
		req.high = high;
		req.times = 2;
		req.initialValue = player->getPlayerLeval();
		PersistSystem::getSingleton().postThreadMsg(req, 0);

	}

	if (otherPlayerID != 0)
	{
		sprintf(dest,"%d",otherPlayerID);
		playerUidStr = (String)dest;	
		str =  getDetailInfo(playerUidStr.c_str());
	}

	if (str == "")
	{
		LOG_WARN("dealSimililarPower playerid = %llu but not found DetailInfo",playerID);
	}	
	LOG_INFO("dealSimililarPower otherPlayerID = %llu DetailInfo = %s ",otherPlayerID,str.c_str());


	fireConfirmData = player->GetFireConfirmData();

	const  ConnId& connId = player->getConnId();
	if (connId == 0)
	{
		return;
	}

	resp.awardType = 1;
	resp.otherPlayerID = otherPlayerID;
	if( otherPlayerID >RobotMinRoleID && otherPlayerID <RobotMaxRoleID)
	{
		resp.isRobot = 1;
	}
	resp.otherPlayer = str;
	resp.id = fireConfirmData.m_CopyID;
	if (resp.otherPlayer == "")
	{
		resp.result = LynxErrno::RobotNotFound;
	}
	std::string jsonStr = resp.convertDataToJson();	
	NetworkSystem::getSingleton().sender( connId,TWELVE_PALACE_BEGIN_RESP,jsonStr);
}



void 
LogicSystem::kickOutAllPlayer()
{

	LOG_INFO("kickOutAllPlayer！！！  ");
	ClimbTowerManager::getSingleton().updateClimTower(1);
	// 踢人，存档
	for (Map<Guid, Player*>::Iter* iter = mPlayerGuidMap.begin();
		iter != NULL; iter = mPlayerGuidMap.next(iter))
	{
		Player* player = iter->mValue;
		if (player)
		{		

			onPersistOffLineSaveReq( player->getPlayerGuid());
			
			LOG_INFO("kickOutAllPlayer！！！  ");
		}
	}



}


Set<Player >  LogicSystem::getPlayerLvSet(Player * selfPlayer)
{
	Map<UInt32,  Set<Player > > playerLvSet;

	for(Map<ConnId, Player*>::Iter * playerIter = mPlayerConnectionMap.begin();  playerIter != NULL;  
		playerIter = mPlayerConnectionMap.next(playerIter))
	{
		UInt32 level = playerIter->mValue->getPlayerLeval();
		Map<UInt32,  Set<Player > >::Iter * setFindIter =  playerLvSet.find(level);
		if(setFindIter)
		{
			setFindIter->mValue.insert(*(playerIter->mValue));
		}
		else
		{
			Set<Player > playerSet;
			playerSet.insert(*(playerIter->mValue));
			playerLvSet.insert(level, playerSet);
		}
	}

	Json::Value root;

	int findCount = 0;
	Set<Player> setReturn;

	UInt64 selfUid = selfPlayer->getPlayerGuid();	
	UInt32 selfLv = selfPlayer->getPlayerLeval();

	//先找等级在上下不超过5以内的
	for(Map<UInt32,  Set<Player > >::Iter* playerLvIter = playerLvSet.begin();  playerLvIter != NULL; 
		playerLvIter = playerLvSet.next(playerLvIter))
	{
		UInt32 otherLv = playerLvIter->mKey;

		UInt32 lvDiff = 0;
		if(otherLv < selfLv)
		{
			lvDiff = selfLv - otherLv;
		}
		else
		{
			lvDiff = otherLv - selfLv;
		}

		if(lvDiff > 5 )
		{
			continue;
		}

		//找到不是等级对应的map
		for(Set<Player >::Iter * setIter = playerLvIter->mValue.begin();  setIter != NULL; 
			setIter = playerLvIter->mValue.next(setIter)  )
		{
			UInt64 otherUid = setIter->mValue.getPlayerGuid();

			//是自己，跳过
			if(selfUid == otherUid)
			{
				continue;
			}

			//判断是否是好友
			bool isFriend = selfPlayer->getFriendBlackManager().judgeFriend(otherUid);
			if(isFriend)
			{
				continue;
			}

			findCount++;
			setReturn.insert(setIter->mValue);
			if(findCount == 10)
			{
				return setReturn;
			}
		}
	}


	//找范围以外的
	for(Map<UInt32,  Set<Player > >::Iter* playerLvIter = playerLvSet.begin();  playerLvIter != NULL; 
		playerLvIter = playerLvSet.next(playerLvIter))
	{
		UInt32 lvDiff = 0;
		UInt32 otherLv = playerLvIter->mKey;
		if(otherLv < selfLv)
		{
			lvDiff = selfLv - otherLv;
		}
		else
		{
			lvDiff = otherLv - selfLv;
		}

		if(lvDiff <= 5 )
		{
			continue;
		}

		//找到不是等级对应的map
		for(Set<Player >::Iter * setIter = playerLvIter->mValue.begin();  setIter != NULL; 
			setIter = playerLvIter->mValue.next(setIter)  )
		{

			UInt64 otherUid = setIter->mValue.getPlayerGuid();

			bool isFriend = selfPlayer->getFriendBlackManager().judgeFriend(otherUid);
			if(isFriend)
			{
				continue;
			}

			if(otherUid == selfUid)
			{
				continue;
			}

			findCount++;
			setReturn.insert(setIter->mValue);
			if(findCount == 10)
			{
				return setReturn;
			}
		}
	}


	return setReturn;
}


void LogicSystem::sendSystemMsg(UInt64 tempId, std::string param1, UInt32 param2)
{
	//chang by cqy

	GCClientChatResp clientChatResp;
	clientChatResp.mPacketID = BOC_CLIENTCHAT_RESP;

	Json::Value root;
	root["channelType"] = SYSTEMMSG;
	root["fromPlayer"] = 0;
	root["chatMsg"] = "";
	root["modelid"] =0;
	root["name"] = "";
	root["chattime"] = TimeUtil::getTimeSec();
	root["msgType"] = 0;
	root["tempId"] = tempId;

	root["param1"] = param1;
	root["param2"] = param2;

	Json::StyledWriter writer;

	clientChatResp.mRespJsonStr = writer.write(root);

	const Map<ConnId, Player*> connectionMap = LogicSystem::getSingleton().getPlayerConnectionMap();

	LOG_INFO("connectionMap size = %d ",connectionMap.size());
	LOG_INFO("mPlayerConnectionMap size = %d ",mPlayerConnectionMap.size());



	for( Map<ConnId, Player*>::Iter* mapIter = connectionMap.begin(); mapIter != NULL; 
		mapIter = connectionMap.next(mapIter))
	{



		cout << clientChatResp.mRespJsonStr << endl;

		NetworkSystem::getSingleton().sendMsg(clientChatResp, mapIter->mKey);


	}
}


void LogicSystem::sendGMSystemMsg(std::string msg)
{
	GCClientChatResp clientChatResp;
	clientChatResp.mPacketID = BOC_CLIENTCHAT_RESP;

	Json::Value root;
	root["channelType"] = GMMSG;
	root["fromPlayer"] = 0;
	root["chatMsg"] = msg;
	root["modelid"] =0;
	root["name"] = "";
	root["chattime"] = TimeUtil::getTimeSec();
	root["msgType"] = 1;
	

	Json::StyledWriter writer;

	clientChatResp.mRespJsonStr = writer.write(root);

	const Map<ConnId, Player*> connectionMap = LogicSystem::getSingleton().getPlayerConnectionMap();

	LOG_INFO("connectionMap size = %d ",connectionMap.size());
	LOG_INFO("mPlayerConnectionMap size = %d ",mPlayerConnectionMap.size());

	for( Map<ConnId, Player*>::Iter* mapIter = connectionMap.begin(); mapIter != NULL; 
		mapIter = connectionMap.next(mapIter))
	{

		NetworkSystem::getSingleton().sendMsg(clientChatResp, mapIter->mKey);
	}
}

void LogicSystem::sendGMAwardBag(UInt64 playeruid, UInt64 awardID)
{
	List<Goods> tmpItemList;
	List<Goods> itemList;
	
	GiftManager::getSingleton().getAwardByID(awardID,0,tmpItemList);
	for(List<Goods>::Iter *it = tmpItemList.begin();it!= NULL;it = tmpItemList.next(it))
	{
		GiftManager::getSingleton().getContentByID(it->mValue.subtype,itemList);
	}
	GiftManager::getSingleton().combineSame(itemList);

	char  des[2048] = {};
	char dest[2048] = {};
	String str1;
	String str2;
	char tmp[64] = {};
	sprintf(des,"兑换码奖励;系统;你获得的奖励有：;");
	for(List<Goods>::Iter * item = itemList.begin();item!=NULL;item = itemList.next(item))
	{
		sprintf(tmp,"(%d,%d,%d);",item->mValue.resourcestype,item->mValue.subtype,item->mValue.num);
		strcat(dest,tmp);
	}
	str1 = (String)des;
	str2 = (String)dest;

	str1 = InlineActivityManager::getSingleton().stringToUtf(str1);
	//sdasd;asdsadasd;asdasdasdas;####(1,2,100);(1,1,1000):(1,2,1000)
	//发送人;标题;正文;奖励用四个####隔开，后面（1,2,100）你知道的格式		

	GMManager::getSingleton().GMEmailSend(0, playeruid, str1.c_str(),str2.c_str());
}


void LogicSystem::setForbidLogin(UInt32 i)
{
	mForbidLogin = i;
}

void LogicSystem::sendGMMsg(std::string msg,  UInt64 endtime, UInt32 term)
{
	GMMsg gmMsg;
	gmMsg.m_nTerminal = term;
	gmMsg.m_nLastTime = 0;
	gmMsg.m_nEndTime = endtime;
	gmMsg.m_strGMMsg = msg;

	mGMMsgList.insertTail(gmMsg);
}

void LogicSystem::gmResetTongBao(UInt64 playeruid)
{
   Player* player = getPlayerByGuid(playeruid);
  
  if(player)
  {
	 PlayerDailyResetData dailyResetData =  player->getPlayerDailyResetData();
	 dailyResetData.m_nDailyChaiCount = 0;
	 player->setPlayerDailyResetData(dailyResetData);

  }
 
  PersistGMResetTongbao tongbao;
  tongbao.playeruid = playeruid;
  tongbao.reset = 0;
  PersistSystem::getSingleton().postThreadMsg(tongbao, playeruid);

}

void LogicSystem::gmResetMengChu(UInt64 playeruid)
{	
	 Player* player = getPlayerByGuid(playeruid);
  if(player)
  {
	PlayerDailyResetData dailyResetData =  player->getPlayerDailyResetData();
	dailyResetData.m_nDailyMiCount = 0;
	player->setPlayerDailyResetData(dailyResetData);
  }
  

  PersistGMResetMengchu tongbao;
  tongbao.playeruid = playeruid;
  tongbao.reset = 0;
  PersistSystem::getSingleton().postThreadMsg(tongbao, playeruid);
}


void LogicSystem::gmResetYushiGF(UInt64 playeruid)
{
	 Player* player = getPlayerByGuid(playeruid);
    if(player)
	{
		PlayerDailyResetData dailyResetData =  player->getPlayerDailyResetData();
		dailyResetData.m_nDailyYouCount = 0;
		player->setPlayerDailyResetData(dailyResetData);
	}
  

  PersistGMResetYushiGF tongbao;
  tongbao.playeruid = playeruid;
  tongbao.reset = 0;
  PersistSystem::getSingleton().postThreadMsg(tongbao, playeruid);
}

void LogicSystem::gmResetJiShiAB(UInt64 playeruid)
{
	 Player* player = getPlayerByGuid(playeruid);
  if(player)
  {
		PlayerDailyResetData dailyResetData =  player->getPlayerDailyResetData();
  dailyResetData.m_nDailyYanCount = 0;
  player->setPlayerDailyResetData(dailyResetData);
  }
  

  PersistGMResetJiShiAB tongbao;
  tongbao.playeruid = playeruid;
  tongbao.reset = 0;
  PersistSystem::getSingleton().postThreadMsg(tongbao, playeruid);
}

void LogicSystem::gmResetBashanSL(UInt64 playeruid)
{
	Player* player = getPlayerByGuid(playeruid);
  if(player)
  {
		PlayerDailyResetData dailyResetData =  player->getPlayerDailyResetData();
  dailyResetData.m_nDailyMultipleCopyCount = 0;
  player->setPlayerDailyResetData(dailyResetData);
  }
  

  PersistGMResetBashanSL tongbao;
  tongbao.playeruid = playeruid;
  tongbao.reset = 0;
  PersistSystem::getSingleton().postThreadMsg(tongbao, playeruid);
}

void LogicSystem::gmResetWuxianTZ(UInt64 playeruid)
{
	
	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();
	

	Player* player = getPlayerByGuid(playeruid);
  if(player)
  {
		 PlayerDailyResetData dailyResetData =  player->getPlayerDailyResetData();
  dailyResetData.m_nClimbTowerTimes = globalValue.uTPresetlowerlimit;		
  player->setPlayerDailyResetData(dailyResetData);
  }
 

  PersistGMResetWXTZ tongbao;
  tongbao.playeruid = playeruid;
  tongbao.reset = globalValue.uTPresetlowerlimit;
  PersistSystem::getSingleton().postThreadMsg(tongbao, playeruid);
}

void LogicSystem::gmResetCourage(UInt64 playeruid)
{
		GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();
		Player* player = getPlayerByGuid(playeruid);
		if(player)
		{
				PlayerCourageChallengeData courageChallengeData = player->getCourageChallengeData();
	

	courageChallengeData.m_LeftChallengeTimes = globalValue.uTERRITORYLmaxtimes;
	player->setCourageChallengeData(courageChallengeData);

		}
	 
  PersistGMCourage tongbao;
  tongbao.playeruid = playeruid;
  tongbao.reset = globalValue.uTERRITORYLmaxtimes;
  PersistSystem::getSingleton().postThreadMsg(tongbao, playeruid);

}

void LogicSystem::gmResetPaiWei(UInt64 playeruid)
{
	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();
	

	Player* player = getPlayerByGuid(playeruid);
  if(player)
  {
		PlayerDailyResetData dailyResetData =  player->getPlayerDailyResetData();
  dailyResetData.m_nRankGameLeftTimes    = globalValue.uRankGameTimes;		
  player->setPlayerDailyResetData(dailyResetData);
  }
  

  PersistGMPaiWei tongbao;
  tongbao.playeruid = playeruid;
  tongbao.reset = globalValue.uRankGameTimes;
  PersistSystem::getSingleton().postThreadMsg(tongbao, playeruid);
}

void LogicSystem::gmResetPaiWeiBuy(UInt64 playeruid)
{
	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();
	

	Player* player = getPlayerByGuid(playeruid);
  if(player)
  {
		PlayerDailyResetData dailyResetData =  player->getPlayerDailyResetData();
  dailyResetData.m_nRankGameBuyTimes    = globalValue.uRankGameTimes;		
  player->setPlayerDailyResetData(dailyResetData);
  }
  

  PersistGMPaiWeiBuy tongbao;
  tongbao.playeruid = playeruid;
  tongbao.reset = 0;
  PersistSystem::getSingleton().postThreadMsg(tongbao, playeruid);
}

void LogicSystem::gmReset12HaoJiao(UInt64 playeruid)
{
	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();


	Player* player = getPlayerByGuid(playeruid);
	if(player)
	{
		PlayerDailyResetData dailyResetData =  player->getPlayerDailyResetData();
		dailyResetData.m_nTwelvePalaceUnlockCount = globalValue.uTPresetlowerlimit;
		player->setPlayerDailyResetData(dailyResetData);
		player->getPersistManager().setDirtyBit(DAILYRESETBIT);

	}


	PersistGM12HaoJiao tongbao;
	tongbao.playeruid = playeruid;
	tongbao.reset = globalValue.uTPresetlowerlimit;
	PersistSystem::getSingleton().postThreadMsg(tongbao, playeruid);


}

void LogicSystem::gmResetFoodsCook(UInt64 playeruid)
{
		Player* player = getPlayerByGuid(playeruid);
		if(player)
		{
					PlayerFoodsData foodsData = player->getFoodsData();//位置不能错
					foodsData.beginTime =0;
			player->setFoodsData(foodsData);
			player->getPersistManager().setDirtyBit(FOODDATABIT );
		}

  PersistGMCookFood tongbao;
  tongbao.playeruid = playeruid;
  tongbao.reset = 0;
  PersistSystem::getSingleton().postThreadMsg(tongbao, playeruid);
}

//赏罚令免费抽一次
void LogicSystem::gmResetServantCall1(UInt64 playeruid)
{
		Player* player = getPlayerByGuid(playeruid);
		if(player)
		{
			PlayerCounterData counterData = player->GetPlayerCounterData();
			counterData.m_RewardLotteryOnceTime = 0;
			player->setPlayerCounterData(counterData);
			player->getPersistManager().setDirtyBit(COUNTERDATABIT);
		}
	
		 PersistGMServantOnce tongbao;
  tongbao.playeruid = playeruid;
  tongbao.reset = 0;
  PersistSystem::getSingleton().postThreadMsg(tongbao, playeruid);
}

void LogicSystem::gmResetServantCall10(UInt64 playeruid)
{
		Player* player = getPlayerByGuid(playeruid);
		if(player)
		{
			PlayerCounterData counterData = player->GetPlayerCounterData();
			counterData.m_RewardLotteryTenTime = 0;
			player->setPlayerCounterData(counterData);
			player->getPersistManager().setDirtyBit(COUNTERDATABIT);
		}
	
		 PersistGMServantTen tongbao;
  tongbao.playeruid = playeruid;
  tongbao.reset = 0;
  PersistSystem::getSingleton().postThreadMsg(tongbao, playeruid);
}

void LogicSystem::gmResetDailyTask(UInt64 playeruid)
{
	Player* player = getPlayerByGuid(playeruid);
		if(player)
		{
		
				PlayerDailyTaskData dailyTaskData = player->getDailyTaskData();
				for(	List<DailyTaskData>::Iter * dailyIter = dailyTaskData.dailytaskList.begin();
					dailyIter != NULL; dailyIter = dailyTaskData.dailytaskList.next(dailyIter)  )
				{
					dailyIter->mValue.m_nArg = 0;
					dailyIter->mValue.m_nFlag = 0;
				}
			
		
		}
	
	

		 PersistGMResetDailyTask tongbao;
  tongbao.playeruid = playeruid;
  tongbao.reset = 0;
  PersistSystem::getSingleton().postThreadMsg(tongbao, playeruid);
}


void LogicSystem::gmResetMonthToday(UInt64 playeruid)
{
	Player* player = getPlayerByGuid(playeruid);
		if(player)
		{
		
				PlayerInlineActivityData inlineActivityData = player->getInlineActivityData();
				inlineActivityData.m_MonthSignTime = time(0) - 24*3600;
				player->setInlineActivityData(inlineActivityData);
		
		}
	
		 PersistGMResetMonthSign tongbao;
         tongbao.playeruid = playeruid;
         tongbao.reset = time(0) - 24*3600;
         PersistSystem::getSingleton().postThreadMsg(tongbao, playeruid);
}

void LogicSystem::gmResetSevenDay(UInt64 playeruid)
{
	Player* player = getPlayerByGuid(playeruid);
		if(player)
		{
		
				PlayerInlineActivityData inlineActivityData = player->getInlineActivityData();
				inlineActivityData.m_SevenLoginGotCount  = 0;
				player->setInlineActivityData(inlineActivityData);
		
		}
	
		 PersistGMResetSevenday tongbao;
         tongbao.playeruid = playeruid;
         tongbao.reset = 0;
         PersistSystem::getSingleton().postThreadMsg(tongbao, playeruid);
}

void LogicSystem::gmResetSevenTrain(UInt64 playeruid)
{
	Player* player = getPlayerByGuid(playeruid);
		if(player)
		{
		
				PlayerInlineActivityData inlineActivityData = player->getInlineActivityData(); 
				inlineActivityData.m_FirstLoginTime   = time(0);
				player->setInlineActivityData(inlineActivityData);
		
		}
	
		 PersistGMResetSeventrain tongbao;
         tongbao.playeruid = playeruid;
         tongbao.reset = time(0);
         PersistSystem::getSingleton().postThreadMsg(tongbao, playeruid);
}

void LogicSystem::gmResetGrowBuy(UInt64 playeruid)
{
	Player* player = getPlayerByGuid(playeruid);
		if(player)
		{
		
				PlayerInlineActivityData inlineActivityData = player->getInlineActivityData(); 
				inlineActivityData.m_GrowFoundBuyFlag   = 0;
				player->setInlineActivityData(inlineActivityData);
		
		}
	
		
		 PersistGMResetGrowfound tongbao;
         tongbao.playeruid = playeruid;
         tongbao.reset = GROWTH_FOUND;
         PersistSystem::getSingleton().postThreadMsg(tongbao, playeruid);

}

void LogicSystem::gmResetFenShi(UInt64 playeruid)
{

		Player* player = getPlayerByGuid(playeruid);
		if(player)
		{
		
				PlayerInlineActivityData inlineActivityData = player->getInlineActivityData(); 
				inlineActivityData.m_TimeAwardRefreshTime   = 0;
				player->setInlineActivityData(inlineActivityData);
		
		}
		

 PersistGMResetFenshi tongbao;
         tongbao.playeruid = playeruid;
         tongbao.reset = 0;
         PersistSystem::getSingleton().postThreadMsg(tongbao, playeruid);
}

void LogicSystem::gmResetOnlineAward(UInt64 playeruid)
{
	Player* player = getPlayerByGuid(playeruid);
		if(player)
		{
		
				PlayerInlineActivityData inlineActivityData = player->getInlineActivityData(); 
				inlineActivityData.m_OnlineWelFareGotList.clear();
				player->setInlineActivityData(inlineActivityData);
		
		}
		

 PersistGMResetOnlineAward tongbao;
         tongbao.playeruid = playeruid;
         tongbao.reset = ONLINE_WELFARE;
         PersistSystem::getSingleton().postThreadMsg(tongbao, playeruid);

}

void LogicSystem::gmResetBaiCaiShen(UInt64 playeruid)
{
	Player* player = getPlayerByGuid(playeruid);
		if(player)
		{
		
				PlayerDailyResetData inlineActivityData = player->getPlayerDailyResetData(); 
				inlineActivityData.m_nCoinBuyTimes = 0;
				inlineActivityData.m_nCoinFreeBuyTimes = 0;
				player->setPlayerDailyResetData(inlineActivityData);
		
		}
		

 PersistGMResetBaiCaiShen tongbao;
         tongbao.playeruid = playeruid;
         tongbao.reset = 0;
         PersistSystem::getSingleton().postThreadMsg(tongbao, playeruid);
}


void LogicSystem::gmResetStrengthBuy(UInt64 playeruid)
{
	Player* player = getPlayerByGuid(playeruid);
		if(player)
		{
		
			PlayerStrengthData strengthData = player->getStrengthData();
			strengthData.vipLeftTimes =  ShopManager::getSingleton().getStrengthBuyMaxTimes(player->getVipLevel());
			player->setStrengthData(strengthData);
		
		}
		

 PersistGMResetBuyStrength tongbao;
         tongbao.playeruid = playeruid;
         tongbao.reset = ShopManager::getSingleton().getStrengthBuyMaxTimes(player->getVipLevel());
         PersistSystem::getSingleton().postThreadMsg(tongbao, playeruid);
}


void LogicSystem::gmResetLianPu(UInt64 playeruid)
{
	Player* player = getPlayerByGuid(playeruid);
		if(player)
		{
		
				ShopManager::getSingleton().setShopItems(playeruid,RankGameShop,0,0,true);
		
		}
		

 PersistGMResetLianPu tongbao;
         tongbao.playeruid = playeruid;
         tongbao.reset = RankGameShop ;
         PersistSystem::getSingleton().postThreadMsg(tongbao, playeruid);
}

void LogicSystem::gmResetTongBaoS(UInt64 playeruid)
{
	Player* player = getPlayerByGuid(playeruid);
		if(player)
		{
		
				ShopManager::getSingleton().setShopItems(playeruid,CourageShop ,0,0,true);
		
		}
		

 PersistGMResetTB tongbao;
         tongbao.playeruid = playeruid;
         tongbao.reset = CourageShop  ;
         PersistSystem::getSingleton().postThreadMsg(tongbao, playeruid);
}

void LogicSystem::gmResetZBG(UInt64 playeruid)
{
	Player* player = getPlayerByGuid(playeruid);
		if(player)
		{
		
				ShopManager::getSingleton().setShopItems(playeruid,MysticalShop,0,0,true);
		
		}
		

 PersistGMResetTB tongbao;
         tongbao.playeruid = playeruid;
         tongbao.reset = MysticalShop ;
         PersistSystem::getSingleton().postThreadMsg(tongbao, playeruid);
}

void LogicSystem::gmResetServantShop(UInt64 playeruid)
{
	Player* player = getPlayerByGuid(playeruid);
		if(player)
		{
		
				ShopManager::getSingleton().setShopItems(playeruid,ServantShop,0,0,true);
		
		}
		

 PersistGMResetTB tongbao;
         tongbao.playeruid = playeruid;
         tongbao.reset = ServantShop ;
         PersistSystem::getSingleton().postThreadMsg(tongbao, playeruid);
}


void LogicSystem::logPlayerLogin(PlayerBaseData baseData)
{
	char dest[2048];

	LoggerPlayerTypeLogReq req;

	ServerPublicData &serverPublicData = ServerData::getSingleton().getServerPublicData();

	req.playerID = baseData.m_nPlayerID;
	req.name = baseData.m_strPlayerName.c_str();
	req.accountID = baseData.m_accountID;
	req.serverID = serverPublicData.gServerID;
	req.type = LOG_LOGIN;

	tm tm_time;
	TimeUtil::localTime(tm_time,-1);
	Int32 time = TimeUtil::getDate(-1);

	snprintf(dest,sizeof(dest),"player_log_%0.4u%0.2u%0.2u",1900+tm_time.tm_year,1+tm_time.tm_mon,tm_time.tm_mday);

	req.tableName = (string)dest;

	LoggerSystem::getSingleton().postThreadMsg(req,0);

	char dest1[1024]={};
	LogicSystem::getSingleton().write_log( LogType56,baseData.m_nPlayerID, dest1,LogInfo);

	Player * player = LogicSystem::getSingleton().getPlayerByGuid(baseData.m_nPlayerID);
	if (player == NULL)
	{
		return;
	}
	PlayerInlineActivityData inlineActivityData = player->getInlineActivityData();
	UInt32 nowDayBeiginTime =  TimeUtil::getTimeSec() - TimeUtil::getSecOfToday();
	UInt32 oldDayBeiginTime = inlineActivityData.m_FirstLoginTime  - TimeUtil::getSecOfToThatday( inlineActivityData.m_FirstLoginTime);

	double dDay = (nowDayBeiginTime - oldDayBeiginTime)/(24*3600 -10) +1;
	UInt32 day = (UInt32)dDay;
	if (day <= 30)
	{
		PlayerOnlineDay mPlayerOnlineDay;
		mPlayerOnlineDay = player->getPlayerOnlineDay();

		switch (day)
		{
		case  1:
			mPlayerOnlineDay.day1 = 1;
			break;
		case  2:
			mPlayerOnlineDay.day2 = 1;
			break;
		case  3:
			mPlayerOnlineDay.day3 = 1;
			break;
		case  4:
			mPlayerOnlineDay.day4 = 1;
			break;
		case  5:
			mPlayerOnlineDay.day5 = 1;
			break;
		case  6:
			mPlayerOnlineDay.day6= 1;
			break;
		case  7:
			mPlayerOnlineDay.day7 = 1;
			break;
		case  8:
			mPlayerOnlineDay.day8 = 1;
			break;
		case  9:
			mPlayerOnlineDay.day9 = 1;
			break;
		case  10:
			mPlayerOnlineDay.day10 = 1;
			break;
		case  11:
			mPlayerOnlineDay.day11 = 1;
			break;
		case  12:
			mPlayerOnlineDay.day12 = 1;
			break;
		case  13:
			mPlayerOnlineDay.day13 = 1;
			break;
		case  14:
			mPlayerOnlineDay.day14 = 1;
			break;
		case  15:
			mPlayerOnlineDay.day15 = 1;
			break;
		case  16:
			mPlayerOnlineDay.day16 = 1;
			break;
		case  17:
			mPlayerOnlineDay.day17 = 1;
			break;
		case  18:
			mPlayerOnlineDay.day18 = 1;
			break;
		case  19:
			mPlayerOnlineDay.day19 = 1;
			break;
		case  20:
			mPlayerOnlineDay.day20 = 1;
			break;
		case  21:
			mPlayerOnlineDay.day21 = 1;
			break;
		case  22:
			mPlayerOnlineDay.day22 = 1;
			break;
		case  23:
			mPlayerOnlineDay.day23 = 1;
			break;
		case  24:
			mPlayerOnlineDay.day24 = 1;
			break;
		case  25:
			mPlayerOnlineDay.day25 = 1;
			break;
		case  26:
			mPlayerOnlineDay.day26 = 1;
			break;
		case  27:
			mPlayerOnlineDay.day27 = 1;
			break;
		case  28:
			mPlayerOnlineDay.day28 = 1;
			break;
		case  29:
			mPlayerOnlineDay.day29 = 1;
			break;
		case  30:
			mPlayerOnlineDay.day30 = 1;
			break;
		
		default:
			break;
		}


		player->setPlayerOnlineDay(mPlayerOnlineDay);


		PersistOnlineDaySave req2;
		req2.m_nPlayerUid = baseData.m_nPlayerID;

		PersistSystem::getSingleton().postThreadMsg(req2, 0);
	}



	ServerData::getSingleton().addOnlineNum();
}

void LogicSystem::logPlayerLogout(PlayerBaseData baseData)
{
	char dest[2048];

	LoggerPlayerTypeLogReq req;

	ServerPublicData &serverPublicData = ServerData::getSingleton().getServerPublicData();

	req.playerID = baseData.m_nPlayerID;
	req.name = baseData.m_strPlayerName.c_str();
	req.accountID = baseData.m_accountID;
	req.serverID = serverPublicData.gServerID;
	req.type = LOG_LOGOUT;

	tm tm_time;
	TimeUtil::localTime(tm_time,-1);
	Int32 time = TimeUtil::getDate(-1);

	snprintf(dest,sizeof(dest),"player_log_%0.4u%0.2u%0.2u",1900+tm_time.tm_year,1+tm_time.tm_mon,tm_time.tm_mday);

	req.tableName = (string)dest;

	LoggerSystem::getSingleton().postThreadMsg(req,0);

	LOG_DEBUG(" logout log playerid  =  %lld", baseData.m_nPlayerID );



	char dest1[1024] = {0};
	LogicSystem::getSingleton().write_log(LogType57, baseData.m_nPlayerID, dest1,LogInfo);

	Player * player = LogicSystem::getSingleton().getPlayerByGuid(baseData.m_nPlayerID);
	if (player == NULL)
	{
		return;
	}
	PlayerInlineActivityData inlineActivityData = player->getInlineActivityData();
	char dest2[1024]={0};
	UInt32 timeLong = TimeUtil::getTimeSec() - inlineActivityData.m_LoginTime;
	snprintf(dest2,sizeof(dest2),"%d",timeLong);
	LogicSystem::getSingleton().write_log( LogType58,baseData.m_nPlayerID, dest2,LogInfo);

	ServerData::getSingleton().subOnlineNum();





}


void  LogicSystem::updateSevenDayTask(Guid playerID,UInt32 eventType,UInt32 change)
{
	InlineActivityManager::getSingleton().updateSevenDayTaskData(playerID,eventType,change);
}




void LogicSystem::initRoleInfoReq(UInt64 playerID,std::string name,UInt32 modleID)
{
	std::string detailInfo ;
	UInt32 playerPower = 0;	
	String detailStr;
	char dest[2048] ={};
	char dest1[64] ={};
	String roleStr;
	String nameStr;
	String src;
	String outPutStr;

	LOG_INFO("initRoleInfoReq 1" );

	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();

	sprintf(dest1,"%llu",playerID);
	roleStr = (String)dest1;
	nameStr = name.c_str();

	if (modleID == 1)
	{
		detailInfo = globalValue.sRole1;
		playerPower = globalValue.uRolePower1;
	}
	if (modleID == 2)
	{
		detailInfo = globalValue.sRole2;
		playerPower = globalValue.uRolePower2;
	}
	if (modleID == 3)
	{
		detailInfo = globalValue.sRole3;
		playerPower = globalValue.uRolePower3;
	}

	src = detailInfo.c_str();
	detailInfo = "";

	UInt32 cpyPos = 0;
	UInt32 firstTypeFlag=0;
	UInt32 secondTypeFlag=0;
	UInt32 count = src.length();
//ascii  44 ,  34 "
	for (UInt32 i =0; i < count; i++)
	{
		if (src[i] == 44 )
		{
			firstTypeFlag ++;		
		}
		if (src[i] == 34)
		{
			secondTypeFlag++;
		}
		if (firstTypeFlag == 1)
		{
			dest[cpyPos] = src[i];
			cpyPos++;
			for (UInt32 ii =0; ii < roleStr.length(); ii++)
			{
				dest[cpyPos] = roleStr[ii];
				cpyPos++;
			}
			firstTypeFlag++;
			continue;
		}
		if (firstTypeFlag == 2)
		{
			continue;
		}

		if (secondTypeFlag == 1)
		{
			dest[cpyPos] = src[i];
			cpyPos++;
			for (UInt32 iii =0; iii < nameStr.length(); iii++)
			{
				dest[cpyPos] = nameStr[iii];
				cpyPos++;
			}
			secondTypeFlag++;
			continue;
		}	
		if (secondTypeFlag == 2)
		{
			continue;
		}
		dest[cpyPos] = src[i];
		cpyPos++;		
	}

	detailInfo =(string) dest;	

	LogicSystem::getSingleton().setDetailInfo(playerID, detailInfo);
	LOG_INFO("initRoleInfoReq setDetailInfo" );

}

void LogicSystem::write_log(UInt32 logType,UInt64 playerID, char * dest,UInt32 logLevel)
{
	char logChar[2048];
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	if (player == NULL)
	{
		return;
	}
	
	ServerPublicData  serverPublicData = ServerData::getSingleton().getServerPublicData();
	snprintf(logChar,sizeof(logChar),"%d,%d,%s,%llu,%d,%d,%d,%s",logType,serverPublicData.gServerID ,player->getPlayerName().c_str(),playerID,player->getPlayerPower(),player->getPlayerLeval(),player->getVipLevel(),dest);
	 Logcxx::getSingleton().log_writeFile(logChar,logLevel);
}

void LogicSystem::write_server_log(UInt32 logType,char * dest,UInt32 logLevel)
{
	char logChar[2048];


	ServerPublicData  serverPublicData = ServerData::getSingleton().getServerPublicData();
	snprintf(logChar,sizeof(logChar),"%d,%d,%s",logType,serverPublicData.gServerID ,dest);
	Logcxx::getSingleton().log_writeFile(logChar,logLevel);
}

void LogicSystem::setLoginTime(UInt64 timemill)
{
	gLoginTime = timemill;

}

UInt64 LogicSystem::getLoinTime()
{
	return gLoginTime;

}


void LogicSystem::onPersistOffLineSaveReq(UInt64 playerUid)
{
	Player* mPlayer	= LogicSystem::getSingleton().getPlayerByGuid(playerUid);

	if(!mPlayer)
	{
		LOG_WARN("Failed to destroy player [%llu] for player isn't exist.", playerUid);
		return;
	}

	mPlayer->setLastPostState(1);

	mPlayer->setDelDirty();

	UInt64 connectCloseTime = mPlayer->getConnectCloseTime();
	if(!connectCloseTime)
	{
		return;
	}


	if (mPlayer->getConnId() != 0)
	{
		LogicSystem::getSingleton().closeConnection(mPlayer);
		LOG_INFO("onPersistOffLineSaveReq connId = %d ",mPlayer->getConnId());
	}


	//检测不同模块dirty变量，并且存盘
	PersistSystem::getSingleton().SaveAllDirty();
}