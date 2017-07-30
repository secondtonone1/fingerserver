#include "StageManager.h"
#include "Player.h"
#include "../LogicSystem.h"
#include "../FireConfirm/Gift.h"
#include "CommonLib/PlayerData.h"
#include "Ranking/Ranking.h"
#include "../FireConfirm/StageCalc.h"
#include "../FireConfirm/GlobalValue.h"
#include "../TimeManager.h"

using namespace Lynx;

StageManager::StageManager()
{
	m_pPlayer = NULL;
	m_pListChapterData = NULL;

}


StageManager:: ~StageManager()
{

}


void StageManager::refreshDailyresetData(UInt64 playerID)
{
	UInt32 refreshTime  = 0;
	List<UInt32> refreshList;

	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	PlayerDailyResetData dailyResetData= player->getPlayerDailyResetData();
	dailyResetData.m_nTwelvePalaceResetFlag = 0;
	player->setPlayerDailyResetData(dailyResetData);


	for(UInt32 i = 0;i<24;i++)
	{
		refreshTime = i*3600;
		if(TimeManager::timeIsTodayDelayHours(dailyResetData.m_nLastUpdateTime,i) == false)		
		{
			refreshList.clear();			
			LogicSystem::getSingleton().getDailyRefreshList(refreshTime,refreshList);				
			LogicSystem::getSingleton().updateDailyResetData(player,i,refreshList);
		}
	}


}

bool StageManager::initial(Player* player)
{

	UInt32 chapterID = 0;
	UInt32 star=0;
	UInt32 isStageBegin = 0;
	UInt32 isChapterBegin = 0;
	String strData ;
	char dest[64]={};
	Goods goods;
	StageData stageData;
	PlayerChapterData chapterData;

	gDocument.SetObject();
	m_pPlayer = player;
	m_pListChapterData = &player->mPlayerData.mChapterListData.m_listChapterData;

	for(List<UInt32>::Iter * it = gFirstAddStage.begin();it != NULL; it = gFirstAddStage.next(it))//更新n个关卡
	{
		isChapterBegin =0;
		isStageBegin =0;
		chapterID =getChapterID(it->mValue);
		for (List<PlayerChapterData>::Iter * iter = m_pListChapterData->begin();iter != NULL;iter = m_pListChapterData->next(iter))
		{
			if (iter->mValue.m_nChapterId == chapterID)
			{
				isChapterBegin = 1;
				for(List<StageData>::Iter *iter1 = iter->mValue.m_listStageDatas.begin();iter1 != NULL; iter1 = iter->mValue.m_listStageDatas.next(iter1))
				{

					if (iter1->mValue.m_nStageID == it->mValue)
					{
						isStageBegin = 1;
						break;
					}
				}
				if (isStageBegin == 0)
				{
					stageData.m_nChallengTimes =0;
					stageData.m_nLastChallengTime = 0;
					stageData.m_nGetStar = 0;
					stageData.m_nStageID = it->mValue;
					iter->mValue.m_listStageDatas.insertTail(stageData);


					needSaveDateList.insertTail(it->mValue);
					// 					chapterData = iter->mValue;
					// 					// 					chapterData.m_listStageDatas.clear();
					// 					chapterData.m_listStageDatas.insertTail(stageData);
				}			

				break;
			}

		}
		if (isChapterBegin == 0)
		{
			stageData.m_nChallengTimes =0;
			stageData.m_nLastChallengTime = 0;
			stageData.m_nGetStar = 0;
			stageData.m_nStageID = it->mValue;

			chapterData.m_listStageDatas.clear();
			chapterData.m_nChapterId = chapterID;		
			chapterData.m_nIselite = 0;
			chapterData.m_nTreasure1getstate = 0;
			chapterData.m_nTreasure2getstate = 0;
			chapterData.m_nTreasure3getstate = 0;
			chapterData.m_listStageDatas.insertTail(stageData);
			chapterData.m_nChapterstarttime = TimeUtil::getTimeSec();
			chapterData.m_nChapterfinishtime = 0;

			m_pListChapterData->insertTail(chapterData);

			needSaveDateList.insertTail(it->mValue);
		}
	}


	for(List<PlayerChapterData>::Iter * iterChapter = m_pListChapterData->begin(); iterChapter != NULL;
		iterChapter = m_pListChapterData->next(iterChapter))
	{
		m_mapIDToChapter.insert(iterChapter->mValue.m_nChapterId,&iterChapter->mValue);
	}

	return true;
}

void StageManager::stageReset(Player* player,bool needSend,UInt32 stageType)
{

	m_pListChapterData = &player->mPlayerData.mChapterListData.m_listChapterData;



	for (List<PlayerChapterData>::Iter * iter = m_pListChapterData->begin();iter != NULL;iter = m_pListChapterData->next(iter))
	{

		for(List<StageData>::Iter *iter1 = iter->mValue.m_listStageDatas.begin();iter1 != NULL; iter1 = iter->mValue.m_listStageDatas.next(iter1))
		{
			if (iter1->mValue.m_nStageID == 0)
			{
				continue;
			}
			// 是否所有的关卡都要重置			
			if (gStageTable->get(iter1->mValue.m_nStageID)->mType != stageType)
			{
				continue;
			}


			if(iter1->mValue.m_nChallengTimes != 0)
			{
				needSaveDateList.insertTail(iter1->mValue.m_nStageID);
			}
			if (needSend == false)
			{
				player->getStageManager().addNeedSaveDateList(iter1->mValue.m_nStageID);			

			}
			iter1->mValue.m_nChallengTimes = 0;
		}
	}

	player->getPersistManager().setDirtyBit(STAGEDATABIT);//上面有保存的

	if(needSend == true)
	{
		StageManager::getSingleton().saveAndSend(player->getPlayerGuid(), player->getChapterList(),needSaveDateList);
	}	
}

bool StageManager::initePreStageIDs()
{
	Vector<String> strVector;

	gStageTable->mMap += gPalacesStageTable->mMap;

	Map<UInt64, StageTemplate>::Iter *stageIter = gStageTable->mMap.find(XINSHOUYINDAOSTAGE);
	gStageTable->mMap.erase(stageIter);

	for(Map<UInt64, StageTemplate>::Iter *iter = gStageTable->mMap.begin();iter!=NULL;iter = gStageTable->mMap.next(iter))
	{
		if (iter->mValue.mType == STAGE_NORMAL ||iter->mValue.mType == STAGE_ELITE ||iter->mValue.mType == STAGE_BONUS ||iter->mValue.mType == STAGE_MASTAR)
		{
 			continue;
			// 			STAGE_NORMAL	 = 1,	//普通
			// 				STAGE_ELITE		 = 2,	//精英2
			// 				STAGE_BONUS		 = 3,	//奖励3
			// 				STAGE_MASTAR	 = 4,	//大师4
		}
		strVector.clear();
		lynxStrSplit(iter->mValue.mStrFinShowList, ";", strVector, true);
		for(UInt32 i=0;i<strVector.size();i++)
		{
			gPreStageIDs.insert(atoi(strVector[i].c_str()),iter->mValue.mStageId);
		}

	}
	for(Map<UInt64, StageTemplate>::Iter *iter = gStageTable->mMap.begin();iter!=NULL;iter = gStageTable->mMap.next(iter))
	{
		if (iter->mValue.mType == STAGE_NORMAL ||iter->mValue.mType == STAGE_ELITE ||iter->mValue.mType == STAGE_BONUS ||iter->mValue.mType == STAGE_MASTAR)
		{
			continue;
			// 			STAGE_NORMAL	 = 1,	//普通
			// 				STAGE_ELITE		 = 2,	//精英2
			// 				STAGE_BONUS		 = 3,	//奖励3
			// 				STAGE_MASTAR	 = 4,	//大师4
		}

		UInt32 iFlag =0;
		for(Map<UInt32,UInt32>::Iter *it = gPreStageIDs.begin();it !=NULL;it = gPreStageIDs.next(it))
		{
			if (iter->mKey == it->mKey)
			{
				iFlag =1;
				break;
			}

		}
		if (iFlag ==0)
		{
			gFirstAddStage.insertTail(iter->mKey);
		}
	}

	for(Map<UInt64, StageTemplate>::Iter *iter = gStageTable->mMap.begin();iter!=NULL;iter = gStageTable->mMap.next(iter))
	{

		if (iter->mValue.mType == STAGE_NORMAL ||iter->mValue.mType == STAGE_ELITE ||iter->mValue.mType == STAGE_BONUS ||iter->mValue.mType == STAGE_MASTAR)
		{
		
			strVector.clear();
			lynxStrSplit(iter->mValue.mStartLimit1, ";", strVector, true);
			if(strVector.size() != 2)
			{
				continue;
			}

			if ( atoi(strVector[1].c_str()) == 1)
			{
				gFirstAddStage.insertTail(iter->mValue.mStageId);
			}

		}

	}
	return true;
}


void StageManager::release()
{
	m_pPlayer = NULL;
	m_pListChapterData = NULL;
	m_mapIDToChapter.clear();
}

std::string StageManager::convertStageDataToJson()
{
	MsgStageDataToJson stageDataToJson;
	stageDataToJson.m_mapIDToChapter = m_mapIDToChapter;
	Json::Value node;
	return stageDataToJson.convertDataToJson(node);
}

UInt32 StageManager::getChapterID(UInt32 stageID)
{
	StageTemplate* stageTemplate = gStageTable->get(stageID);
	if (stageTemplate == NULL)
	{
		return 0;
	}

	return stageTemplate->mChapterId;
}

UInt32 StageManager::binarytoStarNum(UInt32 num)
{
	UInt32 star = 0;
	if ((num & 0x00000001)== 0x00000001)
	{
		star ++;
	}
	if ((num & 0x00000002)== 0x00000002)
	{
		star ++;
	}
	if ((num & 0x00000004)== 0x00000004)
	{
		star ++;
	}
	if ((num & 0x00000008)== 0x00000008)
	{
		star ++;
	}
	if ((num & 0x00000010)== 0x00000010)
	{
		star ++;
	}
	if ((num & 0x00000020)== 0x00000020)
	{
		star ++;
	}
	if ((num & 0x00000040)== 0x00000040)
	{
		star ++;
	}

	return star;

}

UInt32 StageManager::addStageData(Guid playerID,UInt32 copyID,Int32 num)//活动副本
{
	String strData ;
	char dest[64]={};

	UInt32 chapterID;
	StageData stageData;
	UInt32 star;
	UInt32 binaryNum;
	UInt32 isStageBegin = 0;
	UInt32 isChapterBegin = 0;
	UInt32 nowTime = TimeUtil::getTimeSec();
	PlayerChapterData chapterData;
	PlayerChapterList chapterList;

	if (copyID == 0)
	{
		return LynxErrno::NotFound;
	}

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	PlayerFireConfirmData mFireConfirmData =  player->GetFireConfirmData();
	chapterList = player->getChapterList();//顺序不能动
	if(mFireConfirmData.m_CopyID != copyID)
	{
		List<UInt32> stageIDs;
		stageIDs.insertTail(copyID);	
		saveAndSend(player->getPlayerGuid(), chapterList,stageIDs);
		return 0;
	}
	binaryNum = mFireConfirmData.m_Star;

	star = binarytoStarNum(binaryNum);


	chapterID = StageManager::getSingleton().getChapterID(copyID);

	for (List<PlayerChapterData>::Iter * iter = chapterList.m_listChapterData.begin();iter != NULL;iter = chapterList.m_listChapterData.next(iter))
	{
		if (iter->mValue.m_nChapterId == chapterID)
		{
			isChapterBegin = 1;

			for(List<StageData>::Iter *iter1 = iter->mValue.m_listStageDatas.begin();iter1 != NULL; iter1 = iter->mValue.m_listStageDatas.next(iter1))
			{

				if (iter1->mValue.m_nStageID == copyID)
				{
					isStageBegin = 1;
					iter1->mValue.m_nChallengTimes += num;
					iter1->mValue.m_nLastChallengTime = nowTime;


					if (star > iter1->mValue.m_nGetStar%10)
					{
						iter1->mValue.m_nGetStar = binaryNum * 10 + star;						
					}
					else if (star ==0)//扫荡星数为0 不做修改
					{
					}
					else
					{
						iter1->mValue.m_nGetStar =  binaryNum * 10 +  iter1->mValue.m_nGetStar % 10 ;//把后面一个星的值去掉再加

					}
					star = iter1->mValue.m_nGetStar;
					break;
				}
			}

			if (isStageBegin == 0)
			{
				stageData.m_nChallengTimes =1;
				stageData.m_nLastChallengTime = nowTime;
				stageData.m_nGetStar = star;
				stageData.m_nStageID = copyID;
				iter->mValue.m_listStageDatas.insertTail(stageData);
			}
			// 			chapterData = iter->mValue;
			// 			chapterData.m_listStageDatas.clear();
			// 			chapterData.m_listStageDatas.insertTail(stageData);
			break;
		}
	}
	if (isChapterBegin == 0)
	{
		stageData.m_nChallengTimes =1;
		stageData.m_nLastChallengTime = nowTime;
		stageData.m_nGetStar = star;
		stageData.m_nStageID = copyID;

		chapterData.m_nChapterId = chapterID;		
		chapterData.m_nIselite = 0;
		chapterData.m_nTreasure1getstate = 0;
		chapterData.m_nTreasure2getstate = 0;
		chapterData.m_nTreasure3getstate = 0;
		chapterData.m_listStageDatas.insertTail(stageData);
		chapterData.m_nChapterstarttime = nowTime;
		chapterData.m_nChapterfinishtime = 0;

		chapterList.m_listChapterData.insertTail(chapterData);
	}
	player->setchapterList(chapterList);
	if (star > 0)//大于0星要开启下一关
	{
		showNexStage( playerID, copyID, num);
	}
	else
	{
		List<UInt32> stageIDs;
		stageIDs.insertTail(copyID);	
		saveAndSend(player->getPlayerGuid(), chapterList,stageIDs);
	}

	return 1;
}


UInt32 StageManager::showNexStage(Guid playerID,UInt32 copyID,Int32 num)//活动副本
{
	String strData ;
	char dest[64]={};	
	UInt32 chapterID;
	StageData stageData;
	UInt32 isStageBegin = 0;
	UInt32 isChapterBegin = 0;	
	UInt32 nextStageID =0;
	List<UInt32> stageIDs;
	PlayerChapterList chapterList;
	PlayerChapterData chapterData;


	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);

	StageTemplate *stageTemplate = gStageTable->get(copyID);
	if (stageTemplate == NULL)
	{
		LOG_WARN("stageTemplate not found!!");
		return 0;
	}
	Vector<String> strVector;
	lynxStrSplit(stageTemplate->mStrFinShowList, ";", strVector, true);

	chapterList = player->getChapterList();

	stageIDs.insertTail(copyID);

	for(int i = 0;i<strVector.size();i++)
	{
		nextStageID = atoi(strVector[i].c_str());
		chapterID = getChapterID(nextStageID);

		isChapterBegin = 0;
		for (List<PlayerChapterData>::Iter * iter = chapterList.m_listChapterData.begin();iter != NULL;iter = chapterList.m_listChapterData.next(iter))
		{
			if (iter->mValue.m_nChapterId == chapterID)
			{
				isChapterBegin = 1;
				isStageBegin = 0;

				for(List<StageData>::Iter *iter1 = iter->mValue.m_listStageDatas.begin();iter1 != NULL; iter1 = iter->mValue.m_listStageDatas.next(iter1))
				{
					if (iter1->mValue.m_nStageID == nextStageID)
					{
						isStageBegin = 1;
						break;
					}
				}

				if (isStageBegin == 0)
				{
					stageData.m_nStageID = nextStageID;
					iter->mValue.m_listStageDatas.insertTail(stageData);

					stageIDs.insertTail(nextStageID);
				}	
				break;
			}
		}
		if (isChapterBegin == 0)
		{
			stageData.m_nStageID = nextStageID;
			chapterData.m_nChapterId = chapterID;	
			chapterData.m_listStageDatas.insertTail(stageData);
			chapterList.m_listChapterData.insertTail(chapterData);
			stageIDs.insertTail(nextStageID);
		}
	}

	saveAndSend(player->getPlayerGuid(), chapterList,stageIDs);
	return 1;
}


UInt32 StageManager::getOpenChapter(Guid playerID,UInt32 copyID,Int32 num,List<Goods> &stageIDList)
{
	String strData ;
	char dest[64]={};	
	UInt32 chapterID;
	StageData stageData;
	UInt32 isStageBegin = 0;
	UInt32 isChapterBegin = 0;	
	UInt32 nextStageID =0;
	List<UInt32> stageIDs;
	PlayerChapterList chapterList;
	PlayerChapterData chapterData;


	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);

	StageTemplate *stageTemplate = gStageTable->get(copyID);
	if (stageTemplate == NULL)
	{
		LOG_WARN("stageTemplate not found!!");
		return 0;
	}
	Vector<String> strVector;
	lynxStrSplit(stageTemplate->mStrFinShowList, ";", strVector, true);

	chapterList = player->getChapterList();

	stageIDs.insertTail(copyID);

	for(int i = 0;i<strVector.size();i++)
	{
		nextStageID = atoi(strVector[i].c_str());
		chapterID = getChapterID(nextStageID);

		isChapterBegin = 0;
		for (List<PlayerChapterData>::Iter * iter = chapterList.m_listChapterData.begin();iter != NULL;iter = chapterList.m_listChapterData.next(iter))
		{
			if (iter->mValue.m_nChapterId == chapterID)
			{
				isChapterBegin = 1;
				isStageBegin = 0;

				for(List<StageData>::Iter *iter1 = iter->mValue.m_listStageDatas.begin();iter1 != NULL; iter1 = iter->mValue.m_listStageDatas.next(iter1))
				{
					if (iter1->mValue.m_nStageID == nextStageID)
					{
						isStageBegin = 1;
						break;
					}
				}

				if (isStageBegin == 0)
				{
					stageData.m_nStageID = nextStageID;
					iter->mValue.m_listStageDatas.insertTail(stageData);

					stageIDs.insertTail(nextStageID);
				}	
				break;
			}
		}
		if (isChapterBegin == 0)
		{
			stageData.m_nStageID = nextStageID;
			chapterData.m_nChapterId = chapterID;	
			chapterData.m_listStageDatas.insertTail(stageData);
			chapterList.m_listChapterData.insertTail(chapterData);
			stageIDs.insertTail(nextStageID);
		}
	}

	for (List<UInt32>::Iter * it = stageIDs.begin();it!=NULL;it = stageIDs.next(it))
	{
		StageTemplate *newStageTemplate = gStageTable->get(it->mValue);
		if (newStageTemplate == NULL)
		{
			continue;
		}
		if (stageTemplate->mChapterId != newStageTemplate->mChapterId)
		{
			Goods goods;
			if (stageTemplate->mType ==STAGE_TWELVEPALACE )
			{
				goods.resourcestype = AWARD_TWELVEPALACE_STAGEDATA;
			}
			else
			{
				goods.resourcestype = AWARD_STAGEDATA;
			}
			goods.subtype = it->mValue;
			goods.num = 1;
			stageIDList.insertTail(goods);

		}
	}

	// 	saveAndSend(player->getPlayerGuid(), chapterList,stageIDs);
	return 1;
}

void StageManager::onOpenChapterBox(const ConnId& connId ,CGOpenChapterBox & msg )
{

	String strData ;
	UInt32 chapterID;	
	UInt32 isStageBegin = 0;
	UInt32 isChapterBegin = 0;	
	UInt32 nextStageID =0;
	UInt32 flag = 0;

	UInt64 guid = 0;
	UInt32 stageID =0;
	char dest[64]={};
	List<UInt32> stageIDs;
	List<Goods> itemList;
	StageData stageData;
	ChapterEndResp resp;
	PlayerChapterList chapterList;
	PlayerChapterData chapterData;

	msg.convertJsonToData(msg.strReceive);
	chapterID = msg.chapterID;

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}
	chapterList = player->getChapterList();

	for (List<PlayerChapterData>::Iter * iter = chapterList.m_listChapterData.begin();iter != NULL;iter = chapterList.m_listChapterData.next(iter))
	{
		if (iter->mValue.m_nChapterId == chapterID)
		{
			isChapterBegin = 1;
			flag = StageManager::getSingleton().chapterBoxCanOpen(iter->mValue,msg.whichOne);
			// 			if (flag < 4)
			// 			{
			// 				resp.result = flag;
			// 				break;
			// 			}

			GiftManager::getSingleton().getGift(connId,flag,resp);

			if (msg.whichOne == 1)
			{
				iter->mValue.m_nTreasure1getstate = 1;
			}
			else if (msg.whichOne == 2)
			{
				iter->mValue.m_nTreasure2getstate = 1;
			}
			else if (msg.whichOne == 3)
			{
				iter->mValue.m_nTreasure3getstate = 1;
			}


			for(List<StageData>::Iter *iter1 = iter->mValue.m_listStageDatas.begin();iter1 != NULL; iter1 = iter->mValue.m_listStageDatas.next(iter1))
			{
				stageID = iter1->mValue.m_nStageID;
				stageIDs.insertTail(stageID);
				break;
			}

			break;
		}
	}


	if (stageIDs.size() != 0)
	{
		StageManager::getSingleton().saveAndSend(player->getPlayerGuid(), chapterList,stageIDs);
	}
	Award award;

	itemList = resp.ends;

	// 	GiftManager::getSingleton().splitKind(itemList, resp.fistKind,AWARD_BASE);
	award.award = itemList;

	if (award.award.size() != 0)
	{
		resp.firstAwards.insertTail(award);
	}

	if (award.award.size() != 0)
	{
		resp.awards.insertTail(award);
	}

	Goods goods;
	goods.resourcestype = AWARD_STAGEDATA;
	goods.subtype = chapterID * 100+1;
	resp.ends.insertTail(goods);
	Json::Value stages;
	GiftManager::getSingleton().saveEndsAttr(player->getPlayerGuid(),resp.ends,resp.allAttr,stages,MiniLog55 );

	resp.stages = stages;	

	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,CHAPTER_BOX_OPEN_RESP,jsonStr);

}

UInt32 StageManager::chapterBoxCanOpen(PlayerChapterData chapterData,UInt32 whichOne)
{
	UInt32 allStar = 0;

	ChapterTemplate *chapterTemplate = gChapterTable->get(chapterData.m_nChapterId);

	if (chapterTemplate == NULL)
	{
		LOG_WARN("chapterTemplate not found!!");
		return LynxErrno::NotFound;
	}
	for(List<StageData>::Iter *iter1 = chapterData.m_listStageDatas.begin();iter1 != NULL; iter1 = chapterData.m_listStageDatas.next(iter1))
	{
		allStar += iter1->mValue.m_nGetStar%10;
	}

	if (whichOne == 1)
	{
		if (chapterData.m_nTreasure1getstate != 0)
		{
			return LynxErrno::Used;
		}
		if (allStar < chapterTemplate->mStarAwardCount1)
		{
			return LynxErrno::StarNotEnouth;
		}
		return chapterTemplate->mStarAwardContent1;

	}
	else if (whichOne == 2)
	{
		if (chapterData.m_nTreasure2getstate != 0)
		{
			return LynxErrno::Used;
		}
		if (allStar < chapterTemplate->mStarAwardCount2)
		{
			return LynxErrno::StarNotEnouth;
		}
		return chapterTemplate->mStarAwardContent2;

	}
	else if (whichOne == 3)
	{
		if (chapterData.m_nTreasure3getstate != 0)
		{
			return LynxErrno::Used;
		}
		if (allStar < chapterTemplate->mStarAwardCount3)
		{
			return LynxErrno::StarNotEnouth;
		}
		return chapterTemplate->mStarAwardContent3;

	}
	return LynxErrno::NotFound;

}
void StageManager::saveAndSend(Guid playerID,PlayerChapterList chapterList,List<UInt32> stageIDs)
{
	String strData ;
	char dest[64]={};	
	UInt32 chapterID;
	UInt32 exitChapter = 0;
	StageData tmpStageData;
	PlayerChapterData *chapterData;
	Map<UInt16, PlayerChapterData *> mapIDToChapter;
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);

	player->setchapterList(chapterList);	

	for(List<UInt32>::Iter * it = stageIDs.begin();it != NULL; it = stageIDs.next(it))//更新n个关卡
	{
		if (it->mValue == 0)
		{
			break;
		}
		chapterID = getChapterID(it->mValue);
		if (chapterID == 0)
		{
			break;
		}

		for (List<PlayerChapterData>::Iter * iter = chapterList.m_listChapterData.begin();iter != NULL;iter = chapterList.m_listChapterData.next(iter))
		{
			if (iter->mValue.m_nChapterId == chapterID)//找章
			{

				chapterData = &iter->mValue;
				for(List<StageData>::Iter *iter1 = iter->mValue.m_listStageDatas.begin();iter1 != NULL; iter1 = iter->mValue.m_listStageDatas.next(iter1))
				{
					if (iter1->mValue.m_nStageID == it->mValue)//找关卡
					{
						tmpStageData = iter1->mValue;
						break;
					}
				}
				exitChapter = 0;

				for(Map<UInt16, PlayerChapterData *>::Iter * result = mapIDToChapter.begin(); result != NULL;result = mapIDToChapter.next(result))
				{
					if (result->mKey == chapterID)
					{
						exitChapter = 1;//章存在
						break;
					}

				}
				if (exitChapter == 0)//章不存在
				{
					mapIDToChapter.insert(chapterID,chapterData);

				}
				break;
			}			
		}


		player->getStageManager().addNeedSaveDateList(it->mValue);	
	}
	player->getPersistManager().setDirtyBit(STAGEDATABIT);

	//更新客户端
	MsgStageDataToJson stageDataToJson;
	stageDataToJson.m_mapIDToChapter = mapIDToChapter;
	StageManager::getSingleton().sendStageData( player->getPlayerGuid(), mapIDToChapter);

}

UInt32 StageManager::getNormalStageLeftTimes(const ConnId& connId,UInt32 stageID)
{
	UInt32 getIt =0;
	String strData ;
	char dest[64]={};	
	UInt32 allChapterStar = 0;
	UInt32 isOpened =0;
	UInt32 chapterID = getChapterID(stageID);
	UInt32 strength =0;
	UInt32 leftTime =0;
	Vector<String> strVector;
	StageTemplate *stageTemplate;
	PlayerChapterList chapterList;	

	stageTemplate = gStageTable->get(stageID);
	if (stageTemplate == NULL)
	{
		LOG_WARN("stageTemplate not found!!");
		return 0;
	}
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	chapterList = player->getChapterList();
	PlayerDailyResetData playerDailyResetData = player->getPlayerDailyResetData();

	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();

	//次数
	for (List<PlayerChapterData>::Iter * iter = chapterList.m_listChapterData.begin();iter != NULL;iter = chapterList.m_listChapterData.next(iter))
	{
		if (iter->mValue.m_nChapterId == chapterID)
		{
			for(List<StageData>::Iter *iter1 = iter->mValue.m_listStageDatas.begin();iter1 != NULL; iter1 = iter->mValue.m_listStageDatas.next(iter1))
			{
				if (iter1->mValue.m_nStageID == stageID)
				{
					leftTime = (stageTemplate->mChallengetimesLimit - iter1->mValue.m_nChallengTimes );
					if ( (globalValue.uSDsweeptimes - playerDailyResetData.m_NormalMopUpTimes) < leftTime)
					{
						leftTime = (globalValue.uSDsweeptimes - playerDailyResetData.m_NormalMopUpTimes);
					}
					return leftTime;
				}				
			}
			break;
		}
	}
	return 0;

}

UInt32 StageManager::canChallengeStage(const ConnId& connId,UInt32 stageID)
{

	UInt32 getIt =0;
	String strData ;
	char dest[64]={};	
	UInt32 allChapterStar = 0;
	UInt32 isOpened =0;
	UInt32 chapterID = getChapterID(stageID);
	UInt32 strength =0;
	Vector<String> strVector;
	StageTemplate *stageTemplate;
	PlayerChapterList chapterList;	

	if(stageID == RANKGAMECOPYID )
	{
		return LynxErrno::None;
	}
	stageTemplate = gStageTable->get(stageID);
	if (stageTemplate == NULL)
	{
		return LynxErrno::NotFound;
	}
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	strength = FireConfirmManager::getSingleton().refreshgetStrength(connId);
	chapterList = player->getChapterList();


	List<KeyValue> m_ChapterUnlockeded = player->getChapterUnlocked();
	if (gStageTable->get(stageID)->mType == STAGE_TWELVEPALACE)//十二宗宫 需解锁
	{
		getIt =0;
		for(List<KeyValue>::Iter * it = m_ChapterUnlockeded.begin();it != NULL; it = m_ChapterUnlockeded.next(it))
		{
			if (it->mValue.key == chapterID)
			{
				if (it->mValue.value == 1)
				{
					getIt = 1;
				}				
				break;
			}
		}
		if ( getIt == 0)
		{
			return LynxErrno::IsLocked;
		}
	}

	if (stageTemplate->mStrengthCost > strength)//体力
	{
		return LynxErrno::StrengthNotEnough;
	}


	//次数
	for (List<PlayerChapterData>::Iter * iter = chapterList.m_listChapterData.begin();iter != NULL;iter = chapterList.m_listChapterData.next(iter))
	{
		if (iter->mValue.m_nChapterId == chapterID)
		{
			for(List<StageData>::Iter *iter1 = iter->mValue.m_listStageDatas.begin();iter1 != NULL; iter1 = iter->mValue.m_listStageDatas.next(iter1))
			{
				allChapterStar +=iter1->mValue.m_nGetStar%10;

				if (iter1->mValue.m_nStageID == stageID)
				{
					isOpened = 1;
					if (gStageTable->get(stageID)->mType == STAGE_TRIAL)//多人副本
					{
						if (gVipTable->get(player->getVipLevel()) == NULL)
						{
							LOG_WARN("gVipTable->get(player->getVipLevel()) not found!!");
							return LynxErrno::NotFound;
						}
						if (gVipTable->get(player->getVipLevel())->trailtimes <= player->getPlayerDailyResetData().m_nDailyMultipleCopyCount)//todo 次数不是这个
						{
							return LynxErrno::TimesNotEnough;
						}	
					}
					else
					{
						if (stageTemplate->mChallengetimesLimit <= iter1->mValue.m_nChallengTimes)
						{
							return LynxErrno::TimesNotEnough;
						}	
					}

				}				
			}
			break;
		}
	}
	if (isOpened ==0 && stageTemplate->mType != STAGE_TRIAL)//todo
	{
		return LynxErrno::NotOpen;
	}

	for(UInt32 i=1;i<=4;i++)//1等级 2本章星数 3前一关星数，4任务ID
	{
		strVector.clear();
		if (i== 1)
		{
			lynxStrSplit(stageTemplate->mStartLimit1, ";", strVector, true);
		}
		else if (i ==2)
		{
			lynxStrSplit(stageTemplate->mStartLimit2, ";", strVector, true);
		}
		else if (i ==3)
		{
			lynxStrSplit(stageTemplate->mStartLimit3, ";", strVector, true);
		}

		else if (i ==4)
		{
			lynxStrSplit(stageTemplate->mStartLimit4, ";", strVector, true);
		}

		if (strVector.size()==2)
		{

			if(atoi(strVector[0].c_str()) == 1)
			{
				if (player->getPlayerLeval() < atoi(strVector[1].c_str()))
				{
					return LynxErrno::LevelNotEnough;
				}
			}
			if(atoi(strVector[0].c_str()) == 2)
			{
				if (allChapterStar < atoi(strVector[1].c_str()))
				{
					return LynxErrno::StarNotEnouth;
				}
			}
			if(atoi(strVector[0].c_str()) == 3)
			{
				UInt32 preStageID =0;
				Map<UInt32,UInt32>::Iter *it	=	gPreStageIDs.find(stageID);
				if (it!=NULL)
				{
					preStageID = it->mValue;
				}

				if (getStageStar(connId,preStageID) < atoi(strVector[1].c_str()))
				{
					return LynxErrno::StarNotEnouth;
				}
			}
			if(atoi(strVector[0].c_str()) == 4)//todo 任务ID
			{
				// 				
				// 					if (getPreStageStar(connId,preStageID) >= atoi(strVector[1].c_str()))
				// 					{
				// 					}
				// 					else
				// 					{
				// 						return LynxErrno::LevelNotEnough;
				// 					}

			}
		}
	}
	return LynxErrno::None;
}


//只判断是否满足条件，不判断解锁
UInt32 StageManager::twelvePalaceCanDo(const ConnId& connId,UInt32 stageID)
{

	UInt32 getIt =0;
	String strData ;
	char dest[64]={};	
	UInt32 allChapterStar = 0;
	UInt32 isOpened =0;
	UInt32 chapterID = getChapterID(stageID);
	UInt32 strength =0;
	Vector<String> strVector;
	StageTemplate *stageTemplate;
	PlayerChapterList chapterList;	

	stageTemplate = gStageTable->get(stageID);
	if (stageTemplate == NULL)
	{
		LOG_WARN("stageTemplate not found!!");
		return LynxErrno::NotFound;
	}
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	strength = FireConfirmManager::getSingleton().refreshgetStrength(connId);
	chapterList = player->getChapterList();


	// 	List<KeyValue> m_ChapterUnlockeded = player->getChapterUnlocked();
	// 	if (gStageTable->get(stageID)->mType == STAGE_TWELVEPALACE)//十二宗宫 需解锁 
	// 	{
	// 		getIt =0;
	// 		for(List<KeyValue>::Iter * it = m_ChapterUnlockeded.begin();it != NULL; it = m_ChapterUnlockeded.next(it))
	// 		{
	// 			if (it->mValue.key == chapterID)
	// 			{
	// 				if (it->mValue.value == 1)
	// 				{
	// 					getIt = 1;
	// 				}				
	// 				break;
	// 			}
	// 		}
	// 		if ( getIt == 0)
	// 		{
	// 			return LynxErrno::IsLocked;
	// 		}
	// 	}

	// 	if (stageTemplate->mStrengthCost > strength)//体力
	// 	{
	// 		return LynxErrno::StrengthNotEnough;
	// 	}


	// 	//次数
	// 	for (List<PlayerChapterData>::Iter * iter = chapterList.m_listChapterData.begin();iter != NULL;iter = chapterList.m_listChapterData.next(iter))
	// 	{
	// 		if (iter->mValue.m_nChapterId == chapterID)
	// 		{
	// 			for(List<StageData>::Iter *iter1 = iter->mValue.m_listStageDatas.begin();iter1 != NULL; iter1 = iter->mValue.m_listStageDatas.next(iter1))
	// 			{
	// 				allChapterStar +=iter1->mValue.m_nGetStar%10;
	// 
	// 				if (iter1->mValue.m_nStageID == stageID)
	// 				{
	// 					isOpened = 1;
	// 					if (gStageTable->get(stageID)->mType == STAGE_TRIAL)//多人副本
	// 					{
	// 						if (stageTemplate->mChallengetimesLimit <= player->getPlayerDailyResetData().m_nDailyMultipleCopyCount)//todo 次数不是这个
	// 						{
	// 							return LynxErrno::TimesNotEnough;
	// 						}	
	// 					}
	// 					else
	// 					{
	// 						if (stageTemplate->mChallengetimesLimit <= iter1->mValue.m_nChallengTimes)
	// 						{
	// 							return LynxErrno::TimesNotEnough;
	// 						}	
	// 					}
	// 
	// 				}				
	// 			}
	// 			break;
	// 		}
	// 	}
	// 	if (isOpened ==0 && stageTemplate->mType != STAGE_TRIAL)//todo
	// 	{
	// 		return LynxErrno::NotOpen;
	// 	}

	for(UInt32 i=1;i<=4;i++)//1等级 2本章星数 3前一关星数，4任务ID
	{
		strVector.clear();
		if (i== 1)
		{
			lynxStrSplit(stageTemplate->mStartLimit1, ";", strVector, true);
		}
		else if (i ==2)
		{
			lynxStrSplit(stageTemplate->mStartLimit2, ";", strVector, true);
		}
		else if (i ==3)
		{
			lynxStrSplit(stageTemplate->mStartLimit3, ";", strVector, true);
		}

		else if (i ==4)
		{
			lynxStrSplit(stageTemplate->mStartLimit4, ";", strVector, true);
		}

		if (strVector.size()==2)
		{

			if(atoi(strVector[0].c_str()) == 1)
			{
				if (player->getPlayerLeval() < atoi(strVector[1].c_str()))
				{
					return LynxErrno::LevelNotEnough;
				}
			}
			if(atoi(strVector[0].c_str()) == 2)
			{
				if (allChapterStar < atoi(strVector[1].c_str()))
				{
					return LynxErrno::StarNotEnouth;
				}
			}
			if(atoi(strVector[0].c_str()) == 3)
			{
				UInt32 preStageID =0;
				Map<UInt32,UInt32>::Iter *it	=	gPreStageIDs.find(stageID);
				if (it!=NULL)
				{
					preStageID = it->mValue;
				}

				if (getStageStar(connId,preStageID) < atoi(strVector[1].c_str()))
				{
					return LynxErrno::StarNotEnouth;
				}
			}
			if(atoi(strVector[0].c_str()) == 4)//todo 任务ID
			{
				// 				
				// 					if (getPreStageStar(connId,preStageID) >= atoi(strVector[1].c_str()))
				// 					{
				// 					}
				// 					else
				// 					{
				// 						return LynxErrno::LevelNotEnough;
				// 					}

			}
		}
	}
	return LynxErrno::None;
}

UInt32 StageManager::isMeetCondition(const ConnId& connId,UInt32 stageID)
{
	UInt32 getIt =0;
	String strData ;
	char dest[64]={};	
	UInt32 allChapterStar = 0;
	UInt32 isOpened =0;
	UInt32 chapterID = getChapterID(stageID);
	UInt32 strength =0;
	Vector<String> strVector;
	StageTemplate *stageTemplate;
	PlayerChapterList chapterList;	

	stageTemplate = gStageTable->get(stageID);
	if (stageTemplate == NULL)
	{
		LOG_WARN("stageTemplate not found!!");
		return LynxErrno::NotFound;
	}
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	strength = FireConfirmManager::getSingleton().refreshgetStrength(connId);
	chapterList = player->getChapterList();




	if (stageTemplate->mStrengthCost > strength)//体力
	{
		return LynxErrno::StrengthNotEnough;
	}


	//次数
	for (List<PlayerChapterData>::Iter * iter = chapterList.m_listChapterData.begin();iter != NULL;iter = chapterList.m_listChapterData.next(iter))
	{
		if (iter->mValue.m_nChapterId == chapterID)
		{
			for(List<StageData>::Iter *iter1 = iter->mValue.m_listStageDatas.begin();iter1 != NULL; iter1 = iter->mValue.m_listStageDatas.next(iter1))
			{
				allChapterStar +=iter1->mValue.m_nGetStar%10;


			}
			break;
		}
	}

	for(UInt32 i=1;i<=4;i++)//1等级 2本章星数 3前一关星数，4任务ID
	{
		strVector.clear();
		if (i== 1)
		{
			lynxStrSplit(stageTemplate->mStartLimit1, ";", strVector, true);
		}
		else if (i ==2)
		{
			lynxStrSplit(stageTemplate->mStartLimit2, ";", strVector, true);
		}
		else if (i ==3)
		{
			lynxStrSplit(stageTemplate->mStartLimit3, ";", strVector, true);
		}

		else if (i ==4)
		{
			lynxStrSplit(stageTemplate->mStartLimit4, ";", strVector, true);
		}

		if (strVector.size()==2)
		{

			if(atoi(strVector[0].c_str()) == 1)
			{
				if (player->getPlayerLeval() < atoi(strVector[1].c_str()))
				{
					return LynxErrno::LevelNotEnough;
				}
			}
			if(atoi(strVector[0].c_str()) == 2)
			{
				if (allChapterStar < atoi(strVector[1].c_str()))
				{
					return LynxErrno::StarNotEnouth;
				}
			}
			if(atoi(strVector[0].c_str()) == 3)
			{
				UInt32 preStageID =0;
				Map<UInt32,UInt32>::Iter *it	=	gPreStageIDs.find(stageID);
				if (it!=NULL)
				{
					preStageID = it->mValue;
				}

				if (getStageStar(connId,preStageID) < atoi(strVector[1].c_str()))
				{
					return LynxErrno::StarNotEnouth;
				}
			}
			if(atoi(strVector[0].c_str()) == 4)//todo 任务ID
			{
				// 				
				// 					if (getPreStageStar(connId,preStageID) >= atoi(strVector[1].c_str()))
				// 					{
				// 					}
				// 					else
				// 					{
				// 						return LynxErrno::LevelNotEnough;
				// 					}

			}
		}
	}
	return LynxErrno::None;
}

UInt32 StageManager::getChapterType(UInt32 stageID)
{
	for(Map<UInt64, StageTemplate>::Iter *iter = gStageTable->mMap.begin();iter!=NULL;iter = gStageTable->mMap.next(iter))
	{
		if (iter->mKey == stageID)
		{
			return iter->mValue.mType;
		}
	}
	return 0;
}

UInt32 StageManager::getStageStar(const ConnId& connId,UInt32 stageID)
{

	PlayerChapterList chapterList;
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);

	UInt32 chapterID = getChapterID(stageID);
	chapterList = player->getChapterList();


	if (stageID ==0)
	{
		return 0;
	}
	for (List<PlayerChapterData>::Iter * iter = chapterList.m_listChapterData.begin();iter != NULL;iter = chapterList.m_listChapterData.next(iter))
	{
		if (iter->mValue.m_nChapterId == chapterID)
		{
			for(List<StageData>::Iter *iter1 = iter->mValue.m_listStageDatas.begin();iter1 != NULL; iter1 = iter->mValue.m_listStageDatas.next(iter1))
			{

				if (iter1->mValue.m_nStageID == stageID)
				{
					return iter1->mValue.m_nGetStar%10;;
				}
			}
		}
	}
	return 0;

}


void StageManager::sendStageData(Guid playerID,Map<UInt16, PlayerChapterData *> mapIDToChapter,bool isLogin )
{


	Map<UInt16, PlayerChapterData *> mapIDToChapterNormal;
	Map<UInt16, PlayerChapterData *> mapIDToChapterTwelvePalace;

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);

	if (player->getConnId() == 0)
	{
		return;
	}

	for(Map<UInt16, PlayerChapterData *>::Iter * iter =  mapIDToChapter.begin();iter != NULL;iter = mapIDToChapter.next(iter))
	{
		for(List<StageData>::Iter * it = iter->mValue->m_listStageDatas.begin();it != NULL;it = iter->mValue->m_listStageDatas.next(it))
		{
			if (it->mValue.m_nStageID == 0)
			{
				continue;
			}
			if(getChapterType(it->mValue.m_nStageID) == 7)
			{
				mapIDToChapterTwelvePalace.insert(iter->mKey,iter->mValue);
			}
			else
			{
				mapIDToChapterNormal.insert(iter->mKey,iter->mValue);
			}
			break;
		}		
	}
	if (mapIDToChapterNormal.size()!= 0)
	{
		MsgStageDataToJson stageDataToJson;
		stageDataToJson.playerID = player->getPlayerGuid();
		stageDataToJson.m_mapIDToChapter = mapIDToChapterNormal;		
		Json::Value node ;
		std::string jsonStr = stageDataToJson.convertDataToJson(node);

		if (isLogin == true)
		{			
			NetworkSystem::getSingleton().sender( player->getConnId(),BOC_CHAPTER_LIST_RESP,jsonStr);
		}
		else
		{	
		}

	}

	if (mapIDToChapterTwelvePalace.size()!= 0)
	{
		MsgStageDataToJsonTwelvePalace stageDataToJson;
		stageDataToJson.playerID = player->getPlayerGuid();
		stageDataToJson.m_mapIDToChapter = mapIDToChapterTwelvePalace;
		stageDataToJson.m_ChapterUnlockeded = player->getChapterUnlocked();
		//升级的时候客户端无法获取
		//   		delDoNotMeet(player->getConnId(),stageDataToJson.m_mapIDToChapter);
		stageDataToJson.m_mapIDToSelect = player->getPlayerLocalData().m_IDToSelect;

		Json::Value node;
		std::string jsonStr;


		stageDataToJson.records = RankingManager::getSingleton().getRecords(TWELVE_PALACE_RECORD_TYPE);
		stageDataToJson.myRecods = player->getRankingPersonalManager().getRecords(TWELVE_PALACE_MY_RECORD);
		if (StageCalcManager::getSingleton().getResetStages(playerID) != NULL)
		{
			stageDataToJson.resetStages = *StageCalcManager::getSingleton().getResetStages(playerID);
		}

		jsonStr = stageDataToJson.convertDataToJson(node);	

		if (isLogin == true)
		{
			NetworkSystem::getSingleton().sender( player->getConnId(),TWELVE_PALACE_LIST_RESP,jsonStr);
		}
		else
		{		

		}
	}

	if (isLogin == true)
	{
		saveAndSend(player->getPlayerGuid(), player->getChapterList(),player->getStageManager().getNeedSaveDateList());
	}

}


void StageManager::getStageData(Guid playerID, UInt32 stageID,Json::Value &root )
{

	bool isLogin = false;
	Map<UInt16, PlayerChapterData *> mapIDToChapterNormal;
	Map<UInt16, PlayerChapterData *> mapIDToChapterTwelvePalace;
	UInt32 chapterID = getChapterID(stageID);
	if (chapterID == 0)
	{
		return;
	}

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);

	if (player->getConnId() == 0)
	{
		return;
	}
	Map<UInt16, PlayerChapterData *> mapIDToChapter = player->getStageManager().getMapIDToChapter();

	for(Map<UInt16, PlayerChapterData *>::Iter * iter =  mapIDToChapter.begin();iter != NULL;iter = mapIDToChapter.next(iter))
	{
		for(List<StageData>::Iter * it = iter->mValue->m_listStageDatas.begin();it != NULL;it = iter->mValue->m_listStageDatas.next(it))
		{
			if (it->mValue.m_nStageID == 0)
			{
				continue;
			}
			if (iter->mKey == chapterID)
			{
				if(getChapterType(it->mValue.m_nStageID) == 7)
				{
					mapIDToChapterTwelvePalace.insert(iter->mKey,iter->mValue);
				}
				else
				{
					mapIDToChapterNormal.insert(iter->mKey,iter->mValue);
				}
				break;
			}

		}		
	}
	if (mapIDToChapterNormal.size()!= 0)
	{
		MsgStageDataToJson stageDataToJson;
		stageDataToJson.playerID = player->getPlayerGuid();
		stageDataToJson.m_mapIDToChapter = mapIDToChapterNormal;		
		// 		Json::Value node ;
		std::string jsonStr = stageDataToJson.convertDataToJson(root);


	}

	if (mapIDToChapterTwelvePalace.size()!= 0)
	{
		MsgStageDataToJsonTwelvePalace stageDataToJson;
		stageDataToJson.playerID = player->getPlayerGuid();
		stageDataToJson.m_mapIDToChapter = mapIDToChapterTwelvePalace;
		stageDataToJson.m_ChapterUnlockeded = player->getChapterUnlocked();
		//升级的时候客户端无法获取
		//   		delDoNotMeet(player->getConnId(),stageDataToJson.m_mapIDToChapter);
		stageDataToJson.m_mapIDToSelect = player->getPlayerLocalData().m_IDToSelect;

		// 		Json::Value node;
		std::string jsonStr;

		if (isLogin == true)
		{
			stageDataToJson.records = RankingManager::getSingleton().getRecords(TWELVE_PALACE_RECORD_TYPE);
			stageDataToJson.myRecods = player->getRankingPersonalManager().getRecords(TWELVE_PALACE_MY_RECORD);
			if (StageCalcManager::getSingleton().getResetStages(playerID) != NULL)
			{
				stageDataToJson.resetStages = *StageCalcManager::getSingleton().getResetStages(playerID);
			}

			jsonStr = stageDataToJson.convertDataToJson(root);	

			// 			NetworkSystem::getSingleton().sender( player->getConnId(),TWELVE_PALACE_LIST_RESP,jsonStr);
		}
		else
		{		
			jsonStr = stageDataToJson.convertDataToJson(root);
			// 			if( LogicSystem::getSingleton().combinSendData(player->getPlayerGuid(),AWARD_TWELVEPALACE_STAGEDATA,node) ==false)	
			// 			{			
			// 				NetworkSystem::getSingleton().sender( player->getConnId(),TWELVE_PALACE_LIST_CHANG_RESP,jsonStr);
			// 			}
		}
	}

	// 	if (isLogin == true)
	// 	{
	// 		saveAndSend(player->getPlayerGuid(), player->getChapterList(),player->getStageManager().getNeedSaveDateList());
	// 	}

}


void StageManager::delDoNotMeet(const ConnId& connId,Map<UInt16, PlayerChapterData *> &mapIDToChapter)
{

	for(Map<UInt16, PlayerChapterData *>::Iter * iter =  mapIDToChapter.begin();iter != NULL;iter = mapIDToChapter.next(iter))
	{
		for(List<StageData>::Iter * it = iter->mValue->m_listStageDatas.begin();it != NULL;it = iter->mValue->m_listStageDatas.next(it))
		{
			if (it->mValue.m_nStageID == 0)
			{
				continue;
			}
			if(getChapterType(it->mValue.m_nStageID) == STAGE_TWELVEPALACE && StageManager::getSingleton().twelvePalaceCanDo( connId, it->mValue.m_nStageID) != 0)
			{
				mapIDToChapter.erase(iter);
			}
			break;
		}		
	}
}

Map<UInt16, PlayerChapterData *>  StageManager::getMapIDToChapter()
{
	m_mapIDToChapter.clear();

	for(List<PlayerChapterData>::Iter * iterChapter = m_pListChapterData->begin(); iterChapter != NULL;
		iterChapter = m_pListChapterData->next(iterChapter))
	{
		m_mapIDToChapter.insert(iterChapter->mValue.m_nChapterId,&iterChapter->mValue);
	}
	return m_mapIDToChapter;

}

void StageManager::addNeedSaveDateList(UInt32 stageID)
{
	needSaveDateList.insertTail(stageID);
}


List<UInt32> StageManager::getNeedSaveDateList()
{	
	return needSaveDateList;

}

void StageManager::resetNeedSaveDateList()
{
	needSaveDateList.clear();

}


UInt32 StageManager::getTwelvePalaceNeedReset()
{
	return mTwelvePalaceNeedReset;
}

void StageManager::setTwelvePalaceNeedReset(UInt32 num)
{
	mTwelvePalaceNeedReset = num;

}

Map<UInt64, StageTemplate>  StageManager::getMapIDToStageByChapterID(UInt32 id)
{
	Map<UInt64, StageTemplate> IDToStage;

	for(Map<UInt64, StageTemplate>::Iter *iter = gStageTable->mMap.begin();iter!=NULL;iter = gStageTable->mMap.next(iter))
	{
		if (iter->mValue.mChapterId == id)
		{
			IDToStage.insert(iter->mKey,iter->mValue);
		}

	}
	return IDToStage;

}


void StageManager::addMatchingPlayers(UInt64 playerID)
{
	matchingPlayers.insertTail(playerID);
}


List<UInt64> StageManager::getMatchingPlayers()
{	
	return matchingPlayers;
}

void StageManager::resetMatchingPlayers()
{
	matchingPlayers.clear();
}



void StageManager::addMatchingPlayersTwelvePalace(UInt64 playerID)
{
	matchingPlayersTwelvePalace.insertTail(playerID);
}


List<UInt64> StageManager::getMatchingPlayersTwelvePalace()
{	
	return matchingPlayersTwelvePalace;
}

void StageManager::resetMatchingPlayersTwelvePalace()
{
	matchingPlayersTwelvePalace.clear();
}

void StageManager::resetChapterUnlock()
{
	List<KeyValue> m_ChapterUnlockeded = m_pPlayer->getChapterUnlocked();

	for(List<KeyValue>::Iter * it = m_ChapterUnlockeded.begin();it != NULL; it = m_ChapterUnlockeded.next(it))
	{
		it->mValue.key = 0;
		it->mValue.value = 0;
	}
	m_pPlayer->setChapterUnlocked(m_ChapterUnlockeded);
	m_pPlayer->getPersistManager().setDirtyBit(CHAPTERUNLOCKEDTABIT);
}


UInt32 StageManager::resetStagechallegeTimes(UInt32 stageID)
{

	UInt32 chapterID = StageManager::getSingleton().getChapterID(stageID);
	if (chapterID == 0)
	{
		return LynxErrno::NotFound;
	}
	for (List<PlayerChapterData>::Iter * iter = m_pListChapterData->begin();iter != NULL;iter = m_pListChapterData->next(iter))
	{
		if (iter->mValue.m_nChapterId == chapterID)
		{				
			for(List<StageData>::Iter *iter1 = iter->mValue.m_listStageDatas.begin();iter1 != NULL; iter1 = iter->mValue.m_listStageDatas.next(iter1))
			{

				if (iter1->mValue.m_nStageID == stageID)
				{
					iter1->mValue.m_nChallengTimes = 0;
					addNeedSaveDateList(stageID);
					m_pPlayer->getPersistManager().setDirtyBit(STAGEDATABIT);
					return LynxErrno::None;
				}
			}

			break;			
		}
	}
	return LynxErrno::NotFound;
}



UInt32 StageManager::resetMultipleCopychallegeTimes()
{

	PlayerDailyResetData dailyResetData= m_pPlayer->getPlayerDailyResetData();

	dailyResetData.m_nDailyMultipleCopyCount = 0;

	m_pPlayer->setPlayerDailyResetData(dailyResetData);
	m_pPlayer->getPersistManager().setDirtyBit(DAILYRESETBIT);
	return 0;

}


UInt32 StageManager::resetWelfaleMarketchallegeTimes(UInt32 id)  
{

	PlayerDailyResetData dailyResetData= m_pPlayer->getPlayerDailyResetData();

	if (id == 1)
	{
		dailyResetData.m_nDailyChaiCount = 0;
	}
	else if (id == 2)
	{
		dailyResetData.m_nDailyMiCount = 0;
	}
	else if (id == 3)
	{
		dailyResetData.m_nDailyYouCount = 0;
	}
	else if (id == 4)
	{
		dailyResetData.m_nDailyYanCount = 0;
	}

	m_pPlayer->setPlayerDailyResetData(dailyResetData);
	m_pPlayer->getPersistManager().setDirtyBit(DAILYRESETBIT);
	return 0;
}

UInt32 StageManager::resetClimbTowerchallegeTimes()  
{

	PlayerDailyResetData dailyResetData= m_pPlayer->getPlayerDailyResetData();

	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();

	dailyResetData.m_nClimbTowerTimes = globalValue.uTPresetlowerlimit;

	m_pPlayer->setPlayerDailyResetData(dailyResetData);
	m_pPlayer->getPersistManager().setDirtyBit(DAILYRESETBIT);
	return 0;
}


void StageManager::resetgDocument()
{
	// 	 rapidjson::Document::AllocatorType& allocator = gDocument.GetAllocator();
	gDocument.SetObject();
	rapidjson::Value array(kArrayType);
	gRapidArray = array;
}

void StageManager::addgDocument( rapidjson::Value &object)
{

	rapidjson::Document::AllocatorType& allocator = gDocument.GetAllocator();
	gRapidArray.PushBack(object, gDocument.GetAllocator());
}

rapidjson::Document::AllocatorType& StageManager::getgDocumentAllocator()
{
	return gDocument.GetAllocator();
}

std::string StageManager::getgDocumentStr()
{
	gDocument.AddMember("allAttr", gRapidArray, gDocument.GetAllocator());
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	gDocument.Accept(writer);
	std::string str = buffer.GetString();
	// 	gRapidArray.Clear();
	gDocument.GetAllocator().Clear();
	// 	gDocument.Clear();
	return str;
}

rapidjson::Document& StageManager::getDocument()
{
	return gDocument;
}

rapidjson::Value& StageManager::getArry()
{
	return gRapidArray;
}
