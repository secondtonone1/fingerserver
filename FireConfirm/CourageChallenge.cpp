#include "../LogicSystem.h"
#include "StringConverter.h"
#include "../PlatformLib/SerializeType.h"
#include "../PersistSystem.h"
#include "../PlatformLib/Utilex.h"
#include "../CommonLib/CopyFinishTime.h"
#include "Gift.h"
#include "CourageChallenge.h"

using namespace Lynx;

//reqtype 0请求信息 1刷新 2购买 3激活
void CourageChallengeManager::onCourageChallengeReq(const  ConnId& connId,CourageChallengeReq & req)
{
	UInt32 i = 0;
	UInt32 ii = 0;
	UInt32 maxBuyTimes = 0;
	int lastFinishTimes = 0;
	UInt32 finishTimes = 0;
	int maxCanAddTimes = 0;
	UInt32 cost = 0;
	UInt32 nowTime = TimeUtil::getTimeSec();
	UInt32 dayBeiginTime =  TimeUtil::getTimeSec() - TimeUtil::getSecOfToday();
	Goods goods;
	List<Goods> itemList;
	CourageChallengeResp resp;
	PlayerCourageChallengeData courageChallengeData;	
	
	req.convertJsonToData(req.strReceive);
	resp.reqType = req.reqType;	
	resp.id = req.id;

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	courageChallengeData = player->getCourageChallengeData();
	TerritoryLuckyTemplate territoryLuckyTemplate = GlobalVarManager::getSingleton().getTerritoryLucky();


	for (List<VipTemplate>::Iter * iter = gVipTable->mVip.begin();iter!= NULL;iter = gVipTable->mVip.next(iter))
	{
		if (iter->mValue.id == player->getVipLevel())
		{
			maxBuyTimes =  iter->mValue.territorybuytimes; 

			if ((nowTime - dayBeiginTime)< 6 * 3600 )
			{
				if (courageChallengeData.m_BuyTime < (dayBeiginTime - 6 * 3600))
				{
					courageChallengeData.m_BuyTime = nowTime;
					courageChallengeData.m_BuyTimes = 0;
					courageChallengeData.m_BuyTimes = 0;
					courageChallengeData.m_RefreshTimes = 0;
				}
			}
			else if ( (nowTime - dayBeiginTime)>= (6 * 3600) &&(nowTime - dayBeiginTime) < (18 * 3600))
			{
				if (courageChallengeData.m_BuyTime < (dayBeiginTime + 6 * 3600))
				{
					courageChallengeData.m_BuyTime = nowTime;
					courageChallengeData.m_BuyTimes = 0;
					courageChallengeData.m_BuyTimes = 0;
					courageChallengeData.m_RefreshTimes = 0;
				}
			}
			else
			{				
				if (courageChallengeData.m_BuyTime < (dayBeiginTime + 18 * 3600))
				{
					courageChallengeData.m_BuyTime = nowTime;
					courageChallengeData.m_BuyTimes = 0;
					courageChallengeData.m_BuyTimes = 0;
					courageChallengeData.m_RefreshTimes = 0;
				}
			}
			break;
		}
	}

	lastFinishTimes = (nowTime - courageChallengeData.m_BeginTime) / (territoryLuckyTemplate.recovertime);


	if(lastFinishTimes < 0)//有时时间异常
	{
		finishTimes = 0;
	}
	else if (lastFinishTimes > territoryLuckyTemplate.recovertime)
	{
		finishTimes = territoryLuckyTemplate.recovertime;
	}
	else
	{
		finishTimes = lastFinishTimes;
	}

	maxCanAddTimes =(int)( territoryLuckyTemplate.maxtimes - courageChallengeData.m_LeftChallengeTimes);
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
			courageChallengeData.m_BeginTime = courageChallengeData.m_BeginTime + finishTimes * (territoryLuckyTemplate.recovertime);			
		}			
	}
	else
	{
		courageChallengeData.m_BeginTime = nowTime;
	}

	if (req.reqType == 0)
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
		GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_WELFALE_MARKET,itemList);
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
	resp.needTime = territoryLuckyTemplate.recovertime - (nowTime - courageChallengeData.m_BeginTime);
	resp.buyTimes  = courageChallengeData.m_BuyTimes;
	resp.lightOfLife  = player->getAllItemManager().getAwardCount(AWARD_SERVANTSWITCH,0); 
	resp.monsterID1  = courageChallengeData.monsterID1;
	resp.monsterID2  = courageChallengeData.monsterID2;
	resp.monsterID3  = courageChallengeData.monsterID3;
	resp.catCoins  = courageChallengeData.catCoins;
	resp.coinGroup  = courageChallengeData.coinGroup;
	resp.refreshTimes   = courageChallengeData.m_RefreshTimes;
	resp.maxChallengeTimes   = territoryLuckyTemplate.maxtimes;
	resp.maxFreeRefreshTimes   = territoryLuckyTemplate.freerefresh;
	resp.recoverTime = territoryLuckyTemplate.recovertime;

	resp.nextRefreshCost = CourageChallengeManager::getSingleton().getNextRefreshCost(courageChallengeData.m_RefreshTimes);
	
	for (List<UInt32>::Iter *it = territoryLuckyTemplate.buytimess.begin();it != NULL;it = territoryLuckyTemplate.buytimess.next(it))
	{		
		if (ii == courageChallengeData.m_BuyTimes)
		{
			resp.nextBuyCost = it->mValue;
		}
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

	for(List<KeyValue>::Iter* it = territoryTemplate->conditionMonsters.begin(); it != NULL;it = territoryTemplate->conditionMonsters.next(it))
	{
		if (it->mValue.key > maxLuckValues)
		{
			maxLuckValues = it->mValue.key;
			maxContentID = it->mValue.value;
		}
	}

	if (courageChallengeData.m_LuckyValues > maxLuckValues)
	{
		courageChallengeData.m_LuckyValues = 0;//todo 是否已经挑战成功
	}

	for(List<KeyValue>::Iter* it = territoryTemplate->conditionMonsters.begin(); it != NULL;it = territoryTemplate->conditionMonsters.next(it))
	{
		if (  courageChallengeData.m_LuckyValues < it->mValue.key)
		{
			contentID = it->mValue.value;
			break;
		}
	}
	if (contentID == 0)
	{
		contentID = maxContentID;
	}
	getCourageContentByID(contentID,itemList);
	for(List<Goods>::Iter * contentIter =  itemList.begin();contentIter != NULL;contentIter = itemList.next(contentIter))
	{		
		monsterID  = contentIter->mValue.subtype;		
		awardID = contentIter->mValue.num;
	}

	if (id == CourageID1)
	{
		courageChallengeData.monsterID1 = monsterID;
		courageChallengeData.contentID1 = awardID;
	}
	else if (id == CourageID2)
	{
		courageChallengeData.monsterID2 = monsterID;
		courageChallengeData.contentID2 = awardID;
	}
	else if (id == CourageID3)
	{
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
	TerritoryLuckyTemplate territoryLuckyTemplate = GlobalVarManager::getSingleton().getTerritoryLucky();

	goldRefreshTimes = (int)courageChallengeData.m_RefreshTimes - territoryLuckyTemplate.freerefreshtimes + 1;
	if (goldRefreshTimes < 0)
	{
		goldRefreshTimes = 0;
	}
	
	for(List<UInt32>::Iter *iter = territoryLuckyTemplate.costs.begin();iter != NULL;iter = territoryLuckyTemplate.costs.next(iter))
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

	if (goldRefreshTimes == 0)
	{		
		courageChallengeData.m_LuckyValues += territoryLuckyTemplate.freerefresh;
		getIt = 1;
	}
	else
	{
		i = 0;
		for(List<UInt32>::Iter *iter = territoryLuckyTemplate.refreshs.begin();iter!= NULL;iter = territoryLuckyTemplate.refreshs.next(iter))
		{
			i++;
			if (i == goldRefreshTimes)
			{
				getIt = 1;
				courageChallengeData.m_LuckyValues += iter->mValue;
			}
			lastLucky = iter->mValue;
		}
	}
	if (getIt == 0)
	{
		courageChallengeData.m_LuckyValues += lastLucky;
	}


	TerritoryTemplate * territoryTemplate = gTerritoryTable->get(id);

	for(List<KeyValue>::Iter* it = territoryTemplate->conditionMonsters.begin(); it != NULL;it = territoryTemplate->conditionMonsters.next(it))
	{
		if (courageChallengeData.m_LuckyValues < it->mValue.key)
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
		courageChallengeData.m_LuckyValues = 0;
	}
	getCourageContentByID(contentID,itemList);
	for(List<Goods>::Iter * contentIter =  itemList.begin();contentIter != NULL;contentIter = itemList.next(contentIter))
	{		
		monsterID  = contentIter->mValue.subtype;		
		awardContentID = contentIter->mValue.num;
	}

	if (id == CourageID1)
	{
		courageChallengeData.monsterID1 = monsterID;
		courageChallengeData.contentID1 = awardContentID;
	}
	else if (id == CourageID2)
	{
		courageChallengeData.monsterID2 = monsterID;
		courageChallengeData.contentID2 = awardContentID;
	}
	else if (id == CourageID3)
	{
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

	TerritoryLuckyTemplate territoryLuckyTemplate = GlobalVarManager::getSingleton().getTerritoryLucky();
	goldRefreshTimes = (int)refreshTimes - territoryLuckyTemplate.freerefreshtimes + 1;
	if (goldRefreshTimes < 0)
	{
		goldRefreshTimes = 0;
	}

	for(List<UInt32>::Iter *iter = territoryLuckyTemplate.costs.begin();iter != NULL;iter = territoryLuckyTemplate.costs.next(iter))
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
	TerritoryLuckyTemplate territoryLuckyTemplate = GlobalVarManager::getSingleton().getTerritoryLucky();

	goldBuyTimes = courageChallengeData.m_BuyTimes + 1;

	for(List<UInt32>::Iter *iter = territoryLuckyTemplate.buytimess.begin();iter != NULL;iter = territoryLuckyTemplate.buytimess.next(iter))
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

	i = 0;
	for(List<UInt32>::Iter *iter = territoryLuckyTemplate.costs.begin();iter!= NULL;iter = territoryLuckyTemplate.costs.next(iter))
	{
		i++;
		if (i == goldBuyTimes)
		{
			courageChallengeData.m_LuckyValues += iter->mValue;
		}
	}
	
	courageChallengeData.m_BuyTimes ++;
	courageChallengeData.m_LeftChallengeTimes ++;
	
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
	TerritoryLuckyTemplate territoryLuckyTemplate = GlobalVarManager::getSingleton().getTerritoryLucky();

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

	UInt32 road = 1;//初始路为1
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
	courageChallengeData = player->getCourageChallengeData();
	req.convertJsonToData(req.strReceive);
	resp.id = req.id;
	
	CCreq.id = req.id;
	CCreq.needSend = false;

	onCourageChallengeReq(connId ,CCreq);

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

	TerritoryLuckyTemplate territoryLuckyTemplate = GlobalVarManager::getSingleton().getTerritoryLucky();
	resp.needTime = territoryLuckyTemplate.recovertime - (nowTime - courageChallengeData.m_BeginTime);
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
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);	
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
		if (coinGroupTemplate->territoryID == id)
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
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if (player == NULL)
	{
		return;
	}
	courageChallengeData = player->getCourageChallengeData();
	
	resp.doType = req.doType;
	resp.id = req.id;

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

	for (List<Goods>::Iter *iter = itemList.begin();iter != NULL;iter= itemList.next(iter))
	{
		if (iter->mValue.resourcestype == AWARD_CATCOIN)
		{
			CourageChallengeManager::getSingleton().setCatCoins(player->getPlayerGuid(),courageChallengeData,iter->mValue,true);
		}
	}
	courageChallengeData = player->getCourageChallengeData();//顺序在前一点
	UInt32 flag = CourageChallengeManager::getSingleton().checkLimit(player->getPlayerGuid(),req.id);
	if(flag != LynxErrno::None)
	{
		resp.result = flag;
	}
	
	if (resp.result == LynxErrno::None)
	{
		TerritoryLuckyTemplate territoryLuckyTemplate = GlobalVarManager::getSingleton().getTerritoryLucky();
		courageChallengeData.m_LuckyValues += territoryLuckyTemplate.challage;
		CourageChallengeManager::getSingleton().initCourageMonster(courageChallengeData,req.id );
	}

	player->setCourageChallengeData(courageChallengeData);
	player->getPersistManager().setDirtyBit(COURAGECHALLENGEDATABIT);

	GiftManager::getSingleton().combineSame(itemList);

	GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,itemList);

	Award award;
	award.award = itemList;
	resp.awards.insertTail(award);

	//更新成就
	player->getAchieveManager().updateAchieveData(COURAGESUCCESS, 1);

	//更新每日勇气试炼
	player->getAchieveManager().updateDailyTaskData(DLYDOCOURAGECNT, 1);

	GiftManager::getSingleton().PlayerItemChangeResult(player->getPlayerGuid(),0,resp.ends);

	
// 	resp.goodsList = itemList;
	resp.catCoins = courageChallengeData.catCoins;
	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,COURAGE_CHALLENG_END_RESP,jsonStr);

	

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
				lightOfLife += it->mValue.num * coinTemplate->light;
				goods.resourcestype = AWARD_CATCOIN;
				goods.subtype = req.catCoinID;
				goods.num = 0 - it->mValue.num;
				CourageChallengeManager::getSingleton().setCatCoins(player->getPlayerGuid(),courageChallengeData,goods,true);
				if (it->mValue.num == 0)
				{
					courageChallengeData.catCoins.erase(it);
				}
				break;
			}
		}
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
			lightOfLife += it->mValue.num * coinTemplate->light;
			goods.resourcestype = AWARD_CATCOIN;
			goods.subtype = it->mValue.catCoinID;
			goods.num = 0 - it->mValue.num;
			CourageChallengeManager::getSingleton().setCatCoins(player->getPlayerGuid(),courageChallengeData,goods,true);			
		}
		for(List<CatCoin>::Iter*it =  courageChallengeData.catCoins.begin();it != NULL;it = courageChallengeData.catCoins.next(it))
		{
			if (it->mValue.num == 0)
			{
				courageChallengeData.catCoins.erase(it);
			}			
		}
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
				if (req.num *coinTemplate->light > player->getAllItemManager().getAwardCount(AWARD_SERVANTSWITCH,0))
				{
					resp.result = LynxErrno::CoinNotEnough;//todo
				}
				else
				{
					Goods goods;
					List<Goods> itemList;
					goods.resourcestype = AWARD_SERVANTSWITCH;
					goods.num = 0 - it->mValue.num * coinTemplate->light;
					itemList.insertTail(goods);
					GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),1,itemList);
					goods.resourcestype = AWARD_CATCOIN;
					goods.subtype = req.catCoinID;
					goods.num = req.num;
					CourageChallengeManager::getSingleton().setCatCoins(player->getPlayerGuid(),courageChallengeData,goods,true);
				}				
				break;
			}
		}
	}
	if (lightOfLife > 0)
	{
		Goods goods;
		List<Goods> itemList;
		goods.resourcestype = AWARD_SERVANTSWITCH;
		goods.num = lightOfLife;
		itemList.insertTail(goods);
		GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),1,itemList);
	}

	player->setCourageChallengeData(courageChallengeData);
	player->getPersistManager().setDirtyBit(COURAGECHALLENGEDATABIT);

	resp.lightOfLife =  player->getAllItemManager().getAwardCount(AWARD_SERVANTSWITCH,0); 
	resp.catCoins = courageChallengeData.catCoins;

	resp.luckyValues = courageChallengeData.m_LuckyValues;
	
	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,CAT_COIN_RESP,jsonStr);

}

void CourageChallengeManager::setCatCoins(Guid playerID,PlayerCourageChallengeData &courageChallengeData, Goods goods, bool needSave = true)
{

	ShopItems shopItems;
	UInt32 needInsert = 0;
	UInt32 getIt = 0;
	UInt32 coinNum = goods.num;
	CatCoinSaveReq req;

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
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


	if (needSave == true)//todo
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