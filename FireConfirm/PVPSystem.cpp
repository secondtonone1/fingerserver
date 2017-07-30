#include "PVPSystem.h"
#include "../LogicSystem.h"
#include "./PlayerData.h"

using namespace Lynx;

PVPSystem::PVPSystem()
{

}

PVPSystem::~PVPSystem()
{

}

bool 
PVPSystem::initial()
{


// 	REGISTER_THREAD_MSG(mThreadMsgHandler, PVPTreadReq1, PVPSystem::onPVPSystemPVP);
	#if defined (_WIN32)
	for (UInt32 i = 0; i < PVP_THREAD_WORKER_COUNT; ++i)
	{
		if (!mPVPWorkerArray[i].initial(i))
		{
			LOG_WARN("Failed to initial PVP thread worker [%u]", i);
			return false;
		}
	}

  #endif
// 		m_nLastSyncTime = 0;
	//LOG_INFO("Initial PVPSystem");

	SceneManager *sceneManager;

	Character * character;	

	for(UInt32 j =1;j<= PVP_SCENE_MAX_COUNT;j++)
	{
		sceneManager  = new SceneManager();
		if (sceneManager == NULL)
		{
			return false;
		}
		sceneManager->mSceneID = j;
 		sceneManager->m_SceneTempelete = new SceneTempelete();

		for(UInt32 jj =1;jj <= PVP_SCENE_CHARACTER_NUM;jj++)
		{
			character = new Character();
			if (character == NULL)
			{
				return false;
			}
			sceneManager->CharacterPool.push(character);
		}
		
		mScenePool.push(sceneManager);
	}	

	return true;
}




void 
PVPSystem::release()
{
	SceneManager *sceneManager;
	Character * character;
	for(Map<UInt32,SceneManager*>::Iter *iter = mSceneMap.begin();iter != NULL;iter = mSceneMap.next(iter))
	{
		removeScene(iter->mKey);
	}
	for(UInt32 j =1;j<= PVP_SCENE_MAX_COUNT;j++)
	{
		if (mScenePool.size() != 0)
		{
			sceneManager = mScenePool.top();
			mScenePool.pop();

			for(UInt32 jj = 1;jj <= PVP_SCENE_CHARACTER_NUM;jj++)
			{
				character = sceneManager->CharacterPool.top();
				sceneManager->CharacterPool.pop();
				assert(character);
				delete character;
				//添加置空
				character = NULL;
				
			}
			//断言测试空指针，王戊辰
			assert(sceneManager->m_SceneTempelete);
			//程庆银源代码
			delete sceneManager->m_SceneTempelete;
			//添加置空，王戊辰
			sceneManager->m_SceneTempelete = NULL;
			assert(sceneManager);
			delete sceneManager;
			sceneManager = NULL;
		}
		else
		{
			break;
		}		
	}	

	
	for (UInt32 i = 0; i < PVP_THREAD_WORKER_COUNT; ++i)
	{
		mPVPWorkerArray[i].release();
	}
	
	//LOG_INFO("Release PVPSystem");

	
// 	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PersistNotifyWorkerSaveDbResp);

}

void 
PVPSystem::update(const UInt64& accTime)
{
	
	for (UInt32 i = 0; i < PVP_THREAD_WORKER_COUNT; ++i)
	{
		StreamBuffer& threadMsgStream = mPVPWorkerArray[i].getOutputMsgQueue().getMsgs();
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
PVPSystem::dispatchThreadMsg(ThreadMsg& msg)
{
 	mThreadMsgHandler.onDispatchMsg(msg);
}


SceneManager* PVPSystem::initScene(UInt32 stageID)
{
	SceneManager *sceneManager =NULL;

	if (mScenePool.size() == 0)
	{
		return sceneManager;
	}
	sceneManager = mScenePool.top();
	mScenePool.pop();
	Pos sceneEdge;
	sceneEdge.x =SCENE_EDGEX;//SCENE_EDGEX
	sceneEdge.y =SCENE_EDGEY;//SCENE_EDGEY
	sceneManager->m_SceneTempelete->sceneEdge = sceneEdge;

	sceneManager->m_SceneTempelete->stageID = stageID;
	 
	mSceneMap.insert(sceneManager->mSceneID,sceneManager);

	return sceneManager;
}
void PVPSystem::removeScene(UInt32 sceneID)
{

	for(Map<UInt32,SceneManager*>::Iter *iter = mSceneMap.begin();iter != NULL;iter = mSceneMap.next(iter))
	{
		if (iter->mKey == sceneID)
		{
			
			for(List<Guid>::Iter *it = iter->mValue->m_SceneTempelete->playerPool.begin();it != NULL;it =iter->mValue->m_SceneTempelete->playerPool.next(it) )
			{
				iter->mValue->removeScenePlayer(it->mValue);

			}
			
			iter->mValue->m_SceneTempelete->playerPool.clear();
			iter->mValue->m_SceneTempelete->sceneCharacter.CharacterMap.clear();

			mScenePool.push(iter->mValue);
			mSceneMap.erase(iter);			
		}
	}
}

UInt32 PVPSystem::getRoomMaxNum()
{
	return PVP_SCENE_CHARACTER_NUM;
}

SceneManager* PVPSystem::getScene(UInt32 id)
{
	Map<UInt32,SceneManager*>::Iter* iter = mSceneMap.find(id);

	if (iter == mSceneMap.end())
	{
		return NULL;
	}
	return iter->mValue;
}

void PVPSystem::setScene(SceneManager* val)
{
	for(Map<UInt32,SceneManager*>::Iter* iter = mSceneMap.begin();iter != NULL ;iter = mSceneMap.next(iter))
	{
		if (iter->mValue->mSceneID == val->mSceneID)
		{
			iter->mValue = val;
		}
	}

	
}
void PVPSystem::getRoomList(const  ConnId& connId,PVPRoomReq& msg)
{

	PVPRoomResp resp;
	resp.typeID = msg.typeID;
	resp.result =1;	
	for(List<RoomData>::Iter *iter = mRoomData.begin();iter !=NULL;iter = mRoomData.next(iter))
	{
		resp.roomDatas.insertTail(iter->mValue);
	}	
	std::string jsonStr;
	jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender(connId,PVP_ROOM_RESP,jsonStr);

}

void PVPSystem::createRoom(const  ConnId& connId,PVPRoomReq& msg)
{
	UInt32 cost = 0;
	Player *player;
	RoomData roomData;
	RoomPlayer roomPlayer;
	PVPRoomResp resp;
	std::string jsonStr;

	player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	msg.convertJsonToData(msg.jsonStr);
	resp.typeID = msg.typeID;
	resp.result =LynxErrno::None;


	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();
	VipTemplate vipTemplate;
	for(List<VipTemplate>::Iter *iter = gVipTable->mVip.begin();iter != NULL;iter = gVipTable->mVip.next(iter))
	{
		if (iter->mValue.id == player->getVipLevel())
		{
			vipTemplate = iter->mValue;
			break;
		}
	}
	if (msg.value >= globalValue.uTRIALGainsRate)
	{
		if (globalValue.uTRIALVipNeed  > player->getVipLevel())
		{
			resp.result = LynxErrno::VipLevelNotEnough;
			jsonStr = resp.convertDataToJson();
			NetworkSystem::getSingleton().sender(connId,PVP_ROOM_RESP,jsonStr);
			return;
		}

	}
	
	roomData.roomID = ++mRoomID;
	roomData.stageID = msg.stageID;
	roomData.sceneID = 0;
	roomData.multiple = msg.value;
	roomData.roomState = 0;

	roomPlayer.playerID = player->getPlayerGuid();
	roomPlayer.name = player->getPlayerName();
	roomPlayer.level = player->getPlayerLeval();
	roomPlayer.vipLevel = player->getVipLevel();
	roomPlayer.title = player->getPlayerTitle();//todo 加数据库
	roomPlayer.camp =1;
	roomPlayer.state = 0;
	roomPlayer.level = player->getPlayerLeval();
	roomPlayer.modID = player->getPlayerModelID();
	roomPlayer.combatCapability = player->getPlayerPower();//todo 计算攻击力 有个表
	roomPlayer.delay = 100;//todo 计算网络延迟
	roomPlayer.score = 0;//todo 有没有积分

	


	roomData.playerList.insertTail(roomPlayer);
	mRoomData.insertTail(roomData);
	player->setRoomID(mRoomID);	

	resp.roomDatas.insertTail(roomData);
	resp.gold = player->getPlayerGold();
	
	jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender(connId,PVP_ROOM_RESP,jsonStr);
}


void PVPSystem::joinRoom(const  ConnId& connId,PVPRoomReq& msg)
{
	Player *player;
	RoomPlayer roomPlayer;

	player = LogicSystem::getSingleton().getPlayerByConnId(connId);

	msg.convertJsonToData(msg.jsonStr);
	PVPRoomResp resp;
	resp.typeID = msg.typeID;
	resp.result = LynxErrno::None;
	

	for(List<RoomData>::Iter *iter = mRoomData.begin();iter != NULL;iter = mRoomData.next(iter))
	{
		if (iter->mValue.roomID == msg.value)
		{
			if (iter->mValue.playerList.size()>2)
			{			
				resp.result = LynxErrno::TimesNotEnough;
				break;
			}
			roomPlayer.playerID = player->getPlayerGuid();
			roomPlayer.name = player->getPlayerName();
			roomPlayer.level = player->getPlayerLeval();
			roomPlayer.vipLevel = player->getVipLevel();
			roomPlayer.title = player->getPlayerTitle();//todo 加数据库
			roomPlayer.camp =1;
			roomPlayer.state = 0;
			roomPlayer.level = player->getPlayerLeval();
			roomPlayer.modID = player->getPlayerModelID();
			roomPlayer.combatCapability = player->getPlayerPower();//todo 计算攻击力 有个表
			roomPlayer.delay = 100;//todo 计算网络延迟
			roomPlayer.score = 0;//todo 有没有积分
			Goods good;
			good.resourcestype =2 ;
			good.subtype = 10001;
			good.num = 1;
			roomPlayer.equipList.insertTail(good);//todo 装备 时装有没有完成
			good.resourcestype =2 ;
			good.subtype = 10002;
			good.num = 1;
			roomPlayer.equipList.insertTail(good);//todo 装备 时装有没有完成

			iter->mValue.playerList.insertTail(roomPlayer);
			resp.roomDatas.insertTail(iter->mValue);
			break;
		}
	}

	if (resp.roomDatas.size()==0)
	{
		resp.result = 2;
	}
	resp.result = LynxErrno::None;
	resp.gold = player->getPlayerGold();
	std::string jsonStr;
	jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender(connId,PVP_ROOM_RESP,jsonStr);

}
Robot getRobot(Guid playerID)
{
	char dest[64] = {};

	Robot robot;
	robot.playerID = playerID;
	
 	sprintf(dest,"player%llu",playerID);
	robot.name = dest;

	robot.level = rand()%100;
	robot.vipLevel = 8;
	robot.title = "";//todo 加数据库
	robot.camp =1;
	robot.state = 0;
	robot.modID = 2;
	robot.power = rand()%100 *100000;//todo 计算攻击力 有个表
	robot.delay = 100;//todo 计算网络延迟
	robot.score = 0;//todo 有没有积分
	Goods good;
	good.resourcestype =14 ;
	good.subtype = 1003;
	good.num = 1;
	robot.equipList.insertTail(good);//todo 装备 时装有没有完成
	good.resourcestype =14 ;
	good.subtype = 2001;
	good.num = 1;
	robot.equipList.insertTail(good);//todo 装备 时装有没有完成
	return robot;
}

//typeID 1优先级好友，正负2再到正负5；2 高于等级2的
Robot PVPSystem::getSimilarPlayer(Guid playerID,UInt32 typeID)//todo 找战力相近玩家 
{
	Robot robot;
	UInt32 isUsed =0;
	Player *player;
	player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	PlayerDailyResetData playerDailyResetData =player->getPlayerDailyResetData();
	PlayerFireConfirmData mFireConfirmData = player->GetFireConfirmData();


	if (mFriendData.size() ==0)
	{
		for(UInt32 i =0;i<10;i++)
		{
			mFriendData.insertTail(getRobot(1060+i));

		}
	}
	if (typeID == 1)//好友
	{
	}
	


	for(List<Robot>::Iter *iter = mFriendData.begin();iter != NULL;iter = mFriendData.next(iter) )
	{
		isUsed =0;
		for(List<Guid>::Iter * it =  playerDailyResetData.m_MatchingList.begin(); it != NULL;it = playerDailyResetData.m_MatchingList.next(it))
		{
			if (iter->mValue.playerID == it->mValue )
			{
				isUsed =1;
				break;
			}
			
		}
		if(iter->mValue.playerID == playerID) 
		{
			isUsed =1;
		}
		if(typeID ==2)//是正的而不是正负
		{
			if (player->getPlayerLeval() == 80)
			{
			}
			else
			{
				if (iter->mValue.level < player->getPlayerLeval())
				{
					isUsed = 1;
				}
			}
			
		}
		if (isUsed == 0)
		{
			return iter->mValue;
		}
		
	}
	return robot;

}



void PVPSystem::matchingRoom(const  ConnId& connId,PVPRoomReq& msg)//todo
{
	Player *player;
// 	Player *otherPlayer;
	RoomPlayer roomPlayer;

	PVPRoomResp resp;
	std::string jsonStr;
	UInt32 isUsed =0;

	msg.convertJsonToData(msg.jsonStr);
	resp.typeID = msg.typeID;
	resp.result = LynxErrno::None;

	player = LogicSystem::getSingleton().getPlayerByConnId(connId);

	PlayerBaseData baseData = player->getPlayerBaseData();
	PersistFindSimilarPowerReq req;
	req.playerGuid = player->getPlayerGuid();
	int lowLevel =baseData.m_nLevel -5;
	if (lowLevel < 0)
	{
		lowLevel = 0;
	}
	req.low = lowLevel;
	req.high = baseData.m_nLevel +5;
	req.times = 10001;//10001 多人副本标识
	req.initialValue = baseData.m_nLevel;

	PersistSystem::getSingleton().postThreadMsg(req, 0);
}


void PVPSystem::matchingRoomResp(Guid playerID,Guid otherPlayerID)
{
	String playerUidStr;
	char dest[64] = {};
	Player *player;
	RoomPlayer roomPlayer;

	PVPRoomResp resp;
	std::string jsonStr;
	UInt32 isUsed =0;

	resp.typeID = 7;
	resp.result = LynxErrno::None;

	player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	if(player == NULL)
	{
		return;
	}

	BaseInfoData baseInfoData;
	LogicSystem::getSingleton().getBaseInfo(otherPlayerID, baseInfoData);

	
	LogicSystem::getSingleton().getBaseInfo(otherPlayerID, baseInfoData);
	Goods good;
	List<Goods>equipList;
// 	good.resourcestype =14 ;
// 	good.subtype = 1003;
// 	good.num = 1;
// 	equipList.insertTail(good);//todo 装备 时装有没有完成
// 	good.resourcestype =14 ;
// 	good.subtype = 2001;
// 	good.num = 1;
// 	equipList.insertTail(good);//todo 装备 时装有没有完成

	for(List<RoomData>::Iter *iter = mRoomData.begin();iter != NULL;iter = mRoomData.next(iter))
	{
		if (iter->mValue.roomID == player->getRoomID())
		{
			if (iter->mValue.playerList.size()>2)
			{			
				resp.result = LynxErrno::SeatIsFull;
				break;
			}

			roomPlayer.playerID = otherPlayerID;
			roomPlayer.name = baseInfoData.name.c_str();
			roomPlayer.level = baseInfoData.level;
			roomPlayer.vipLevel = baseInfoData.vipLv;
			roomPlayer.title = "";//todo 加数据库
			roomPlayer.camp = 2;
			roomPlayer.state = 0;
			roomPlayer.modID = baseInfoData.modelId;
			roomPlayer.combatCapability = baseInfoData.power;//todo 计算攻击力 有个表
			roomPlayer.delay = 100;//todo 计算网络延迟
			roomPlayer.score = 0;//todo 有没有积分

 			roomPlayer.equipList= equipList;//todo 装备 时装有没有完成
			iter->mValue.playerList.insertTail(roomPlayer);
			resp.roomDatas.insertTail(iter->mValue);
			break;
		}
	}

	if (otherPlayerID != 0)
	{
		sprintf(dest,"%llu",otherPlayerID);
		playerUidStr = (String)dest;	
		resp.otherPlayerID = otherPlayerID;
		if (otherPlayerID > RobotMinRoleID && otherPlayerID <RobotMaxRoleID)
		{
			resp.isRobot = 1;
		}		
		resp.otherPlayer =  LogicSystem::getSingleton().getDetailInfo(playerUidStr.c_str());
		

	}

	if (resp.roomDatas.size()== 0)
	{
		resp.result = LynxErrno::NotFound;
	}
	PlayerFireConfirmData mFireConfirmData =  player->GetFireConfirmData();
	resp.value = mFireConfirmData.m_AwardTimes;
	if (resp.otherPlayer == "")
	{
		resp.result = LynxErrno::RobotNotFound;
	}
	resp.gold = player->getPlayerGold();

	jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender(player->getConnId(),PVP_ROOM_RESP,jsonStr);

}



void PVPSystem::leaveRoom(const  ConnId& connId,PVPRoomReq& msg)
{
	Player *player;

	player = LogicSystem::getSingleton().getPlayerByConnId(connId);

	msg.convertJsonToData(msg.jsonStr);
	PVPRoomResp resp;
	resp.typeID = msg.typeID;
	resp.result = LynxErrno::None;

	for(List<RoomData>::Iter *iter = mRoomData.begin();iter != NULL;iter = mRoomData.next(iter))
	{
		if (iter->mValue.roomID == msg.value)
		{
			for(List<RoomPlayer>::Iter *it = iter->mValue.playerList.begin();it !=NULL;it = iter->mValue.playerList.next(it))
			{
				if (it->mValue.playerID == player->getPlayerGuid())
				{
					iter->mValue.playerList.erase(it);
					break;
				}	

			}
			if (iter->mValue.playerList.size() >0)
			{
				for(List<RoomPlayer>::Iter *it = iter->mValue.playerList.begin();it !=NULL;it = iter->mValue.playerList.next(it))
				{
					resp.roomDatas.insertTail(iter->mValue);
					player = LogicSystem::getSingleton().getPlayerByConnId(connId);
					if (player == NULL)
					{
						continue;
					}
					std::string jsonStr;
					jsonStr = resp.convertDataToJson();
					NetworkSystem::getSingleton().sender(player->getConnId(),PVP_ROOM_RESP,jsonStr);	

				}
			}
			if (iter->mValue.playerList.size() == 0)
			{
				mRoomData.erase(iter);
				resp.result = LynxErrno::BeDissolved;
			}			
		}
	}

	std::string jsonStr;
	resp.result = LynxErrno::None;
	resp.gold = player->getPlayerGold();
	jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender(connId,PVP_ROOM_RESP,jsonStr);
}

void PVPSystem::PVPStart(const  ConnId& connId,PVPRoomReq& msg)
{
	UInt32 cost = 0;
	Guid otherPlayerID = 0;
	UInt32 flag = LynxErrno::None;
	UInt32 trailTimes =0;
	Goods goods;
	List<Goods> itemList;
	std::string jsonStr;
	Player *player;
	SceneManager *sceneManager;
	PVPRoomResp resp;

	//LOG_INFO("PVPStart  = %s ",msg.jsonStr.c_str());

	player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}

	msg.convertJsonToData(msg.jsonStr);
	resp.typeID =msg.typeID;
	resp.value = msg.value;

 	player->ResetFireConfirmData();
	
	flag = StageManager::getSingleton().canChallengeStage( connId, msg.stageID);
	if (flag != LynxErrno::None)
	{
		resp.result =flag;
		std::string jsonStr = resp.convertDataToJson();	
		NetworkSystem::getSingleton().sender( connId,PVP_ROOM_RESP,jsonStr);
		return;
	}

	if (gStageTable->get(msg.stageID) == NULL)
	{
		LOG_WARN("gStageTable->get(msg.stageID) not found!!");
		return;
	}
	if (gStageTable->get(msg.stageID)->mType == STAGE_TRIAL)//多人副本
	{

		GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();
		//正常是10000不扣钱，2倍是20000 扣钱
		if (globalValue.uTRIALGainsRate == msg.value)
		{
			cost = globalValue.uTRIALratecosts;
		}
		

		if (player->getPlayerGold()<cost)
		{
			resp.result = LynxErrno::RmbNotEnough;

		}
		PlayerDailyResetData playerDailyResetData =player->getPlayerDailyResetData();
		
		VipTemplate vipTemplate;
		for(List<VipTemplate>::Iter *iter1 = gVipTable->mVip.begin();iter1 != NULL;iter1 = gVipTable->mVip.next(iter1))
		{
			if (iter1->mValue.id == player->getVipLevel())
			{
				vipTemplate = iter1->mValue;
				break;
			}
		}
		if (globalValue.uTRIALVipNeed > player->getVipLevel() && globalValue.uTRIALGainsRate == msg.value)
		{
			resp.result = LynxErrno::VipLevelNotEnough;
			jsonStr = resp.convertDataToJson();
			NetworkSystem::getSingleton().sender(connId,PVP_ROOM_RESP,jsonStr);
			return;
		}

		trailTimes = vipTemplate.trailtimes;
		if ( trailTimes <= playerDailyResetData.m_nDailyMultipleCopyCount)
		{
			resp.result = LynxErrno::TimesNotEnough;
		}
		if (resp.result != LynxErrno::None)//失败
		{
			for(List<RoomData>::Iter *iter = mRoomData.begin();iter != NULL;iter = mRoomData.next(iter))
			{
				if (iter->mValue.roomID == player->getRoomID())
				{
					mRoomData.erase(iter);					
					player->setRoomID(0);
				}
			}
			jsonStr = resp.convertDataToJson();
			NetworkSystem::getSingleton().sender(connId,PVP_ROOM_RESP,jsonStr);
			return;
		}
		
		for(List<RoomData>::Iter *iter = mRoomData.begin();iter != NULL;iter = mRoomData.next(iter))
		{
			
			if (iter->mValue.roomID == player->getRoomID())
			{
				iter->mValue.roomState =1;
				for (List<RoomPlayer>::Iter * it = iter->mValue.playerList.begin();it!= NULL;it =iter->mValue.playerList.next(it))
				{
					if (it->mValue.playerID != player->getPlayerGuid())
					{
						otherPlayerID = it->mValue.playerID;
						break;						
					}					
				}
				break;
			}
		}
		playerDailyResetData.m_nDailyMultipleCopyCount++;
		playerDailyResetData.m_MatchingList.insertTail(otherPlayerID);
		player->setPlayerDailyResetData(playerDailyResetData);
		player->getPersistManager().setDirtyBit(DAILYRESETBIT);
		goods.resourcestype =1;
		goods.subtype =2;
		goods.num =0 - cost;
		itemList.insertTail(goods);
		GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),1,itemList,MiniLog8);

		GCPlayerDetailResp detailInfoResp;
		detailInfoResp.mPacketID = BOC_PLAYER_DETAIL_RESP;
		detailInfoResp.mRespJsonStr = player->getPlayerInfoManager().convertDetailInfoToJson();
		NetworkSystem::getSingleton().sendMsg(detailInfoResp, connId);

		PlayerFireConfirmData mFireConfirmData =  player->GetFireConfirmData();

		//LOG_INFO("m_AwardTimes  = %u ",msg.value);

		mFireConfirmData.m_AwardTimes =  msg.value;
		player->SetFireConfirmData(mFireConfirmData);
		//更新多人副本每日任务
		player->getAchieveManager().updateDailyTaskData(DLYMULTYSUCCESS, 1 );
	}

	FireConfirmManager::getSingleton().SetCopyStart(player,msg.stageID);

// 	resp.confirmIDs =  FireConfirmManager::getSingleton().getFireConfirmCondition(connId);

	resp.result = LynxErrno::None;
	resp.gold = player->getPlayerGold();
	
	jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender(connId,PVP_ROOM_RESP,jsonStr);

	//LOG_INFO("PVP_ROOM_RESP = %s",jsonStr.c_str());
	

	for(List<RoomData>::Iter *iter = mRoomData.begin();iter != NULL;iter = mRoomData.next(iter))
	{
		if (iter->mValue.roomID == msg.stageID)//todo
		{
			if (iter->mValue.playerList.mIterCount >= 2)
			{			
				sceneManager = PVPSystem::getSingleton().initScene(iter->mValue.stageID);
			}
			else
			{
				return;
			}
			for(List<RoomPlayer>::Iter *it = iter->mValue.playerList.begin();it !=NULL;it = iter->mValue.playerList.next(it))
			{				
				sceneManager->addPlayer(it->mValue.playerID,sceneManager->mSceneID);
			}
			sceneManager->mRoomID = msg.value;
			iter->mValue.roomState = 1;
			
			break;
		}
	}

// 	sceneManager->update();

}

//1.获取房间列表2.创建房间3.加入房间4.开始PVP 5.离开房间6.改变房间状态7.排队
void PVPSystem::onPVPRoomReq(const ConnId& connId,PVPRoomReq& msg)
{
	msg.convertJsonToData(msg.jsonStr);
	if (msg.typeID == 1)
	{
		PVPSystem::getSingleton().getRoomList(connId, msg);
	}
	else if (msg.typeID == 2)
	{
		PVPSystem::getSingleton().createRoom( connId, msg);
	}
	else if (msg.typeID == 3)
	{
		PVPSystem::getSingleton().joinRoom( connId, msg);
	}	
	else if (msg.typeID == 4)
	{
		PVPSystem::getSingleton().PVPStart( connId, msg);
	}
	else if (msg.typeID == 5)
	{
		PVPSystem::getSingleton().leaveRoom( connId, msg);
	}
	else if (msg.typeID == 6)
	{
		//PVPSystem::getSingleton().leaveRoom( connId, msg);
	}
	else if (msg.typeID == 7)
	{
		PVPSystem::getSingleton().matchingRoom( connId, msg);
	}

}
//1.左移动2.右移动3.技能4.离开关卡5.切换技能列表6.复活
void PVPSystem::onPVPBattleReq(const ConnId& connId,PVPBattleReq& msg)
{

	msg.convertJsonToData(msg.jsonStr);
	PVPTreadReq req;
	req.typeID = msg.typeID;
	req.value = msg.value;
	req.connId = connId;
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		return;
	}
	UInt32 sceneID = player->getSceneID();
	if (sceneID == 0)
	{
		return;
	}
	UInt32 chanel = sceneID %20;
	PVPSystem::getSingleton().postThreadMsg(req,chanel);

}

void PVPSystem::PlayerAction(PVPTreadReq& msg)
{
	return;//todo
	
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(msg.connId);

	UInt32 sceneID = player->getSceneID();
	if (sceneID == 0)
	{
		return;
	}
	SceneManager* sceneManager;
	sceneManager = PVPSystem::getSingleton().getScene(sceneID);

	// 	NameVal nameVal;
	// 	nameVal.name = msg.typeID;
	// 	nameVal.value = msg.value;
	if (msg.typeID <10)
	{
		// 		nameVal.name =	ButtonClick_direction;
		// 		nameVal.value = JOYSTICKDIR_right;
		sceneManager->onEvent(msg);
	}	
}


void PVPSystem::removeRoom(UInt32 roomID)
{
	for(List<RoomData>::Iter *iter = mRoomData.begin();iter != NULL;iter = mRoomData.next(iter))
	{
		if (iter->mValue.roomID == roomID)
		{
			mRoomData.erase(iter);			
		}
	}
}