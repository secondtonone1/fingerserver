#include "RhymeManager.h"
#include "Player.h"
#include "CommonLib/Table.h"
#include "LogicSystem.h"
#include "FireConfirm/Gift.h"

using namespace Lynx;

RhymeManager::RhymeManager()
{
	m_pPlayer = NULL;
	

}


RhymeManager:: ~RhymeManager()
{

}

bool RhymeManager::initial(Player* player)
{
	m_pPlayer = player;
	m_pRhymeData = &player->mPlayerData.mRhymeData;
	

	return true;
}


void RhymeManager::release()
{
	m_pPlayer = NULL;
	
}

void RhymeManager::onEnhanceOnce()
{	
	UInt64 * rymePoint = RHYMELEVEL_TABLE().getIdByStepLv(m_pPlayer->mPlayerData.mRhymeData.m_RhymeStep, m_pPlayer->mPlayerData.mRhymeData.m_RhymeLv);
	
	if(!rymePoint)
	{
		GCRhymeEnhanceResp rhymeEnhanceResp;
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;

		Json::StyledWriter styledWriter;

		rhymeEnhanceResp.mPacketID = BOC_RHYME_ENHANCE_RESP;
		rhymeEnhanceResp.mRespJsonStr = styledWriter.write(root);

		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(rhymeEnhanceResp,connId);

		cout << rhymeEnhanceResp.mRespJsonStr;	

		return ;
	}

	UInt64 rhymeID = *rymePoint;
	
	RhymeLevelTemplate * rhymeLevelNext = RHYMELEVEL_TABLE().getNext(rhymeID);
	
	if(rhymeLevelNext == NULL)
	{
		//提示玩家已达到最高等级，发协议
		GCRhymeEnhanceResp rhymeEnhanceResp;
		Json::Value root;
		root["errorId"] = LynxErrno::RhymeLevelLimit;
		root["rhymeStep"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeStep;
		root["rhymeLv"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeLv;
		root["rhymeSoul"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeSoul;
		root["rhymeExp"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeExp;
		root["playerCoin"] = m_pPlayer->mPlayerData.mBaseData.m_nCoin;
		
		Json::StyledWriter styledWriter;
			
		rhymeEnhanceResp.mPacketID = BOC_RHYME_ENHANCE_RESP;
		rhymeEnhanceResp.mRespJsonStr = styledWriter.write(root);

		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(rhymeEnhanceResp,connId);

		cout << rhymeEnhanceResp.mRespJsonStr;	
		return;
	}
	else
	{
		//逻辑处理，判断玩家现有的金币和韵魂，以及经验是否满足升级条件
		RhymeLevelTemplate * rhymeLevelCur = RHYMELEVEL_TABLE().get(rhymeID);

		if(!rhymeLevelCur)
		{
			GCRhymeEnhanceResp rhymeEnhanceResp;
			Json::Value root;
			root["errorId"] = LynxErrno::TableElementNotExit;
			root["rhymeStep"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeStep;
			root["rhymeLv"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeLv;
			root["rhymeSoul"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeSoul;
			root["rhymeExp"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeExp;
			root["playerCoin"] = m_pPlayer->mPlayerData.mBaseData.m_nCoin;

			Json::FastWriter styledWriter;

			rhymeEnhanceResp.mPacketID = BOC_RHYME_ENHANCE_RESP;
			rhymeEnhanceResp.mRespJsonStr = styledWriter.write(root);

			cout << rhymeEnhanceResp.mRespJsonStr;
			const ConnId& connId = m_pPlayer->getConnId();

			NetworkSystem::getSingleton().sendMsg(rhymeEnhanceResp,connId);


			return;	
		}

		UInt32 &needCoin = rhymeLevelCur->mNextNeedCoin;
		UInt32 &needExp = rhymeLevelCur->mNextNeedExp;
		UInt32 &needSoul = rhymeLevelCur->mNextNeedSoul;
		
		UInt64 & curSoul = m_pPlayer->mPlayerData.mRhymeData.m_RhymeSoul;
		UInt64 & curCoin = m_pPlayer->mPlayerData.mBaseData.m_nCoin;
		UInt32 & curExp = m_pPlayer->mPlayerData.mRhymeData.m_RhymeExp;

		Vector<UInt32> critNumVec;
		if(curSoul < needSoul || curCoin < needCoin)
		{
			//提示玩家升级所需材料或金币不足
			GCRhymeEnhanceResp rhymeEnhanceResp;
			Json::Value root;
			root["errorId"] = LynxErrno::MaterialNotEnough;
			root["rhymeStep"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeStep;
			root["rhymeLv"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeLv;
			root["rhymeSoul"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeSoul;
			root["rhymeExp"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeExp;
			root["playerCoin"] = m_pPlayer->mPlayerData.mBaseData.m_nCoin;

			Json::FastWriter styledWriter;

			rhymeEnhanceResp.mPacketID = BOC_RHYME_ENHANCE_RESP;
			rhymeEnhanceResp.mRespJsonStr = styledWriter.write(root);
			cout << rhymeEnhanceResp.mRespJsonStr;
			const ConnId& connId = m_pPlayer->getConnId();

			NetworkSystem::getSingleton().sendMsg(rhymeEnhanceResp,connId);


			return;
		}
		else
		{
			

			//韵魂减少
			curSoul -= needSoul;
			//金币减少

			Goods goods;
			List<Goods> itemList;

			goods.resourcestype =AWARD_BASE;
			goods.subtype = AWARD_BASE_COIN;
			goods.num = 0 - needCoin;
			itemList.insertTail(goods);
			GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog35);

		
			UInt32 rate = rand()%100;
			
			if(rate < rhymeLevelCur->mRateEnhanceVec[0])
			{
				curExp += rhymeLevelCur->mGetExp;
				critNumVec.push_back(0);
			}
			else if(rate <  rhymeLevelCur->mRateEnhanceVec[1] + rhymeLevelCur->mRateEnhanceVec[0])
			{
				curExp += rhymeLevelCur->mGetExp * 2;
				critNumVec.push_back(1);
			}
			else
			{
				curExp += rhymeLevelCur->mGetExp * 3;
				critNumVec.push_back(2);
			}

		}

		//考虑韵文经验升级情况
		UInt32 expNeedTemp = rhymeLevelCur->mNextNeedExp;
		
		while(curExp >= expNeedTemp && rhymeLevelNext)
		{
			//新增需求，如果经验值满足升阶，在升级逻辑不进行进阶操作，要打断
			if(m_pPlayer->mPlayerData.mRhymeData.m_RhymeStep != rhymeLevelNext->mStep)
			{
				break;
			}

			//玩家韵文等级和阶数改变
			m_pPlayer->mPlayerData.mRhymeData.m_RhymeLv = rhymeLevelNext->mLevel;
			m_pPlayer->mPlayerData.mRhymeData.m_RhymeStep = rhymeLevelNext->mStep;
			//升级后经验值减少
			curExp -= expNeedTemp;

			//下次升级需要的经验值
			expNeedTemp = rhymeLevelNext->mNextNeedExp;
			rhymeLevelNext = RHYMELEVEL_TABLE().getNext(rhymeLevelNext->mRhymeID);
			if(!rhymeLevelNext)
			{
				break;
			}
		}
		
		m_pPlayer->getPersistManager().setDirtyBit(RHYMEDATABIT|BASEDATABIT);
		
		//更新成就进度
		UInt32 arg = m_pPlayer->mPlayerData.mRhymeData.m_RhymeStep*1000 + m_pPlayer->mPlayerData.mRhymeData.m_RhymeLv;
		m_pPlayer->getAchieveManager().updateAchieveData(RHYMEENHANCELV, arg);


		//发送信息给玩家

		GCRhymeEnhanceResp rhymeEnhanceResp;
		Json::Value root;
		root["errorId"] = LynxErrno::None;
		root["rhymeStep"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeStep;
		root["rhymeLv"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeLv;
		root["rhymeSoul"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeSoul;
		root["rhymeExp"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeExp;
		root["playerCoin"] = m_pPlayer->mPlayerData.mBaseData.m_nCoin;
		
		for(UInt32 i = 0; i < critNumVec.size(); i++)
		{
			root["crit"].append(critNumVec[i]);
		}
		

		Json::StyledWriter styledWriter;

		rhymeEnhanceResp.mPacketID = BOC_RHYME_ENHANCE_RESP;
		rhymeEnhanceResp.mRespJsonStr = styledWriter.write(root);
		cout << rhymeEnhanceResp.mRespJsonStr;
		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(rhymeEnhanceResp,connId);
		//更新每日任务
		m_pPlayer->getAchieveManager().updateDailyTaskData(DLYRHYMEENHANCECNT, critNumVec.size() );
		return;
	}

	
}

void RhymeManager::onEnhanceTen()
{
	UInt64* rymePoint = RHYMELEVEL_TABLE().getIdByStepLv(m_pPlayer->mPlayerData.mRhymeData.m_RhymeStep, m_pPlayer->mPlayerData.mRhymeData.m_RhymeLv);
	
	if(!rymePoint)
	{
		GCRhymeEnhanceResp rhymeEnhanceResp;
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;

		Json::StyledWriter styledWriter;

		rhymeEnhanceResp.mPacketID = BOC_RHYME_ENHANCE_RESP;
		rhymeEnhanceResp.mRespJsonStr = styledWriter.write(root);

		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(rhymeEnhanceResp,connId);

		cout << rhymeEnhanceResp.mRespJsonStr;	

		return ;
	}

	UInt64 rhymeID = *rymePoint;


	List<UInt32> listRandom = LogicSystem::getSingleton().getRandomList(10,100);
	
	RhymeLevelTemplate * rhymeLevelNext = RHYMELEVEL_TABLE().getNext(rhymeID);

	if(rhymeLevelNext == NULL)
	{
		//提示玩家已达到最高等级，发协议
		GCRhymeEnhanceResp rhymeEnhanceResp;
		Json::Value root;
		root["errorId"] = LynxErrno::RhymeLevelLimit;
		root["rhymeStep"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeStep;
		root["rhymeLv"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeLv;
		root["rhymeSoul"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeSoul;
		root["rhymeExp"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeExp;
		root["playerCoin"] = m_pPlayer->mPlayerData.mBaseData.m_nCoin;

		Json::FastWriter styledWriter;

		rhymeEnhanceResp.mPacketID = BOC_RHYME_ENHANCE_RESP;
		rhymeEnhanceResp.mRespJsonStr = styledWriter.write(root);

		cout << rhymeEnhanceResp.mRespJsonStr;
		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(rhymeEnhanceResp,connId);


		return;
	}
	else
	{
		//逻辑处理，判断玩家现有的金币和韵魂，以及经验是否满足升级条件
		RhymeLevelTemplate * rhymeLevelCur = RHYMELEVEL_TABLE().get(rhymeID);
		if(!rhymeLevelCur)
		{
			GCRhymeEnhanceResp rhymeEnhanceResp;
			Json::Value root;
			root["errorId"] = LynxErrno::TableElementNotExit;
			root["rhymeStep"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeStep;
			root["rhymeLv"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeLv;
			root["rhymeSoul"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeSoul;
			root["rhymeExp"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeExp;
			root["playerCoin"] = m_pPlayer->mPlayerData.mBaseData.m_nCoin;

			Json::FastWriter styledWriter;

			rhymeEnhanceResp.mPacketID = BOC_RHYME_ENHANCE_RESP;
			rhymeEnhanceResp.mRespJsonStr = styledWriter.write(root);

			cout << rhymeEnhanceResp.mRespJsonStr;
			const ConnId& connId = m_pPlayer->getConnId();

			NetworkSystem::getSingleton().sendMsg(rhymeEnhanceResp,connId);


			return;	
		}

		UInt32 &needCoin = rhymeLevelCur->mNextNeedCoin ;
		UInt32 &needExp = rhymeLevelCur->mNextNeedExp;
		UInt32 &needSoul = rhymeLevelCur->mNextNeedSoul;

		UInt64 & curSoul = m_pPlayer->mPlayerData.mRhymeData.m_RhymeSoul;
		UInt64 & curCoin = m_pPlayer->mPlayerData.mBaseData.m_nCoin;
		UInt32 & curExp = m_pPlayer->mPlayerData.mRhymeData.m_RhymeExp;

		Vector<UInt32> critNumVec;
		if( curCoin < needCoin)
		{
			//提示玩家升级所需材料或金币不足
			GCRhymeEnhanceResp rhymeEnhanceResp;
			Json::Value root;
			root["errorId"] = LynxErrno::MaterialNotEnough;
			root["rhymeStep"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeStep;
			root["rhymeLv"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeLv;
			root["rhymeSoul"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeSoul;
			root["rhymeExp"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeExp;
			root["playerCoin"] = m_pPlayer->mPlayerData.mBaseData.m_nCoin;

			Json::FastWriter styledWriter;
			rhymeEnhanceResp.mPacketID = BOC_RHYME_ENHANCE_RESP;
			rhymeEnhanceResp.mRespJsonStr = styledWriter.write(root);
			cout << rhymeEnhanceResp.mRespJsonStr;
			const ConnId& connId = m_pPlayer->getConnId();

			NetworkSystem::getSingleton().sendMsg(rhymeEnhanceResp,connId);


			return;
		}

		if(curSoul < needSoul * 10 || curCoin < needCoin * 10)
		{
			int times = curSoul/needSoul;
			int cointimes = curCoin/needCoin;
			if(cointimes < times)
			{
				times = cointimes;
			}

			if(times == 0)
			{
				//提示玩家升级所需材料或金币不足
				GCRhymeEnhanceResp rhymeEnhanceResp;
				Json::Value root;
				root["errorId"] = LynxErrno::MaterialNotEnough;
				root["rhymeStep"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeStep;
				root["rhymeLv"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeLv;
				root["rhymeSoul"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeSoul;
				root["rhymeExp"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeExp;
				root["playerCoin"] = m_pPlayer->mPlayerData.mBaseData.m_nCoin;

				Json::FastWriter styledWriter;
				rhymeEnhanceResp.mPacketID = BOC_RHYME_ENHANCE_RESP;
				rhymeEnhanceResp.mRespJsonStr = styledWriter.write(root);
				cout << rhymeEnhanceResp.mRespJsonStr;
				const ConnId& connId = m_pPlayer->getConnId();

				NetworkSystem::getSingleton().sendMsg(rhymeEnhanceResp,connId);

				return;
			}


			//韵魂减少
			curSoul -= needSoul*times;
			//金币减少
			Goods goods;
			List<Goods> itemList;

			goods.resourcestype =AWARD_BASE;
			goods.subtype = AWARD_BASE_COIN;
			goods.num = 0 - needCoin * 10;
			itemList.insertTail(goods);
			GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog35);
			
			List<UInt32>::Iter * rateIter = listRandom.begin();
			for(UInt32 i = 0; i < times; i++)
			{
				if(!rateIter)
				{
					break;
				}
				
				if(rateIter->mValue < rhymeLevelCur->mRateEnhanceVec[0])
				{
					curExp += rhymeLevelCur->mGetExp;
					critNumVec.push_back(0);
				}
				else if(rateIter->mValue < rhymeLevelCur->mRateEnhanceVec[0] + rhymeLevelCur->mRateEnhanceVec[1])
				{
					curExp += rhymeLevelCur->mGetExp*2;
					critNumVec.push_back(1);
				}
				else
				{
					curExp += rhymeLevelCur->mGetExp*3;
					critNumVec.push_back(2);
				}

				rateIter = listRandom.next(rateIter);

			}

		}
		else
		{
			//韵魂减少
			curSoul -= needSoul*10;
			//金币减少
			Goods goods;
			List<Goods> itemList;

			goods.resourcestype =AWARD_BASE;
			goods.subtype = AWARD_BASE_COIN;
			goods.num = 0 - needCoin*10;
			itemList.insertTail(goods);
			GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog7);


			for(List<UInt32>::Iter * rateIter = listRandom.begin(); rateIter != listRandom.end(); rateIter = listRandom.next(rateIter))
			{
				if(rateIter->mValue < rhymeLevelCur->mRateEnhanceVec[0])
				{
					curExp += rhymeLevelCur->mGetExp;
					critNumVec.push_back(0);
				}
				else if(rateIter->mValue < rhymeLevelCur->mRateEnhanceVec[0] + rhymeLevelCur->mRateEnhanceVec[1])
				{
					curExp += rhymeLevelCur->mGetExp*2;
					critNumVec.push_back(1);
				}
				else
				{
					curExp += rhymeLevelCur->mGetExp*3;
					critNumVec.push_back(2);
				}
			}



		}
		
		
		//考虑韵文经验升级情况
		UInt32 expNeedTemp = rhymeLevelCur->mNextNeedExp;
			
		while(curExp >= expNeedTemp)
		{
			
			//新增需求，如果经验值满足升阶，在升级逻辑不进行进阶操作，要打断
			if(m_pPlayer->mPlayerData.mRhymeData.m_RhymeStep != rhymeLevelNext->mStep)
			{
				break;
			}
			
			//玩家韵文等级和阶数改变
			m_pPlayer->mPlayerData.mRhymeData.m_RhymeLv = rhymeLevelNext->mLevel;
			m_pPlayer->mPlayerData.mRhymeData.m_RhymeStep = rhymeLevelNext->mStep;
			//升级后经验值减少
			curExp -= expNeedTemp;

			//下次升级需要的经验值
			expNeedTemp = rhymeLevelNext->mNextNeedExp;
			rhymeLevelNext = RHYMELEVEL_TABLE().getNext(rhymeLevelNext->mRhymeID);
			if(!rhymeLevelNext)
			{
				break;
			}
		}

		m_pPlayer->getPersistManager().setDirtyBit(RHYMEDATABIT|BASEDATABIT);
		//发送信息给玩家

		//更新成就进度
		UInt32 arg = m_pPlayer->mPlayerData.mRhymeData.m_RhymeStep*1000 + m_pPlayer->mPlayerData.mRhymeData.m_RhymeLv;
		m_pPlayer->getAchieveManager().updateAchieveData(RHYMEENHANCELV, arg);


		GCRhymeEnhanceResp rhymeEnhanceResp;
		Json::Value root;
		root["errorId"] = LynxErrno::None;
		root["rhymeStep"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeStep;
		root["rhymeLv"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeLv;
		root["rhymeSoul"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeSoul;
		root["rhymeExp"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeExp;
		root["playerCoin"] = m_pPlayer->mPlayerData.mBaseData.m_nCoin;

		for(UInt32 i = 0; i < critNumVec.size(); i++)
		{
			root["crit"].append(critNumVec[i]);
		}

		Json::StyledWriter styledWriter;


		rhymeEnhanceResp.mPacketID = BOC_RHYME_ENHANCE_RESP;
		rhymeEnhanceResp.mRespJsonStr = styledWriter.write(root);

		cout << rhymeEnhanceResp.mRespJsonStr;
		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(rhymeEnhanceResp,connId);
		//更新每日任务
		m_pPlayer->getAchieveManager().updateDailyTaskData(DLYRHYMEENHANCECNT, critNumVec.size() );
		return;

	}
}


void RhymeManager::rhymeStep()
{
	UInt64* rymePoint = RHYMELEVEL_TABLE().getIdByStepLv(m_pPlayer->mPlayerData.mRhymeData.m_RhymeStep, m_pPlayer->mPlayerData.mRhymeData.m_RhymeLv);

	if(!rymePoint)
	{
		GCRhymeStepResp rhymeStepResp;
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;

		Json::StyledWriter styledWriter;

		rhymeStepResp.mPacketID = BOC_RHYME_STEP_RESP;
		rhymeStepResp.mRespJsonStr = styledWriter.write(root);

		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(rhymeStepResp,connId);

		cout << rhymeStepResp.mRespJsonStr;	

		return ;
	}

	UInt64 rhymeID = *rymePoint;

	RhymeLevelTemplate * rhymeLevelNext = RHYMELEVEL_TABLE().getNext(rhymeID);

	if(rhymeLevelNext == NULL)
	{
		//提示玩家已达到最高等级，发协议
		GCRhymeStepResp rhymeStepResp;
		Json::Value root;
		root["errorId"] = LynxErrno::RhymeLevelLimit;
		
		Json::StyledWriter styledWriter;

		rhymeStepResp.mPacketID = BOC_RHYME_STEP_RESP;
		rhymeStepResp.mRespJsonStr = styledWriter.write(root);

		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(rhymeStepResp,connId);

		cout << rhymeStepResp.mRespJsonStr;	
		return;
	}
	else
	{
		if(m_pPlayer->mPlayerData.mRhymeData.m_RhymeStep == rhymeLevelNext->mStep)
		{
			//提示玩家参数不对
			GCRhymeStepResp rhymeStepResp;
			Json::Value root;
			root["errorId"] = LynxErrno::InvalidParameter;

			Json::StyledWriter styledWriter;

			rhymeStepResp.mPacketID = BOC_RHYME_STEP_RESP;
			rhymeStepResp.mRespJsonStr = styledWriter.write(root);
			cout << rhymeStepResp.mRespJsonStr;
			const ConnId& connId = m_pPlayer->getConnId();

			NetworkSystem::getSingleton().sendMsg(rhymeStepResp,connId);


			return;
		}

		RhymeLevelTemplate * rhymeLevelCur = RHYMELEVEL_TABLE().get(rhymeID);
		if (rhymeLevelCur == NULL)
		{
			LOG_WARN("rhymeLevelCur not found!!");
			return;
		}

		UInt32 &needCoin = rhymeLevelCur->mNextNeedCoin;
		UInt32 &needExp = rhymeLevelCur->mNextNeedExp;
		UInt32 &needSoul = rhymeLevelCur->mNextNeedSoul;

		UInt64 & curSoul = m_pPlayer->mPlayerData.mRhymeData.m_RhymeSoul;
		UInt64 & curCoin = m_pPlayer->mPlayerData.mBaseData.m_nCoin;
		UInt32 & curExp = m_pPlayer->mPlayerData.mRhymeData.m_RhymeExp;


		if(curSoul < needSoul || curCoin < needCoin)
		{
			//提示玩家升级所需材料或金币不足
			GCRhymeStepResp rhymeStepResp;
			Json::Value root;
			root["errorId"] = LynxErrno::MaterialNotEnough;
		
			Json::StyledWriter styledWriter;

			rhymeStepResp.mPacketID = BOC_RHYME_STEP_RESP;
			rhymeStepResp.mRespJsonStr = styledWriter.write(root);
			cout << rhymeStepResp.mRespJsonStr;
			const ConnId& connId = m_pPlayer->getConnId();

			NetworkSystem::getSingleton().sendMsg(rhymeStepResp,connId);


			return;
		}
		
		UInt32 expNeedTemp = rhymeLevelCur->mNextNeedExp;

		if(curExp < expNeedTemp)
		{
			//提示玩家升级所需材料或金币不足
			GCRhymeStepResp rhymeStepResp;
			Json::Value root;
			root["errorId"] = LynxErrno::StepExpNotEnough;

			Json::StyledWriter styledWriter;

			rhymeStepResp.mPacketID = BOC_RHYME_STEP_RESP;
			rhymeStepResp.mRespJsonStr = styledWriter.write(root);
			cout << rhymeStepResp.mRespJsonStr;
			const ConnId& connId = m_pPlayer->getConnId();

			NetworkSystem::getSingleton().sendMsg(rhymeStepResp,connId);


			return;
		}


        //韵魂减少
	    curSoul -= needSoul;
	    //金币减少

		Goods goods;
		List<Goods> itemList;

		goods.resourcestype =AWARD_BASE;
		goods.subtype = AWARD_BASE_COIN;
		goods.num = 0 - needCoin;
		itemList.insertTail(goods);
		GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog35);
		
		//考虑韵文进阶情况
		
		while(curExp >= expNeedTemp && rhymeLevelNext)
		{
			
			//玩家韵文等级和阶数改变
			m_pPlayer->mPlayerData.mRhymeData.m_RhymeLv = rhymeLevelNext->mLevel;
			m_pPlayer->mPlayerData.mRhymeData.m_RhymeStep = rhymeLevelNext->mStep;
			//升级后经验值减少
			curExp -= expNeedTemp;

			//下次升级需要的经验值
			expNeedTemp = rhymeLevelNext->mNextNeedExp;
			rhymeLevelNext = RHYMELEVEL_TABLE().getNext(rhymeLevelNext->mRhymeID);
		}

		m_pPlayer->mPlayerData.mRhymeData.m_RhymeAcupointStr.clear();

		m_pPlayer->mPlayerData.mRhymeData.m_RhymeAcupointList.clear();

		m_pPlayer->getPersistManager().setDirtyBit(RHYMEDATABIT|BASEDATABIT);

		//更新成就进度
		UInt32 arg = m_pPlayer->mPlayerData.mRhymeData.m_RhymeStep*1000 + m_pPlayer->mPlayerData.mRhymeData.m_RhymeLv;
		m_pPlayer->getAchieveManager().updateAchieveData(RHYMEENHANCELV, arg);

		//发送信息给玩家

		GCRhymeStepResp rhymeStepResp;
		Json::Value root;
		root["errorId"] = LynxErrno::None;
		root["rhymeStep"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeStep;
		root["rhymeLv"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeLv;
		root["rhymeSoul"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeSoul;
		root["rhymeExp"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeExp;
		root["playerCoin"] = m_pPlayer->mPlayerData.mBaseData.m_nCoin;


		Json::StyledWriter styledWriter;

		rhymeStepResp.mPacketID = BOC_RHYME_STEP_RESP;
		rhymeStepResp.mRespJsonStr = styledWriter.write(root);
		cout << rhymeStepResp.mRespJsonStr;
		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(rhymeStepResp,connId);

		return;
	}

	
}


bool RhymeManager::checkGrassTypeCount(UInt32 type, UInt32 count)
{
	bool res = false;
	
	switch(type)
	{
	    case SmallGrass:
			{
				
				res =  (m_pRhymeData->m_RhymeGrass >= count);
				
			}
		   break;

		case MidGrass:
			{

				res =  (m_pRhymeData->m_RhymeGrassMid >= count);
			}
			break;

		case LargeGrass:
			{

				 res =  (m_pRhymeData->m_RhymeGrassHigh >= count);
			}
			break;
		default:
			
			break;
	}

	return res;
}

void  RhymeManager::activeRhymeAcupoint(UInt64 acupointIndex,UInt32 step ,UInt32 level)
{
	const ConnId& connId = m_pPlayer->getConnId();


	if(m_pPlayer->mPlayerData.mRhymeData.m_RhymeStep != step || m_pPlayer->mPlayerData.mRhymeData.m_RhymeLv != level)
	{
		GCRhymeAcupointActiveResp resp;

		Json::Value root;

		root["errorId"] = LynxErrno::InvalidParameter;

		Json::StyledWriter writer;
		resp.mRespJsonStr = writer.write(root);

		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(resp,connId);

		cout << resp.mRespJsonStr;

		return;
	}
	
	RhymeAcupointTemplate * rhymeAcupointTemp = RHYMEACUPOINT_TABLE().get(acupointIndex);
	if (rhymeAcupointTemp == NULL)
	{
		GCRhymeAcupointActiveResp resp;

		Json::Value root;

		root["errorId"] = LynxErrno::ClienServerDataNotMatch;

		Json::StyledWriter writer;
		resp.mRespJsonStr = writer.write(root);

		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(resp,connId);

		LOG_WARN("rhymeAcupointTemp not found!!");
		return;
	}

	bool conditionRes = checkGrassTypeCount(rhymeAcupointTemp->mGrassType, rhymeAcupointTemp->mActiveNeedGrass);

	if(!conditionRes)
	{
		//提示玩家背包里的韵魂草不足

		GCRhymeAcupointActiveResp resp;

		Json::Value root;

		root["errorId"] = LynxErrno::MaterialNotEnough;

		Json::StyledWriter writer;
		resp.mRespJsonStr = writer.write(root);

		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(resp,connId);
		
		cout << resp.mRespJsonStr;
		return;
	}
	
		List<UInt64>::Iter* acupointIter = 
		m_pPlayer->mPlayerData.mRhymeData.m_RhymeAcupointList.find(acupointIndex,
			m_pPlayer->mPlayerData.mRhymeData.m_RhymeAcupointList.begin(),m_pPlayer->mPlayerData.mRhymeData.m_RhymeAcupointList.end());

		if(acupointIter)
		{
			//提示玩家已经激活的穴道
			GCRhymeAcupointActiveResp resp;

			Json::Value root;

			root["errorId"] = LynxErrno::AcupointActived;

			Json::StyledWriter writer;
			resp.mRespJsonStr = writer.write(root);

			cout << resp.mRespJsonStr;

			NetworkSystem::getSingleton().sendMsg(resp,connId);

			return;
		}

		delRhymeGrass(rhymeAcupointTemp->mGrassType, rhymeAcupointTemp->mActiveNeedGrass);
		
		std::stringstream mystream;
		mystream.clear();
		mystream.str("");

		mystream << acupointIndex;
		std::string acupointStr;
		mystream >> acupointStr;
	
		m_pPlayer->mPlayerData.mRhymeData.m_RhymeAcupointStr.append(acupointStr);

		m_pPlayer->mPlayerData.mRhymeData.m_RhymeAcupointStr.append(";");
		
		m_pPlayer->mPlayerData.mRhymeData.m_RhymeAcupointList.insertTail(acupointIndex);

	

		GCRhymeAcupointActiveResp resp;

		Json::Value root;

		root["errorId"] = LynxErrno::None;
		root["mintGrass"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeGrass;
		root["coin"] = m_pPlayer->getPlayerCoin();
		root["acupointId"] = acupointIndex;
		root["midGrass"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeGrassMid;
		root["largeGrass"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeGrassHigh;

		Json::StyledWriter writer;
		resp.mRespJsonStr = writer.write(root);

		
		NetworkSystem::getSingleton().sendMsg(resp,connId);

		m_pPlayer->getPersistManager().setDirtyBit(RHYMEDATABIT);

		cout << resp.mRespJsonStr<< endl;

		//cout << "end" << time(0);

		return;

	

}

void RhymeManager::activeSkillSet(UInt32 rhymeSkillIndex)
{
	m_pPlayer->mPlayerData.mRhymeData.m_RhymeSkillIndex = rhymeSkillIndex;
	GCRhymeSkillResp resp;
	resp.mPacketID = BOC_RHYME_SKILLSET_RESP;
	Json::FastWriter writer;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["equipIdx"] = rhymeSkillIndex;

	resp.mRespJsonStr = writer.write(root);

	const ConnId& connId = m_pPlayer->getConnId();

	NetworkSystem::getSingleton().sendMsg(resp,connId);

	m_pPlayer->getPersistManager().setDirtyBit(RHYMEDATABIT);

}

UInt32 RhymeManager::getRhymeSkillEquip(void)
{
	return m_pPlayer->mPlayerData.mRhymeData.m_RhymeSkillIndex;
}

std::string RhymeManager::convertRhymeDataToJson()
{
	Json::Value root;
	root["level"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeLv;
	root["step"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeStep;
	root["exp"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeExp;
	root["soul"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeSoul;
	root["mintGrass"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeGrass;
	root["equipId"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeSkillIndex;
	root["midGrass"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeGrassMid;
	root["largeGrass"] = m_pPlayer->mPlayerData.mRhymeData.m_RhymeGrassHigh;


	Json::Value acupointEle;
	List<UInt64> & acupointList = m_pPlayer->mPlayerData.mRhymeData.m_RhymeAcupointList;
	
	for(List<UInt64>::Iter * acupointIter = acupointList.begin(); acupointIter != NULL; 
		acupointIter = acupointList.next(acupointIter))
	{
		root["acupointList"].append(acupointIter->mValue);
	}

	Json::StyledWriter writer;
	std::string jsonStr = writer.write(root);
	//cout << jsonStr;
	return jsonStr;
	
}

void RhymeManager::addRhymeSoul(UInt32 count)
{
	m_pPlayer->mPlayerData.mRhymeData.m_RhymeSoul += count;
	m_pPlayer->getPersistManager().setDirtyBit(RHYMEDATABIT);
}

void RhymeManager::delRhymeSoul(UInt32 count)
{
	m_pPlayer->mPlayerData.mRhymeData.m_RhymeSoul -= count;
	m_pPlayer->getPersistManager().setDirtyBit(RHYMEDATABIT);
}

UInt32 RhymeManager::getRhymeSoul()
{
	return m_pPlayer->mPlayerData.mRhymeData.m_RhymeSoul;
}

UInt32 RhymeManager::getRhymeGrass(UInt32 grassType)
{
	switch(grassType)
	{
	case SmallGrass:
		{
		
			return m_pPlayer->mPlayerData.mRhymeData.m_RhymeGrass;
		}
		break;

	case MidGrass:
		{
			return m_pPlayer->mPlayerData.mRhymeData.m_RhymeGrassMid;
		}
		break;

	case LargeGrass:
		{
			return m_pPlayer->mPlayerData.mRhymeData.m_RhymeGrassHigh;
		}
		break;

	default:
		return 0;
		break;
	}
	

}

void RhymeManager::addRhymeGrass(UInt32 grassType, UInt32 count)
{
	switch(grassType)
	{
		case SmallGrass:
			{
				m_pPlayer->mPlayerData.mRhymeData.m_RhymeGrass += count;
			}
		break;

		case MidGrass:
			{
				m_pPlayer->mPlayerData.mRhymeData.m_RhymeGrassMid += count;
			}
			break;

		case LargeGrass:
			{
				m_pPlayer->mPlayerData.mRhymeData.m_RhymeGrassHigh += count;
			}
			break;

		default:
			break;
	}
	
		m_pPlayer->getPersistManager().setDirtyBit(RHYMEDATABIT);
}

bool RhymeManager::delRhymeGrass(UInt32 grassType, UInt32 count)
{
	bool res = false;

	switch(grassType)
	{
	case SmallGrass:
		{
			if(m_pRhymeData->m_RhymeGrass >= count)
			{
				m_pPlayer->mPlayerData.mRhymeData.m_RhymeGrass -= count;
				res = true;
			}
			else
			{
				res = false;
			}
			
			
		}
		break;

	case MidGrass:
		{
			if(m_pRhymeData->m_RhymeGrassMid >= count)
			{
				m_pPlayer->mPlayerData.mRhymeData.m_RhymeGrassMid -= count;
				res = true;
			}
			else
			{
				res = false;
			}
			
		}
		break;

	case LargeGrass:
		{
			if(m_pRhymeData->m_RhymeGrassHigh >= count)
			{
				m_pPlayer->mPlayerData.mRhymeData.m_RhymeGrassHigh -= count;
				res = true;
			}
			else
			{
				res = false;
			}
		}
		break;

	default:
		break;
	}
		m_pPlayer->getPersistManager().setDirtyBit(RHYMEDATABIT);
	return res;

}

/******************************************************************************************************/
;//2015.10.13








