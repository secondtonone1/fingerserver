#include "../LogicSystem.h"
#include "StringConverter.h"
#include "../PlatformLib/SerializeType.h"
#include "../PersistSystem.h"
#include "../PlatformLib/Utilex.h"
#include "Shop.h"

using namespace Lynx;

void ShopManager::onShopReq(const  ConnId& connId, ShopReq & msg)
{
	UInt32 refreshTimes = 0;
	ShopResp resp;

	msg.convertJsonToData(msg.jsonStr);
	resp.reqType = msg.reqType;
	resp.shopType = msg.shopType;
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		return;
	}

	if (msg.shopType == MysticalShop)
	{
		resp.result = ShopManager::getSingleton().getMysticalShopInfo( player->getPlayerGuid(),msg.reqType,resp);
	}
	else if (msg.shopType == ServantShop)
	{
		resp.result = ShopManager::getSingleton().getServantShopInfo( player->getPlayerGuid(),msg.reqType,resp);
	}
	else if (msg.shopType == GiftShop)
	{
		resp.result = ShopManager::getSingleton().getGiftShopInfo( player->getPlayerGuid(),msg.reqType,resp);
	}
	resp.gold = player->getPlayerGold();

	if (msg.needSend == 1)
	{
		std::string jsonStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,SHOP_INFO_RESP,jsonStr);
	}	
}
void ShopManager::onShopBuyReq(const  ConnId& connId, ShopBuyReq & msg)
{
	UInt32 cost = 0;
	ShopBuyResp resp;
	msg.convertJsonToData(msg.jsonStr);
	resp.shopType = msg.shopType;
	resp.position = msg.position;

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		return;
	}

	//先刷新
	ShopReq msg1;
	msg1.reqType = 0;
	msg1.shopType = msg.shopType;
	msg1.needSend = 0;
	ShopManager::getSingleton().onShopReq(connId,msg1);

	if (msg.shopType == MysticalShop ||msg.shopType == ServantShop )
	{
		ShopManager::getSingleton().dealMysticalshopbuy(player->getPlayerGuid(),msg.shopType,msg.position,resp);
	}
	else if (msg.shopType == GiftShop)
	{	
		resp.result = ShopManager::getSingleton().dealGiftshopbuy(player->getPlayerGuid(),msg.shopType,msg.position,resp);
	}


	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,SHOP_BUY_RESP,jsonStr);
}

UInt32 ShopManager::dealMysticalshopbuy(Guid playerID,UInt32 shopType,UInt32 position,ShopBuyResp &resp)
{
	Goods goods;
	List<Goods> addGoodsList;

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);


	ShopItems  shopItems = getShopItems( playerID,  shopType);

	Item  item = getShopItem( playerID, shopType, position);

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
	UInt32 cost = commodityTemplate->num1 * commodityTemplate->costnumber1;

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
	

	item.buyTimes += commodityTemplate->num1;
	setShopItem(playerID,shopType,position,item,true);
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
	GiftManager::getSingleton().servanNeedChangeToPiece(player->getPlayerGuid(),resp.awards,addGoodsList);

	goods.resourcestype = commodityTemplate->costype1;
	goods.subtype = commodityTemplate->costsubtype1;
	goods.num = 0 - cost;
	addGoodsList.insertTail(goods);	

	GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,addGoodsList);
	GiftManager::getSingleton().PlayerItemChangeResult(player->getPlayerGuid(),0,addGoodsList);
	resp.ends = addGoodsList;

	for(List<Goods>::Iter * iter = addGoodsList.begin(); iter != NULL; iter = addGoodsList.next(iter))
	{
		if (iter->mValue.resourcestype == AWARD_SERVANT)
		{
			GiftManager::getSingleton().getAllAttr(player->getPlayerGuid(),AWARD_SERVANT,iter->mValue.subtype,resp.allAttr);
		}
	}

	return LynxErrno::None;
}

UInt32 ShopManager::dealGiftshopbuy(Guid playerID,UInt32 shopType,UInt32 position,ShopBuyResp &resp)
{
	UInt32 weekWhichDay = TimeUtil::getDayNum(-1);
	UInt32 nowTime = TimeUtil::getTimeSec();
	UInt32 dayBeiginTime =  TimeUtil::getTimeSec() - TimeUtil::getSecOfToday();
	List<UInt32> CommodityIDs;
	Vector<String> strVector;
	Vector<String> strVector1;
	Goods goods;
	List<Goods> addGoodsList;

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);

	Item  item = getShopItem( playerID, shopType, position);

	if(item.position == 0)
	{
		return LynxErrno::NotFound;
	}

	if (item.buyTimes > 0)
	{
		return LynxErrno::HadBuy;
	}

	GiftShopTemplate* giftShopTemplate = gGiftShopTable->get(position);
	if (giftShopTemplate == NULL)
	{
		return LynxErrno::NotFound;
	}

	if (giftShopTemplate->gifttype == 1)
	{
		if (item.buyTimes >= giftShopTemplate->canbuyNum)
		{
			return LynxErrno::HadBuy;
		}
	}
	else if (giftShopTemplate->gifttype == 2)
	{
		if (item.buyTime > dayBeiginTime)
		{
			return LynxErrno::HadBuy;
		}
	}

	if (giftShopTemplate->qualification == 1)
	{
		if (player->getVipLevel() < giftShopTemplate->program1)
		{
			return LynxErrno::VipLevelNotEnough;
		}
	}
	else if (giftShopTemplate->qualification == 2)
	{
		if (player->getPlayerLeval() < giftShopTemplate->program1)
		{
			return LynxErrno::LevelNotEnough;
		}
	}
	else if (giftShopTemplate->qualification == 3)
	{
		if (player->getPlayerPower() < giftShopTemplate->program1)
		{
			return LynxErrno::AttackPowerNotRight;
		}
	}

	if(player->getPlayerGold() < giftShopTemplate->costNum)
	{
		return LynxErrno::RmbNotEnough;
	}

	if(giftShopTemplate->starttime != 0)
	{
		if (nowTime < giftShopTemplate->starttime || nowTime > giftShopTemplate->endtime)
		{
			return LynxErrno::TimeNotRight;
		}
	}

	lynxStrSplit(giftShopTemplate->whichday, ";", strVector, true);

	for (UInt32 i = 0; i < strVector.size();i++)
	{
		lynxStrSplit(strVector[i], "_", strVector1, true);		
		if (strVector1.size() != 3)
		{
			break;
		}
		if ( atoi(strVector1[0].c_str()) == weekWhichDay)
		{
			UInt32 weekDayBeginTime = dayBeiginTime + atoi(strVector1[1].c_str())*3600;
			UInt32 weekDayEndTime = dayBeiginTime + atoi(strVector1[2].c_str())*3600;
			if (nowTime < weekDayBeginTime || nowTime > weekDayEndTime )
			{
				return LynxErrno::TimeNotRight;
			}
		}
	}



	GiftTemplate* giftTemplate = gGiftTable->get(item.goods.subtype);
	if (giftTemplate == NULL)
	{
		return LynxErrno::NotFound;
	}
	for(List<UInt32>::Iter* it = giftTemplate->awardcontentshopIDs.begin();it != NULL;it = giftTemplate->awardcontentshopIDs.next(it))
	{
		if (it->mValue != 0)
		{
			CommodityIDs.insertTail(it->mValue);
		}
	}

	for(List<UInt32>::Iter* it = giftTemplate->packageIDs.begin();it != NULL;it = giftTemplate->packageIDs.next(it))
	{
		GiftTemplate* giftTemplate1 = gGiftTable->get(item.goods.subtype);
		if (giftTemplate1 == NULL)
		{
			return LynxErrno::NotFound;
		}
		for(List<UInt32>::Iter* it = giftTemplate1->awardcontentshopIDs.begin();it != NULL;it = giftTemplate1->awardcontentshopIDs.next(it))
		{
			if (it->mValue != 0)
			{
				CommodityIDs.insertTail(it->mValue);
			}
		}
	}


	UInt32 cost = giftShopTemplate->costNum;
	if(player->getPlayerGold() < cost)
	{
		return LynxErrno::RmbNotEnough;
	}
	item.buyTimes ++;
	setShopItem(playerID,shopType,position,item,true);
	resp.buyNum = item.buyTimes;


	List<Goods> commodityIDList;
	for(List<UInt32>::Iter* it = CommodityIDs.begin();it != NULL;it = CommodityIDs.next(it))
	{
		ShopManager::getSingleton().getShopContentByID(it->mValue,commodityIDList);
	}

	for (List<Goods>::Iter * iter = commodityIDList.begin();iter != NULL;iter = commodityIDList.next(iter))
	{
		CommodityTemplate* commodityTemplate = gCommodityTable->get(iter->mValue.subtype);
		if (commodityTemplate == NULL)
		{
			continue;
		}
		goods.resourcestype = commodityTemplate->resourcesType1;
		goods.subtype = commodityTemplate->subtype1;
		goods.num = commodityTemplate->num1 ;
		addGoodsList.insertTail(goods);
	}

	Award award;
	award.award = addGoodsList;
	resp.awards.insertTail(award);

	addGoodsList.clear();
	GiftManager::getSingleton().servanNeedChangeToPiece(player->getPlayerGuid(),resp.awards,addGoodsList);

	goods.resourcestype = giftShopTemplate->costtype;
	goods.subtype = giftShopTemplate->secondarytype;
	goods.num = 0 - cost;
	addGoodsList.insertTail(goods);	


	GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,addGoodsList);
	GiftManager::getSingleton().PlayerItemChangeResult(player->getPlayerGuid(),0,addGoodsList);
	resp.ends = addGoodsList;

	for(List<Goods>::Iter * iter = addGoodsList.begin(); iter != NULL; iter = addGoodsList.next(iter))
	{
		if (iter->mValue.resourcestype == AWARD_SERVANT)
		{
			GiftManager::getSingleton().getAllAttr(player->getPlayerGuid(),AWARD_SERVANT,iter->mValue.subtype,resp.allAttr);
		}
	}

	return LynxErrno::None;
}

//refresh 0获取信息 1刷新 2 时间到刷新
//refreshTimes 第几个

UInt32 ShopManager::getMysticalShopInfo(Guid playerID,UInt32 refresh,ShopResp &resp)
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


	UInt32 flag = checkRefreshTime(playerID,MysticalShop,resp.refreshNeedTime);

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
	MysticalShopCostTemplate mysticalShopCostTemplate =  GlobalVarManager::getSingleton().getMysticalShopCost();
	refreshMaxTimes = mysticalShopCostTemplate.refreshTimeList.size();
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
		setShopItems(playerID,MysticalShop,refreshTimes,refreshTime,true);

	}	

	if (refresh ==1 )
	{
		for(List<UInt32>::Iter *iter = mysticalShopCostTemplate.refreshTimeList.begin();iter!= NULL;iter = mysticalShopCostTemplate.refreshTimeList.next(iter))
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

		goods.resourcestype = 1;
		goods.subtype = 2;
		goods.num = 0 - cost;
		addGoodsList.insertTail(goods);
		GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),1,addGoodsList);
	}

	for (UInt32 i = 1;i < 51;i++)
	{
		acID = 0;

		if (refresh != 0)
		{
			acID = getMysticalShopID(i,player->getPlayerLeval(),player->getVipLevel(),refreshTimes,refresh);
		}


		Item item;
		getShopNewItems(playerID, i,acID,MysticalShop,item);

		if (acID != 0)
		{
			item.buyTimes = 0;
			setShopItem(playerID,MysticalShop,i,item,true);
		}
		if (item.goods.subtype != 0)
		{
			resp.items.insertTail(item);
		}

	}

	j = 0;
	for(List<UInt32>::Iter *iter = mysticalShopCostTemplate.refreshTimeList.begin();iter!= NULL;iter = mysticalShopCostTemplate.refreshTimeList.next(iter))
	{
		j++;
		if (j == (refreshTimes+1))
		{
			cost = iter->mValue;
			resp.refreshCost = cost;
			break;
		}
	}
	return LynxErrno::None;

}


UInt32 ShopManager::getServantShopInfo(Guid playerID,UInt32 refresh,ShopResp &resp)
{
	UInt32 j = 0;
	UInt32 acID = 0;
	UInt32 level = 0;
	UInt32 vipLevel = 0;
	UInt32 refreshTimes = 0;
	UInt32 cost = 0;
	UInt32 getIt = 0;
	UInt32 refreshTime = 0;
	UInt32 refreshMaxTimes = 0;
	Goods goods;
	List<Goods> addGoodsList;
	List<UInt32> awardcontentIDs;
	List<Goods> goodsList;

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);


	UInt32 flag = checkRefreshTime(playerID,ServantShop,resp.refreshNeedTime);

	ServantShopCostTemplate servantShopCostTemplate =  GlobalVarManager::getSingleton().getServantShopCost();

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
	refreshMaxTimes = servantShopCostTemplate.refreshTimeList.size();
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
		setShopItems(playerID,ServantShop,refreshTimes,refreshTime,true);

	}

	if (refresh ==1 )
	{


		for(List<UInt32>::Iter *iter = servantShopCostTemplate.refreshTimeList.begin();iter!= NULL;iter = servantShopCostTemplate.refreshTimeList.next(iter))
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

		goods.resourcestype = 1;
		goods.subtype = 2;
		goods.num = 0 - cost;
		addGoodsList.insertTail(goods);
		GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),1,addGoodsList);
	}

	for (UInt32 i = 1;i < 51;i++)
	{
		acID = 0;

		if (refresh != 0)
		{		
			acID = getServantShopID(i,player->getPlayerLeval(),player->getVipLevel(),refreshTimes,refresh);				
		}

		Item item;
		getShopNewItems(playerID, i,acID,ServantShop,item);
		if (acID != 0)
		{
			item.buyTimes = 0;
			setShopItem(playerID,ServantShop,i,item,true);			
		}
		if (item.goods.subtype != 0)
		{
			resp.items.insertTail(item);
		}
	}
	j = 0;
	for(List<UInt32>::Iter *iter = servantShopCostTemplate.refreshTimeList.begin();iter!= NULL;iter = servantShopCostTemplate.refreshTimeList.next(iter))
	{
		j++;
		if (j == (refreshTimes+1))
		{
			cost = iter->mValue;
			resp.refreshCost = cost;
			break;
		}
	}
	return 0;
}


//refresh 0获取信息 1刷新 2 时间到刷新
//refreshTimes 第几个

UInt32 ShopManager::getGiftShopInfo(Guid playerID,UInt32 refresh,ShopResp &resp)
{
	UInt32 acID = 0;
	UInt32 level = 0;
	UInt32 vipLevel = 0;
	UInt32 refreshTimes = 0;
	UInt32 cost = 0;
	UInt32 getIt = 0;
	UInt32 refreshTime = 0;
	Goods goods;
	List<Goods> addGoodsList;
	List<UInt32> awardcontentIDs;
	List<Goods> goodsList;

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);


	UInt32 flag = checkRefreshTime(playerID,GiftShop,resp.refreshNeedTime);

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
	if (refreshTimes > RefreshMaxTimes)
	{
		refreshTimes = RefreshMaxTimes;
		return LynxErrno::RefreshTimesNotEnough;
	}
	resp.refreshTimes = refreshTimes;
	if (refresh != 0)
	{
		refreshTime = TimeUtil::getTimeSec();
		setShopItems(playerID,GiftShop,refreshTimes,refreshTime,true);

	}

	if (refresh ==1 )
	{
		cost = 10; 
		if (player->getPlayerGold() < cost)
		{
			return LynxErrno::RmbNotEnough;
		}

		goods.resourcestype = 1;
		goods.subtype = 2;
		goods.num = 0 - cost;
		addGoodsList.insertTail(goods);
		GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),1,addGoodsList);
	}

	for (UInt32 i = 1;i < 51;i++)
	{
		Item item;
		acID = 0;

		acID = getGiftShopID(i,player->getPlayerLeval(),player->getVipLevel(),refreshTimes,refresh);


		Item  tmpItem = getShopItem( playerID, GiftShop, i);


		if (refresh == 0)
		{
			if (acID != 0 && tmpItem.goods.subtype == 0)//满足新增礼包
			{
			}
			else
			{
				acID = 0;
			}
		}


		getShopNewItems(playerID, i,acID,GiftShop,item);
		if (acID != 0)
		{
			setShopItem(playerID,GiftShop,i,item,true);
		}
		if (item.goods.subtype != 0)
		{
			resp.items.insertTail(item);
		}
	}

	return 0;
}


void ShopManager::getShopNewItems(Guid playerID, UInt32 position,UInt32 acID,UInt32 shopType,Item &item)
{
	UInt32 refreshTime;
	UInt32 refreshTimes;
	UInt32 nowTime = TimeUtil::getTimeSec();
	UInt32 dayBeiginTime =  TimeUtil::getTimeSec() - TimeUtil::getSecOfToday();
	List<Goods> goodsList;
	getPlayerItem( playerID,shopType, position,item,refreshTime,refreshTimes);

	if (acID != 0)
	{
		item.itemID = acID;
		item.position = position;
		item.shopType = shopType;
		item.buyTime = nowTime;
		item.buyTimes = 0;
		if (shopType == MysticalShop ||shopType == ServantShop)
		{
			ShopManager::getSingleton().getShopContentByID(acID,goodsList);
			for (List<Goods>::Iter * iter = goodsList.begin();iter != NULL;iter = goodsList.next(iter))
			{
				item.goods = iter->mValue;			
				break;
			}
		}
		else
		{
			item.goods.resourcestype = GiftShop;
			item.goods.subtype = acID;
			item.goods.num = 1;
		}
		setShopItem(playerID,shopType,position,item,true);
	}
	if (shopType == GiftShop && item.buyTimes != 0)
	{
		GiftShopTemplate * giftShopTemplate = gGiftShopTable->get(position);
		if (giftShopTemplate == NULL)
		{
			return;
		}

		if (giftShopTemplate->gifttype == 2 )
		{
			if (item.buyTime < dayBeiginTime)
			{
				item.buyTime = nowTime;
				item.buyTimes = 0;
				setShopItem(playerID,shopType,position,item,true);
			}
		}
	}

}

void ShopManager::getPlayerItem(Guid playerID,UInt32 shopType,UInt32 position,Item &item,UInt32 &refreshTime,UInt32 &refreshTimes)
{
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	PlayerShopData playerShopData = player->getPlayerShopData();

	for (List<ShopItems>::Iter *iter =  playerShopData.m_ShopItems.begin(); iter != NULL;iter = playerShopData.m_ShopItems.next(iter))
	{
		if (iter->mValue.shopType == shopType)
		{
			refreshTime = iter->mValue.refreshTime;
			refreshTimes = iter->mValue.refreshTimes;
			if (position == 0)//不获取下面的信息
			{
				break;
			}

			for(List<Item>::Iter * it = iter->mValue.items.begin();it != NULL;it = iter->mValue.items.next(it))
			{
				if (it->mValue.position == position)
				{
					item = it->mValue;
					break;
				}
			}
			break;
		}
	}

}



UInt32 ShopManager::getMysticalShopID(UInt32 position,UInt32 level,UInt32 vipLevel,UInt32 refreshTimes,UInt32 refresh)
{
	UInt32 nowTime = TimeUtil::getTimeSec();
	MysticalshopTemplate *mysticalshopTemplate = NULL;

	if (level % 10 == 0)
	{
		level = (level/10)*10;
	}
	else
	{
		level = (level/10)*10+ 10;
	}

	vipLevel = (vipLevel/5)*5;

	for(Map<UInt32, MysticalshopTemplate>::Iter *iter = gMysticalshopTable->mMap.begin();iter != NULL;iter = gMysticalshopTable->mMap.next(iter))
	{
		if (iter->mValue.shopposition == position && iter->mValue.playerLv == level && iter->mValue.vipLv == vipLevel)
		{
			mysticalshopTemplate = &iter->mValue;
			break;
		}
	}
	if (mysticalshopTemplate == NULL)
	{
		return 0;
	}

	if (refreshTimes == 0)
	{
		return mysticalshopTemplate->timeawardcontentshop;
	}
	else
	{
		UInt32 i = 0;
		for (List<UInt32>::Iter * it =  mysticalshopTemplate->awardcontentshops.begin(); it != NULL; it = mysticalshopTemplate->awardcontentshops.next(it))
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


UInt32 ShopManager::getServantShopID(UInt32 position,UInt32 level,UInt32 vipLevel,UInt32 refreshTimes,UInt32 refresh)
{
	UInt32 nowTime = TimeUtil::getTimeSec();
	ServantshopTemplate *servantshopTemplate = NULL;

	// 	if (level % 10 == 0)
	// 	{
	// 		level = (level/10)*10;
	// 	}
	// 	else
	// 	{
	// 		level = (level/10)*10+ 10;
	// 	}

	vipLevel = (vipLevel/5)*5;

	for(Map<UInt32, ServantshopTemplate>::Iter *iter = gServantshopTable->mMap.begin();iter != NULL;iter = gServantshopTable->mMap.next(iter))
	{
		if (iter->mValue.shopposition == position /*&& iter->mValue.playerLv == level*/ && iter->mValue.vipLv == vipLevel)
		{
			servantshopTemplate = &iter->mValue;
			break;
		}
	}
	if (servantshopTemplate == NULL)
	{
		return 0;
	}
	if (refreshTimes > 0)
	{
		refreshTimes = refreshTimes -1;
	}

	if (refreshTimes == 0)
	{
		return servantshopTemplate->timeawardcontentshop;
	}
	else
	{
		UInt32 i = 0;
		for (List<UInt32>::Iter * it =  servantshopTemplate->awardcontentshops.begin(); it != NULL; it = servantshopTemplate->awardcontentshops.next(it))
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


UInt32 ShopManager::getGiftShopID(UInt32 position,UInt32 level,UInt32 vipLevel,UInt32 refreshTimes,UInt32 refresh)
{
	UInt32 nowTime = TimeUtil::getTimeSec();

	GiftShopTemplate * giftShopTemplate = gGiftShopTable->get(position);
	if (giftShopTemplate == NULL)
	{
		return 0;
	}
	// 	if (giftShopTemplate->canbuyNum == 0)
	// 	{
	// 		return 0;
	// 	}	

	return giftShopTemplate->giftID;
}

UInt32 ShopManager::checkRefreshTime(Guid playerID,UInt32 shopType,UInt32 &refreshNeedTime)
{
	UInt32 lastRefreshTime = 0;
	UInt32 nextRefreshTime = 0;
	UInt32 refreshTime = 0;
	UInt32 refreshTimes = 0;
	UInt32 tmp = TimeUtil::getSecOfToday();
	UInt32 dayBeiginTime =  TimeUtil::getTimeSec() - TimeUtil::getSecOfToday();
	Vector<String> strVector;
	Item item;


	UInt32 nowTime = TimeUtil::getTimeSec();
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);

	getPlayerItem( playerID,shopType, 0,item,refreshTime,refreshTimes);

	if (shopType == MysticalShop)
	{
		MysticalshopTemplate *mysticalshopTemplate;//只是为了找时间，其他用途则要重新赋值
		for ( Map<UInt32, MysticalshopTemplate>::Iter *iter = gMysticalshopTable->mMap.begin();iter != NULL;iter = gMysticalshopTable->mMap.next(iter))
		{
			if (iter->mValue.shopposition == 1)
			{
				mysticalshopTemplate = &iter->mValue;
				break;
			}
		}
		lynxStrSplit(mysticalshopTemplate->refreshTime, ";", strVector, true);
	}
	else if (shopType == ServantShop)
	{
		ServantshopTemplate *servantshopTemplate;//只是为了找时间，其他用途则要重新赋值
		for ( Map<UInt32, ServantshopTemplate>::Iter *iter1 = gServantshopTable->mMap.begin();iter1 != NULL;iter1 = gServantshopTable->mMap.next(iter1))
		{
			if (iter1->mValue.shopposition == 1)
			{
				servantshopTemplate = &iter1->mValue;
				break;
			}
		}
		lynxStrSplit(servantshopTemplate->refreshTime, ";", strVector, true);
	}
	else if (shopType == GiftShop)
	{
		if (refreshTime == 0)
		{
			return 10000;
		}
		else
		{
			return 0;
		}
	}	
	UInt32 getIt = 0;

	for (UInt32 i = 0; i < strVector.size();i++)
	{
		if (nowTime < (dayBeiginTime + atoi(strVector[i].c_str()) * 3600 ) )
		{
			nextRefreshTime = (dayBeiginTime + atoi(strVector[i].c_str()) * 3600 );
			if (i == 0)
			{
				lastRefreshTime = dayBeiginTime - 24*3600 + atoi(strVector[strVector.size() -1].c_str())* 3600;//昨天最后一个
			}
			else
			{
				lastRefreshTime = dayBeiginTime + atoi(strVector[i -1].c_str())* 3600;

			}
			getIt = 1;
			break;			
		}		
	}
	if (getIt == 0)
	{
		nextRefreshTime = (dayBeiginTime +  (24+atoi(strVector[0].c_str())) * 3600 );//明天第一次
		lastRefreshTime = dayBeiginTime  + atoi(strVector[strVector.size() -1].c_str())* 3600;//今天最后一次刷新
	}
	refreshNeedTime = nextRefreshTime - nowTime;

	if(refreshTime < lastRefreshTime )
	{
		return 10000;		
	}

	return refreshTimes;
}


void ShopManager::getShopContentByID(UInt32 itemID,List<Goods> &ItemList)
{
	List<Content> contentList;
	UInt32 allWeight1 = 0;
	UInt32 weight1 = 0;
	UInt32 weight2 = 0;
	UInt32 randomNum =0;
	UInt32 finished = 0;
	Goods goods;


	for (List<AwardContentTemplate>::Iter *iter = gAwardContentShopTable->mContentList.begin();iter!= NULL;iter = gAwardContentShopTable->mContentList.next(iter))
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

UInt32 ShopManager::setShopItems(Guid playerID, UInt32 type,UInt32 refreshTimes, UInt32 refreshTime, bool needSave =false)
{

	ShopItems shopItems;
	UInt32 needInsert = 0;
	PersistShopSaveReq req;

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	PlayerShopData playerShopData = player->getPlayerShopData();

	shopItems = getShopItems(playerID,type);

	if (shopItems.shopType == 0)
	{
		needInsert = 1;		
	}

	shopItems.shopType = type;
	shopItems.refreshTimes = refreshTimes;
	shopItems.refreshTime = refreshTime;

	if (needInsert == 1)
	{
		playerShopData.m_ShopItems.insertTail(shopItems);		
	}

	for(List<ShopItems>::Iter * iter = playerShopData.m_ShopItems.begin();iter != NULL;iter = playerShopData.m_ShopItems.next(iter) )
	{
		if (iter->mValue.shopType == type)
		{
			iter->mValue = shopItems;
			break;
		}
	}

	player->setPlayerShopData(playerShopData);

	req.shopType = type;
	req.refreshTimes = refreshTimes;
	req.playerID = playerID;
	req.refreshTime = refreshTime;
	PersistSystem::getSingleton().postThreadMsg(req, 0);
	return 0;
}

UInt32 ShopManager::setShopItem(Guid playerID, UInt32 type,UInt32 rank, Item item, bool needSave =false)
{

	UInt32 flag =5;
	PersistShopItemSaveReq req;
	ShopItems shopItems;


	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	PlayerShopData playerShopData = player->getPlayerShopData();

	shopItems = getShopItems(playerID,type);


	if(shopItems.shopType == 0)
	{
		shopItems.shopType = type;
		shopItems.items.insertTail(item);
		playerShopData.m_ShopItems.insertTail(shopItems);
		flag = 1;
	}
	else
	{
		for(List<Item>::Iter * it = shopItems.items.begin();it != NULL;it = shopItems.items.next(it))
		{
			if (it->mValue.position == rank)
			{
				it->mValue = item;
				flag = 3;
				break;
			}
		}
		if (flag != 3)
		{
			flag = 2;
			shopItems.items.insertTail(item);
		}
	}

	if (flag < 4 && needSave == true)
	{
		req.shopType = type;
		req.position = rank;
		req.playerID = playerID;		
		req.buyTime = item.buyTime;
		req.buyTimes = item.buyTimes;
		req.resType = item.goods.resourcestype;
		req.subType = item.goods.subtype;
		req.num = item.goods.num;
		PersistSystem::getSingleton().postThreadMsg(req, 0);
	}
	for(List<ShopItems>::Iter * iter = playerShopData.m_ShopItems.begin();iter != NULL;iter = playerShopData.m_ShopItems.next(iter) )
	{
		if (iter->mValue.shopType == type)
		{
			iter->mValue = shopItems;
			break;
		}
	}
	player->setPlayerShopData(playerShopData);	

	return flag;
}



ShopItems ShopManager::getShopItems(Guid playerID, UInt32 type)
{
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	PlayerShopData playerShopData = player->getPlayerShopData();

	for (List<ShopItems>::Iter *iter =  playerShopData.m_ShopItems.begin(); iter != NULL;iter = playerShopData.m_ShopItems.next(iter))
	{

		if (iter->mValue.shopType == type)
		{
			return iter->mValue;
		}
	}
	ShopItems shopItems;
	return shopItems;
}

Item ShopManager::getShopItem(Guid playerID,UInt32 type,UInt32 rank)
{

	Item item;
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	PlayerShopData playerShopData = player->getPlayerShopData();

	for (List<ShopItems>::Iter *iter =  playerShopData.m_ShopItems.begin(); iter != NULL;iter = playerShopData.m_ShopItems.next(iter))
	{
		if (iter->mValue.shopType == type)
		{
			for(List<Item>::Iter * it = iter->mValue.items.begin();it != NULL;it = iter->mValue.items.next(it))
			{
				if (it->mValue.position == rank)
				{
					return it->mValue;
					break;
				}
			}
			break;
		}
	}
	return item;
}



void ShopManager::onCoinBuyReq(const  ConnId& connId, CoinBuyReq & msg)
{
	UInt32 refreshTimes = 0;
	UInt32 maxBuyTimes = 0;
	int nowBuyTimes = 0;
	UInt32 price = 0;
	UInt32 num = 0;
	UInt32 buyTimes = 0;
	UInt32 cost = 0;
	UInt32 coin = 0;
	int maxCanBuyTimes = 0;
	//wwc
	UInt32 dailybuytimes = 0;
	Goods goods;
	List<Goods> itemList;	
	CoinBuyResp resp;

	msg.convertJsonToData(msg.jsonStr);
	resp.reqType = msg.reqType;

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		return;
	}

	PlayerDailyResetData dailyResetData= player->getPlayerDailyResetData();

	maxBuyTimes = ShopManager::getSingleton().getCoinBuyMaxTimes(player->getVipLevel());

	GainWealth gainWealth = GlobalVarManager::getSingleton().getGainWealth();

	if (msg.reqType == 1 ||msg.reqType == 2)
	{
		maxCanBuyTimes = msg.num;
		if((maxCanBuyTimes + dailyResetData.m_nCoinBuyTimes + dailyResetData.m_nCoinFreeBuyTimes) >= maxBuyTimes)
		{
			maxCanBuyTimes = maxBuyTimes - dailyResetData.m_nCoinBuyTimes - dailyResetData.m_nCoinFreeBuyTimes;
		}
	
		for (UInt32 i = 1;i <= maxCanBuyTimes;i++)
		{
			nowBuyTimes = dailyResetData.m_nCoinBuyTimes + dailyResetData.m_nCoinFreeBuyTimes + i;
			
			FireConfirmManager::getSingleton().getBuyCoinPrice(player->getPlayerGuid(),nowBuyTimes,price,num);
			if (msg.reqType == 2)
			{
				if (dailyResetData.m_nCoinFreeBuyTimes >= gainWealth.freeTimes)
				{
					break;
				}
				dailyResetData.m_nCoinFreeBuyTimes ++;
				price = 0;
			}
			else
			{				
				buyTimes ++;
			}
			dailybuytimes++;
			if(player->getPlayerGold() < cost )
			{
				break;
			}			
			cost += price;
			coin += num;
			ShopManager::getSingleton().getPremiumsContent(nowBuyTimes,itemList);
		}

		if (itemList.size() != 0)
		{
			Award award;
			award.award = itemList;
			resp.awards.insertTail(award);			
		}

		dailyResetData.m_nCoinBuyTimes += buyTimes;

		goods.resourcestype = AWARD_BASE;
		goods.subtype = AWARD_BASE_GOLD;
		goods.num -= cost;		
		itemList.insertTail(goods);

		goods.resourcestype = AWARD_BASE;
		goods.subtype = AWARD_BASE_COIN;
		goods.num = coin;		
		itemList.insertTail(goods);

		GiftManager::getSingleton().combineSame(itemList);
		GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,itemList);
		GiftManager::getSingleton().PlayerItemChangeResult(player->getPlayerGuid(),0,itemList);
		resp.ends = itemList;

		player->setPlayerDailyResetData(dailyResetData);	
		player->getPersistManager().setDirtyBit(DAILYRESETBIT);

	}

	resp.freeTimes = gainWealth.freeTimes - dailyResetData.m_nCoinFreeBuyTimes;
	resp.buyNum = dailyResetData.m_nCoinBuyTimes + dailyResetData.m_nCoinFreeBuyTimes;
	resp.maxBuyTimes = maxBuyTimes;

	if (msg.needSend == 1)
	{
		std::string jsonStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,COIN_BUY_RESP,jsonStr);
	}	

	//每日任务拜财神打点 wwc
	player->getAchieveManager().updateDailyTaskData(DLYBAICAISHEN, dailybuytimes );
}


void ShopManager::getPremiumsContent(UInt32 nowBuyTimes,List<Goods>& itemList)
{
	Goods goods;
	Vector<String> strVector;
	Vector<String> strVector1;
	PremiumsTemplate *premiumsTemplate = NULL;

	for (Map<UInt32, PremiumsTemplate>::Iter*iter = gPremiumsTable->mMap.begin();iter!= NULL;iter = gPremiumsTable->mMap.next(iter))
	{
		if (iter->mValue.type == nowBuyTimes)
		{
			premiumsTemplate = &iter->mValue;
		}
	}
	if (premiumsTemplate == NULL)
	{
		return;
	}

	lynxStrSplit(premiumsTemplate->contentID, ";", strVector, true);

	for (UInt32 i = 0; i < strVector.size();i++)
	{
		lynxStrSplit(strVector[i], "_", strVector1, true);		
		if (strVector1.size() != 3)
		{
			break;
		}
		goods.resourcestype =  atoi(strVector1[0].c_str());
		goods.subtype =  atoi(strVector1[1].c_str());
		goods.num =  atoi(strVector1[2].c_str());
		itemList.insertTail(goods);
	}
};


UInt32  ShopManager::getCoinBuyMaxTimes(UInt32 vipLevel)
{
	UInt32 maxBuyTimes = 0;
	for(List<GainWealthTemplate>::Iter *iter = gGainWealthTable->mMap.begin();iter != NULL;iter = gGainWealthTable->mMap.next(iter) )
	{
		if (iter->mValue.VIPlevel == vipLevel)
		{
			if (iter->mValue.times > maxBuyTimes)
			{
				maxBuyTimes = iter->mValue.times;
			}
		}
	}
	return maxBuyTimes;
}

UInt32  ShopManager::getStrengthBuyMaxTimes(UInt32 vipLevel)
{
	UInt32 maxBuyTimes = 0;
	for(List<GainStrengthTemplate>::Iter *iter = gGainStrengthTable->mMap.begin();iter != NULL;iter = gGainStrengthTable->mMap.next(iter) )
	{
		if (iter->mValue.viplevel == vipLevel)
		{
			if (iter->mValue.buytimes > maxBuyTimes)
			{
				maxBuyTimes = iter->mValue.buytimes;
			}
		}
	}
	return maxBuyTimes;
}