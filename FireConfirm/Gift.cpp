#include "Gift.h"
#include "../LogicSystem.h"
#include "StringConverter.h"
#include "../PlatformLib/SerializeType.h"
#include "../PersistSystem.h"
#include "../CommonLib/CopyFinishTime.h"
#include "../PassportSystem.h"
#include "../ItemManager.h"

using namespace Lynx;


void GiftManager::getContentByID(UInt32 itemID,List<Goods> &ItemList)
{
	List<Content> contentList;
	UInt32 allWeight1 = 0;
	UInt32 weight1 = 0;
	UInt32 weight2 = 0;
	UInt32 randomNum =0;
	UInt32 finished = 0;
	Goods goods;


	for (List<AwardContentTemplate>::Iter *iter = gAwardContentTable->mContentList.begin();iter!= NULL;iter = gAwardContentTable->mContentList.next(iter))
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


void GiftManager::getAwardCardByID(UInt32 itemID,List<Goods> &ItemList)
{
	UInt32 allWeight1 = 0;
	UInt32 weight1 = 0;
	UInt32 weight2 = 0;	
	List<Resource> resourcesList;

	for (List<AwardCardTemplate>::Iter *iter = gAwardCardTable->mCardList.begin();iter!= NULL;iter = gAwardCardTable->mCardList.next(iter))
	{
		if (iter->mValue.id == itemID)
		{
			resourcesList = iter->mValue.resourcesList;
			break;			
		}
	}
	ChoiseNFromWeigthList(resourcesList.size(), resourcesList, ItemList);

}


void GiftManager::getAwardByID(UInt32 itemID,UInt32 num,List<Goods> &ItemList)
{
	UInt32 allWeight1 = 0;
	UInt32 weight1 = 0;
	UInt32 weight2 = 0;	
	List<AwardItem> AwardItems;
	UInt32 randomNum;
	UInt32 finished = 0;
	UInt32 count =0;

	for (List<AwardTemplate>::Iter *iter = gAwardTable->mList.begin();iter!= NULL;iter = gAwardTable->mList.next(iter))
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


UInt32 GiftManager::PlayerItemChangeResult(Guid playerID,UInt32 reFlashType, List<Goods> &itemList)
{
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	List<UInt64> newItemUids;
	List<Goods> goodsList;
	List<Goods> resultList;
	Goods goods;

	for(List<Goods>::Iter *iter = itemList.begin();iter != NULL ;iter =itemList.next(iter))
	{
		if (iter->mValue.resourcestype == AWARD_BASE)
		{
			if (iter->mValue.subtype == AWARD_BASE_COIN)
			{
				goods.resourcestype =AWARD_BASE;
				goods.subtype =AWARD_BASE_COIN;
				goods.num = player->getPlayerCoin();
				resultList.insertTail(goods);
				continue;
			}
			else if (iter->mValue.subtype == AWARD_BASE_GOLD)
			{
				goods.resourcestype =AWARD_BASE;
				goods.subtype =AWARD_BASE_GOLD;
				goods.num = player->getPlayerGold();
				resultList.insertTail(goods);
				continue;
			}
			else if (iter->mValue.subtype == AWARD_BASE_STRENGTH )
			{				
				goods.resourcestype =AWARD_BASE;
				goods.subtype =AWARD_BASE_STRENGTH;
				goods.num = FireConfirmManager::getSingleton().refreshgetStrength(player->getConnId());			
				resultList.insertTail(goods);
				continue;
			}
			else if (iter->mValue.subtype == AWARD_BASE_EXP)
			{
				goods.resourcestype =AWARD_BASE;
				goods.subtype =AWARD_BASE_EXP;
				goods.num = player->getPlayerExp();
				resultList.insertTail(goods);
				continue;
			}
			else if (iter->mValue.subtype == AWARD_BASE_COMPETITIVE_POINT)
			{
				goods.resourcestype =AWARD_BASE;
				goods.subtype =AWARD_BASE_COMPETITIVE_POINT;
// 				goods.num = player->getPlayerPower();
				resultList.insertTail(goods);
				continue;
			}
			else if (iter->mValue.subtype == AWARD_BASE_RHYME_SOUL)
			{
				goods.resourcestype =AWARD_BASE;
				goods.subtype =AWARD_BASE_RHYME_SOUL;
				goods.num = player->getPlayerRhyme();				
				resultList.insertTail(goods);
				continue;
			}
			else if (iter->mValue.subtype == AWARD_BASE_SCORE)
			{
				goods.resourcestype =AWARD_BASE;
				goods.subtype =AWARD_BASE_SCORE;
				goods.num = player->getPlayerScore();				
				resultList.insertTail(goods);
				continue;
			}			
		}
		else if (iter->mValue.resourcestype == AWARD_GRASS )
		{
			goods.resourcestype =AWARD_GRASS;
			goods.subtype = iter->mValue.subtype;
			goods.num = player->getRhymeManager().getRhymeGrass(iter->mValue.subtype);
			resultList.insertTail(goods);	
			continue;
		}
		else if (iter->mValue.resourcestype == AWARD_WELFAREMARKET )
		{
			continue;
		}
		else if (iter->mValue.resourcestype == AWARD_FOOD )
		{
			continue;
		}

		else if (iter->mValue.resourcestype == AWARD_HOARSTONEPIECE )
		{
			goods.resourcestype =AWARD_HOARSTONEPIECE;
			goods.subtype = iter->mValue.subtype;
			goods.num = player->getHoarStoneManager().getHoarStonePiece(iter->mValue.subtype);
			resultList.insertTail(goods);	
			continue;
		}
		else if (iter->mValue.resourcestype == AWARD_SERVANTMATERIAL )
		{
			goods.resourcestype =AWARD_SERVANTMATERIAL;
			goods.subtype = iter->mValue.subtype;
			goods.num = player->getServantManager().getServantFood(iter->mValue.subtype);
			resultList.insertTail(goods);	
			continue;
		}
		else if (iter->mValue.resourcestype == AWARD_SERVANT )
		{
			continue;
		}
		else if (iter->mValue.resourcestype == AWARD_SERVANTPIECE )
		{
			goods.resourcestype =AWARD_SERVANTPIECE;
			goods.subtype = iter->mValue.subtype;
			goods.num = player->getServantManager().getServantPieceCount(iter->mValue.subtype);
			resultList.insertTail(goods);	
			continue;
		}
		else if (iter->mValue.resourcestype == AWARD_SERVANTTREASURE )
		{
			continue;
		}
		else if (iter->mValue.resourcestype == AWARD_SERVANTSWITCH )
		{
			goods.resourcestype =AWARD_SERVANTSWITCH;
			goods.subtype = iter->mValue.subtype;
			goods.num = player->getServantManager().getServantSwitch();
			resultList.insertTail(goods);	
			continue;
		}
		else if (iter->mValue.resourcestype == AWARD_ENHANCEMATERIAL )
		{
			resultList.insertTail(iter->mValue);
			continue;
		}
		else if (iter->mValue.resourcestype == AWARD_GEM )
		{
			goods.resourcestype =AWARD_GEM;
			goods.subtype = iter->mValue.subtype;
			goods.num = player->getGemManager().getGems(iter->mValue.subtype);
			resultList.insertTail(goods);	
			continue;
		}
		else if (iter->mValue.resourcestype == AWARD_JEWELRY )
		{
			continue;
		}
		else if (iter->mValue.resourcestype == AWARD_FASHION )
		{
			continue;
		}
		else if (iter->mValue.resourcestype == 15)
		{
			continue;
		}
		else if (iter->mValue.resourcestype == AWARD_HOARSTONEEQUIP )
		{
			continue;
		}
		else if (iter->mValue.resourcestype == AWARD_STAGEDATA)
		{
			// 			StageManager::getSingleton().addStageData(player->getPlayerGuid(),iter->mValue.subtype,iter->mValue.num);
			continue;
		}
		else if (iter->mValue.resourcestype == AWARD_COUNTER)
		{
			// 			updateCounter(player->getPlayerGuid(),iter->mValue.subtype,iter->mValue.num);
			continue;
		}
		else if (iter->mValue.resourcestype == AWARD_REWARDCOUNTER)
		{
			// 			updateRewardCounter(player->getPlayerGuid(),iter->mValue.subtype,iter->mValue.num);
			continue;
		}
		else if (iter->mValue.resourcestype == AWARD_FOODDATA)
		{
			// 			updateFood(player->getPlayerGuid(),iter->mValue.subtype,iter->mValue.num);//美食屋存档
			continue;
		}


	}

	PlayerFireConfirmData mFireConfirmData =  player->GetFireConfirmData();

	resultList += mFireConfirmData.m_SendList;//等级

	itemList.clear();
	itemList = resultList;

	return 1;
}

UInt32 GiftManager::addToPlayer(Guid playerID,UInt32 reFlashType, List<Goods> itemList)
{
	List<AwardData>  awardList;
	List<AwardData>  costList;

	for(List<Goods>::Iter *iter = itemList.begin();iter != NULL ;iter =itemList.next(iter))
	{
		AwardData awardData;
		if (iter->mValue.num >= 0)
		{
			awardData.resType = iter->mValue.resourcestype;
			awardData.resID = iter->mValue.subtype;
			awardData.resCount = iter->mValue.num;
			awardList.insertTail(awardData);
		}
		else
		{
			awardData.resType = iter->mValue.resourcestype;
			awardData.resID = iter->mValue.subtype;
			awardData.resCount = -iter->mValue.num;
			costList.insertTail(awardData);
		}

	}

	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerID);

	player->getAllItemManager().costMaterialsList(costList);

	player->getAllItemManager().addAwardGetSendJson(awardList);

	return 1;
}



void GiftManager::splitKind( List<Goods> &goodsList, List<Goods> &kindgoodsList,UInt32 kind)
{
	for( List<Goods>::Iter * iter = goodsList.begin();iter != NULL;iter = goodsList.next(iter))
	{
		if (iter->mValue.resourcestype == AWARD_BASE || iter->mValue.resourcestype == AWARD_GRASS/* ||iter->mValue.resourcestype == AWARD_HOARSTONEPIECE*/			
			||(iter->mValue.resourcestype == AWARD_SERVANTMATERIAL && iter->mValue.subtype == 6) /*|| iter->mValue.resourcestype == AWARD_SERVANTPIECE */
			||iter->mValue.resourcestype == AWARD_SERVANTSWITCH
			)
		{
			kindgoodsList.insertTail(iter->mValue);
			goodsList.erase(iter);
		}

	}

}

void GiftManager::combineSame( List<Goods> &goodsList)
{

	for( List<Goods>::Iter * iter = goodsList.begin();iter != NULL;iter = goodsList.next(iter))
	{
		for( List<Goods>::Iter * iter1 = goodsList.next(iter);iter1 != NULL;iter1 = goodsList.next(iter1))
		{
			if (iter->mValue.resourcestype == iter1->mValue.resourcestype && iter->mValue.subtype == iter1->mValue.subtype)
			{
				iter->mValue.num += iter1->mValue.num;
				goodsList.erase(iter1);				
			}
		}
	}
}


void GiftManager::sendToPlayer( Guid playerID,UInt32 type,List<Goods> itemList,List<UInt64> newItemUids)
{
	PropertysChange resp;
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerID);

	resp.type = type;
	resp.goodsList = itemList;
	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( player->getConnId(),PROPERTYS_CHAGE_RESP,jsonStr);
	sendItemListChange( playerID, newItemUids);
}

UInt32 GiftManager::subFromPlayer(Guid playerID,Goods goods)
{
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerID);

	return 1;
}

void GiftManager::getChapterAward(const  ConnId& connId,List<AwardMonsterDamage> awardMonsterList)
{

	UInt32 countNum = 0;
	UInt32 flag = 0;
	UInt32 firstContentID = 0;
	UInt32 contentID = 0;
	ItemTemplate mItem;
	UInt32 bigID;
	UInt32 chapterID;
	UInt32 firstFinish =0 ;
	UInt32 contentCardID = 0;
	UInt32 isFull = 0;

	bool bFlag = false;
	Goods goods;
	bool isCounted = false;
	Award tmpAward;
	Card tmpCard;
	List<Goods> fixedList;
	List<Goods> itemList;
	List<Goods> tmpItemList;
	List<Goods> awardCardList;
	List<Goods> monsterAwardList;
	List<Goods> tmpMonsterAwardList;
	List<KeyValue> counter;
	List<Counter> counterList;
	List<StepContent> stepContent;
	List<AwardMonsterDamage> awardMonsterDropList;
	PlayerBaseData baseData;
	PlayerFireConfirmData fireConfirmData;



	PlayerChapterList chapterList;

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);

	fireConfirmData = player->GetFireConfirmData();


	if (fireConfirmData.m_JewlryCanAddNum <= 0)
	{
		isFull = 1;
	}

	chapterID = fireConfirmData.m_CopyID;
	StageTemplate *stageTemplate = gStageTable->get(chapterID);
	// 	stageTemplate->mType STAGE_TWELVEPALACE


	bigID = StageManager::getSingleton().getChapterID(chapterID);
	chapterList = player->getChapterList();

	for (List<PlayerChapterData>::Iter * iter = chapterList.m_listChapterData.begin();iter != NULL;iter = chapterList.m_listChapterData.next(iter))
	{
		if (iter->mValue.m_nChapterId == bigID)
		{
			for(List<StageData>::Iter *iter1 = iter->mValue.m_listStageDatas.begin();iter1 != NULL; iter1 = iter->mValue.m_listStageDatas.next(iter1))
			{
				if (iter1->mValue.m_nStageID == chapterID)
				{
					if (iter1->mValue.m_nLastChallengTime > 0)
					{
						firstFinish = 1;
					}
				}
			}
		}
	}

	for(Map<UInt64, StageTemplate>::Iter *item = gStageTable->mMap.begin();item != NULL ; item = gStageTable->mMap.next(item))//地图掉落
	{
		if (chapterID == item->mKey)
		{
			if (firstFinish == 0)
			{
				firstContentID = lynxAtoi<UInt32>(item->mValue.mFirstAwardID.c_str());
			}		

			contentID = lynxAtoi<UInt32>(item->mValue.mAwardID.c_str());

			contentCardID = item->mValue.awardCardID;

			if (stageTemplate->mType == STAGE_TWELVEPALACE)
			{
				goods.resourcestype =  AWARD_TWELVEPALACE_STAGEDATA;
			}
			else
			{
				goods.resourcestype = AWARD_STAGEDATA;
			}
			goods.subtype = chapterID;
			goods.num = 1;
			itemList.insertTail(goods);


			goods.resourcestype = AWARD_BASE;
			goods.subtype = AWARD_BASE_COIN;
			if (item->mValue.mAwardGold != 0)
			{
				goods.num = item->mValue.mAwardGold * fireConfirmData.m_Gain/10000;
				fixedList.insertTail(goods);
			}


			goods.resourcestype = AWARD_BASE;
			goods.subtype = AWARD_BASE_EXP;
			if (item->mValue.mAwardExp != 0)
			{
				goods.num = item->mValue.mAwardExp * fireConfirmData.m_Gain/10000;
				fixedList.insertTail(goods);
			}

			Vector<String> strVector;
			lynxStrSplit(item->mValue.mAwardOther, ";", strVector, true);
			if (strVector.size()==3)
			{
				goods.resourcestype =atoi(strVector[0].c_str()) ;
				goods.subtype = atoi(strVector[1].c_str());
				goods.num = atoi(strVector[2].c_str());
				if (atoi(strVector[2].c_str()) != 0)
				{
					goods.num = atoi(strVector[2].c_str()) * fireConfirmData.m_Gain/10000;
					fixedList.insertTail(goods);
				}
			}


			strVector.clear();//哥布林掉落
			lynxStrSplit(item->mValue.monsterAward, ";", strVector, true);
			for(UInt32 i=0;i<strVector.size();i++)
			{
				Vector<String> subStrVector;
				lynxStrSplit(strVector[i], "_", subStrVector, true);
				if (subStrVector.size()!=3)
				{
					break;
				}
				for(List<AwardMonsterDamage>::Iter *it = awardMonsterList.begin();it!=NULL;it = awardMonsterList.next(it))
				{
					if (atoi(subStrVector[0].c_str()) == it->mValue.ID && atoi(subStrVector[1].c_str()) == it->mValue.damageType)
					{						
						getAwardByID(it->mValue.ID,it->mValue.times, tmpMonsterAwardList);
					}
				}	
			}
			break;
		}		
	}

	fireConfirmData.m_FixedList += fixedList;

	getAwardByID(contentID,0, tmpItemList);	
	for(List<Goods>::Iter *it = tmpItemList.begin();it!= NULL;it = tmpItemList.next(it))
	{
		getContentByID(it->mValue.subtype,itemList);
	}

	checkJewelry(itemList,fireConfirmData,isFull);
	tmpAward.award = itemList;
	fireConfirmData.m_AwardsList.insertTail(tmpAward);



	//顺序不能错，掉落里有了个stageid
	tmpItemList.clear();
	itemList.clear();
	getAwardByID(firstContentID,0, tmpItemList);	
	for(List<Goods>::Iter *it = tmpItemList.begin();it!= NULL;it = tmpItemList.next(it))
	{
		getContentByID(it->mValue.subtype,itemList);
	}	
	checkJewelry(itemList,fireConfirmData,isFull);

	tmpAward.award = itemList;
	if (itemList.size() != 0)
	{
		fireConfirmData.m_FirstAwardsList.insertTail(tmpAward);
	}

	getAwardCardByID(contentCardID, awardCardList);

	tmpCard.card = awardCardList;

	fireConfirmData.m_CardsList.insertTail(tmpCard);

	for(List<Goods>::Iter *it = tmpMonsterAwardList.begin();it!= NULL;it = tmpMonsterAwardList.next(it))
	{
		getContentByID(it->mValue.subtype,monsterAwardList);
	}
	fireConfirmData.m_MonsterAwardList = monsterAwardList;

	player->SetFireConfirmData(fireConfirmData);
}

void GiftManager::checkJewelry(List<Goods> &itemList,PlayerFireConfirmData &fireConfirmData,UInt32 isFull)
{
	Goods goods;
	for(List<Goods>::Iter *it = itemList.begin();it!= NULL;it = itemList.next(it))
	{
		if (it->mValue.resourcestype == AWARD_JEWELRY)
		{
			if (fireConfirmData.m_JewlryCanAddNum <= 0 && isFull ==1)
			{
				NewJewelryTemplate * newJewelryTemp = NEWJEWELRY_TABLE().get(it->mValue.subtype);
				if(newJewelryTemp == NULL)
				{
					continue;
				}

				it->mValue.resourcestype = AWARD_JEWELRY_SELL;//显示为卖出

				goods.resourcestype = AWARD_BASE;
				goods.subtype = AWARD_BASE_COIN;
				goods.resourcestype = newJewelryTemp->mPrice;

				fireConfirmData.m_FixedList.insertTail(goods);
			}
			else
			{
				fireConfirmData.m_JewlryCanAddNum -= it->mValue.num;
			}
		}
	}

}

void GiftManager::onOpenGiftReq(const ConnId& connId ,GiftReq & msg )
{

	bool bFlag = false;
	Goods goods;
	UInt32 isCounted = 0;
	UInt32 countNum = 1;
	UInt32 flag = 0;
	List<Goods> ItemList;
	List<KeyValue> counter;
	List<BoxTemplate> counterList;
	List<KeyValue> stepContent;	
	UInt32 contentID ;
	mAwardList mList = gAwardTable->mList;
	ItemTemplate mItem;
	Guid uID ;
	KeyValue keyValue;

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	msg.convertJsonToData(msg.strReceive);	

	uID = msg.giftID;

	contentID = msg.giftID;

	counterList = gBoxCounterTable->mContentList;
	counter = player->GetBoxCounter();
	for (List<BoxTemplate>::Iter * iter = counterList.begin();iter != NULL;iter= counterList.next(iter))//需要计数
	{
		if (msg.giftID == iter->mValue.id)
		{
			isCounted = 1;
			for(List<KeyValue>::Iter * it = counter.begin();it != NULL;it = counter.next(it))//已经计数过
			{
				if (msg.giftID == it->mValue.key)
				{
					isCounted = 2;
					isCounted = true;
					countNum = ++(it->mValue.value) ;
					stepContent = iter->mValue.ContentList;

					for (List<KeyValue>::Iter * it2 = stepContent.end();it2 != NULL; it2 = stepContent.prev(it2))
					{
						if (it->mValue.key == it2->mValue.key && it->mValue.value >0)
						{
							contentID = it2->mValue.value;
							break;
						}
					}
				}
			}
		}	
	}	

	GiftManager::getSingleton().getContentByID( contentID,ItemList);


	if (flag ==0)//这时候才能判断是否加，发送的是失败还是成功
	{

		if (isCounted == 1)
		{
			keyValue.key = msg.giftID;
			keyValue.value = countNum;
			counter.insertTail(keyValue);		
			player->setBoxCounter(counter);
		}		
		GiftManager::getSingleton().updateCounter(player->getPlayerGuid(),msg.giftID,countNum);

		GiftManager::getSingleton().combineSame(ItemList);
		GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_GIFT,ItemList);
	}


}

void GiftManager::getGift(const ConnId& connId ,UInt32 id,ChapterEndResp &resp)
{

	bool bFlag = false;
	Goods goods;
	UInt32 isCounted = 0;
	UInt32 countNum = 1;
	UInt32 flag = 0;
	List<Goods> ItemList;
	List<KeyValue> counter;
	List<BoxTemplate> counterList;
	List<KeyValue> stepContent;	
	UInt32 contentID ;
	mAwardList mList = gAwardTable->mList;
	ItemTemplate mItem;
	KeyValue keyValue;

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);


	contentID = id;

	counterList = gBoxCounterTable->mContentList;
	counter = player->GetBoxCounter();
	for (List<BoxTemplate>::Iter * iter = counterList.begin();iter != NULL;iter= counterList.next(iter))//需要计数
	{
		if (id == iter->mValue.id)
		{
			isCounted = 1;
			for(List<KeyValue>::Iter * it = counter.begin();it != NULL;it = counter.next(it))//已经计数过
			{
				if (id == it->mValue.key)
				{
					isCounted = 2;
					countNum = ++(it->mValue.value) ;
					stepContent = iter->mValue.ContentList;

					for (List<KeyValue>::Iter * it2 = stepContent.end();it2 != NULL; it2 = stepContent.prev(it2))
					{
						if (it->mValue.key == it2->mValue.key && it->mValue.value >0)
						{
							contentID = it2->mValue.value;
							break;
						}
					}
				}
			}
		}	
	}	

	GiftManager::getSingleton().getContentByID(contentID,ItemList);


	if (flag ==0)//这时候才能判断是否加，发送的是失败还是成功
	{

		if (isCounted == 1)
		{
			keyValue.key = id;
			keyValue.value = countNum;
			counter.insertTail(keyValue);		
			player->setBoxCounter(counter);
		}		
		GiftManager::getSingleton().updateCounter(player->getPlayerGuid(),id,countNum);
	}

	resp.ends += ItemList;	
}
UInt32 GiftManager::modifyCounter(const ConnId& connId,UInt32 counterType,UInt32 counterID,Goods &goods)
{

	// 	Goods goods;
	List<KeyValue> counter;
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	counter = player->GetBoxCounter();
	if (counterType == 1)
	{
		for(List<KeyValue>::Iter *iter =  counter.begin();iter != NULL;iter = counter.next(iter))
		{
			if (iter->mValue.key == 500000)
			{
				if (iter->mValue.value == 100)
				{
					goods.resourcestype = 22;
					goods.subtype = 10;
					goods.num = 1;
					break;
				}
			}
		}
	}
	return 1;

}

void GiftManager::onOpenBoxReq(const ConnId& connId ,BoxReq & msg )
{
	bool bFlag = false;
	GiftResp resp;	
	Goods goods;
	UInt32 isCounted = 0;
	UInt32 countNum = 1;
	UInt32 flag = 0;
	List<Goods> ItemList;
	List<KeyValue> counter;
	List<BoxTemplate> counterList;
	List<KeyValue> stepContent;	
	UInt32 contentID ;
	mAwardList mList = gAwardTable->mList;
	ItemTemplate mItem;
	const ItemData *item ;
	Guid uID = msg.boxID;
	KeyValue keyValue;

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	msg.convertJsonToData(msg.strReceive);	

	item =  player->getItemManager().getItemByGuid(msg.boxID);
	if (item == NULL)
	{
		return;
	}
	msg.boxID = item->m_nItemId;

	counterList = gBoxCounterTable->mContentList;
	counter = player->GetBoxCounter();
	resp.ID = msg.boxID;
	contentID = msg.boxID;
	for (List<BoxTemplate>::Iter * iter = counterList.begin();iter != NULL;iter= counterList.next(iter))//需要计数
	{
		if (msg.boxID == iter->mValue.id)
		{
			isCounted =1;
			for(List<KeyValue>::Iter * it = counter.begin();it != NULL;it = counter.next(it))//已经计数过
			{
				if (msg.boxID == it->mValue.key)
				{

					isCounted = 2;
					countNum = ++(it->mValue.value);
					stepContent = iter->mValue.ContentList;

					for (List<KeyValue>::Iter * it2 = stepContent.end();it2 != NULL; it2 = stepContent.prev(it2))
					{
						if (it->mValue.key == it2->mValue.key && it->mValue.value >0)
						{
							contentID = it2->mValue.value;
							break;
						}

					}
					break;
				}
				break;
			}
		}	
	}	


	GiftManager::getSingleton().getContentByID( contentID,ItemList);

	if (player->getItemManager().getEmputyNum(1) < ItemList.size())
	{
		flag =2;
	}

	if (flag == 0)
	{
		if(player->getItemManager().delItem(uID,1) != 1)
		{
			flag = 3;
		}
	}	

	if (flag ==0)
	{
		if (isCounted == 1)
		{
			keyValue.key = msg.boxID;
			keyValue.value = countNum;
			counter.insertTail(keyValue);		
			player->setBoxCounter(counter);
		}		
		GiftManager::getSingleton().updateCounter(player->getPlayerGuid(),msg.boxID,countNum);

		goods.resourcestype = 2;
		goods.subtype = uID;
		goods.num = -1;
		ItemList.insertTail(goods);
		GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_BOX,ItemList);
		resp.ID = msg.boxID;
		resp.goodsList = ItemList;
	}
	else
	{
		resp.ID =flag;	
		resp.goodsList.clear();
	}	

	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,OPEN_BOX_RESP,jsonStr);
}

//
void GiftManager::onqiyongReq(const  ConnId& connId ,LotteryDrawReq & msg )
{
	LotteryDrawResp resp;
	UInt32 contentID;
	UInt32 maxTimes = 1;
	bool bFlag = false;
	Goods goods;
	bool isCounted = false;
	UInt32 countNum = 1;
	UInt32 flag = 0;
	List<Goods> ItemList;
	List<KeyValue> counter;
	List<LotteryTemplate> counterList;
	List<TypeNumValue> stepContent;
	KeyValue keyValue;
	Player * player = LogicSystem::getSingleton().getPlayerByConnId(connId);

	msg.convertJsonToData(msg.strReceive);

	counterList =gLotteryTable ->mContentList;
	counter = player->GetBoxCounter();

	resp.drawType = msg.drawType;
	contentID = 0;
	UInt32 rightType = 0;
	UInt32 counterID;

	if(msg.drawType == 1)
	{
		counterID = LOTTERY_TYPE_TO_ID1;
	}
	if(msg.drawType == 2)
	{
		counterID = LOTTERY_TYPE_TO_ID2;
		maxTimes = LOTTERY_MAX_DRAW;
	}
	if(msg.drawType == 3)
	{
		counterID = LOTTERY_TYPE_TO_ID3;
	}

	for(int i =0;i< maxTimes;i++)
	{
		for (List<LotteryTemplate>::Iter * iter = counterList.begin();iter != NULL;iter= counterList.next(iter))//类型
		{
			if (msg.drawType == iter->mValue.id)
			{
				contentID = iter->mValue.firstAward;
				for(List<KeyValue>::Iter * it = counter.begin();it != NULL;it = counter.next(it))//已经计数过
				{
					if (counterID == it->mValue.key)
					{
						isCounted = true;
						countNum = ++(it->mValue.value) ;
						if (msg.drawType ==3)
						{
							if (it->mValue.value -  TimeUtil::getTimeSec() < LOTTERY_INTERVAL)
							{
								resp.drawType = 4;
								std::string jsonStr = resp.convertDataToJson();
								NetworkSystem::getSingleton().sender( connId,OPEN_LOTTERY_DRAW_RESP,jsonStr);
								return ;
							}
							countNum =  TimeUtil::getTimeSec();
						}

						stepContent = iter->mValue.ContentList;

						for (List<TypeNumValue>::Iter * it2 = stepContent.end();it2 != NULL; it2 = stepContent.prev(it2))
						{
							if (it2->mValue.uType ==1)
							{
								if (it2->mValue.num == it->mValue.value)
								{
									contentID = it2->mValue.uValue;
									rightType =1;
									break;
								}
							}							
						}
						if (rightType >0)
						{
							break;
						}
						for (List<TypeNumValue>::Iter * it2 = stepContent.end();it2 != NULL; it2 = stepContent.prev(it2))
						{
							if (it2->mValue.uType ==2)
							{
								if (it2->mValue.num == 0)
								{
									break;
								}
								if ((it->mValue.value % it2->mValue.num  ) ==0)
								{
									contentID = it2->mValue.uValue;
									rightType =2;
									break;
								}
							}
						}					
					}				
				}							
			}	
		}	
		if (msg.drawType == 3)
		{
			countNum = TimeUtil::getTimeSec();
		}
		GiftManager::getSingleton().updateCounter(player->getPlayerGuid(),counterID,countNum);
		keyValue.key = counterID;
		keyValue.value = countNum;
		counter.insertTail(keyValue);		
		player->setBoxCounter(counter);
		GiftManager::getSingleton().getContentByID(contentID, ItemList);	
	}

	resp.goodsList = ItemList;
	GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_LOTTERY,ItemList);

	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,OPEN_LOTTERY_DRAW_RESP,jsonStr);
}
void GiftManager::onRewardLotteryMsgReq(const  ConnId& connId ,RewardLotteryMsgReq & msg )
{

	UInt32 nowTime = TimeUtil::getTimeSec();
	RewardLotteryTemplate rewardLotteryTemplate1;
	RewardLotteryTemplate rewardLotteryTemplate4;
	RewardLotteryMsgResp resp;
	Player * player = LogicSystem::getSingleton().getPlayerByConnId(connId);


	if(player == NULL)
	{
		return;
	}
	PlayerCounterData counterData = player->GetPlayerCounterData();
	
	UInt32 onceTime = 0;
	UInt32 tenTime = 0;
	GiftManager::getSingleton().getLotteryLeftTime(player->getPlayerGuid(),onceTime,tenTime);

	resp.onceLeftTime = onceTime;
	resp.tenLeftTime = tenTime;
	resp.couponOnceNum = counterData.m_RewardLotteryOnceTicket;
	resp.couponTenNum = counterData.m_RewardLotteryTenTicket;

	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,OPEN_LOTTERY_MSG_RESP,jsonStr); 

}

UInt32 GiftManager::getOpenLotteryID()
{
	UInt32 nowTime = TimeUtil::getTimeSec();
	for (Map<UInt32, LotteryActivityTemplate>::Iter*iter = gLotteryActivityTable->mMap.begin();iter != NULL;iter = gLotteryActivityTable->mMap.next(iter))
	{
		if (nowTime >iter->mValue.starttime &&nowTime <= iter->mValue.closetime)
		{
			return iter->mValue.lotteryid;
		}
	}

	//循环一遍没找到就给第一个
	for (Map<UInt32, LotteryActivityTemplate>::Iter*iter = gLotteryActivityTable->mMap.begin();iter != NULL;iter = gLotteryActivityTable->mMap.next(iter))
	{
		if (iter->mValue.id == 1)
		{
			return iter->mValue.lotteryid;
		}		
	}
	return 0;
}

List<UInt32> GiftManager::getRewardLotteryIDs(UInt32 typeID)
{
	List<UInt32> IDList;

	if(typeID == RL_VIP)
	{
		UInt32 id = getOpenLotteryID();
		if (id != 0)
		{
			IDList.insertTail(id);
		}
	}
	else
	{
		for( Map<UInt32, RewardLotteryTemplate>::Iter * iter = gRewardLotteryTable->mMap.begin();iter != NULL;iter = gRewardLotteryTable->mMap.next(iter))
		{
			if (iter->mValue.type == typeID)
			{			
				IDList.insertTail(iter->mKey);			
			}
		}
	}

	return IDList;
}

UInt32 GiftManager::getRewardLotteryAwardID(Guid playerID,UInt32 id,UInt32 &awardID)
{

	UInt32 getIt = 0;
	UInt32 maxKey = 0;
	UInt32 nowTime = TimeUtil::getTimeSec();	
	Player * player  = LogicSystem::getSingleton().getPlayerByGuid(playerID);		  
	RewardLotteryTemplate *rewardLotteryTemplate = gRewardLotteryTable->get(id);
	PlayerCounterData counterData = player->GetPlayerCounterData();

	if (rewardLotteryTemplate->refreshtime > 0)
	{
		if (rewardLotteryTemplate->type == 1)
		{
			if ((nowTime - counterData.m_RewardLotteryOnceTime) < rewardLotteryTemplate->refreshtime)
			{
				return LynxErrno::BeingCooled;
			}			 
		}		
	}

	if (player->getPlayerGold() < rewardLotteryTemplate->cost)
	{
		return LynxErrno::RmbNotEnough;
	}

	if (rewardLotteryTemplate->vipneed > player->getVipLevel())
	{		
		return LynxErrno::VipLevelNotEnough;		
	}

	//找maxkey
	for (List<KeyValue>::Iter * it = rewardLotteryTemplate->keyValues.begin();it !=NULL;it = rewardLotteryTemplate->keyValues.next(it))
	{
		if (it->mValue.key > maxKey)
		{
			maxKey = it->mValue.key;
		}		
	}

	if (rewardLotteryTemplate->type == 1)//免费单抽
	{
		UInt32 onceTime = 0;
		UInt32 tenTime = 0;
		GiftManager::getSingleton().getLotteryLeftTime(player->getPlayerGuid(),onceTime,tenTime);
		if (onceTime > 0)
		{
			return LynxErrno::TimeNotRight;
		}	

		
		if (counterData.m_RewardLotteryOnceFreeCount < maxKey)
		{
			for (List<KeyValue>::Iter * it = rewardLotteryTemplate->keyValues.begin();it !=NULL;it = rewardLotteryTemplate->keyValues.next(it))
			{
				if (it->mValue.key == (counterData.m_RewardLotteryOnceFreeCount+1))
				{
					getIt = 1;
					awardID = it->mValue.value;
					break;
				}
			}
			if (getIt == 0)
			{
				awardID = rewardLotteryTemplate->normalaward;
			}
		}
		else
		{
			if (rewardLotteryTemplate->nextbestneedtimes == 0)
			{
				return LynxErrno::NotFound;
			}
			//第6个直接给最好的并重置
			if ( (counterData.m_RewardLotteryOnceFreeCount - maxKey)% (rewardLotteryTemplate->nextbestneedtimes + 1)>= rewardLotteryTemplate->nextbestneedtimes )
			{
				counterData.m_RewardLotteryOnceFreeCount = maxKey-1 ;
				awardID = rewardLotteryTemplate->bestaward;
			}
			else if ( (counterData.m_RewardLotteryOnceFreeCount - maxKey)% (rewardLotteryTemplate->nextbestneedtimes + 1)>= (rewardLotteryTemplate->nextbestneedtimes -2)&&
				(counterData.m_RewardLotteryOnceFreeCount - maxKey)% (rewardLotteryTemplate->nextbestneedtimes + 1)< (rewardLotteryTemplate->nextbestneedtimes))
			{
				if((rand()%10000) >6666)
				{
					counterData.m_RewardLotteryOnceFreeCount = maxKey-1 ;
					awardID = rewardLotteryTemplate->bestaward;
				}
				else
				{
					awardID = rewardLotteryTemplate->normalaward;
				}
			}			 
			else
			{				 
				awardID = rewardLotteryTemplate->normalaward;
			}		
		}
		counterData.m_RewardLotteryOnceTime = nowTime;
		counterData.m_RewardLotteryOnceFreeCount ++;
	}
	else if (rewardLotteryTemplate->type == 2)//单抽券
	{
		if (counterData.m_RewardLotteryOnceTicket < 1)
		{
			return LynxErrno::OnceTicketNotEnough;
		}
		
		if (counterData.m_RewardLotteryOnceTicketCount < maxKey)
		{
			for (List<KeyValue>::Iter * it = rewardLotteryTemplate->keyValues.begin();it !=NULL;it = rewardLotteryTemplate->keyValues.next(it))
			{
				if (it->mValue.key == (counterData.m_RewardLotteryOnceTicketCount+1))
				{
					getIt = 1;
					awardID = it->mValue.value;
					break;
				}
			}
			if (getIt == 0)
			{
				awardID = rewardLotteryTemplate->normalaward;
			}
		}
		else
		{
			if (rewardLotteryTemplate->nextbestneedtimes == 0)
			{
				return LynxErrno::NotFound;
			}
			//第6个直接给最好的并重置
			if ( (counterData.m_RewardLotteryOnceTicketCount - maxKey)% (rewardLotteryTemplate->nextbestneedtimes + 1)>= rewardLotteryTemplate->nextbestneedtimes )
			{
				counterData.m_RewardLotteryOnceTicketCount = maxKey-1 ;
				awardID = rewardLotteryTemplate->bestaward;
			}
			else if ( (counterData.m_RewardLotteryOnceTicketCount - maxKey)% (rewardLotteryTemplate->nextbestneedtimes + 1)>= (rewardLotteryTemplate->nextbestneedtimes -2)&&
				(counterData.m_RewardLotteryOnceTicketCount - maxKey)% (rewardLotteryTemplate->nextbestneedtimes + 1)< (rewardLotteryTemplate->nextbestneedtimes))
			{
				if((rand()%10000) >6666)
				{
					counterData.m_RewardLotteryOnceTicketCount = maxKey-1 ;
					awardID = rewardLotteryTemplate->bestaward;
				}
				else
				{
					awardID = rewardLotteryTemplate->normalaward;
				}
			}			 
			else
			{				 
				awardID = rewardLotteryTemplate->normalaward;
			}		
		}
		counterData.m_RewardLotteryOnceTicket --;
		counterData.m_RewardLotteryOnceTicketCount ++;
	}

	else if (rewardLotteryTemplate->type == 3)//单抽
	{
		if (player->getPlayerGold() < rewardLotteryTemplate->cost)
		{
			return LynxErrno::RmbNotEnough;
		}

		if (counterData.m_RewardLotteryOnceCount < maxKey)
		{
			for (List<KeyValue>::Iter * it = rewardLotteryTemplate->keyValues.begin();it !=NULL;it = rewardLotteryTemplate->keyValues.next(it))
			{
				if (it->mValue.key == (counterData.m_RewardLotteryOnceCount+1))
				{
					getIt = 1;
					awardID = it->mValue.value;
					break;
				}
			}
			if (getIt == 0)
			{
				awardID = rewardLotteryTemplate->normalaward;
			}
		}
		else
		{
			if (rewardLotteryTemplate->nextbestneedtimes == 0)
			{
				return LynxErrno::NotFound;
			}
			UInt32 tmp = (counterData.m_RewardLotteryOnceCount - maxKey)% (rewardLotteryTemplate->nextbestneedtimes + 1);
			//第6个直接给最好的并重置
			if ( (counterData.m_RewardLotteryOnceCount - maxKey)% (rewardLotteryTemplate->nextbestneedtimes + 1) >= rewardLotteryTemplate->nextbestneedtimes )
			{
				counterData.m_RewardLotteryOnceCount = maxKey-1 ;
				awardID = rewardLotteryTemplate->bestaward;
			}
			else if ( (counterData.m_RewardLotteryOnceCount - maxKey)% (rewardLotteryTemplate->nextbestneedtimes + 1)>= (rewardLotteryTemplate->nextbestneedtimes -2)&&
				(counterData.m_RewardLotteryOnceCount - maxKey)% (rewardLotteryTemplate->nextbestneedtimes + 1)< (rewardLotteryTemplate->nextbestneedtimes))
			{
				if((rand()%10000) >6666)
				{
					counterData.m_RewardLotteryOnceCount = maxKey-1 ;
					awardID = rewardLotteryTemplate->bestaward;
				}
				else
				{
					awardID = rewardLotteryTemplate->normalaward;
				}
			}			 
			else
			{				 
				awardID = rewardLotteryTemplate->normalaward;
			}		
		}
		PlayerBaseData baseData = player->getPlayerBaseData();
		baseData.m_nGold -= rewardLotteryTemplate->cost;
		player->setPlayerBaseData(baseData);
		player->getPersistManager().setDirtyBit(BASEDATABIT);

		counterData.m_RewardLotteryOnceCount ++;
	}
	else if (rewardLotteryTemplate->type == 4)//免费十连抽
	{
		if (counterData.m_RewardLotteryTenFreeCount < maxKey)
		{
			for (List<KeyValue>::Iter * it = rewardLotteryTemplate->keyValues.begin();it !=NULL;it = rewardLotteryTemplate->keyValues.next(it))
			{
				if (it->mValue.key == (counterData.m_RewardLotteryTenFreeCount+1))
				{
					getIt = 1;
					awardID = it->mValue.value;
					break;
				}
			}
			if (getIt == 0)
			{
				awardID = rewardLotteryTemplate->normalaward;
			}
		}
		else
		{
			if (rewardLotteryTemplate->nextbestneedtimes == 0)
			{
				return LynxErrno::NotFound;
			}
			if(counterData.m_RewardLotteryTenFreeCount == maxKey + rewardLotteryTemplate->nextbestneedtimes + 1)
			{
				awardID = rewardLotteryTemplate->bestaward;
			}
			else
			{
				awardID = rewardLotteryTemplate->normalaward;
			}				
		}
	}
	else if (rewardLotteryTemplate->type == 5)//十连抽
	{
		if (player->getPlayerGold() < rewardLotteryTemplate->cost)
		{
			return LynxErrno::RmbNotEnough;
		}
	

// 		counterData.m_RewardLotteryTenCount ++;//十连抽后面加
		if (counterData.m_RewardLotteryTenCount < maxKey)
		{
			for (List<KeyValue>::Iter * it = rewardLotteryTemplate->keyValues.begin();it !=NULL;it = rewardLotteryTemplate->keyValues.next(it))
			{
				if (it->mValue.key == (counterData.m_RewardLotteryTenCount+1))
				{
					getIt = 1;
					awardID = it->mValue.value;
					break;
				}
			}
			if (getIt == 0)
			{
				awardID = rewardLotteryTemplate->normalaward;
			}
		}
		else
		{
			if (rewardLotteryTemplate->nextbestneedtimes == 0)
			{
				return LynxErrno::NotFound;
			}
			//第6个直接给最好的并重置
			if(counterData.m_RewardLotteryTenCount == maxKey + rewardLotteryTemplate->nextbestneedtimes + 1)
			{
				awardID = rewardLotteryTemplate->bestaward;
			}
			else
			{
				awardID = rewardLotteryTemplate->normalaward;
			}	
		}
		PlayerBaseData baseData = player->getPlayerBaseData();
		baseData.m_nGold -= rewardLotteryTemplate->cost;
		player->setPlayerBaseData(baseData);
		player->getPersistManager().setDirtyBit(BASEDATABIT);
	}
	else if (rewardLotteryTemplate->type == 6)//vip抽
	{
		if(player->getVipLevel() < rewardLotteryTemplate->vipneed)
		{
			return LynxErrno::VipLevelNotEnough;

		}
		if (player->getPlayerGold() < rewardLotteryTemplate->cost)
		{
			return LynxErrno::RmbNotEnough;
		}
		UInt32 tmpAwardID = 0;
		UInt32 count = 0;

		for (Map<UInt32, LotteryActivityTemplate>::Iter*iter = gLotteryActivityTable->mMap.begin();iter != NULL;iter = gLotteryActivityTable->mMap.next(iter))
		{
			if (iter->mValue.id == 1)
			{
				tmpAwardID = iter->mValue.lotteryid;
				break;
			}
		}

		if (rewardLotteryTemplate->id == tmpAwardID)
		{
			count = counterData.m_RewardLotteryVipDefaultCount;			
		}
		else if (rewardLotteryTemplate->id == counterData.m_RewardLotteryVipAwardID)
		{
			count = counterData.m_RewardLotteryVipElseCount;
		}
		else
		{
			count = 0;			
		}
		counterData.m_RewardLotteryVipAwardID = rewardLotteryTemplate->id;
		
		
		if (count < maxKey)
		{
			for (List<KeyValue>::Iter * it = rewardLotteryTemplate->keyValues.begin();it !=NULL;it = rewardLotteryTemplate->keyValues.next(it))
			{
				if (it->mValue.key == (count+1))
				{
					getIt = 1;
					awardID = it->mValue.value;
					break;
				}
			}
			if (getIt == 0)
			{
				awardID = rewardLotteryTemplate->normalaward;
			}
		}
		else
		{
			
			//第6个直接给最好的并重置
			if ( (count - maxKey)>= rewardLotteryTemplate->nextbestneedtimes )
			{
				count = maxKey-1 ;
				awardID = rewardLotteryTemplate->bestaward;
			}
			else if ( (count - maxKey)>= (rewardLotteryTemplate->nextbestneedtimes -2)&&
				(count - maxKey) < (rewardLotteryTemplate->nextbestneedtimes))
			{
				if((rand()%10000) >6666)
				{
					count = maxKey-1 ;
					awardID = rewardLotteryTemplate->bestaward;
				}
				else
				{
					awardID = rewardLotteryTemplate->normalaward;
				}
			}			 
			else
			{				 
				awardID = rewardLotteryTemplate->normalaward;
			}		
		}
		PlayerBaseData baseData = player->getPlayerBaseData();
		baseData.m_nGold -= rewardLotteryTemplate->cost;
		player->setPlayerBaseData(baseData);
		player->getPersistManager().setDirtyBit(BASEDATABIT);
	
		count ++;
		if (rewardLotteryTemplate->id == tmpAwardID)
		{
			counterData.m_RewardLotteryVipDefaultCount = count;
		}
		else
		{
			counterData.m_RewardLotteryVipElseCount = count;
		}
	}
	else if (rewardLotteryTemplate->type == 7)//单抽
	{
		if (counterData.m_RewardLotteryTenTicketCount < maxKey)
		{
			for (List<KeyValue>::Iter * it = rewardLotteryTemplate->keyValues.begin();it !=NULL;it = rewardLotteryTemplate->keyValues.next(it))
			{
				if (it->mValue.key == (counterData.m_RewardLotteryTenTicketCount+1))
				{
					getIt = 1;
					awardID = it->mValue.value;
					break;
				}
			}
			if (getIt == 0)
			{
				awardID = rewardLotteryTemplate->normalaward;
			}
		}
		else
		{			
			if(counterData.m_RewardLotteryTenTicketCount == maxKey + rewardLotteryTemplate->nextbestneedtimes + 1)
			{
				awardID = rewardLotteryTemplate->bestaward;
			}
			else
			{
				awardID = rewardLotteryTemplate->normalaward;
			}
		}
	}

	player->setPlayerCounterData(counterData);
	player->getPersistManager().setDirtyBit(COUNTERDATABIT);


	return LynxErrno::None;

}

UInt32 GiftManager::checkRewardLotteryTenCount(Guid playerID,UInt32 typeID,UInt32 &count)
{

	UInt32 onceTime = 0;
	UInt32 tenTime = 0;
	UInt32 maxKey = 0;
	UInt32 getIt = 0;
	UInt32 randNum = 0;
	PlayerCounterData counterData;

	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	if (player == NULL)
	{
		return LynxErrno::NotFound;
	}

	RewardLotteryTemplate *rewardLotteryTemplate;

	for( Map<UInt32, RewardLotteryTemplate>::Iter * iter = gRewardLotteryTable->mMap.begin();iter != NULL;iter = gRewardLotteryTable->mMap.next(iter))
	{
		if (iter->mValue.type == typeID)
		{
			rewardLotteryTemplate = &iter->mValue;
			break;
		}
	}
	counterData = player->GetPlayerCounterData();//位置不能换

	for (List<KeyValue>::Iter * it = rewardLotteryTemplate->keyValues.begin();it !=NULL;it = rewardLotteryTemplate->keyValues.next(it))
	{
		if (it->mValue.key > maxKey)
		{
			maxKey = it->mValue.key;
		}		
	}
	PlayerBaseData baseData = player->getPlayerBaseData();
	player->setPlayerBaseData(baseData);
	player->getPersistManager().setDirtyBit(BASEDATABIT);

	
	// 		counterData.m_RewardLotteryTenCount ++;//十连抽后面加
	if (count < maxKey)
	{			
	}
	else
	{
		if (rewardLotteryTemplate->nextbestneedtimes == 0)
		{
			return LynxErrno::NotFound;
		}
		//第6个直接给最好的并重置
		if ( (count - maxKey)% (rewardLotteryTemplate->nextbestneedtimes + 1) >= rewardLotteryTemplate->nextbestneedtimes )//没有++
		{
			count = maxKey + rewardLotteryTemplate->nextbestneedtimes + 1;	
		}
		else if ( (count - maxKey)% (rewardLotteryTemplate->nextbestneedtimes + 1) >= (rewardLotteryTemplate->nextbestneedtimes -2)&&
			(count - maxKey)% (rewardLotteryTemplate->nextbestneedtimes + 1)< (rewardLotteryTemplate->nextbestneedtimes))//没有++
		{		
			randNum =(rand()%10000);
			if(randNum > 6666)
			{
				count = maxKey + rewardLotteryTemplate->nextbestneedtimes + 1;					
			}
			else
			{
			}
		}			 
		else
		{				 
		}		
	}
	return LynxErrno::None;
	
}
UInt32 GiftManager::checkRewardLotteryTen(Guid playerID,UInt32 typeID)
{

	UInt32 onceTime = 0;
	UInt32 tenTime = 0;
	PlayerCounterData counterData;

	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	if (player == NULL)
	{
		return LynxErrno::NotFound;
	}
	counterData = player->GetPlayerCounterData();//位置不能换


	if (typeID == RL_TEN_FREE)
	{
		GiftManager::getSingleton().getLotteryLeftTime(player->getPlayerGuid(),onceTime,tenTime);
		if (tenTime != 0)
		{
			return LynxErrno::BeingCooled;			
		}
		checkRewardLotteryTenCount(player->getPlayerGuid(),typeID,counterData.m_RewardLotteryTenFreeCount);
	}
	else if(typeID == RL_TEN)//十连抽判断元宝
	{
		RewardLotteryTemplate * rewardLotteryTemplate;
		for( Map<UInt32, RewardLotteryTemplate>::Iter * iter = gRewardLotteryTable->mMap.begin();iter != NULL;iter = gRewardLotteryTable->mMap.next(iter))
		{
			if (iter->mValue.type == RL_TEN)
			{
				rewardLotteryTemplate = &iter->mValue;
				break;
			}
		}
		if(player->getPlayerGold() < (rewardLotteryTemplate->cost * 10))
		{
			return LynxErrno::RmbNotEnough;			
		}
		checkRewardLotteryTenCount(player->getPlayerGuid(),typeID,counterData.m_RewardLotteryTenCount);
	}
	else if (typeID == RL_TEN_TICKET)
	{
		if (counterData.m_RewardLotteryTenTicket < 1)
		{
			return LynxErrno::TenTicketNotEnough;
		}
		checkRewardLotteryTenCount(player->getPlayerGuid(),typeID,counterData.m_RewardLotteryTenTicketCount);
	}
	player->setPlayerCounterData(counterData);
	return LynxErrno::None;
}

void GiftManager::onRewardLotteryReq(const  ConnId& connId ,RewardLotteryReq & msg )
{

	UInt32 maxTimes = 1;
	bool bFlag = false;
	Goods goods;
	UInt32 countNum = 1;
	UInt32 flag = 0;
	UInt32 onceTime = 0;
	UInt32 tenTime = 0;
	UInt32 nowTime = TimeUtil::getTimeSec();	
	List<UInt32> idList;
	List<UInt32> awardIDList;	
	List<Goods> itemList;
	RewardLotteryTemplate rewardLotteryTemplate3;
	RewardLotteryTemplate rewardLotteryTemplate6;
	PlayerCounterData counterData;
	Json::Value jsonValue;
	RewardLotteryResp resp;


	Player * player = LogicSystem::getSingleton().getPlayerByConnId(connId);

	msg.convertJsonToData(msg.strReceive);
	resp.reqType = msg.reqType;

	

	resp.result = GiftManager::getSingleton().checkRewardLotteryTen(player->getPlayerGuid(),msg.reqType);
	if ( resp.result != LynxErrno::None)
	{
		std::string jsonStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,OPEN_REWARD_LOTTERY_RESP,jsonStr);
		return;
	}

	idList = GiftManager::getSingleton().getRewardLotteryIDs(msg.reqType);


	for (List<UInt32>::Iter * iter = idList.begin();iter !=NULL;iter = idList.next(iter))
	{
		UInt32 awardID = 0;
		UInt32 result = GiftManager::getSingleton().getRewardLotteryAwardID(player->getPlayerGuid(),iter->mValue,awardID);
		if ( result != LynxErrno::None)
		{
			resp.result = result;
			std::string jsonStr = resp.convertDataToJson();
			NetworkSystem::getSingleton().sender( connId,OPEN_REWARD_LOTTERY_RESP,jsonStr);
			return;
		}
		awardIDList.insertTail(awardID);
	}
	UInt32 maxKey = 0;
	RewardLotteryTemplate *rewardLotteryTemplate;

	for( Map<UInt32, RewardLotteryTemplate>::Iter * iter = gRewardLotteryTable->mMap.begin();iter != NULL;iter = gRewardLotteryTable->mMap.next(iter))
	{
		if (iter->mValue.type == msg.reqType)
		{
			rewardLotteryTemplate = &iter->mValue;
			break;
		}
	}

	for (List<KeyValue>::Iter * it = rewardLotteryTemplate->keyValues.begin();it !=NULL;it = rewardLotteryTemplate->keyValues.next(it))
	{
		if (it->mValue.key > maxKey)
		{
			maxKey = it->mValue.key;
		}		
	}
	counterData = player->GetPlayerCounterData();//位置不能换
	//十连抽 vip ticket 加次数 减票数
	if(msg.reqType == RL_TEN )
	{
		//元宝在里面扣
		if (counterData.m_RewardLotteryTenCount >= maxKey + rewardLotteryTemplate->nextbestneedtimes +1)
		{
			counterData.m_RewardLotteryTenCount = maxKey-1 ;
		}
		counterData.m_RewardLotteryTenCount ++;	
	}
	else if (msg.reqType == RL_TEN_TICKET )		
	{
		counterData.m_RewardLotteryTenTicket --;
		if (counterData.m_RewardLotteryTenTicketCount >= maxKey + rewardLotteryTemplate->nextbestneedtimes +1)
		{
			counterData.m_RewardLotteryTenTicketCount = maxKey-1 ;
		}
		counterData.m_RewardLotteryTenTicketCount ++;	
	}
	else if (msg.reqType == RL_TEN_FREE )		
	{
		counterData.m_RewardLotteryTenTime = nowTime;
		
		if (counterData.m_RewardLotteryTenFreeCount >= maxKey + rewardLotteryTemplate->nextbestneedtimes +1)
		{
			counterData.m_RewardLotteryTenFreeCount = maxKey-1 ;
		}
		counterData.m_RewardLotteryTenFreeCount ++;	
	}
	player->setPlayerCounterData(counterData);
	player->getPersistManager().setDirtyBit(COUNTERDATABIT);
	

	for (List<UInt32>::Iter * it = awardIDList.begin();it !=NULL;it = awardIDList.next(it))
	{
		Award award;
		GiftManager::getSingleton().getContentByID(it->mValue,award.award);
		resp.awards.insertTail(award);
	}
	//判断是否已经有佣兵
	GiftManager::getSingleton().servanNeedChangeToPiece(player->getPlayerGuid(),resp.awards,itemList);

	UInt32 getIt = 0;
	for(List<Award>::Iter * item = resp.awards.begin();item != NULL; item = resp.awards.next(item))
	{
		for(List<Goods>::Iter *item1 = item->mValue.award.begin();item1 != NULL;item1 = item->mValue.award.next(item1))
		{
			if (item1->mValue.resourcestype == AWARD_SERVANT)
			{
				getIt = 0;//判断是否重复的佣兵
				for(List<UInt32>::Iter * item2 = resp.newServant.begin();item2 != NULL;item2 = resp.newServant.next(item2)  )
				{
					if (item2->mValue == item1->mValue.subtype)
					{
						getIt = 1;
						break;
					}
				}
				if (getIt == 0)
				{
					resp.newServant.insertTail(item1->mValue.subtype);//需要显示的佣兵
				}				
			}			
		}
	}

	GiftManager::getSingleton().combineSame(itemList);

	GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,itemList);

	GiftManager::getSingleton().PlayerItemChangeResult(player->getPlayerGuid(),0,itemList);

	counterData = player->GetPlayerCounterData();//位置不能换
	
	for(List<UInt32>::Iter * iter1 = resp.newServant.begin();iter1 != NULL;iter1 = resp.newServant.next(iter1))
	{
		GiftManager::getSingleton().getAllAttr(player->getPlayerGuid(),AWARD_SERVANT,iter1->mValue,jsonValue);
	}
	
	GiftManager::getSingleton().getLotteryLeftTime(player->getPlayerGuid(),onceTime,tenTime);
	
	resp.gold = player->getPlayerGold();
	resp.onceLeftTime = onceTime;
	resp.tenLeftTime = tenTime;
	resp.couponOnceNum = counterData.m_RewardLotteryOnceTicket;
	resp.couponTenNum = counterData.m_RewardLotteryTenTicket;
	resp.ends = itemList;
	resp.allAttr = jsonValue;

	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,OPEN_REWARD_LOTTERY_RESP,jsonStr);
	
	//添加成就打点 wwc
	if(resp.reqType == RL_ONCE_FREE || resp.reqType == RL_ONCE_TICKET || resp.reqType == RL_ONCE)	
	{
		player->getAchieveManager().updateAchieveData(LINGRENCALLSERVANT,1);
		//每日任务打点 wwc
		player->getAchieveManager().updateDailyTaskData(DLYLINGRENCALLCNT, 1 );
	}
	else if(resp.reqType == RL_VIP)
	{
		player->getAchieveManager().updateAchieveData(MINGLINGCALLSERVANT,1);
	}
	else if(resp.reqType == RL_TEN_FREE || resp.reqType == RL_TEN_TICKET || resp.reqType == RL_TEN)
	{
		player->getAchieveManager().updateAchieveData(XIBANCALLSERVANT,1);
		//每日任务打点wwc
		player->getAchieveManager().updateDailyTaskData(DLYXIBANCALLCNT, 1 );
	}

	
	
}


void GiftManager::servanNeedChangeToPiece(Guid playerID, List<Award> &awards,List<Goods> &ends)
{
	bool servantExist = false;
	Goods goods;	
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	
	for(List<Award>::Iter * item = awards.begin();item != NULL; item = awards.next(item))
	{
		GiftManager::getSingleton().combineSame(item->mValue.award);

		//先删除AWARD_SERVANT 在加到ends
		for( List<Goods>::Iter * iter = item->mValue.award.begin();iter != NULL;iter = item->mValue.award.next(iter))
		{
			if (iter->mValue.resourcestype == AWARD_SERVANT)
			{
				 ServantTemplate * servantTemp = SERVANT_TABLE().get(iter->mValue.subtype);
				 if (servantTemp == NULL)
				 {
					 continue;//todo 多个情况有问题
				 }
				if(player->getServantManager().isServantExist(iter->mValue.subtype) == true)
				{
					servantExist = true;
					goods.resourcestype = AWARD_SERVANTPIECE;
					goods.subtype = iter->mValue.subtype;
					goods.num = iter->mValue.num * servantTemp->mPieceCount;
					item->mValue.award.insertTail(goods);
					item->mValue.award.erase(iter);
				}
				else
				{
					if (servantExist == false)
					{
						goods.resourcestype = AWARD_SERVANTPIECE;
						goods.subtype = iter->mValue.subtype;
						goods.num = (iter->mValue.num - 1) * servantTemp->mPieceCount;
						if (goods.num != 0)
						{
							item->mValue.award.insertTail(goods);
						}						
						iter->mValue.num = 1;						
					}
					else
					{
						goods.resourcestype = AWARD_SERVANTPIECE;
						goods.subtype = iter->mValue.subtype;
						goods.num = iter->mValue.num * servantTemp->mPieceCount;
						item->mValue.award.insertTail(goods);
						item->mValue.award.erase(iter);
					}
					servantExist = true;

				}
			}
		}
		
		ends += item->mValue.award;//先存储stage信息，再删除
		for( List<Goods>::Iter * iter = item->mValue.award.begin();iter != NULL;iter = item->mValue.award.next(iter))
		{
			if (iter->mValue.resourcestype == AWARD_STAGEDATA || iter->mValue.resourcestype == AWARD_TWELVEPALACE_STAGEDATA)
			{
				item->mValue.award.erase(iter);
			}
		}
	}
}
void GiftManager::getAllAttr(Guid playerID,UInt32 resType,UInt32 subType,Json::Value &jsonValue)
{
	Json::Value son; 
	Json::Value root; 
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	if(player == NULL)
	{
		return;
	}
	
	if (resType == AWARD_SERVANT)
	{
		player->getServantManager().getServantDataJson(subType,son);
	}

	root["resourcestype"] = resType ;
	root["subtype"] = subType ;
	root["num"] = 1;
	root["attr"]= Json::Value(son);
	jsonValue.append(root);

}
void GiftManager::getLotteryLeftTime(Guid playerID,UInt32 &onceTime,UInt32 &tenTime)//
{
	UInt32 nowTime = TimeUtil::getTimeSec();
	RewardLotteryTemplate rewardLotteryTemplate1;
	RewardLotteryTemplate rewardLotteryTemplate4;

	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	if(player == NULL)
	{
		return;
	}
	PlayerCounterData counterData = player->GetPlayerCounterData();
	for( Map<UInt32, RewardLotteryTemplate>::Iter * iter = gRewardLotteryTable->mMap.begin();iter != NULL;iter = gRewardLotteryTable->mMap.next(iter))
	{
		if (iter->mValue.type == RL_ONCE_FREE)
		{
			rewardLotteryTemplate1 = iter->mValue;
			break;
		}		
	}
	for( Map<UInt32, RewardLotteryTemplate>::Iter * iter = gRewardLotteryTable->mMap.begin();iter != NULL;iter = gRewardLotteryTable->mMap.next(iter))
	{
		if (iter->mValue.type == RL_TEN_FREE)
		{
			rewardLotteryTemplate4 = iter->mValue;
			break;
		}
	}

	int tmpOnceTime = rewardLotteryTemplate1.refreshtime - ( nowTime - counterData.m_RewardLotteryOnceTime);
	if (tmpOnceTime <0)
	{
		tmpOnceTime = 0;
	}

	int tmpTenTime = rewardLotteryTemplate4.refreshtime - ( nowTime - counterData.m_RewardLotteryTenTime);
	if (tmpTenTime <0)
	{
		tmpTenTime = 0;
	}
	onceTime = tmpOnceTime;
	tenTime = tmpTenTime;
}

UInt32 GiftManager::modifyCoin(Guid playerID,Int32 num)//铜钱
{
	String strData ;
	char dest[64] = {};
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);

	UInt32 gold = player->getPlayerCoin();
	gold += num;
	player->setPlayerCoin(gold);	
	sprintf(dest,"%d,%d",1,gold);
	strData = (String)dest;
	updateSingleProperty( playerID, "1", strData);	
	return 1;

}
UInt32 GiftManager::modifyExp(Guid playerID,Int32 num)//经验
{
	String strData ;
	char dest[64] = {};

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);

	UInt32 exp = player->getPlayerExp();
	exp += num;
	player->setPlayerExp(exp);

	sprintf(dest,"%d,%d",5,exp);
	strData = (String)dest;
	updateSingleProperty( playerID, "1", strData);	

	return 1;

}
UInt32 GiftManager::modifyItem(Guid playerID,Guid templateID,Int32 num)//装备
{
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	List<ItemData>* itemsList = player->getItemManager().getItemDatasList();

	if (player->getItemManager().getEmputyNum(1) < num)
	{
		return 2;
	}

	for (int i=0;i<num;i++)
	{
		player->getItemManager().getNewItemsSame(templateID,num);
	}
	return 1;

}

UInt32 GiftManager::modifyShengWang(Guid playerID,Int32 num)//声望
{
	String strData ;
	char dest[64] = {};

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);

	UInt32 iFlag = player->getPlayerShengWang();

	iFlag += num;
	player->setPlayerShengWang(iFlag);
	sprintf(dest,"%d,%d",5,iFlag);
	strData = (String)dest;
	updateSingleProperty( playerID, "1", strData);	
	return 1;
}

UInt32 GiftManager::modifyRhyme(Guid playerID,Int32 num)//声望
{
	String strData ;
	char dest[64] = {};

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);

	UInt32 iFlag = player->getPlayerRhyme();

	iFlag += num;
	player->setPlayerRhyme(iFlag);
	sprintf(dest,"%d,%d",6,iFlag);
	strData = (String)dest;
	player->getPersistManager().setDirtyBit(RHYMEDATABIT);
	// 	updateSingleProperty( playerID, "1", strData);	
	return 1;
}

UInt32 GiftManager::modifyYuanBao(Guid playerID,Int32 num)//元宝
{
	String strData ;
	char dest[64] = {};

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);

	UInt32 yuanBao = player->getPlayerGold();

	yuanBao += num;
	player->setPlayerGold(yuanBao);
	sprintf(dest,"%d,%d",2,yuanBao);
	strData = (String)dest;
	updateSingleProperty( playerID, "1", strData);	

	return 1;

}
UInt32 GiftManager::modifyMercenary(Guid playerID,Int32 num)//佣兵
{
	String strData ;
	char dest[64] = {};
	UInt32 yongBingID =22222;

	sprintf(dest,"%d,%d",yongBingID,num);
	strData = (String)dest;
	updateSingleProperty( playerID, "6", strData);
	return 1;
}

UInt32 GiftManager::updateCounter(Guid playerID,UInt32 boxID,Int32 num)//计数器
{
	String strData ;
	char dest[64]={};

	sprintf(dest,"%d,%d",boxID,num);
	strData = (String)dest;
	updateSingleProperty( playerID, "21", strData);
	return 1;
}

UInt32 GiftManager::updateRewardCounter(Guid playerID,UInt32 boxID,Int32 num)//计数器
{
	String strData ;
	char dest[64]={};
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerID);

	List<UInt32> rewardCounter;
	rewardCounter = player->GetRewardCounter();

	UInt32 i =0;
	for (List<UInt32>::Iter * iter = rewardCounter.begin();iter!=NULL; iter = rewardCounter.next(iter))
	{
		if (i == boxID)
		{
			iter->mValue += num;
			break;
		}
		i++;
	}
	player->setRewardCounter(rewardCounter);
	sprintf(dest,"%d,%d",boxID,num);
	strData = (String)dest;
	updateSingleProperty( playerID, "22", strData);
	return 1;
}

UInt32 GiftManager::updateFood(Guid playerID,UInt32 boxID,Int32 num)//美食屋存档
{
	String strData ;
	char dest[64]={};

	sprintf(dest,"%d,%d",boxID,num);
	strData = (String)dest;
	updateSingleProperty( playerID,"23", strData);
	return 1;
}
UInt32 GiftManager::updateStrength(Guid playerID,UInt32 boxID,Int32 num)//美食屋存档
{
	String strData ;
	char dest[64]={};

	sprintf(dest,"%d,%d",boxID,num);
	strData = (String)dest;
	updateSingleProperty( playerID, "24", strData);
	return 1;
}



void GiftManager::updateSingleProperty(Guid playerID,String strFlag,String strData)
{
	PersistPlayerSinglePropertySaveReq req;
	req.playerGuid = playerID;
	req.strFlag = strFlag;
	req.strData = strData;
	PersistSystem::getSingleton().postThreadMsg(req, 0);
}
void GiftManager::ChoiseNFromWeigthList(UInt32 num , List<Resource> inList,List<Goods> &outList)
{

	UInt32 allWeight ;
	UInt32 weight;
	UInt32 randomnum;
	UInt32 maxnum = inList.size();

	Goods goods;
	if (num<1 || num > maxnum)
	{
		return ;
	}
	for(int i =0;i<num;i++)
	{
		allWeight =0;
		weight =0;
		for(List<Resource>::Iter *it = inList.begin();it!=NULL;it = inList.next(it))
		{
			allWeight += it->mValue.weight;
		}
		if (allWeight == 0)
		{
			break;
		}
		randomnum = rand()%(allWeight);

		for(List<Resource>::Iter *it = inList.begin();it!=NULL;it = inList.next(it))
		{
			weight += it->mValue.weight;
			if (randomnum < weight)
			{
				goods.subtype = it->mValue.subtype;		
				goods.resourcestype = it->mValue.resourcestype;
				goods.num = it->mValue.num;
				outList.insertTail(goods);
				inList.erase(it);
				break;
			}
		}
	}
	return ;
}

void GiftManager::onPropertyChang(const  ConnId& connId,CGPropertyChange & msg)		//属性更改
{
	char dest[64] = {} ;
	String strData;

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	msg.convertJsonToData(msg.jsonStr);
	sprintf(dest,"%s,%s",msg.sKey.c_str(),msg.sValue.c_str());	
	strData = (String)dest;

	if (msg.sFlag == "1")
	{
		GiftManager::getSingleton().modifyCoin(player->getPlayerGuid(),lynxAtoi<UInt32>(msg.sValue.c_str()));
	}
}

void GiftManager::sendItemListChange(Guid playerID,List<UInt64> ItemUids)
{
	MsgItemDataToJson itemDataToJson;

	if (ItemUids.size() == 0)
	{
		return;
	}
	ItemData *itemData ;
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	for(List<UInt64>::Iter *iter = ItemUids.begin();iter != NULL;iter = ItemUids.next(iter))
	{

		itemData =  player->getItemManager().getItemByGuid(iter->mValue);
		if (itemData == NULL)
		{
			continue;
		}

		//将物品根据类型，插入对应的类型map里
		ItemTemplate * itemTemplate =  ITEM_TABLE().get(itemData->m_nItemId);

		if(itemTemplate->mBagTab == 1)
		{
			if(itemTemplate->mTypeId == 1)
			{
				itemDataToJson.m_mapUidEquipType.insert(itemData->m_nUid, itemData);
			}
		}
		else
		{
			if(itemTemplate->mBagTab == 2)
			{
				itemDataToJson.m_mapUidItemType.insert(itemData->m_nUid, itemData);
			}
			else
			{
				itemDataToJson.m_mapUidOtherType.insert(itemData->m_nUid, itemData);
			}
		}
	}

	std::string jsonStr = itemDataToJson.convertDataToJson();
	NetworkSystem::getSingleton().sender( player->getConnId(),ITME_LIST_CHANGE_RESP,jsonStr);

}


UInt32 GiftManager::updateStageDatas(Guid playerID,List<Goods> itemList)//关卡信息
{
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	for(List<Goods>::Iter *iter =  itemList.begin();iter != NULL;iter = itemList.next(iter))
	{
		player->getStageManager().addNeedSaveDateList(iter->mValue.subtype);		
	}
	player->getPersistManager().setDirtyBit(STAGEDATABIT);

	return 1;
}

