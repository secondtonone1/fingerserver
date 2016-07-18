
#include "../LogicSystem.h"
#include "InlineActivity.h"
#include "../TimeManager.h"
#include "../PersistSystem.h"
#include "../ServerData.h"

using namespace Lynx;


void InlineActivityManager::onGrowFoundReq(const  ConnId& connId,GrowFoundReq & req)
{
	UInt32 gold = 0;
	InlineActivityTemplate* inlineActivityTemplate;

	GrowFoundResp resp;
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if (player == NULL)
	{
		return;
	}
	req.convertJsonToData(req.strReceive);
	resp.reqType = req.reqType;
	gold = player->getPlayerGold();

	PlayerInlineActivityData inlineActivityData = player->getInlineActivityData();
	GrowthFoundTemplate growthFoundTemplate = GlobalVarManager::getSingleton().getGrowthFound();	

	if (req.reqType == 0)
	{
	}
	else if (req.reqType == 1)
	{				

		if (gold < growthFoundTemplate.cost)
		{
			resp.result = LynxErrno::RmbNotEnough;
		}
		if (player->getVipLevel() < growthFoundTemplate.viplvNeed)
		{
			resp.result =  LynxErrno::VipLevelNotEnough;
		}
		if (resp.result == LynxErrno::None)
		{
			gold -= growthFoundTemplate.cost;

			inlineActivityData.m_GrowFoundBuyFlag = 1;

			player->setPlayerGold(gold);
			player->setInlineActivityData(inlineActivityData);
			InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),GROWTH_FOUND,inlineActivityData);


		}

	}
	else if (req.reqType == 2)
	{
		if (inlineActivityData.m_GrowFoundBuyFlag != 1)
		{
			resp.result = LynxErrno::IsLocked;
		}
		for (List<UInt32>::Iter *iter = inlineActivityData.m_GrowFoundGotList.begin();iter !=NULL;iter = inlineActivityData.m_GrowFoundGotList.next(iter))
		{
			if (iter->mValue == req.id)
			{
				resp.result =  LynxErrno::HadGot;
			}
		}
		if (resp.result == LynxErrno::None)
		{
			inlineActivityTemplate = gInlineActivityTable->get(req.id);
			if (inlineActivityTemplate == NULL)
			{
				return;
			}
			gold += inlineActivityTemplate->gold;
			player->setPlayerGold(gold);		
			inlineActivityData.m_GrowFoundGotList.insertTail(req.id);

			player->setInlineActivityData(inlineActivityData);
			InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),GROWTH_FOUND,inlineActivityData);
		}

	}
	resp.buyFlag = inlineActivityData.m_GrowFoundBuyFlag;
	resp.gotList = inlineActivityData.m_GrowFoundGotList;
	resp.vipLv = growthFoundTemplate.viplvNeed;
	resp.cost = growthFoundTemplate.cost;
	resp.gold = player->getPlayerGold();

	UInt32 getIt = 0;
	for( Map<UInt64, InlineActivityTemplate>::Iter *it = gInlineActivityTable->mMap.begin();it!= NULL;it = gInlineActivityTable->mMap.next(it))
	{
		if (it->mValue.type == GROWTH_FOUND )
		{
			UInt32 gotIt = 0;
			for (List<UInt32>::Iter *iter = inlineActivityData.m_GrowFoundGotList.begin();iter !=NULL;iter = inlineActivityData.m_GrowFoundGotList.next(iter))
			{
				if (iter->mValue == it->mKey)
				{
					gotIt = 1;
					break;
				}
			}
			if (gotIt == 0)
			{
				getIt = 1;
				break;
			}
		}
	}
	if (getIt == 0 && req.reqType == 0)
	{
		resp.reqType = 2;
	}

	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( player->getConnId(),GROWTH_FOUND_RESP,jsonStr);

}
//resp.signCount是id，inlineActivityData.m_MonthSignCount是第几天 1开始
void InlineActivityManager::onMonthSingnReq(const  ConnId& connId,MonthSingnReq & req)
{
	UInt32 month = TimeManager::getMonth(-1) + 1;
	UInt32 count = 0;
	UInt32 getIt = 0;
	UInt32 canSignCount = 0;
	InlineActivityTemplate* inlineActivityTemplate;
	MonthSingnResp resp;

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if (player == NULL)
	{
		return;
	}
	req.convertJsonToData(req.strReceive);
	resp.reqType = req.reqType;
	PlayerInlineActivityData inlineActivityData = player->getInlineActivityData();


	if (TimeManager::timeIsSameMonth(inlineActivityData.m_MonthSignTime) == false)
	{
		inlineActivityData.m_MonthSignCount = 0;
	}

	if( TimeManager::timeIsTodayDelayHours(inlineActivityData.m_MonthSignTime,0) == true&&req.reqType == 1)
	{
		resp.result = LynxErrno::HadSign;
	}
	if (inlineActivityData.m_MonthSignCount >= 28)
	{
		resp.result = LynxErrno::SignCountNotEnough;
	}


	if( TimeManager::timeIsTodayDelayHours(inlineActivityData.m_MonthSignTime,0) == true)
	{
		canSignCount = inlineActivityData.m_MonthSignCount;
	}
	else
	{
		canSignCount =  inlineActivityData.m_MonthSignCount + 1;
	}

	if (req.reqType == 1 && resp.result == LynxErrno::None)
	{			
		for( Map<UInt64, InlineActivityTemplate>::Iter *it = gInlineActivityTable->mMap.begin();it!= NULL;it = gInlineActivityTable->mMap.next(it))
		{
			if (it->mValue.type == MONTH_SIGN &&it->mValue.month == month )
			{
				count ++ ;
				if (count == canSignCount)
				{
					getIt = 1;
					inlineActivityTemplate = &it->mValue;
					break;
				}
			}
		}

		if (resp.result == LynxErrno::None && getIt == 1)
		{
			inlineActivityData.m_MonthSignTime = TimeUtil::getTimeSec();
			inlineActivityData.m_MonthSignCount++;
			InlineActivityManager::getSingleton().getgoodsList(inlineActivityTemplate->award1,inlineActivityTemplate->award2,inlineActivityTemplate->award3,resp.ends);

			if (player->getVipLevel() >= inlineActivityTemplate->viplv)
			{
				for (List<Goods>::Iter * item = resp.ends.begin();item!= NULL;item = resp.ends.next(item))
				{
					item->mValue.num *= 2;//2倍
				}
			}
			GiftManager::getSingleton().combineSame(resp.ends);
			GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_FOOD,resp.ends);
			GiftManager::getSingleton().PlayerItemChangeResult(player->getPlayerGuid(),0,resp.ends);

			player->setInlineActivityData(inlineActivityData);
			InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),MONTH_SIGN,inlineActivityData);
		}
	}

	getIt = 0;
	count = 0;
	{
		for( Map<UInt64, InlineActivityTemplate>::Iter *it = gInlineActivityTable->mMap.begin();it!= NULL;it = gInlineActivityTable->mMap.next(it))
		{
			if (it->mValue.type == MONTH_SIGN &&it->mValue.month == month)
			{
				count ++ ;
				if (count == inlineActivityData.m_MonthSignCount)//0的时候不用再赋值0了
				{
					resp.signCount = it->mValue.id;
				}
				if (count == canSignCount)
				{
					getIt = 1;
					inlineActivityTemplate = &it->mValue;
					break;
				}
			}
		}		
		if (getIt == 1)
		{
			resp.canSignCount = inlineActivityTemplate->id;
		}		
	}

	if (getIt == 0 && req.reqType == 0)
	{
		resp.reqType = 2;
	}

	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( player->getConnId(),MONTH_SIGN_RESP,jsonStr);
}

void InlineActivityManager::getgoodsList(String awardStr1,String awardStr2,String awardStr3,List<Goods> &goodsList)
{
	Goods goods;

	Vector<String> strVector1;
	lynxStrSplit(awardStr1, "_", strVector1, true);
	if (strVector1.size() == 3)
	{
		goods.resourcestype = atoi(strVector1[0].c_str());
		goods.subtype = atoi(strVector1[1].c_str());
		goods.num = atoi(strVector1[2].c_str());
		goodsList.insertTail(goods);
	}

	Vector<String> strVector2;
	lynxStrSplit(awardStr2, "_", strVector2, true);
	if (strVector2.size() == 3)
	{
		goods.resourcestype = atoi(strVector2[0].c_str());
		goods.subtype = atoi(strVector2[1].c_str());
		goods.num = atoi(strVector2[2].c_str());
		goodsList.insertTail(goods);
	}	

	Vector<String> strVector3;
	lynxStrSplit(awardStr3, "_", strVector3, true);
	if (strVector3.size() == 3)
	{
		goods.resourcestype = atoi(strVector3[0].c_str());
		goods.subtype = atoi(strVector3[1].c_str());
		goods.num = atoi(strVector3[2].c_str());
		goodsList.insertTail(goods);
	}

}

void InlineActivityManager::onOnlineWelFareReq(const  ConnId& connId,OnlineWelFareReq & req)
{
	UInt32 onlineTime = 0;
	UInt32 nowTime = TimeUtil::getTimeSec();
	InlineActivityTemplate* inlineActivityTemplate;

	OnlineWelFareResp resp;
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if (player == NULL)
	{
		return;
	}
	req.convertJsonToData(req.strReceive);
	resp.reqType = req.reqType;
	PlayerInlineActivityData inlineActivityData = player->getInlineActivityData();




	onlineTime = inlineActivityData.m_OnlineWelFareOnlineTime + nowTime - inlineActivityData.m_LoginTime;
	if (req.reqType == 1)
	{
		inlineActivityTemplate = gInlineActivityTable->get(req.id);
		if (inlineActivityTemplate == NULL)
		{
			resp.result = LynxErrno::NotFound;
		}
		if (inlineActivityTemplate->type != ONLINE_WELFARE)
		{
			resp.result = LynxErrno::NotFound;
		}
		if (onlineTime < inlineActivityTemplate->time)
		{
			resp.result = LynxErrno::TimeNotRight;
		}
		for (List<UInt32>::Iter *iter = inlineActivityData.m_OnlineWelFareGotList.begin();iter !=NULL;iter = inlineActivityData.m_OnlineWelFareGotList.next(iter))
		{
			if (iter->mValue == req.id)
			{
				resp.result = LynxErrno::HadGot;
			}
		}

		if (resp.result == LynxErrno::None)
		{
			inlineActivityData.m_OnlineWelFareGotList.insertTail(req.id);
			InlineActivityManager::getSingleton().getgoodsList(inlineActivityTemplate->award1,inlineActivityTemplate->award2,inlineActivityTemplate->award3,resp.ends);

			GiftManager::getSingleton().combineSame(resp.ends);
			GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_FOOD,resp.ends);
			GiftManager::getSingleton().PlayerItemChangeResult(player->getPlayerGuid(),0,resp.ends);


			player->setInlineActivityData(inlineActivityData);
			InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),ONLINE_WELFARE,inlineActivityData);
		}
	}
	resp.onlineTime = onlineTime;

	resp.gotList = inlineActivityData.m_OnlineWelFareGotList;	


	UInt32 getIt = 0;
	for( Map<UInt64, InlineActivityTemplate>::Iter *it = gInlineActivityTable->mMap.begin();it!= NULL;it = gInlineActivityTable->mMap.next(it))
	{
		if (it->mValue.type == ONLINE_WELFARE )
		{
			UInt32 gotIt = 0;
			for (List<UInt32>::Iter *iter = inlineActivityData.m_OnlineWelFareGotList.begin();iter !=NULL;iter = inlineActivityData.m_OnlineWelFareGotList.next(iter))
			{
				if (iter->mValue == it->mKey)
				{
					gotIt = 1;
					break;
				}
			}
			if (gotIt == 0)
			{
				getIt = 1;
				break;
			}
		}
	}
	if (getIt == 0 && req.reqType == 0)
	{
		resp.reqType = 2;
	}

	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( player->getConnId(),ONLINE_WELFARE_RESP,jsonStr);

}

void InlineActivityManager::onPeopleWelfareReq(const  ConnId& connId,PeopleWelfareReq & req)
{
	UInt32 nowTime = TimeUtil::getTimeSec();
	InlineActivityTemplate* inlineActivityTemplate;

	PeopleWelfareResp resp;
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if (player == NULL)
	{
		return;
	}
	req.convertJsonToData(req.strReceive);
	resp.reqType = req.reqType;
	PlayerInlineActivityData inlineActivityData = player->getInlineActivityData();

	ServerPublicData  serverPublicData = ServerData::getSingleton().getServerPublicData();

	if (req.reqType == 1)
	{
		inlineActivityTemplate = gInlineActivityTable->get(req.id);
		if (inlineActivityTemplate == NULL)
		{
			resp.result = LynxErrno::NotFound;
		}
		if (inlineActivityTemplate->type != PEOPLE_WELFARE)
		{
			resp.result = LynxErrno::NotFound;
		}
		if (serverPublicData.gRechargeNum < inlineActivityTemplate->neednum)
		{
			resp.result = LynxErrno::RechargeNumNotEnough;
		}
		for (List<UInt32>::Iter * it = inlineActivityData.m_PeopleWelfareGotList.begin();it != NULL;it = inlineActivityData.m_PeopleWelfareGotList.next(it))
		{
			if (it->mValue == req.id)
			{
				resp.result = LynxErrno::HadGot;
			}
		}

		GrowthFoundTemplate growthFoundTemplate = GlobalVarManager::getSingleton().getGrowthFound();

		if (resp.result == LynxErrno::None)
		{
			inlineActivityData.m_PeopleWelfareGotList.insertTail(req.id);
			InlineActivityManager::getSingleton().getgoodsList(inlineActivityTemplate->award1,inlineActivityTemplate->award2,inlineActivityTemplate->award3,resp.ends);

			GiftManager::getSingleton().combineSame(resp.ends);
			GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_FOOD,resp.ends);
			GiftManager::getSingleton().PlayerItemChangeResult(player->getPlayerGuid(),0,resp.ends);


			player->setInlineActivityData(inlineActivityData);
			InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),PEOPLE_WELFARE,inlineActivityData);
		}
	}
	resp.rechargeNum = serverPublicData.gRechargeNum;

	resp.gotList = inlineActivityData.m_PeopleWelfareGotList;	

	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( player->getConnId(),PEOPLE_WELFARE_RESP,jsonStr);

}

void InlineActivityManager::onSevenLoginAwardReq(const  ConnId& connId,SevenLoginAwardReq & req)
{
	UInt32 count = 0;
	UInt32 getIt = 0;
	UInt32 flag = 0;
	UInt32 nowTime = TimeUtil::getTimeSec();
	InlineActivityTemplate* inlineActivityTemplate;

	SevenLoginAwardResp resp;
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if (player == NULL)
	{
		return;
	}
	req.convertJsonToData(req.strReceive);
	resp.reqType = req.reqType;
	PlayerInlineActivityData inlineActivityData = player->getInlineActivityData();
	resp.canSignCount = inlineActivityData.m_SevenLoginGotCount;



	if(TimeManager::timeIsTodayDelayHours(inlineActivityData.m_LastGetTime,0) == true)
	{
		flag = LynxErrno::HadGot;
	}
	if (flag == LynxErrno::None)
	{
		for( Map<UInt64, InlineActivityTemplate>::Iter *it = gInlineActivityTable->mMap.begin();it!= NULL;it = gInlineActivityTable->mMap.next(it))
		{
			if (it->mValue.type == SEVENDAY_LOGIN_AWARD)
			{
				count ++;
				if((inlineActivityData.m_SevenLoginGotCount + 1) == count )
				{
					getIt = 1;
					inlineActivityTemplate = &it->mValue;
					break;
				}			
			}
		}
		if (getIt == 1)
		{
			resp.canSignCount = inlineActivityTemplate->id;
		}
	}
	else
	{
		resp.canSignCount = inlineActivityData.m_SevenLoginGotCount;
	}

	if (req.reqType == 1)
	{
		if (inlineActivityData.m_SevenLoginGotCount == resp.canSignCount)
		{
			resp.result = LynxErrno::HadGot;
		}
		if (resp.result == LynxErrno::None)
		{
			inlineActivityData.m_SevenLoginGotCount ++;
			inlineActivityData.m_LastGetTime = nowTime;
			InlineActivityManager::getSingleton().getgoodsList(inlineActivityTemplate->award1,inlineActivityTemplate->award2,inlineActivityTemplate->award3,resp.ends);

			GiftManager::getSingleton().combineSame(resp.ends);
			GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_FOOD,resp.ends);
			GiftManager::getSingleton().PlayerItemChangeResult(player->getPlayerGuid(),0,resp.ends);


			player->setInlineActivityData(inlineActivityData);
			InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),SEVENDAY_LOGIN_AWARD,inlineActivityData);
		}
	}
	count = 0;
	getIt = 0;
	inlineActivityTemplate = NULL;
	for( Map<UInt64, InlineActivityTemplate>::Iter *it = gInlineActivityTable->mMap.begin();it!= NULL;it = gInlineActivityTable->mMap.next(it))
	{
		if (it->mValue.type == SEVENDAY_LOGIN_AWARD)
		{
			count ++;
			if((inlineActivityData.m_SevenLoginGotCount ) == count )
			{
				getIt = 1;
				inlineActivityTemplate = &it->mValue;
				break;
			}			
		}
	}
	if (getIt == 1)
	{
		resp.signCount = inlineActivityTemplate->id;
	}


	if(TimeManager::timeIsTodayDelayHours(inlineActivityData.m_LastGetTime,0) == true)
	{
		flag = LynxErrno::HadGot;
	}

	count = 0;
	getIt = 0;
	inlineActivityTemplate = NULL;
	if (flag == LynxErrno::None)
	{
		for( Map<UInt64, InlineActivityTemplate>::Iter *it = gInlineActivityTable->mMap.begin();it!= NULL;it = gInlineActivityTable->mMap.next(it))
		{
			if (it->mValue.type == SEVENDAY_LOGIN_AWARD)
			{
				count ++;
				if((inlineActivityData.m_SevenLoginGotCount + 1) == count )
				{
					getIt = 1;
					inlineActivityTemplate = &it->mValue;
					break;
				}			
			}
		}
		if (getIt == 1)
		{
			resp.canSignCount = inlineActivityTemplate->id;
		}
	}
	else
	{
		resp.canSignCount = resp.signCount;
	}
	if (req.reqType == 2)
	{
		resp.reqType = 0;
		if (resp.canSignCount != resp.signCount)
		{
			std::string jsonStr = resp.convertDataToJson();
			NetworkSystem::getSingleton().sender( player->getConnId(),SEVENDAY_LOGIN_AWARD_RESP,jsonStr);
		}
	}
	else
	{
		std::string jsonStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( player->getConnId(),SEVENDAY_LOGIN_AWARD_RESP,jsonStr);
	}

}

void InlineActivityManager::onSevenDayTaskReq(const  ConnId& connId,SevenDayTaskReq & req)
{
	UInt32 nowTime = TimeUtil::getTimeSec();
	InlineActivityTemplate* inlineActivityTemplate;

	SevenDayTaskResp resp;
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if (player == NULL)
	{
		return;
	}
	req.convertJsonToData(req.strReceive);
	resp.reqType = req.reqType;
	PlayerInlineActivityData inlineActivityData = player->getInlineActivityData();

	//for test
// 	{
// 		InlineActivityManager::getSingleton().addSevenDayTaskFinish(inlineActivityData);
// 		player->setInlineActivityData(inlineActivityData);
// 		InlineActivityManager::getSingleton().checkSevenDayTaskFinish(inlineActivityData);
// 	}

	resp.reqType = InlineActivityManager::getSingleton().checkSevenDayTaskClose(connId,req.reqType);
	if (resp.reqType != 0)
	{
		std::string jsonStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( player->getConnId(),SEVENDAY_TASK_RESP,jsonStr);
		return;
	}

	for( Map<UInt64, InlineActivityTemplate>::Iter *it = gInlineActivityTable->mMap.begin();it!= NULL;it = gInlineActivityTable->mMap.next(it))
	{
		if (it->mValue.type == SEVENDAY_TASK)
		{
			inlineActivityTemplate = &it->mValue;
			break;
		}
	}

	UInt32 day = (nowTime - inlineActivityData.m_FirstLoginTime)/inlineActivityTemplate->time;//从0开始


	UInt32 count = 0;

	inlineActivityTemplate = NULL;
	for( Map<UInt64, InlineActivityTemplate>::Iter *it = gInlineActivityTable->mMap.begin();it!= NULL;it = gInlineActivityTable->mMap.next(it))
	{
		if (it->mValue.type == SEVENDAY_TASK)
		{			
			if (count == day)
			{
				inlineActivityTemplate = &it->mValue;
				break;
			}
			count++;			
		}
	}
	if (inlineActivityTemplate == NULL)
	{
		resp.result = LynxErrno::NotFound;
	}


	if (req.reqType == 1)
	{
		inlineActivityTemplate = gInlineActivityTable->get(req.id);
		if (inlineActivityTemplate == NULL)
		{
			resp.result = LynxErrno::NotFound;
		}
		if (inlineActivityTemplate->type != SEVENDAY_TASK)
		{
			resp.result = LynxErrno::NotFound;
		}
		if (resp.result == LynxErrno::None )
		{
			UInt32 count = 0;
			for (List<KeyValue>::Iter *iter = inlineActivityData.m_SevenDayTaskGotList.begin();iter !=NULL;iter = inlineActivityData.m_SevenDayTaskGotList.next(iter))
			{
				count ++;
				if (count == (req.id%10))
				{
					if (iter->mValue.key == iter->mValue.value)
					{
						resp.result = LynxErrno::HadGot;
					}
					else
					{
						if (iter->mValue.key == 1)
						{
							InlineActivityManager::getSingleton().getgoodsList(inlineActivityTemplate->award1,"","",resp.ends);
							iter->mValue.value = 1;
						}
						else if (iter->mValue.key == 2 && iter->mValue.value == 0)
						{
							InlineActivityManager::getSingleton().getgoodsList(inlineActivityTemplate->award1,inlineActivityTemplate->award2,"",resp.ends);
							iter->mValue.value = 2;
						}
						else if (iter->mValue.key == 2 && iter->mValue.value == 1)
						{
							InlineActivityManager::getSingleton().getgoodsList("",inlineActivityTemplate->award2,"",resp.ends);
							iter->mValue.value = 2;
						}
					}
					break;					
				}
			}
			GiftManager::getSingleton().combineSame(resp.ends);
			GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_FOOD,resp.ends);
			GiftManager::getSingleton().PlayerItemChangeResult(player->getPlayerGuid(),0,resp.ends);


			player->setInlineActivityData(inlineActivityData);
			InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),SEVENDAY_TASK,inlineActivityData);
		}
	}
	else if (req.reqType == 3)
	{
		resp.result = InlineActivityManager::getSingleton().checkSevenDayCanGotIt( req.day,req.id, inlineActivityData,resp.ends);
		player->setInlineActivityData(inlineActivityData);
		InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),SEVENDAY_FINISHTASK,inlineActivityData);
		GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_FOOD,resp.ends);
		GiftManager::getSingleton().PlayerItemChangeResult(player->getPlayerGuid(),0,resp.ends);
	}
	else if (req.reqType == 7)
	{
		InlineActivityManager::getSingleton().getSevenDayAllAward(inlineActivityData,resp.ends);
		player->setInlineActivityData(inlineActivityData);
		InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),SEVENDAY_FINISHTASK,inlineActivityData);
		InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),SEVENDAY_TASK,inlineActivityData);
		GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_FOOD,resp.ends);
		GiftManager::getSingleton().PlayerItemChangeResult(player->getPlayerGuid(),0,resp.ends);
	}

	if (day > 6)
	{
		resp.reqType = 2;
	}
	resp.firstLoginTime = inlineActivityData.m_FirstLoginTime;
	resp.gotList = inlineActivityData.m_SevenDayTaskGotList;	
	resp.day = day + 1;
	resp.dayLeftTime = inlineActivityTemplate->time - (nowTime - inlineActivityData.m_FirstLoginTime)%inlineActivityTemplate->time;
	resp.finishList = inlineActivityData.m_SevenDayTaskFinishList;

	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( player->getConnId(),SEVENDAY_TASK_RESP,jsonStr);

}
UInt32 InlineActivityManager::checkSevenDayCanGotIt(UInt32 day,UInt32 id, PlayerInlineActivityData &inlineActivityData,List<Goods> &itemList)
{

	UInt32 getIt = 0;
	DailyTaskTemplate *dailyTaskTemplate = gSevenDayTrainningTable->get(id);
	if (dailyTaskTemplate == NULL)
	{
		return LynxErrno::NotFound;
	}


	for(List<KeyList>::Iter * it2 = inlineActivityData.m_SevenDayTaskFinishList.begin(); it2 != NULL; it2 = inlineActivityData.m_SevenDayTaskFinishList.next(it2))
	{
// 		if (it2->mValue.key == day)
		{
			for(List<Goods>::Iter *iter1 = it2->mValue.values.begin();iter1 != NULL;iter1 = it2->mValue.values.next(iter1))
			{
				if (iter1->mValue.resourcestype == id)
				{
					getIt = 1;
					if (iter1->mValue.num == 1)
					{
						return LynxErrno::HadGot;
					}
					if (iter1->mValue.subtype < dailyTaskTemplate->mArg)
					{
						return LynxErrno::TaskNotFinished;
					}
					iter1->mValue.num = 1;
				}
			}		
		}
	}
	if (getIt == 0)
	{
		return LynxErrno::TaskNotFinished;
	}
	Goods goods;
	Vector<String> strVector1;
	Vector<String> strVector2;
	lynxStrSplit(dailyTaskTemplate->mAward, ";", strVector1, true);
	for (UInt32 i = 0;i<strVector1.size();i++)
	{
		strVector2.clear();
		lynxStrSplit(strVector1[i], ",", strVector2, true);
		if (strVector2.size() == 3)
		{
			goods.resourcestype = atoi(strVector2[0].c_str());
			goods.subtype = atoi(strVector2[1].c_str());
			goods.num = atoi(strVector2[2].c_str());
			itemList.insertTail(goods);
		}
	}
	return LynxErrno::None;

}


void InlineActivityManager::getSevenDayAllAward( PlayerInlineActivityData &inlineActivityData,List<Goods> &itemList)
{
	UInt32 count = 0;
	UInt32 count1 = 0;
	InlineActivityTemplate* inlineActivityTemplate;
	DailyTaskTemplate *dailyTaskTemplate;

	checkSevenDayTaskFinish(inlineActivityData);
	
	for(List<KeyList>::Iter * it2 = inlineActivityData.m_SevenDayTaskFinishList.begin(); it2 != NULL; it2 = inlineActivityData.m_SevenDayTaskFinishList.next(it2))
	{
		for(List<Goods>::Iter *iter1 = it2->mValue.values.begin();iter1 != NULL;iter1 = it2->mValue.values.next(iter1))
		{
			if (iter1->mValue.num == 0)
			{
				dailyTaskTemplate = gSevenDayTrainningTable->get(iter1->mValue.resourcestype);
				if (dailyTaskTemplate != NULL)
				{
					if (iter1->mValue.subtype >= dailyTaskTemplate->mArg)
					{
						Goods goods;
						Vector<String> strVector1;
						lynxStrSplit(dailyTaskTemplate->mAward, ",", strVector1, true);
						if (strVector1.size() == 3)
						{
							goods.resourcestype = atoi(strVector1[0].c_str());
							goods.subtype = atoi(strVector1[1].c_str());
							goods.num = atoi(strVector1[2].c_str());
							itemList.insertTail(goods);
						}
						iter1->mValue.num = 1;
					}
				}										
			}				
		}			
	}

	count = 0;
	for (List<KeyValue>::Iter *iter = inlineActivityData.m_SevenDayTaskGotList.begin();iter !=NULL;iter = inlineActivityData.m_SevenDayTaskGotList.next(iter))
	{
		count ++;
		
			if (iter->mValue.key == iter->mValue.value)
			{
				continue;
			}
			else
			{
				count1 = 0;
				for( Map<UInt64, InlineActivityTemplate>::Iter *it = gInlineActivityTable->mMap.begin();it!= NULL;it = gInlineActivityTable->mMap.next(it))
				{				
					if (it->mValue.type == SEVENDAY_TASK)
					{	
						count1 ++;
						if (count == count1)
						{
							inlineActivityTemplate = &it->mValue;
							break;
						}
					}
				}
				if (inlineActivityTemplate == NULL)
				{
					continue;
				}

				if (iter->mValue.key == 1)
				{
					InlineActivityManager::getSingleton().getgoodsList(inlineActivityTemplate->award1,"","",itemList);
					iter->mValue.value = 1;
				}
				else if (iter->mValue.key == 2 && iter->mValue.value == 0)
				{
					InlineActivityManager::getSingleton().getgoodsList(inlineActivityTemplate->award1,inlineActivityTemplate->award2,"",itemList);
					iter->mValue.value = 2;
				}
				else if (iter->mValue.key == 2 && iter->mValue.value == 1)
				{
					InlineActivityManager::getSingleton().getgoodsList("",inlineActivityTemplate->award2,"",itemList);
					iter->mValue.value = 2;
				}
				else 
				{
					UInt32 tmp = 0;
				}
			}
						
		
	}
}

void InlineActivityManager::checkSevenDayTaskFinish( PlayerInlineActivityData &inlineActivityData)
{
	UInt32 count = 0;
	UInt32 count1 = 0;
	UInt32 count2 = 0;
	UInt32 finishCount = 0;
	UInt32 notFinishCount = 0;
	UInt32 taskCount = 0;
	DailyTaskTemplate *dailyTaskTemplate;

	for (List<KeyValue>::Iter *iter = inlineActivityData.m_SevenDayTaskGotList.begin();iter !=NULL;iter = inlineActivityData.m_SevenDayTaskGotList.next(iter))
	{
		count ++;
		count1 = 0;
		for(List<KeyList>::Iter * it2 = inlineActivityData.m_SevenDayTaskFinishList.begin(); it2 != NULL; it2 = inlineActivityData.m_SevenDayTaskFinishList.next(it2))
		{
			count1 ++;
			if (count == count1)
			{
				finishCount = 0;
				notFinishCount = 0;
				for(List<Goods>::Iter *iter1 = it2->mValue.values.begin();iter1 != NULL;iter1 = it2->mValue.values.next(iter1))
				{

					dailyTaskTemplate = gSevenDayTrainningTable->get(iter1->mValue.resourcestype);
					if (dailyTaskTemplate != NULL)
					{
						if (iter1->mValue.subtype >= dailyTaskTemplate->mArg)
						{
							finishCount ++;
						}
						else
						{
							notFinishCount ++;
						}
					}										

				}
				taskCount = 0;
				count2 = 0;
				for( Map<UInt64, InlineActivityTemplate>::Iter *it = gInlineActivityTable->mMap.begin();it!= NULL;it = gInlineActivityTable->mMap.next(it))
				{
					if (it->mValue.type == SEVENDAY_TASK )
					{
						count2 ++;
						if (count == count2)
						{
							if (it->mValue.taskid1 != 0)
							{
								taskCount++;
							}
							if (it->mValue.taskid2 != 0)
							{
								taskCount++;
							}
							if (it->mValue.taskid3 != 0)
							{
								taskCount++;
							}
							if (it->mValue.taskid4 != 0)
							{
								taskCount++;
							}
						}
					}
				}
				if (taskCount == finishCount)
				{
					iter->mValue.key = 2;
				}
				if (finishCount > 0 &&taskCount != finishCount)
				{
					iter->mValue.key = 1;
				}
				break;
			}		
		}	
	}
}



void InlineActivityManager::addSevenDayTaskFinish( PlayerInlineActivityData &inlineActivityData)
{
	UInt32 count = 0;
	UInt32 count1 = 0;
	UInt32 count2 = 0;
	UInt32 finishCount = 0;
	UInt32 notFinishCount = 0;
	UInt32 taskCount = 0;

	count ++;
	count1 = 0;
	for(List<KeyList>::Iter * it2 = inlineActivityData.m_SevenDayTaskFinishList.begin(); it2 != NULL; it2 = inlineActivityData.m_SevenDayTaskFinishList.next(it2))
	{
		finishCount = 0;
		notFinishCount = 0;

		it2->mValue.values.clear();
		count2 = 0;
		for( Map<UInt64, InlineActivityTemplate>::Iter *it = gInlineActivityTable->mMap.begin();it!= NULL;it = gInlineActivityTable->mMap.next(it))
		{
			if (it->mValue.type == SEVENDAY_TASK )
			{

				if (count1 == count2)
				{
					it2->mValue.key = it->mValue.id;
					if (it->mValue.taskid1 != 0)
					{
						Goods goods;
						goods.resourcestype = it->mValue.taskid1;
						goods.subtype = 5;
						it2->mValue.values.insertTail(goods);
					}
					if (it->mValue.taskid2 != 0)
					{
						Goods goods;
						goods.resourcestype = it->mValue.taskid2;
						goods.subtype = 5;
						it2->mValue.values.insertTail(goods);
					}
					if (it->mValue.taskid3 != 0)
					{
						Goods goods;
						goods.resourcestype = it->mValue.taskid3;
						goods.subtype = 5;
						it2->mValue.values.insertTail(goods);
					}
					if (it->mValue.taskid4 != 0)
					{
						Goods goods;
						goods.resourcestype = it->mValue.taskid4;
						goods.subtype = 5;
						it2->mValue.values.insertTail(goods);
					}
					break;
				}
				count2 ++;
			}
		}
		count1 ++;
	}
	

}


String InlineActivityManager::stringToUtf(const String& str)
{

	std::locale langLocale("");
	std::locale old = std::locale::global(langLocale);
	String dst = EncodingConvert::ansi2Utf8(str);
	std::locale::global(old);
	return dst;
}


UInt32 InlineActivityManager::checkSevenDayTaskClose(const  ConnId& connId,UInt32 reqType)
{
	UInt32 count = 0;
	UInt32 count1 = 0;
	UInt32 getIt = 0;
	List<Goods> ends;
	InlineActivityTemplate* inlineActivityTemplate;
	UInt32 nowTime = TimeUtil::getTimeSec();
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	PlayerInlineActivityData inlineActivityData = player->getInlineActivityData();

	for( Map<UInt64, InlineActivityTemplate>::Iter *it = gInlineActivityTable->mMap.begin();it!= NULL;it = gInlineActivityTable->mMap.next(it))
	{
		if (it->mValue.type == SEVENDAY_TASK)
		{
			inlineActivityTemplate = &it->mValue;
			break;
		}
	}
	UInt32 day = (nowTime - inlineActivityData.m_FirstLoginTime)/inlineActivityTemplate->time;//从0开始
	if (day > 6 /*|| reqType == 3*/)
	{		
		inlineActivityTemplate = NULL;
		for (List<KeyValue>::Iter *iter = inlineActivityData.m_SevenDayTaskGotList.begin();iter !=NULL;iter = inlineActivityData.m_SevenDayTaskGotList.next(iter))
		{
			count ++;
			if (iter->mValue.key == iter->mValue.value)
			{
			}
			else
			{				
				count1 = 0;
				for( Map<UInt64, InlineActivityTemplate>::Iter *it = gInlineActivityTable->mMap.begin();it!= NULL;it = gInlineActivityTable->mMap.next(it))
				{				
					if (it->mValue.type == SEVENDAY_TASK)
					{	
						count1 ++;
						if (count == count1)
						{
							inlineActivityTemplate = &it->mValue;
							break;
						}
					}
				}

				getIt = 1;
				if (iter->mValue.key == 1)
				{
					InlineActivityManager::getSingleton().getgoodsList(inlineActivityTemplate->award1,"","",ends);
					iter->mValue.value = 1;
				}
				else if (iter->mValue.key == 2 && iter->mValue.value == 0)
				{
					InlineActivityManager::getSingleton().getgoodsList(inlineActivityTemplate->award1,inlineActivityTemplate->award2,"",ends);
					iter->mValue.value = 2;
				}
				else if (iter->mValue.key == 2 && iter->mValue.value == 1)
				{
					InlineActivityManager::getSingleton().getgoodsList("",inlineActivityTemplate->award2,"",ends);
					iter->mValue.value = 2;
				}
			}
		
		}
		if(getIt == 0)
		{
			return 2;
		}

		char  des[2048] = {};
		char dest[2048] = {};
		String str1;
		String str2;
		char tmp[64] = {};
		sprintf(des,"7天任务奖励;系统;你获得的奖励有：;");
		for(List<Goods>::Iter * item = ends.begin();item!=NULL;item = ends.next(item))
		{
			sprintf(tmp,"(%d,%d,%d);",item->mValue.resourcestype,item->mValue.subtype,item->mValue.num);
			strcat(dest,tmp);
		}
		str1 = (String)des;
		str2 = (String)dest;

		str1 = stringToUtf(str1);
		//sdasd;asdsadasd;asdasdasdas;####(1,2,100);(1,1,1000):(1,2,1000)
		//发送人;标题;正文;奖励用四个####隔开，后面（1,2,100）你知道的格式		

		GMManager::getSingleton().GMEmailSend(0, player->getPlayerGuid(), str1.c_str(),str2.c_str());
		return 2;
	}

	return 0;
}


void InlineActivityManager::onTimeAwardReq(const  ConnId& connId,TimeAwardReq & req)
{
	UInt32 cost = 0;
	UInt32 nowTime = TimeUtil::getTimeSec();
	InlineActivityTemplate* inlineActivityTemplate;

	TimeAwardResp resp;
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if (player == NULL)
	{
		return;
	}
	req.convertJsonToData(req.strReceive);
	resp.reqType = req.reqType;
	PlayerInlineActivityData inlineActivityData = player->getInlineActivityData();


	if( TimeManager::timeIsTodayDelayHours(inlineActivityData.m_TimeAwardRefreshTime,0) == false)
	{
		inlineActivityData.m_TimeAwardGotList.clear();
		inlineActivityData.m_TimeAwardRefreshTime = nowTime;
	}

	if (req.reqType == 1)
	{
		inlineActivityTemplate = gInlineActivityTable->get(req.id);
		if (inlineActivityTemplate == NULL)
		{
			resp.result = LynxErrno::NotFound;
		}
		if (inlineActivityTemplate->type != TIME_AWARD)
		{
			resp.result = LynxErrno::NotFound;
		}

		if (inlineActivityTemplate->daystart*3600 > TimeUtil::getSecOfToday())
		{
			resp.result = LynxErrno::TimeNotRight;
		}
		if (inlineActivityTemplate->dayend*3600 < TimeUtil::getSecOfToday())
		{
			if (player->getPlayerGold() < inlineActivityTemplate->cost)
			{
				resp.result = LynxErrno::RmbNotEnough;
			}
			cost = inlineActivityTemplate->cost;
		}

		UInt32 count = 0;
		for (List<UInt32>::Iter *iter = inlineActivityData.m_TimeAwardGotList.begin();iter !=NULL;iter = inlineActivityData.m_TimeAwardGotList.next(iter))
		{
			if (count == (req.id%10))
			{
				if (iter->mValue == 1)
				{
					resp.result = LynxErrno::HadGot;
					break;
				}
				break;
			}
		}

		if (resp.result == LynxErrno::None)
		{
			Goods goods;
			goods.resourcestype = 1;
			goods.subtype = 2;
			goods.num = 0 - cost;
			resp.ends.insertTail(goods);
			inlineActivityData.m_TimeAwardGotList.insertTail(req.id);
			InlineActivityManager::getSingleton().getgoodsList(inlineActivityTemplate->award1,inlineActivityTemplate->award2,inlineActivityTemplate->award3,resp.ends);

			GiftManager::getSingleton().combineSame(resp.ends);
			GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_FOOD,resp.ends);
			GiftManager::getSingleton().PlayerItemChangeResult(player->getPlayerGuid(),0,resp.ends);


			player->setInlineActivityData(inlineActivityData);
			InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),TIME_AWARD,inlineActivityData);
		}
	}

	resp.gotList = inlineActivityData.m_TimeAwardGotList;	

	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( player->getConnId(),TIME_AWARD_RESP,jsonStr);

}

void InlineActivityManager::inlineActivitySave(Guid playerId,UInt32 type, PlayerInlineActivityData inlineActivityData)
{
	PersistInlineActivityReq req;

	req.playerGuid = playerId;
	req.type = type;
	req.m_GrowFoundBuyFlag = inlineActivityData.m_GrowFoundBuyFlag;
	req.m_GrowFoundGotList = inlineActivityData.m_GrowFoundGotList;

	req.m_MonthSignCount = inlineActivityData.m_MonthSignCount;
	req.m_MonthSignTime = inlineActivityData.m_MonthSignTime;
	req.m_OnlineWelFareOnlineTime = inlineActivityData.m_OnlineWelFareOnlineTime;
	req.m_OnlineWelFareGotList = inlineActivityData.m_OnlineWelFareGotList;
	req.m_PeopleWelfareRechargeNum = inlineActivityData.m_PeopleWelfareRechargeNum;
	req.m_PeopleWelfareGotList = inlineActivityData.m_PeopleWelfareGotList;
	req.m_FirstLoginTime = inlineActivityData.m_FirstLoginTime;
	req.m_LastGetTime = inlineActivityData.m_LastGetTime;
	req.m_SevenLoginGotCount = inlineActivityData.m_SevenLoginGotCount;
	req.m_SevenDayTaskGotList = inlineActivityData.m_SevenDayTaskGotList;
	req.m_SevenDayTaskFinishList = inlineActivityData.m_SevenDayTaskFinishList;
	req.m_TimeAwardRefreshTime = inlineActivityData.m_TimeAwardRefreshTime;
	req.m_LoginTime = inlineActivityData.m_LoginTime;

	PersistSystem::getSingleton().postThreadMsg(req,0);
}

void InlineActivityManager::incRechargeNum(Guid playerId)
{
	ServerPublicData  serverPublicData = ServerData::getSingleton().getServerPublicData();
	serverPublicData.gRechargeNum += 100;
	ServerData::getSingleton().setServerPublicData(serverPublicData);
}

void InlineActivityManager::checkLogin(Guid playerId)
{
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerId);	
	PlayerInlineActivityData inlineActivityData = player->getInlineActivityData();

	if (inlineActivityData.m_FirstLoginTime == 0)
	{
		inlineActivityData.m_FirstLoginTime = TimeUtil::getTimeSec();
	}

	inlineActivityData.m_LoginTime = TimeUtil::getTimeSec();

	player->setInlineActivityData(inlineActivityData);
	InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),0,inlineActivityData);	
}

void InlineActivityManager::checkLogOut(Guid playerId,UInt32 closeTime)
{
	InlineActivityTemplate* inlineActivityTemplate;
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerId);	
	PlayerInlineActivityData inlineActivityData = player->getInlineActivityData();

	for( Map<UInt64, InlineActivityTemplate>::Iter *it = gInlineActivityTable->mMap.begin();it!= NULL;it = gInlineActivityTable->mMap.next(it))
	{
		if (it->mValue.type == ONLINE_WELFARE )
		{
			inlineActivityTemplate = &it->mValue;
			break;
		}
	}

	if (player->getPlayerLeval() > inlineActivityTemplate->lv)
	{
		inlineActivityData.m_OnlineWelFareOnlineTime += closeTime - inlineActivityData.m_LoginTime;
	}


	player->setInlineActivityData(inlineActivityData);
	InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),TIME_AWARD,inlineActivityData);	
}

void InlineActivityManager::getIdByEventType(UInt64 eventType, List<UInt32>  &idList)
{
	for (Map<UInt64, DailyTaskTemplate>::Iter * iter = gSevenDayTrainningTable->mMap.begin();iter != NULL;iter = gSevenDayTrainningTable->mMap.next(iter))
	{
		if (iter->mValue.mEvent == eventType)
		{
			idList.insertTail(iter->mKey);
		}
	}
}

void InlineActivityManager::updateSevenDayTaskData(Guid playerId, UInt64 eventType,   UInt32 change)
{
	UInt32 getIt = 0;
	UInt32 count = 0;
	UInt32 nowTime = TimeUtil::getTimeSec();
	DailyTaskTemplate* dailyTaskTemplate = NULL;
	InlineActivityTemplate* inlineActivityTemplate;
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerId);	
	PlayerInlineActivityData inlineActivityData = player->getInlineActivityData();

	for( Map<UInt64, InlineActivityTemplate>::Iter *it = gInlineActivityTable->mMap.begin();it!= NULL;it = gInlineActivityTable->mMap.next(it))
	{
		if (it->mValue.type == SEVENDAY_TASK)
		{			
			inlineActivityTemplate = &it->mValue;
		}
	}
	if (inlineActivityTemplate == NULL)
	{
		return;
	}

	UInt32 day = (nowTime - inlineActivityData.m_FirstLoginTime)/inlineActivityTemplate->time;//从0开始
	if (day > 6)
	{
		return;
	}
	inlineActivityTemplate = NULL;
	for( Map<UInt64, InlineActivityTemplate>::Iter *it = gInlineActivityTable->mMap.begin();it!= NULL;it = gInlineActivityTable->mMap.next(it))
	{
		if (it->mValue.type == SEVENDAY_TASK)
		{			
			if (count == day)
			{
				inlineActivityTemplate = &it->mValue;
				break;
			}
			count++;			
		}
	}
	if (inlineActivityTemplate == NULL)
	{
		return;
	}
	List<UInt32>  doneIdList;
	List<UInt32>  idList;
	getIdByEventType( eventType, idList);
	for(List<UInt32>::Iter *iter = idList.begin();iter != NULL;iter = idList.next(iter))
	{
		if (iter->mValue == inlineActivityTemplate->taskid1)
		{
			doneIdList.insertTail(iter->mValue);
		}
		if (iter->mValue == inlineActivityTemplate->taskid2)
		{
			doneIdList.insertTail(iter->mValue);
		}
		if (iter->mValue == inlineActivityTemplate->taskid3)
		{
			doneIdList.insertTail(iter->mValue);
		}
		if (iter->mValue == inlineActivityTemplate->taskid4)
		{
			doneIdList.insertTail(iter->mValue);
		}		
	}

	for(List<UInt32>::Iter *iter = doneIdList.begin();iter != NULL;iter = doneIdList.next(iter))
	{
		getIt = 0;
		dailyTaskTemplate = gSevenDayTrainningTable->get(iter->mValue);
		if (dailyTaskTemplate == NULL)
		{
			return;
		}

		UInt32 count = 0;
		for(List<KeyList>::Iter * it2 = inlineActivityData.m_SevenDayTaskFinishList.begin(); it2 != NULL; it2 = inlineActivityData.m_SevenDayTaskFinishList.next(it2))
		{
			if (count == day)
			{
				for(List<Goods>::Iter *iter1 = it2->mValue.values.begin();iter1 != NULL;iter1 = it2->mValue.values.next(iter1))
				{
					if (iter1->mValue.resourcestype == iter->mValue)
					{
						getIt = 1;
						iter1->mValue.subtype ++;						
					}

				}
				if (getIt == 0)
				{
					Goods goods;
					goods.resourcestype = iter->mValue;
					goods.subtype = 1;
					goods.num = 0;

					it2->mValue.values.insertTail(goods);
				}		
				break;
			}
			count ++;
		}
	}
	if (doneIdList.size() > 0)
	{
		checkSevenDayTaskFinish(inlineActivityData);
		player->setInlineActivityData(inlineActivityData);
		InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),SEVENDAY_TASK,inlineActivityData);
		InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),SEVENDAY_FINISHTASK,inlineActivityData);
	}
	
}
