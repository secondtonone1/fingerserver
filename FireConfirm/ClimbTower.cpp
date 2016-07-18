#include "../LogicSystem.h"
#include "StringConverter.h"
#include "../PlatformLib/SerializeType.h"
#include "../PersistSystem.h"
#include "../PlatformLib/Utilex.h"
#include "../CommonLib/CopyFinishTime.h"
#include "Gift.h"
#include "ClimbTower.h"

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
	towerData = player->getPlayerTowerData();
	PlayerDailyResetData dailyResetData= player->getPlayerDailyResetData();

	if (towerData.m_HighID == 0)
	{
		TowerTypeTemplate* towerTypeTemplate = gTowerTypeTable->get(chapterID);
		if (towerTypeTemplate == NULL)
		{
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
		ClimbTowerTemplate climbTowerTemplate =  GlobalVarManager::getSingleton().getclimbtower();
		gold = player->getPlayerGold();
		if (gold < climbTowerTemplate.buycost)
		{
			resp.result = LynxErrno::RmbNotEnough;
		}
		else if ((gVipTable->get(player->getVipLevel())->climtowertimes - dailyResetData.m_nTowerBuyTimes) < 1)
		{
			resp.result = LynxErrno::VipBuyTimesNotEnough;
		}
		else
		{
			gold -= climbTowerTemplate.buycost;
			player->setPlayerGold(gold);

			dailyResetData.m_nClimbTowerTimes ++;
			dailyResetData.m_nTowerBuyTimes ++;
			player->setPlayerDailyResetData(dailyResetData);
			player->getPersistManager().setDirtyBit(DAILYRESETBIT);			
			player->getPersistManager().setDirtyBit(BASEDATABIT);

		}
	}
	player->setPlayerTowerData(towerData);

	resp.reqtype = req.reqtype;	
	resp.highID = towerData.m_HighID;
	resp.score = towerData.m_Score;
	resp.ap = towerData.m_AP;
	resp.hp = towerData.m_HP;
	resp.times = dailyResetData.m_nClimbTowerTimes;
	resp.buyTimes = gVipTable->get(player->getVipLevel())->climtowertimes - dailyResetData.m_nTowerBuyTimes;

	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,TOWER_INFO_RESP,jsonStr);


}

void ClimbTowerManager::onTowerBeginReq(const  ConnId& connId,TowerBeginReq & req)
{

	UInt32 road = 1;//初始路为1
	UInt32 stageID = 0;
	UInt32 chapterID = 0;

	PlayerTowerData towerData;
	TowerBeginResp resp;

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if (player == NULL)
	{
		return;
	}
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
	eventList.roadType = road;
	towerTemplate = gTowersTable->get(stageID);
	if (towerTemplate == NULL)
	{
		return;
	}
	TowerTypeTemplate* towerTypeTemplate = gTowerTypeTable->get(towerTemplate->chapterId);
	eventList.towerData.m_HighID = stageID;
	eventList.towerData.m_Score = towerTypeTemplate->startscore + towerData.m_Score;
	eventList.towerData.m_AP = towerData.m_AP;
	eventList.towerData.m_HP = towerData.m_HP;
	eventList.towerData.m_SP = towerTypeTemplate->strength;

	towerData.m_SP = towerTypeTemplate->strength;

	player->setPlayerTowerData(towerData);
	player->getPersistManager().setDirtyBit(TOWERDATABIT);
	ClimbTowerManager::getSingleton().setEventList(player->getPlayerGuid(),eventList);
	ClimbTowerManager::getSingleton().getNextEvents(player->getPlayerGuid(),stageID,resp.events,eventList.roadType,1000);

	resp.id = req.id;
	resp.score = towerTypeTemplate->startscore + towerData.m_Score;	
	resp.times = dailyResetData.m_nClimbTowerTimes;	
	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,TOWER_BEGIN_RESP,jsonStr);

}


void ClimbTowerManager::setEventList(Guid playerID,EventList &eventList)
{
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);	

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
		return;
	}

	//checkIsOver
	if(ClimbTowerManager::getSingleton().checkIsOver(connId,req.tID))
	{
		resp.isOver = 1;
		std::string jsonStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender(connId,IN_TOWER_CHOICE_RESP,jsonStr);
		return;
	}


	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	EventList *eventList = mTowerEvents.findRealValue(player->getPlayerGuid());

	//防止出现崩溃
	UInt32 maxhigh =  ClimbTowerManager::getSingleton().getTowerHighest(req.tID);
	if (eventList == NULL)
	{
		return;
	}
	if (maxhigh == 0 && req.tID > maxhigh && req.tID < eventList->towerData.m_HighID)
	{
		return;
	}

	ClimbTowerManager::getSingleton().changSP(player->getPlayerGuid(),0);

	UInt32 road = 1;
	eventList->roadType --;
	if (eventList->roadType <= 1)
	{
		eventList->roadType = 1;
	}
	else
	{
		road = 2;
	}

	resp.result =  ClimbTowerManager::getSingleton().addEventValues(player->getPlayerGuid(), req,resp,road);
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
		ClimbTowerManager::getSingleton().sendChoiseResp(connId, resp);
		return;
	}


	eventList->towerData.m_HighID ++;
	if (eventList->towerData.m_HighID >= maxhigh)
	{
		resp.isOver = 1;
		eventList->towerData.m_HighID = maxhigh;
	}
	

	//add to player	
	if (req.type == 7 &&req.wID == 0)
	{
		UInt32 getIt = 0;
		TowerEvent towerEvent;
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
			return;
		}

		for (UInt32 i = 0;i<towerEvent.typeValue;i++)
		{
			TowerTemplate* towerTemplate = gTowersTable->get(req.tID+i);
			if (towerTemplate != NULL)			
			{
				UInt32 addScore = towerTemplate->awardOther; 
				ClimbTowerManager::getSingleton().getBuffValue(player->getPlayerGuid(),TOWER_ADD_SCORE,addScore);
				eventList->towerData.m_Score += addScore;  

				if (playerTowerData.m_HighID < (req.tID+i))
				{
					List<Goods> goodsList;
					if ((req.tID+i) >= maxhigh)//todo =?
					{
						playerTowerData.m_HighID = maxhigh + 1000 - maxhigh%1000;
					}
					else
					{
						playerTowerData.m_HighID = req.tID+i;
					}
					GiftManager::getSingleton().getContentByID(towerTemplate->firstAwardID,goodsList);
					GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),1,goodsList);
					itemList += goodsList;
					player->setPlayerTowerData(playerTowerData);
					player->getPersistManager().setDirtyBit(TOWERDATABIT);
				}
			}			
		}
	}
	else
	{
		TowerTemplate* towerTemplate = gTowersTable->get(req.tID);
		if (towerTemplate != NULL)
		{
			UInt32 addScore = towerTemplate->awardOther; 
			ClimbTowerManager::getSingleton().getBuffValue(player->getPlayerGuid(),TOWER_ADD_SCORE,addScore);
			eventList->towerData.m_Score += addScore;  

			if (playerTowerData.m_HighID < (req.tID+1))
			{
				if ((req.tID+1) > maxhigh)
				{
					playerTowerData.m_HighID = maxhigh + 1000 - maxhigh%1000;
				}
				else
				{
					playerTowerData.m_HighID = req.tID+1;
				}

				GiftManager::getSingleton().getContentByID(towerTemplate->firstAwardID,itemList);
				GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),1,itemList);
				player->setPlayerTowerData(playerTowerData);
				player->getPersistManager().setDirtyBit(TOWERDATABIT);
			}
		}		
	}
	ClimbTowerManager::getSingleton().changSP(player->getPlayerGuid(),0);//刷新2次开始前，结束后

	//checkIsOver
	if (maxhigh <= req.tID)
	{
		resp.isOver = 1;
		eventList->towerData.m_HighID = maxhigh;
	}
	ClimbTowerManager::getSingleton().sendChoiseResp(connId, resp);


}

void ClimbTowerManager::sendChoiseResp(const  ConnId& connId,InTowerChoiseResp resp)
{
	PlayerTowerData towerData;
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if (player == NULL)
	{
		return;
	}
	EventList *eventList = mTowerEvents.findRealValue(player->getPlayerGuid());
	if(ClimbTowerManager::getSingleton().checkIsOver(connId,eventList->towerData.m_HighID))
	{
		resp.isOver = 1;
	}
	double tmpSP = 0;
	towerData = eventList->towerData;
	resp.tID = towerData.m_HighID;
	resp.Ap = towerData.m_AP;
	resp.Hp = towerData.m_HP;
	tmpSP = (double)towerData.m_SP - (TimeUtil::getTimeSec() - eventList->startTime);
	if (tmpSP < 0)
	{
		tmpSP = 0;
	}
	resp.Sp = tmpSP;
	resp.score = towerData.m_Score;
	resp.isOver = 0;
	if (resp.Sp == 0)
	{
		resp.isOver = 1;
	}
	getTowerBuff(player->getPlayerGuid(),resp.buffID1,resp.time1,resp.buffID2,resp.time2);
	
	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender(connId,IN_TOWER_CHOICE_RESP,jsonStr);

}


UInt32 ClimbTowerManager::checkIsOver(const  ConnId& connId,UInt32 tID)
{
	TowerFloorEndResp resp;

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	

	EventList *eventList = mTowerEvents.findRealValue(player->getPlayerGuid());
	if (eventList == NULL)
	{
		return 0;
	}

	//判断胜负
	UInt32 nowTime = TimeUtil::getTimeSec();

	TowerTemplate* towerTemplate = gTowersTable->get(tID);
	if (towerTemplate == NULL)
	{
		return 0;
	}

	TowerTypeTemplate* towerTypeTemplate = gTowerTypeTable->get(towerTemplate->chapterId);
	if (towerTypeTemplate == NULL)
	{
		return 0;
	}

	if ((nowTime - eventList->startTime) >= towerTypeTemplate->strength)
	{
		// 		TowerEndReq req2;
		// 		ClimbTowerManager::getSingleton().onTowerEndReq(connId ,req2);
		return 1;
	}

	resp.result = ClimbTowerManager::getSingleton().changHP(player->getPlayerGuid(),0,false);

	if (resp.result != LynxErrno::None)
	{		
		return 1;
	}

	// 	UInt32 maxhigh =  ClimbTowerManager::getSingleton().getTowerHighest(eventList->towerData.m_HighID);
	// 
	// 	if (eventList->towerData.m_HighID > maxhigh)
	// 	{
	// 		eventList->towerData.m_HighID = maxhigh;
	// 		TowerEndReq req3;
	// 		// 		req3.id = eventList->towerData.m_HighID;
	// 		req3.reqType = 2;
	// 		ClimbTowerManager::getSingleton().onTowerEndReq(connId ,req3 );
	// 		return 1;
	// 	}
	return 0;
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
	UInt32 endHight =0;
	TowerEventTemplate* towerEventTemplate;
	List<Goods> goods;	
	EventList *eventList = mTowerEvents.findRealValue(playerID);


	maxhigh =  getTowerHighest(towerHight);

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
				if (eventList->latestJumpFloor != 0 && (eventList->latestJumpFloor < 10))
				{
					continue;					
				}
				else
				{
					break;
				}
			}
			if (towerEventTemplate->type == 9)
			{
				if (eventList->latestCrossRoadFloor != 0 && (eventList->latestCrossRoadFloor < 10))
				{
					continue;				
				}
				else
				{
					break;
				}
			}
			break;
		}
		
		if (towerEvent.eID == 1||towerEvent.eID == 0)
		{
			continue;
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
		return 1;
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
	EventList *eventList = mTowerEvents.findRealValue(player->getPlayerGuid());
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
		if (towerEventTemplate == NULL)
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
// 		setTowerBuff(playerID,towerEvent.typeValue);
	}
	else if (eventType == 5 )
	{
		towerEvent.Hp = towerData.m_HP * towerEvent.typeValue/10000;	  
	}
	else if (eventType == 6 )
	{
		towerEvent.typeValue = towerData.m_SP * towerEvent.typeValue/10000;	   
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
		towerEvent.typeValue = towerData.m_SP * towerEvent.typeValue/10000;	  
	}
	else if (eventType == 12 )
	{
		towerEvent.Hp = towerData.m_HP * towerEvent.typeValue/10000;	  
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
		ClimbTowerManager::getSingleton().getNextEvents(playerID,high,towerEvent.crossEvents,CrossRoad,EventNum);
		UInt32 tmp = 0;//for test
	}
	else if (eventType == 13)
	{
		TowerEvent towerEvent100;
		towerEvent100.eID = towerEvent.typeValue;

		spiltEvent(towerEvent100);

		dealEventfollowEvent(playerID,towerEvent100,events);

		towerEvent.isRandom = 1;
		towerEvent.eID = towerEvent100.eID;
		towerEvent.typeValue = towerEvent100.typeValue;
		towerEvent.typeValue = towerEvent100.typeValue;

		TowerEventTemplate* towerEventTemplate = gTowerEventTable->get(towerEvent100.eID);
		if (towerEventTemplate != NULL)
		{
			towerEvent.type = towerEventTemplate->type ;
		}

		// 		events.insertTail(towerEvent100);
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
	int val = 0;
	UInt32 flag = 0;

	EventList *eventList = mTowerEvents.findRealValue(playerID);

	int enemyBlood =  towerEvent.Hp;
	int ownBlood = eventList->towerData.m_HP;

	TowerTemplate*	towerTemplate = gTowersTable->get(eventList->towerData.m_HighID);
	if (towerTemplate == NULL)
	{
		return LynxErrno::NotFound;
	}

	TowerTypeTemplate *towerTypeTemplate = gTowerTypeTable->get(towerTemplate->chapterId);

	UInt32 recover = towerTypeTemplate->hprecover;

// 	if (((towerEvent.Hp * towerEvent.Ap) % eventList->towerData.m_AP) >0 && ((towerEvent.Hp * towerEvent.Ap) % eventList->towerData.m_AP) < eventList->towerData.m_AP/2)
// 	{
// 		val = (towerEvent.Hp  / eventList->towerData.m_AP +1)*towerEvent.Ap;				
// 	}
// 	else
// 	{
// 		val = (towerEvent.Hp  / eventList->towerData.m_AP)*towerEvent.Ap;
// 	}



	changSP(playerID,0);
	for (UInt32 i = 0;i < 1000;i++)
	{
		val += towerEvent.Ap;
		ownBlood = ownBlood - towerEvent.Ap + recover;
		enemyBlood = enemyBlood - towerEvent.Ap;
		if (ownBlood <= 0)
		{
			towerEvent.Hp = enemyBlood;
			flag = 2;			
		}
		if (enemyBlood <= 0)
		{
			flag = 1;
		}
		int tmpSp = eventList->towerData.m_SP  - i;	
		if (tmpSp <= 0)
		{
			flag = 3;
		}
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
	changHP(playerID,val,false);
	return flag;
}
// 1遇敌 2神秘驿站 3遭遇宝箱 4随机buff 5回复生命 6回复体力
// 7跃层 8获得积分 9分岔路 10减少积分 11减少体力 12减少hp 13随机事件
UInt32 ClimbTowerManager::addEventValues(Guid playerID,InTowerChoiseReq & req,InTowerChoiseResp &resp,UInt32 road)
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
	towerData = player->getPlayerTowerData();
	EventList *eventList = mTowerEvents.findRealValue(playerID);


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
			// 			int val = 0;
			// 			if (((towerEvent.Hp * towerEvent.Ap) % eventList->towerData.m_AP) >0 && ((towerEvent.Hp * towerEvent.Ap) % eventList->towerData.m_AP) < eventList->towerData.m_AP/2)
			// 			{
			// 				val = (towerEvent.Hp  / eventList->towerData.m_AP +1)*towerEvent.Ap;				
			// 			}
			// 			else
			// 			{
			// 				val = (towerEvent.Hp  / eventList->towerData.m_AP)*towerEvent.Ap;
			// 			}
			// 
			// 			changHP(playerID,val,false);


			flag = getBattleReport(playerID,resp.battleReport,*editTowerEvent);
			if (flag == 1)
			{
				UInt32 addScore = towerEvent.score; 
				ClimbTowerManager::getSingleton().getBuffValue(player->getPlayerGuid(),TOWER_ADD_SCORE,addScore);
				eventList->towerData.m_Score += addScore;  
			}
			else if (flag == 2)
			{
				return LynxErrno::Lose;
			}
			else if (flag == 3)
			{
				return LynxErrno::TimeOut;
			}
		}
		else
		{
			if (eventList->towerData.m_Score < towerEvent.scoreCost)
			{
				return LynxErrno::scoreNotEnough;
			}
			eventList->towerData.m_Score -= towerEvent.scoreCost;
		}

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
				eventList->towerData.m_Score -= atoi(strVector[2].c_str());

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

		if (req.wID != 0)
		{
			return LynxErrno::IsBuying;
		}
		else
		{				
			return LynxErrno::None;
		}


	}
	else if (eventType == 3)
	{
		if (req.wID == 1)//打开箱子//修改过
		{
			// 			int val = 0;
			// 			if (((towerEvent.Hp * towerEvent.Ap) % eventList->towerData.m_AP) >0 && ((towerEvent.Hp * towerEvent.Ap) % eventList->towerData.m_AP) < eventList->towerData.m_AP/2)
			// 			{
			// 				val = (towerEvent.Hp  / eventList->towerData.m_AP +1)*towerEvent.Ap;				
			// 			}
			// 			else
			// 			{
			// 				val = (towerEvent.Hp  / eventList->towerData.m_AP)*towerEvent.Ap;
			// 			}
			// 			changHP(playerID,val,false);
			flag = getBattleReport(playerID,resp.battleReport,*editTowerEvent);
			if (flag == 1)
			{
				UInt32 addScore = towerEvent.score; 
				ClimbTowerManager::getSingleton().getBuffValue(player->getPlayerGuid(),TOWER_ADD_SCORE,addScore);
				eventList->towerData.m_Score += addScore;  
				dealTowerGoods(playerID,towerEvent.goods);
			}
			else if (flag == 2)
			{
				return LynxErrno::Lose;
			}
			else if (flag == 3)
			{
				return LynxErrno::TimeOut;
			}
		}
		else
		{
			

		}
	}
	else if (eventType == 4)//BUFF
	{
		setTowerBuff(playerID,towerEvent.typeValue);
	}
	else if (eventType == 5 )
	{		
		int val = towerData.m_HP * towerEvent.typeValue/10000;
		changHP(playerID,val,false);
	}
	else if (eventType == 6 )
	{		
		changSP(playerID,towerEvent.typeValue);
	}
	else if (eventType == 8 )
	{
		addTowerData(playerID,towerEvent.goods,TOWER_ADD_SCORE,towerEvent.typeValue);
	}
	else if (eventType == 10 )
	{
		changSP(playerID,towerEvent.typeValue);
	}
	else if (eventType == 11 )
	{
		addTowerData(playerID,towerEvent.goods,TOWER_SUB_SP,towerEvent.typeValue);		
	}
	else if (eventType == 12 )
	{
		int val = 0 -(towerData.m_HP * (int)towerEvent.typeValue/10000);
		changHP(playerID,val,false);
	}
	else if (eventType == 7 )
	{
		if (req.wID == 0)
		{
			UInt32 maxhigh =  getTowerHighest(eventList->towerData.m_HighID);
			eventList->towerData.m_HighID += towerEvent.typeValue;
			eventList->towerData.m_HighID --;//跃层要少加一层，后面跟着的代码判断成功会加一层
			if (eventList->towerData.m_HighID >maxhigh)
			{
				eventList->towerData.m_HighID  = maxhigh;
			}
			//todo
			//给东西
		}
		else
		{

		}
	}
	else if (eventType == 9 )
	{	
		if (req.wID == 1)
		{
			eventList->roadType = 1;
		}
		else
		{
			eventList->roadType = 4;
		}		
	}

	return LynxErrno::None;
}

void ClimbTowerManager::dealTowerGoods(Guid playerID,List<Goods> goodsList)
{
	UInt32 getIt = 0;
	Player* player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	EventList *eventList = mTowerEvents.findRealValue(playerID);

	for( List<Goods>::Iter * iter = goodsList.begin();iter!= NULL;iter = goodsList.next(iter))
	{
		if (iter->mValue.resourcestype == AWARD_BASE )
		{
			if (iter->mValue.subtype == AWARD_BASE_SCORE )
			{
				getIt = 1;
				UInt32 addScore = iter->mValue.num; 
				ClimbTowerManager::getSingleton().getBuffValue(player->getPlayerGuid(),TOWER_ADD_SCORE,addScore);
				eventList->towerData.m_Score += addScore;  
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
				//加当前血量，和玩家总属性血量
				changHP(playerID,iter->mValue.num,true);
			}
		}
		if (getIt == 0)		
		{			
			GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_FOOD,goodsList);
		}
	}
}

//dataType 1 hp 2 ap 3 sp 4 score  
//bufftype 1攻击力加成（百分比）2	无敌	3	体力不减	4	积分获取翻倍	
bool ClimbTowerManager::getBuffValue(Guid playerID,UInt32 dataType,int num)
{

	TowerEvent towerEvent;
	UInt32 getIt =0;
	EventList *eventList;
	UInt32 buffID1 = 0;
	UInt32 time1 = 0;
	UInt32 buffID2 = 0;
	UInt32 time2 = 0;

	Player* player = LogicSystem::getSingleton().getPlayerByGuid(playerID);

	getTowerBuff(playerID,buffID1,time1,buffID2,time2);
	eventList = mTowerEvents.findRealValue(playerID);

	for (List<TowerBuff>::Iter * it1 = eventList->towerBuff.begin();it1 != NULL;it1 = eventList->towerBuff.next(it1))
	{
		TowerBuffTemplate * towerBuffTemplate = gTowerBuffTable->get(it1->mValue.buffId);
		if (towerBuffTemplate == NULL)
		{
			continue;
		}
		if (towerBuffTemplate->attr == 2)
		{
			if (dataType == TOWER_SUB_AP)
			{
				return false;
			}

		}
		if (towerBuffTemplate->attr == 3)
		{
			if (dataType == TOWER_SUB_HP)
			{
				return false;
			}

		}
		if (towerBuffTemplate->attr == 4)
		{
			if (dataType == TOWER_ADD_SCORE)
			{
				num = num * towerBuffTemplate->num/1000 ;
				return true;				
			}

		}

	}
	
	return true;

}


//dataType 1 hp 2 ap 3 sp 4 score  
//bufftype 1攻击力加成（百分比）2	无敌	3	体力不减	4	积分获取翻倍	

bool ClimbTowerManager::addTowerData(Guid playerID,List<Goods> goods,UInt32 dataType,int num)
{

	TowerEvent towerEvent;
	UInt32 getIt =0;
	EventList *eventList;
	UInt32 buffID1 = 0;
	UInt32 time1 = 0;
	UInt32 buffID2 = 0;
	UInt32 time2 = 0;

	Player* player = LogicSystem::getSingleton().getPlayerByGuid(playerID);

	getTowerBuff(playerID,buffID1,time1,buffID2,time2);
	eventList = mTowerEvents.findRealValue(playerID);

	for (List<TowerBuff>::Iter * it1 = eventList->towerBuff.begin();it1 != NULL;it1 = eventList->towerBuff.next(it1))
	{
		TowerBuffTemplate * towerBuffTemplate = gTowerBuffTable->get(it1->mValue.buffId);
		if (towerBuffTemplate == NULL)
		{
			continue;
		}
		if (towerBuffTemplate->attr == 2)
		{
			if (dataType == TOWER_SUB_AP)
			{
				return false;
			}

		}
		if (towerBuffTemplate->attr == 3)
		{
			if (dataType == TOWER_SUB_HP)
			{
				return false;
			}

		}
		if (towerBuffTemplate->attr == 4)
		{
			if (dataType == TOWER_ADD_SCORE)
			{
				num += num * towerBuffTemplate->num/1000 ;
				return true;				
			}

		}

	}
	if (dataType == TOWER_SUB_AP)
	{
		PlayerTowerData towerData;
		towerData = player->getPlayerTowerData();
		towerData.m_AP += num;
		player->setPlayerTowerData(towerData);
		player->getPersistManager().setDirtyBit(TOWERDATABIT);
	}
	if (dataType == TOWER_SUB_HP)
	{
		changHP(player->getPlayerGuid(),num,false);
	}
	if (dataType == TOWER_ADD_SCORE)
	{
		UInt32 addScore = num; 
		ClimbTowerManager::getSingleton().getBuffValue(player->getPlayerGuid(),TOWER_ADD_SCORE,addScore);
		eventList->towerData.m_Score += addScore;  
	
		return true;				
	}
	GiftManager::getSingleton().addToPlayer(playerID,REFLASH_AWARD,goods);
	return true;

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

	UInt32 isBuff = 1;
	UInt32 isExit = 0;
	UInt32 count = 0;
	UInt32 getIt = 0;
	UInt32 buffID1 = 0;
	UInt32 time1 = 0;
	UInt32 buffID2 = 0;
	UInt32 time2 = 0;
	int num = 0;	
	PlayerTowerData towerData;

	getTowerBuff(playerID,buffID1,time1,buffID2,time2);//刷新buff
	UInt32 nowTime = TimeUtil::getTimeSec();
	TowerBuffTemplate * towerBuffTemplate = gTowerBuffTable->get(id);

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);

	towerData = player->getPlayerTowerData();

	EventList *eventList = mTowerEvents.findRealValue(player->getPlayerGuid());
	if(towerBuffTemplate->attr == 1)//攻击力加成
	{
		num = towerData.m_AP * towerBuffTemplate->num/10000;
		changAP(playerID,num,false);
	}
	if(towerBuffTemplate->attr == 3)//体力停止
	{
		changSP(playerID,0);
	}

	for (List<TowerBuff>::Iter * it1 = eventList->towerBuff.begin();it1 != NULL;it1 = eventList->towerBuff.next(it1))
	{
		count ++;
		TowerBuffTemplate * towerBuffTemplate1 = gTowerBuffTable->get(it1->mValue.buffId);
		if (towerBuffTemplate1->attr == towerBuffTemplate->attr)
		{
			isExit = 1;

			it1->mValue.buffId = id;
			it1->mValue.buffStartTime = eventList->towerHight;
			break;
		}

		if (count >= 2)//只有2个buff 替换
		{
			if (isExit == 0)//删除第一个，加到后面
			{
				isExit = 1;
				for (List<TowerBuff>::Iter * it1 = eventList->towerBuff.begin();it1 != NULL;it1 = eventList->towerBuff.next(it1))
				{
					eventList->towerBuff.erase(it1);					
					break;
				}

				TowerBuff towerBuff;
				towerBuff.buffId = id;
				towerBuff.buffStartTime = eventList->towerHight;
				
				eventList->towerBuff.insertTail(towerBuff);
			}
		}
		
	}
	if(isExit == 0)
	{
		isExit = 1;
		TowerBuff towerBuff;
		towerBuff.buffId = id;
		towerBuff.buffStartTime = eventList->towerHight;
		
		eventList->towerBuff.insertTail(towerBuff);
	}


// 	TowerBuffReq req1;	
// 	ClimbTowerManager::getSingleton().onTowerBuffReq(player->getConnId(),req1);			

}
//1攻击力2无敌3体力不减4积分翻倍
void ClimbTowerManager::getTowerBuff(Guid playerID,UInt32 &id1,UInt32 &time1,UInt32 &id2,UInt32 &time2)
{
	TowerEvent towerEvent;
	UInt32 count =0;
	PlayerTowerData towerData;
	TowerBuffResp resp;
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);	
	towerData = player->getPlayerTowerData();
	EventList *eventList = mTowerEvents.findRealValue(playerID);


	UInt32 nowTime = TimeUtil::getTimeSec();
	UInt32 towerHigh = eventList->towerData.m_HighID;

	

	for (List<TowerBuff>::Iter * it = eventList->towerBuff.begin();it != NULL;it = eventList->towerBuff.next(it))
	{
		count++;
		TowerBuffTemplate * towerBuffTemplate = gTowerBuffTable->get(it->mValue.buffId);

		if (towerBuffTemplate->attr == 3)
		{
			if (count == 1)
			{
				resp.buffID1 = towerBuffTemplate->id;
				resp.time1 = towerBuffTemplate->keepfloor - ( towerHigh - it->mValue.buffStartTime);
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
				resp.time2 = towerBuffTemplate->keepfloor - ( towerHigh - it->mValue.buffStartTime);
				if (resp.time1 <= 0)
				{
					eventList->towerBuff.erase(it);
					resp.buffID2 = 0;
					resp.time2 = 0;
				}
			}
		}
		else
		{
			if (count == 1)
			{
				resp.buffID1 = towerBuffTemplate->id;
				resp.time1 = towerBuffTemplate->keepfloor - ( towerHigh - it->mValue.buffStartTime);
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
				resp.time2 = towerBuffTemplate->keepfloor - ( towerHigh - it->mValue.buffStartTime);
				if (resp.time1 <= 0)
				{
					eventList->towerBuff.erase(it);
					resp.buffID2 = 0;
					resp.time2 = 0;
				}
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
		NetworkSystem::getSingleton().sender(player->getConnId(),TOWER_END_RESP,jsonStr);
		return;
	}

	TowerTemplate*	towerTemplate =gTowersTable->get(eventList->towerData.m_HighID);
	if (towerTemplate == NULL)
	{
		return;
	}

	towerData = player->getPlayerTowerData();

	UInt32 towerTypeID = towerTemplate->chapterId;

	TowerTypeTemplate *towerTypeTemplate = gTowerTypeTable->get(towerTypeID);

	UInt32 maxhigh =  ClimbTowerManager::getSingleton().getTowerHighest(eventList->towerData.m_HighID); 

	if (towerData.m_HighID == maxhigh)
	{
		towerData.m_HighID = maxhigh + 1000 - maxhigh%1000;
		player->setPlayerTowerData(towerData);
		player->getPersistManager().setDirtyBit(TOWERDATABIT);
	}

	towerData.m_Score = eventList->towerData.m_Score * 0.01;

	resp.score = eventList->towerData.m_Score;

	eventList->towerData.m_Score -= towerData.m_Score;

	rouletteTimes = eventList->towerData.m_Score /towerTypeTemplate->roulette;



	for (UInt32 i =0;i<rouletteTimes;i++)
	{
		if(i%towerTypeTemplate->roulettemaxtimes ==0)
		{
			contentID = towerTypeTemplate->rouletteid1;

		}
		if(i%towerTypeTemplate->roulettemaxtimes ==1)
		{
			contentID = towerTypeTemplate->rouletteid2;

		}
		if(i%towerTypeTemplate->roulettemaxtimes ==2)
		{
			contentID = towerTypeTemplate->rouletteid3;

		}
		if(i%towerTypeTemplate->roulettemaxtimes ==3)
		{
			contentID = towerTypeTemplate->rouletteid4;

		}
		if(i%towerTypeTemplate->roulettemaxtimes ==4)
		{
			contentID = towerTypeTemplate->rouletteid5;

		}
		GiftManager::getSingleton().getContentByID(contentID,itemList);
	}

	GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,itemList);//其他的已经加到身上了
	for(List<Goods>::Iter * it = itemList.begin();it != NULL;it =itemList.next(it))
	{
		Vector<String> strVector;
		lynxStrSplit(towerTypeTemplate->awardshow, ";", strVector, true);
		UInt32 tmp = strVector.size();
		for (UInt32 i = 0;i<strVector.size();i++)
		{
			Vector<String> strVector1;
			lynxStrSplit(strVector[i], "_", strVector1, true);
			if (strVector1.size() < 2)
			{
				continue;
			}
			if (atoi(strVector1[0].c_str()) == it->mValue.resourcestype &&atoi(strVector1[1].c_str()) == it->mValue.subtype&&atoi(strVector1[2].c_str()) == it->mValue.num)
			{
				resp.awardIndexList.insertTail(i);
				break;
			}
		}
	}
	resp.awardContentList = itemList;
	itemList += eventList->goods;
	goods.resourcestype = AWARD_BASE;//不能放到awardContentList
	goods.subtype = AWARD_BASE_COIN;
	goods.num = (eventList->towerData.m_Score % towerTypeTemplate->roulette) * towerTypeTemplate->scorerate;
	itemList.insertTail(goods);

	GiftManager::getSingleton().combineSame(itemList);
	resp.ends = itemList;

	Award award;
	award.award = itemList;
	resp.awards.insertTail(award);

	GiftManager::getSingleton().PlayerItemChangeResult(player->getPlayerGuid(),0,resp.ends);

	player->getPersistManager().setDirtyBit(TOWERDATABIT);


	resp.reqType = reqType;

	resp.nextScore = towerData.m_Score;
	resp.leftscore = eventList->towerData.m_Score % towerTypeTemplate->roulette;
	
	resp.highID = towerData.m_HighID;
	resp.currentHighID = eventList->towerData.m_HighID;
	resp.coin = player->getPlayerCoin();

	player->getAchieveManager().updateAchieveData(WXCHALLENGESUC, towerTemplate->chapterId);

	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender(player->getConnId(),TOWER_END_RESP,jsonStr);

	ClimbTowerManager::getSingleton().removeEvent(player->getPlayerGuid());

	player->setPlayerTowerData(towerData);
	player->getPersistManager().setDirtyBit(TOWERDATABIT);


	//无限挑战日常任务打点 wwc
	player->getAchieveManager().updateDailyTaskData(DLYDOWXCHALLENGE, 1 );

}

UInt32  ClimbTowerManager::changAP(Guid playerID,int num,bool bFlag = false)
{
	UInt32 nowTime = TimeUtil::getTimeSec();
	PlayerTowerData towerData;

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);	

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

	UInt32 recover = towerTypeTemplate->hprecover;

	//顺序要正确
	eventList->towerData.m_AP += num;
	//总hp增加
	if (bFlag == true)
	{
		towerData.m_AP += num;
		player->setPlayerTowerData(towerData);
		player->getPersistManager().setDirtyBit(TOWERDATABIT);
	}

// 	if (eventList->HPStartTime > 0)
// 	{
// 		eventList->towerData.m_HP += recover * (nowTime - eventList->HPStartTime);
// 		eventList->HPStartTime = nowTime;
// 	}
// 	if (eventList->towerData.m_HP >towerData.m_HP)
// 	{
// 		eventList->towerData.m_HP = towerData.m_HP;
// 		eventList->HPStartTime = 0;
// 	}
// 	if (eventList->towerData.m_HP < towerData.m_HP)
// 	{
// 		eventList->HPStartTime = nowTime;
// 	}
// 	if (eventList->towerData.m_HP <= 0)
// 	{
// 		eventList->HPStartTime = 0;
// 		return LynxErrno::IsDad;
// 	}
	return LynxErrno::None;

}

UInt32  ClimbTowerManager::changHP(Guid playerID,int num,bool bFlag = false)
{
	UInt32 nowTime = TimeUtil::getTimeSec();
	PlayerTowerData towerData;

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);	

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

	UInt32 recover = towerTypeTemplate->hprecover;

	//顺序要正确
	eventList->towerData.m_HP += num;
	//总hp增加
	if (bFlag == true)
	{
		towerData.m_HP += num;
		player->setPlayerTowerData(towerData);
		player->getPersistManager().setDirtyBit(TOWERDATABIT);
	}

	if (eventList->HPStartTime > 0)
	{
		eventList->towerData.m_HP += recover * (nowTime - eventList->HPStartTime);
		eventList->HPStartTime = nowTime;
	}
	if (eventList->towerData.m_HP >towerData.m_HP)
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
	eventList->towerData.m_SP += num;


	if (ClimbTowerManager::getSingleton().getBuffValue(playerID,TOWER_SUB_SP,0) == true)//体力不减
	{
		eventList->towerData.m_SP -= reduce * (nowTime - eventList->HPStartTime);
	}	

	if (eventList->towerData.m_SP >towerTypeTemplate->strength)
	{
		eventList->towerData.m_SP = towerTypeTemplate->strength;
	}
	if (eventList->towerData.m_SP < towerTypeTemplate->strength)
	{
	}
	if (eventList->towerData.m_SP <= 0)
	{
		return LynxErrno::StrengthNotEnough;
		//send over
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
	List<Goods> itemList;
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);	
	EventList *eventList = mTowerEvents.findRealValue(playerID);
	PlayerDailyResetData playerDailyResetData =player->getPlayerDailyResetData();
	{
		SpecialReliveTemplate specialReliveTemplate;
		specialReliveTemplate = GlobalVarManager::getSingleton().getSpecialRelive();

		// 		if (specialReliveTemplate.dayfreetimes > playerDailyResetData.m_NormalTimes )
		// 		{
		// 			playerDailyResetData.m_NormalTimes ++;
		// 		}
		// 		else
		// 		{			
		for(List<UInt32>::Iter* it = specialReliveTemplate.costs.begin();it != NULL;it = specialReliveTemplate.costs.next(it))
		{				
			if (i == eventList->reliveTimes)
			{
				cost = it->mValue;
				break;
			}
			i++;
		}
		if (i >= specialReliveTemplate.costs.size())
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
			GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,itemList);
			eventList->goods.insertTail(goods);
		}
		else
		{
			return LynxErrno::RmbNotEnough;				
		}
	}
	// 	}


	towerData = player->getPlayerTowerData();
	eventList->towerData.m_HP = towerData.m_HP;
	player->getPersistManager().setDirtyBit(BASEDATABIT);	
	return LynxErrno::None;
}


void ClimbTowerManager::onTowerReliveReq(const  ConnId& connId,TowerReliveReq & msg)
{
	std::string jsonStr;
	TowerReliveResp resp;

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	EventList *eventList = mTowerEvents.findRealValue(player->getPlayerGuid());
	resp.result = ClimbTowerManager::getSingleton().checkCanRelive(player->getPlayerGuid());

	if (resp.result != LynxErrno::None)
	{
		jsonStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,TOWER_RELIVE_RESP,jsonStr);
		return;
	}
	resp.reliveTimes = eventList->reliveTimes;
	resp.buyTimes = eventList->reliveTimes;//todo 一天递增
	resp.gold = player->getPlayerGold();
	resp.Hp = eventList->towerData.m_HP;
	resp.Sp = eventList->towerData.m_SP;

	jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,TOWER_RELIVE_RESP,jsonStr);

	UInt32 getIt =0;
	UInt32 eventType = 0;
	TowerEvent towerEvent;
	UInt32 road = 1;
	eventList->roadType --;
	if (eventList->roadType <= 1)
	{
		eventList->roadType = 1;
	}
	else
	{
		road = 2;
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
	List<Goods> itemList;
	TowerMopUpResp resp;
	msg.convertJsonToData(msg.strReceive);
	UInt32 stageID = msg.type*1000 + 1;
	TowerTemplate*towerTemplate = gTowersTable->get(stageID);
	if (towerTemplate == NULL)
	{
		return;
	}
	UInt32 maxhigh =  ClimbTowerManager::getSingleton().getTowerHighest(stageID); 

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		return;
	}
	//条件判断
	PlayerTowerData towerData = player->getPlayerTowerData();
	UInt32 chapterID = towerData.m_HighID;
	if (maxhigh > towerData.m_HighID)
	{
		resp.result = LynxErrno::NotOpen;
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

		GiftManager::getSingleton().getContentByID(awardID,itemList);

		GiftManager::getSingleton().combineSame(itemList);

		Award award;
		award.award = itemList;
		resp.awards.insertTail(award);

		GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,itemList);

		GiftManager::getSingleton().PlayerItemChangeResult(player->getPlayerGuid(),0,itemList);

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


	UInt32 allRate =  towerTypeTemplate->mopupawardrate1 + towerTypeTemplate->rouletteid2 + towerTypeTemplate->rouletteid3 + towerTypeTemplate->rouletteid4;

	if (allRate == 0)
	{
		return 0;
	}
	randNum = rand()%allRate;

	if (randNum < towerTypeTemplate->mopupawardrate1)
	{
		return towerTypeTemplate->mopupaward1;
	}
	else if (randNum < towerTypeTemplate->mopupawardrate1+ towerTypeTemplate->rouletteid2)
	{
		return towerTypeTemplate->mopupaward2;
	}
	else if (randNum < towerTypeTemplate->mopupawardrate1+ towerTypeTemplate->rouletteid2 + towerTypeTemplate->rouletteid3)
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
		if ((nowTime - iter->mValue.startTime) > towerTypeTemplate->strength*2 ||isShutDown == 1)
		{

// 			Player *player = LogicSystem::getSingleton().getPlayerByGuid(iter->mKey);
// 			if (player == NULL)
// 			{
// 				continue;
// 			}
// 
// 			ClimbTowerManager::getSingleton().towerEnd(player,2);

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

					ClimbTowerManager::getSingleton().towerEnd(player,2);
				}
			}
			break;
		}
	}
	return;
}

UInt32 ClimbTowerManager::changeScore(Guid playerID,int num)
{
	UInt32 nowTime = TimeUtil::getTimeSec();
	PlayerTowerData towerData;

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);	

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


	UInt32 addScore = num; 
	ClimbTowerManager::getSingleton().getBuffValue(player->getPlayerGuid(),TOWER_ADD_SCORE,addScore);
	eventList->towerData.m_Score += addScore;  

	
	return LynxErrno::None;

}

