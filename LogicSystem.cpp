#include "LogicSystem.h"
#include "GameServer.h"
#include "RedisManager.h"
#include "LoginMsgHandler.h"
#include "RankSystem.h"

using namespace Lynx;

LogicSystem::LogicSystem() : mPlayerGuidBase(0), mServerId(0),mLastSyncGuidBaseTime(0),mItemGuidBase(0),
							mLansquenetGuid(0),mGuildGuid(0),mEmailGuid(0),mMaterialGuid(0), mLastRestGuidBaseTime(0)
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
	
    LOG_INFO("Initial LogicSystem");
        
    return true;
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


static UInt64 serverBeginTime = 0;
static UInt32 gCount = 0;
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

		PersistOffLineSaveReq saveReq;
		saveReq.mPlayerUid = *iterVec;

		PersistSystem::getSingleton().postThreadMsg(saveReq, saveReq.mPlayerUid);
		player->setLastPostState(1);
    }


    // 同步GUIDBase
    if (accTime - mLastSyncGuidBaseTime >= SAVE_INV_TIME)//这个5000下面也要用到
    {
        mLastSyncGuidBaseTime = accTime;
		baseUidPersist();

		dailyRefresh(accTime);

		ClimbTowerManager::getSingleton().updateClimTower(0);
    }
	
	if(accTime - mLastRestGuidBaseTime > RESET_INV_TIME )
	{
			mLastRestGuidBaseTime = accTime;
			//检测在线玩家每日重置刷新情况,对于要刷新的玩家发送刷新通知
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
	PersistSystem::getSingleton().postThreadMsg(notify, 0);
}


//每天0时4时
void 
LogicSystem::dailyRefresh(const UInt64& accTime)
{	
	UInt32 nowSec = TimeUtil::getSecOfToday();
	UInt32 flag = 10000;

	if (nowSec >= 0 && nowSec < ( SAVE_INV_TIME/1000))//每天0时
	{
		DailyResetResp resp;
		flag = 0;

		resp.dailyresetHour = flag;
		resp.mRespJsonStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().broadcastMsg(resp);
	}
	
	if (nowSec >= 4*3600 && nowSec < (4*3600 + SAVE_INV_TIME/1000))//每天4时
	{
		DailyResetResp resp;
		flag = 4;
		StageCalcManager::getSingleton().resetAllResetStages();

		resp.dailyresetHour = flag;
		resp.mRespJsonStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().broadcastMsg(resp);
	}
	if (nowSec >= 5*3600 && nowSec < (5*3600 + SAVE_INV_TIME/1000))//每天5时
	{
		DailyResetResp resp;
		flag = 5;

		resp.dailyresetHour = flag;
		resp.mRespJsonStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().broadcastMsg(resp);
	}
	if (flag == 10000)
	{
		return;
	}
	Player *player;
	for(Map<Guid, Player*>::Iter* iter = mPlayerGuidMap.begin();iter != NULL; iter = mPlayerGuidMap.next(iter))
	{
		if (iter->mValue == NULL )
		{
			break;
		}
		player = iter->mValue;


// 		resp.dailyresetHour = flag;
// 		std::string str = resp.convertDataToJson();		
// 		NetworkSystem::getSingleton().sender(player->getConnId(),BOC_DAILYRESET_RESP,str);

		if (flag == 4)
		{
			LogicSystem::getSingleton().updateDailyResetData(player,flag);

			player->getStageManager().stageReset(player,true);

			player->getStageManager().resetMatchingPlayers();

			player->getStageManager().resetChapterUnlock();
		}
		else if(flag == 5)
		{
			LogicSystem::getSingleton().updateDailyResetData(player,flag);
		}
		else if(flag == 0)
		{
			LogicSystem::getSingleton().updateDailyResetData(player,flag);
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
	mPlayerConnectionMap.erase(connId);
	player->onClientConnectClosed();
	AccountWasLoginNotify notify;
	NetworkSystem::getSingleton().sendMsg(notify, connId);
	pushEvent(EVENT_CLIENT_CONNECTION_CLOSE, connId);

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
	}
}

bool LogicSystem::checkReset(UInt64 curTime, Player * player)
{
		 PlayerDailyTaskData & dailytaskData = player->getDailyTaskData();
	

	bool isToday = TimeManager::timeIsOneDay(curTime, dailytaskData.lastResetTime);
	//不是今天，那么需要重置数据
	if(!isToday)
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
	updateBaseInfo(player->getPlayerBaseData());
	
	//日常任务和活跃点刷新
	UInt64 curTime = time(0);
	checkReset(curTime, player);

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


// 	GCStageListResp stageListResp;
// 	stageListResp.mPacketID = BOC_CHAPTER_LIST_RESP;
// 	stageListResp.mRespJsonStr = player->getStageManager().convertStageDataToJson();
// 	NetworkSystem::getSingleton().sendMsg(stageListResp,connId);

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
	CourageChallengeManager::getSingleton().onCourageChallengeReq(connId,courageChallengeReq);

	InlineActivityManager::getSingleton().checkLogin(player->getPlayerGuid());//顺序不能错，在线奖励用到

	OnlineWelFareReq  req42;
	req42.reqType = 0;		
	InlineActivityManager::getSingleton().onOnlineWelFareReq( connId,req42);

	SevenLoginAwardReq  req43;
	req43.reqType = 2;
	InlineActivityManager::getSingleton().onSevenLoginAwardReq(connId,req43);

	SevenDayTaskReq req46;
	InlineActivityManager::onSevenDayTaskReq(connId,req46);

	GCAchieveDailyListResp achieveDailyListResp;
	achieveDailyListResp.mPacketID = BOC_ACHIEVEDAILYLIST_RESP;
	achieveDailyListResp.mRespJsonStr = player->getAchieveManager().convertAchieveDataToJson();
	//cout << achieveDailyListResp.mRespJsonStr;
	NetworkSystem::getSingleton().sendMsg(achieveDailyListResp, connId);

	




}



void LogicSystem::loadPlayerData(const ConnId& connId,const Guid& playerGuid)
{
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
		
	
		updateBaseInfo(playerData.mBaseData);
		
		
		player->checkLvActiveConditon();
		
		
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

// 		GCStageListResp stageListResp;
// 		stageListResp.mPacketID = BOC_CHAPTER_LIST_RESP;
// 		stageListResp.mRespJsonStr = player->getStageManager().convertStageDataToJson();
// 		NetworkSystem::getSingleton().sendMsg(stageListResp,connId);

		StageManager::getSingleton().sendStageData( player->getPlayerGuid(), player->getStageManager().getMapIDToChapter(),true);

		GCPlayerFashionListResp fashionListResp;
		fashionListResp.mPacketID = BOC_PLAYER_FASHION_LIST_RESP;
		fashionListResp.mRespJsonStr = player->getFashionManager().convertFashionDataToJson();
		NetworkSystem::getSingleton().sendMsg(fashionListResp,connId);
		
		GCPlayerEquipStateResp equipStateResp;
		equipStateResp.mPacketID = BOC_PLAYER_EQUIP_STATE_RESP;
		equipStateResp.mRespJsonStr = player->getEquipManager().convertEquipStateToJson();
		NetworkSystem::getSingleton().sendMsg(equipStateResp,connId);
 
		//客户端没有写包处理逻辑，目前先注释，以后打开
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

		GCMaterialListResp materialListResp;
		materialListResp.mPacketID = BOC_MATERIAL_LIST_RESP;
		materialListResp.mRespJsonStr = player->getMaterialManager().convertDataToJson();
		NetworkSystem::getSingleton().sendMsg(materialListResp, connId);
		//cout << materialListResp.mRespJsonStr;
		
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
		//cout << friendblackListResp.mRespJsonStr;
		NetworkSystem::getSingleton().sendMsg(friendblackListResp, connId);

		EmailListResp emailListResp;
		emailListResp.mPacketID = EMAIL_LIST_RESP;
		emailListResp.mRespJsonStr = player->getEmailManager().convertDataToJson();
		NetworkSystem::getSingleton().sendMsg(emailListResp, connId);

		GCCharactorListResp charactorList;
		charactorList.mPacketID = BOC_CHARACTORLIST_RESP;
		charactorList.mRespJsonStr = player->getCharactorManager().convertCharactorDataToJson();
		NetworkSystem::getSingleton().sendMsg(charactorList, connId);


		RewardLotteryMsgReq rewardLotteryMsgReq;
		GiftManager::getSingleton().onRewardLotteryMsgReq(connId,rewardLotteryMsgReq);

		CourageChallengeReq courageChallengeReq;
		courageChallengeReq.reqType =0;
		CourageChallengeManager::getSingleton().onCourageChallengeReq(connId,courageChallengeReq);

		InlineActivityManager::getSingleton().checkLogin(playerData.mBaseData.m_nPlayerID);//顺序不能错，在线奖励用到

		SevenLoginAwardReq  req43;
		req43.reqType = 2;
		InlineActivityManager::getSingleton().onSevenLoginAwardReq(connId,req43);

		SevenDayTaskReq req46;
		InlineActivityManager::onSevenDayTaskReq(connId,req46);

		GCAchieveDailyListResp achieveDailyListResp;
		achieveDailyListResp.mPacketID = BOC_ACHIEVEDAILYLIST_RESP;
		achieveDailyListResp.mRespJsonStr = player->getAchieveManager().convertAchieveDataToJson();
		//cout << achieveDailyListResp.mRespJsonStr;
		NetworkSystem::getSingleton().sendMsg(achieveDailyListResp, connId);
		OnlineWelFareReq  req42;
		req42.reqType = 0;		
		InlineActivityManager::getSingleton().onOnlineWelFareReq( connId,req42);

	


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
        return;
    }

    if (player->getLoginToken() != loginToken)
    {
        ReconnectFailResp resp;
        resp.mErrno = LynxErrno::InvalidPermission;
        NetworkSystem::getSingleton().sendMsg(resp, connId);
        pushEvent(EVENT_CLIENT_CONNECTION_CLOSE, connId);
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
        NetworkSystem::getSingleton().sendMsg(notify, oldConnId);
        pushEvent(EVENT_CLIENT_CONNECTION_CLOSE, oldConnId);
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
void LogicSystem::updateDailyResetData(Player *player,UInt32 flag)
{

	PlayerDailyResetData dailyResetData = player->getPlayerDailyResetData();


	if (flag == 4)	
	{
		dailyResetData.m_nDailyMultipleCopyCount = 0;		
		if (dailyResetData.m_nTwelvePalaceUnlockCount > GlobalVarManager::getSingleton().getTwelvePalace().resetlowerlimit)
		{
			dailyResetData.m_nTwelvePalaceUnlockCount = dailyResetData.m_nTwelvePalaceUnlockCount;

		}
		else
		{
			dailyResetData.m_nTwelvePalaceUnlockCount = GlobalVarManager::getSingleton().getTwelvePalace().resetlowerlimit;			
		}

		dailyResetData.m_nTwelvePalaceResetFlag = 0;//登录的时候是0，在线更新是0

		dailyResetData.m_nLastUpdateTime = TimeUtil::getTimeSec();

// 		dailyResetData.m_nStrengthBuyTimes = 0;
	}
	else if (flag == 5)
	{
		dailyResetData.spare5 = 0;
		dailyResetData.m_nCoinBuyTimes = 0;
		dailyResetData.m_nCoinFreeBuyTimes = 0;
		dailyResetData.notUsed3 = 0;
		dailyResetData.notUsed4 = 0;
		dailyResetData.notUsed5 = 0;
		dailyResetData.notUsed6 = 0;
		dailyResetData.notUsed7 = 0;
	}
	else if (flag == 0)
	{


		//如果不是同一天，说明数据需要刷新，那么更新数据
		dailyResetData.m_nArenaEnterCount = 0;
		dailyResetData.m_nAmphitheaterEnterCount = 0;
		dailyResetData.m_nAmphitheaterWinCount = 0;
		dailyResetData.m_nAmphitheaterLoseCount = 0;
		dailyResetData.m_nDailyChaiCount = 0;
		dailyResetData.m_nDailyMiCount = 0;
		dailyResetData.m_nDailyYouCount = 0;
		dailyResetData.m_nDailyYanCount = 0;
		dailyResetData.m_nTwelvepalaceEnterCount = 0;

		dailyResetData.m_strTwelvepalaceData = "";


		dailyResetData.m_nCouragetrialDailyPoint = 10;

		dailyResetData.m_nActivepoint = 0;
		dailyResetData.m_nLastUpdateTime = TimeUtil::getTimeSec();
		//m_nDailyMultipleCopyCount
		//m_nTwelvePalaceUnlockCount		


		
// 		dailyResetData.m_nTwelvePalaceBuyTimes = 0;
		//先加载周更新的数据，之后根据判断进行
		
			
		dailyResetData.m_strCourageData = dailyResetData.m_strCourageData;
		dailyResetData.m_nTowerBuyTimes = 0;
		if (dailyResetData.m_nTowerBuyTimes > GlobalVarManager::getSingleton().getclimbtower().resetlowerlimit)
		{
			dailyResetData.m_nClimbTowerTimes = dailyResetData.m_nTowerBuyTimes;

		}
		else
		{
			dailyResetData.m_nClimbTowerTimes = GlobalVarManager::getSingleton().getclimbtower().resetlowerlimit;			
		}

	}

	player->setPlayerDailyResetData(dailyResetData);
	// 		player->getPersistManager().setDirtyBit(DAILYRESETBIT);//不存盘 等要修改再存盘

	GCPlayerDetailResp detailInfoResp;
	detailInfoResp.mPacketID = BOC_PLAYER_DETAIL_RESP;
	detailInfoResp.mRespJsonStr = player->getPlayerInfoManager().convertDetailInfoToJson();

	if(player->getConnId() == 0)
	{
		return;
	}
	NetworkSystem::getSingleton().sendMsg(detailInfoResp, player->getConnId());

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


Guid LogicSystem::listRandOne(List<Guid> playerIDList)
{
	if (playerIDList.size() == 0)
	{
		return 0;
	}
	UInt32 count = rand()% playerIDList.size();

	UInt32 i = 0;
	for (List<Guid>::Iter * iter = playerIDList.begin();iter != NULL;iter = playerIDList.next(iter))
	{		
		if (i == count)
		{
			return iter->mValue;
		}
		i++;		
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

	if (otherPlayerID == 0 && times != 2)//战力上下3%找不着就5%
	{
		PlayerBaseData baseData = player->getPlayerBaseData();
		PersistFindSimilarPowerReq req;
		req.playerGuid = player->getPlayerGuid();
		req.low = baseData.m_nPower *(100 - 5) /100;
		req.high = baseData.m_nPower * (100 + 5) /100;
		req.times = 2;

		PersistSystem::getSingleton().postThreadMsg(req, 0);
		LOG_WARN(" not found SimililarPower player");	
		
		return;
			
	}

	if (otherPlayerID != 0)
	{
		player->getStageManager().addMatchingPlayers(otherPlayerID);
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
	if( otherPlayerID >1000 && otherPlayerID <10000)
	{
		resp.isRobot = 1;
	}
	resp.otherPlayer = str;
	resp.id = fireConfirmData.m_CopyID;
	resp.result = LynxErrno::None;
	std::string jsonStr = resp.convertDataToJson();	
	NetworkSystem::getSingleton().sender( connId,TWELVE_PALACE_BEGIN_RESP,jsonStr);
}



void 
LogicSystem::kickOutAllPlayer()
{

	ClimbTowerManager::getSingleton().updateClimTower(1);
	// 踢人，存档
	Vector<Guid> destroyPlayerList;
	for (Map<Guid, Player*>::Iter* iter = mPlayerGuidMap.begin();
		iter != NULL; iter = mPlayerGuidMap.next(iter))
	{
		Player* player = iter->mValue;
		if (player)
		{
			
			LogicSystem::getSingleton().closeConnection(player);

			destroyPlayerList.push_back(player->getPlayerGuid());

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

		PersistOffLineSaveReq saveReq;
		saveReq.mPlayerUid = *iterVec;

		PersistSystem::getSingleton().postThreadMsg(saveReq, saveReq.mPlayerUid);
		player->setLastPostState(1);
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


