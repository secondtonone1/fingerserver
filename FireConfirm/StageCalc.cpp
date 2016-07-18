
#include "FireConfirmManager.h"
#include "../LogicSystem.h"
#include "StringConverter.h"
#include "../PlatformLib/SerializeType.h"
#include "../PersistSystem.h"
#include "../PlatformLib/Utilex.h"
#include "../CommonLib/CopyFinishTime.h"
#include "Gift.h"
#include "Box.h"
#include "StageCalc.h"
#include "LocalData.h"
#include "PVPSystem.h"
#include "NewException.h"

#include "../Ranking/Ranking.h"
using namespace Lynx;


void StageCalcManager::onTwelvePalaceUnlock(const  ConnId&  connId,CGTwelvePalaceUnlock& msg )
{

	UInt32 chapterID =0;
	UInt32 getIt =0;
	UInt32 isExit = 0;
	UInt32 isMeet = 0;
	UInt32 stageID = 0;

	Goods goods;

	TwelvePalaceUnlockResp resp;
	List<UInt32> stageIDs;
	List<Goods> itemList;

	Map<UInt64, StageTemplate> IDToStage;
	PlayerChapterList chapterList;


	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	msg.convertJsonToData(msg.jsonStr);
	resp.doType = msg.doType;
	resp.id = msg.id;
	resp.result =LynxErrno::None;

	chapterID = msg.id;


	PlayerDailyResetData dailyResetData = player->getPlayerDailyResetData();
	List<KeyValue> m_ChapterUnlockeded = player->getChapterUnlocked();
	chapterList = player->getChapterList();
	if(msg.doType ==1)//解锁
	{

		getIt =0;
		for(List<KeyValue>::Iter * it = m_ChapterUnlockeded.begin();it != NULL; it = m_ChapterUnlockeded.next(it))
		{
			if (it->mValue.key == msg.id)
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
			if ( dailyResetData.m_nTwelvePalaceUnlockCount >=1)
			{

				//消耗
				dailyResetData.m_nTwelvePalaceUnlockCount --;


				//解锁	
				StageCalcManager::getSingleton().setChapterLock(connId ,msg.id,1);	
				m_ChapterUnlockeded = player->getChapterUnlocked();
			}
			else
			{
				resp.result = LynxErrno::bugleNotEnough;
			}
		}
		else
		{
			resp.result = LynxErrno::IsUnlocked;
		}


		IDToStage = StageManager::getSingleton().getMapIDToStageByChapterID(msg.id);

		stageID = StageCalcManager::getSingleton().getMinKey(IDToStage,0 );

		isMeet = StageManager::getSingleton().isMeetCondition( connId, stageID);
		if (isMeet != LynxErrno::None)
		{
			resp.result = isMeet;
		}
	}
	else if (msg.doType == 2)//重置次数 满足3条件 已解锁，有号角，章存在
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
		if ( getIt == 1)
		{
			if ( dailyResetData.m_nTwelvePalaceUnlockCount >=1)
			{
				for (List<PlayerChapterData>::Iter * iter = chapterList.m_listChapterData.begin();iter != NULL;iter = chapterList.m_listChapterData.next(iter))
				{
					if (iter->mValue.m_nChapterId == chapterID)
					{
						isExit = 1;
						dailyResetData.m_nTwelvePalaceUnlockCount --;


						//挑战次数变0
						for(List<StageData>::Iter* it = iter->mValue.m_listStageDatas.begin(); it != NULL;it = iter->mValue.m_listStageDatas.next(it))
						{
							it->mValue.m_nChallengTimes = 0;	
							if (it->mValue.m_nStageID != 0)
							{
								stageIDs.insertTail(it->mValue.m_nStageID);
							}
						}

						break;						
					}
				}

				if(isExit == 0)
				{
					resp.result = LynxErrno::IsUnlocked;
				}
			}
			else
			{
				resp.result = LynxErrno::bugleNotEnough;
			}
		}
		else
		{
			resp.result = LynxErrno::IsLocked;
		}
	}	

	else if(msg.doType == 3)//doType 3 单个重置，花费元宝 vip等级
	{
		chapterID = StageManager::getSingleton().getChapterID(msg.id);
		UInt32 canBuyTimes = 0;
		UInt32 buyCost =0;
		if(gStageTable->get(msg.id)->isBoss == 1)
		{
			canBuyTimes = gVipTable->get(player->getVipLevel())->twelvePalaceBossBuy;
			buyCost = GlobalVarManager::getSingleton().getTwelvePalace().buybosscost;
		}
		else
		{
			canBuyTimes = gVipTable->get(player->getVipLevel())->twelvePalaceBuy;
			buyCost = GlobalVarManager::getSingleton().getTwelvePalace().buycost;
		}
		
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
		if ( getIt == 1)
		{

			if ( canBuyTimes <= StageCalcManager::getSingleton().getResetStageCount(player->getPlayerGuid(),msg.id)   )
			{
				resp.result = LynxErrno::VipBuyTimesNotEnough;
			}
			else
			{
				if ( player->getPlayerGold() >= buyCost)
				{
					goods.resourcestype = AWARD_BASE;
					goods.subtype = AWARD_BASE_GOLD;
					goods.num = 0 - buyCost;
					for (List<PlayerChapterData>::Iter * iter = chapterList.m_listChapterData.begin();iter != NULL;iter = chapterList.m_listChapterData.next(iter))
					{
						if (iter->mValue.m_nChapterId == chapterID)
						{
							isExit = 1;
							StageCalcManager::getSingleton().addResetStages(player->getPlayerGuid(),msg.id,1,true);


							//挑战次数变0
							for(List<StageData>::Iter* it = iter->mValue.m_listStageDatas.begin(); it != NULL;it = iter->mValue.m_listStageDatas.next(it))
							{							
								if (it->mValue.m_nStageID == msg.id)
								{
									itemList.insertTail(goods);
									it->mValue.m_nChallengTimes = 0;	
									stageIDs.insertTail(it->mValue.m_nStageID);
									break;
								}
							}

							break;						
						}

					}

					if(isExit == 0)
					{
						resp.result = LynxErrno::IsUnlocked;
					}
				}
				else
				{
					resp.result = LynxErrno::RmbNotEnough;
				}
			}
		}
		else
		{
			resp.result = LynxErrno::IsLocked;
		}

		resp.buyTimes = StageCalcManager::getSingleton().getResetStageCount(player->getPlayerGuid(),msg.id) ;		
	}

	if (resp.result == LynxErrno::None)
	{
		player->setPlayerDailyResetData(dailyResetData);
		player->setchapterList(chapterList);
		player->setChapterUnlocked(m_ChapterUnlockeded);
		GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,itemList);
		player->getPersistManager().setDirtyBit(DAILYRESETBIT);	
		resp.num = dailyResetData.m_nTwelvePalaceUnlockCount;
		resp.gold = player->getPlayerGold();
		
	}

	StageManager::getSingleton().saveAndSend(player->getPlayerGuid(),player->getChapterList(),stageIDs);

	std::string jsonStr = resp.convertDataToJson();	
	NetworkSystem::getSingleton().sender( connId,TWELVE_PALACE_UNLOCK_RESP,jsonStr);

}

//onTwelvePalaceStartReq 拷贝了onStartCopy 代码
void StageCalcManager::onTwelvePalaceStartReq(const  ConnId& connId ,CGTwelvePalaceStart& msg )
{
	UInt32 flag;
	TwelvePalaceStartResp resp;
	PlayerFireConfirmData fireConfirmData;	

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	msg.convertJsonToData(msg.jsonStr);

	player->ResetFireConfirmData();
	flag = StageManager::getSingleton().canChallengeStage( connId, msg.id);
	if (flag != LynxErrno::None)
	{
		resp.id = msg.id;
		resp.result =flag;
		std::string jsonStr = resp.convertDataToJson();	
		NetworkSystem::getSingleton().sender( connId,TWELVE_PALACE_BEGIN_RESP,jsonStr);
		return;
	}

	FireConfirmManager::getSingleton().SetCopyStart(player,msg.id);	

	FireConfirmManager::getSingleton().SendFireConfirmCondition(connId);

	StageTemplate *stageTemplate = gStageTable->get(msg.id);
	if (stageTemplate == NULL)
	{
		return;		
	}
	if(stageTemplate->isBoss == 1)
	{
		LocalDataManager::getSingleton().setTwelvePalaceLocalData(connId,msg.id,msg.awardType);
	}	
	if (msg.awardType == 1 )
	{
		if(stageTemplate->isBoss != 1)
		{
			return;
		}
		fireConfirmData = player->GetFireConfirmData();
		fireConfirmData.m_AwardTypes.insertTail(msg.awardType);
		player->SetFireConfirmData(fireConfirmData);

		//send 乱入者
		PlayerBaseData baseData = player->getPlayerBaseData();
		PersistFindSimilarPowerReq req;
		req.playerGuid = player->getPlayerGuid();
		req.low = baseData.m_nPower *(100 - 3) /100;
		req.high = baseData.m_nPower * (100 + 3) /100;
		req.times = 1;
		PersistSystem::getSingleton().postThreadMsg(req, 0);
		return;
	}
	else
	{
		resp.id = msg.id;
		resp.result =LynxErrno::None;
		std::string jsonStr = resp.convertDataToJson();	
		NetworkSystem::getSingleton().sender( connId,TWELVE_PALACE_BEGIN_RESP,jsonStr);
	}

}
// onTwelvePalaceEndReq拷贝了onEndCopy代码
void StageCalcManager::onTwelvePalaceEndReq(const  ConnId& connId ,CGTwelvePalaceEnd& msg )
{
	//步骤：清理数据，验证，战斗验证，扣除，增益，发放，存盘，发送
	UInt32 isRecord =0;//破纪录
	UInt32 star = 0;
	UInt32 finishTimes =0 ;
	UInt32 recordBreaking =0;
	UInt32 ret = 0 ;
	Goods goods;	
	KeyValue keyValue;

	List<UInt32> checkList;
	List<KeyValue> counter;
	List<Goods> tmpItemList;
	List<Goods> cost;
	List<Goods> card;	
	List<Goods> itemList;
	List<Award> awards;	
	Award award;
	Map<UInt16, PlayerChapterData *> mapIDToChapter;
	ChapterEndResp tmpResp;
	TwelvePalaceEndResp resp;
	PlayerFireConfirmData mFireConfirmData;

	msg.convertJsonToData(msg.jsonStr);
	tmpResp.chapterID = msg.id;
	tmpResp.result = msg.result;


	resp.star = msg.star;
	resp.doType = msg.doType;
	star = msg.star;


	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);

	if (gStageTable->get(msg.id)->mType == STAGE_TRIAL)//多人副本
	{
		PVPSystem::getSingleton().removeRoom(player->getRoomID());
		player->setRoomID(0);		
	}

	if (msg.doType == 0)//扫荡
	{

		UInt32 mopupTimes = msg.mopupTimes;
		if (msg.mopupTimes > 5)
		{
			msg.mopupTimes =5;
		}
		if (msg.mopupTimes > StageManager::getSingleton().getNormalStageLeftTimes(connId,msg.id))
		{
			msg.mopupTimes = StageManager::getSingleton().getNormalStageLeftTimes(connId,msg.id);
		}		
		
		player->ResetFireConfirmData();
		
		ret = StageCalcManager::getSingleton().getAwards(connId ,msg.doType,msg.awardMonsterList,msg.id,msg.mopupTimes);

		if ( ret != LynxErrno::None && msg.result == LynxErrno::None)
		{
			tmpResp.result = msg.result;
		}
		
		mFireConfirmData =  player->GetFireConfirmData();

	}
	else
	{

		ret = FireConfirmManager::getSingleton().CheckCopyFinishTime(player);
		if ( ret != LynxErrno::None && msg.result == LynxErrno::None)
		{
			tmpResp.result = msg.result;
		}

		if (msg.result != LynxErrno::None)
		{
			player->ResetFireConfirmData();
			std::string jsonStr = tmpResp.convertDataToJson();	
			NetworkSystem::getSingleton().sender( connId,CHAPTER_END_RESP,jsonStr);
			return;
		}	

		ret =StageCalcManager::getSingleton().getAwards(connId ,msg.doType,msg.awardMonsterList,msg.id,1);
		if(ret != LynxErrno::None)
		{
			tmpResp.result = msg.result;
		}

		FireConfirmManager::getSingleton().updateFireConfirmData(connId);

		mFireConfirmData =  player->GetFireConfirmData();

		mFireConfirmData.m_Star = msg.star;
		player->SetFireConfirmData(mFireConfirmData);//这里不保存下面就会清掉了


		if (mFireConfirmData.m_CopyStartTime == -1 || mFireConfirmData.m_CopyID != msg.id)
		{		
			tmpResp.result =LynxErrno::AttackPowerNotRight;
			std::string jsonStr = tmpResp.convertDataToJson();	
			NetworkSystem::getSingleton().sender( connId,CHAPTER_END_RESP,jsonStr);
			return;
		}
		UInt32 flag =0;
		if (flag != LynxErrno::None)
		{
			tmpResp.result =flag;
			std::string jsonStr = tmpResp.convertDataToJson();	
			NetworkSystem::getSingleton().sender( connId,CHAPTER_END_RESP,jsonStr);
			return;
		}
	}

	UInt32 goldcost =0;
	UInt32  gold = player->getPlayerGold();

	if (gStageTable->get(msg.id)->mType == STAGE_TRIAL )//多人副本 
	{
		TrialTemplate trialTemplate = GlobalVarManager::getSingleton().gettrial();
		goldcost = trialTemplate.ratecost.findValue(mFireConfirmData.m_AwardTimes);//倍数 元宝花费

		if (gold < goldcost)
		{
			tmpResp.result = LynxErrno::RmbNotEnough;

		}
		PlayerDailyResetData playerDailyResetData =player->getPlayerDailyResetData();


		VipTemplate vipTemplate;//todo mei yanzheng
		for(List<VipTemplate>::Iter *iter1 = gVipTable->mVip.begin();iter1 != NULL;iter1 = gVipTable->mVip.next(iter1))
		{
			if (iter1->mValue.id == player->getVipLevel())
			{
				vipTemplate = iter1->mValue;
				break;
			}
		}
		if (tmpResp.result != LynxErrno::None)//失败
		{
			std::string jsonStr = tmpResp.convertDataToJson();	
			NetworkSystem::getSingleton().sender(connId,CHAPTER_END_RESP,jsonStr);
			return;
		}	
		double multiply =0;
		multiply += mFireConfirmData.m_AwardTimes;
		Record record;
		record.name = player->getPlayerName();
		record.playerID = player->getPlayerGuid();
		record.val = TimeUtil::getTimeSec() - mFireConfirmData.m_CopyStartTime;
		if (record.val <= gStageTable->get(msg.id)->limittime1)//
		{			
			multiply += 0.1;
			if (RankingManager::getSingleton().setRecord(STAGE_RECORD_TYPE,mFireConfirmData.m_CopyID,record,true) <4)//破纪录 4以下是成功
			{
				CGRecord recordMsg;
				recordMsg.typeID = STAGE_RECORD_TYPE;
				RankingManager::getSingleton().onRecordReq(connId,recordMsg);
				recordBreaking =1;
				//todo 加入到成就 

			}
		}

		if (/*isFriend()*/1)//是否是好友
		{
			multiply += 0.1;
		}


		for(List<Award>::Iter * item = mFireConfirmData.m_AwardsList.begin();item != NULL; item = mFireConfirmData.m_AwardsList.next(item))
		{			
			for( List<Goods>::Iter * iter = item->mValue.award.begin();iter != NULL;iter = item->mValue.award.next(iter))
			{
				iter->mValue.num = iter->mValue.num * (multiply + mFireConfirmData.m_AwardTimes); 
			}
			break;
		}
		for(List<Card>::Iter * item1 = mFireConfirmData.m_CardsList.begin();item1 != NULL; item1 = mFireConfirmData.m_CardsList.next(item1))
		{			
			for( List<Goods>::Iter * iter = item1->mValue.card.begin();iter != NULL;iter = item1->mValue.card.next(iter))
			{
				iter->mValue.num = iter->mValue.num * (multiply + mFireConfirmData.m_AwardTimes); 
			}
			break;
		}

		goods.resourcestype =AWARD_BASE;
		goods.subtype =AWARD_BASE_GOLD;
		goods.num -= goldcost;
		mFireConfirmData.m_CostList.insertTail(goods);


	}
	else if (gStageTable->get(msg.id)->mType == STAGE_TWELVEPALACE)//十二宗宫
	{
		mFireConfirmData =  player->GetFireConfirmData();
		Record record;
		record.name = player->getPlayerName();
		record.playerID = player->getPlayerGuid();
		record.modleID = player->getPlayerModelID();
		record.val = TimeUtil::getTimeSec() - mFireConfirmData.m_CopyStartTime;

		
		

		msg.time = TimeUtil::getTimeSec() - mFireConfirmData.m_CopyStartTime;

		mFireConfirmData =  player->GetFireConfirmData();		
		StageCalcManager::getSingleton().addAwardGain( connId ,mFireConfirmData,msg.awardTypes,msg.time, recordBreaking);

	}
	UInt32 getIt = 0;	
	for (List<UInt32>::Iter* it = mFireConfirmData.m_AwardTypes.begin();it != NULL;it = mFireConfirmData.m_AwardTypes.next(it))
	{
		if (it->mValue ==1)//入侵者
		{
			getIt = 1;
		}
	}
	StageTemplate *stageTemplate;
	stageTemplate = gStageTable->get(msg.id);
	if(stageTemplate == NULL)
	{
		LOG_WARN("stageTemplate is NULL stageID = %u", msg.id);
		return;
	}

	if (msg.doType == 1 && getIt == 1)//挑战，入侵者
	{		
		if (stageTemplate->isBoss != 1)
		{
			LOG_WARN("Selected random enter bu is not boss stageID =  %u ", msg.id);
			return;
		}
	}
	

	//普通关卡 一般消耗，多人副本为零
	
	if (stageTemplate->mStrengthCost >0)
	{
		goods.resourcestype = AWARD_BASE ;
		goods.subtype = AWARD_BASE_STRENGTH;
		goods.num -= stageTemplate->mStrengthCost;
		mFireConfirmData.m_CostList.insertTail(goods);
	}

	player->SetFireConfirmData(mFireConfirmData);
	FireConfirmManager::getSingleton().saveAndGetResult(connId,tmpResp,0);
	
	tmpResp.result =LynxErrno::None;
	tmpResp.star = star;
	tmpResp.recordBreaking = recordBreaking;

	resp.allAttr = mFireConfirmData.m_AddSendjs;
	resp.time = msg.time;
	resp.firstAwards = tmpResp.firstAwards;
	resp.awards = tmpResp.awards;
	resp.cost = tmpResp.cost;
	resp.cards = tmpResp.cards;
	resp.monsterDropList = tmpResp.monsterDropList;
	resp.fixedList = tmpResp.fixedList;
	resp.ends = tmpResp.ends;
	resp.id = tmpResp.chapterID;
	resp.result = tmpResp.result;
	resp.recordBreaking = tmpResp.recordBreaking;
	resp.mopupTimes = msg.mopupTimes;
			

	if (msg.doType == 1 && stageTemplate->isBoss == 1&& getIt == 1)//挑战
	{		
		
		Record record = RankingManager::getSingleton().getRecord(TWELVE_PALACE_RECORD_TYPE,msg.id);
		resp.recordPlayerID = record.playerID;
		resp.recordName = record.name;
		resp.recordModleID = record.modleID;
		resp.recordTime = record.val;		
	}
	mFireConfirmData = player->GetFireConfirmData();
	resp.allAttr = mFireConfirmData.m_AddSendjs;

	std::string jsonStr = resp.convertDataToJson();	
	NetworkSystem::getSingleton().sender( connId,TWELVE_PALACE_END_RESP,jsonStr);

	//更新每日任务 wwc
	if(msg.doType == 1)
		player->getAchieveManager().updateDailyTaskData(DLYTELVESUCCESS, 1 );

	if(msg.doType == 0)
		player->getAchieveManager().updateDailyTaskData(DLYTELVESUCCESS, msg.mopupTimes );
}


//doType 0 扫荡 1 挑战
UInt32 StageCalcManager::getAwards(const  ConnId& connId ,UInt32 doType,List<AwardMonsterDamage> awardMonsterList,UInt32 stageID,UInt32 times)
{
	UInt32 getIt = 0 ;
	UInt32 chapterID =0;	
	Goods goods;
	StageData stageData;
	PlayerChapterData *chapterData = NULL;
	List<Goods> itemList;


	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);

	List<KeyValue> m_ChapterUnlockeded = player->getChapterUnlocked();

	chapterID = StageManager::getSingleton().getChapterID(stageID);

	PlayerChapterList chapterList = player->getChapterList();

	StageTemplate *stageTemplate = gStageTable->get(stageID);

	for (List<PlayerChapterData>::Iter * iter = chapterList.m_listChapterData.begin();iter != NULL;iter = chapterList.m_listChapterData.next(iter))
	{
		if (iter->mValue.m_nChapterId == chapterID)
		{
			chapterData = &iter->mValue;

		}
	}
	if (chapterData == NULL)
	{
		return LynxErrno::NotFound;
	}


	if (doType == 0)//扫荡
	{
		if (gStageTable->get(stageID)->mType == STAGE_TWELVEPALACE)//十二宗宫
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
			if (times == 0)
			{
				return LynxErrno::TimesNotEnough;
			}


			for (List<StageData>::Iter *iter1 = chapterData->m_listStageDatas.begin();iter1 != NULL;iter1 = chapterData->m_listStageDatas.next(iter1))
			{
				if (iter1->mValue.m_nStageID != stageID)
				{
					continue;
				}

				stageTemplate = gStageTable->get(iter1->mValue.m_nStageID);
				if (stageTemplate->mopuptype == 0)
				{
					continue;
				}

				if ( StageManager::getSingleton().getStageStar(connId,iter1->mValue.m_nStageID) < 3)
				{
					continue;
				}
				FireConfirmManager::getSingleton().SetCopyStart(player,iter1->mValue.m_nStageID);
 				for(UInt32 i =0;i<times;i++)//1次 5次
				{
					GiftManager::getSingleton().getChapterAward( connId,awardMonsterList);					
				}
			}
			// 			player->setchapterList(chapterList);

		}
		else
		{

			if (stageTemplate->mopuptype == 0 || StageManager::getSingleton().getStageStar(connId,stageID) < 3)
			{
				return LynxErrno::StarNotEnouth;
			}
			for (List<StageData>::Iter *iter1 = chapterData->m_listStageDatas.begin();iter1 != NULL;iter1 = chapterData->m_listStageDatas.next(iter1))
			{
				if (iter1->mValue.m_nStageID == stageID)
				{					
					stageData = iter1->mValue;
					break;
				}

			}
			for(UInt32 i =0;i<(gStageTable->get(stageID)->mChallengetimesLimit - stageData.m_nChallengTimes );i++)
			{
				GiftManager::getSingleton().getChapterAward( connId,awardMonsterList);
			}

		}
	}
	else
	{
		GiftManager::getSingleton().getChapterAward( connId,awardMonsterList);

	}
	// 	GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,itemList);

	// 	PlayerFireConfirmData mFireConfirmData =  player->GetFireConfirmData();
	// 	for(List<Award>::Iter *iter2 = mFireConfirmData.m_AwardsList.begin();iter2 != NULL;iter2 = mFireConfirmData.m_AwardsList.next(iter2))
	// 	{
	// 		iter2->mValue.award += itemList;
	// 		break;
	// 	}
	// 	player->SetFireConfirmData(mFireConfirmData);

	return LynxErrno::None;

}

void StageCalcManager::addAwardGain(const  ConnId& connId ,PlayerFireConfirmData &mFireConfirmData,List<UInt32> awardType,UInt32 time ,UInt32 &recordBreaking)
{
	//勾选宗主关的狭路相逢功能，并击败乱入者，宗主关的所有收益都会提升至200%
	UInt32 goldcost =0;
	double num =0;
	Goods goods;
	List<RateOfGood> rateOfGoods;

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);

	// 	PlayerDailyResetData playerDailyResetData =player->getPlayerDailyResetData();


	double multiply =0;
	multiply += mFireConfirmData.m_AwardTimes;
	Record record;
	record.name = player->getPlayerName();
	record.playerID = player->getPlayerGuid();
	record.val = time;
	if (mFireConfirmData.m_CopyID == 0)
	{
		return;
	}
	UInt32 getIt = 0;	
	for (List<UInt32>::Iter* it = mFireConfirmData.m_AwardTypes.begin();it != NULL;it = mFireConfirmData.m_AwardTypes.next(it))
	{
		if (it->mValue ==1)//入侵者
		{
			getIt = 1;
		}
	}
	if (time != 0 && time <= gStageTable->get(mFireConfirmData.m_CopyID)->limittime1&&getIt == 1)//
	{
// 		multiply += 0.1;//奖励加成去掉
		if (RankingManager::getSingleton().setRecord(TWELVE_PALACE_RECORD_TYPE,mFireConfirmData.m_CopyID,record,true) <4)//破纪录 4以下是成功
		{			
			recordBreaking =1;
			//todo 加入到成就 
		}
	}

	if (time != 0&&getIt == 1)
	{
		 player->getRankingPersonalManager().setRecord(TWELVE_PALACE_MY_RECORD,mFireConfirmData.m_CopyID,record,true);//设置个人最好成绩
	}
	


	// 	if (/*isFriend()*/1)//是否是好友
	// 	{
	// 		multiply += 0.1;
	// 	}



	RateOfGood rateOfGood;

	for(List<UInt32>::Iter * it = mFireConfirmData.m_AwardTypes.begin();it != NULL;it = mFireConfirmData.m_AwardTypes.next(it))
	{
		if (it->mValue == 1)
		{
			rateOfGood.resourcesType =0;
			rateOfGood.subtype =0;
			rateOfGood.rate = 10000;//十二宗宫加倍
			if(rateOfGood.rate != 0)
			{
				rateOfGoods.insertTail(rateOfGood);
			}
		}
	}



	for(List<UInt32>::Iter * it = awardType.begin();it != NULL;it = awardType.next(it))
	{

		AwardAddRateTemplate* awardAddRateTemplate = gAwardAddRateTable->get(it->mValue);
		rateOfGoods += awardAddRateTemplate->rateOfGoods;
	}

	for( List<Goods>::Iter * iter = mFireConfirmData.m_FixedList.begin();iter != NULL;iter = mFireConfirmData.m_FixedList.next(iter))
	{
		num =0;
		for (List<RateOfGood>::Iter * iter2 = rateOfGoods.begin();iter2 != NULL;iter2 = rateOfGoods.next(iter2))
		{
			if (iter2->mValue.resourcesType == iter->mValue.resourcestype ||iter2->mValue.resourcesType == 0)
			{
				if (iter2->mValue.subtype == iter->mValue.subtype || iter2->mValue.subtype ==0)
				{
					num += iter->mValue.num * (iter2->mValue.rate/10000);
				}
			}
		}
		iter->mValue.num += num;
	}

	for(List<Award>::Iter * item = mFireConfirmData.m_AwardsList.begin();item != NULL; item = mFireConfirmData.m_AwardsList.next(item))
	{			
		for( List<Goods>::Iter * iter = item->mValue.award.begin();iter != NULL;iter = item->mValue.award.next(iter))
		{
			num =0;
			for (List<RateOfGood>::Iter * iter2 = rateOfGoods.begin();iter2 != NULL;iter2 = rateOfGoods.next(iter2))
			{
				if (iter2->mValue.resourcesType == iter->mValue.resourcestype ||iter2->mValue.resourcesType == 0)
				{					
					if (iter->mValue.resourcestype == AWARD_STAGEDATA || iter->mValue.resourcestype == AWARD_TWELVEPALACE_STAGEDATA)//关卡信息不用加倍
					{
						continue;
					}

					if (iter2->mValue.subtype == iter->mValue.subtype || iter2->mValue.subtype ==0)
					{
						num += iter->mValue.num * (iter2->mValue.rate/10000);
					}
				}
			}
			iter->mValue.num += num;
		}
		break;
	}
	for(List<Award>::Iter * item = mFireConfirmData.m_FirstAwardsList.begin();item != NULL; item = mFireConfirmData.m_FirstAwardsList.next(item))
	{			
		for( List<Goods>::Iter * iter = item->mValue.award.begin();iter != NULL;iter = item->mValue.award.next(iter))
		{
			num =0;
			for (List<RateOfGood>::Iter * iter2 = rateOfGoods.begin();iter2 != NULL;iter2 = rateOfGoods.next(iter2))
			{
				if (iter2->mValue.resourcesType == iter->mValue.resourcestype ||iter2->mValue.resourcesType == 0)
				{
					if (iter->mValue.resourcestype == AWARD_STAGEDATA || iter->mValue.resourcestype == AWARD_TWELVEPALACE_STAGEDATA)//关卡信息不用加倍
					{
						continue;
					}
					if (iter2->mValue.subtype == iter->mValue.subtype || iter2->mValue.subtype ==0)
					{
						num += iter->mValue.num * (iter2->mValue.rate/10000);
					}
				}
			}
			iter->mValue.num += num;
		}
		break;
	}

	for( List<Goods>::Iter * iter = mFireConfirmData.m_MonsterAwardList.begin();iter != NULL;iter = mFireConfirmData.m_MonsterAwardList.next(iter))
	{
		num =0;
		for (List<RateOfGood>::Iter * iter2 = rateOfGoods.begin();iter2 != NULL;iter2 = rateOfGoods.next(iter2))
		{
			if (iter2->mValue.resourcesType == iter->mValue.resourcestype ||iter2->mValue.resourcesType == 0)
			{
				if (iter2->mValue.subtype == iter->mValue.subtype || iter2->mValue.subtype ==0)
				{
					num += iter->mValue.num * (iter2->mValue.rate/10000);
				}
			}
		}
		iter->mValue.num += num;
	}

	for( List<Goods>::Iter * iter = mFireConfirmData.m_IntruderAwardList.begin();iter != NULL;iter = mFireConfirmData.m_IntruderAwardList.next(iter))
	{
		num =0;
		for (List<RateOfGood>::Iter * iter2 = rateOfGoods.begin();iter2 != NULL;iter2 = rateOfGoods.next(iter2))
		{
			if (iter2->mValue.resourcesType == iter->mValue.resourcestype ||iter2->mValue.resourcesType == 0)
			{
				if (iter2->mValue.subtype == iter->mValue.subtype || iter2->mValue.subtype ==0)
				{
					num += iter->mValue.num * (iter2->mValue.rate/10000);
				}
			}
		}
		iter->mValue.num += num;
	}
	//翻牌奖励和其他的不用加成
	// 	for(List<Card>::Iter * item1 = mFireConfirmData.m_CardsList.begin();item1 != NULL; item1 = mFireConfirmData.m_CardsList.next(item1))
	// 	{			
	// 		for( List<Goods>::Iter * iter = item1->mValue.card.begin();iter != NULL;iter = item1->mValue.card.next(iter))
	// 		{
	// 			num =0;
	// 			for (List<RateOfGood>::Iter * iter2 = rateOfGoods.begin();iter2 != NULL;iter2 = rateOfGoods.next(iter2))
	// 			{
	// 				if (iter2->mValue.resourcesType == iter->mValue.resourcestype ||iter2->mValue.resourcesType == 0)
	// 				{
	// 					if (iter2->mValue.subtype == iter->mValue.subtype || iter2->mValue.subtype ==0)
	// 					{
	// 						num += iter->mValue.num * (iter2->mValue.rate/10000);
	// 					}
	// 				}
	// 			}
	// 			iter->mValue.num += num;
	// 		}
	// 		break;
	// 	}

	goods.resourcestype =AWARD_BASE;
	goods.subtype =AWARD_BASE_GOLD;
	goods.num -= goldcost;
	if(goods.num != 0)
	{
		mFireConfirmData.m_CostList.insertTail(goods);
	}

}

void StageCalcManager::setChapterLock(const  ConnId& connId ,UInt32 key,UInt32 value)
{
	UInt32 getIt =0;
	UInt32 changeIt =0;
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	List<KeyValue> m_ChapterUnlockeded = player->getChapterUnlocked();

	for(List<KeyValue>::Iter * it = m_ChapterUnlockeded.begin();it != NULL; it = m_ChapterUnlockeded.next(it))
	{
		if (it->mValue.key == key)
		{
			getIt = 1;
			if (it->mValue.value != value)
			{
				it->mValue.value = value;
				changeIt = 1;
			}				
			break;
		}
	}	
	if (getIt ==0)
	{
		for(List<KeyValue>::Iter * it = m_ChapterUnlockeded.begin();it != NULL; it = m_ChapterUnlockeded.next(it))
		{
			if (it->mValue.key == 0)
			{
				it->mValue.key = key;
				it->mValue.value = value;
				changeIt = 1;				
				break;
			}
		}	
	}
	if (changeIt == 1)
	{
		player->setChapterUnlocked(m_ChapterUnlockeded);
		player->getPersistManager().setDirtyBit(CHAPTERUNLOCKEDTABIT);
	}

}



void testfunc()
{
	tryAll
	{
		LOG_WARN("error 111 1 ...........");
		UInt32 i =0;
		i = 5/i;

	}
	catchAll
	{
		LOG_WARN("error 111  ...........");
	}

	// // 	try
	// // 	{
	// // 	}
	// // 	catch (CMemoryException* e)
	// // 	{
	// // 		
	// // 	}
	// // 	catch (CFileException* e)
	// // 	{
	// // 	}
	// // 	catch (CException* e)
	// // 	{
	// // 	}
	// 
	// 	__try
	// 	{
	// 		PlayerDailyResetData * pb = 0;
	// 		// 		typeid(*pb);  // throws a bad_typeid exception
	// 		UInt32 i =0;
	// 
	// 		cout << "111" << endl;
	// 		int tmp = 3/i;
	// 		cout << " 222" << endl;
	// 	}
	// 	__except(EXCEPTION_EXECUTE_HANDLER)
	// 	{
	// 		cout << "An exception was caught in __except." << endl;
	// 		// 		std::cerr << "exception caught: " << e.what() << '\n';
	// 	}
}
// 
// template <class KeyType, class ValueType>
// //flag 0最小 9999 最大 
// UInt32 StageCalcManager::getKey(Map<KeyType,ValueType> values,UInt32 flag )
// {
// 
// 	UInt64 ret = 0 ;
// 	UInt32 count = 0;
// 	for (Map<KeyType,ValueType>::Iter * iter = values.begin();iter != NULL;iter = values.next(iter))
// 	{
// 		if (count == 0)
// 		{
// 			ret = iter->mKey;
// 		}		
// 		count ++;
// 
// 		if (flag == 0)
// 		{
// 			if (iter->mKey < ret)
// 			{
// 				ret = iter->mKey;
// 			}
// 		}
// 		else if (flag = 9999)
// 		{
// 			if (iter->mKey > ret)
// 			{
// 				ret = iter->mKey;
// 			}
// 		}		
// 		else if(count == flag)
// 		{
// 			return (UInt32)iter->mKey;
// 
// 		}
// 
//  	}
// 	return (UInt32)ret;
// 
// }

//flag 0最小 9999 最大 
UInt32 StageCalcManager::getMinKey(Map<UInt64, StageTemplate> values,UInt32 flag )
{

	UInt64 ret = 0 ;
	UInt32 count = 0;

	for (Map<UInt64, StageTemplate>::Iter * iter = values.begin();iter != NULL;iter = values.next(iter))
	{
		if (count == 0)
		{
			ret = iter->mKey;
		}		
		count ++;

		if (flag == 0)
		{
			if (iter->mKey < ret)
			{
				ret = iter->mKey;
			}
		}
		else if (flag = 9999)
		{
			if (iter->mKey > ret)
			{
				ret = iter->mKey;
			}
		}		
		else if(count == flag)
		{
			return (UInt32)iter->mKey;

		}
	}


	return (UInt32)ret;


}



void StageCalcManager::findSimilarPowerReq(Guid playerID,UInt32 times,UInt32 high,UInt32 low)
{
	PersistFindSimilarPowerReq req;
	req.playerGuid = playerID;
	req.times = times;
	req.high = high;
	req.low = low;
	PersistSystem::getSingleton().postThreadMsg(req, 0);
}


Map<UInt32,UInt32> *StageCalcManager::getResetStages(UInt64 playerID)
{
	Map<UInt32,UInt32> *resetStages = NULL;
	for (Map<UInt64,ResetStages*>::Iter * iter = gResetStages.begin();iter != NULL;iter = gResetStages.next(iter))
	{
		if (iter->mKey == playerID)
		{
			return &iter->mValue->resetStages;
		}
	}
	return resetStages;
}

void StageCalcManager::addResetStages(UInt64 playerID,UInt32 stageID,UInt32 count,bool needSave = true)
{
	UInt32 val = count;


	Map<UInt64,ResetStages*>::Iter* iter = gResetStages.find(playerID);
	if (iter != NULL)
	{
		Map<UInt32,UInt32>::Iter * it = iter->mValue->resetStages.find(stageID);	
		if (it == NULL)
		{
			iter->mValue->resetStages.insert(stageID,count);
		}
		else
		{
			it->mValue += count;	
			val = it->mValue;
		}
	}
	else
	{
		mResetStages.resetStages.insert(stageID,count);

		gResetStages.insert(playerID,&mResetStages);
	}
	

	if (needSave == true)
	{
		addNeedSaveResetStages(playerID,stageID,val);
	}
	
}

void StageCalcManager::addNeedSaveResetStages(UInt64 playerID,UInt32 stageID,UInt32 val)
{
	NeedSaveResetStages needSaveResetStages;
	needSaveResetStages.playerID = playerID;
	needSaveResetStages.stageID = stageID;
	needSaveResetStages.val = val;
	gNeedSaveResetStages.insertTail(needSaveResetStages);

}
List<NeedSaveResetStages>& StageCalcManager::getNeedSaveResetStages()
{
	return gNeedSaveResetStages;

}
void StageCalcManager::resetNeedSaveResetStages()
{
	gNeedSaveResetStages.clear();	
}


UInt32 StageCalcManager::getResetStageCount(UInt64 playerID,UInt32 stageID)
{
	UInt32 num = 0;

	Map<UInt64,ResetStages*>::Iter* iter = gResetStages.find(playerID);
	if (iter != NULL)
	{
		Map<UInt32,UInt32>::Iter * it = iter->mValue->resetStages.find(stageID);
		if (it != NULL)
		{
			return it->mValue;
		}
	}
	return num;
}

void StageCalcManager::resetAllResetStages()
{
	gResetStages.clear();
	GiftManager::getSingleton().updateSingleProperty(0,"10000","");//清理数据库里的十二宗宫单个购买次数
}

UInt32 StageCalcManager::resetHaoJiao(Guid playerID)
{

	Player* player = LogicSystem::getSingleton().getPlayerByGuid(playerID);

	PlayerDailyResetData dailyResetData= player->getPlayerDailyResetData();

	dailyResetData.m_nTwelvePalaceUnlockCount = GlobalVarManager::getSingleton().getTwelvePalace().resetlowerlimit;
	player->setPlayerDailyResetData(dailyResetData);
	player->getPersistManager().setDirtyBit(DAILYRESETBIT);
	return 0;

}

void StageCalcManager::onBattleStart(const  ConnId& connId,CGBattleStart & msg)
{

	TwelveBattleStartResp resp;
	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);

	msg.convertJsonToData(msg.jsonStr); 

	PlayerFireConfirmData mFireConfirmData =  player->GetFireConfirmData();

	if (mFireConfirmData.m_CopyID != msg.id)
	{
	}

	
	FireConfirmManager::getSingleton().SetCopyStart(player,msg.id);
	resp.id = msg.id;
	std::string jsonStr = resp.convertDataToJson();
	
	NetworkSystem::getSingleton().sender(connId,BATTLE_START_RESP,jsonStr);

}

bool StageCalcManager::checkStageClearance(Guid playerID,UInt32 stageId)
{
	PlayerChapterList chapterList;

	UInt32 chapterID = StageManager::getSingleton().getChapterID(stageId);
	Player* player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	chapterList = player->getChapterList();
	for (List<PlayerChapterData>::Iter * iter = chapterList.m_listChapterData.begin();iter != NULL;iter = chapterList.m_listChapterData.next(iter))
	{
		if (iter->mValue.m_nChapterId == chapterID)
		{
			//挑战次数变0
			for(List<StageData>::Iter* it = iter->mValue.m_listStageDatas.begin(); it != NULL;it = iter->mValue.m_listStageDatas.next(it))
			{
				if (it->mValue.m_nStageID == stageId)
				{
					if (it->mValue.m_nLastChallengTime >0)
					{
						return true;
					}
				}
				
			}
			break;						
		}
	}
	return false;
}