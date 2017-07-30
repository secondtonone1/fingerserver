#include "../LogicSystem.h"
#include "StringConverter.h"
#include "../PlatformLib/SerializeType.h"
#include "../PersistSystem.h"
#include "../PlatformLib/Utilex.h"
#include "../CommonLib/CopyFinishTime.h"
#include "Gift.h"
#include "ClimbTower.h"
#include "GlobalValue.h"
#include "../LoggerSystem.h"

using namespace Lynx;



//reqtype 2购买次数
void ClimbTowerManager::onTowerReq(const  ConnId& connId,TowerReq & req)
{
	TowerResp resp;
	UInt32 gold = 0;
	UInt32 chapterID = 1;//第一个章节id
	UInt32 firstStageID  = 1001;//第一个关卡id

	resp.result = LynxErrno::None;

	PlayerTowerData towerData;
	req.convertJsonToData(req.strReceive);
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}


	towerData = player->getPlayerTowerData();
	PlayerDailyResetData dailyResetData= player->getPlayerDailyResetData();

	if (towerData.m_HighID == 0)
	{
		TowerTypeTemplate* towerTypeTemplate = gTowerTypeTable->get(chapterID);
		if (towerTypeTemplate == NULL)
		{
			resp.result = LynxErrno::ClienServerDataNotMatch;
			std::string sendStr = resp.convertDataToJson();
			NetworkSystem::getSingleton().sender( connId,TOWER_INFO_RESP,sendStr);
			return;
		}
		towerData.m_HighID = firstStageID - 1;
		// 		towerData.m_Score = towerTypeTemplate->startscore;//重复加了
		towerData.m_AP = towerTypeTemplate->attackpower;
		towerData.m_HP = towerTypeTemplate->hp;
		player->setPlayerTowerData(towerData);
		player->getPersistManager().setDirtyBit(TOWERDATABIT);
	}

	if (req.reqtype == 2)
	{
		if (gVipTable->get(player->getVipLevel()) == NULL)
		{
			LOG_WARN("gVipTable->get(player->getVipLevel()) not found!!");
			return;
		}
		GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();
		gold = player->getPlayerGold();
		if (gold < globalValue.uCLIMBTbuycost)
		{
			resp.result = LynxErrno::RmbNotEnough;
		}
		else if ((gVipTable->get(player->getVipLevel())->climtowertimes - dailyResetData.m_nTowerBuyTimes) < 1)
		{
			resp.result = LynxErrno::VipBuyTimesNotEnough;
		}
		else
		{
			Goods goods;
			List<Goods> itemList;

			goods.resourcestype =AWARD_BASE;
			goods.subtype = AWARD_BASE_GOLD;
			goods.num = 0 - globalValue.uCLIMBTbuycost;
			itemList.insertTail(goods);
			GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog105);

		

			dailyResetData.m_nClimbTowerTimes ++;
			dailyResetData.m_nTowerBuyTimes ++;
			player->setPlayerDailyResetData(dailyResetData);
			player->getPersistManager().setDirtyBit(DAILYRESETBIT);			

		}
	}
// 	player->setPlayerTowerData(towerData);
// 	player->getPersistManager().setDirtyBit(TOWERDATABIT);

	resp.reqtype = req.reqtype;	
	resp.highID = towerData.m_HighID;
	resp.score = towerData.m_Score;
	resp.ap = towerData.m_AP;
	resp.hp = towerData.m_HP;
	resp.times = dailyResetData.m_nClimbTowerTimes;
	if (gVipTable->get(player->getVipLevel()) == NULL)
	{
		LOG_WARN("gVipTable->get(player->getVipLevel()) not found!!");
		resp.result = LynxErrno::ClienServerDataNotMatch;
		std::string sendStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,TOWER_INFO_RESP,sendStr);
		return;
	}
	resp.buyTimes = gVipTable->get(player->getVipLevel())->climtowertimes - dailyResetData.m_nTowerBuyTimes;

	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,TOWER_INFO_RESP,jsonStr);
	//LOG_INFO("TOWER_INFO_RESP= %s",jsonStr.c_str());



}

void ClimbTowerManager::onTowerBeginReq(const  ConnId& connId,TowerBeginReq & req)
{

	UInt32 stageID = 0;
	UInt32 chapterID = 0;

	PlayerTowerData towerData;
	TowerBeginResp resp;

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if (player == NULL)
	{
		return;
	}
	player->ResetFireConfirmData();	

	towerData = player->getPlayerTowerData();
	req.convertJsonToData(req.strReceive);
	resp.result = LynxErrno::None;

	stageID = req.id*1000 + 1;

	ClimbTowerManager::getSingleton().removeEvent(player->getPlayerGuid());

	PlayerDailyResetData dailyResetData= player->getPlayerDailyResetData();
	if(dailyResetData.m_nClimbTowerTimes <= 0)
	{
		resp.result = LynxErrno::TimesNotEnough;		
	}


	TowerTemplate* towerTemplate = gTowersTable->get(stageID);
	if (towerTemplate != NULL)
	{		
		chapterID = towerTemplate->chapterId;
	}

	if (req.id > chapterID && chapterID != 0)
	{
		resp.result = LynxErrno::NotOpen;
	}

	if (resp.result != LynxErrno::None)
	{
		std::string jsonStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,TOWER_BEGIN_RESP,jsonStr);
		return;
	}
	dailyResetData.m_nClimbTowerTimes --;
	player->setPlayerDailyResetData(dailyResetData);
	player->getPersistManager().setDirtyBit(DAILYRESETBIT);

	//初始化
	EventList eventList;
	eventList.towerHight = stageID;
	eventList.startTime = TimeUtil::getTimeSec();
	eventList.SPStartTime = TimeUtil::getTimeSec();
	towerTemplate = gTowersTable->get(stageID);
	if (towerTemplate == NULL)
	{
		resp.result = LynxErrno::ClienServerDataNotMatch;
		std::string sendStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,TOWER_BEGIN_RESP,sendStr);
		return;
	}
	TowerTypeTemplate* towerTypeTemplate = gTowerTypeTable->get(towerTemplate->chapterId);
	if (towerTypeTemplate == NULL)
	{
		LOG_WARN("towerTypeTemplate not found!!");
		resp.result = LynxErrno::ClienServerDataNotMatch;
		std::string sendStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,TOWER_BEGIN_RESP,sendStr);
		return;
	}


	eventList.towerData.m_HighID = stageID;	
 	eventList.towerData.m_Score = towerTypeTemplate->startscore + towerData.m_Score;
	eventList.towerData.m_AP = towerData.m_AP;
	eventList.towerData.m_HP = towerData.m_HP;
	eventList.towerData.m_SP = towerTypeTemplate->strength;
// 	eventList.reliveTimes = 0;//已经remove掉了，所以不用再清了 
// 	eventList.buyList.clear();
// 	eventList.towerBuff.clear();
// 	eventList.goods.clear();


	towerData.m_SP = towerTypeTemplate->strength;

	player->setPlayerTowerData(towerData);
	player->getPersistManager().setDirtyBit(TOWERDATABIT);
	ClimbTowerManager::getSingleton().setEventList(player->getPlayerGuid(),eventList);
	ClimbTowerManager::getSingleton().getNextEvents(player->getPlayerGuid(),stageID,resp.events,DefaultRoad,1000);

	resp.id = req.id;
	resp.score = towerTypeTemplate->startscore + towerData.m_Score;	
	resp.times = dailyResetData.m_nClimbTowerTimes;	
	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,TOWER_BEGIN_RESP,jsonStr);

}


void ClimbTowerManager::setEventList(Guid playerID,EventList &eventList)
{
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);	
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}

	for (Map<Guid,EventList>::Iter * it = mTowerEvents.begin();it!= NULL;it = mTowerEvents.next(it) )
	{
		if (it->mKey == playerID)
		{
			mTowerEvents.erase(it);
			break;
		}
	}
	mTowerEvents.insert(playerID,eventList);

}

void ClimbTowerManager::removeEvent(Guid playerID)
{
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);	
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}

	for (Map<Guid,EventList>::Iter * it = mTowerEvents.begin();it!= NULL;it = mTowerEvents.next(it) )
	{
		if (it->mKey == playerID)
		{
			mTowerEvents.erase(it);
			break;
		}
	}

}

void ClimbTowerManager::onInTowerChoiseReq(const  ConnId& connId,InTowerChoiseReq & req)
{

	PlayerTowerData playerTowerData;
	PlayerTowerData towerData;
	List<Goods> itemList;
	InTowerChoiseResp resp;

	req.convertJsonToData(req.strReceive);
	resp.chooseType = req.type;

	TowerTemplate*	towerTemplate1 =gTowersTable->get(req.tID);
	if (towerTemplate1 == NULL)
	{
		resp.result = LynxErrno::ClienServerDataNotMatch;
		std::string sendStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,IN_TOWER_CHOICE_RESP,sendStr);
		return;
	}

	//checkIsOver
	UInt32 ret = ClimbTowerManager::getSingleton().checkIsOver(connId);
	if(ret == LynxErrno::TimeOut)//体力不够发isover =1 hp为零发isover=0
	{
		resp.isOver = 1;
		ClimbTowerManager::getSingleton().sendChoiseResp(connId, resp);
		return;
	}
	if(ret == LynxErrno::Lose)
	{
		ClimbTowerManager::getSingleton().sendChoiseResp(connId, resp);
		return;
	}


	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}
	EventList *eventList = mTowerEvents.findRealValue(player->getPlayerGuid());

	//防止出现崩溃
	UInt32 maxhigh =  ClimbTowerManager::getSingleton().getTowerHighest(req.tID);
	if (eventList == NULL)
	{
		resp.result = LynxErrno::ClienServerDataNotMatch;
		std::string sendStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,IN_TOWER_CHOICE_RESP,sendStr);
		return;
	}
	if (maxhigh == 0 || req.tID > maxhigh || req.tID != eventList->towerData.m_HighID)
	{
		LOG_WARN("not the same tower high !!");
		resp.result = LynxErrno::ClienServerDataNotMatch;
		std::string sendStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,IN_TOWER_CHOICE_RESP,sendStr);
		return;
	}


	
	resp.result =  ClimbTowerManager::getSingleton().addEventValues(player->getPlayerGuid(), req,resp);
	playerTowerData = player->getPlayerTowerData();
	eventList = mTowerEvents.findRealValue(player->getPlayerGuid());

	if (resp.result == LynxErrno::IsBuying)//购买可以多次
	{
		resp.result = LynxErrno::None;
		ClimbTowerManager::getSingleton().sendChoiseResp(connId, resp);
		return;
	}
	else if (resp.result != LynxErrno::None && resp.result != LynxErrno::IsBuying)
	{
		if (resp.result == LynxErrno::TimeOut)
		{
			resp.isOver = 1;
		}
		resp.result = LynxErrno::None;
		ClimbTowerManager::getSingleton().sendChoiseResp(connId, resp);
		return;
	}
	else if (eventList->towerHight > maxhigh +1)
	{
		//LOG_INFO(" over maxhigh+1 eventList->towerHight = %d",eventList->towerHight);
		resp.result = LynxErrno::ClienServerDataNotMatch;
		std::string sendStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,IN_TOWER_CHOICE_RESP,sendStr);
		return;
	}

	//add to player	
	if (req.type == 7 &&req.wID == 0)
	{
		UInt32 getIt = 0;
		TowerEvent towerEvent;
		UInt32 road = DefaultRoad;
		if ((eventList->towerData.m_HighID - eventList->roadType) < 4 )
		{
			road = CrossRoad;
		}
		for (List<TowerEvent>::Iter * iter = eventList->events.begin(); iter!=NULL;iter = eventList->events.next(iter))
		{		
			if (iter->mValue.tID == req.tID && iter->mValue.mID == road )//road 必须是之前的road
			{
				getIt =1;			
				towerEvent =iter->mValue;
				break;
			}
		}
		if (getIt == 0)
		{
			resp.result = LynxErrno::ClienServerDataNotMatch;
			std::string sendStr = resp.convertDataToJson();
			NetworkSystem::getSingleton().sender( connId,IN_TOWER_CHOICE_RESP,sendStr);
			return;
		}

		UInt32 jumpCount = 0;		
		jumpCount = towerEvent.typeValue ;
		if(eventList->towerHight + towerEvent.typeValue +1 >maxhigh+1)
		{
			jumpCount = maxhigh + 1 - eventList->towerHight - 1;//越几层
		}

		for (UInt32 i = 0;i<=jumpCount;i++)//原来1层，加跳跃的几层
		{
			TowerTemplate* towerTemplate = gTowersTable->get(eventList->towerData.m_HighID);
			if (towerTemplate != NULL)			
			{
				ClimbTowerManager::getSingleton().changeScore(player->getPlayerGuid(),towerTemplate->awardOther,true);//前面有判断了
				
				if (playerTowerData.m_HighID < eventList->towerHight && eventList->towerHight <= maxhigh)
				{
					List<Goods> goodsList;
					if (eventList->towerHight >= maxhigh)//先加了
					{
						playerTowerData.m_HighID = maxhigh + 1000 - maxhigh%1000;
					}
					else
					{
						playerTowerData.m_HighID = eventList->towerHight;
					}
					GiftManager::getSingleton().getContentByID(towerTemplate->firstAwardID,goodsList);
					GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),1,goodsList,MiniLog108);
					eventList->itemList += goodsList;
					player->setPlayerTowerData(playerTowerData);
					player->getPersistManager().setDirtyBit(TOWERDATABIT);

					if (playerTowerData.m_HighID == 4000)
					{
						LogicSystem::getSingleton().sendSystemMsg(8, player->getPlayerName(), 0);
						player->getAchieveManager().updateAchieveData(WXCHALLENGESUC, 3);
					}
					else if (playerTowerData.m_HighID == 3000)
					{
						player->getAchieveManager().updateAchieveData(WXCHALLENGESUC, 2);
					}
					else if (playerTowerData.m_HighID == 2000)
					{
						player->getAchieveManager().updateAchieveData(WXCHALLENGESUC, 1);
					}
				}
				

			}	
			eventList->towerData.m_HighID ++;
			eventList->towerHight ++;

			if (eventList->towerData.m_HighID > maxhigh)
			{
				eventList->towerData.m_HighID = maxhigh;
			}
		}
	}
	else
	{
		if(eventList->towerHight <= maxhigh )
		{
			TowerTemplate* towerTemplate = gTowersTable->get(eventList->towerData.m_HighID);
			if (towerTemplate != NULL)			
			{
				ClimbTowerManager::getSingleton().changeScore(player->getPlayerGuid(),towerTemplate->awardOther,true);//前面有判断了

				if (playerTowerData.m_HighID < eventList->towerHight && eventList->towerHight <= maxhigh)
				{
					List<Goods> goodsList;
					if (eventList->towerHight >= maxhigh)//先加了
					{
						playerTowerData.m_HighID = maxhigh + 1000 - maxhigh%1000;
					}
					else
					{
						playerTowerData.m_HighID = eventList->towerHight;
					}
					GiftManager::getSingleton().getContentByID(towerTemplate->firstAwardID,goodsList);
					GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),1,goodsList,MiniLog108);
					eventList->itemList += goodsList;
					player->setPlayerTowerData(playerTowerData);
					player->getPersistManager().setDirtyBit(TOWERDATABIT);

					if (playerTowerData.m_HighID == 4000)
					{
						LogicSystem::getSingleton().sendSystemMsg(8, player->getPlayerName(), 0);
						player->getAchieveManager().updateAchieveData(WXCHALLENGESUC, 3);
					}
					else if (playerTowerData.m_HighID == 3000)
					{
						player->getAchieveManager().updateAchieveData(WXCHALLENGESUC, 2);
					}
					else if (playerTowerData.m_HighID == 2000)
					{
						player->getAchieveManager().updateAchieveData(WXCHALLENGESUC, 1);
					}
				}


			}	
			eventList->towerData.m_HighID ++;//这个值下面还要用所以不能超过最大值
			eventList->towerHight ++;//这个要发送给客户端因此要发到maxhigh+1

			if (eventList->towerData.m_HighID > maxhigh)
			{
				eventList->towerData.m_HighID = maxhigh;
			}
		}
	}

	ClimbTowerManager::getSingleton().sendChoiseResp(connId, resp);
}

void ClimbTowerManager::sendChoiseResp(const  ConnId& connId,InTowerChoiseResp resp)
{
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if (player == NULL)
	{
		return;
	}
	EventList *eventList = mTowerEvents.findRealValue(player->getPlayerGuid());
	if (eventList == NULL)
	{
		LOG_WARN("eventList not found!!");
		return;
	}
	//checkIsOver
	if(ClimbTowerManager::getSingleton().checkIsOver(connId)==LynxErrno::TimeOut)//体力不够发isover =1 hp为零发isover=0
	{
		resp.isOver = 1;
	}

	resp.Hp = eventList->towerData.m_HP;
	resp.Sp = eventList->towerData.m_SP;
	resp.tID = eventList->towerHight;
	resp.Ap = eventList->towerData.m_AP;
	resp.score = eventList->towerData.m_Score;
	
	getTowerBuff(player->getPlayerGuid(),resp.buffID1,resp.time1,resp.buffID2,resp.time2);
	int tmpAP = resp.Ap;
	getBuffValue(player->getPlayerGuid(),TOWER_ADD_AP,tmpAP);//可能会有增加攻击力buff
	resp.Ap = tmpAP;


	resp.time1 = player->getPlayerTowerData().m_HP;
	
	
	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender(connId,IN_TOWER_CHOICE_RESP,jsonStr);
	//LOG_INFO("IN_TOWER_CHOICE_RESP = %s",jsonStr.c_str());

}


UInt32 ClimbTowerManager::checkIsOver(const  ConnId& connId)
{

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return LynxErrno::NotFound;
	}

	EventList *eventList = mTowerEvents.findRealValue(player->getPlayerGuid());
	if (eventList == NULL)
	{
		return LynxErrno::NotFound;
	}

	//判断胜负

	UInt32 ret = changSP(player->getPlayerGuid(),0);
	if(ret != LynxErrno::None)
	{
		return LynxErrno::TimeOut;
	}

	 ret = changHP(player->getPlayerGuid(),0,false);

	if (ret != LynxErrno::None)
	{		
		return LynxErrno::Lose;
	}

	
	
	
	return LynxErrno::None;
}


// void ClimbTowerManager::onTowerFloorEndReq(const  ConnId& connId,TowerFloorEndReq & req)
// {
// 	UInt32 nextID = req.id + 1;
// 	TowerFloorEndResp resp;
// 
// 
// 	req.convertJsonToData(req.strReceive);
// 	resp.result = LynxErrno::None;
// 
// 	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
// 
// 	EventList *eventList = mTowerEvents.findRealValue(player->getPlayerGuid());
// 
// 	std::string jsonStr = resp.convertDataToJson();
// 	NetworkSystem::getSingleton().sender( connId,TOWER_FLOOR_END_RESP,jsonStr);
// 
// 	//判断胜负
// 	UInt32 nowTime = TimeUtil::getTimeSec();
// 
// 	TowerTemplate* towerTemplate = gTowersTable->get(req.id);
// 	if (towerTemplate == NULL)
// 	{
// 		return;
// 	}
// 
// 	TowerTypeTemplate* towerTypeTemplate = gTowerTypeTable->get(towerTemplate->chapterId);
// 
// 	if ((nowTime - eventList->startTime)*towerTypeTemplate->strengthreduce >= towerTypeTemplate->strength)
// 	{
// 		TowerEndReq req2;
// 		onTowerEndReq(connId ,req2);
// 		return;
// 	}
// 
// 	resp.result = ClimbTowerManager::getSingleton().changHP(player->getPlayerGuid(),0,false);
// 
// 
// 	if (resp.result != LynxErrno::None)
// 	{
// 		std::string jsonStr = resp.convertDataToJson();
// 		NetworkSystem::getSingleton().sender( connId,TOWER_FLOOR_END_RESP,jsonStr);
// 	}
// 
// 	UInt32 maxhigh =  ClimbTowerManager::getSingleton().getTowerHighest(eventList->towerData.m_HighID);
// 	eventList->towerData.m_HighID++;
// 
// 	if (eventList->towerData.m_HighID > maxhigh)
// 	{
// 		eventList->towerData.m_HighID = maxhigh;
// 		TowerEndReq req3;
// 		// 		req3.id = eventList->towerData.m_HighID;
// 		req3.reqType = 2;
// 		ClimbTowerManager::getSingleton().onTowerEndReq(connId ,req3 );
// 	}
// 
// 	ClimbTowerManager::getSingleton().subPlayerEvent(player->getPlayerGuid(),eventList->towerData.m_HighID);
// 
// }

//填错表有可能进入死循环
void ClimbTowerManager::getNextEvents(Guid playerID,UInt32 towerHight,List<TowerEvent> &events,UInt32 road,UInt32 num)
{

	UInt32 towerID = 0;
	UInt32 msgID = 0;
	UInt32 AP = 0;
	UInt32 HP = 0;
	UInt32 SP = 0;
	UInt32 score = 0;
	UInt32 times = 0;	  
	UInt32 maxhigh = 0;
	UInt32 minhigh = 0;
	UInt32 endHight =0;
	TowerEventTemplate* towerEventTemplate;
	List<Goods> goods;	

	EventList *eventList = mTowerEvents.findRealValue(playerID);
	if (eventList == NULL)
	{
		LOG_WARN("eventList not found!!");
		return;
	}

		

	maxhigh =  getTowerHighest(towerHight);
	minhigh =  maxhigh - maxhigh%100 + 1;

	if (num >= 1000)
	{
		endHight = maxhigh;
	}
	else
	{
		endHight = towerHight + num -1;
	}
	if (endHight > maxhigh)
	{
		endHight = maxhigh;
	}

	events.clear();
	for(UInt32 i = towerHight;i<= endHight;i++)
	{

		
		TowerEvent towerEvent;
		getPlayerEvent( playerID,towerEvent, towerHight,road);

		if (towerEvent.eID > 0)
		{
			towerHight++;
			if (towerHight > maxhigh)
			{
				towerHight = maxhigh;
			}
			if (towerEvent.mID == CrossRoad)//分岔路重复要继续加入进去
			{
				events.insertTail(towerEvent);
			}
			continue;
		}

		for (UInt32 ii = 0;ii<100;ii++)
		{
			towerEvent.eID = rollEvent(towerHight);
			towerEventTemplate = gTowerEventTable->get(towerEvent.eID);



			if (towerEventTemplate == NULL)
			{			
				break;
			}
			if (towerEventTemplate->type == 7)
			{
				if (towerHight < (minhigh + 10) )
				{
					continue;
				}
				
			}
			if (towerEventTemplate->type == 9)
			{
				if (towerHight < (minhigh + 10)|| (towerHight > maxhigh - 3)||((towerHight - eventList->latestCrossRoadFloor) <= 3))
				{
					continue;
				}
			}
			break;
		}
		
		if (towerEvent.eID == 1||towerEvent.eID == 0)
		{
			continue;
		}

		if (towerEventTemplate->type == 7)
		{
			eventList->latestJumpFloor = towerHight;
		}
		if (towerEventTemplate->type == 9)
		{
			eventList->latestCrossRoadFloor = towerHight;			
		}
		towerEvent.tID = towerHight;
		towerEvent.mID = road;

		spiltEvent(towerEvent);

		dealEventfollowEvent(playerID,towerEvent,events);

		events.insertTail(towerEvent);

		towerHight++;
		if (towerHight > maxhigh)
		{
			towerHight = maxhigh;
		}

	}
}

UInt32 ClimbTowerManager::rollEvent(UInt32 towerHight)
{	 
	UInt32 allRate =0;
	UInt32 randomNum = 0;
	UInt32 rates = 0;
	UInt32 eventId = 0;
	TowerTemplate* towerTemplate = gTowersTable->get(towerHight);

	if (towerTemplate == NULL)
	{
		return 1;
	}
	allRate = towerTemplate->eventrate1 + towerTemplate->eventrate2 +towerTemplate->eventrate3 + towerTemplate->eventrate4 + towerTemplate->eventrate5;
	if (allRate == 0)
	{
		return 0;//除0
	}
	randomNum = rand() % allRate;
	for (UInt32 i =1;i<11;i++)
	{

		if(i==1)
		{
			rates += towerTemplate->eventrate1;
			eventId = towerTemplate->event1;
		}
		else if (i==2)
		{
			rates += towerTemplate->eventrate2;
			eventId = towerTemplate->event2;
		}
		else if (i==3)
		{
			rates += towerTemplate->eventrate3;
			eventId = towerTemplate->event3;
		}
		else if (i==4)
		{
			rates += towerTemplate->eventrate4;
			eventId = towerTemplate->event4;
		}
		else if (i==5)
		{
			rates += towerTemplate->eventrate5;
			eventId = towerTemplate->event5;
		}

		if(rates > randomNum)//没有与最大值相等
		{
			return eventId;
		}
	}
	return 0;

}

void ClimbTowerManager::spiltEvent(TowerEvent &towerEvent)
{
	UInt32 randomNum = rand()%10000;
	UInt32 rates = 0;
	UInt32 oID =0;

	TowerEventTemplate* towerEventTemplate = gTowerEventTable->get(towerEvent.eID);

	if (towerEventTemplate == NULL)
	{
		return;
	}

	// 	 60100101_2000;60100102_2000;60100103_2000;60100104_2000;60100105_2000

	Vector<String> strVector1;

	if (towerEventTemplate->type == 1 ||towerEventTemplate->type == 2||towerEventTemplate->type == 3||towerEventTemplate->type == 4||towerEventTemplate->type == 13)
	{
		lynxStrSplit(towerEventTemplate->param, ";", strVector1, true);
		for(UInt32 i =0;i<strVector1.size();i ++)
		{
			Vector<String> strVector2;
			lynxStrSplit(strVector1[i], "_", strVector2, true);
			rates += atoi(strVector2[1].c_str());
			if (rates > randomNum)
			{
				oID = atoi(strVector2[0].c_str());
				break;
			}
		}
	}
	else 
	{
		oID = atoi(towerEventTemplate->param.c_str());
	}

	towerEvent.eID = towerEventTemplate->id;	
	towerEvent.typeValue = oID;
	towerEvent.type = towerEventTemplate->type ;	
	return ;
}
void ClimbTowerManager::dealEventfollowEvent(Guid playerID,TowerEvent &towerEvent,List<TowerEvent> &events)
{
	UInt32 eventType = 0;
	UInt32 high = 0;
	PlayerTowerData towerData;
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);	
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}
	EventList *eventList = mTowerEvents.findRealValue(player->getPlayerGuid());
	if (eventList == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}
	towerData = player->getPlayerTowerData();
	high = towerEvent.tID;
	if (high == 0)
	{
		high = 1001;
	}

	TowerEventTemplate* towerEventTemplate = gTowerEventTable->get(towerEvent.eID);

	if (towerEventTemplate != NULL)
	{
		eventType = towerEventTemplate->type;
	}


	if (eventType == 1)
	{
		TowerMonsterTemplate* towerMonsterTemplate = gTowerMonsterTable->get(towerEvent.typeValue);
		if (towerMonsterTemplate == NULL)
		{
			LOG_WARN("towerMonsterTemplate not found!!");
			return;
		}

		towerEvent.Ap = towerMonsterTemplate->attackpower;
		towerEvent.Hp = towerMonsterTemplate->HP;
		towerEvent.score = towerMonsterTemplate->score;
		towerEvent.scoreCost = towerMonsterTemplate->briberycost;
	}
	else if (eventType == 2)
	{	 

	}
	else if (eventType == 3)
	{

		TowerBoxTemplate * towerBoxTemplate = gTowerBoxTable->get(towerEvent.typeValue);
		if (towerBoxTemplate == NULL)
		{
			return;
		}
		towerEvent.Ap = towerBoxTemplate->attackpower;
		towerEvent.Hp = towerBoxTemplate->HP;
		towerEvent.score = 0;
		towerEvent.scoreCost = towerBoxTemplate->scorecost;	  
		GiftManager::getSingleton().getContentByID(towerBoxTemplate->awardcontent,towerEvent.goods);

	}
	else if (eventType == 4)
	{
	}
	else if (eventType == 5 )
	{
	}
	else if (eventType == 6 )
	{
	}
	else if (eventType == 8 )
	{
		towerEvent.score = towerEvent.typeValue;	  
	}
	else if (eventType == 10 )
	{
		towerEvent.scoreCost = towerEvent.typeValue;	  
	}
	else if (eventType == 11 )
	{
	}
	else if (eventType == 12 )
	{
// 		towerEvent.Hp = towerData.m_HP * towerEvent.typeValue/10000;	  
	}
	else if (eventType == 7 )
	{
		EventList *eventList = mTowerEvents.findRealValue(playerID);
		if (eventList == NULL)
		{
			return;
		}
		if((towerEvent.tID - eventList->lastJumpFloor) < 10)
		{
			return;
		}
	}
	else if (eventType == 9 )
	{	 
		ClimbTowerManager::getSingleton().getNextEvents(playerID,high+1,towerEvent.crossEvents,CrossRoad,EventNum);
	}
	else if (eventType == 13)
	{
		TowerEvent towerEvent100;
		towerEvent100.eID = towerEvent.typeValue;

		spiltEvent(towerEvent100);

		dealEventfollowEvent(playerID,towerEvent100,events);

		towerEvent.isRandom = 1;
		towerEvent.type = towerEvent100.type;
		towerEvent.eID = towerEvent100.eID;
		towerEvent.typeValue = towerEvent100.typeValue;
		towerEvent.Ap = towerEvent100.Ap;
		towerEvent.Hp = towerEvent100.Hp;
		towerEvent.score = towerEvent100.score;
		towerEvent.scoreCost = towerEvent100.scoreCost;
		towerEvent.goods = towerEvent100.goods;
		towerEvent.crossEvents = towerEvent100.crossEvents;


		TowerEventTemplate* towerEventTemplate = gTowerEventTable->get(towerEvent100.eID);
		if (towerEventTemplate != NULL)
		{
			towerEvent.type = towerEventTemplate->type ;
		}
	}
	setPlayerEvent( playerID, towerEvent);

}

void ClimbTowerManager::getPlayerEvent(Guid playerID,TowerEvent &towerEvent,UInt32 towerHight,UInt32 mID)
{
	UInt32 getIt =0;
	EventList *eventList = mTowerEvents.findRealValue(playerID);

	if (eventList == NULL)
	{
		return;
	}
	for (List<TowerEvent>::Iter * iter = eventList->events.begin(); iter!=NULL;iter = eventList->events.next(iter))
	{
		if (iter->mValue.tID == towerHight && iter->mValue.mID == mID)
		{
			getIt =1;
			towerEvent = iter->mValue;
			break;
		}
	}

	return;
}

void ClimbTowerManager::setPlayerEvent(Guid playerID,TowerEvent towerEvent)
{
	UInt32 getIt =0;
	EventList *eventList = mTowerEvents.findRealValue(playerID);
	for (List<TowerEvent>::Iter * iter = eventList->events.begin(); iter!=NULL;iter = eventList->events.next(iter))
	{
		if (iter->mValue.tID == towerEvent.tID && iter->mValue.eID == towerEvent.eID&& iter->mValue.mID == towerEvent.mID)
		{
			getIt =1;
			iter->mValue = towerEvent;
		}
	}
	if (getIt == 0)
	{
		eventList->events.insertTail(towerEvent);
	}
	return;
}

void ClimbTowerManager::subPlayerEvent(Guid playerID,UInt32 towerHight)
{
	UInt32 getIt =0;
	EventList *eventList = mTowerEvents.findRealValue(playerID);
	if(eventList == NULL)
	{
		return;
	}
	for (List<TowerEvent>::Iter * iter = eventList->events.begin(); iter!=NULL;iter = eventList->events.next(iter))
	{
		if (iter->mValue.tID <= towerHight)
		{
			eventList->events.erase(iter);
			break;
		}
	}

	return;
}

UInt32 ClimbTowerManager::getBattleReport(Guid playerID,List<BattleReport> &battleReportList,TowerEvent &towerEvent)
{
	int ap = 0;	
	int addAp = 0;
	UInt32 flag = 0;
	UInt32 spReduce = 0;
	string str;
	string playerName;
	string fileName = "towerNoReport.log";
	char dest[1024] = {0};

	changSP(playerID,0);//要放最前面


	EventList *eventList = mTowerEvents.findRealValue(playerID);
	if(eventList == NULL)
	{
		str = "eventList not found!!!";
		LoggerSystem::getSingleton().writeLog(fileName.c_str(),playerID,"",str);
		return LynxErrno::NotFound;
	}

	int enemyBlood =  towerEvent.Hp;
	int ownBlood = eventList->towerData.m_HP;
	int sp = eventList->towerData.m_SP;
	ap = eventList->towerData.m_AP;

	if (enemyBlood == 0)
	{
		str = "already lose !!!";
		LoggerSystem::getSingleton().writeLog(fileName.c_str(),playerID,"",str);
		return LynxErrno::Lose;
	}

	TowerTemplate*	towerTemplate = gTowersTable->get(eventList->towerData.m_HighID);
	if (towerTemplate == NULL)
	{
		sprintf(dest,"towerTemplate not found  !!! m_HighID = %d !!! ",eventList->towerData.m_HighID);
		str = (string)dest;
		LoggerSystem::getSingleton().writeLog(fileName.c_str(),playerID,"",str);
		return LynxErrno::NotFound;
	}

	TowerTypeTemplate *towerTypeTemplate = gTowerTypeTable->get(towerTemplate->chapterId);
	if (towerTypeTemplate == NULL)
	{
		sprintf(dest,"towerTypeTemplate not found  !!! chapterId = %d !!! ",towerTemplate->chapterId);
		str = (string)dest;
		LoggerSystem::getSingleton().writeLog(fileName.c_str(),playerID,"",str);
		return 0;
	}
	getBuffValue(playerID,TOWER_ADD_AP,ap);		
	if (getBuffValue(playerID,TOWER_SUB_SP,ap)==false )//体力不减
	{
		spReduce = 1;
	}

	
	for (UInt32 i = 0;i < 200;i++)
	{
		changHP(playerID,(0 - towerEvent.Ap),false);
		if (spReduce == 0)
		{
			sp --;
		}
		eventList = mTowerEvents.findRealValue(playerID);
		ownBlood = eventList->towerData.m_HP;
		enemyBlood = enemyBlood - ap;
		if (ownBlood <= 0)
		{
			if (enemyBlood <= 0)//同归于尽就给10血
			{
				enemyBlood = 10;
			}
			towerEvent.Hp = enemyBlood;
			flag = 2;
			ownBlood = 0;
		}
		if (enemyBlood <= 0&&flag == 0)
		{
			flag = 1;
			enemyBlood = 0;
		}
		if (sp <= 0&&flag == 0)
		{
			flag = 3;
		}
		towerEvent.Hp = enemyBlood;
		BattleReport battleReport;
		battleReport.ownBlood = ownBlood;
		battleReport.enemyBlood = enemyBlood;
		battleReport.flag = flag;
		battleReportList.insertTail(battleReport);
		


		if (flag != 0)
		{
			break;
		}
	}
	if (battleReportList.size() == 0)
	{
		str = "battleReportList size = 0!!!!";
		LoggerSystem::getSingleton().writeLog(fileName.c_str(),playerID,"",str);
	}
	//LOG_INFO("battleReportList size = %d",battleReportList.size());
	return flag;
}
// 1遇敌 2神秘驿站 3遭遇宝箱 4随机buff 5回复生命 6回复体力
// 7跃层 8获得积分 9分岔路 10减少积分 11减少体力 12减少hp 13随机事件
UInt32 ClimbTowerManager::addEventValues(Guid playerID,InTowerChoiseReq & req,InTowerChoiseResp &resp)
{
	
	UInt32 getIt =0;
	UInt32 getShopItem =0;
	UInt32 eventType = 0;
	UInt32 result = 0;
	UInt32 flag = 0;
	TowerEvent towerEvent;
	TowerEvent *editTowerEvent;
	PlayerTowerData towerData;
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);	
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return LynxErrno::NotFound;
	}
	towerData = player->getPlayerTowerData();
	EventList *eventList = mTowerEvents.findRealValue(playerID);

	if (eventList == NULL)
	{
		LOG_WARN("eventList not found!!");
		return LynxErrno::NotFound;
	}

	UInt32 road = DefaultRoad;
	if ((eventList->towerData.m_HighID - eventList->roadType) < 4 )
	{
		road = CrossRoad;
	}
	for (List<TowerEvent>::Iter * iter = eventList->events.begin(); iter!=NULL;iter = eventList->events.next(iter))
	{		
		if (iter->mValue.tID == req.tID && iter->mValue.mID == road )
		{
			getIt =1;			
			towerEvent = iter->mValue;
			editTowerEvent = &iter->mValue;
			break;
		}
	}


	if (getIt == 0)
	{
		return LynxErrno::NotFound;
	}

	TowerEventTemplate* towerEventTemplate = gTowerEventTable->get(towerEvent.eID);

	if (towerEventTemplate != NULL)
	{	
		eventType = towerEventTemplate->type;
	}
	

	if (eventType == 1)
	{
		if (req.wID == 1)
		{			
			flag = getBattleReport(playerID,resp.battleReport,*editTowerEvent);
			if (flag == 1)
			{
				int var = towerEvent.score;
				ClimbTowerManager::getSingleton().changeScore(player->getPlayerGuid(),var,true);				
			}
			else if (flag == 2)
			{
				return LynxErrno::Lose;
			}
			else if (flag == 3)
			{
				return LynxErrno::Lose;
			}
		}
		else
		{
			if (eventList->towerData.m_Score < towerEvent.scoreCost)
			{
				return LynxErrno::scoreNotEnough;
			}
			int var = 0 - towerEvent.scoreCost;
			ClimbTowerManager::getSingleton().changeScore(player->getPlayerGuid(),var,true);		

		}
		return LynxErrno::None;

	}
	else if (eventType == 2)
	{	 	
		//0的时候购买结束
		UInt32 which = 0;
		Goods goods;
		if (req.wID == 0)
		{		
			eventList->buyList.clear();
			return LynxErrno::None;
		}
		ShopsTemplate* shopsTemplate = gShopsTable->get(towerEvent.typeValue);
		if (shopsTemplate == NULL)
		{
			LOG_WARN("shopsTemplate not found!!");
			return LynxErrno::NotFound;
		}
		for(List<Sell>::Iter * it = shopsTemplate->sellList.begin();it != NULL;it = shopsTemplate->sellList.next(it))
		{
			which ++;
			if (which == req.wID)
			{
				getShopItem = 1;
				if(checkCanBuy(playerID,towerEvent.tID,towerEvent.typeValue, which) == LynxErrno::Found)
				{
					return LynxErrno::Found;
				}
				Vector<String> strVector;
				lynxStrSplit(it->mValue.cost, ";", strVector, true);
				if (strVector.size() < 3)
				{
					return LynxErrno::scoreNotEnough;
				}
				if (atoi(strVector[2].c_str()) > eventList->towerData.m_Score)
				{
					return LynxErrno::scoreNotEnough;
				}
				int var = 0 - atoi(strVector[2].c_str());
				ClimbTowerManager::getSingleton().changeScore(player->getPlayerGuid(),var,true);	

				goods.resourcestype = it->mValue.restype;
				goods.subtype = it->mValue.subtype;
				goods.num = it->mValue.num;

				List<Goods> goodsList;
				goodsList.insertTail(goods);
				resp.goods = goodsList;

				dealTowerGoods(playerID,goodsList);

				ShopItem  shopItem;
				shopItem.high = towerEvent.tID;
				shopItem.shopID = towerEvent.typeValue;
				shopItem.which = which;
				eventList->buyList.insertTail(shopItem);

				break;
			}
		}
		return LynxErrno::IsBuying;	
	}
	else if (eventType == 3)
	{
		if (req.wID == 1)//打开箱子//修改过
		{
			int var = 0 - editTowerEvent->scoreCost;
			editTowerEvent->scoreCost = 0;
			if (changeScore(player->getPlayerGuid(),var,false) != LynxErrno::None)
			{
				return LynxErrno::scoreNotEnough;
			}
			if (editTowerEvent->Hp > 0)
			{
				flag = getBattleReport(playerID,resp.battleReport,*editTowerEvent);
				if (flag == 1)
				{	
					dealTowerGoods(playerID,editTowerEvent->goods);
					resp.goods = editTowerEvent->goods;
				}
				else if (flag == 2)
				{
					return LynxErrno::Lose;
				}
				else if (flag == 3)
				{
					return LynxErrno::Lose;
				}
			}
			else
			{
				dealTowerGoods(playerID,editTowerEvent->goods);
				resp.goods = editTowerEvent->goods;
			}
		}
		else
		{

		}
		return LynxErrno::None;
	}
	else if (eventType == 4)//BUFF
	{
		setTowerBuff(playerID,towerEvent.typeValue);
		return LynxErrno::None;
	}
	else if (eventType == 5 )
	{		
		int val = 0;
		double tmpValue = 0;
		tmpValue = (double)towerEvent.typeValue/10000;
		tmpValue =(double) towerData.m_HP *tmpValue;
		val = (int)tmpValue;


		return changHP(playerID,val,false);
	}
	else if (eventType == 6 )
	{	
		//LOG_INFO("changSP 6 towerEvent.typeValue = %d",towerEvent.typeValue);
		int val = 0;
		double tmpValue = 0;
		tmpValue = (double)towerEvent.typeValue/10000;
		tmpValue =(double) towerData.m_SP *tmpValue;
		val = (int)tmpValue;

		return changSP(playerID,val);

	}
	else if (eventType == 8 )
	{	
		return addTowerData(playerID,towerEvent.goods,TOWER_ADD_SCORE,towerEvent.typeValue);
	}
	else if (eventType == 10 )
	{
		int var = 0 - towerEvent.typeValue;
		return addTowerData(playerID,towerEvent.goods,TOWER_ADD_SCORE,var);
	}
	else if (eventType == 11 )
	{
		int val = 0;
		double tmpValue = 0;
		tmpValue = (double)editTowerEvent->typeValue/10000;
		tmpValue =(double)towerData.m_SP *tmpValue;
		val = 0 -  (int)tmpValue;

		//LOG_INFO("changSP 11 val = %d",val);
		return addTowerData(playerID,towerEvent.goods,TOWER_SUB_SP,val);		
	}
	else if (eventType == 12 )
	{
		int val = 0;
		double tmpValue = 0;
		tmpValue = (double)editTowerEvent->typeValue/10000;
		tmpValue =(double)towerData.m_HP *tmpValue;
		val = 0 -  (int)tmpValue;
	
		editTowerEvent->typeValue  = 0;
		return addTowerData(playerID,towerEvent.goods,TOWER_SUB_HP,val);
	}
	else if (eventType == 7 )
	{
		if (req.wID == 0)
		{
			UInt32 maxhigh =  getTowerHighest(eventList->towerData.m_HighID);
			if (eventList->towerData.m_HighID >maxhigh)
			{
				eventList->towerData.m_HighID  = maxhigh;
			}
		}
		else
		{

		}
		return LynxErrno::None;
	}
	else if (eventType == 9 )
	{	
		if (req.wID == 1)
		{
		}
		else
		{
			eventList->roadType = eventList->towerData.m_HighID;
		}		
		return LynxErrno::None;
	}

	return LynxErrno::None;
}

void ClimbTowerManager::dealTowerGoods(Guid playerID,List<Goods> goodsList)
{
	UInt32 getIt = 0;
	List<Goods> itemList;
	Player* player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}
	EventList *eventList = mTowerEvents.findRealValue(playerID);

	for( List<Goods>::Iter * iter = goodsList.begin();iter!= NULL;iter = goodsList.next(iter))
	{
		getIt = 0;
		if (iter->mValue.resourcestype == AWARD_BASE )
		{
			if (iter->mValue.subtype == AWARD_BASE_RANKEDSCORE )
			{
				getIt = 1;
				changeScore(player->getPlayerGuid(),iter->mValue.num,true);
			}
		}
		else if (iter->mValue.resourcestype == AWARD_TOWERBUFF )
		{
			getIt = 1;
			setTowerBuff(playerID,iter->mValue.subtype);
		}
		else if (iter->mValue.resourcestype == AWARD_TOWERATTR )
		{
			TowerAttrTemplate* towerAttrTemplate = gTowerAttrTable->get(iter->mValue.subtype );
			if (towerAttrTemplate == NULL)
			{
				return;
			}
			if (towerAttrTemplate->attr == AWARD_TOWERATTR_AP )
			{
				getIt = 1;
		
				changAP(playerID,iter->mValue.num,true);

			}
			else if (towerAttrTemplate->attr == AWARD_TOWERATTR_HP )
			{
				getIt = 1;
				//不加当前血量加玩家总属性血量
				changHP(playerID,iter->mValue.num,true);
			}
		}
		if (getIt == 0)		
		{		
			itemList.insertTail(iter->mValue);			
		}
	}
	if (itemList.size() != 0)
	{
		GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_FOOD,itemList,MiniLog108);
	}
	eventList->itemList += itemList;
}

//dataType 1 hp 2 ap 3 sp 4 score  
//bufftype 1攻击力加成（百分比）2	无敌	3	体力不减	4	积分获取翻倍	
bool ClimbTowerManager::getBuffValue(Guid playerID,UInt32 dataType,int &num)
{
	UInt32 getIt =0;	
	UInt32 buffID1 = 0;
	UInt32 time1 = 0;
	UInt32 buffID2 = 0;
	UInt32 time2 = 0;
	EventList *eventList;
	TowerEvent towerEvent;

	Player* player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return false;
	}

	getTowerBuff(playerID,buffID1,time1,buffID2,time2);
	eventList = mTowerEvents.findRealValue(playerID);

	for (List<TowerBuff>::Iter * it1 = eventList->towerBuff.begin();it1 != NULL;it1 = eventList->towerBuff.next(it1))
	{
		TowerBuffTemplate * towerBuffTemplate = gTowerBuffTable->get(it1->mValue.buffId);
		if (towerBuffTemplate == NULL)
		{
			continue;
		}
		if (towerBuffTemplate->attr == 1)
		{
			if (dataType == TOWER_ADD_AP)
			{
				int val = 0;
				double tmpValue = 0;
				tmpValue = (double)towerBuffTemplate->num/10000;
				tmpValue =(double)eventList->towerData.m_AP *tmpValue;
				num = eventList->towerData.m_AP +  (int)tmpValue;

				return false;
			}

		}
		if (towerBuffTemplate->attr == 2)
		{
			if (dataType == TOWER_SUB_HP)
			{
				return false;
			}

		}
		if (towerBuffTemplate->attr == 3)
		{
			if (dataType == TOWER_SUB_SP)
			{
				return false;
			}

		}
		if (towerBuffTemplate->attr == 4)
		{
			if (dataType == TOWER_ADD_SCORE)
			{
				if (num > 0)//负的不增加
				{
					int val = 0;
					double tmpValue = 0;
					tmpValue = (double)towerBuffTemplate->num/10000;
					tmpValue  = (double)num *tmpValue; 
					num = (int)tmpValue;
					
				}				
				return true;				
			}
		}
	}
	
	return true;
}


//dataType 1 hp 2 ap 3 sp 4 score  
//bufftype 1攻击力加成（百分比）2	无敌	3	体力不减	4	积分获取翻倍	

UInt32 ClimbTowerManager::addTowerData(Guid playerID,List<Goods> itemList,UInt32 dataType,int num)
{

	TowerEvent towerEvent;
	UInt32 getIt =0;
	EventList *eventList;
	UInt32 buffID1 = 0;
	UInt32 time1 = 0;
	UInt32 buffID2 = 0;
	UInt32 time2 = 0;

	Player* player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return LynxErrno::NotFound;
	}

	getTowerBuff(playerID,buffID1,time1,buffID2,time2);
	eventList = mTowerEvents.findRealValue(playerID);

	for (List<TowerBuff>::Iter * it1 = eventList->towerBuff.begin();it1 != NULL;it1 = eventList->towerBuff.next(it1))//有buff存在不减
	{
		TowerBuffTemplate * towerBuffTemplate = gTowerBuffTable->get(it1->mValue.buffId);
		if (towerBuffTemplate == NULL)
		{
			continue;
		}
		if (towerBuffTemplate->attr == 2)
		{
			if (dataType == TOWER_SUB_HP )
			{
				return LynxErrno::None;
			}

		}
		if (towerBuffTemplate->attr == 3)
		{
			if (dataType == TOWER_SUB_SP)
			{
				return LynxErrno::None;
			}

		}
		
	}
	UInt32 ret = 0 ;
	if (dataType == TOWER_ADD_AP)//有购买也有buff buff是直接getbuff 事件加攻击力
	{		
 		changAP(playerID,num,true);
	}
	if (dataType == TOWER_SUB_HP)
	{
		ret = changHP(player->getPlayerGuid(),num,false);
		if (ret != LynxErrno::None)
		{
			return ret;
		}
	}
	if (dataType == TOWER_ADD_SCORE)
	{
		changeScore(player->getPlayerGuid(),num,true);
	}
	if (dataType == TOWER_SUB_SP)
	{
		//LOG_INFO("changSP addTowerData num= %d",num);
		ret = changSP(player->getPlayerGuid(),num);
		if (ret != LynxErrno::None)
		{
			return ret;
		}
		
	}
	if (itemList.size() != 0)
	{
		GiftManager::getSingleton().addToPlayer(playerID,REFLASH_AWARD,itemList,MiniLog108);
		eventList->itemList+=itemList;
	}
	
	return LynxErrno::None;

}

UInt32 ClimbTowerManager::getTowerHighest(UInt32 towerHigh)
{

	TowerTemplate*	towerTemplate =gTowersTable->get(towerHigh);
	if (towerTemplate == NULL)
	{
		return 0;
	}
	if (towerTemplate->chapterId == 1)
	{
		return 1030;
	}
	else if (towerTemplate->chapterId == 2)
	{
		return 2060;
	}
	else if (towerTemplate->chapterId == 3)
	{
		return 3100;
	}

	return 0;

}
// 
// void ClimbTowerManager::onTowerBuffReq(const  ConnId& connId,TowerBuffReq & req)
// {
// 	TowerEvent towerEvent;
// 	UInt32 count =0;
// 	PlayerTowerData towerData;
// 	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
// 	towerData = player->getPlayerTowerData();
// 	EventList *eventList = mTowerEvents.findRealValue(player->getPlayerGuid());
// 	TowerBuffResp resp;
// 
// 	UInt32 nowTime = TimeUtil::getTimeSec();
// 	UInt32 towerHigh = eventList->towerData.m_HighID;
// 
// 	for (List<TowerBuff>::Iter * it = eventList->towerBuff.begin();it != NULL;it = eventList->towerBuff.next(it))
// 	{
// 		count++;
// 		TowerBuffTemplate * towerBuffTemplate = gTowerBuffTable->get(it->mValue.buffId);
// 
// 		if (towerBuffTemplate->attr == 4)
// 		{
// 			if (count == 1)
// 			{
// 				resp.buffID1 = towerBuffTemplate->id;
// 				resp.time1 = towerBuffTemplate->keepfloor - ( towerHigh - it->mValue.buffStartTime);
// 				if (resp.time1 <= 0)
// 				{
// 					eventList->towerBuff.erase(it);
// 					resp.buffID1 = 0;
// 					resp.time1 = 0;
// 				}
// 			}
// 			else if (count == 2)
// 			{
// 
// 				resp.buffID2 = towerBuffTemplate->id;
// 				resp.time2 = towerBuffTemplate->keepfloor - ( towerHigh - it->mValue.buffStartTime);
// 				if (resp.time1 <= 0)
// 				{
// 					eventList->towerBuff.erase(it);
// 					resp.buffID2 = 0;
// 					resp.time2 = 0;
// 				}
// 			}
// 
// 
// 		}
// 		else
// 		{
// 			if (count == 1)
// 			{
// 				resp.buffID1 = towerBuffTemplate->id;
// 				resp.time1 = towerBuffTemplate->keeptime - ( nowTime - it->mValue.buffStartTime);
// 				if (resp.time1 <= 0)
// 				{
// 					eventList->towerBuff.erase(it);
// 					resp.buffID1 = 0;
// 					resp.time1 = 0;
// 				}
// 			}
// 			else if (count == 2)
// 			{
// 				resp.buffID2 = towerBuffTemplate->id;
// 				resp.time2 = towerBuffTemplate->keeptime - ( nowTime - it->mValue.buffStartTime);
// 				if (resp.time1 <= 0)
// 				{
// 					eventList->towerBuff.erase(it);
// 					resp.buffID2 = 0;
// 					resp.time2 = 0;
// 				}
// 			}
// 
// 		}
// 	}
// 
// 	std::string jsonStr = resp.convertDataToJson();
// 	NetworkSystem::getSingleton().sender( connId,TOWER_BUFF_RESP,jsonStr);
// 
// }
//相同则替换，再者替换后者，没有添加
void ClimbTowerManager::setTowerBuff(Guid playerID,UInt32 id)
{

	UInt32 isExit = 0;
	UInt32 count = 0;
	UInt32 buffID1 = 0;
	UInt32 time1 = 0;
	UInt32 buffID2 = 0;
	UInt32 time2 = 0;

	getTowerBuff(playerID,buffID1,time1,buffID2,time2);//刷新buff
	UInt32 nowTime = TimeUtil::getTimeSec();
	TowerBuffTemplate * towerBuffTemplate = gTowerBuffTable->get(id);
	if (towerBuffTemplate == NULL)
	{
		LOG_WARN("towerBuffTemplate not found!!");
		return;
	}

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}

	EventList *eventList = mTowerEvents.findRealValue(player->getPlayerGuid());

	if(towerBuffTemplate->attr == 3)
	{
		changSP(playerID,0);
	}

	for (List<TowerBuff>::Iter * it1 = eventList->towerBuff.begin();it1 != NULL;it1 = eventList->towerBuff.next(it1))
	{
		count++;
		TowerBuffTemplate * towerBuffTemplate1 = gTowerBuffTable->get(it1->mValue.buffId);
		if (towerBuffTemplate1 == NULL)
		{
			LOG_WARN("towerBuffTemplate1 not found!!");
			return;
		}
		if (towerBuffTemplate1->attr == towerBuffTemplate->attr)
		{
			isExit = 1;

			it1->mValue.buffId = id;
			it1->mValue.buffStartTime = eventList->towerData.m_HighID;
			eventList->towerBuff.erase(it1);

			TowerBuff towerBuff;
			towerBuff.buffId = id;
			towerBuff.buffStartTime = eventList->towerData.m_HighID;
			eventList->towerBuff.insertTail(towerBuff);
			break;
		}			
	}
	if(eventList->towerBuff.size() >= 2&&isExit == 0)
	{
		for (List<TowerBuff>::Iter * it1 = eventList->towerBuff.begin();it1 != NULL;it1 = eventList->towerBuff.next(it1))
		{	
			eventList->towerBuff.erase(it1);
			break;
		}
	}

	
	if(isExit == 0)
	{
		
		isExit = 1;
		TowerBuff towerBuff;
		towerBuff.buffId = id;
		towerBuff.buffStartTime = eventList->towerData.m_HighID;		
		eventList->towerBuff.insertTail(towerBuff);
	}

}
//1攻击力2无敌3体力不减4积分翻倍
void ClimbTowerManager::getTowerBuff(Guid playerID,UInt32 &id1,UInt32 &time1,UInt32 &id2,UInt32 &time2)
{
	TowerEvent towerEvent;
	UInt32 count =0;
	PlayerTowerData towerData;
	TowerBuffResp resp;
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);	
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}
	towerData = player->getPlayerTowerData();
	EventList *eventList = mTowerEvents.findRealValue(playerID);

	UInt32 nowTime = TimeUtil::getTimeSec();
	UInt32 towerHigh = eventList->towerData.m_HighID;

	for (List<TowerBuff>::Iter * it = eventList->towerBuff.begin();it != NULL;it = eventList->towerBuff.next(it))
	{
		count++;
		TowerBuffTemplate * towerBuffTemplate = gTowerBuffTable->get(it->mValue.buffId);
		if (towerBuffTemplate == NULL)
		{
			LOG_WARN("towerBuffTemplate not found!!");
			return;
		}
		if (count == 1)
		{
			resp.buffID1 = towerBuffTemplate->id;
			resp.time1 = towerBuffTemplate->keepfloor - ( towerHigh - it->mValue.buffStartTime) + 1;
			if (resp.time1 <= 0)
			{
				eventList->towerBuff.erase(it);
				resp.buffID1 = 0;
				resp.time1 = 0;
			}
		}
		else if (count == 2)
		{
			resp.buffID2 = towerBuffTemplate->id;
			resp.time2 = towerBuffTemplate->keepfloor - ( towerHigh - it->mValue.buffStartTime) + 1;
			if (resp.time2 <= 0)
			{
				eventList->towerBuff.erase(it);
				resp.buffID2 = 0;
				resp.time2 = 0;
			}
		}		
	}
	id1 = resp.buffID1;
	time1 = resp.time1;
	id2 = resp.buffID2;
	time2 = resp.time2;

}



void ClimbTowerManager::onTowerEndReq(const  ConnId& connId,TowerEndReq & req)
{

	req.convertJsonToData(req.strReceive);
	LOG_WARN("towerTypeTemplate receive!!");
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if (player == NULL)
	{
		return;
	}

	ClimbTowerManager::getSingleton().towerEnd(player,req.reqType);

}

void ClimbTowerManager::towerEnd(Player *player,UInt32 reqType)
{

	UInt32 rouletteTimes = 0;
	UInt32 contentID = 0;
	List<Goods> itemList;
	PlayerTowerData towerData;
	Goods goods;
	TowerEndResp resp;

	if (reqType == 0)
	{
		//??
	}

	if (player == NULL)
	{
		return;
	}
	EventList *eventList = mTowerEvents.findRealValue(player->getPlayerGuid());
	if (eventList == NULL)
	{
		resp.result = LynxErrno::NotFound;
		std::string jsonStr = resp.convertDataToJson();
		if (reqType != 1000)
		{
			NetworkSystem::getSingleton().sender(player->getConnId(),TOWER_END_RESP,jsonStr);
		}
		LOG_WARN("towerTypeTemplate not found 1!!");
		return;
	}

	TowerTemplate*	towerTemplate =gTowersTable->get(eventList->towerData.m_HighID);
	if (towerTemplate == NULL)
	{
		LOG_WARN("towerTypeTemplate not found 4!!");
		return;
	}

	towerData = player->getPlayerTowerData();

	UInt32 towerTypeID = towerTemplate->chapterId;

	TowerTypeTemplate *towerTypeTemplate = gTowerTypeTable->get(towerTypeID);
	if (towerTypeTemplate == NULL)
	{
		LOG_WARN("towerTypeTemplate not found!!");
		return;
	}

	UInt32 maxhigh =  ClimbTowerManager::getSingleton().getTowerHighest(eventList->towerData.m_HighID); 

	if (towerData.m_HighID == maxhigh)
	{
		towerData.m_HighID = maxhigh + 1000 - maxhigh%1000;
		player->setPlayerTowerData(towerData);
		player->getPersistManager().setDirtyBit(TOWERDATABIT);
	}
	int val = 0;
	double tmpValue = 0;
	tmpValue = (double)towerTypeTemplate->scorerate/10000;
	tmpValue =(double)eventList->towerData.m_Score *tmpValue;
	towerData.m_Score = (int)tmpValue;


	resp.score = eventList->towerData.m_Score;

	eventList->towerData.m_Score -= towerData.m_Score;

	rouletteTimes = eventList->towerData.m_Score /towerTypeTemplate->roulette;

	if (rouletteTimes > towerTypeTemplate->roulettemaxtimes)
	{
		rouletteTimes = towerTypeTemplate->roulettemaxtimes;
	}
	if (towerTypeTemplate->roulette == 0)
	{
		LOG_WARN("towerTypeTemplate not found 3!!");
		return; 
	}
	
	eventList->towerData.m_Score = eventList->towerData.m_Score - rouletteTimes * towerTypeTemplate->roulette;

	getAwardRoulettes(towerTypeTemplate->rouletteid,rouletteTimes,itemList,resp.awardShow);

	resp.awardContentList = itemList;//先赋值给轮盘在给玩家加金币
	goods.resourcestype = AWARD_BASE;//不能放到awardContentList
	goods.subtype = AWARD_BASE_COIN;
	goods.num = eventList->towerData.m_Score * towerTypeTemplate->exchange;
	itemList.insertTail(goods);

	List<Goods>tmpItemList = itemList;
	Json::Value stages;
	GiftManager::getSingleton().saveEndsAttr(player->getPlayerGuid(),itemList,resp.allAttr,stages,MiniLog106);

	tmpItemList += eventList->itemList;//已经加到玩家身上的物品返回给客户端
	GiftManager::getSingleton().combineSame(tmpItemList);
	resp.ends = tmpItemList;

	Award award;
	award.award = tmpItemList;
	resp.awards.insertTail(award);

	GiftManager::getSingleton().PlayerItemChangeResult(player->getPlayerGuid(),0,resp.ends);
	player->getPersistManager().setDirtyBit(TOWERDATABIT);

	resp.reqType = reqType;
	resp.nextScore = towerData.m_Score;
	resp.leftscore = eventList->towerData.m_Score;	
	resp.highID = towerData.m_HighID;
	resp.currentHighID = eventList->towerHight;
	resp.coin = player->getPlayerCoin();

	std::string jsonStr = resp.convertDataToJson();
	if (reqType != 1000)
	{
		NetworkSystem::getSingleton().sender(player->getConnId(),TOWER_END_RESP,jsonStr);
		//LOG_INFO("TOWER_END_RESP = %s",jsonStr.c_str());
	}
	
	ClimbTowerManager::getSingleton().removeEvent(player->getPlayerGuid());
	player->setPlayerTowerData(towerData);
	player->getPersistManager().setDirtyBit(TOWERDATABIT);

	//无限挑战日常任务打点 wwc
	player->getAchieveManager().updateDailyTaskData(DLYDOWXCHALLENGE, 1 );


	LogicSystem::getSingleton().updateSevenDayTask(player->getPlayerGuid(),SDT19,1);


}

UInt32  ClimbTowerManager::changAP(Guid playerID,int num,bool bFlag = false)
{
	UInt32 nowTime = TimeUtil::getTimeSec();
	PlayerTowerData towerData;

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);	
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return 1;
	}

	EventList *eventList = mTowerEvents.findRealValue(playerID);
	if (eventList == NULL)
	{
		return 1;
	}

	TowerTemplate*	towerTemplate = gTowersTable->get(eventList->towerData.m_HighID);

	if (towerTemplate == NULL)
	{
		return 1;
	}

	towerData = player->getPlayerTowerData();

	//顺序要正确
	eventList->towerData.m_AP += num;
	//总hp增加
	if (bFlag == true)
	{
		towerData.m_AP += num;
		player->setPlayerTowerData(towerData);
		player->getPersistManager().setDirtyBit(TOWERDATABIT);
	}

	return LynxErrno::None;

}

UInt32  ClimbTowerManager::changHP(Guid playerID,int num,bool bFlag = false)
{
	UInt32 nowTime = TimeUtil::getTimeSec();
	PlayerTowerData towerData;

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);	
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return 1;
	}

	EventList *eventList = mTowerEvents.findRealValue(playerID);
	if (eventList == NULL)
	{
		return 1;
	}

	TowerTemplate*	towerTemplate = gTowersTable->get(eventList->towerData.m_HighID);

	if (towerTemplate == NULL)
	{
		return 1;
	}

	towerData = player->getPlayerTowerData();


	UInt32 towerTypeID = towerTemplate->chapterId;

	TowerTypeTemplate *towerTypeTemplate = gTowerTypeTable->get(towerTypeID);
	if (towerTypeTemplate == NULL)
	{
		LOG_WARN("towerTypeTemplate not found!!");
		return 1;
	}


	if (num > 0)
	{
		if (bFlag == true)//只加上限不加当前血量
		{
			towerData.m_HP += num;
			player->setPlayerTowerData(towerData);
			player->getPersistManager().setDirtyBit(TOWERDATABIT);
		}
		else
		{
			eventList->towerData.m_HP += num;
		}
	}
	else
	{
		int tmp = 0;
		if (ClimbTowerManager::getSingleton().getBuffValue(playerID,TOWER_SUB_HP,tmp) == false)//不减
		{			
		}
		else
		{		
			eventList->towerData.m_HP += num;				
		}
	}


	
	if (eventList->HPStartTime > 0)
	{
		eventList->HPStartTime = nowTime;
	}
	if (eventList->towerData.m_HP >=towerData.m_HP)
	{
		eventList->towerData.m_HP = towerData.m_HP;
		eventList->HPStartTime = 0;
	}

	if (eventList->towerData.m_HP < towerData.m_HP)
	{
		eventList->HPStartTime = nowTime;
	}


	if (eventList->towerData.m_HP <= 0)
	{
		eventList->towerData.m_HP = 0;
		eventList->HPStartTime = 0;
		return LynxErrno::IsDad;
	}
	return LynxErrno::None;

}

UInt32  ClimbTowerManager::changSP(Guid playerID,int num)
{

	UInt32 nowTime = TimeUtil::getTimeSec();

	EventList *eventList = mTowerEvents.findRealValue(playerID);
	if(eventList == NULL)
	{
		return LynxErrno::None;
	}

	TowerTemplate*	towerTemplate =gTowersTable->get(eventList->towerData.m_HighID);
	if(towerTemplate == NULL)
	{
		return LynxErrno::None;
	}
	
	UInt32 towerTypeID = towerTemplate->chapterId;

	TowerTypeTemplate *towerTypeTemplate = gTowerTypeTable->get(towerTypeID);
	if(towerTypeTemplate == NULL)
	{
		return LynxErrno::None;
	}

	UInt32 reduce = towerTypeTemplate->strengthreduce;

	//顺序要正确
	if (num > 0)
	{
		eventList->towerData.m_SP += num;
	}
	int tmp = 0;
	if (ClimbTowerManager::getSingleton().getBuffValue(playerID,TOWER_SUB_SP,tmp) == true)//体力不减
	{
		if (num < 0)
		{
			eventList->towerData.m_SP += num;
		}
		if (eventList->SPStartTime > 0)
		{
			eventList->towerData.m_SP -= reduce * (nowTime - eventList->SPStartTime);
			eventList->SPStartTime = nowTime;
		}
	}	
	eventList->SPStartTime = nowTime;//重置

	if (eventList->towerData.m_SP <= 0)
	{
		eventList->towerData.m_SP = 0;
		return LynxErrno::StrengthNotEnough;
		//send over
	}

	int maxStrength = towerTypeTemplate->strength;
	if (eventList->towerData.m_SP >= maxStrength)//int 不能与uint比较
	{
		eventList->towerData.m_SP = maxStrength;
	}

	
	
	return LynxErrno::None;
}

UInt32 ClimbTowerManager::checkCanBuy(Guid playerID,UInt32 high,UInt32 shop,UInt32 which)
{
	EventList *eventList = mTowerEvents.findRealValue(playerID);
	for(List<ShopItem>::Iter* iter = eventList->buyList.begin();iter!= NULL;iter = eventList->buyList.next(iter))
	{
		if(iter->mValue.high == high && iter->mValue.shopID == shop && iter->mValue.which == which)
		{
			return LynxErrno::Found;
		}
	}
	return LynxErrno::NotFound;
}

UInt32 ClimbTowerManager::checkCanRelive(Guid playerID)
{
	UInt32 i =0;
	UInt32 cost = 0;
	Goods goods;
	PlayerTowerData towerData;
	TowerTemplate* towerTemplate;
	TowerTypeTemplate* towerTypeTemplate;
	List<Goods> itemList;
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);	
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return LynxErrno::NotFound;
	}
	changSP(player->getPlayerGuid(),0);
	EventList *eventList = mTowerEvents.findRealValue(playerID);

	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();

	for(List<UInt32>::Iter* it = globalValue.uSRLcosts.begin();it != NULL;it = globalValue.uSRLcosts.next(it))
	{				
		if (i == eventList->reliveTimes)
		{
			cost = it->mValue;
			break;
		}
		i++;
	}
	if (i >= globalValue.uSRLcosts.size())
	{
		return LynxErrno::ReliveTimesUsedUp;
	}

	if (player->getPlayerGold() >= cost)
	{
		eventList->reliveTimes ++;

		goods.resourcestype =AWARD_BASE;
		goods.subtype = AWARD_BASE_GOLD;
		goods.num = 0 - cost;
		itemList.insertTail(goods);
		GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog109);
// 		eventList->itemList.insertTail(goods);
	}
	else
	{
		return LynxErrno::RmbNotEnough;				
	}



	towerData = player->getPlayerTowerData();
	eventList->towerData.m_HP = towerData.m_HP;

	towerTemplate = gTowersTable->get(eventList->towerData.m_HighID);
	if (towerTemplate == NULL)
	{
		return LynxErrno::NotFound;
	}
	towerTypeTemplate = gTowerTypeTable->get(towerTemplate->chapterId);
	if (towerTypeTemplate == NULL)
	{
		LOG_WARN("towerTypeTemplate not found!!");
		return LynxErrno::NotFound;
	}
	int val = 0;
	double tmpValue = 0;
	tmpValue = (double)towerTypeTemplate->strengthrecover/10000;
	tmpValue =(double)towerData.m_SP *tmpValue;
	val = (int)tmpValue;

	changSP(player->getPlayerGuid(),val);

	return LynxErrno::None;
}


void ClimbTowerManager::onTowerReliveReq(const  ConnId& connId,TowerReliveReq & msg)
{
	std::string jsonStr;
	TowerReliveResp resp;

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}
	
	resp.result = ClimbTowerManager::getSingleton().checkCanRelive(player->getPlayerGuid());
	EventList *eventList = mTowerEvents.findRealValue(player->getPlayerGuid());//要放在checkCanRelive后面
	if (eventList == NULL)
	{
		LOG_WARN("eventList not found!!");
		resp.result = LynxErrno::ClienServerDataNotMatch;
		std::string sendStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,TOWER_RELIVE_RESP,sendStr);
		return;
	}

	if (resp.result != LynxErrno::None)
	{
		jsonStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,TOWER_RELIVE_RESP,jsonStr);
		return;
	}
	resp.reliveTimes = eventList->reliveTimes;
	resp.buyTimes = eventList->reliveTimes;//
	resp.gold = player->getPlayerGold();
	resp.Hp = eventList->towerData.m_HP;
	resp.Sp = eventList->towerData.m_SP;//已结更新过sp了

	jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,TOWER_RELIVE_RESP,jsonStr);

	UInt32 getIt =0;
	UInt32 eventType = 0;
	TowerEvent towerEvent;
	UInt32 road = DefaultRoad;
	if ((eventList->towerData.m_HighID - eventList->roadType) < 4 )
	{
		road = CrossRoad;
	}

	for (List<TowerEvent>::Iter * iter = eventList->events.begin(); iter!=NULL;iter = eventList->events.next(iter))
	{		
		if (iter->mValue.tID == eventList->towerData.m_HighID && iter->mValue.mID == road )
		{
			getIt =1;			
			towerEvent =iter->mValue;
			break;
		}
	}


	if (getIt == 0)
	{
		LOG_ERROR("onInTowerChoiseResp towerEventTemplate not found m_HighID = %d road = %d",eventList->towerData.m_HighID,road);
		return;
	}

	TowerEventTemplate* towerEventTemplate = gTowerEventTable->get(towerEvent.eID);

	if (towerEventTemplate != NULL)
	{	
		eventType = towerEventTemplate->type;
	}

	InTowerChoiseReq  req;
	req.type = eventType;
	req.tID = eventList->towerData.m_HighID;
	req.wID = 1;
	onInTowerChoiseReq(connId, req);

}

void ClimbTowerManager::onTowerMopUpReq(const  ConnId& connId,TowerMopUpReq & msg)
{

	UInt32 awardID = 0;
	Goods goods;
	List<Goods> itemList;
	List<Goods> tmpItemList;
	TowerMopUpResp resp;


	msg.convertJsonToData(msg.strReceive);
	UInt32 stageID = msg.type*1000 + 1;
	TowerTemplate*towerTemplate = gTowersTable->get(stageID);
	if (towerTemplate == NULL)
	{
		resp.result = LynxErrno::ClienServerDataNotMatch;
		std::string sendStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,TOWER_MOPUP_RESP,sendStr);
		return;
	}
	TowerTypeTemplate *towerTypeTemplate = gTowerTypeTable->get(towerTemplate->chapterId);
	if (towerTypeTemplate == NULL)
	{
		resp.result = LynxErrno::ClienServerDataNotMatch;
		std::string sendStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,TOWER_MOPUP_RESP,sendStr);
		return;
	}

	UInt32 maxhigh =  ClimbTowerManager::getSingleton().getTowerHighest(stageID); 

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		return;
	}

	player->ResetFireConfirmData();	

	//条件判断
	PlayerTowerData towerData = player->getPlayerTowerData();
	UInt32 chapterID = towerData.m_HighID;
	if (maxhigh > towerData.m_HighID)
	{
		resp.result = LynxErrno::NotOpen;
	}
	if (player->getPlayerGold() < towerTypeTemplate->mopupcost)
	{
		resp.result = LynxErrno::RmbNotEnough;
	}


	PlayerDailyResetData dailyResetData= player->getPlayerDailyResetData();

	if (dailyResetData.m_nClimbTowerTimes < 1)
	{
		resp.result = LynxErrno::TimesNotEnough;
	}

	if (resp.result == LynxErrno::None)
	{
		dailyResetData.m_nClimbTowerTimes --;
		player->setPlayerDailyResetData(dailyResetData);
		player->getPersistManager().setDirtyBit(DAILYRESETBIT);

		awardID = ClimbTowerManager::getSingleton().getMopUpAwardID(msg.type);

		GiftManager::getSingleton().getAwardByID(awardID,0,tmpItemList);

		for(List<Goods>::Iter *it = tmpItemList.begin();it!= NULL;it = tmpItemList.next(it))
		{
			GiftManager::getSingleton().getContentByID(it->mValue.subtype,itemList);
		}	

		GiftManager::getSingleton().combineSame(itemList);

		Award award;
		award.award = itemList;
		resp.awards.insertTail(award);

		goods.resourcestype = AWARD_BASE;
		goods.subtype = AWARD_BASE_GOLD;
		goods.num = 0 - towerTypeTemplate->mopupcost;
		itemList.insertTail(goods);


		Json::Value stages;
		GiftManager::getSingleton().saveEndsAttr(player->getPlayerGuid(),itemList,resp.allAttr,stages,MiniLog107);


		resp.ends = itemList;

		resp.times = dailyResetData.m_nClimbTowerTimes;

	}


	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,TOWER_MOPUP_RESP,jsonStr);


}

UInt32 ClimbTowerManager::getMopUpAwardID(UInt32 type)
{
	UInt32 randNum = 0;
	TowerTypeTemplate *towerTypeTemplate = gTowerTypeTable->get(type);
	if (towerTypeTemplate == NULL)
	{
		return 0;
	}


	UInt32 allRate =  towerTypeTemplate->mopupawardrate1 + towerTypeTemplate->mopupawardrate2 + towerTypeTemplate->mopupawardrate3 + towerTypeTemplate->mopupawardrate4;

	if (allRate == 0)
	{
		return 0;
	}
	randNum = rand()%allRate;

	if (randNum < towerTypeTemplate->mopupawardrate1)
	{
		return towerTypeTemplate->mopupaward1;
	}
	else if (randNum < towerTypeTemplate->mopupawardrate1+ towerTypeTemplate->mopupawardrate2)
	{
		return towerTypeTemplate->mopupaward2;
	}
	else if (randNum < towerTypeTemplate->mopupawardrate1+ towerTypeTemplate->mopupawardrate2 + towerTypeTemplate->mopupawardrate3 )
	{
		return towerTypeTemplate->mopupaward3;
	}
	else 
	{
		return towerTypeTemplate->mopupaward4;
	}
	return 0;
}

//掉线或者关服处理
//isShutDown = 1 关服
void ClimbTowerManager::updateClimTower(UInt32 isShutDown)
{
	UInt32 nowTime = TimeUtil::getTimeSec();
	TowerTypeTemplate *towerTypeTemplate;
	TowerTemplate* towerTemplate;


	for (Map<Guid,EventList>::Iter *iter = mTowerEvents.begin();iter!=NULL;iter = mTowerEvents.next(iter))
	{
		towerTemplate = gTowersTable->get(iter->mValue.towerData.m_HighID);
		if (towerTemplate == NULL)
		{
			continue;
		}
		towerTypeTemplate = gTowerTypeTable->get(towerTemplate->chapterId);
		if (towerTypeTemplate == NULL)
		{
			continue;
		}
		if (/*(nowTime - iter->mValue.startTime) > towerTypeTemplate->strength*2 ||*/isShutDown == 1)//有不减体力buff所以不能有这个
		{
			Map<Guid,EventList> towerEvents = mTowerEvents;

			for (Map<Guid,EventList>::Iter *iter1 = towerEvents.begin();iter1!=NULL;iter1 = towerEvents.next(iter1))
			{
				towerTemplate = gTowersTable->get(iter1->mValue.towerData.m_HighID);
				if (towerTemplate == NULL)
				{
					continue;
				}
				towerTypeTemplate = gTowerTypeTable->get(towerTemplate->chapterId);
				if (towerTypeTemplate == NULL)
				{
					continue;
				}
				if ((nowTime - iter1->mValue.startTime) > towerTypeTemplate->strength*2 ||isShutDown == 1)
				{

					Player *player = LogicSystem::getSingleton().getPlayerByGuid(iter1->mKey);
					if (player == NULL)
					{
						continue;
					}

					ClimbTowerManager::getSingleton().towerEnd(player,1000);
				}
			}
			break;
		}
	}
	return;
}

//flag = false 减积分如果积分小于0则不减返回错误值
UInt32 ClimbTowerManager::changeScore(Guid playerID,int num,bool bFlag)
{
	UInt32 nowTime = TimeUtil::getTimeSec();
	PlayerTowerData towerData;

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);	

	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return 1;
	}
	EventList *eventList = mTowerEvents.findRealValue(playerID);
	if (eventList == NULL)
	{
		return 1;
	}

	int val = num;//有可能num传入的是UInt
	int score = eventList->towerData.m_Score;
	ClimbTowerManager::getSingleton().getBuffValue(player->getPlayerGuid(),TOWER_ADD_SCORE,val);
	score += val;  

	
	if (bFlag == false)//减积分如果积分小于0则不减返回错误值,购买花积分
	{
		if (score < 0)
		{
			return LynxErrno::scoreNotEnough;
		}			
	}


	if (score < 0)//积分不能减成负数，最小为0.
	{
		eventList->towerData.m_Score = 0;
	}
	else
	{
		eventList->towerData.m_Score = score;
	}
	return LynxErrno::None;
}


void ClimbTowerManager::getAwardRoulettes(UInt32 itemID,UInt32 times,List<Goods> &itemList,List<Goods> &itemShowList)
{
	Goods goods;
	List<Goods> tmpList;	

	getAwardRouletteByID(itemID,0,tmpList);

	for (List<Goods>::Iter *iter = tmpList.begin();iter!=NULL;iter = tmpList.next(iter))
	{//应该是只能roll出来一个
		goods = iter->mValue;
		break;
	}

	getRouletteContentByID(goods.subtype,times,itemList,itemShowList);
	if (times != itemList.size())
	{
		LOG_WARN("getAwardRoulettes itemList num error");
	}

	if (itemShowList.size() != 8)
	{
		LOG_WARN("getAwardRoulettes itemShowList num error");
	}

}



void ClimbTowerManager::getAwardRouletteByID(UInt32 itemID,UInt32 num,List<Goods> &ItemList)
{
	UInt32 allWeight1 = 0;
	UInt32 weight1 = 0;
	UInt32 weight2 = 0;		
	UInt32 randomNum;
	UInt32 finished = 0;
	UInt32 count =0;
	List<AwardItem> AwardItems;

	for (List<AwardTemplate>::Iter *iter = ROULETTE_TABLE().mList.begin();iter!= NULL;iter = ROULETTE_TABLE().mList.next(iter))
	{
		if (iter->mValue.id == itemID)
		{
			AwardItems = iter->mValue.AwardItems;
			break;			
		}
	}

	for(List<AwardItem>::Iter *it = AwardItems.begin();it!=NULL;it = AwardItems.next(it))
	{
		if (it->mValue.bigtype == 1)
		{
			allWeight1 += it->mValue.weight;
		}

	}
	if(allWeight1 != 0)
	{		
		randomNum = rand()% allWeight1;
	}

	for(List<AwardItem>::Iter *it = AwardItems.begin();it!=NULL;it = AwardItems.next(it))
	{
		count++;
		Goods goods;
		if (it->mValue.bigtype == 1 && finished ==0)
		{
			weight1 += it->mValue.weight;
			if (randomNum < weight1)
			{
				goods.subtype = it->mValue.awardcontent;
				goods.resourcestype = 2;
				goods.num = 1;
				ItemList.insertTail(goods);
				finished =1;
			}
		}
		if (it->mValue.bigtype == 2)
		{

			weight2 = it->mValue.weight;
			if (rand()% 10000 < weight2)
			{
				goods.subtype = it->mValue.awardcontent;
				goods.resourcestype = 2;
				goods.num = 1;
				ItemList.insertTail(goods);
			}
		}
		if (it->mValue.bigtype == 3)
		{
			goods.subtype = it->mValue.awardcontent;
			goods.resourcestype = 2;
			goods.num = 1;
			ItemList.insertTail(goods);
		}

		if (it->mValue.bigtype == 4)
		{
			if (count <= num)
			{
				goods.subtype = it->mValue.awardcontent;
				goods.resourcestype = 2;
				goods.num = 1;
				ItemList.insertTail(goods);
			}
		}
	}
}




void ClimbTowerManager::getRouletteContentByID(UInt32 itemID,UInt32 times,List<Goods> &itemList,List<Goods> &itemShowList)
{
	
	UInt32 allWeight1 = 0;
	UInt32 weight1 = 0;
	UInt32 weight2 = 0;
	UInt32 randomNum =0;
	UInt32 finished = 0;
	Goods goods;
	List<Content> contentList;


	for (List<AwardContentTemplate>::Iter *iter = ROULETTECONTENT_TABLE().mContentList.begin();iter!= NULL;iter = ROULETTECONTENT_TABLE().mContentList.next(iter))
	{
		if (iter->mValue.id == itemID)
		{
			contentList = iter->mValue.ContentList;
			break;			
		}
	}

	for(List<Content>::Iter *it = contentList.begin();it!=NULL;it = contentList.next(it))
	{
		if (it->mValue.contenttype == 1)
		{
			allWeight1 += it->mValue.weight;
		}
		goods.resourcestype = it->mValue.resourcestype;
		goods.subtype = it->mValue.subtype;
		goods.num = it->mValue.num;
		itemShowList.insertTail(goods);
	}

	for (UInt32 i = 0;i<times;i++)
	{
		finished = 0;
		if (allWeight1 != 0)
		{
			randomNum = rand()% allWeight1;
		}
		
		for(List<Content>::Iter *it = contentList.begin();it!=NULL;it = contentList.next(it))
		{		
			if (it->mValue.contenttype == 1)
			{
				weight1 += it->mValue.weight;
				if (allWeight1 == 0)
				{
					break;
				}
				if (randomNum < weight1 && finished == 0)
				{
					goods.subtype = it->mValue.subtype;
					goods.resourcestype = it->mValue.resourcestype;				
					goods.num = it->mValue.num;
					itemList.insertTail(goods);
					finished = 1;
				}
			}
			if (it->mValue.contenttype == 2)
			{


				weight2 = it->mValue.weight;
				if ((rand()%10000) < weight2)
				{
					goods.subtype = it->mValue.subtype;
					goods.resourcestype = it->mValue.resourcestype;		
					goods.num = it->mValue.num;
					itemList.insertTail(goods);
				}
			}
			if (it->mValue.contenttype == 3)
			{
				goods.subtype = it->mValue.subtype;
				goods.resourcestype = it->mValue.resourcestype;		
				goods.num = it->mValue.num;
				itemList.insertTail(goods);

			}
		}
	}

	
}
