#include "../LogicSystem.h"
#include "StringConverter.h"
#include "../PlatformLib/SerializeType.h"
#include "../PersistSystem.h"
#include "../PlatformLib/Utilex.h"
#include "../CommonLib/CopyFinishTime.h"
#include "Gift.h"
#include "CourageChallenge.h"
#include "GlobalValue.h"
#include "RankGame.h"



#include "StringConverter.h"
#include "../PlatformLib/SerializeType.h"
#include "../PlatformLib/Utilex.h"
#include "Shop.h"


using namespace Lynx;


void CourageChallengeManager::refreshTimes(Player *player,PlayerCourageChallengeData &courageChallengeData)
{
	UInt32 maxBuyTimes = 0;
	int lastFinishTimes = 0;
	UInt32 finishTimes = 0;
	int maxCanAddTimes = 0;

	UInt32 nowTime = TimeUtil::getTimeSec();
	UInt32 dayBeiginTime =  nowTime - TimeUtil::getSecOfToday();

	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();

	UInt32 buyTimesRefreshTime = LogicSystem::getSingleton().getRefreshTime(TIME_REFRESH_ID14);
	UInt32 challangeRefreshTime1 = LogicSystem::getSingleton().getRefreshTime(TIME_REFRESH_ID15);
	UInt32 challangeRefreshTime2 = LogicSystem::getSingleton().getRefreshTime(TIME_REFRESH_ID16);

	UInt32 refreshTime1 = dayBeiginTime + challangeRefreshTime1;
	UInt32 refreshTime2 = dayBeiginTime + challangeRefreshTime2;

	for (List<VipTemplate>::Iter * iter = gVipTable->mVip.begin();iter!= NULL;iter = gVipTable->mVip.next(iter))
	{
		if (iter->mValue.id == player->getVipLevel())
		{
			maxBuyTimes =  iter->mValue.territorybuytimes; 
			break;
		}
	}


	if ((nowTime - dayBeiginTime)< buyTimesRefreshTime )
	{
		if (courageChallengeData.m_BuyTime < (dayBeiginTime - (24*3600-buyTimesRefreshTime) ))
		{
			courageChallengeData.m_BuyTime = nowTime;
			courageChallengeData.m_BuyTimes = 0;
		}		
	}
	else if((nowTime - dayBeiginTime) >= buyTimesRefreshTime)
	{				
		if (courageChallengeData.m_BuyTime < (dayBeiginTime + buyTimesRefreshTime))
		{
			courageChallengeData.m_BuyTime = nowTime;
			courageChallengeData.m_BuyTimes = 0;
		}
	}



/*
	if ((nowTime - dayBeiginTime)< challangeRefreshTime1 )
	{
		if (courageChallengeData.m_RefreshTime < (dayBeiginTime - (24*3600-challangeRefreshTime1) ))
		{
			courageChallengeData.m_RefreshTime = nowTime;
			courageChallengeData.m_RefreshTimes = 0;
		}
	}
	else if ( (nowTime - dayBeiginTime)>= (challangeRefreshTime1) &&(nowTime - dayBeiginTime) < challangeRefreshTime2)
	{
		if (courageChallengeData.m_RefreshTime < (dayBeiginTime + challangeRefreshTime1))
		{
			courageChallengeData.m_RefreshTime = nowTime;
			courageChallengeData.m_RefreshTimes = 0;
		}
	}
	else if((nowTime - dayBeiginTime) >= challangeRefreshTime2)
	{				
		if (courageChallengeData.m_RefreshTime < (dayBeiginTime + challangeRefreshTime2))
		{
			courageChallengeData.m_RefreshTime = nowTime;
			courageChallengeData.m_RefreshTimes = 0;
		}
	}
	*/

	if (nowTime < refreshTime1)
	{
		if (courageChallengeData.m_RefreshTime < (dayBeiginTime  - 24*3600 + challangeRefreshTime1))
		{
			courageChallengeData.m_RefreshTime = nowTime;
			courageChallengeData.m_RefreshTimes = 0;
		}
	}
	else if (nowTime < refreshTime2)
	{	
		if (courageChallengeData.m_RefreshTime < refreshTime1)
		{
			courageChallengeData.m_RefreshTime = nowTime;
			courageChallengeData.m_RefreshTimes = 0;
		}
	}
	else if(nowTime > refreshTime2)
	{
		if (courageChallengeData.m_RefreshTime < refreshTime2)
		{
			courageChallengeData.m_RefreshTime = nowTime;
			courageChallengeData.m_RefreshTimes = 0;
		}
	}




	lastFinishTimes = (nowTime - courageChallengeData.m_BeginTime) / (globalValue.uTERRITORYLrecovertime);


	if(lastFinishTimes < 0)//有时时间异常
	{
		finishTimes = 0;
	}
	else if (lastFinishTimes > globalValue.uTERRITORYLrecovertime)
	{
		finishTimes = globalValue.uTERRITORYLrecovertime;
	}
	else
	{
		finishTimes = lastFinishTimes;
	}

	maxCanAddTimes =(int)( globalValue.uTERRITORYLmaxtimes - courageChallengeData.m_LeftChallengeTimes);
	if (maxCanAddTimes > 0 )
	{
		if (maxCanAddTimes <= finishTimes )//次数满了
		{
			courageChallengeData.m_LeftChallengeTimes += maxCanAddTimes;
			courageChallengeData.m_BeginTime = nowTime;
		}
		else //次数没满还要倒计时
		{
			courageChallengeData.m_LeftChallengeTimes += finishTimes;
			courageChallengeData.m_BeginTime = courageChallengeData.m_BeginTime + finishTimes * (globalValue.uTERRITORYLrecovertime);			
		}			
	}
	else
	{
		courageChallengeData.m_BeginTime = nowTime;
	}

	if (courageChallengeData.m_LeftChallengeTimes >= globalValue.uTERRITORYLmaxtimes)
	{
		courageChallengeData.m_BeginTime = nowTime;
	}

}

//reqtype 0请求信息 1刷新 2购买 3激活 4未解锁
void CourageChallengeManager::onCourageChallengeReq(const  ConnId& connId,CourageChallengeReq & req)
{
	UInt32 i = 0;
	UInt32 ii = 0;	
	UInt32 cost = 0;
	UInt32 nowTime = TimeUtil::getTimeSec();
	Goods goods;
	List<Goods> itemList;
	CourageChallengeResp resp;
	PlayerCourageChallengeData courageChallengeData;	

	req.convertJsonToData(req.strReceive);
	resp.reqType = req.reqType;	
	resp.id = req.id;

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}

	player->ResetFireConfirmData();	

	courageChallengeData = player->getCourageChallengeData();
	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();

// 	if (RankGameManager::getSingleton().checkSystemOpen(player->getPlayerGuid(),SYSTEM_COURAGE) == false)
// 	{
// 		LOG_INFO("Open CourageChallenge level not enough");	
// 			
// 		resp.reqType = 4;
// 		std::string jsonStr = resp.convertDataToJson();
// 		NetworkSystem::getSingleton().sender( connId,COURAGE_CHALLENG_RESP,jsonStr);
// 		return;
// 	
// 	}


	CourageChallengeManager::getSingleton().refreshTimes(player,courageChallengeData);



	if (req.reqType == 0||req.reqType == 10)
	{
		//need do nothing
	}
	else if (req.reqType == 1)
	{
		resp.result = CourageChallengeManager::getSingleton().courageChallengeRefresh(player->getPlayerGuid(),courageChallengeData,cost,req.id);
	}
	else if (req.reqType == 2)
	{
		resp.result = CourageChallengeManager::getSingleton().courageChallengeBuy(player->getPlayerGuid(),courageChallengeData,cost,req.id);		
	}
	else if (req.reqType == 3)
	{
		resp.result = CourageChallengeManager::getSingleton().courageChallengeActivate(player->getPlayerGuid(),courageChallengeData,req.id);		
	}
	if (resp.result != LynxErrno::None)
	{
		std::string jsonStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,COURAGE_CHALLENG_RESP,jsonStr);
		return;
	}
	goods.resourcestype = AWARD_BASE;
	goods.subtype = AWARD_BASE_GOLD;
	goods.num = 0 - cost;
	if (cost != 0 )
	{
		itemList.insertTail(goods);
		GiftManager::getSingleton().combineSame( itemList);
		if(req.reqType == 1)
		{
			GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_WELFALE_MARKET,itemList,MiniLog85);
		}
		else
		{
			GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_WELFALE_MARKET,itemList,MiniLog86);
		}

		
	}

	if (courageChallengeData.monsterID1 == 0)
	{
		CourageChallengeManager::getSingleton().initCourageMonster(courageChallengeData,CourageID1);
		CourageChallengeManager::getSingleton().initCourageMonster(courageChallengeData,CourageID2);
		CourageChallengeManager::getSingleton().initCourageMonster(courageChallengeData,CourageID3);
	}

	player->setCourageChallengeData(courageChallengeData);
	player->getPersistManager().setDirtyBit(COURAGECHALLENGEDATABIT);

	resp.gold  = player->getPlayerGold();
	resp.leftChallengeTimes  = courageChallengeData.m_LeftChallengeTimes;
	resp.needTime = globalValue.uTERRITORYLrecovertime - (nowTime - courageChallengeData.m_BeginTime);
	resp.buyTimes  = courageChallengeData.m_BuyTimes;
	resp.lightOfLife  = courageChallengeData.m_LightOfLife ;
	resp.monsterID1  = courageChallengeData.monsterID1;
	resp.monsterID2  = courageChallengeData.monsterID2;
	resp.monsterID3  = courageChallengeData.monsterID3;
	resp.catCoins  = courageChallengeData.catCoins;
	 CourageChallengeManager::getSingleton().divisionCoinGrop(resp.coinGroup1,resp.coinGroup2,resp.coinGroup3,courageChallengeData.coinGroup);
// 	resp.coinGroup  = courageChallengeData.coinGroup;
	resp.refreshTimes   = courageChallengeData.m_RefreshTimes;
	resp.maxChallengeTimes   = globalValue.uTERRITORYLmaxtimes;
	resp.maxFreeRefreshTimes   = globalValue.uTERRITORYLfreerefreshtimes;
	resp.recoverTime = globalValue.uTERRITORYLrecovertime;

	resp.luckyValues1 = courageChallengeData.m_LuckyValues1;
	resp.luckyValues2 = courageChallengeData.m_LuckyValues2;
	resp.luckyValues3 = courageChallengeData.m_LuckyValues3;



	resp.nextRefreshCost = CourageChallengeManager::getSingleton().getNextRefreshCost(courageChallengeData.m_RefreshTimes);

	for (List<UInt32>::Iter *it = globalValue.uTERRITORYLbuytimess.begin();it != NULL;it = globalValue.uTERRITORYLbuytimess.next(it))
	{		
		if (ii == courageChallengeData.m_BuyTimes)
		{
			resp.nextBuyCost = it->mValue;
			break;
		}
		resp.nextBuyCost = it->mValue;
		ii++;
	}
	for(List<VipTemplate>::Iter *iter1 = gVipTable->mVip.begin();iter1 != NULL;iter1 = gVipTable->mVip.next(iter1))
	{
		if (iter1->mValue.id == player->getVipLevel())
		{
			resp.maxBuyTimes = iter1->mValue.territorybuytimes;
			break;
		}
	}

	if (req.needSend == true)
	{
		std::string jsonStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,COURAGE_CHALLENG_RESP,jsonStr);
	}
}

void CourageChallengeManager::initCourageMonster(PlayerCourageChallengeData &courageChallengeData,UInt32 id)
{

	UInt32 contentID = 0;
	UInt32 monsterID = 0;
	UInt32 awardID = 0;
	UInt32 maxLuckValues = 0;
	UInt32 maxContentID = 0;
	List<Goods> itemList;

	TerritoryTemplate * territoryTemplate = gTerritoryTable->get(id);
	if (territoryTemplate == NULL)
	{
		LOG_WARN("territoryTemplate not found!!");
		return;
	}
	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();

	for(List<KeyValue>::Iter* it = territoryTemplate->conditionMonsters.begin(); it != NULL;it = territoryTemplate->conditionMonsters.next(it))
	{
		if (it->mValue.key > maxLuckValues)
		{
			maxLuckValues = it->mValue.key;
			maxContentID = it->mValue.value;
		}
	}

	if (id == CourageID1)
	{
		for(List<KeyValue>::Iter* it = territoryTemplate->conditionMonsters.begin(); it != NULL;it = territoryTemplate->conditionMonsters.next(it))
		{
			if (  courageChallengeData.m_LuckyValues1 < it->mValue.key)
			{
				contentID = it->mValue.value;
				break;
			}
		}
		if (contentID == 0)
		{
			contentID = maxContentID;			
		}
		if (contentID == maxContentID)
		{
			courageChallengeData.m_LuckyValues1 = 0 - 100000;
		}
		getCourageContentByID(contentID,itemList);
		for(List<Goods>::Iter * contentIter =  itemList.begin();contentIter != NULL;contentIter = itemList.next(contentIter))
		{		
			monsterID  = contentIter->mValue.subtype;		
			awardID = contentIter->mValue.num;
		}
		courageChallengeData.monsterID1 = monsterID;
		courageChallengeData.contentID1 = awardID;
	}
	else if (id == CourageID2)
	{	
		
		for(List<KeyValue>::Iter* it = territoryTemplate->conditionMonsters.begin(); it != NULL;it = territoryTemplate->conditionMonsters.next(it))
		{
			if (  courageChallengeData.m_LuckyValues2 < it->mValue.key)
			{
				contentID = it->mValue.value;
				break;
			}
		}
		if (contentID == 0)
		{
			contentID = maxContentID;
		}
		if (contentID == maxContentID)
		{
			courageChallengeData.m_LuckyValues2 = 0 - 100000;
		}
		getCourageContentByID(contentID,itemList);
		for(List<Goods>::Iter * contentIter =  itemList.begin();contentIter != NULL;contentIter = itemList.next(contentIter))
		{		
			monsterID  = contentIter->mValue.subtype;		
			awardID = contentIter->mValue.num;
		}
		courageChallengeData.monsterID2 = monsterID;
		courageChallengeData.contentID2 = awardID;
	}
	else if (id == CourageID3)
	{		
		
		for(List<KeyValue>::Iter* it = territoryTemplate->conditionMonsters.begin(); it != NULL;it = territoryTemplate->conditionMonsters.next(it))
		{
			if (  courageChallengeData.m_LuckyValues3 < it->mValue.key)
			{
				contentID = it->mValue.value;
				break;
			}
		}
		if (contentID == 0)
		{
			contentID = maxContentID;
		}
		if (contentID == maxContentID)
		{
			courageChallengeData.m_LuckyValues3 = 0 - 100000;
		}
		getCourageContentByID(contentID,itemList);
		for(List<Goods>::Iter * contentIter =  itemList.begin();contentIter != NULL;contentIter = itemList.next(contentIter))
		{		
			monsterID  = contentIter->mValue.subtype;		
			awardID = contentIter->mValue.num;
		}
		courageChallengeData.monsterID3 = monsterID;
		courageChallengeData.contentID3 = awardID;
	}

}

UInt32 CourageChallengeManager::courageChallengeRefresh(Guid playerID, PlayerCourageChallengeData &courageChallengeData,UInt32 &cost,UInt32 id)
{

	UInt32 i = 0;
	UInt32 contentID = 0;
	UInt32 lastContentID = 0;
	UInt32 lastLucky = 0;
	UInt32 getIt = 0;
	int goldRefreshTimes = 0;
	UInt32 monsterID = 0;
	UInt32 lastCost = 0;
	UInt32 awardContentID = 0;
	List<Goods> itemList;
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);	
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return LynxErrno::NotFound;
	}
	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();

	TerritoryTemplate * territoryTemplate = gTerritoryTable->get(id);
	if (territoryTemplate == NULL)
	{
		LOG_WARN("territoryTemplate not found!!");
		return 0;
	}

	goldRefreshTimes = (int)courageChallengeData.m_RefreshTimes - globalValue.uTERRITORYLfreerefreshtimes + 1;
	if (goldRefreshTimes < 0)
	{
		goldRefreshTimes = 0;
	}

	for(List<UInt32>::Iter *iter = globalValue.uTERRITORYLcosts.begin();iter != NULL;iter = globalValue.uTERRITORYLcosts.next(iter))
	{
		i++;
		if (i == goldRefreshTimes)
		{
			cost = iter->mValue;
			break;
		}
		if (iter->mValue > lastCost)
		{
			lastCost = iter->mValue;
		}
	}

	if (goldRefreshTimes != 0 &&cost == 0)//刷新次数超过就走最后一个
	{
		cost = lastCost;

	}
	if (player->getPlayerGold() < cost)
	{
		return LynxErrno::RmbNotEnough;
	}



	if (id == CourageID1)
	{
		
		if (goldRefreshTimes == 0)
		{		
			courageChallengeData.m_LuckyValues1 += globalValue.uTERRITORYLfreerefresh;
			getIt = 1;
		}
		else
		{
			i = 0;
			for(List<UInt32>::Iter *iter = globalValue.uTERRITORYLrefreshs.begin();iter!= NULL;iter = globalValue.uTERRITORYLrefreshs.next(iter))
			{
				i++;
				if (i == goldRefreshTimes)
				{
					getIt = 1;
					courageChallengeData.m_LuckyValues1 += iter->mValue;
				}
				lastLucky = iter->mValue;
			}
		}
		if (getIt == 0)
		{
			courageChallengeData.m_LuckyValues1 += lastLucky;
		}

		if (courageChallengeData.m_LuckyValues1 < 0)
		{
			courageChallengeData.m_LuckyValues1 = 0;
		}

		
		for(List<KeyValue>::Iter* it = territoryTemplate->conditionMonsters.begin(); it != NULL;it = territoryTemplate->conditionMonsters.next(it))
		{
			if (courageChallengeData.m_LuckyValues1 < it->mValue.key)
			{
				contentID = it->mValue.value;
				break;
			}
		}
		for(List<KeyValue>::Iter* it = territoryTemplate->conditionMonsters.begin(); it != NULL;it = territoryTemplate->conditionMonsters.next(it))
		{		
			lastContentID = it->mValue.value;
		}
		if(contentID == 0)
		{
			contentID = lastContentID;
		}
		if (contentID == lastContentID)
		{
			courageChallengeData.m_LuckyValues1 =  0 -10000;
		}
		getCourageContentByID(contentID,itemList);
		for(List<Goods>::Iter * contentIter =  itemList.begin();contentIter != NULL;contentIter = itemList.next(contentIter))
		{		
			monsterID  = contentIter->mValue.subtype;		
			awardContentID = contentIter->mValue.num;
		}
		courageChallengeData.monsterID1 = monsterID;
		courageChallengeData.contentID1 = awardContentID;
	}
	else if (id == CourageID2)
	{
	
		if (goldRefreshTimes == 0)
		{		
			courageChallengeData.m_LuckyValues2 += globalValue.uTERRITORYLfreerefresh;
			getIt = 1;
		}
		else
		{
			i = 0;
			for(List<UInt32>::Iter *iter = globalValue.uTERRITORYLrefreshs.begin();iter!= NULL;iter = globalValue.uTERRITORYLrefreshs.next(iter))
			{
				i++;
				if (i == goldRefreshTimes)
				{
					getIt = 1;
					courageChallengeData.m_LuckyValues2 += iter->mValue;
				}
				lastLucky = iter->mValue;
			}
		}
		if (getIt == 0)
		{
			courageChallengeData.m_LuckyValues2 += lastLucky;
		}

		if (courageChallengeData.m_LuckyValues2 < 0)
		{
			courageChallengeData.m_LuckyValues2 = 0;
		}

		TerritoryTemplate * territoryTemplate = gTerritoryTable->get(id);
		if (territoryTemplate == NULL)
		{
			LOG_WARN("territoryTemplate not found!!");
			return 0;
		}

		for(List<KeyValue>::Iter* it = territoryTemplate->conditionMonsters.begin(); it != NULL;it = territoryTemplate->conditionMonsters.next(it))
		{
			if (courageChallengeData.m_LuckyValues2 < it->mValue.key)
			{
				contentID = it->mValue.value;
				break;
			}
		}
		for(List<KeyValue>::Iter* it = territoryTemplate->conditionMonsters.begin(); it != NULL;it = territoryTemplate->conditionMonsters.next(it))
		{		
			lastContentID = it->mValue.value;
		}
		if(contentID == 0)
		{
			contentID = lastContentID;
		}
		if (contentID == lastContentID)
		{
			courageChallengeData.m_LuckyValues2 =  0 - 100000;
		}
		getCourageContentByID(contentID,itemList);
		for(List<Goods>::Iter * contentIter =  itemList.begin();contentIter != NULL;contentIter = itemList.next(contentIter))
		{		
			monsterID  = contentIter->mValue.subtype;		
			awardContentID = contentIter->mValue.num;
		}
		courageChallengeData.monsterID2 = monsterID;
		courageChallengeData.contentID2 = awardContentID;
	}
	else if (id == CourageID3)
	{
		
		if (goldRefreshTimes == 0)
		{		
			courageChallengeData.m_LuckyValues3 += globalValue.uTERRITORYLfreerefresh;
			getIt = 1;
		}
		else
		{
			i = 0;
			for(List<UInt32>::Iter *iter = globalValue.uTERRITORYLrefreshs.begin();iter!= NULL;iter = globalValue.uTERRITORYLrefreshs.next(iter))
			{
				i++;
				if (i == goldRefreshTimes)
				{
					getIt = 1;
					courageChallengeData.m_LuckyValues3 += iter->mValue;
				}
				lastLucky = iter->mValue;
			}
		}
		if (getIt == 0)
		{
			courageChallengeData.m_LuckyValues3 += lastLucky;
		}

		if (courageChallengeData.m_LuckyValues3 < 0)
		{
			courageChallengeData.m_LuckyValues3 = 0;
		}


		TerritoryTemplate * territoryTemplate = gTerritoryTable->get(id);
		if (territoryTemplate == NULL)
		{
			LOG_WARN("territoryTemplate not found!!");
			return 0;
		}

		for(List<KeyValue>::Iter* it = territoryTemplate->conditionMonsters.begin(); it != NULL;it = territoryTemplate->conditionMonsters.next(it))
		{
			if (courageChallengeData.m_LuckyValues3 < it->mValue.key)
			{
				contentID = it->mValue.value;
				break;
			}
		}
		for(List<KeyValue>::Iter* it = territoryTemplate->conditionMonsters.begin(); it != NULL;it = territoryTemplate->conditionMonsters.next(it))
		{		
			lastContentID = it->mValue.value;
		}
		if(contentID == 0)
		{
			contentID = lastContentID;
		}
		if (contentID == lastContentID)
		{
			courageChallengeData.m_LuckyValues3 =  0 - 10000;
		}
		getCourageContentByID(contentID,itemList);
		for(List<Goods>::Iter * contentIter =  itemList.begin();contentIter != NULL;contentIter = itemList.next(contentIter))
		{		
			monsterID  = contentIter->mValue.subtype;		
			awardContentID = contentIter->mValue.num;
		}
		courageChallengeData.monsterID3 = monsterID;
		courageChallengeData.contentID3 = awardContentID;
	}

	courageChallengeData.m_RefreshTimes ++;
	return LynxErrno::None;

}

UInt32 CourageChallengeManager::getNextRefreshCost(UInt32 refreshTimes)
{

	UInt32 cost = 0;
	UInt32 i = 0;
	int goldRefreshTimes = 0;
	UInt32 lastCost = 0;

	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();

	goldRefreshTimes = (int)refreshTimes - globalValue.uTERRITORYLfreerefreshtimes + 1;
	if (goldRefreshTimes < 0)
	{
		goldRefreshTimes = 0;
	}

	for(List<UInt32>::Iter *iter = globalValue.uTERRITORYLcosts.begin();iter != NULL;iter = globalValue.uTERRITORYLcosts.next(iter))
	{
		i++;
		if (i == goldRefreshTimes)
		{
			cost = iter->mValue;
			break;
		}
		if (iter->mValue > lastCost)
		{
			lastCost = iter->mValue;
		}
	}

	if (goldRefreshTimes != 0 &&cost == 0)//刷新次数超过就走最后一个
	{
		cost = lastCost;
	}
	return cost;

}

UInt32 CourageChallengeManager::courageChallengeBuy(Guid playerID, PlayerCourageChallengeData &courageChallengeData,UInt32 &cost,UInt32 id)
{

	UInt32 maxBuyTimes = 0;
	UInt32 contentID = 0;
	UInt32 i = 0;
	int goldBuyTimes = 0;
	UInt32 lastCost = 0;
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);	
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return LynxErrno::NotFound;
	}
	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();

	goldBuyTimes = courageChallengeData.m_BuyTimes + 1;

	for(List<UInt32>::Iter *iter = globalValue.uTERRITORYLbuytimess.begin();iter != NULL;iter = globalValue.uTERRITORYLbuytimess.next(iter))
	{
		i++;
		if (i == goldBuyTimes)
		{
			cost = iter->mValue;
			break;
		}
		if (iter->mValue > lastCost)
		{
			lastCost = iter->mValue;
		}
	}

	for(List<VipTemplate>::Iter *iter1 = gVipTable->mVip.begin();iter1 != NULL;iter1 = gVipTable->mVip.next(iter1))
	{
		if (iter1->mValue.id == player->getVipLevel())
		{
			maxBuyTimes = iter1->mValue.territorybuytimes;
			break;
		}
	}
	if (goldBuyTimes > maxBuyTimes)
	{
		return LynxErrno::VipBuyCourageChallengeTimesNotEnough;
	}

	if (goldBuyTimes != 0 &&cost == 0)//刷新次数超过就走最后一个
	{
		cost = lastCost;

	}
	if (player->getPlayerGold() < cost)
	{
		return LynxErrno::RmbNotEnough;
	}

	UInt32 nowTime = TimeUtil::getTimeSec();
	courageChallengeData.m_BuyTimes ++;
	courageChallengeData.m_LeftChallengeTimes ++;
	courageChallengeData.m_BuyTime =nowTime;

	if (courageChallengeData.m_LeftChallengeTimes >= globalValue.uTERRITORYLmaxtimes)
	{
		courageChallengeData.m_BeginTime = nowTime;
	}

	return LynxErrno::None;
}

UInt32 CourageChallengeManager::courageChallengeActivate(Guid playerID, PlayerCourageChallengeData &courageChallengeData,UInt32 id)
{
	UInt32 getIt = 0;
	UInt32 cost = 0;
	UInt32 i = 0;
	UInt32 groupMaxCount = 0;
	int goldBuyTimes = 0;
	UInt32 lastCost = 0;
	Goods goods;
	CoinGroupSaveReq req;
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);	
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return LynxErrno::NotFound;
	}

	for(List<UInt32>::Iter *iter = courageChallengeData.coinGroup.begin(); iter != NULL;iter = courageChallengeData.coinGroup.next(iter))
	{
		if (iter->mValue == id)
		{
			return LynxErrno::IsActivated;
		}		
	}
	CoinGroupTemplate * coinGroupTemplate = gCoinGroupTable->get(id);
	if (coinGroupTemplate == NULL)
	{
		return LynxErrno::NotFound;
	}


	for(List<CatCoin>::Iter *it = courageChallengeData.catCoins.begin(); it != NULL;it = courageChallengeData.catCoins.next(it))
	{
		if (it->mValue.catCoinID == coinGroupTemplate->coin1 && coinGroupTemplate->coin1 != 0)
		{
			if (it->mValue.num >= coinGroupTemplate->num1)
			{
				getIt ++;
			}
		}
		if (it->mValue.catCoinID == coinGroupTemplate->coin2 && coinGroupTemplate->coin2 != 0)
		{
			if (it->mValue.num >= coinGroupTemplate->num2)
			{
				getIt ++;
			}			
		}	
		if (it->mValue.catCoinID == coinGroupTemplate->coin3 && coinGroupTemplate->coin3 != 0)
		{
			if (it->mValue.num >= coinGroupTemplate->num3)
			{
				getIt ++;
			}			
		}	
	}


	if (getIt == 0)
	{
		return LynxErrno::CatCoinNotEnough;
	}
	if (coinGroupTemplate->coin1 != 0)
	{
		groupMaxCount ++;
	}
	if (coinGroupTemplate->coin2 != 0)
	{
		groupMaxCount ++;
	}
	if (coinGroupTemplate->coin3 != 0)
	{
		groupMaxCount ++;
	}
	if (getIt != groupMaxCount)
	{
		return LynxErrno::CatCoinNotEnough;
	}


	for(List<CatCoin>::Iter *it = courageChallengeData.catCoins.begin(); it != NULL;it = courageChallengeData.catCoins.next(it))
	{
		if (it->mValue.catCoinID == coinGroupTemplate->coin1&& coinGroupTemplate->coin1 != 0)
		{
			goods.resourcestype = AWARD_CATCOIN;
			goods.subtype = coinGroupTemplate->coin1;
			goods.num = 0 - coinGroupTemplate->num1;
			CourageChallengeManager::getSingleton().setCatCoins(player->getPlayerGuid(),courageChallengeData,goods,true);
		}
		if (it->mValue.catCoinID == coinGroupTemplate->coin2&& coinGroupTemplate->coin2 != 0)
		{
			goods.resourcestype = AWARD_CATCOIN;
			goods.subtype = coinGroupTemplate->coin2;
			goods.num = 0 - coinGroupTemplate->num2;
			CourageChallengeManager::getSingleton().setCatCoins(player->getPlayerGuid(),courageChallengeData,goods,true);
		}	
		if (it->mValue.catCoinID == coinGroupTemplate->coin3&& coinGroupTemplate->coin3 != 0)
		{
			goods.resourcestype = AWARD_CATCOIN;
			goods.subtype = coinGroupTemplate->coin3;
			goods.num = 0 - coinGroupTemplate->num3;
			CourageChallengeManager::getSingleton().setCatCoins(player->getPlayerGuid(),courageChallengeData,goods,true);
		}	
	}

	courageChallengeData.coinGroup.insertTail(id);

	req.groupID = id;
	req.playerID = playerID;
	PersistSystem::getSingleton().postThreadMsg(req, 0);

	return LynxErrno::None;

}

void CourageChallengeManager::onCourageChallengeBeginReq(const  ConnId& connId,CourageChallengeBeginReq & req)
{

	UInt32 stageID = 0;
	UInt32 chapterID = 0;
	UInt32 nowTime = TimeUtil::getTimeSec();
	CourageChallengeReq CCreq;

	PlayerCourageChallengeData courageChallengeData;
	CourageChallengeBeginResp resp;

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if (player == NULL)
	{
		return;
	}
	
	req.convertJsonToData(req.strReceive);
	resp.id = req.id;

	CCreq.id = req.id;
	CCreq.needSend = false;
	onCourageChallengeReq(connId ,CCreq);
	courageChallengeData = player->getCourageChallengeData();

	if (courageChallengeData.m_LeftChallengeTimes < 1)
	{
		resp.result = LynxErrno::TimesNotEnough;
	}
	UInt32 flag = CourageChallengeManager::getSingleton().checkLimit(player->getPlayerGuid(),req.id);
	if(flag != LynxErrno::None)
	{
		resp.result = flag;
	}

	if (resp.result == LynxErrno::None)
	{
		courageChallengeData.m_LeftChallengeTimes --;		
	}

	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();
	
	resp.needTime = globalValue.uTERRITORYLrecovertime - (nowTime - courageChallengeData.m_BeginTime);
	if (resp.needTime == globalValue.uTERRITORYLrecovertime)//客户端不显示1小时
	{
		courageChallengeData.m_BeginTime ++;
		resp.needTime --;
	}
	resp.leftChallengeTimes = courageChallengeData.m_LeftChallengeTimes;

	player->setCourageChallengeData(courageChallengeData);
	player->getPersistManager().setDirtyBit(COURAGECHALLENGEDATABIT);

	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,COURAGE_CHALLENG_BEGIN_RESP,jsonStr);

}

UInt32 CourageChallengeManager::checkLimit(Guid playerID,UInt32 id)
{
	UInt32 allChapterStar = 0;
	UInt32 count = 0;
	PlayerCourageChallengeData courageChallengeData;


	TerritoryTemplate *territoryTemplate =  gTerritoryTable->get(id);
	if (territoryTemplate == NULL)
	{
		LOG_WARN("territoryTemplate not found!!");
		return 0;
	}

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return LynxErrno::NotFound; 
	}
	courageChallengeData = player->getCourageChallengeData();

	if (player->getPlayerLeval() < territoryTemplate->condition1)
	{
		return LynxErrno::LevelNotEnough;
	}
	for(List<UInt32>::Iter *iter = courageChallengeData.coinGroup.begin();iter != NULL;iter = courageChallengeData.coinGroup.next(iter))
	{
		CoinGroupTemplate *coinGroupTemplate = gCoinGroupTable->get(iter->mValue);
		if (coinGroupTemplate == NULL)
		{
			continue;
		}
		if (coinGroupTemplate->territoryID == (id-1))
		{
			count ++;
		}
	}

	if (count < territoryTemplate->condition2)
	{
		return LynxErrno::CoinGroupNotEnough;
	}
	return LynxErrno::None;
}



void CourageChallengeManager::onCourageChallengeEndReq(const  ConnId& connId,CourageChallengeEndReq & req)
{
	List<Goods> itemList;
	List<Goods> tmpItemList;

	CourageChallengeEndResp resp;
	PlayerCourageChallengeData courageChallengeData;


	req.convertJsonToData(req.strReceive);
	resp.doType = req.doType;
	resp.id = req.id;

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if (player == NULL)
	{
		return;
	}
	courageChallengeData = player->getCourageChallengeData();

	UInt32 reqResult = req.doType;
	if (reqResult == 1)
	{
		reqResult = 0;
	}
	//判断是否作弊
	UInt32 checkRet =  FireConfirmManager::getSingleton().checkIsCheat(player->getPlayerGuid(), req.fireConfirmData,reqResult);	
	if(checkRet != LynxErrno::None)
	{
		FireConfirmManager::getSingleton().UpdataConfirmLevel(connId,true);
		resp.result = checkRet;
		std::string jsonStr = resp.convertDataToJson();	
		NetworkSystem::getSingleton().sender( connId,COURAGE_CHALLENG_END_RESP,jsonStr);
		return;
	}	

	if( req.doType !=0 && req.doType !=1)
	{
		FireConfirmManager::getSingleton().UpdataConfirmLevel(connId,true);
		resp.result = req.doType;
		std::string jsonStr = resp.convertDataToJson();	
		NetworkSystem::getSingleton().sender( connId,COURAGE_CHALLENG_END_RESP,jsonStr);
		return;
	}	


	if (req.doType == 1)
	{
		if (req.id == CourageID1)
		{
			GiftManager::getSingleton().getAwardByID(courageChallengeData.contentID1,0,tmpItemList);
		}
		else if (req.id == CourageID2)
		{
			GiftManager::getSingleton().getAwardByID(courageChallengeData.contentID2,0,tmpItemList);
		}
		else if (req.id == CourageID3)
		{
			GiftManager::getSingleton().getAwardByID(courageChallengeData.contentID3,0,tmpItemList);
		}
	}

	for(List<Goods>::Iter *it = tmpItemList.begin();it!= NULL;it = tmpItemList.next(it))
	{
		GiftManager::getSingleton().getContentByID(it->mValue.subtype,itemList);
	}

	courageChallengeData = player->getCourageChallengeData();//顺序在前一点
	UInt32 flag = CourageChallengeManager::getSingleton().checkLimit(player->getPlayerGuid(),req.id);
	if(flag != LynxErrno::None)
	{
		resp.result = flag;
	}

	if (resp.result == LynxErrno::None)
	{
		GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();

		if (req.id == CourageID1 )
		{
			courageChallengeData.m_LuckyValues1 += globalValue.uTERRITORYLchallage;	
			if (courageChallengeData.m_LuckyValues1 < 0)
			{
				courageChallengeData.m_LuckyValues1 = 0;
			}
		}
		if (req.id == CourageID2 )
		{
			courageChallengeData.m_LuckyValues2 += globalValue.uTERRITORYLchallage;	
			if (courageChallengeData.m_LuckyValues2 < 0)
			{
				courageChallengeData.m_LuckyValues2 = 0;
			}
		}
		if (req.id == CourageID3 )
		{
			courageChallengeData.m_LuckyValues3 += globalValue.uTERRITORYLchallage;	
			if (courageChallengeData.m_LuckyValues3 < 0)
			{
				courageChallengeData.m_LuckyValues3 = 0;
			}
		}
		player->setCourageChallengeData(courageChallengeData);//必须存档1
		CourageChallengeManager::getSingleton().initCourageMonster(courageChallengeData,req.id );
		player->setCourageChallengeData(courageChallengeData);//必须存档2
	}

	player->getPersistManager().setDirtyBit(COURAGECHALLENGEDATABIT);//要和setCourageChallengeData分开


	Json::Value stages;
	GiftManager::getSingleton().saveEndsAttr(player->getPlayerGuid(),itemList,resp.allAttr,stages,MiniLog87);

	//更新成就
	player->getAchieveManager().updateAchieveData(COURAGESUCCESS, 1);

	//更新每日勇气试炼
	player->getAchieveManager().updateDailyTaskData(DLYDOCOURAGECNT, 1);

	GiftManager::getSingleton().PlayerItemChangeResult(player->getPlayerGuid(),0,resp.ends);


	// 	resp.goodsList = itemList;
// 	resp.catCoins = courageChallengeData.catCoins;
	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,COURAGE_CHALLENG_END_RESP,jsonStr);

	LogicSystem::getSingleton().updateSevenDayTask(player->getPlayerGuid(),SDT16,1);


	
	CourageChallengeReq courageChallengeReq;
	courageChallengeReq.reqType =0;
	CourageChallengeManager::getSingleton().onCourageChallengeReq(connId,courageChallengeReq);



}


//1 猫币兑换光 2一键兑换 3 光兑换猫币
void CourageChallengeManager::onCatCoinReq(const  ConnId& connId,CatCoinReq  & req)
{

	UInt32 stageID = 0;
	UInt32 chapterID = 0;
	UInt32 lightOfLife = 0;
	UInt32 getIt = 0;
	UInt32 nowTime = TimeUtil::getTimeSec();
	Goods goods;
	CourageChallengeReq CCreq;
	PlayerCourageChallengeData courageChallengeData;
	CatCoinResp resp;	
	List<Goods> itemList;
	List<CatCoin> catCoins;	

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if (player == NULL)
	{
		return;
	}
	courageChallengeData = player->getCourageChallengeData();
	req.convertJsonToData(req.strReceive);
	resp.reqType = req.reqType;
	resp.result = LynxErrno::None;


	CCreq.needSend = false;
	onCourageChallengeReq(connId ,CCreq);


	if (req.reqType == 1)
	{		
		for(List<CatCoin>::Iter*it =  courageChallengeData.catCoins.begin();it != NULL;it = courageChallengeData.catCoins.next(it))
		{
			if (it->mValue.catCoinID == req.catCoinID)
			{
				CoinTemplate* coinTemplate = gCoinTable->get(req.catCoinID);
				if(coinTemplate == NULL)
				{
					continue;
				}
				goods.resourcestype = AWARD_CURRENCY;
				goods.subtype = 1;
				goods.num = it->mValue.num * coinTemplate->light;
				itemList.insertTail(goods);
				goods.resourcestype = AWARD_CATCOIN;
				goods.subtype = req.catCoinID;
				goods.num = 0 - it->mValue.num;
				itemList.insertTail(goods);
				GiftManager::getSingleton().saveEndsGetAttr(player->getPlayerGuid(),itemList,resp.allAttr,MiniLog89);	
// 				CourageChallengeManager::getSingleton().setCatCoins(player->getPlayerGuid(),courageChallengeData,goods,true);
				courageChallengeData = player->getCourageChallengeData();
				if (it->mValue.num == 0)
				{
					courageChallengeData.catCoins.erase(it);
				}
				
				break;
			}
		}
		player->setCourageChallengeData(courageChallengeData);
	}
	else if(req.reqType == 2)
	{		
		for(List<CatCoin>::Iter*it =  courageChallengeData.catCoins.begin();it != NULL;it = courageChallengeData.catCoins.next(it))
		{
			CoinTemplate* coinTemplate = gCoinTable->get(it->mValue.catCoinID);		
			if(coinTemplate == NULL)
			{
				continue;
			}
			goods.resourcestype = AWARD_CURRENCY;
			goods.subtype = 1;
			goods.num = it->mValue.num * coinTemplate->light;
			itemList.insertTail(goods);
			goods.resourcestype = AWARD_CATCOIN;
			goods.subtype = it->mValue.catCoinID;
			goods.num = 0 - it->mValue.num;
			if (goods.num == 0)
			{
				continue;
			}
			itemList.insertTail(goods);
					
		}
		GiftManager::getSingleton().combineSame(itemList);
		GiftManager::getSingleton().saveEndsGetAttr(player->getPlayerGuid(),itemList,resp.allAttr,MiniLog89);	
		courageChallengeData = player->getCourageChallengeData();
		
		for(List<CatCoin>::Iter*it =  courageChallengeData.catCoins.begin();it != NULL;it = courageChallengeData.catCoins.next(it))
		{
			if (it->mValue.num == 0)
			{
				courageChallengeData.catCoins.erase(it);
			}			
		}
		player->setCourageChallengeData(courageChallengeData);
	}
	else if(req.reqType == 3)
	{
		CoinTemplate* coinTemplate = gCoinTable->get(req.catCoinID);	
		if(coinTemplate == NULL)
		{
			return;
		}
		for(List<CatCoin>::Iter*it =  courageChallengeData.catCoins.begin();it != NULL;it = courageChallengeData.catCoins.next(it))
		{
			if (it->mValue.catCoinID == req.catCoinID)
			{
				getIt = 1;
				if (req.num *coinTemplate->light > courageChallengeData.m_LightOfLife )
				{
					resp.result = LynxErrno::CatCoinNotEnough;
				}
				else
				{
					goods.resourcestype = AWARD_CURRENCY;
					goods.subtype = 1;
					goods.num = 0 - it->mValue.num * coinTemplate->light;
					itemList.insertTail(goods);
					Goods goods;
					goods.resourcestype = AWARD_CATCOIN;
					goods.subtype = req.catCoinID;
					goods.num = req.num;
					itemList.insertTail(goods);
					
				}				
				break;
			}
		}
		GiftManager::getSingleton().saveEndsGetAttr(player->getPlayerGuid(),itemList,resp.allAttr,MiniLog89);	
	}

// 	courageChallengeData.m_LightOfLife += lightOfLife;
// 	player->setCourageChallengeData(courageChallengeData);
// 	player->getPersistManager().setDirtyBit(COURAGECHALLENGEDATABIT);

	courageChallengeData = player->getCourageChallengeData();
	resp.lightOfLife = courageChallengeData.m_LightOfLife;
	resp.catCoins = courageChallengeData.catCoins;

	
	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,CAT_COIN_RESP,jsonStr);

}

UInt32 CourageChallengeManager::getCatCoinNum(Guid playerID,UInt32 id)
{

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return 0;
	}
	PlayerCourageChallengeData courageChallengeData = player->getCourageChallengeData();

	for (List<CatCoin>::Iter *iter = courageChallengeData.catCoins.begin();iter != NULL;iter = courageChallengeData.catCoins.next(iter) )
	{
		if (iter->mValue.catCoinID == id)
		{
			return iter->mValue.num;
		}
	}
	return 0;
}

void CourageChallengeManager::setCatCoins(Guid playerID,PlayerCourageChallengeData &courageChallengeData, Goods goods, bool needSave = true)
{

	ShopItems shopItems;
	UInt32 needInsert = 0;
	UInt32 getIt = 0;
	UInt32 coinNum = goods.num;
	CatCoinSaveReq req;

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}
	if (goods.resourcestype != AWARD_CATCOIN)
	{
		return;
	}

	for (List<CatCoin>::Iter *iter = courageChallengeData.catCoins.begin();iter != NULL;iter = courageChallengeData.catCoins.next(iter) )
	{
		if (iter->mValue.catCoinID == goods.subtype)
		{
			getIt = 1;
			iter->mValue.num = iter->mValue.num + goods.num;
			coinNum = iter->mValue.num;			
			break;
		}
	}
	if (getIt == 0)
	{
		CatCoin catCoin;
		catCoin.catCoinID = goods.subtype;
		catCoin.num = goods.num;
		if (catCoin.num != 0)
		{
			courageChallengeData.catCoins.insertTail(catCoin);
		}		
	}

	player->setCourageChallengeData(courageChallengeData);
	player->getPersistManager().setDirtyBit(COURAGECHALLENGEDATABIT);


	if (needSave == true)
	{
		req.catCoinID = goods.subtype;
		req.playerID = playerID;
		req.num = coinNum;
		PersistSystem::getSingleton().postThreadMsg(req, 0);
	}

	return ;
}



void CourageChallengeManager::getCourageContentByID(UInt32 itemID,List<Goods> &ItemList)
{
	List<Content> contentList;
	UInt32 allWeight1 = 0;
	UInt32 weight1 = 0;
	UInt32 weight2 = 0;
	UInt32 randomNum =0;
	UInt32 finished = 0;
	Goods goods;


	for (List<AwardContentTemplate>::Iter *iter = gCourageContentTable->mContentList.begin();iter!= NULL;iter = gCourageContentTable->mContentList.next(iter))
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
	}
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
				ItemList.insertTail(goods);
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
				ItemList.insertTail(goods);
			}
		}
		if (it->mValue.contenttype == 3)
		{
			goods.subtype = it->mValue.subtype;
			goods.resourcestype = it->mValue.resourcestype;		
			goods.num = it->mValue.num;
			ItemList.insertTail(goods);

		}
	}
}


void CourageChallengeManager::onCourageShopReq(const  ConnId& connId, CourageShopReq & msg)
{
	UInt32 refreshTimes = 0;
	CourageShopResp resp;

	LOG_INFO("onCourageShopReq = %s",msg.jsonStr.c_str());	

	msg.convertJsonToData(msg.jsonStr);
	resp.reqType = msg.reqType;
	resp.shopType = msg.shopType;
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);

	if (player == NULL)
	{
		return;
	}
	PlayerCourageChallengeData courageChallengeData = player->getCourageChallengeData();

	if (msg.shopType == CourageShop)
	{
		resp.result = CourageChallengeManager::getSingleton().getCourageShopInfo( player->getPlayerGuid(),msg.reqType,resp);
	}

	resp.gold = player->getPlayerGold();

	resp.lightOfLife = courageChallengeData.m_LightOfLife;

	if (msg.needSend == 1)
	{
		std::string jsonStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,COURAGE_SHOP_INFO_RESP,jsonStr);

		LOG_INFO("onCourageShopReq resp= %s",jsonStr.c_str());	
	}	
}




//refresh 0获取信息 1刷新 2 时间到刷新
//refreshTimes 第几个

UInt32 CourageChallengeManager::getCourageShopInfo(Guid playerID,UInt32 refresh,CourageShopResp &resp)
{
	UInt32 acID = 0;
	UInt32 level = 0;
	UInt32 vipLevel = 0;
	UInt32 refreshTimes = 0;
	double cost = 0;
	UInt32 getIt = 0;
	UInt32 refreshTime = 0;
	UInt32 refreshMaxTimes = 0;
	UInt32 j = 0;
	Goods goods;
	List<Goods> addGoodsList;
	List<UInt32> awardcontentIDs;
	List<Goods> goodsList;

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();


	UInt32 flag = ShopManager::getSingleton().checkRefreshTime(playerID,CourageShop,resp.refreshNeedTime);

	refreshTimes = flag;
	if(refresh == 1)
	{
		refreshTimes ++;
	}
	else
	{
		if (flag == 10000)
		{
			refresh = 2;
			refreshTimes = 0;			 
		}
	}




	refreshMaxTimes = globalValue.uCOURAGESHOPrefreshtimes.size();


	resp.refreshMaxTimes = refreshMaxTimes;

	if (refreshTimes > refreshMaxTimes)
	{
		refreshTimes = refreshMaxTimes;
		return LynxErrno::RefreshTimesNotEnough;
	}
	resp.refreshTimes = refreshTimes;
	if (refresh != 0)
	{
		refreshTime = TimeUtil::getTimeSec();
		ShopManager::getSingleton().setShopItems(playerID,CourageShop,refreshTimes,refreshTime,true);

	}	

	if (refresh ==1 )
	{
		for(List<UInt32>::Iter *iter = globalValue.uCOURAGESHOPrefreshtimes.begin();iter!= NULL;iter = globalValue.uCOURAGESHOPrefreshtimes.next(iter))
		{
			j++;
			if (j == refreshTimes)
			{
				cost = iter->mValue;
				break;
			}
		}

		if (player->getPlayerGold() < cost)
		{
			return LynxErrno::RmbNotEnough;
		}

		goods.resourcestype = AWARD_BASE;
		goods.subtype = AWARD_BASE_GOLD;
		goods.num = 0 - cost;
		addGoodsList.insertTail(goods);
		GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),1,addGoodsList,MiniLog84);
	}

	UInt32 needSave = 0;
	List<Item> itemList;
	UInt32 getItemCount = 0;
	UInt32 max = ShopItemCount; 
	for (UInt32 i = 1;i <= max;i++)
	{
		acID = 0;

		if (refresh != 0)
		{
			acID = getCourageShopID(i,player->getPlayerLeval(),player->getVipLevel(),refreshTimes,refresh);
		}


		Item item;

		ShopManager::getSingleton().getShopNewItems(playerID, i,acID,CourageShop,item);

		if (acID != 0)
		{
			needSave = 1;
			item.buyTimes = 0;			
		}

		itemList.insertTail(item);//要发送给客户端数据，存盘数据
		if (item.goods.subtype != 0)
		{
			getItemCount ++;
			// 			resp.items.insertTail(item);
		}
		else
		{
			LOG_WARN("failed  getMysticalShop item.goods.subtype is 0 i = %d !", i);
		}

	}





	if (getItemCount != max  )
	{
		LOG_WARN("failed  getMysticalShop count not right refreshTimes = %d !", refreshTimes);		
	}

	if ( needSave == 1)
	{
		UInt32 count1 = 0;
		UInt32 count2 = 0;
		List<Item> tmpItemList;
		List<UInt32> indexList;

		RankGameManager::getSingleton().getRandomOrderNums(max ,indexList);	
		for (List<UInt32>::Iter *indexIter = indexList.begin();indexIter != NULL;indexIter = indexList.next(indexIter))
		{
			count1 ++;
			count2 = 0;
			for (List<Item>::Iter *itemIter = itemList.begin();itemIter!=NULL;itemIter= itemList.next(itemIter))
			{
				count2 ++;
				if (count1 == count2 )
				{
					itemIter->mValue.position = indexIter->mValue +1;

					break;
				}
			}
		}

		count1 = 0;
		for (UInt32 i = 1;i <= max;i++)
		{	
			count1 ++;//1开始
			for (List<Item>::Iter *itemIter = itemList.begin();itemIter!=NULL;itemIter= itemList.next(itemIter))
			{
				if (itemIter->mValue.position == count1)
				{
					tmpItemList.insertTail(itemIter->mValue);
					break;
				}
			}			
		}


		for (List<Item>::Iter *itemIter = tmpItemList.begin();itemIter!=NULL;itemIter= tmpItemList.next(itemIter))
		{
			ShopManager::getSingleton().setShopItem(playerID,CourageShop,itemIter->mValue.position,itemIter->mValue,true);
			resp.items.insertTail(itemIter->mValue);
		}
	}
	else
	{
		for (List<Item>::Iter *itemIter = itemList.begin();itemIter!=NULL;itemIter= itemList.next(itemIter))
		{
			resp.items.insertTail(itemIter->mValue);
		}
	}


	j = 0;
	for(List<UInt32>::Iter *iter = globalValue.uCOURAGESHOPrefreshtimes.begin();iter!= NULL;iter = globalValue.uCOURAGESHOPrefreshtimes.next(iter))
	{
		j++;
		cost = iter->mValue;			
		if (j == (refreshTimes+1))
		{
			break;
		}
	}
	resp.refreshCost = cost;
	return LynxErrno::None;

}
// 
// 
// UInt32 CourageChallengeManager::checkCourageRefreshTime(Guid playerID,UInt32 shopType,UInt32 &refreshNeedTime)
// {
// 	UInt32 lastRefreshTime = 0;
// 	UInt32 nextRefreshTime = 0;
// 	UInt32 refreshTime = 0;
// 	UInt32 refreshTimes = 0;
// 	UInt32 tmp = TimeUtil::getSecOfToday();
// 	UInt32 dayBeiginTime =  TimeUtil::getTimeSec() - TimeUtil::getSecOfToday();
// 	Vector<String> strVector;
// 	Item item;
// 
// 
// 	UInt32 nowTime = TimeUtil::getTimeSec();
// 	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
// 	if (player == NULL)
// 	{
// 		LOG_WARN("player not found!!");
// 		return 0;
// 	}
// 
// 	ShopManager::getSingleton().getPlayerItem( playerID,shopType, 0,item,refreshTime,refreshTimes);
// 
// 	UInt32 systemRefreshTime = LogicSystem::getSingleton().getRefreshTime(TIME_REFRESH_ID13);
// 	char dest[64] = {};
// 	sprintf(dest,"%d",systemRefreshTime);
// 	String sSystemRefreshTime = (String)dest;
// 	strVector.push_back(sSystemRefreshTime);
// 
// 	UInt32 getIt = 0;
// 
// 	for (UInt32 i = 0; i < strVector.size();i++)
// 	{
// 		if (nowTime < (dayBeiginTime + atoi(strVector[i].c_str())  ) )
// 		{
// 			nextRefreshTime = (dayBeiginTime + atoi(strVector[i].c_str())  );
// 			if (i == 0)
// 			{
// 				lastRefreshTime = dayBeiginTime - 24*3600 + atoi(strVector[strVector.size() -1].c_str());//昨天最后一个
// 			}
// 			else
// 			{
// 				lastRefreshTime = dayBeiginTime + atoi(strVector[i -1].c_str());
// 
// 			}
// 			getIt = 1;
// 			break;			
// 		}		
// 	}
// 	if (getIt == 0)
// 	{
// 		nextRefreshTime = (dayBeiginTime +  (24*3600+atoi(strVector[0].c_str()))  );//明天第一次
// 		lastRefreshTime = dayBeiginTime  + atoi(strVector[strVector.size() -1].c_str());//今天最后一次刷新
// 	}
// 	refreshNeedTime = nextRefreshTime - nowTime;
// 
// 	if(refreshTime < lastRefreshTime )
// 	{
// 		return 10000;		
// 	}
// 
// 	return refreshTimes;
// }

UInt32 CourageChallengeManager::getCourageShopID(UInt32 position,UInt32 level,UInt32 vipLevel,UInt32 refreshTimes,UInt32 refresh)
{
	UInt32 nowTime = TimeUtil::getTimeSec();
	CourageshopTemplate *courageshopTemplate = NULL;

	if (level % 10 == 0)
	{
		level = (level/10)*10;
	}
	else
	{
		level = (level/10)*10+ 10;
	}

	vipLevel = (vipLevel/5)*5;

	for(Map<UInt32, CourageshopTemplate>::Iter *iter = gCourageshopTable->mMap.begin();iter != NULL;iter = gCourageshopTable->mMap.next(iter))
	{
		if (iter->mValue.shopposition == position /*&& iter->mValue.playerLv == level*/ && iter->mValue.vipLv == vipLevel)
		{
			courageshopTemplate = &iter->mValue;
			break;
		}
	}
	if (courageshopTemplate == NULL)
	{
		return 0;
	}

	if (refreshTimes == 0)
	{
		return courageshopTemplate->timeawardcontentshop;
	}
	else
	{
		UInt32 i = 0;
		for (List<UInt32>::Iter * it =  courageshopTemplate->awardcontentshops.begin(); it != NULL; it = courageshopTemplate->awardcontentshops.next(it))
		{
			i++;
			if (i == refreshTimes)
			{
				return it->mValue;
			}
		}
	}	

	return 0;
}

void CourageChallengeManager::onCourageShopBuyReq(const  ConnId& connId, CourageShopBuyReq & msg)
{
	UInt32 cost = 0;
	CourageShopBuyResp resp;
	msg.convertJsonToData(msg.jsonStr);
	resp.shopType = msg.shopType;
	resp.position = msg.position;

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		return;
	}

	//先刷新	
	CourageShopReq  msg1;
	msg1.shopType = CourageShop;
	msg1.reqType = 0;
	msg1.needSend = 0;
	CourageChallengeManager::getSingleton().onCourageShopReq(connId,msg1);

	if (msg.shopType == CourageShop)
	{
		resp.result = CourageChallengeManager::getSingleton().dealCourageshopbuy(player->getPlayerGuid(),msg.shopType,msg.position,resp);
	}

	PlayerCourageChallengeData courageChallengeData = player->getCourageChallengeData();

	resp.lightOfLife = courageChallengeData.m_LightOfLife;
	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,COURAGE_SHOP_BUY_RESP,jsonStr);

	
}


// 
// 
// void CourageChallengeManager::getCourageShopNewItems(Guid playerID, UInt32 position,UInt32 acID,UInt32 shopType,Item &item)
// {
// 	UInt32 refreshTime;
// 	UInt32 refreshTimes;
// 	UInt32 nowTime = TimeUtil::getTimeSec();
// 	UInt32 dayBeiginTime =  TimeUtil::getTimeSec() - TimeUtil::getSecOfToday();
// 	List<Goods> goodsList;
// 	CourageshopTemplate *courageshopTemplate;
// 	courageshopTemplate =  gCourageshopTable->get(position);
// 	if (courageshopTemplate == NULL)
// 	{
// 		return;
// 	}
// 
// 	ShopManager::getSingleton().getPlayerItem( playerID,shopType, position,item,refreshTime,refreshTimes);
// 
// 	if (item.goods.subtype == 0)
// 	{
// 		item.itemID = acID;
// 		item.position = position;
// 		item.shopType = shopType;
// 		item.buyTime = 0;
// 		item.buyTimes = 0;
// 
// 		item.goods.resourcestype = courageshopTemplate->subtype1;
// 		item.goods.subtype = courageshopTemplate->num1;
// 		item.goods.num = 1;
// 
// 		item.cost = courageshopTemplate->costnumber1;
// 
// 		ShopManager::getSingleton().setShopItem(playerID,shopType,position,item,true);
// 	}
// 	else
// 	{
// 		item.cost = courageshopTemplate->costnumber1;
// 	}
// 
// }



UInt32 CourageChallengeManager::dealCourageshopbuy(Guid playerID,UInt32 shopType,UInt32 position,CourageShopBuyResp &resp)
{
	Goods goods;
	List<Goods> addGoodsList;

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);


	ShopItems  shopItems = ShopManager::getSingleton().getShopItems( playerID,  shopType);

	Item  item = ShopManager::getSingleton().getShopItem( playerID, shopType, position);
	PlayerCourageChallengeData courageChallengeData = player->getCourageChallengeData();

	if(item.position == 0)
	{
		return LynxErrno::NotFound;
	}

	if (item.buyTimes > 0)
	{
		return LynxErrno::HadBuy;
	}

	CommodityTemplate* commodityTemplate = gCommodityTable->get(item.goods.subtype);
	if (commodityTemplate == NULL)
	{
		return LynxErrno::NotFound;
	}
	UInt32 cost = commodityTemplate->costnumber1;

	if( commodityTemplate->costype1 == AWARD_BASE &&  commodityTemplate->subtype1 == AWARD_BASE_GOLD)
	{
		if(player->getPlayerGold() < cost)
		{
			return LynxErrno::RmbNotEnough;
		}
	}
	if (commodityTemplate->costype1 == AWARD_SERVANTSWITCH)
	{
		if(player->getAllItemManager().getAwardCount(AWARD_SERVANTSWITCH,0) < cost)
		{
			return LynxErrno::ServantSwitchNotEnough;
		}
	}
	if(commodityTemplate->costype1 == AWARD_CURRENCY)
	{
		if (courageChallengeData.m_LightOfLife < cost)
		{
			return LynxErrno::CurrencyNotEnough;
		}
	}
	if( commodityTemplate->costype1 == AWARD_BASE &&  commodityTemplate->subtype1 == AWARD_BASE_COIN)
	{
		if(player->getPlayerCoin() < cost)
		{
			return LynxErrno::CoinNotEnough;
		}
	}

	item.buyTimes += commodityTemplate->num1;
	ShopManager::getSingleton().setShopItem(playerID,shopType,position,item,true);
	resp.buyNum = item.buyTimes;

	// 	getShopItemGoods();
	goods.resourcestype = commodityTemplate->resourcesType1;
	goods.subtype = commodityTemplate->subtype1;
	goods.num = commodityTemplate->num1 ;
	addGoodsList.insertTail(goods);

	Award award;
	award.award = addGoodsList;
	resp.awards.insertTail(award);



	addGoodsList.clear();
	goods.resourcestype = commodityTemplate->costype1;
	goods.subtype = commodityTemplate->costsubtype1;
	goods.num = 0 - cost;
	addGoodsList.insertTail(goods);	

	player->ResetFireConfirmData();
	PlayerFireConfirmData mFireConfirmData =  player->GetFireConfirmData();

	mFireConfirmData.m_AwardsList = resp.awards;
	mFireConfirmData.m_CostList = addGoodsList;
	mFireConfirmData.m_CopyID = GETAWARDSCOPYID;
	player->SetFireConfirmData(mFireConfirmData);

	ChapterEndResp awardResult;
	FireConfirmManager::getSingleton().saveAndGetResult(player->getConnId(),awardResult,0,MiniLog100);
	resp.ends = awardResult.ends;
	resp.allAttr = awardResult.allAttr;
	player->ResetFireConfirmData();

	return LynxErrno::None;
}

void CourageChallengeManager::divisionCoinGrop(List<UInt32> &group1,List<UInt32> &group2,List<UInt32> &group3,List<UInt32> group)
{
	CoinGroupTemplate * coinGroupTemplate;
	for(List<UInt32>::Iter *iter = group.begin();iter!=NULL;iter = group.next(iter))
	{
		coinGroupTemplate = gCoinGroupTable->get(iter->mValue);
		if (coinGroupTemplate == NULL)
		{
			continue;
		}
		if (coinGroupTemplate->territoryID == CourageID1 )
		{
			group1.insertTail(iter->mValue);
		}
		else if (coinGroupTemplate->territoryID == CourageID2 )
		{
			group2.insertTail(iter->mValue);
		}
		else if (coinGroupTemplate->territoryID == CourageID3 )
		{
			group3.insertTail(iter->mValue);
		}

	}

}