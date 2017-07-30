#include "Gift.h"
#include "../LogicSystem.h"
#include "StringConverter.h"
#include "../PlatformLib/SerializeType.h"
#include "../PersistSystem.h"
#include "../CommonLib/CopyFinishTime.h"
#include "../PassportSystem.h"
#include "../ItemManager.h"
#include "StageCalc.h"
#include "CourageChallenge.h"
#include "GlobalValue.h"

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
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return 1;
	}
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
 				goods.num =  player->getRankGameData().m_MaskNum;
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
			else if (iter->mValue.subtype == AWARD_BASE_RANKEDSCORE)
			{
				goods.resourcestype =AWARD_BASE;
				goods.subtype =AWARD_BASE_RANKEDSCORE;
				goods.num =  player->getRankGameData().m_Score;
				resultList.insertTail(goods);
				continue;
			}	

			else if (iter->mValue.subtype == AWARD_BASE_SCORE)
			{
				goods.resourcestype =AWARD_BASE;
				goods.subtype =AWARD_BASE_RANKEDSCORE;
				goods.num =  player->getPlayerScore();
				resultList.insertTail(goods);
				continue;
			}
			else if (iter->mValue.subtype == AWARD_BASE_VIP_EXP)
			{
				goods.resourcestype =AWARD_BASE;
				goods.subtype =AWARD_BASE_VIP_EXP;
				goods.num = player->getPlayerVIPExp();
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
			goods.resourcestype =AWARD_SERVANTTREASURE;
			goods.subtype = iter->mValue.subtype;
			goods.num = player->getServantManager().getTreasure(iter->mValue.subtype)->count;
			resultList.insertTail(goods);
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

		else if (iter->mValue.resourcestype == AWARD_HOARSTONEEQUIP )
		{
			goods.resourcestype =AWARD_HOARSTONEEQUIP;
			goods.subtype = iter->mValue.subtype;
			goods.num = player->getHoarStoneManager().getHoarStoneRune(iter->mValue.subtype);
			resultList.insertTail(goods);
			continue;
		}
		else if (iter->mValue.resourcestype == AWARD_STAGEDATA)
		{
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
		else if (iter->mValue.resourcestype == AWARD_FOOD)
		{
			
			PlayerFoodsData foodsData = player->getFoodsData();
			if (iter->mValue.subtype == FOODTYPE1)
			{				
				goods.resourcestype =iter->mValue.resourcestype;
				goods.subtype = iter->mValue.subtype;
				goods.num = foodsData.food11;			
				resultList.insertTail(goods);
				continue;
			}
			else if (iter->mValue.subtype == FOODTYPE2)
			{				
				goods.resourcestype =iter->mValue.resourcestype;
				goods.subtype = iter->mValue.subtype;
				goods.num = foodsData.food12;			
				resultList.insertTail(goods);
				continue;
			}
			else if (iter->mValue.subtype == FOODTYPE3)
			{				
				goods.resourcestype =iter->mValue.resourcestype;
				goods.subtype = iter->mValue.subtype;
				goods.num = foodsData.food13;			
				resultList.insertTail(goods);
				continue;
			}
			continue;
		}
		else if (iter->mValue.resourcestype == AWARD_CATCOIN)
		{
			goods.resourcestype = iter->mValue.subtype;
			goods.subtype = iter->mValue.subtype;
			goods.num = CourageChallengeManager::getSingleton().getCatCoinNum(playerID,iter->mValue.subtype);			
			resultList.insertTail(goods);
			continue;

		}
		
		else if (iter->mValue.resourcestype == AWARD_CURRENCY)
		{
			goods.resourcestype =iter->mValue.resourcestype;
			goods.subtype = iter->mValue.subtype;
			PlayerCourageChallengeData courageChallengeData = player->getCourageChallengeData();
			goods.num =courageChallengeData.m_LightOfLife;		
			resultList.insertTail(goods);
			continue;

		}

		else if (iter->mValue.resourcestype == AWARD_FREETICKET)
		{
			PlayerCounterData counterData = player->GetPlayerCounterData();
			if (iter->mValue.subtype == ONCEFREELOTTERYTICKET)
			{
				goods.resourcestype =iter->mValue.resourcestype;
				goods.subtype = iter->mValue.subtype;
				goods.num =counterData.m_RewardLotteryOnceFreeCount;		
				resultList.insertTail(goods);
				continue;
			}
			else	if (iter->mValue.subtype == TENFREELOTTERYTICKET)
			{
				goods.resourcestype =iter->mValue.resourcestype;
				goods.subtype = iter->mValue.subtype;
				goods.num =counterData.m_RewardLotteryOnceFreeCount;		
				resultList.insertTail(goods);
				continue;
			}	
		}



	}

	PlayerFireConfirmData mFireConfirmData =  player->GetFireConfirmData();

	resultList += mFireConfirmData.m_SendList;//等级

	itemList.clear();
	itemList = resultList;

	return 1;
}

UInt32 GiftManager::addToPlayer(Guid playerID,UInt32 reFlashType, List<Goods> itemList,UInt32 systemID)
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
			awardData.resCount = 0 - iter->mValue.num;
			costList.insertTail(awardData);
		}

	}

	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerID);

	player->getAllItemManager().costMaterialsList(costList,systemID);

	player->getAllItemManager().addAwardGetSendJson(awardList,systemID);

	return 1;
}


UInt32 GiftManager::addToPlayerAttr(Guid playerID,List<ReturnItemEle> &rtItemList,Json::Value &root, List<Goods> itemList,UInt32 systemID =0)
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
			iter->mValue.num = 0;
			costList.insertTail(awardData);
		}

	}

	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	if (player == NULL)
	{
		LOG_WARN("player not found!! addToPlayerAttr");
		return 0;
	}

	player->getAllItemManager().costMaterialsList(costList,systemID);

// 	player->getAllItemManager().addAwardGetSendJson(awardList);


// 	List<ReturnItemEle> rtItemList;

	for(List<Goods>::Iter * awardIter = itemList.begin();  awardIter != NULL;  awardIter = itemList.next(awardIter) )
	{
		
		if(awardIter->mValue.resourcestype == AWARD_SERVANT || awardIter->mValue.resourcestype == AWARD_JEWELRY)
			{
				for(UInt32 i = 0; i < awardIter->mValue.num; i++)
				{
						ReturnItemEle rtIemEle;
						player->getAllItemManager().addAwardMaterial(awardIter->mValue.resourcestype,  awardIter->mValue.subtype, 1 , rtIemEle,systemID);
						rtItemList.insertTail(rtIemEle);
				}
			}
			else
			{
				ReturnItemEle rtIemEle;
				player->getAllItemManager().addAwardMaterial(awardIter->mValue.resourcestype,  awardIter->mValue.subtype,  awardIter->mValue.num,rtIemEle,systemID);
				rtItemList.insertTail(rtIemEle);
			}

	}
	player->getAllItemManager().convertItemListToStr(rtItemList, root);
	


	return 1;
}



void GiftManager::splitKind( List<Goods> &goodsList, List<Goods> &kindgoodsList,UInt32 kind)
{
	for( List<Goods>::Iter * iter = goodsList.begin();iter != NULL;iter = goodsList.next(iter))
	{
		if (iter->mValue.resourcestype == AWARD_BASE || iter->mValue.resourcestype == AWARD_GRASS/* ||iter->mValue.resourcestype == AWARD_HOARSTONEPIECE*/			
			||(iter->mValue.resourcestype == AWARD_SERVANTMATERIAL && iter->mValue.subtype == AWARD_SERVANTMATERIAL) /*|| iter->mValue.resourcestype == AWARD_SERVANTPIECE */
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


// void GiftManager::sendToPlayer( Guid playerID,UInt32 type,List<Goods> itemList,List<UInt64> newItemUids)
// {
// 	PropertysChange resp;
// 	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
// 
// 	resp.type = type;
// 	resp.goodsList = itemList;
// 	std::string jsonStr = resp.convertDataToJson();
// 	NetworkSystem::getSingleton().sender( player->getConnId(),PROPERTYS_CHAGE_RESP,jsonStr);
// 	sendItemListChange( playerID, newItemUids);
// }

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
	List<Goods> newOpenChapterStageIDList;
	List<KeyValue> counter;
	List<Counter> counterList;
	List<StepContent> stepContent;
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
	if (stageTemplate == NULL)
	{
		LOG_WARN("stageTemplate not found!!");
		return;
	}


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
			
			StageManager::getSingleton().getOpenChapter(player->getPlayerGuid(),chapterID,0,newOpenChapterStageIDList);
			itemList += newOpenChapterStageIDList;


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

	
	//哥布林掉落
	itemList.clear();
	StageCalcManager::getSingleton().getMonsterAward(chapterID,itemList,awardMonsterList,fireConfirmData.m_IsMopUp);
	tmpAward.award = itemList;
	checkJewelry(itemList,fireConfirmData,isFull);
	if (itemList.size() != 0)
	{
		fireConfirmData.m_MonsterAwardList.insertTail(tmpAward);
	}
	

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


void GiftManager::getGift(const ConnId& connId ,UInt32 id,ChapterEndResp &resp)
{

	bool bFlag = false;
	Goods goods;
	UInt32 isCounted = 0;
	UInt32 countNum = 1;
	UInt32 flag = 0;
	List<Goods> ItemList;
	List<Goods> tmpItemList;
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

	//awardcontent改为award
	tmpItemList.clear();
	GiftManager::getSingleton().getAwardByID(contentID,0, tmpItemList);	
	for(List<Goods>::Iter *it = tmpItemList.begin();it!= NULL;it = tmpItemList.next(it))
	{
		GiftManager::getSingleton().getContentByID(it->mValue.subtype,ItemList);
	}


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
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}
	msg.convertJsonToData(msg.strReceive);	

	item =  player->getItemManager().getItemByGuid(msg.boxID);
	if (item == NULL)
	{
// 		resp.result = LynxErrno::ClienServerDataNotMatch;
		std::string sendStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,OPEN_BOX_RESP,sendStr);
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
		GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_BOX,ItemList,MiniLog54);
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
	
	int onceTime = 0;
	int tenTime = 0;
	GiftManager::getSingleton().getLotteryLeftTime(player->getPlayerGuid(),onceTime,tenTime);


	List<UInt32> idList;
	idList = GiftManager::getSingleton().getRewardLotteryIDs(RL_ONCE);
	for (List<UInt32>::Iter * iter = idList.begin();iter !=NULL;iter = idList.next(iter))
	{
		resp.awardId1 = iter->mValue;
		break;
	}

	idList = GiftManager::getSingleton().getRewardLotteryIDs(RL_TEN);
	for (List<UInt32>::Iter * iter = idList.begin();iter !=NULL;iter = idList.next(iter))
	{
		resp.awardId2 = iter->mValue;
		break;
	}

	idList = GiftManager::getSingleton().getRewardLotteryIDs(RL_VIP);
	for (List<UInt32>::Iter * iter = idList.begin();iter !=NULL;iter = idList.next(iter))
	{
		resp.awardId3 = iter->mValue;
		break;
	}
	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();


	PlayerDailyResetData dailyResetData = player->getPlayerDailyResetData();

	resp.onceLeftTime = onceTime;
	resp.tenLeftTime = tenTime;
	resp.freeOnceLeftTimes = globalValue.uRewardLotteryFreeOnceMaxTimes - dailyResetData.m_RewardLotteryDailyOnceFreeCount;
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

UInt32 GiftManager::getUpDownOne( UInt32 &nowCount,UInt32 maxKey, RewardLotteryTemplate *rewardLotteryTemplate)
{
	UInt32 awardID = 0;
	UInt32 overMaxKeyTimes =  (nowCount - maxKey)% (rewardLotteryTemplate->nextbestneedtimes + 1);

	if ( overMaxKeyTimes>= rewardLotteryTemplate->nextbestneedtimes )
	{
		nowCount = maxKey-1 ;
		awardID = rewardLotteryTemplate->bestaward;
	}
	else if ( overMaxKeyTimes== (rewardLotteryTemplate->nextbestneedtimes -2))
	{
		if((rand()%10000) >= 6666)
		{
			nowCount = maxKey-1 ;
			awardID = rewardLotteryTemplate->bestaward;
		}
		else
		{
			awardID = rewardLotteryTemplate->normalaward;
		}
	}
	else if(overMaxKeyTimes == (rewardLotteryTemplate->nextbestneedtimes-1))
	{
		if((rand()%10000) >= 5000)
		{
			nowCount = maxKey-1 ;
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
	return awardID;
}

UInt32 GiftManager::getRewardLotteryAwardID(Guid playerID,UInt32 id,UInt32 &awardID)
{

	UInt32 getIt = 0;
	UInt32 maxKey = 0;
	UInt32 nowTime = TimeUtil::getTimeSec();	
	Goods goods;
	List<Goods> itemList;

	Player * player  = LogicSystem::getSingleton().getPlayerByGuid(playerID);		  
	RewardLotteryTemplate *rewardLotteryTemplate = gRewardLotteryTable->get(id);
	if (rewardLotteryTemplate == NULL)
	{
		return 0;
	}
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
		int onceTime = 0;
		int tenTime = 0;
		GiftManager::getSingleton().getLotteryLeftTime(player->getPlayerGuid(),onceTime,tenTime);
		if (onceTime != 0)
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

			awardID =  getUpDownOne(counterData.m_RewardLotteryOnceFreeCount, maxKey,rewardLotteryTemplate);
		}
		counterData.m_RewardLotteryOnceTime = nowTime;
		counterData.m_RewardLotteryOnceFreeCount ++;
		PlayerDailyResetData dailyResetData = player->getPlayerDailyResetData();
		dailyResetData.m_RewardLotteryDailyOnceFreeCount++;
		player->setPlayerDailyResetData(dailyResetData);
		player->getPersistManager().setDirtyBit(DAILYRESETBIT);


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
			awardID =  getUpDownOne(counterData.m_RewardLotteryOnceTicketCount, maxKey,rewardLotteryTemplate);
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
			awardID =  getUpDownOne(counterData.m_RewardLotteryOnceCount, maxKey,rewardLotteryTemplate);
		}
		

		goods.resourcestype =AWARD_BASE;
		goods.subtype = AWARD_BASE_GOLD;
		goods.num = 0 - rewardLotteryTemplate->cost;
		itemList.insertTail(goods);
		GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog91);


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
			//第maxKey+1个直接给最好的并重置
			if(counterData.m_RewardLotteryTenCount == maxKey + rewardLotteryTemplate->nextbestneedtimes + 1)
			{
				awardID = rewardLotteryTemplate->bestaward;
			}
			else
			{
				awardID = rewardLotteryTemplate->normalaward;
			}	
		}
		
		goods.resourcestype =AWARD_BASE;
		goods.subtype = AWARD_BASE_GOLD;
		goods.num = 0 - rewardLotteryTemplate->cost;
		itemList.insertTail(goods);
		GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog93);

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
			if (rewardLotteryTemplate->nextbestneedtimes == 0)
			{
				return LynxErrno::NotFound;
			}			
			

		awardID =  getUpDownOne(count, maxKey,rewardLotteryTemplate);
		}
		goods.resourcestype =AWARD_BASE;
		goods.subtype = AWARD_BASE_GOLD;
		goods.num = 0 - rewardLotteryTemplate->cost;
		itemList.insertTail(goods);
		GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog94);
	
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
	else if (rewardLotteryTemplate->type == 7)//十连抽券
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


UInt32 GiftManager::onlyGetRewardLotteryAwardID(Guid playerID,UInt32 id,UInt32 &awardID)
{

	UInt32 getIt = 0;
	UInt32 maxKey = 0;
	UInt32 nowTime = TimeUtil::getTimeSec();	
	Goods goods;
	List<Goods> itemList;

	Player * player  = LogicSystem::getSingleton().getPlayerByGuid(playerID);		  
	RewardLotteryTemplate *rewardLotteryTemplate = gRewardLotteryTable->get(id);
	if (rewardLotteryTemplate == NULL)
	{
		return 0;
	}
	PlayerCounterData counterData = player->GetPlayerCounterData();

	if (rewardLotteryTemplate->refreshtime > 0)
	{
		if (rewardLotteryTemplate->type == 1)
		{
// 			if ((nowTime - counterData.m_RewardLotteryOnceTime) < rewardLotteryTemplate->refreshtime)
// 			{
// 				return LynxErrno::BeingCooled;
// 			}			 
		}		
	}


// 	if (rewardLotteryTemplate->vipneed > player->getVipLevel())
// 	{		
// 		return LynxErrno::VipLevelNotEnough;		
// 	}

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
// 		int onceTime = 0;
// 		int tenTime = 0;
// 		GiftManager::getSingleton().getLotteryLeftTime(player->getPlayerGuid(),onceTime,tenTime);
// 		if (onceTime > 0)
// 		{
// 			return LynxErrno::TimeNotRight;
// 		}	


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
// 			if (rewardLotteryTemplate->nextbestneedtimes == 0)
// 			{
// 				return LynxErrno::NotFound;
// 			}

			awardID =  getUpDownOne(counterData.m_RewardLotteryOnceFreeCount, maxKey,rewardLotteryTemplate);
		}
// 		counterData.m_RewardLotteryOnceTime = nowTime;
// 		counterData.m_RewardLotteryOnceFreeCount ++;
	}
	else if (rewardLotteryTemplate->type == 2)//单抽券
	{
// 		if (counterData.m_RewardLotteryOnceTicket < 1)
// 		{
// 			return LynxErrno::OnceTicketNotEnough;
// 		}

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
// 			if (rewardLotteryTemplate->nextbestneedtimes == 0)
// 			{
// 				return LynxErrno::NotFound;
// 			}
			awardID =  getUpDownOne(counterData.m_RewardLotteryOnceTicketCount, maxKey,rewardLotteryTemplate);
		}
// 		counterData.m_RewardLotteryOnceTicket --;
// 		counterData.m_RewardLotteryOnceTicketCount ++;
	}

	else if (rewardLotteryTemplate->type == 3)//单抽
	{
// 		if (player->getPlayerGold() < rewardLotteryTemplate->cost)
// 		{
// 			return LynxErrno::RmbNotEnough;
// 		}

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
// 			if (rewardLotteryTemplate->nextbestneedtimes == 0)
// 			{
// 				return LynxErrno::NotFound;
// 			}
			awardID =  getUpDownOne(counterData.m_RewardLotteryOnceCount, maxKey,rewardLotteryTemplate);
		}


// 		goods.resourcestype =AWARD_BASE;
// 		goods.subtype = AWARD_BASE_GOLD;
// 		goods.num = 0 - rewardLotteryTemplate->cost;
// 		itemList.insertTail(goods);
// 		GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog91);


// 		counterData.m_RewardLotteryOnceCount ++;
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
// 			if (rewardLotteryTemplate->nextbestneedtimes == 0)
// 			{
// 				return LynxErrno::NotFound;
// 			}
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
// 		if (player->getPlayerGold() < rewardLotteryTemplate->cost)
// 		{
// 			return LynxErrno::RmbNotEnough;
// 		}


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
// 			if (rewardLotteryTemplate->nextbestneedtimes == 0)
// 			{
// 				return LynxErrno::NotFound;
// 			}
			//第maxKey+1个直接给最好的并重置
			if(counterData.m_RewardLotteryTenCount == maxKey + rewardLotteryTemplate->nextbestneedtimes + 1)
			{
				awardID = rewardLotteryTemplate->bestaward;
			}
			else
			{
				awardID = rewardLotteryTemplate->normalaward;
			}	
		}

// 		goods.resourcestype =AWARD_BASE;
// 		goods.subtype = AWARD_BASE_GOLD;
// 		goods.num = 0 - rewardLotteryTemplate->cost;
// 		itemList.insertTail(goods);
// 		GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog93);

	}
	else if (rewardLotteryTemplate->type == 6)//vip抽
	{
// 		if(player->getVipLevel() < rewardLotteryTemplate->vipneed)
// 		{
// 			return LynxErrno::VipLevelNotEnough;
// 
// 		}
// 		if (player->getPlayerGold() < rewardLotteryTemplate->cost)
// 		{
// 			return LynxErrno::RmbNotEnough;
// 		}
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
// 			if (rewardLotteryTemplate->nextbestneedtimes == 0)
// 			{
// 				return LynxErrno::NotFound;
// 			}			


			awardID =  getUpDownOne(count, maxKey,rewardLotteryTemplate);
		}
// 		goods.resourcestype =AWARD_BASE;
// 		goods.subtype = AWARD_BASE_GOLD;
// 		goods.num = 0 - rewardLotteryTemplate->cost;
// 		itemList.insertTail(goods);
// 		GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog94);

// 		count ++;
// 		if (rewardLotteryTemplate->id == tmpAwardID)
// 		{
// 			counterData.m_RewardLotteryVipDefaultCount = count;
// 		}
// 		else
// 		{
// 			counterData.m_RewardLotteryVipElseCount = count;
// 		}
	}
	else if (rewardLotteryTemplate->type == 7)//十连抽券
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

// 	player->setPlayerCounterData(counterData);
// 	player->getPersistManager().setDirtyBit(COUNTERDATABIT);


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

	int onceTime = 0;
	int tenTime = 0;
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

void GiftManager::canRewardLotteryType(UInt64 playerID, UInt32 &reqType)
{
	int onceTime = 0;
	int tenTime = 0;

	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}

	PlayerCounterData counterData;
	counterData = player->GetPlayerCounterData();//位置不能换

	GiftManager::getSingleton().getLotteryLeftTime(playerID,onceTime,tenTime);
	if (reqType == BIG_RL_ONCE)
	{
		if (onceTime == 0)
		{
			reqType = RL_ONCE_FREE;	
			return;
		}
		else if (counterData.m_RewardLotteryOnceTicket > 0)
		{
			reqType = RL_ONCE_FREE;			
			return;
		}
		else
		{
			reqType = RL_ONCE;
			return;
		}
		return;

	}
	else if (reqType == BIG_RL_TEN)
	{
		if (tenTime == 0)
		{
			reqType = RL_TEN_FREE;	
			return;
		}
		else if (counterData.m_RewardLotteryTenTicket > 0)
		{
			reqType = RL_TEN_TICKET;	
			return;
		}
		else
		{
			reqType = RL_TEN;
			return;
		}
		return;
	}
	else if (reqType == BIG_RL_VIP)
	{
		reqType = RL_VIP;
		return;
	}

}
void GiftManager::onRewardLotteryReq(const  ConnId& connId ,RewardLotteryReq & msg )
{

	UInt32 maxTimes = 1;
	bool bFlag = false;
	Goods goods;
	UInt32 countNum = 1;
	UInt32 flag = 0;
	int onceTime = 0;
	int tenTime = 0;
	UInt32 reqType = 0;
	UInt32 nowTime = TimeUtil::getTimeSec();	
	List<UInt32> idList;
	List<UInt32> awardIDList;	
	List<Goods> itemList;
	List<Goods> tmpItemList;	
	RewardLotteryTemplate rewardLotteryTemplate3;
	RewardLotteryTemplate rewardLotteryTemplate6;
	PlayerCounterData counterData;
	Json::Value jsonValue;
	RewardLotteryResp resp;


	Player * player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}
	msg.convertJsonToData(msg.strReceive);
	resp.reqType = msg.reqType;
	reqType = msg.reqType;

	GiftManager::getSingleton().canRewardLotteryType(player->getPlayerGuid(),reqType);
	

	resp.result = GiftManager::getSingleton().checkRewardLotteryTen(player->getPlayerGuid(),reqType);
	if ( resp.result != LynxErrno::None)
	{
		std::string jsonStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,OPEN_REWARD_LOTTERY_RESP,jsonStr);
		return;
	}

	idList = GiftManager::getSingleton().getRewardLotteryIDs(reqType);


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
		if (iter->mValue.type == reqType)
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
	if(reqType == RL_TEN )
	{
		//元宝在里面扣
		if (counterData.m_RewardLotteryTenCount >= maxKey + rewardLotteryTemplate->nextbestneedtimes +1)
		{
			counterData.m_RewardLotteryTenCount = maxKey-1 ;
		}
		counterData.m_RewardLotteryTenCount ++;	
	}
	else if (reqType == RL_TEN_TICKET )		
	{
		counterData.m_RewardLotteryTenTicket --;
		if (counterData.m_RewardLotteryTenTicketCount >= maxKey + rewardLotteryTemplate->nextbestneedtimes +1)
		{
			counterData.m_RewardLotteryTenTicketCount = maxKey-1 ;
		}
		counterData.m_RewardLotteryTenTicketCount ++;	
	}
	else if (reqType == RL_TEN_FREE )		
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
	

	//awardcontent 改成award
	for (List<UInt32>::Iter * awardIter = awardIDList.begin();awardIter !=NULL;awardIter = awardIDList.next(awardIter))
	{
		Award award;
		tmpItemList.clear();
		GiftManager::getSingleton().getAwardByID(awardIter->mValue,0, tmpItemList);	
	
		LOG_INFO("RewardLottery awardid  = %d ",awardIter->mValue);
		for(List<Goods>::Iter *contentIter = tmpItemList.begin();contentIter!= NULL;contentIter = tmpItemList.next(contentIter))
		{
			LOG_INFO("RewardLottery awardcontentid  = %d ",contentIter->mValue.subtype);
			GiftManager::getSingleton().getContentByID(contentIter->mValue.subtype,award.award);
		}
		resp.awards.insertTail(award);
	}

	for(List<Award>::Iter * item2 = resp.awards.begin();item2 != NULL; item2 = resp.awards.next(item2))
	{
		itemList += item2->mValue.award;
	}
	

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



	Json::Value stages;
	if (reqType == 1)
	{
		GiftManager::getSingleton().saveEndsAttrNotCombine(player->getPlayerGuid(),itemList,resp.allAttr,stages,MiniLog90);
	}
	else if (reqType == 2)
	{
		GiftManager::getSingleton().saveEndsAttrNotCombine(player->getPlayerGuid(),itemList,resp.allAttr,stages,MiniLog95);
	}
	else if (reqType == 3)
	{
		GiftManager::getSingleton().saveEndsAttrNotCombine(player->getPlayerGuid(),itemList,resp.allAttr,stages,MiniLog91);
	}
	else if (reqType == 4)
	{
		GiftManager::getSingleton().saveEndsAttrNotCombine(player->getPlayerGuid(),itemList,resp.allAttr,stages,MiniLog92);
	}
	else if (reqType == 5)
	{
		GiftManager::getSingleton().saveEndsAttrNotCombine(player->getPlayerGuid(),itemList,resp.allAttr,stages,	MiniLog93);
	}
	else if (reqType == 6)
	{
		GiftManager::getSingleton().saveEndsAttrNotCombine(player->getPlayerGuid(),itemList,resp.allAttr,stages,MiniLog94);
	}
	else if (reqType == 7)
	{
		GiftManager::getSingleton().saveEndsAttrNotCombine(player->getPlayerGuid(),itemList,resp.allAttr,stages,MiniLog96);
	}
	else
	{
		GiftManager::getSingleton().saveEndsAttrNotCombine(player->getPlayerGuid(),itemList,resp.allAttr,stages,MiniLog97);
	}
	

// 	GiftManager::getSingleton().PlayerItemChangeResult(player->getPlayerGuid(),0,itemList);

	counterData = player->GetPlayerCounterData();//位置不能换
	
	//for(List<UInt32>::Iter * iter1 = resp.newServant.begin();iter1 != NULL;iter1 = resp.newServant.next(iter1))
	//{
	//	 ServantTemplate * servantTemp = SERVANT_TABLE().get(iter1->mValue);
	//	 if (servantTemp->mQuality ==4)
	//	 {			
	//		 LogicSystem::getSingleton().sendSystemMsg(2, player->getPlayerName(), iter1->mValue);//紫色和橙色
	//	 }
	//	 if (servantTemp->mQuality ==5)
	//	 {
	//		 LogicSystem::getSingleton().sendSystemMsg(1, player->getPlayerName(), iter1->mValue);//紫色和橙色
	//	 }
 //	
	//}
	//
	if (reqType == RL_TEN)
	{
		assert(resp.awards.size()==10);
	}
	

	GiftManager::getSingleton().getLotteryLeftTime(player->getPlayerGuid(),onceTime,tenTime);
	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();
	PlayerDailyResetData dailyResetData = player->getPlayerDailyResetData();

	resp.gold = player->getPlayerGold();
	resp.onceLeftTime = onceTime;
	resp.tenLeftTime = tenTime;
	resp.freeOnceLeftTimes = globalValue.uRewardLotteryFreeOnceMaxTimes - dailyResetData.m_RewardLotteryDailyOnceFreeCount;

	
	resp.couponOnceNum = counterData.m_RewardLotteryOnceTicket;
	resp.couponTenNum = counterData.m_RewardLotteryTenTicket;
	resp.ends = itemList;


// 	//for test
// 	resp.rlOnceFreeCount = counterData.m_RewardLotteryOnceFreeCount;
// 	resp.rlOnceTicketCount = counterData.m_RewardLotteryOnceTicketCount;
// 	resp.rlOnceCount = counterData.m_RewardLotteryOnceCount;
// 	resp.rlTenFreeCount = counterData.m_RewardLotteryTenFreeCount;
// 	resp.rlTenTicketCount = counterData.m_RewardLotteryTenTicketCount;
// 	resp.rlTenCount = counterData.m_RewardLotteryTenCount;
// 	resp.rlVipDefaultCount = counterData.m_RewardLotteryVipDefaultCount;
// 	resp.rlVipAwardID = counterData.m_RewardLotteryVipAwardID;
// 	resp.rlVipElseCount = counterData.m_RewardLotteryVipElseCount;

	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,OPEN_REWARD_LOTTERY_RESP,jsonStr);
	LOG_INFO("RewardLottery resp  = %s ",jsonStr.c_str());

	

	//添加成就打点 wwc
	if(reqType == RL_ONCE_FREE || reqType == RL_ONCE_TICKET || reqType == RL_ONCE)	
	{
		player->getAchieveManager().updateAchieveData(LINGRENCALLSERVANT,1);
		//每日任务打点 wwc
		player->getAchieveManager().updateDailyTaskData(DLYLINGRENCALLCNT, 1 );
	}
	else if(reqType == RL_VIP)
	{
		player->getAchieveManager().updateAchieveData(MINGLINGCALLSERVANT,1);
	}
	else if(reqType == RL_TEN_FREE || reqType == RL_TEN_TICKET || reqType == RL_TEN)
	{
		player->getAchieveManager().updateAchieveData(XIBANCALLSERVANT,1);
		//每日任务打点wwc
		player->getAchieveManager().updateDailyTaskData(DLYXIBANCALLCNT, 1 );
	}
	if(reqType == RL_TEN_FREE || reqType == RL_TEN_TICKET || reqType == RL_TEN)
	{
		//更新七日训
		LogicSystem::getSingleton().updateSevenDayTask(player->getPlayerGuid(),SDT08,10);
	}
	else
	{
		//更新七日训
		LogicSystem::getSingleton().updateSevenDayTask(player->getPlayerGuid(),SDT08,1);
	}
	
	
}
bool GiftManager::findValueExit(UInt32 id, List<UInt32> ids)
{
	for( List<UInt32>::Iter *iter = ids.begin();iter!=NULL;iter = ids.next(iter))
	{		
		if (id == iter->mValue)
		{
			return true;
		}
	}
	return false;

}

void GiftManager::servanNeedChangeToPiece(Guid playerID, List<Award> &awards,List<Goods> &ends)
{
	Goods goods;	
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	List<UInt32> newServantIds;

	
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
				if(player->getServantManager().isServantExist(iter->mValue.subtype) == true ||findValueExit(iter->mValue.subtype,newServantIds) == true)
				{
					goods.resourcestype = AWARD_SERVANTPIECE;
					goods.subtype = iter->mValue.subtype;
					goods.num = iter->mValue.num * servantTemp->mPieceCount;
					item->mValue.award.insertTail(goods);
					item->mValue.award.erase(iter);
				}
				else
				{
					newServantIds.insertTail(iter->mValue.subtype);

					goods.resourcestype = AWARD_SERVANTPIECE;
					goods.subtype = iter->mValue.subtype;
					goods.num = (iter->mValue.num - 1) * servantTemp->mPieceCount;
					if (goods.num > 0)
					{
						item->mValue.award.insertTail(goods);
					}						
					iter->mValue.num = 1;		

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


void GiftManager::servanNeedChangeToPieceEX(Guid playerID, List<Goods> &itemList,List<Goods> &ends)
{
	Goods goods;
	List<UInt32> newServantIds;
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerID);


	GiftManager::getSingleton().combineSame(itemList);

	//先删除AWARD_SERVANT 在加到ends
	for( List<Goods>::Iter * iter = itemList.begin();iter != NULL;iter = itemList.next(iter))
	{
		if (iter->mValue.resourcestype == AWARD_SERVANT)
		{
			ServantTemplate * servantTemp = SERVANT_TABLE().get(iter->mValue.subtype);
			if (servantTemp == NULL)
			{
				continue;//todo 多个情况有问题
			}
			if(player->getServantManager().isServantExist(iter->mValue.subtype) == true||findValueExit(iter->mValue.subtype,newServantIds) == true)
			{
				goods.resourcestype = AWARD_SERVANTPIECE;
				goods.subtype = iter->mValue.subtype;
				goods.num = iter->mValue.num * servantTemp->mPieceCount;
				itemList.insertTail(goods);
				itemList.erase(iter);
			}
			else
			{
				goods.resourcestype = AWARD_SERVANTPIECE;
				goods.subtype = iter->mValue.subtype;
				goods.num = (iter->mValue.num - 1) * servantTemp->mPieceCount;
				if (goods.num > 0)
				{
					itemList.insertTail(goods);
				}						
				iter->mValue.num = 1;	

			}
		}
	}

	ends += itemList;//先存储stage信息，再删除

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
void GiftManager::getLotteryLeftTime(Guid playerID,int &onceTime,int &tenTime)//
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

	
	if (nowTime < counterData.m_RewardLotteryOnceTime)
	{
		counterData.m_RewardLotteryOnceTime = nowTime;
		player->setPlayerCounterData(counterData);
		player->getPersistManager().setDirtyBit(COUNTERDATABIT);
	}
	int tmpOnceTime = rewardLotteryTemplate1.refreshtime - ( nowTime - counterData.m_RewardLotteryOnceTime);
	if (tmpOnceTime <0)
	{
		tmpOnceTime = 0;
	}

	if (nowTime < counterData.m_RewardLotteryTenTime)
	{
		counterData.m_RewardLotteryTenTime = nowTime;
		player->setPlayerCounterData(counterData);
		player->getPersistManager().setDirtyBit(COUNTERDATABIT);
	}
	int tmpTenTime = rewardLotteryTemplate4.refreshtime - ( nowTime - counterData.m_RewardLotteryTenTime);
	if (tmpTenTime <0)
	{
		tmpTenTime = 0;
	}

	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();

	PlayerDailyResetData dailyResetData = player->getPlayerDailyResetData();

	if ( dailyResetData.m_RewardLotteryDailyOnceFreeCount >= globalValue.uRewardLotteryFreeOnceMaxTimes)
	{
		tmpOnceTime = -1;
	}

	onceTime = tmpOnceTime;
	tenTime = tmpTenTime;
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

void GiftManager::saveEndsGetAttr(Guid playerID,List<Goods> &ends,Json::Value &allAttr,UInt32 systemID= 0)
{
	Json::Value stages;
	saveEndsAttr(playerID,ends,allAttr,stages,systemID);

}

void GiftManager::saveEndsAttr(Guid playerID,List<Goods> &ends,Json::Value &allAttr,Json::Value &stages,UInt32 systemID = 0)
{
	List<Goods> stagesEnds;
	List<Goods> stagesNullEnds;
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerID);


	GiftManager::getSingleton().combineSame(ends);
	for(List<Goods>::Iter *endsIter = ends.begin();endsIter!=NULL;endsIter = ends.next(endsIter))
	{
		if (endsIter->mValue.resourcestype == AWARD_STAGEDATA||endsIter->mValue.resourcestype == AWARD_TWELVEPALACE_STAGEDATA)
		{
			stagesEnds.insertTail(endsIter->mValue);
		}
		else
		{
			stagesNullEnds.insertTail(endsIter->mValue);
		}
	}

	List<ReturnItemEle> rtItemList;
	GiftManager::getSingleton().addToPlayerAttr(player->getPlayerGuid(),rtItemList,allAttr, stagesNullEnds,systemID);

	List<ReturnItemEle> stageRtItemList;
	GiftManager::getSingleton().addToPlayerAttr(player->getPlayerGuid(),stageRtItemList,stages, stagesEnds,systemID);

	GiftManager::getSingleton().PlayerItemChangeResult(player->getPlayerGuid(),0,ends);
}


void GiftManager::saveEndsAttrNotCombine(Guid playerID,List<Goods> &ends,Json::Value &allAttr,Json::Value &stages,UInt32 systemID = 0)
{
	List<Goods> stagesEnds;
	List<Goods> stagesNullEnds;
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerID);


	for(List<Goods>::Iter *endsIter = ends.begin();endsIter!=NULL;endsIter = ends.next(endsIter))
	{
		if (endsIter->mValue.resourcestype == AWARD_STAGEDATA||endsIter->mValue.resourcestype == AWARD_TWELVEPALACE_STAGEDATA)
		{
			stagesEnds.insertTail(endsIter->mValue);
		}
		else
		{
			stagesNullEnds.insertTail(endsIter->mValue);
		}
	}

	List<ReturnItemEle> rtItemList;
	GiftManager::getSingleton().addToPlayerAttr(player->getPlayerGuid(),rtItemList,allAttr, stagesNullEnds,systemID);

	List<ReturnItemEle> stageRtItemList;
	GiftManager::getSingleton().addToPlayerAttr(player->getPlayerGuid(),stageRtItemList,stages, stagesEnds,systemID);

	GiftManager::getSingleton().PlayerItemChangeResult(player->getPlayerGuid(),0,ends);
}