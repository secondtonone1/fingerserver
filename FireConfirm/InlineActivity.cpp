
#include "../LogicSystem.h"
#include "InlineActivity.h"
#include "../TimeManager.h"
#include "../PersistSystem.h"
#include "../ServerData.h"
#include "GlobalValue.h"

using namespace Lynx;


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

UInt32 InlineActivityManager::checkSevenDayCanGotIt(UInt32 day,UInt32 id, PlayerInlineActivityData &inlineActivityData,List<Goods> &itemList)
{

	UInt32 getIt = 0;
	SevenDayTrainningTemplate *sevenDayTrainningTemplate = gSevenDayTrainningTable->get(id);
	if (sevenDayTrainningTemplate == NULL)
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
					if (iter1->mValue.subtype < sevenDayTrainningTemplate->needarg)
					{
						return LynxErrno::TaskNotFinished;
					}
					iter1->mValue.num = 1;
					break;
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
	lynxStrSplit(sevenDayTrainningTemplate->award, ";", strVector1, true);
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
	SevenDayTrainningTemplate *sevenDayTrainningTemplate;

	checkSevenDayTaskFinish(inlineActivityData);
	
	for(List<KeyList>::Iter * it2 = inlineActivityData.m_SevenDayTaskFinishList.begin(); it2 != NULL; it2 = inlineActivityData.m_SevenDayTaskFinishList.next(it2))
	{
		for(List<Goods>::Iter *iter1 = it2->mValue.values.begin();iter1 != NULL;iter1 = it2->mValue.values.next(iter1))
		{
			if (iter1->mValue.num == 0)
			{
				sevenDayTrainningTemplate = gSevenDayTrainningTable->get(iter1->mValue.resourcestype);
				if (sevenDayTrainningTemplate != NULL)
				{
					if (iter1->mValue.subtype >= sevenDayTrainningTemplate->needarg)
					{
						Goods goods;
						Vector<String> strVector1;
						lynxStrSplit(sevenDayTrainningTemplate->award, ",", strVector1, true);
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
	SevenDayTrainningTemplate *sevenDayTrainningTemplate;

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

					sevenDayTrainningTemplate = gSevenDayTrainningTable->get(iter1->mValue.resourcestype);
					if (sevenDayTrainningTemplate != NULL)
					{
						if (iter1->mValue.subtype >= sevenDayTrainningTemplate->needarg)
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

// 
// 
// void InlineActivityManager::addSevenDayTaskFinish( PlayerInlineActivityData &inlineActivityData)
// {
// 	UInt32 count = 0;
// 	UInt32 count1 = 0;
// 	UInt32 count2 = 0;
// 	UInt32 finishCount = 0;
// 	UInt32 notFinishCount = 0;
// 	UInt32 taskCount = 0;
// 
// 	count ++;
// 	count1 = 0;
// 	for(List<KeyList>::Iter * it2 = inlineActivityData.m_SevenDayTaskFinishList.begin(); it2 != NULL; it2 = inlineActivityData.m_SevenDayTaskFinishList.next(it2))
// 	{
// 		finishCount = 0;
// 		notFinishCount = 0;
// 
// 		it2->mValue.values.clear();
// 		count2 = 0;
// 		for( Map<UInt64, InlineActivityTemplate>::Iter *it = gInlineActivityTable->mMap.begin();it!= NULL;it = gInlineActivityTable->mMap.next(it))
// 		{
// 			if (it->mValue.type == SEVENDAY_TASK )
// 			{
// 
// 				if (count1 == count2)
// 				{
// 					it2->mValue.key = it->mValue.id;
// 					if (it->mValue.taskid1 != 0)
// 					{
// 						Goods goods;
// 						goods.resourcestype = it->mValue.taskid1;
// 						goods.subtype = 5;
// 						it2->mValue.values.insertTail(goods);
// 					}
// 					if (it->mValue.taskid2 != 0)
// 					{
// 						Goods goods;
// 						goods.resourcestype = it->mValue.taskid2;
// 						goods.subtype = 5;
// 						it2->mValue.values.insertTail(goods);
// 					}
// 					if (it->mValue.taskid3 != 0)
// 					{
// 						Goods goods;
// 						goods.resourcestype = it->mValue.taskid3;
// 						goods.subtype = 5;
// 						it2->mValue.values.insertTail(goods);
// 					}
// 					if (it->mValue.taskid4 != 0)
// 					{
// 						Goods goods;
// 						goods.resourcestype = it->mValue.taskid4;
// 						goods.subtype = 5;
// 						it2->mValue.values.insertTail(goods);
// 					}
// 					break;
// 				}
// 				count2 ++;
// 			}
// 		}
// 		count1 ++;
// 	}
// 	
// 
// }


String InlineActivityManager::stringToUtf(const String& str)
{
	String dst = str;

#ifdef _WIN32
	std::locale langLocale("");
	std::locale old = std::locale::global(langLocale);
	dst = EncodingConvert::ansi2Utf8(str);
	std::locale::global(old);
#endif
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
	if(inlineActivityTemplate == NULL)
	{
		return 0;
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
				if(inlineActivityTemplate == NULL)
				{
					return 0;
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
		player->setInlineActivityData(inlineActivityData);
		InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),SEVENDAY_TASK,inlineActivityData);
		return 2;
	}

	UInt32 oneKeyDone = 0;
	if (day == 6)//第七天只检查不领取
	{		
		count = 0;
		inlineActivityTemplate = NULL;
		for (List<KeyValue>::Iter *iter = inlineActivityData.m_SevenDayTaskGotList.begin();iter !=NULL;iter = inlineActivityData.m_SevenDayTaskGotList.next(iter))
		{
			count ++;
			if (count == 7&&iter->mValue.value == 2)
			{
				oneKeyDone = 1;
			}
			if (iter->mValue.key == iter->mValue.value)
			{
			}
			else
			{		
				getIt = 1;
			}

		}
		if(getIt == 0&&oneKeyDone ==1)
		{
			return 2;
		}


	}

	return 0;
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
	req.m_TimeAwardGotList = inlineActivityData.m_TimeAwardGotList;
	req.m_LoginTime = inlineActivityData.m_LoginTime;
	req.m_LogoutTime = inlineActivityData.m_LogoutTime;
	req.m_onlieFinishTime = inlineActivityData.m_onlieFinishTime;

	PersistSystem::getSingleton().postThreadMsg(req,0);
}

void InlineActivityManager::incRechargeNum(Guid playerId)
{
	ServerPublicData  serverPublicData = ServerData::getSingleton().getServerPublicData();
	serverPublicData.gRechargeNum += 100;
	ServerData::getSingleton().setServerPublicData(serverPublicData,true);
}
void InlineActivityManager::initPlayerFirstLogin(UInt64 playerID)
{
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}
	PlayerDailyResetData dailyResetData = player->getPlayerDailyResetData();
	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();
	dailyResetData.m_nTwelvePalaceUnlockCount = globalValue.uTPresetlowerlimit;

	player->setPlayerDailyResetData(dailyResetData);
	player->getPersistManager().setDirtyBit(DAILYRESETBIT);	

}

void InlineActivityManager::checkLogin(Guid playerId)
{
	UInt32 nowTime = TimeUtil::getTimeSec();
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerId);	
	PlayerInlineActivityData inlineActivityData = player->getInlineActivityData();
	PlayerBaseData baseData = player->getPlayerBaseData();

	if (inlineActivityData.m_FirstLoginTime == 0)
	{
		inlineActivityData.m_FirstLoginTime = nowTime;
		baseData.m_nFirstLoinTime =  nowTime;
		PersistBaseDateSave saveReq;
		saveReq.m_nPlayerUid = player->getPlayerGuid();
		PersistSystem::getSingleton().postThreadMsg(saveReq, 0);

		InlineActivityManager::getSingleton().initPlayerFirstLogin(playerId);

	}

	inlineActivityData.m_LoginTime = nowTime;
	baseData.m_nLoginTime = nowTime;
	player->setPlayerBaseData(baseData);
	player->getPersistManager().setDirtyBit(BASEDATABIT);


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

	inlineActivityData.m_LogoutTime = TimeUtil::getTimeSec();


	player->setInlineActivityData(inlineActivityData);
	InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),TIME_AWARD,inlineActivityData);	
}

void InlineActivityManager::getIdByEventType(UInt64 eventType, List<UInt32>  &idList)
{
	for (Map<UInt32, SevenDayTrainningTemplate>::Iter * iter = gSevenDayTrainningTable->mMap.begin();iter != NULL;iter = gSevenDayTrainningTable->mMap.next(iter))
	{
		if (iter->mValue.needevent == eventType)
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
	SevenDayTrainningTemplate* sevenDayTrainningTemplate = NULL;
	InlineActivityTemplate* inlineActivityTemplate;
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerId);	
	PlayerInlineActivityData inlineActivityData = player->getInlineActivityData();

	for( Map<UInt64, InlineActivityTemplate>::Iter *it = gInlineActivityTable->mMap.begin();it!= NULL;it = gInlineActivityTable->mMap.next(it))
	{
		if (it->mValue.type == SEVENDAY_TASK)
		{			
			inlineActivityTemplate = &it->mValue;
			break;
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
		sevenDayTrainningTemplate = gSevenDayTrainningTable->get(iter->mValue);
		if (sevenDayTrainningTemplate == NULL)
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
						iter1->mValue.subtype +=change;						
					}

				}
				if (getIt == 0)
				{
					Goods goods;
					goods.resourcestype = iter->mValue;
					goods.subtype = change;
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




void InlineActivityManager::sendActivitys(const  ConnId& connId )
{
	ActivityResp resp;
	Activity activity;

	MonthSingnReq monthSingnReq;
	MonthSingnResp monthSingnResp;
	InlineActivityManager::getSingleton().monthSingnReq( connId,monthSingnReq,monthSingnResp);
	activity.id = MONTH_SIGN;
 	activity.state =Activity_OPEN;
	activity.data = monthSingnResp.convertDataToJson().c_str();
	resp.activitys.insertTail(activity);

	SevenDayTaskReq sevenDayTaskReq;
	SevenDayTaskResp  sevenDayTaskResp;
	InlineActivityManager::getSingleton().sevenDayTaskReq( connId,sevenDayTaskReq,sevenDayTaskResp);
	activity.id = SEVENDAY_TASK;
	activity.state = sevenDayTaskResp.state;
	activity.data = sevenDayTaskResp.convertDataToJson().c_str();
	resp.activitys.insertTail(activity);


	GrowFoundReq growFoundReq;
	GrowFoundResp growFoundResp;
	InlineActivityManager::getSingleton().growFoundReq( connId,growFoundReq,growFoundResp);
	activity.id = GROWTH_FOUND;
	activity.state = growFoundResp.state;
	activity.data = growFoundResp.convertDataToJson().c_str();
	resp.activitys.insertTail(activity);

	PeopleWelfareReq peopleWelfareReq;
	PeopleWelfareResp peopleWelfareResp;
	InlineActivityManager::getSingleton().peopleWelfareReq( connId,peopleWelfareReq,peopleWelfareResp);
	activity.id = PEOPLE_WELFARE;
	activity.state = peopleWelfareResp.state;
	activity.data = peopleWelfareResp.convertDataToJson().c_str();
	resp.activitys.insertTail(activity);

	TimeAwardReq timeAwardReq;
	TimeAwardResp timeAwardResp;
	InlineActivityManager::getSingleton().timeAwardReq( connId,timeAwardReq,timeAwardResp);
	activity.id = TIME_AWARD;
  	activity.state =Activity_OPEN;
	activity.data = timeAwardResp.convertDataToJson().c_str();
	resp.activitys.insertTail(activity);


	OnlineWelFareReq  onlineWelFareReq;
	OnlineWelFareResp onlineWelFareResp;
	onlineWelFareReq.isLogin = 1;
	InlineActivityManager::getSingleton().onlineWelFareReq( connId,onlineWelFareReq,onlineWelFareResp);
	
	activity.id = ONLINE_WELFARE;
	activity.state = onlineWelFareResp.state;
	activity.data = onlineWelFareResp.convertDataToJson().c_str();
	resp.activitys.insertTail(activity);


	SevenLoginAwardReq sevenLoginAwardReq;
	sevenLoginAwardReq.reqType = 2;
	SevenLoginAwardResp sevenLoginAwardResp;
	InlineActivityManager::getSingleton().sevenLoginAwardReq( connId,sevenLoginAwardReq,sevenLoginAwardResp);
	activity.id = SEVENDAY_LOGIN_AWARD;
	activity.state = sevenLoginAwardResp.state;
	activity.data = sevenLoginAwardResp.convertDataToJson().c_str();
	resp.activitys.insertTail(activity);


	std::string jsonStr = resp.convertDataToJson();
	//LOG_INFO("SEND_ACTIVITY_RESP = %s",jsonStr.c_str());
	NetworkSystem::getSingleton().sender( connId,SEND_ACTIVITY_RESP,jsonStr);
	//2 6 1 4 7

}

void InlineActivityManager::growFoundReq(const  ConnId& connId,GrowFoundReq & req,GrowFoundResp &resp)
{
	UInt32 gold = 0;
	InlineActivityTemplate* inlineActivityTemplate;

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if (player == NULL)
	{
		return;
	}
	req.convertJsonToData(req.strReceive);
	resp.reqType = req.reqType;
	gold = player->getPlayerGold();

	PlayerInlineActivityData inlineActivityData = player->getInlineActivityData();
	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();

	if (req.reqType == 0)
	{
	}
	else if (req.reqType == 1)
	{				

		if (gold < globalValue.GROWTHFOUNDcost)
		{
			resp.result = LynxErrno::RmbNotEnough;
		}
		if (player->getVipLevel() < globalValue.GROWTHFOUNDviplvneed)
		{
			resp.result =  LynxErrno::VipLevelNotEnough;
		}
		if (resp.result == LynxErrno::None)
		{

			Goods goods;
			List<Goods> itemList;

			goods.resourcestype =AWARD_BASE;
			goods.subtype = AWARD_BASE_GOLD;
			goods.num = 0 - globalValue.GROWTHFOUNDcost;
			itemList.insertTail(goods);
			GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog47);



			inlineActivityData.m_GrowFoundBuyFlag = 1;

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
			
			Goods goods;
			List<Goods> itemList;

			goods.resourcestype =AWARD_BASE;
			goods.subtype = AWARD_BASE_GOLD;
			goods.num = inlineActivityTemplate->gold;
			itemList.insertTail(goods);
		

			GiftManager::getSingleton().saveEndsGetAttr(player->getPlayerGuid(),itemList,resp.allAttr,MiniLog47);	


			inlineActivityData.m_GrowFoundGotList.insertTail(req.id);

			player->setInlineActivityData(inlineActivityData);
			InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),GROWTH_FOUND,inlineActivityData);
		}

	}
	resp.buyFlag = inlineActivityData.m_GrowFoundBuyFlag;
	resp.gotList = inlineActivityData.m_GrowFoundGotList;
	resp.vipLv = globalValue.GROWTHFOUNDviplvneed;
	resp.cost = globalValue.GROWTHFOUNDcost;
	resp.gold = player->getPlayerGold();

	if (inlineActivityData.m_GrowFoundBuyFlag == 1)
	{
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
		if (getIt == 0 )
		{
			resp.state = Activity_END;
		}
	}
}


//resp.signCount是id，inlineActivityData.m_MonthSignCount是第几天 1开始
void InlineActivityManager::monthSingnReq(const  ConnId& connId,MonthSingnReq & req,MonthSingnResp& resp)
{
	UInt32 month = TimeManager::getMonth(-1) + 1;
	UInt32 count = 0;
	UInt32 getIt = 0;
	UInt32 canSignCount = 0;
	InlineActivityTemplate* inlineActivityTemplate;
	

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

	UInt32 systemRefreshHour = LogicSystem::getSingleton().getRefreshTime(TIME_REFRESH_ID06)/3600;

	if( TimeManager::timeIsTodayDelayHours(inlineActivityData.m_MonthSignTime,systemRefreshHour) == true&&req.reqType == 1)
	{
		resp.result = LynxErrno::HadSign;
	}
	if (inlineActivityData.m_MonthSignCount >= 28 && req.reqType == 1)
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

			if (player->getVipLevel() >= inlineActivityTemplate->viplv&&inlineActivityTemplate->viplv !=0)
			{
				for (List<Goods>::Iter * item = resp.ends.begin();item!= NULL;item = resp.ends.next(item))
				{
					item->mValue.num *= 2;//2倍
				}
			}

			Json::Value stages;
			GiftManager::getSingleton().saveEndsAttr(player->getPlayerGuid(),resp.ends,resp.allAttr,stages,MiniLog40);

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

// 	if (getIt == 0 && req.reqType == 0)
// 	{
// 		resp.reqType = 2;
// 	}

	
}




void InlineActivityManager::onlineWelFareReq(const  ConnId& connId,OnlineWelFareReq & req,OnlineWelFareResp& resp)
{
	UInt32 onlineTime = 0;
	UInt32 nowTime = TimeUtil::getTimeSec();
	InlineActivityTemplate* inlineActivityTemplate;
	List<Goods> itemList;


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

			Json::Value stages;
			GiftManager::getSingleton().saveEndsAttr(player->getPlayerGuid(),resp.ends,resp.allAttr,stages,MiniLog46);


			player->setInlineActivityData(inlineActivityData);
			InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),ONLINE_WELFARE,inlineActivityData);
		}
	}
	resp.onlineTime = onlineTime;
	resp.gotList = inlineActivityData.m_OnlineWelFareGotList;	


	UInt32 isAllGot = 0;
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
		resp.state = Activity_END;	
		isAllGot = 1;
	}


	if (isAllGot)
	{
		resp.state = Activity_END;
	}
	else//没领取完
	{
		bool allCanFinish = true;
		for( Map<UInt64, InlineActivityTemplate>::Iter *it = gInlineActivityTable->mMap.begin();it!= NULL;it = gInlineActivityTable->mMap.next(it))
		{
			if (it->mValue.type == ONLINE_WELFARE )
			{
				if(it->mValue.time >onlineTime)
				{
					allCanFinish = false;
					break;
				}
			}
		}

		if (allCanFinish == true)
		{
			if (inlineActivityData.m_onlieFinishTime != 0)
			{
			}
			else
			{
				if (req.isLogin == 1)
				{
					inlineActivityData.m_onlieFinishTime = inlineActivityData.m_LastLeaveTime;
				}
				else
				{
					inlineActivityData.m_onlieFinishTime = nowTime;
				}

				player->setInlineActivityData(inlineActivityData);
				InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),ONLINE_WELFARE,inlineActivityData);
			}
		}
		else//没全激活
		{
		}

		if(allCanFinish == true&&(nowTime - inlineActivityData.m_onlieFinishTime) >24*3600 )
		{
			resp.state = Activity_END;									
			for( Map<UInt64, InlineActivityTemplate>::Iter *it = gInlineActivityTable->mMap.begin();it!= NULL;it = gInlineActivityTable->mMap.next(it))
			{
				UInt32 isGot = 0; 
				if (it->mValue.type == ONLINE_WELFARE )
				{
					for (List<UInt32>::Iter *iter = inlineActivityData.m_OnlineWelFareGotList.begin();iter !=NULL;iter = inlineActivityData.m_OnlineWelFareGotList.next(iter))
					{
						if (iter->mValue == it->mKey)
						{
							isGot = 1;
						}
					}
					if (isGot==0)
					{
						isGot =1;
						inlineActivityData.m_OnlineWelFareGotList.insertTail( it->mKey);
						inlineActivityTemplate = &it->mValue;
						InlineActivityManager::getSingleton().getgoodsList(inlineActivityTemplate->award1,inlineActivityTemplate->award2,inlineActivityTemplate->award3,itemList);

						GiftManager::getSingleton().combineSame(itemList);
						player->setInlineActivityData(inlineActivityData);
						InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),ONLINE_WELFARE,inlineActivityData);
					}
				}
			}
			if (itemList.size() > 0)
			{
				char  des[2048] = {};
				char dest[2048] = {};
				String str1;
				String str2;
				char tmp[64] = {};
				sprintf(des,"在线时长奖励;系统;以下为在线时长奖励还未领取的奖励：;");
				for(List<Goods>::Iter * item = itemList.begin();item!=NULL;item = itemList.next(item))
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
			}


		}
		else//没超过24*3600
		{

		}
	}

// 
// 	bool allCanFinish = true;
// 	for( Map<UInt64, InlineActivityTemplate>::Iter *it = gInlineActivityTable->mMap.begin();it!= NULL;it = gInlineActivityTable->mMap.next(it))
// 	{
// 		if (it->mValue.type == ONLINE_WELFARE )
// 		{
// 			if(it->mValue.time >onlineTime)
// 			{
// 				allCanFinish = false;
// 				break;
// 			}
// 		}
// 	}
// 	if (allCanFinish == true)
// 	{
// 		if (inlineActivityData.m_onlieFinishTime == 0)
// 		{
// 			inlineActivityData.m_onlieFinishTime = inlineActivityData.m_LogoutTime;
// 			player->setInlineActivityData(inlineActivityData);
// 			InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),ONLINE_WELFARE,inlineActivityData);
// 		}
// 	}
// 	else
// 	{
// 
// 	}
// 
// 
// 
// 
// 
// 	if (allCanFinish == true&&(nowTime - inlineActivityData.m_onlieFinishTime) >24*3600)
// 	{
// 		resp.state = Activity_END;
// 	}
// 
// 	if (resp.state != Activity_END)
// 	{
// 		UInt32 getIt = 0;
// 		for( Map<UInt64, InlineActivityTemplate>::Iter *it = gInlineActivityTable->mMap.begin();it!= NULL;it = gInlineActivityTable->mMap.next(it))
// 		{
// 			if (it->mValue.type == ONLINE_WELFARE )
// 			{
// 				UInt32 gotIt = 0;
// 				for (List<UInt32>::Iter *iter = inlineActivityData.m_OnlineWelFareGotList.begin();iter !=NULL;iter = inlineActivityData.m_OnlineWelFareGotList.next(iter))
// 				{
// 					if (iter->mValue == it->mKey)
// 					{
// 						gotIt = 1;
// 						break;
// 					}
// 				}
// 				if (gotIt == 0)
// 				{
// 					getIt = 1;
// 					break;
// 				}
// 			}
// 		}
// 		if (getIt == 0 && req.reqType == 0)
// 		{
// 			resp.state = Activity_END;
// // 			inlineActivityData.m_onlieFinishTime = nowTime;
// 		}
// 	}
// 	else
// 	{
// 		for( Map<UInt64, InlineActivityTemplate>::Iter *it = gInlineActivityTable->mMap.begin();it!= NULL;it = gInlineActivityTable->mMap.next(it))
// 		{
// 			UInt32 isGot = 0; 
// 			if (it->mValue.type == ONLINE_WELFARE )
// 			{
// 				for (List<UInt32>::Iter *iter = inlineActivityData.m_OnlineWelFareGotList.begin();iter !=NULL;iter = inlineActivityData.m_OnlineWelFareGotList.next(iter))
// 				{
// 					if (iter->mValue == it->mKey)
// 					{
// 						isGot = 1;
// 					}
// 				}
// 				if (isGot==0)
// 				{
// 					isGot =1;
// 					inlineActivityData.m_OnlineWelFareGotList.insertTail( it->mKey);
// 					inlineActivityTemplate = &it->mValue;
// 					InlineActivityManager::getSingleton().getgoodsList(inlineActivityTemplate->award1,inlineActivityTemplate->award2,inlineActivityTemplate->award3,itemList);
// 
// 					GiftManager::getSingleton().combineSame(itemList);
// 					player->setInlineActivityData(inlineActivityData);
// 					InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),ONLINE_WELFARE,inlineActivityData);
// 				}
// 			}
// 		}
// 		if (itemList.size() > 0)
// 		{
// 			char  des[2048] = {};
// 			char dest[2048] = {};
// 			String str1;
// 			String str2;
// 			char tmp[64] = {};
// 			sprintf(des,"在线时长奖励;系统;以下为在线时长奖励还未领取的奖励：;");
// 			for(List<Goods>::Iter * item = itemList.begin();item!=NULL;item = itemList.next(item))
// 			{
// 				sprintf(tmp,"(%d,%d,%d);",item->mValue.resourcestype,item->mValue.subtype,item->mValue.num);
// 				strcat(dest,tmp);
// 			}
// 			str1 = (String)des;
// 			str2 = (String)dest;
// 
// 			str1 = stringToUtf(str1);
// 			//sdasd;asdsadasd;asdasdasdas;####(1,2,100);(1,1,1000):(1,2,1000)
// 			//发送人;标题;正文;奖励用四个####隔开，后面（1,2,100）你知道的格式		
// 
// 			GMManager::getSingleton().GMEmailSend(0, player->getPlayerGuid(), str1.c_str(),str2.c_str());
// 			
// 		}
// 		
// 	
// 	
// 	}



// 	if (登录)
// 	{
// 		if (全领取)
// 		{
// 			关闭			
// 		}
// 		else
// 		{
// 
// 		}
// 		
// 	}
// 	else
// 	{
// 		if (全领取)
// 		{
// 			关闭			
// 		}
// 		else//没领取完
// 		{
// 			if (全激活)
// 			{
// 				if (设置了全激活时间)
// 				{
// 				}
// 				else
// 				{
// 					设置全激活时间
// 
// 				}
// 			}
// 			else//没全激活
// 			{
// 			}
// 
// 			if(全激活时间超过24小时)
// 			{
// 				关闭
// 				给奖励
// 				设置全领取
// 			}
// 			else
// 			{
// 
// 			}
// 
// 		}	
// 
// 		
// 	}

		



}




void InlineActivityManager::peopleWelfareReq(const  ConnId& connId,PeopleWelfareReq & req,PeopleWelfareResp & resp)
{
	UInt32 nowTime = TimeUtil::getTimeSec();
	InlineActivityTemplate* inlineActivityTemplate;

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if (player == NULL)
	{
		return;
	}
	req.convertJsonToData(req.strReceive);
	resp.reqType = req.reqType;
	PlayerInlineActivityData inlineActivityData = player->getInlineActivityData();

	ServerPublicData  serverPublicData = ServerData::getSingleton().getServerPublicData();

	inlineActivityTemplate = gInlineActivityTable->get(req.id);

	if (req.reqType == 1)
	{
		
		if (inlineActivityTemplate == NULL)
		{
			resp.result = LynxErrno::NotFound;
		}
		if (inlineActivityTemplate->type != PEOPLE_WELFARE)
		{
			resp.result = LynxErrno::NotFound;
		}
		if (nowTime < (serverPublicData.gServerFirstOpenTime + inlineActivityTemplate->begintime) || nowTime > (serverPublicData.gServerFirstOpenTime + inlineActivityTemplate->endtime) )
		{
			resp.result = LynxErrno::TimeNotRight;
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


		if (resp.result == LynxErrno::None)
		{
			inlineActivityData.m_PeopleWelfareGotList.insertTail(req.id);
			InlineActivityManager::getSingleton().getgoodsList(inlineActivityTemplate->award1,inlineActivityTemplate->award2,inlineActivityTemplate->award3,resp.ends);

			Json::Value stages;
			GiftManager::getSingleton().saveEndsAttr(player->getPlayerGuid(),resp.ends,resp.allAttr,stages,MiniLog42);


			player->setInlineActivityData(inlineActivityData);
			InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),PEOPLE_WELFARE,inlineActivityData);
		}
	}

	if (nowTime < (serverPublicData.gServerFirstOpenTime + inlineActivityTemplate->begintime) || nowTime > (serverPublicData.gServerFirstOpenTime + inlineActivityTemplate->endtime) )
	{
		resp.state =  Activity_END;
	}
	resp.rechargeNum = serverPublicData.gRechargeNum;

	resp.gotList = inlineActivityData.m_PeopleWelfareGotList;	

	resp.serverFirstOpenTime = serverPublicData.gServerFirstOpenTime;


}



void InlineActivityManager::sevenLoginAwardReq(const  ConnId& connId,SevenLoginAwardReq & req,SevenLoginAwardResp &resp)
{
	UInt32 count = 0;
	UInt32 getIt = 0;
	UInt32 flag = 0;
	UInt32 nowTime = TimeUtil::getTimeSec();
	InlineActivityTemplate* inlineActivityTemplate;

	
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

			Json::Value stages;
			GiftManager::getSingleton().saveEndsAttr(player->getPlayerGuid(),resp.ends,resp.allAttr,stages,MiniLog44);

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
		
		}
		else
		{
			resp.state = Activity_END;
		}
	}
	else
	{
	
	}


}



//reqtype 1 领取优良奖励 3 领取任务奖励 7领取所有奖励
void InlineActivityManager::sevenDayTaskReq(const  ConnId& connId,SevenDayTaskReq & req,SevenDayTaskResp &resp)
{
	UInt32 nowTime = TimeUtil::getTimeSec();
	InlineActivityTemplate* inlineActivityTemplate;

	
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if (player == NULL)
	{
		return;
	}
	req.convertJsonToData(req.strReceive);
	resp.reqType = req.reqType;
	PlayerInlineActivityData inlineActivityData = player->getInlineActivityData();


	UInt32 ret = InlineActivityManager::getSingleton().checkSevenDayTaskClose(connId,req.reqType);
	if (ret != 0)
	{
		resp.state = Activity_END;
// 		return;
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
			inlineActivityTemplate = &it->mValue;
			break;
		}
	}
	if (inlineActivityTemplate == NULL)
	{
		resp.result = LynxErrno::NotFound;
		return;
	}


	if (req.reqType == 1)
	{
		inlineActivityTemplate = gInlineActivityTable->get(req.id);
		if (inlineActivityTemplate == NULL)
		{
			resp.result = LynxErrno::NotFound;
			return;
		}
		if (inlineActivityTemplate->type != SEVENDAY_TASK)
		{
			resp.result = LynxErrno::NotFound;
			return;
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
						if ( iter->mValue.value == 0)
						{
							InlineActivityManager::getSingleton().getgoodsList(inlineActivityTemplate->award2,"","",resp.ends);
							iter->mValue.value = 1;
						}
						// 						else if (iter->mValue.key == 2 && iter->mValue.value == 0)
						// 						{
						// 							InlineActivityManager::getSingleton().getgoodsList(inlineActivityTemplate->award1,inlineActivityTemplate->award2,"",resp.ends);
						// 							iter->mValue.value = 2;
						// 						}
						else if ( iter->mValue.value == 1)
						{
							InlineActivityManager::getSingleton().getgoodsList("",inlineActivityTemplate->award1,"",resp.ends);
							iter->mValue.value = 2;
						}
					}
					break;					
				}
			}
			Json::Value stages;
			GiftManager::getSingleton().saveEndsAttr(player->getPlayerGuid(),resp.ends,resp.allAttr,stages,MiniLog45);


			player->setInlineActivityData(inlineActivityData);
			InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),SEVENDAY_TASK,inlineActivityData);
		}
	}
	else if (req.reqType == 3)
	{
		resp.result = InlineActivityManager::getSingleton().checkSevenDayCanGotIt( req.day,req.id, inlineActivityData,resp.ends);
		player->setInlineActivityData(inlineActivityData);
		InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),SEVENDAY_FINISHTASK,inlineActivityData);
		Json::Value stages;
		GiftManager::getSingleton().saveEndsAttr(player->getPlayerGuid(),resp.ends,resp.allAttr,stages,MiniLog45);
	}
	else if (req.reqType == 7)
	{
		InlineActivityManager::getSingleton().getSevenDayAllAward(inlineActivityData,resp.ends);
		player->setInlineActivityData(inlineActivityData);
		InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),SEVENDAY_FINISHTASK,inlineActivityData);
		InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),SEVENDAY_TASK,inlineActivityData);
		Json::Value stages;
		GiftManager::getSingleton().saveEndsAttr(player->getPlayerGuid(),resp.ends,resp.allAttr,stages,MiniLog45);
	}

	if (day > 6)
	{	
		resp.state = Activity_END;
	}

	ret = InlineActivityManager::getSingleton().checkSevenDayTaskClose(connId,req.reqType);
	if (ret != 0)
	{
		resp.state = Activity_END;	
	}
	
	resp.firstLoginTime = inlineActivityData.m_FirstLoginTime;
	
	resp.gotList = inlineActivityData.m_SevenDayTaskGotList;	
	resp.day = day + 1;
	resp.dayLeftTime = inlineActivityTemplate->time - (nowTime - inlineActivityData.m_FirstLoginTime)%inlineActivityTemplate->time;
	if (resp.state == Activity_END)
	{
		resp.dayLeftTime = 0;
	}
	resp.finishList = inlineActivityData.m_SevenDayTaskFinishList;


}



void InlineActivityManager::timeAwardReq(const  ConnId& connId,TimeAwardReq & req,	TimeAwardResp&  resp)
{
	UInt32 cost = 0;
	UInt32 nowTime = TimeUtil::getTimeSec();
	Award award;
	InlineActivityTemplate* inlineActivityTemplate;

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if (player == NULL)
	{
		return;
	}
	req.convertJsonToData(req.strReceive);
	resp.reqType = req.reqType;
	PlayerInlineActivityData inlineActivityData = player->getInlineActivityData();


	UInt32 systemRefreshHour = LogicSystem::getSingleton().getRefreshTime(TIME_REFRESH_ID07)/3600;
	if( TimeManager::timeIsTodayDelayHours(inlineActivityData.m_TimeAwardRefreshTime,systemRefreshHour) == false)
	{
		inlineActivityData.m_TimeAwardGotList.clear();

		UInt32 tmp = 0;
		UInt32 gotListCount = 0;
		for( Map<UInt64, InlineActivityTemplate>::Iter *it = gInlineActivityTable->mMap.begin();it!= NULL;it = gInlineActivityTable->mMap.next(it))
		{
			if (it->mValue.type == TIME_AWARD )
			{
				inlineActivityData.m_TimeAwardGotList.insertTail(tmp);
			}
		}

		inlineActivityData.m_TimeAwardRefreshTime = nowTime;
		player->setInlineActivityData(inlineActivityData);
		InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),TIME_AWARD,inlineActivityData);
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

		UInt32 count = 1;
		for (List<UInt32>::Iter *iter = inlineActivityData.m_TimeAwardGotList.begin();iter !=NULL;iter = inlineActivityData.m_TimeAwardGotList.next(iter))
		{
			if (count == (req.id%10))
			{
				if (iter->mValue != 0)
				{
					resp.result = LynxErrno::HadGot;
					break;
				}
				break;
			}
			count ++;
		}

		if (resp.result == LynxErrno::None)
		{
			Goods goods;
			goods.resourcestype = AWARD_BASE;
			goods.subtype = AWARD_BASE_GOLD;
			goods.num = 0 - cost;
			resp.ends.insertTail(goods);
			count = 1;
			for (List<UInt32>::Iter *iter = inlineActivityData.m_TimeAwardGotList.begin();iter !=NULL;iter = inlineActivityData.m_TimeAwardGotList.next(iter))
			{
				if (count == (req.id%10))
				{
					iter->mValue = req.id;
					break;
				}
				count ++;
			}

			InlineActivityManager::getSingleton().getgoodsList(inlineActivityTemplate->award1,inlineActivityTemplate->award2,inlineActivityTemplate->award3,resp.ends);

			for (List<Goods>::Iter * it = resp.ends.begin();it!=NULL;it = resp.ends.next(it))
			{
				if (it->mValue.num > 0 && it->mValue.subtype == AWARD_BASE_STRENGTH&& it->mValue.resourcestype == AWARD_BASE)
				{
					resp.strengthNum = it->mValue.num;
				}
			}

			Json::Value stages;
			GiftManager::getSingleton().saveEndsAttr(player->getPlayerGuid(),resp.ends,resp.allAttr,stages,MiniLog43);


			player->setInlineActivityData(inlineActivityData);
			InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),TIME_AWARD,inlineActivityData);
		}
	}

	resp.gotList = inlineActivityData.m_TimeAwardGotList;


}



void InlineActivityManager::onOneActivityReq(const  ConnId& connId, OneActivityReq &req )
{
	ActivityResp resp;
	Activity activity;

	req.convertJsonToData(req.strReceive);

	
	if (MONTH_SIGN == req.id)
	{
		MonthSingnReq monthSingnReq;
		MonthSingnResp monthSingnResp;
		monthSingnReq.reqType = req.reqType;
		InlineActivityManager::getSingleton().monthSingnReq( connId,monthSingnReq,monthSingnResp);
		activity.id = MONTH_SIGN;
// 		activity.state = monthSingnResp.state;
		activity.data = monthSingnResp.convertDataToJson().c_str();
		resp.activitys.insertTail(activity);
	}


	if (SEVENDAY_TASK == req.id)
	{
		SevenDayTaskReq sevenDayTaskReq;
		SevenDayTaskResp  sevenDayTaskResp;
		sevenDayTaskReq.reqType = req.reqType;
		sevenDayTaskReq.id = req.index;
		//todo 
// 		sevenDayTaskReq.day = 2;
		InlineActivityManager::getSingleton().sevenDayTaskReq( connId,sevenDayTaskReq,sevenDayTaskResp);
		activity.id = SEVENDAY_TASK;
		activity.state = sevenDayTaskResp.state;
		activity.data = sevenDayTaskResp.convertDataToJson().c_str();
		resp.activitys.insertTail(activity);
	}


	if (GROWTH_FOUND == req.id)
	{
		GrowFoundReq growFoundReq;
		GrowFoundResp growFoundResp;
		growFoundReq.id = req.index;
		growFoundReq.reqType = req.reqType;
		InlineActivityManager::getSingleton().growFoundReq( connId,growFoundReq,growFoundResp);
		activity.id = GROWTH_FOUND;
		activity.state = growFoundResp.state;
		activity.data = growFoundResp.convertDataToJson().c_str();
		resp.activitys.insertTail(activity);
	}

	if (PEOPLE_WELFARE == req.id)
	{
		PeopleWelfareReq peopleWelfareReq;
		PeopleWelfareResp peopleWelfareResp;
		peopleWelfareReq.id = req.index;
		peopleWelfareReq.reqType = req.reqType;
		InlineActivityManager::getSingleton().peopleWelfareReq( connId,peopleWelfareReq,peopleWelfareResp);
		activity.id = PEOPLE_WELFARE;
		activity.state = peopleWelfareResp.state;
		activity.data = peopleWelfareResp.convertDataToJson().c_str();
		resp.activitys.insertTail(activity);
	}

	if (TIME_AWARD == req.id)
	{
		TimeAwardReq timeAwardReq;
		TimeAwardResp timeAwardResp;
		timeAwardReq.id = req.index;
		timeAwardReq.reqType = req.reqType;
		InlineActivityManager::getSingleton().timeAwardReq( connId,timeAwardReq,timeAwardResp);
		activity.id = TIME_AWARD;
// 		activity.state = timeAwardResp.reqType;
		activity.data = timeAwardResp.convertDataToJson().c_str();
		resp.activitys.insertTail(activity);
	}

	if (ONLINE_WELFARE == req.id)
	{
		OnlineWelFareReq  onlineWelFareReq;
		OnlineWelFareResp onlineWelFareResp;
		onlineWelFareReq.id = req.index;
		onlineWelFareReq.reqType = req.reqType;
		InlineActivityManager::getSingleton().onlineWelFareReq( connId,onlineWelFareReq,onlineWelFareResp);
		activity.id = ONLINE_WELFARE;
		activity.state = onlineWelFareResp.state;
		activity.data = onlineWelFareResp.convertDataToJson().c_str();
		resp.activitys.insertTail(activity);
	}

	if (SEVENDAY_LOGIN_AWARD == req.id)
	{
		SevenLoginAwardReq sevenLoginAwardReq;
		sevenLoginAwardReq.reqType = req.reqType;
		sevenLoginAwardReq.id = req.index;
		SevenLoginAwardResp sevenLoginAwardResp;
		InlineActivityManager::getSingleton().sevenLoginAwardReq( connId,sevenLoginAwardReq,sevenLoginAwardResp);
		activity.id = SEVENDAY_LOGIN_AWARD;
		activity.state = sevenLoginAwardResp.state;
		activity.data = sevenLoginAwardResp.convertDataToJson().c_str();
		resp.activitys.insertTail(activity);
	}


	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,ONE_ACTIVITY_RESP,jsonStr);
	//2 6 1 4 7

}


void InlineActivityManager::checkOnlineWelfareLastTime(Player *player )
{
	InlineActivityTemplate* inlineActivityTemplate;
	List<Goods> itemList;

	PlayerInlineActivityData inlineActivityData = player->getInlineActivityData();
	if (inlineActivityData.m_onlieFinishTime > 0)
	{
		return;
	}

	UInt32 nowTime = TimeUtil::getTimeSec();

	UInt32 onlineTime = inlineActivityData.m_OnlineWelFareOnlineTime + nowTime - inlineActivityData.m_LoginTime;

	bool allCanFinish = true;
	for( Map<UInt64, InlineActivityTemplate>::Iter *it = gInlineActivityTable->mMap.begin();it!= NULL;it = gInlineActivityTable->mMap.next(it))
	{
		if (it->mValue.type == ONLINE_WELFARE )
		{
			if(it->mValue.time >onlineTime)
			{
				allCanFinish = false;
				break;
			}
		}
	}

	if (allCanFinish == true)
	{
		if (inlineActivityData.m_onlieFinishTime != 0)
		{
		}
		else
		{
			inlineActivityData.m_onlieFinishTime = nowTime;
			player->setInlineActivityData(inlineActivityData);
			InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),ONLINE_WELFARE,inlineActivityData);
		}
	}
	else//没全激活
	{
	}

	if(allCanFinish == true&&(nowTime - inlineActivityData.m_onlieFinishTime) >24*3600 )
	{
		for( Map<UInt64, InlineActivityTemplate>::Iter *it = gInlineActivityTable->mMap.begin();it!= NULL;it = gInlineActivityTable->mMap.next(it))
		{
			UInt32 isGot = 0; 
			if (it->mValue.type == ONLINE_WELFARE )
			{
				for (List<UInt32>::Iter *iter = inlineActivityData.m_OnlineWelFareGotList.begin();iter !=NULL;iter = inlineActivityData.m_OnlineWelFareGotList.next(iter))
				{
					if (iter->mValue == it->mKey)
					{
						isGot = 1;
					}
				}
				if (isGot==0)
				{
					isGot =1;
					inlineActivityData.m_OnlineWelFareGotList.insertTail( it->mKey);
					inlineActivityTemplate = &it->mValue;
					InlineActivityManager::getSingleton().getgoodsList(inlineActivityTemplate->award1,inlineActivityTemplate->award2,inlineActivityTemplate->award3,itemList);

					GiftManager::getSingleton().combineSame(itemList);
					player->setInlineActivityData(inlineActivityData);
					InlineActivityManager::getSingleton().inlineActivitySave(player->getPlayerGuid(),ONLINE_WELFARE,inlineActivityData);
				}
			}
		}
		if (itemList.size() > 0)
		{
			char  des[2048] = {};
			char dest[2048] = {};
			String str1;
			String str2;
			char tmp[64] = {};
			sprintf(des,"在线时长奖励;系统;以下为在线时长奖励还未领取的奖励：;");
			for(List<Goods>::Iter * item = itemList.begin();item!=NULL;item = itemList.next(item))
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
		}


	}
	else
	{

	}


}	


