#include "ServantManager.h"
#include "Player.h"
#include "LogicSystem.h"
#include "PersistSystem.h"
#include "FireConfirm/Gift.h"
using namespace Lynx;


ServantManager::ServantManager()
{
	m_pPlayer = NULL;

	m_bitVec.clear();
	m_bitVec.push_back(12);
	m_bitVec.push_back(SERVANT_MAIN1_OPEN);
	m_bitVec.push_back(SERVANT_MAIN2_OPEN);
	m_bitVec.push_back(SERVANT_MAIN3_OPEN);
	m_bitVec.push_back(SERVANT_MAIN4_OPEN);
	m_bitVec.push_back(SERVANT_MAIN5_OPEN);

	m_bitVec.push_back(SERVANT_ASSIST1_OPEN);
	m_bitVec.push_back(SERVANT_ASSIST2_OPEN);
	m_bitVec.push_back(SERVANT_ASSIST3_OPEN);
	m_bitVec.push_back(SERVANT_ASSIST4_OPEN);
	m_bitVec.push_back(SERVANT_ASSIST5_OPEN);
	m_bitVec.push_back(SERVANT_ASSIST6_OPEN);
	m_bitVec.push_back(SERVANT_ASSIST7_OPEN);

	m_rateVec.resize(5,0);

	m_attrPercentMap.clear();
	m_attrPercentMap.insert(MaxHP,0.333);
	m_attrPercentMap.insert(MaxMP,0.2);
	m_attrPercentMap.insert(MaxSP,0.5);
	m_attrPercentMap.insert(PA ,1);
	m_attrPercentMap.insert(MA,1);
	m_attrPercentMap.insert(6,0.8);
	m_attrPercentMap.insert(7,0.8);
}

ServantManager:: ~ServantManager()
{
	m_rateVec.clear();
	m_attrPercentMap.clear();
}


bool ServantManager::initial(Player* player)
{
	if (player == NULL)
	{
		LOG_WARN("Player pointer is NULL.");
		return false;
	}

	m_pPlayer = player;
	m_pServantBattleData = &m_pPlayer->mPlayerData.mServantData.servantBattleData;
	
	m_pServantList = &m_pPlayer->mPlayerData.mServantData.servantList;
	m_pTreasureList = &m_pPlayer->mPlayerData.mServantData.treasureList;
	m_pServantData = &m_pPlayer->mPlayerData.mServantData;

	for(List<ServantData>::Iter *  servantListIter = m_pServantList->begin(); servantListIter != NULL; 
		servantListIter = m_pServantList->next(servantListIter))
	{
		m_mapIdServant.insert(servantListIter->mValue.servantId, &servantListIter->mValue);
	}

	for(List<ServantTreasure>::Iter * treasureIter = m_pTreasureList->begin();  treasureIter != NULL;
		treasureIter = m_pTreasureList->next(treasureIter))
	{
		m_mapIdTreasure.insert(treasureIter->mValue.treasureId, &treasureIter->mValue);
	}

	m_nPlayerUid = m_pPlayer->getPlayerGuid();

	//calculateLuck();

	return true;
}

void ServantManager::release()
{
	m_pPlayer = NULL;
	m_pServantBattleData = NULL;
	m_pServantList = NULL;
	m_pTreasureList = NULL;
	m_pServantData = NULL;

}

bool ServantManager::isServantExist(UInt32 subTypeID)
{

	Map<UInt64 ,ServantData*>::Iter * findIter =  m_mapIdServant.find(subTypeID);

	//有该佣兵或者佣兵碎片
	if(findIter)
	{
		if (findIter->mValue->level)
		{
			return true;
		}
		else
		{
			return false;
		}
			
	}
	return false;
}
bool ServantManager::addServants(UInt32 subTypeID,UInt32 count)
{
	bool ret = false;//已结拥有佣兵
	Json::Value jsonValue;
	ServantData* servantData;
	List<ServantData>  servantList;

	for(UInt32 i = 0; i < count ; i++)
	{
       //先判断是否有该佣兵
		Map<UInt64 ,ServantData*>::Iter * findIter =  m_mapIdServant.find(subTypeID);
		if(findIter)
		{
			//如果存在，那么判断是否有该佣兵
			 if(findIter->mValue->level)
			 {
				 //存在该佣兵，那么将佣兵转化为佣兵碎片

				  //存在该佣兵，那么将佣兵转化为佣兵碎片
					ServantTemplate * servantTemp = SERVANT_TABLE().get(subTypeID);
					if(!servantTemp)
					{
						LOG_WARN("design table error, servant %llu is not exist",  subTypeID);
						assert(false);
				    }

					//记录增加碎片， 需求更改,主程规定
					//rtItemEle.addCount2	+= servantTemp->mPieceCount; 
				
			 }
			 else
			 {
				//如果有该佣兵碎片，但是没有佣兵，那么佣兵个数+1记录增加个数，需求更改，按照主程规定
				//rtItemEle.addCount1 += 1;
			 }

	    }
		else
		{
		   //没有该佣兵，且没有碎片，需求更改，按照主程规定
         // rtItemEle.addCount1 += 1;
		}

		servantData = addServant(subTypeID);

		if (servantData->pieceCount == 0)
		{
			servantList.insertTail(*servantData);
			ServantManager::getServantListJson(&servantList,jsonValue);

			if( LogicSystem::getSingleton().combinSendData(m_pPlayer->getPlayerGuid(),AWARD_SERVANT,jsonValue) ==false)	
			{
				
			}


		}
		else
		{

		}
	}

	//系统公告
	ServantTemplate * servantTemp = SERVANT_TABLE().get(subTypeID);
	if (servantTemp == NULL)
	{
		LOG_WARN("servantTemp not found!!");
		assert(false);
		return ret;
	}
	if(servantTemp->mQuality >= 4)
	{
		LogicSystem::getSingleton().sendSystemMsg(servantTemp->mQuality-3, m_pPlayer->getPlayerName(), subTypeID);
	}
	
	

 	return ret;
}

ServantData * ServantManager::getServantById(UInt64 servantId)
{
	Map<UInt64 ,ServantData*>::Iter * findIter =  m_mapIdServant.find(servantId);

	//有该佣兵或者佣兵碎片
	if(findIter)
	{		
		return findIter->mValue;
	}

	return NULL;
}


void ServantManager::servantSwitch(UInt64 servantId, UInt32 count)
{
	const ConnId& connId = m_pPlayer->getConnId();
	Map<UInt64 ,ServantData*>::Iter * findIter =  m_mapIdServant.find(servantId);
	if(!findIter)
	{
		GCServantSwitchResp switchresp;
		switchresp.mPacketID = BOC_SERVANTSWITCH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;
		Json::StyledWriter writer;
	
		switchresp.mRespJsonStr = 	writer.write(root);
		NetworkSystem::getSingleton().sendMsg(switchresp, connId);
		cout << switchresp.mRespJsonStr;
		return;
	}
	
	if(findIter->mValue->pieceCount < count)
	{
		GCServantSwitchResp switchresp;
		switchresp.mPacketID = BOC_SERVANTSWITCH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ServantPieceLess;
		Json::StyledWriter writer;

		switchresp.mRespJsonStr = 	writer.write(root);
		NetworkSystem::getSingleton().sendMsg(switchresp, connId);
			cout << switchresp.mRespJsonStr;
		return;
	}

	findIter->mValue->pieceCount -= count;

	ServantTemplate * servantTemp = SERVANT_TABLE().get(servantId);
	if (servantTemp == NULL)
	{
		LOG_WARN("servantTemp not found!!");
		return;
	}
	m_pPlayer->mPlayerData.mServantData.servantSwitch +=  (servantTemp->mServantSwitch *count);

	PersistUpdateServantNotify updateServant;
	updateServant.m_nPlayerUid = m_nPlayerUid;
	updateServant.m_servantData = *findIter->mValue;

	PersistSystem::getSingleton().postThreadMsg(updateServant, updateServant.m_nPlayerUid);

	m_pPlayer->getPersistManager().setDirtyBit(SERVANTFOODBIT);

	Json::Value root;
	GCServantSwitchResp switchResp;
	root["errorId"] = LynxErrno::None;
	root["switchcount"] = m_pPlayer->mPlayerData.mServantData.servantSwitch;
	root["servantid"] = servantId;
	root["piececount"] = findIter->mValue->pieceCount;

	Json::StyledWriter writer;
	switchResp.mRespJsonStr = writer.write(root);
	switchResp.mPacketID = BOC_SERVANTSWITCH_RESP;
	NetworkSystem::getSingleton().sendMsg(switchResp, connId);

		cout << switchResp.mRespJsonStr;

}


void ServantManager::servantSwitch(UInt64 servantId)
{
	ServantTemplate * servantTemp = SERVANT_TABLE().get(servantId);
	if (servantTemp == NULL)
	{
		GCServantSwitchResp switchresp;
		switchresp.mPacketID = BOC_SERVANTSWITCH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ClienServerDataNotMatch;
		Json::StyledWriter writer;

		switchresp.mRespJsonStr = 	writer.write(root);
		NetworkSystem::getSingleton().sendMsg(switchresp,  m_pPlayer->getConnId());
		LOG_WARN("servantTemp not found!!");
		return;
	}

	const ConnId& connId = m_pPlayer->getConnId();
	Map<UInt64 ,ServantData*>::Iter * findIter =  m_mapIdServant.find(servantId);
	if(!findIter)
	{
		GCServantSwitchResp switchresp;
		switchresp.mPacketID = BOC_SERVANTSWITCH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;
		Json::StyledWriter writer;
	
		switchresp.mRespJsonStr = 	writer.write(root);
		NetworkSystem::getSingleton().sendMsg(switchresp, connId);
		cout << switchresp.mRespJsonStr;
		return;
	}
	
	if(findIter->mValue->star < servantTemp->mMaxStar)
	{
		GCServantSwitchResp switchresp;
		switchresp.mPacketID = BOC_SERVANTSWITCH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ServantStarLimit;
		Json::StyledWriter writer;

		switchresp.mRespJsonStr = 	writer.write(root);
		NetworkSystem::getSingleton().sendMsg(switchresp, connId);
			cout << switchresp.mRespJsonStr;
		return;
	}

	

	int count = findIter->mValue->pieceCount;
	m_pPlayer->mPlayerData.mServantData.servantSwitch +=  (servantTemp->mServantSwitch *count);
	findIter->mValue->pieceCount = 0;

	PersistUpdateServantNotify updateServant;
	updateServant.m_nPlayerUid = m_nPlayerUid;
	updateServant.m_servantData = *findIter->mValue;

	PersistSystem::getSingleton().postThreadMsg(updateServant, updateServant.m_nPlayerUid);

	m_pPlayer->getPersistManager().setDirtyBit(SERVANTFOODBIT);

	Json::Value root;
	GCServantSwitchResp switchResp;
	root["errorId"] = LynxErrno::None;
	root["switchcount"] = m_pPlayer->mPlayerData.mServantData.servantSwitch;
	root["servantid"] = servantId;
	root["piececount"] = findIter->mValue->pieceCount;

	Json::StyledWriter writer;
	switchResp.mRespJsonStr = writer.write(root);
	switchResp.mPacketID = BOC_SERVANTSWITCH_RESP;
	NetworkSystem::getSingleton().sendMsg(switchResp, connId);

		cout << switchResp.mRespJsonStr;

}

ServantData* ServantManager::addServant(UInt64 servantId)
{
	 ServantTemplate * servantTemp = SERVANT_TABLE().get(servantId);
		   if(!servantTemp)
		   {
			   LOG_WARN("design table error, servant %llu is not exist",  servantId);
			   assert(false);
		   }

	Map<UInt64 ,ServantData*>::Iter * findIter =  m_mapIdServant.find(servantId);
	
	//有该佣兵或者佣兵碎片
	if(findIter)
	{		
	   if(findIter->mValue->level)
	   {
	       //存在该佣兵，那么将佣兵转化为佣兵碎片
		   ServantTemplate * servantTemp = SERVANT_TABLE().get(servantId);
		   if(!servantTemp)
		   {
			   LOG_WARN("design table error, servant %llu is not exist",  servantId);
			   assert(false);
		   }
		   findIter->mValue->pieceCount += servantTemp->mPieceCount;

	   }
	   else
	   {
		  //不存在这个佣兵，但是之前获取过他的碎片，所以这次获得佣兵后将等级和星级更新为1，层级更新为0
		   findIter->mValue->level =1;
		   findIter->mValue->star = 1;
		   findIter->mValue->floor = 0;
		   findIter->mValue->lvexp = 0;


		   ServantTemplate * servantTemp = SERVANT_TABLE().get(servantId);
		   if(!servantTemp)
		   {
			   LOG_WARN("design table error, servant %llu is not exist",  servantId);
			   assert(false);
		   }

	
		  m_pPlayer->getAchieveManager().updateAchieveData(GETSERVANT,1);

		  if(servantTemp->mQuality==4)
		  {
			  m_pPlayer->getAchieveManager().updateAchieveData(OWNPURPULESERVANT,1);
		  }
	   }

	   //通知存盘

	   PersistUpdateServantNotify updateServant;
	   updateServant.m_nPlayerUid = m_nPlayerUid;
	   updateServant.m_servantData = *findIter->mValue;

	   PersistSystem::getSingleton().postThreadMsg(updateServant, updateServant.m_nPlayerUid);

	   return findIter->mValue;
	}
	else
	{

		//不存在这个佣兵，加入佣兵列表，插入map，并且等级和星级更新为1，层级为0
		ServantData servantData;
		servantData.servantUid = LogicSystem::getSingleton().generateItemGuid();
		servantData.servantId = servantId;
		servantData.pieceCount = 0;
		servantData.level = 1;
		servantData.star = 1;
		servantData.floor = 0;
		servantData.lvexp = 0;

		for(UInt32 i = 0; i < 4; i++)
		{
			servantData.equipTreasures.insertTail((UInt64)0);
		}
		

		List<ServantData>::Iter * insertIter= m_pServantList->insertTail(servantData);
		m_mapIdServant.insert(servantId, &insertIter->mValue);
		m_pPlayer->getAchieveManager().updateAchieveData(GETSERVANT,1);

		 ServantTemplate * servantTemp = SERVANT_TABLE().get(servantId);

		 if(!servantTemp)
		 {
			 LOG_WARN("design table error, servant %llu is not exist",  servantId);
			 assert(false);
		 }

		if(servantTemp->mQuality==4)
		{
			m_pPlayer->getAchieveManager().updateAchieveData(OWNPURPULESERVANT,1);
		}

		//通知存盘

		PersistAddServantNotify addServantNotify;
		addServantNotify.m_nPlayerUid = m_nPlayerUid;
		addServantNotify.m_servantData = servantData;

		PersistSystem::getSingleton().postThreadMsg(addServantNotify, addServantNotify.m_nPlayerUid);

		return &insertIter->mValue;
		
	}

	
}

bool ServantManager::getServantDataJson(UInt32 subTypeID,Json::Value &jsonValue)
{
	bool ret = false;//已结拥有佣兵
	List<ServantData>  servantList;

	Map<UInt64 ,ServantData*>::Iter * findIter =  m_mapIdServant.find(subTypeID);
	servantList.insertTail(*findIter->mValue);
	ServantManager::getServantListJson(&servantList,jsonValue);

	return ret;
}

void ServantManager::infolock(UInt64 servantid, UInt32 index)
{
	const ConnId& connId = m_pPlayer->getConnId();
	ServantTemplate * servantTemp = SERVANT_TABLE().get(servantid);
	if(!servantTemp)
	{
		LOG_WARN("design table error, servant %llu is not exist",  servantid);
		GCServantInfoLockResp servantInfoResp;
		servantInfoResp.mPacketID = BOC_SERVANT_INFOLOCK_RESP;

		Json::Value root;
		
		root["errorId"] = LynxErrno::TableElementNotExit;
	

		Json::StyledWriter writer;
		servantInfoResp.mRespJsonStr = writer.write(root);
		
		NetworkSystem::getSingleton().sendMsg(servantInfoResp, connId);

	
		return;
	}

	Map<UInt64 ,ServantData*>::Iter * findIter =  m_mapIdServant.find(servantid);
	if(!findIter)
	{
		LOG_WARN("design table error, servant %llu is not exist",  servantid);
		GCServantInfoLockResp servantInfoResp;
		servantInfoResp.mPacketID = BOC_SERVANT_INFOLOCK_RESP;

		Json::Value root;

		root["errorId"] = LynxErrno::ServantNotExit;


		Json::StyledWriter writer;
		servantInfoResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(servantInfoResp, connId);

		return;
	}

	UInt32 dirtyBit = 0;
	UInt32 curBit = findIter->mValue->infolock;
	if(index == 0)
	{
		dirtyBit = FIRSTLOCK;
	}

	if(index == 1)
	{
		dirtyBit = SECONDLOCK;
	}

	if(index == 2)
	{
		dirtyBit = THIRDLOCK;
	}

	findIter->mValue->infolock =  curBit | dirtyBit;

	GCServantInfoLockResp servantInfoResp;
	servantInfoResp.mPacketID = BOC_SERVANT_INFOLOCK_RESP;

	Json::Value root;

	root["errorId"] = LynxErrno::None;
	root["infolock"] = findIter->mValue->infolock;
	root["index"] = index;
	root["servantid"] = servantid;


	Json::StyledWriter writer;
	servantInfoResp.mRespJsonStr = writer.write(root);

	NetworkSystem::getSingleton().sendMsg(servantInfoResp, connId);


}




UInt32 ServantManager::getServantPieceCount(UInt64 servantId)
{
	Map<UInt64 ,ServantData*>::Iter * findIter =  m_mapIdServant.find(servantId);
	if(findIter)
	{
		return findIter->mValue->pieceCount;
	}
	return 0;
}

ServantData* ServantManager::addServantPiece(UInt64 servantId,UInt32 pieceCount)
{
	//存在该佣兵或者碎片，
		ServantTemplate * servantTemp = SERVANT_TABLE().get(servantId);

		if(!servantTemp)
		{
			LOG_WARN("design table error, servant %llu is not exist",  servantId);
			assert(false);
		}


	Map<UInt64 ,ServantData*>::Iter * findIter =  m_mapIdServant.find(servantId);

	//有该佣兵或者佣兵碎片
	if(findIter)
	{		
		
		//存在该佣兵或者碎片，
		ServantTemplate * servantTemp = SERVANT_TABLE().get(servantId);

		if(!servantTemp)
		{
			LOG_WARN("design table error, servant %llu is not exist",  servantId);
			assert(false);
		}

		findIter->mValue->pieceCount += pieceCount;
			
		//通知存盘

		PersistUpdateServantNotify updateServant;
		updateServant.m_nPlayerUid = m_nPlayerUid;
		updateServant.m_servantData = *findIter->mValue;

		PersistSystem::getSingleton().postThreadMsg(updateServant, updateServant.m_nPlayerUid);

		

		return findIter->mValue;
	}
	else
	{

		//不存在这个佣兵和佣兵碎片，加入佣兵列表，插入map，插入碎片数量，并且等级和星级都为0
		ServantData servantData;
		servantData.servantUid = LogicSystem::getSingleton().generateItemGuid();
		servantData.servantId = servantId;
		servantData.pieceCount = pieceCount;
		servantData.level = 0;
		servantData.star = 0;
		servantData.floor = 0;
		servantData.lvexp = 0;

		for(UInt32 i = 0; i < 4; i++)
		{
			servantData.equipTreasures.insertTail((UInt64)0);
		}

		List<ServantData>::Iter * insertIter= m_pServantList->insertTail(servantData);
		m_mapIdServant.insert(servantId, &insertIter->mValue);

		//通知存盘

		PersistAddServantNotify addServantNotify;
		addServantNotify.m_nPlayerUid = m_nPlayerUid;
		addServantNotify.m_servantData = servantData;

		PersistSystem::getSingleton().postThreadMsg(addServantNotify, addServantNotify.m_nPlayerUid);

		return &insertIter->mValue;

	}
}

ServantData * ServantManager::GMsetServantPiece(UInt64 servantId, UInt32 pieceCount)
{
	Map<UInt64 ,ServantData*>::Iter * findIter =  m_mapIdServant.find(servantId);

	//有该佣兵或者佣兵碎片
	if(findIter)
	{		

		//存在该佣兵或者碎片，
		ServantTemplate * servantTemp = SERVANT_TABLE().get(servantId);
		if(!servantTemp)
		{
			LOG_WARN("design table error, servant %llu is not exist",  servantId);
			assert(false);
		}

		findIter->mValue->pieceCount = pieceCount;

		//通知存盘

		PersistUpdateServantNotify updateServant;
		updateServant.m_nPlayerUid = m_nPlayerUid;
		updateServant.m_servantData = *findIter->mValue;

		PersistSystem::getSingleton().postThreadMsg(updateServant, updateServant.m_nPlayerUid);



		return findIter->mValue;
	}
	else
	{

		//不存在这个佣兵和佣兵碎片，加入佣兵列表，插入map，插入碎片数量，并且等级和星级都为0
		ServantData servantData;
		servantData.servantUid = LogicSystem::getSingleton().generateItemGuid();
		servantData.servantId = servantId;
		servantData.pieceCount = pieceCount;
		servantData.level = 0;
		servantData.star = 0;
		servantData.floor = 0;
		servantData.lvexp = 0;

		for(UInt32 i = 0; i < 4; i++)
		{
			servantData.equipTreasures.insertTail((UInt64)0);
		}

		List<ServantData>::Iter * insertIter= m_pServantList->insertTail(servantData);
		m_mapIdServant.insert(servantId, &insertIter->mValue);

		//通知存盘

		PersistAddServantNotify addServantNotify;
		addServantNotify.m_nPlayerUid = m_nPlayerUid;
		addServantNotify.m_servantData = servantData;

		PersistSystem::getSingleton().postThreadMsg(addServantNotify, addServantNotify.m_nPlayerUid);

		return &insertIter->mValue;

	}
}


ServantData* ServantManager::combineServant(UInt64 servantId)
{

	ServantTemplate * servantTemp = SERVANT_TABLE().get(servantId);

	if(!servantTemp)
	{
		LOG_WARN("design table error, servant %llu is not exist",  servantId);
		assert(false);
	}

	const ConnId& connId = m_pPlayer->getConnId();
	
	Map<UInt64 ,ServantData*>::Iter * findIter =  m_mapIdServant.find(servantId);

	//有该佣兵或者佣兵碎片
	if(findIter)
	{
		if(findIter->mValue->level == 0 || findIter->mValue->star == 0)
		{
			ServantStarTemplate * servantStarTemp = SERVANTSTAR_TABLE().reverseGet(servantId,0);
			if(servantStarTemp)
			{
				if(findIter->mValue->pieceCount < servantStarTemp->mCount)
				{
                   //提示玩家碎片不足
					Json::Value root;
					root["errorId"] = LynxErrno::ServantPieceLess;
					Json::StyledWriter writer;
					
                   GCServantCombineResp combineResp;
				   combineResp.mPacketID = BOC_SERVANT_COMBINE_RESP;
				   combineResp.mRespJsonStr = writer.write(root);

				   cout << combineResp.mRespJsonStr;

				   NetworkSystem::getSingleton().sendMsg(combineResp,connId);
				   return NULL;
				}
				UInt64 coin = m_pPlayer->getPlayerCoin();
				if(coin  < servantStarTemp->mCostCoin)
				{
						 //提示玩家铜钱不足
					Json::Value root;
					root["errorId"] = LynxErrno::CoinNotEnough;
					Json::StyledWriter writer;
					
                   GCServantCombineResp combineResp;
				   combineResp.mPacketID = BOC_SERVANT_COMBINE_RESP;
				   combineResp.mRespJsonStr = writer.write(root);

				   cout << combineResp.mRespJsonStr;

				   NetworkSystem::getSingleton().sendMsg(combineResp,connId);
				   return NULL;
				}


				//等级和星级变为1，减少碎片数量
				findIter->mValue->level = 1;
				findIter->mValue->star = 1;
				findIter->mValue->pieceCount -= servantStarTemp->mCount;
				coin -= servantStarTemp->mCostCoin;

				Goods goods;
				List<Goods> itemList;

				goods.resourcestype =AWARD_BASE;
				goods.subtype = AWARD_BASE_COIN;
				goods.num = 0 - servantStarTemp->mCostCoin;
				itemList.insertTail(goods);
				GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog20);



				PersistUpdateServantNotify updateServant;
				updateServant.m_nPlayerUid = m_nPlayerUid;
				updateServant.m_servantData = *findIter->mValue;

				PersistSystem::getSingleton().postThreadMsg(updateServant, updateServant.m_nPlayerUid);
				m_pPlayer->getAchieveManager().updateAchieveData(GETSERVANT,1);
				ServantTemplate * servantTemp = SERVANT_TABLE().get(servantId);

				if(!servantTemp)
				{
					LOG_WARN("design table error, servant %llu is not exist",  servantId);
					assert(false);
				}

				if(servantTemp->mQuality==4)
				{
					m_pPlayer->getAchieveManager().updateAchieveData(OWNPURPULESERVANT,1);
				}

				Json::Value root;
				root["errorId"] = LynxErrno::None;
				root["servantId"] = servantId;
				root["star"] = 1;
				root["level"] = 1;
				root["pieceCount"] = findIter->mValue->pieceCount;
				root["lvexp"] = 0;
				root["coin"] = coin;
				root["combineflag"] = 1;
				Json::StyledWriter writer;

				GCServantCombineResp combineResp;
				combineResp.mPacketID = BOC_SERVANT_COMBINE_RESP;
				combineResp.mRespJsonStr = writer.write(root);

				cout << combineResp.mRespJsonStr;

				if( LogicSystem::getSingleton().combinSendData(m_pPlayer->getPlayerGuid(),AWARD_GEM,root) ==false)	
				{
					NetworkSystem::getSingleton().sendMsg(combineResp,connId);
				}
				
				//系统提示
				//系统公告
				
				
				if(servantTemp->mQuality >= 4)
				{
					LogicSystem::getSingleton().sendSystemMsg(servantTemp->mQuality-3, m_pPlayer->getPlayerName(), servantId);
				}


				return findIter->mValue;

			}
			else
			{
				//提示玩家

				Json::Value root;
				root["errorId"] = LynxErrno::InvalidParameter;
				Json::StyledWriter writer;

				GCServantCombineResp combineResp;
				combineResp.mPacketID = BOC_SERVANT_COMBINE_RESP;
				combineResp.mRespJsonStr = writer.write(root);

				cout << combineResp.mRespJsonStr;

				NetworkSystem::getSingleton().sendMsg(combineResp,connId);

				return NULL;
			}

		}
		else
		{
			//提示玩家该佣兵已经存在，不需要合成

			Json::Value root;
			root["errorId"] = LynxErrno::ServantExist;
			Json::StyledWriter writer;

			GCServantCombineResp combineResp;
			combineResp.mPacketID = BOC_SERVANT_COMBINE_RESP;
			combineResp.mRespJsonStr = writer.write(root);

			cout << combineResp.mRespJsonStr;

			
			NetworkSystem::getSingleton().sendMsg(combineResp,connId);


			return NULL;
		}
	}
	else
	{
		//提示玩家碎片不足

		Json::Value root;
		root["errorId"] = LynxErrno::ServantPieceLess;
		Json::StyledWriter writer;

		GCServantCombineResp combineResp;
		combineResp.mPacketID = BOC_SERVANT_COMBINE_RESP;
		combineResp.mRespJsonStr = writer.write(root);

		cout << combineResp.mRespJsonStr;

		NetworkSystem::getSingleton().sendMsg(combineResp,connId);

		return NULL;
	}
	
	
	
}


ServantData* ServantManager::servantStarUp(UInt64 servantId)
{
	const ConnId& connId = m_pPlayer->getConnId();

	Map<UInt64 ,ServantData*>::Iter * findIter =  m_mapIdServant.find(servantId);

	if(findIter)
	{
		if(findIter->mValue->star == 0 || findIter ->mValue->level == 0)
		{
			//提示玩家该佣兵不存在

			Json::Value root;
			root["errorId"] = LynxErrno::ServantNotExit;

			Json::StyledWriter writer;
			
			GCServantCombineResp starupResp;
			starupResp.mPacketID = BOC_SERVANT_COMBINE_RESP;
			starupResp.mRespJsonStr = writer.write(root);

			cout << starupResp.mRespJsonStr;
			NetworkSystem::getSingleton().sendMsg(starupResp,connId);

			return NULL;
		}

		ServantStarTemplate * servantStarTemp = SERVANTSTAR_TABLE().reverseGet(servantId,findIter->mValue->star);
		
		if(!servantStarTemp)
		{
			//提示玩家参数错误

			Json::Value root;
			root["errorId"] = LynxErrno::InvalidParameter;

			Json::StyledWriter writer;

			GCServantCombineResp starupResp;
			starupResp.mPacketID = BOC_SERVANT_COMBINE_RESP;
			starupResp.mRespJsonStr = writer.write(root);

			cout << starupResp.mRespJsonStr;
			NetworkSystem::getSingleton().sendMsg(starupResp,connId);

			return NULL;
		}

		if(!servantStarTemp->mCount)
		{
			//提示玩家星级达到满级
			Json::Value root;
			root["errorId"] = LynxErrno::ServantStarLimit;

			Json::StyledWriter writer;

			GCServantCombineResp starupResp;
			starupResp.mPacketID = BOC_SERVANT_COMBINE_RESP;
			starupResp.mRespJsonStr = writer.write(root);

			cout << starupResp.mRespJsonStr;
			NetworkSystem::getSingleton().sendMsg(starupResp,connId);

			return NULL;
		}

		if(findIter->mValue->pieceCount < servantStarTemp->mCount)
		{
			//提示玩家碎片数量不足

			Json::Value root;
			root["errorId"] = LynxErrno::ServantPieceLess;

			Json::StyledWriter writer;

			GCServantCombineResp starupResp;
			starupResp.mPacketID = BOC_SERVANT_COMBINE_RESP;
			starupResp.mRespJsonStr = writer.write(root);

			cout << starupResp.mRespJsonStr;
			NetworkSystem::getSingleton().sendMsg(starupResp,connId);

			return NULL;
		}

		UInt64 playerCoin = m_pPlayer->getPlayerCoin();
		if(playerCoin < servantStarTemp->mCostCoin)
		{
			//提示玩家铜钱数量不足

			Json::Value root;
			root["errorId"] = LynxErrno::CoinNotEnough;

			Json::StyledWriter writer;

			GCServantCombineResp starupResp;
			starupResp.mPacketID = BOC_SERVANT_COMBINE_RESP;
			starupResp.mRespJsonStr = writer.write(root);

			cout << starupResp.mRespJsonStr;
			NetworkSystem::getSingleton().sendMsg(starupResp,connId);

			return NULL;
		}

		findIter->mValue->pieceCount -= servantStarTemp->mCount;
		findIter->mValue->star++;
		playerCoin -= servantStarTemp->mCostCoin;

		Goods goods;
		List<Goods> itemList;

		goods.resourcestype =AWARD_BASE;
		goods.subtype = AWARD_BASE_COIN;
		goods.num = 0 - servantStarTemp->mCostCoin;
		itemList.insertTail(goods);
		GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog20);

		
		PersistUpdateServantNotify updateServant;
		updateServant.m_nPlayerUid = m_nPlayerUid;
		updateServant.m_servantData = *findIter->mValue;

		PersistSystem::getSingleton().postThreadMsg(updateServant, updateServant.m_nPlayerUid);

		if(findIter->mValue->star == 4)
		{
			m_pPlayer->getAchieveManager().updateAchieveData(OWN4STARSERVANT,1);
		}

		Json::Value root;
		root["errorId"] = LynxErrno::None;
		root["servantId"] = servantId;
		root["star"] = findIter->mValue->star;
		root["coin"] = playerCoin;
		
		root["pieceCount"] = findIter->mValue->pieceCount;
		Json::StyledWriter writer;

		GCServantCombineResp combineResp;
		combineResp.mPacketID = BOC_SERVANT_COMBINE_RESP;
		combineResp.mRespJsonStr = writer.write(root);

		cout << combineResp.mRespJsonStr;

		NetworkSystem::getSingleton().sendMsg(combineResp,connId);

		return findIter->mValue;

	}
	else
	{
		//提示玩家参数错误
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;

		Json::StyledWriter writer;

		GCServantCombineResp starupResp;
		starupResp.mPacketID = BOC_SERVANT_COMBINE_RESP;
		starupResp.mRespJsonStr = writer.write(root);

		cout << starupResp.mRespJsonStr;
		NetworkSystem::getSingleton().sendMsg(starupResp,connId);

		return NULL;
	}
}

void ServantManager::servantFloorUp(UInt64 servantId)
{
	const ConnId& connId = m_pPlayer->getConnId();

	Map<UInt64 ,ServantData*>::Iter * findIter =  m_mapIdServant.find(servantId);

	if(findIter)
	{
		
		ServantFloorTemplate * servantFloorNext = SERVANTFLOOR_TABLE().reverseGet(findIter->mValue->servantId,findIter->mValue->floor + 1);
		if(!servantFloorNext)
		{
			//提示玩家满级层数
			GCServantFloorUpResp floorUpResp;
			floorUpResp.mPacketID = BOC_SERVANT_FLOORUP_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::ServantFloorLimit;
			Json::StyledWriter writer;

			floorUpResp.mRespJsonStr = writer.write(root);
			NetworkSystem::getSingleton().sendMsg(floorUpResp, connId);

			cout << floorUpResp.mRespJsonStr;
			
			return ;
		}

		ServantFloorTemplate * servantFloorTemp =SERVANTFLOOR_TABLE().reverseGet(findIter->mValue->servantId,findIter->mValue->floor);

		UInt32 index = 0;
		for(List<UInt64>::Iter * treasureIter = findIter->mValue->equipTreasures.begin(); treasureIter != NULL;
			treasureIter = findIter->mValue->equipTreasures.next(treasureIter) )
		{
			if(index == 0)
			{
				if(treasureIter->mValue !=  servantFloorTemp->mSlot1)
				{
					//提示玩家参数错误
					GCServantFloorUpResp floorUpResp;
					floorUpResp.mPacketID = BOC_SERVANT_FLOORUP_RESP;
					Json::Value root;
					root["errorId"] = LynxErrno::InvalidParameter;
					Json::StyledWriter writer;

					floorUpResp.mRespJsonStr = writer.write(root);
					NetworkSystem::getSingleton().sendMsg(floorUpResp, connId);

						cout << floorUpResp.mRespJsonStr;
					return ;
				}
			}

			if(index == 1)
			{
				if(treasureIter->mValue !=  servantFloorTemp->mSlot2)
				{
					//提示玩家参数错误

					GCServantFloorUpResp floorUpResp;
					floorUpResp.mPacketID = BOC_SERVANT_FLOORUP_RESP;
					Json::Value root;
					root["errorId"] = LynxErrno::InvalidParameter;
					Json::StyledWriter writer;

					floorUpResp.mRespJsonStr = writer.write(root);
					NetworkSystem::getSingleton().sendMsg(floorUpResp, connId);

						cout << floorUpResp.mRespJsonStr;
					return ;
				}
			}

			if(index == 2)
			{
				if(treasureIter->mValue !=  servantFloorTemp->mSlot3)
				{
					//提示玩家参数错误
					
					GCServantFloorUpResp floorUpResp;
					floorUpResp.mPacketID = BOC_SERVANT_FLOORUP_RESP;
					Json::Value root;
					root["errorId"] = LynxErrno::InvalidParameter;
					Json::StyledWriter writer;

					floorUpResp.mRespJsonStr = writer.write(root);
					NetworkSystem::getSingleton().sendMsg(floorUpResp, connId);

						cout << floorUpResp.mRespJsonStr;
					return ;
				}
			}

			if(index == 3)
			{
				if(treasureIter->mValue !=  servantFloorTemp->mSlot4)
				{
					//提示玩家参数错误

					GCServantFloorUpResp floorUpResp;
					floorUpResp.mPacketID = BOC_SERVANT_FLOORUP_RESP;
					Json::Value root;
					root["errorId"] = LynxErrno::InvalidParameter;
					Json::StyledWriter writer;

					floorUpResp.mRespJsonStr = writer.write(root);
					NetworkSystem::getSingleton().sendMsg(floorUpResp, connId);
						cout << floorUpResp.mRespJsonStr;
					return ;
				}
			}

			index ++;
		}

		//清空所有穿戴的宝物，并且层数+1
		for(List<UInt64>::Iter * treasureIter = findIter->mValue->equipTreasures.begin(); treasureIter != NULL;
			treasureIter = findIter->mValue->equipTreasures.next(treasureIter) )
		{
			treasureIter->mValue = 0;
		}

		findIter->mValue->floor ++;

		PersistUpdateServantNotify servantUpdatMsg;
		servantUpdatMsg.m_nPlayerUid = m_nPlayerUid;
		servantUpdatMsg.m_servantData = * findIter->mValue;

		PersistSystem::getSingleton().postThreadMsg(servantUpdatMsg, m_nPlayerUid);



		//提示玩家成功升层

		GCServantFloorUpResp floorUpResp;
		floorUpResp.mPacketID = BOC_SERVANT_FLOORUP_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::None;
		root["servantId"] = servantId;
		root["floor"] = findIter->mValue->floor;
		
		Json::StyledWriter writer;

		floorUpResp.mRespJsonStr = writer.write(root);
		NetworkSystem::getSingleton().sendMsg(floorUpResp, connId);


			cout << floorUpResp.mRespJsonStr;
		return ;

	}
	else
	{
		//提示玩家参数错误

		GCServantFloorUpResp floorUpResp;
		floorUpResp.mPacketID = BOC_SERVANT_FLOORUP_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;
		Json::StyledWriter writer;

		floorUpResp.mRespJsonStr = writer.write(root);
		NetworkSystem::getSingleton().sendMsg(floorUpResp, connId);

			cout << floorUpResp.mRespJsonStr;
		return ;
		
		
		return;
	}
}

ServantTreasure * ServantManager::getTreasure(UInt64 treasureId)
{
	Map<UInt64, ServantTreasure *>::Iter * treasureDataIter =  m_mapIdTreasure.find(treasureId);
	if(treasureDataIter)
	{
		return treasureDataIter->mValue;
	}
	else
	{
		return NULL;
	}
}

ServantTreasure*  ServantManager::addTreasure(UInt64 treasureId, UInt32 count)
{
	Map<UInt64, ServantTreasure *>::Iter * treasureDataIter =  m_mapIdTreasure.find(treasureId);
	
	//找到对应的宝物
	if(treasureDataIter)
	{
		treasureDataIter->mValue->count += count;

		PersistUpdateServantTreasureNotify updateServantTreasureMsg;
		updateServantTreasureMsg.m_nPlayerUid = m_nPlayerUid;
		updateServantTreasureMsg.m_servantTreasure = *treasureDataIter->mValue;

		PersistSystem::getSingleton().postThreadMsg(updateServantTreasureMsg, m_nPlayerUid);


		return treasureDataIter->mValue;
	}
	else
	{
		//没找到，那么添加到列表
		ServantTreasure servantTreasure;
		servantTreasure.count = count;
		servantTreasure.treasureId = treasureId;
		servantTreasure.treasureUid = LogicSystem::getSingleton().generateItemGuid();
		
		List<ServantTreasure>::Iter * treasureIter = m_pTreasureList->insertTail(servantTreasure);
	
		m_mapIdTreasure.insert(treasureId, &treasureIter->mValue);

		PersistAddServantTreasureNotify addServantTreasureMsg;
		addServantTreasureMsg.m_nPlayerUid = m_nPlayerUid;
		addServantTreasureMsg.m_servantTreasure = servantTreasure;

		PersistSystem::getSingleton().postThreadMsg(addServantTreasureMsg, m_nPlayerUid);

		return &treasureIter->mValue;
	}
}

ServantTreasure * ServantManager::GMsetTreasure(UInt64 treasureId, UInt32 count)
{
	Map<UInt64, ServantTreasure *>::Iter * treasureDataIter =  m_mapIdTreasure.find(treasureId);

	//找到对应的宝物
	if(treasureDataIter)
	{
		treasureDataIter->mValue->count = count;

		PersistUpdateServantTreasureNotify updateServantTreasureMsg;
		updateServantTreasureMsg.m_nPlayerUid = m_nPlayerUid;
		updateServantTreasureMsg.m_servantTreasure = *treasureDataIter->mValue;

		PersistSystem::getSingleton().postThreadMsg(updateServantTreasureMsg, m_nPlayerUid);


		return treasureDataIter->mValue;
	}
	else
	{
		//没找到，那么添加到列表
		ServantTreasure servantTreasure;
		servantTreasure.count = count;
		servantTreasure.treasureId = treasureId;
		servantTreasure.treasureUid = LogicSystem::getSingleton().generateItemGuid();

		List<ServantTreasure>::Iter * treasureIter = m_pTreasureList->insertTail(servantTreasure);

		m_mapIdTreasure.insert(treasureId, &treasureIter->mValue);

		PersistAddServantTreasureNotify addServantTreasureMsg;
		addServantTreasureMsg.m_nPlayerUid = m_nPlayerUid;
		addServantTreasureMsg.m_servantTreasure = servantTreasure;

		PersistSystem::getSingleton().postThreadMsg(addServantTreasureMsg, m_nPlayerUid);

		return &treasureIter->mValue;
	}
}


bool ServantManager::costTreasure(UInt64 treasureId, UInt32 count)
{
	Map<UInt64, ServantTreasure *>::Iter * treasureDataIter =  m_mapIdTreasure.find(treasureId);

	//找到对应的宝物
	if(treasureDataIter)
	{
		if(treasureDataIter->mValue->count < count)
		{
			return false;
		}

		treasureDataIter->mValue->count -= count;

		PersistUpdateServantTreasureNotify updateServantTreasureMsg;
		updateServantTreasureMsg.m_nPlayerUid = m_nPlayerUid;
		updateServantTreasureMsg.m_servantTreasure = *treasureDataIter->mValue;

		PersistSystem::getSingleton().postThreadMsg(updateServantTreasureMsg, m_nPlayerUid);

		return true;

	}
	else
	{
		return false;
	}
}

void ServantManager::addBattleStone(UInt32 count)
{
	m_pServantData->battleStone += count;

	m_pPlayer->getPersistManager().setDirtyBit(SERVANTFOODBIT);
}

void ServantManager::addServantSwitch(UInt32 count)
{
	m_pServantData->servantSwitch += count;
	m_pPlayer->getPersistManager().setDirtyBit(SERVANTFOODBIT);
}

UInt32 ServantManager::getServantSwitch()
{
	return m_pServantData->servantSwitch;
}

bool ServantManager::costBattleStone(UInt32 count)
{
	if(m_pServantData->battleStone < count)
	{
		return false;
	}

	m_pServantData->battleStone -= count;
	m_pPlayer->getPersistManager().setDirtyBit(SERVANTFOODBIT);

	return true;

}

bool ServantManager::costServantSwitch(UInt32 count)
{
	if(m_pServantData->servantSwitch < count)
	{
		return false;
	}

	m_pServantData->servantSwitch -= count;

	m_pPlayer->getPersistManager().setDirtyBit(SERVANTFOODBIT);

	return true;
}



UInt32 ServantManager::addServantBattleStone(UInt32 count)
{
	m_pServantData->battleStone += count;

	m_pPlayer->getPersistManager().setDirtyBit(SERVANTFOODBIT);
	return m_pServantData->battleStone;
}

UInt32 ServantManager::addServantFood1(UInt32 count)
{
	m_pServantData->food1count += count;
	m_pPlayer->getPersistManager().setDirtyBit(SERVANTFOODBIT);
	return m_pServantData->food1count;
}

UInt32 ServantManager::addServantFood2(UInt32 count)
{
	m_pServantData->food2count += count;
	m_pPlayer->getPersistManager().setDirtyBit(SERVANTFOODBIT);
	return m_pServantData->food2count;
}

UInt32 ServantManager::addServantFood3(UInt32 count)
{
	m_pServantData->food3count += count;
	m_pPlayer->getPersistManager().setDirtyBit(SERVANTFOODBIT);
	return m_pServantData->food3count;
}

UInt32 ServantManager::addServantFood4(UInt32 count)
{
	m_pServantData->food4count += count;
	m_pPlayer->getPersistManager().setDirtyBit(SERVANTFOODBIT);
	return m_pServantData->food4count;
}

UInt32 ServantManager::addServantFood5(UInt32 count)
{
	m_pServantData->food5count += count;
	m_pPlayer->getPersistManager().setDirtyBit(SERVANTFOODBIT);
	return m_pServantData->food5count;
}

UInt32 ServantManager::getServantFood(UInt32 ServantType)
{
	if(ServantType == ServantFood1)
	{
		return m_pServantData->food1count;
	}

	if(ServantType == ServantFood2)
	{
		return m_pServantData->food2count;
	}

	if(ServantType == ServantFood3)
	{
		return m_pServantData->food3count;
	}

	if(ServantType == ServantFood4)
	{
		return m_pServantData->food4count;
	}

	if(ServantType == ServantFood5)
	{
		return m_pServantData->food5count;
	}

	if(ServantType == ServantBattleStone)
	{
		return m_pServantData->battleStone;
	}
	return 0;

}
void ServantManager::servantLvUp(UInt64 servantId, const List<UInt32>& foodList)
{
	const ConnId& connId = m_pPlayer->getConnId();
	
	Map<UInt64 ,ServantData*>::Iter * servantDataIter = m_mapIdServant.find(servantId);
	
	if(!servantDataIter)
	{
		//提示玩家id错误
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;
		Json::StyledWriter writer;

		GCServantLvUpResp lvUpResp;
		lvUpResp.mPacketID = BOC_SERVANT_LVUP_RESP;
		lvUpResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(lvUpResp,connId);
		cout << lvUpResp.mRespJsonStr;
		return;
	}

	ServantLvUpTemplate * servantLvUpTemp = SERVANTLVUP_TABLE().get(servantDataIter->mValue->level);
	if(!servantLvUpTemp)
	{
		//提示玩家参数不正确
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;
	
		Json::StyledWriter writer;

		GCServantLvUpResp lvUpResp;
		lvUpResp.mPacketID = BOC_SERVANT_LVUP_RESP;
		lvUpResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(lvUpResp,connId);
		cout << lvUpResp.mRespJsonStr;
		return ;
	}

	UInt64 playerLv = m_pPlayer->getPlayerLeval();
	if(servantDataIter->mValue->level >=  playerLv)
	{
		//提示玩家参数不正确
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;
	
		Json::StyledWriter writer;

		GCServantLvUpResp lvUpResp;
		lvUpResp.mPacketID = BOC_SERVANT_LVUP_RESP;
		lvUpResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(lvUpResp,connId);
		cout << lvUpResp.mRespJsonStr;
		return ;
	}

	if(!servantLvUpTemp->mExp)
	{
		//提示玩家已经达到最大等级
		Json::Value root;
		root["errorId"] = LynxErrno::ServantLvLimit;

		Json::StyledWriter writer;

		GCServantLvUpResp lvUpResp;
		lvUpResp.mPacketID = BOC_SERVANT_LVUP_RESP;
		lvUpResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(lvUpResp,connId);
		cout << lvUpResp.mRespJsonStr;

		return;
	}


	UInt32 getExp = 0;
	UInt32 foodIndex = 0;
	
	Vector<UInt32*> foodscount;
	foodscount.push_back(&m_pServantData->food1count);
	foodscount.push_back(&m_pServantData->food2count);
	foodscount.push_back(&m_pServantData->food3count);
	foodscount.push_back(&m_pServantData->food4count);
	foodscount.push_back(&m_pServantData->food5count);

	for(const List<UInt32>::Iter * foodIter = foodList.begin(); foodIter != NULL; foodIter = foodList.next(foodIter))
	{
		if(foodIter->mValue > *foodscount[foodIndex])
		{
			//提示玩家数量不足
			Json::Value root;
			root["errorId"] = LynxErrno::MaterialNotEnough;

			Json::StyledWriter writer;

			GCServantLvUpResp lvUpResp;
			lvUpResp.mPacketID = BOC_SERVANT_LVUP_RESP;
			lvUpResp.mRespJsonStr = writer.write(root);

			NetworkSystem::getSingleton().sendMsg(lvUpResp,connId);
			cout << lvUpResp.mRespJsonStr;
			return;
		}

		foodIndex ++;
		
	}

	foodIndex = 0;
	for(const List<UInt32>::Iter * foodIter = foodList.begin(); foodIter != NULL; foodIter = foodList.next(foodIter))
	{
		ServantLvUpMaterialTemplate * servantLvUpMaterialTemp = SERVANTMATERIAL_TABLE().get(foodIndex+1);
		if (servantLvUpMaterialTemp == NULL)
		{
			LOG_WARN("servantLvUpMaterialTemp not found!!");
			return;
		}
		*foodscount[foodIndex] -= foodIter->mValue;
		getExp += foodIter->mValue * servantLvUpMaterialTemp->mExpProvide;
		foodIndex ++;
	}

	servantDataIter->mValue->lvexp += getExp;

	//
	while( servantDataIter->mValue->lvexp >= servantLvUpTemp->mExp)
	{
		servantDataIter->mValue->lvexp -= servantLvUpTemp->mExp;
		servantDataIter->mValue->level ++;
		if(servantDataIter->mValue->level >= 80)
		{
			servantDataIter->mValue->level = 80;
			break;
		}

		if(servantDataIter->mValue->level >= playerLv + 1)
		{
			servantDataIter->mValue->level = playerLv + 1;
			servantDataIter->mValue->lvexp = 0;
			break;
		}


		servantLvUpTemp = SERVANTLVUP_TABLE().get(servantDataIter->mValue->level);
		if (servantLvUpTemp == NULL)
		{
			LOG_WARN("servantLvUpTemp not found!!");
			Json::Value root;
			root["errorId"] = LynxErrno::ClienServerDataNotMatch;

			Json::StyledWriter writer;

			GCServantLvUpResp lvUpResp;
			lvUpResp.mPacketID = BOC_SERVANT_LVUP_RESP;
			lvUpResp.mRespJsonStr = writer.write(root);

			NetworkSystem::getSingleton().sendMsg(lvUpResp,connId);
			cout << lvUpResp.mRespJsonStr;
			return;
		}
	}

	PersistUpdateServantNotify updateServant;
	updateServant.m_nPlayerUid = m_nPlayerUid;
	updateServant.m_servantData = *(servantDataIter->mValue);

	PersistSystem::getSingleton().postThreadMsg(updateServant, updateServant.m_nPlayerUid);

	m_pPlayer->getPersistManager().setDirtyBit(SERVANTFOODBIT);


	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["food1"] = m_pServantData->food1count;
	root["food2"] = m_pServantData->food2count;
	root["food3"] = m_pServantData->food3count;
	root["food4"] = m_pServantData->food4count;
	root["food5"] = m_pServantData->food5count;
	root["lvexp"] = servantDataIter->mValue->lvexp;
	root["servantlv"] = servantDataIter->mValue->level;
	root["servantId"] = servantDataIter->mValue->servantId;

	Json::StyledWriter writer;

	GCServantLvUpResp lvUpResp;
	lvUpResp.mPacketID = BOC_SERVANT_LVUP_RESP;
	lvUpResp.mRespJsonStr = writer.write(root);
	cout << lvUpResp.mRespJsonStr;
	NetworkSystem::getSingleton().sendMsg(lvUpResp,connId);

	//更新七日训
	LogicSystem::getSingleton().updateSevenDayTask(m_pPlayer->getPlayerGuid(),SDT09,1);

}

void ServantManager:: checkActiveCondition()
{
	const PlayerBaseData &playerBaseData = m_pPlayer->getPlayerBaseData();

	UInt32 playerLv = m_pPlayer->mPlayerData.mBaseData.m_nLevel;

	for(UInt32 i = 1; i < m_bitVec.size(); i++)
	{
		bool openbool = getOpenState(i);
		if(!openbool)
		{
			checkBattleOpen(i,m_bitVec, playerLv);
		}
		
	}
	//playerBaseData.m_nLevel
}

void ServantManager::checkBattleOpen(UInt32 &battleIndex, const Vector<UInt32> &bitVec,  UInt32 & playerlv)
{
	assert(battleIndex <= 12);
	
	ServantBattleOpenTemplate * servantBattleOpen = SERVANTBATTLEOPEN_TABLE().get(battleIndex);
	if (servantBattleOpen == NULL)
	{
		LOG_WARN("servantBattleOpen not found!!");
		return;
	}

	if(servantBattleOpen->mLevel <= playerlv && !servantBattleOpen->mCost)
	{
		//原逻辑按照花费为0表示自动解锁，现在需求更改，暂时注释
		/*if(battleIndex >= 6)
		{
			m_pServantBattleData->assistBattleBit |= bitVec[battleIndex];			
		}
		else
		{
			m_pServantBattleData->mainBattleBit |= bitVec[battleIndex];
		}*/
		//现更改为主要阵位自动解锁
		if(battleIndex < 6)
		{
			m_pServantBattleData->mainBattleBit |= bitVec[battleIndex];
		}
		
	}

	m_pPlayer->getPersistManager().setDirtyBit(SERVANTBATTLEBIT);
}

//battlePos 1到12
bool ServantManager::getOpenState(UInt32 battlePos)
{
	assert(battlePos <= 12);

	if(battlePos >= 6)
	{
		return  (m_pServantBattleData->assistBattleBit & m_bitVec[battlePos]);
		
	}
	else
	{
		return (m_pServantBattleData->mainBattleBit & m_bitVec[battlePos]);
	}
	
}

//battlePos 1到5
bool ServantManager::getMainOpenState(UInt32 battlePos)
{
	assert(battlePos <= 5);
	return (m_pServantBattleData->mainBattleBit & m_bitVec[battlePos]);
}

//battlePos 6到12
bool ServantManager::getAssistOpenState(UInt32 battlePos)
{
	assert(battlePos >= 6);
    assert(battlePos <= 12);
	return  (m_pServantBattleData->assistBattleBit & m_bitVec[battlePos]);
}

std::string ServantManager::convertDataToJson()
{
	Json::Value root;
	root["food1"]  = m_pServantData->food1count;
	root["food2"]  = m_pServantData->food2count;
	root["food3"]  = m_pServantData->food3count;
	root["food4"]  = m_pServantData->food4count;
	root["food5"]  = m_pServantData->food5count;

	root["battleStone"] = m_pServantData->battleStone;

	root["servantswitch"] = m_pServantData->servantSwitch;

	UInt32 mainIndex = 0;
	for(List<UInt64>::Iter *mainBattleIter = m_pServantBattleData->mainServantBattle.begin(); mainBattleIter != NULL; 
		mainBattleIter = m_pServantBattleData->mainServantBattle.next(mainBattleIter))
	{
		mainIndex++;
		//阵位上阵英雄id非0
		if(mainBattleIter->mValue)
		{
			root["mainbattle"].append(mainBattleIter->mValue);
		}
		else
		{
			//为0判断是否开启阵位
			bool openbool  = getOpenState(mainIndex);
			if(openbool)
			{
				//开启阵位，那么为0表示没上阵
				root["mainbattle"].append(mainBattleIter->mValue);
			}
			else
			{
				//没开启，发送-1表示阵位未开启
				root["mainbattle"].append(-1);
			}
		}
		
	}

	UInt32 assistIndex = 5;
	for(List<AssistBattleData>::Iter *assistBattleIter = m_pServantBattleData->assistServantBattle.begin(); assistBattleIter != NULL; 
		assistBattleIter = m_pServantBattleData->assistServantBattle.next(assistBattleIter))
	{
		assistIndex++;
		//助阵位上阵id非0
		if(assistBattleIter->mValue.equipAssist)
		{
			Json::Value assistRoot;

			assistRoot["equip"] = assistBattleIter->mValue.equipAssist;
			assistRoot["quality"] = assistBattleIter->mValue.quality;

			root["assistbattle"].append(assistRoot);
		}
		else
		{
			//为0要判断是否开启
			bool openbool  = getOpenState(assistIndex);

			//开启，所以为0表示没上阵
			if(openbool)
			{
				Json::Value assistRoot;

				assistRoot["equip"] = assistBattleIter->mValue.equipAssist;
				assistRoot["quality"] = assistBattleIter->mValue.quality;

				root["assistbattle"].append(assistRoot);
			}
			else
			{
				//没开启，为0表示没上阵，阵位也没开启，发送-1
				Json::Value assistRoot;

				assistRoot["equip"] = -1;
				assistRoot["quality"] = assistBattleIter->mValue.quality;

				root["assistbattle"].append(assistRoot);
			}

		}
	}

	for(List<ServantTreasure>::Iter * treasureIter = m_pTreasureList->begin(); treasureIter != NULL; 
		treasureIter = m_pTreasureList->next(treasureIter))
	{
		Json::Value treasureRoot;

		treasureRoot["treasureUid"] = treasureIter->mValue.treasureUid;
		treasureRoot["treasureId"] = treasureIter->mValue.treasureId;
        treasureRoot["count"] = treasureIter->mValue.count;

		root["treasure"].append(treasureRoot);
	}

	for(List<ServantData>::Iter * servantIter = m_pServantList->begin(); servantIter != NULL; 
		servantIter = m_pServantList->next(servantIter))
	{
		Json::Value servantRoot;
		servantRoot["servantUid"] = servantIter->mValue.servantUid;
		servantRoot["servantId"] = servantIter->mValue.servantId;
		servantRoot["pieceCount"] = servantIter->mValue.pieceCount;
		servantRoot["level"] = servantIter->mValue.level;
		servantRoot["lvexp"] = servantIter->mValue.lvexp;
		servantRoot["star"] = servantIter->mValue.star;
		servantRoot["floor"] = servantIter->mValue.floor;
		servantRoot["infolock"] = servantIter->mValue.infolock;

		for(List<UInt64>::Iter * treasureIter = servantIter->mValue.equipTreasures.begin(); treasureIter != NULL;
			treasureIter = servantIter->mValue.equipTreasures.next(treasureIter))
		{
			servantRoot["equipTreasures"].append(treasureIter->mValue);
		}

		

		root["servantData"].append(servantRoot);
	}


	Json::StyledWriter writer;
	std::string jsonStr = writer.write(root);
	//cout << jsonStr;
	return jsonStr;
}


ServantTreasure* ServantManager::combineTreasure(UInt64 treasureId , UInt32 count)
{
	const ConnId& connId = m_pPlayer->getConnId();
	
	ServantTreasureTemplate * servantTreasureTemp = SERVANTTREASURE_TABLE().get(treasureId);
	if (servantTreasureTemp == NULL)
	{
		LOG_WARN("servantTreasureTemp not found!!");
		return NULL;
	}
	
	Vector<UInt64> combineVec;
    Vector<ServantTreasure *> servantTreasureVec;

	if(servantTreasureTemp->mCombineId1)
	{
		Map<UInt64, ServantTreasure *>::Iter * treasureIter = m_mapIdTreasure.find(servantTreasureTemp->mCombineId1);
		
		if(!treasureIter)
		{
			//提示玩家宝物不存在
			GCTreasureCombineResp combineResp;
			combineResp.mPacketID = BOC_STREASURE_COMBINE_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::STreasureNotExit;
			Json::StyledWriter writer;
			combineResp.mRespJsonStr = writer.write(root);
			cout << combineResp.mRespJsonStr;

			NetworkSystem::getSingleton().sendMsg(combineResp,connId);
			return NULL;

		}
		
		if(servantTreasureTemp->mCombineCount1 * count > treasureIter->mValue->count)
		{
           //提示玩家数量不足
			GCTreasureCombineResp combineResp;
			combineResp.mPacketID = BOC_STREASURE_COMBINE_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::STreasureNotEnough;
			Json::StyledWriter writer;
			combineResp.mRespJsonStr = writer.write(root);	
		    cout << combineResp.mRespJsonStr;

			NetworkSystem::getSingleton().sendMsg(combineResp,connId);
			return NULL;
		}

		combineVec.push_back(servantTreasureTemp->mCombineCount1 * count);
		servantTreasureVec.push_back(treasureIter->mValue);
	}

	if(servantTreasureTemp->mCombineId2)
	{
		Map<UInt64, ServantTreasure *>::Iter * treasureIter = m_mapIdTreasure.find(servantTreasureTemp->mCombineId2);

		if(!treasureIter)
		{
			//提示玩家宝物不存在
			GCTreasureCombineResp combineResp;
			combineResp.mPacketID = BOC_STREASURE_COMBINE_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::STreasureNotExit;
			Json::StyledWriter writer;
			combineResp.mRespJsonStr = writer.write(root);
			cout << combineResp.mRespJsonStr;

			NetworkSystem::getSingleton().sendMsg(combineResp,connId);
			return NULL;

		}

		if(servantTreasureTemp->mCombineCount2 * count> treasureIter->mValue->count)
		{
			//提示玩家数量不足
			
			GCTreasureCombineResp combineResp;
			combineResp.mPacketID = BOC_STREASURE_COMBINE_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::STreasureNotEnough;
			Json::StyledWriter writer;
			combineResp.mRespJsonStr = writer.write(root);	
			cout << combineResp.mRespJsonStr;

			NetworkSystem::getSingleton().sendMsg(combineResp,connId);
			return NULL;
		}

		combineVec.push_back(servantTreasureTemp->mCombineCount2 * count);
		servantTreasureVec.push_back(treasureIter->mValue);
	}

	if(servantTreasureTemp->mCombineId3)
	{
		Map<UInt64, ServantTreasure *>::Iter * treasureIter = m_mapIdTreasure.find(servantTreasureTemp->mCombineId3);

		if(!treasureIter)
		{
			//提示玩家宝物不存在
			GCTreasureCombineResp combineResp;
			combineResp.mPacketID = BOC_STREASURE_COMBINE_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::STreasureNotExit;
			Json::StyledWriter writer;
			combineResp.mRespJsonStr = writer.write(root);
			cout << combineResp.mRespJsonStr;

			NetworkSystem::getSingleton().sendMsg(combineResp,connId);
			return NULL;

		}

		if(servantTreasureTemp->mCombineCount3 * count> treasureIter->mValue->count)
		{
			//提示玩家数量不足
			GCTreasureCombineResp combineResp;
			combineResp.mPacketID = BOC_STREASURE_COMBINE_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::STreasureNotEnough;
			Json::StyledWriter writer;
			combineResp.mRespJsonStr = writer.write(root);	
			cout << combineResp.mRespJsonStr;

			NetworkSystem::getSingleton().sendMsg(combineResp,connId);
			return NULL;
		}

		combineVec.push_back(servantTreasureTemp->mCombineCount3 * count);
		servantTreasureVec.push_back(treasureIter->mValue);
	}

	GCTreasureCombineResp combineResp;
	combineResp.mPacketID = BOC_STREASURE_COMBINE_RESP;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	

	for(UInt32 i = 0; i < combineVec.size(); i++)
	{
		servantTreasureVec[i]->count -= combineVec[i];

		PersistUpdateServantTreasureNotify updateServantTreasureMsg;
		updateServantTreasureMsg.m_nPlayerUid = m_nPlayerUid;
		updateServantTreasureMsg.m_servantTreasure = *servantTreasureVec[i];

		PersistSystem::getSingleton().postThreadMsg(updateServantTreasureMsg, m_nPlayerUid);

		Json::Value treasureRoot;
		treasureRoot["treasureId"] = servantTreasureVec[i]->treasureId;
		treasureRoot["treasureCount"] = servantTreasureVec[i]->count;

		root["changeTreasures"].append(treasureRoot);

		

	}

	ServantTreasure* newTreasure =  addTreasure(treasureId, count);

	Json::Value treasureRootNew;
	treasureRootNew["treasureId"] = newTreasure->treasureId;
	treasureRootNew["treasureCount"] = newTreasure->count;

	root["changeTreasures"].append(treasureRootNew);

	Json::StyledWriter writer;
	combineResp.mRespJsonStr = writer.write(root);	
	cout << combineResp.mRespJsonStr;

	if( LogicSystem::getSingleton().combinSendData(m_pPlayer->getPlayerGuid(),AWARD_SERVANTTREASURE,root) ==false)	
	{
		NetworkSystem::getSingleton().sendMsg(combineResp,connId);
	}
	
	return NULL;
}

 bool ServantManager::autoCombineTreasure(UInt64 treasureId, List<UInt64 > & changeList)
{
	
	const ConnId& connId = m_pPlayer->getConnId();
	
	ServantTreasureTemplate * servantTreasureTemp = SERVANTTREASURE_TABLE().get(treasureId);
	if (servantTreasureTemp == NULL)
	{
		LOG_WARN("servantTreasureTemp not found!!"); 
		return false;
	}

	if(servantTreasureTemp->mCombineId1)
	{
		Map<UInt64, ServantTreasure *>::Iter * treasureIter = m_mapIdTreasure.find(servantTreasureTemp->mCombineId1);
		
		if(!treasureIter)
		{
			//提示玩家宝物不存在
			
			return false;

		}
		
		if(servantTreasureTemp->mCombineCount1  > treasureIter->mValue->count)
		{
           //提示玩家数量不足
			
			return false;
		}

		changeList.insertTail(servantTreasureTemp->mCombineId1);
	}
	else
	{
		return false;
	}
	if(servantTreasureTemp->mCombineId2)
	{
		Map<UInt64, ServantTreasure *>::Iter * treasureIter = m_mapIdTreasure.find(servantTreasureTemp->mCombineId2);

		if(!treasureIter)
		{
			//提示玩家宝物不存在
		
			return false;

		}

		if(servantTreasureTemp->mCombineCount2 > treasureIter->mValue->count)
		{
			//提示玩家数量不足
			
			return false;
		}

		changeList.insertTail(servantTreasureTemp->mCombineId2);
	}
	else
	{
		return false;
	}
	if(servantTreasureTemp->mCombineId3)
	{
		Map<UInt64, ServantTreasure *>::Iter * treasureIter = m_mapIdTreasure.find(servantTreasureTemp->mCombineId3);

		if(!treasureIter)
		{
			//提示玩家宝物不存在
		
			return false;

		}

		if(servantTreasureTemp->mCombineCount3> treasureIter->mValue->count)
		{
			//提示玩家数量不足
		
			return false;
		}

		changeList.insertTail(servantTreasureTemp->mCombineId3);
	}
	else
	{
		return false;
	}
	if(servantTreasureTemp->mCombineId1)
	{
		Map<UInt64, ServantTreasure *>::Iter * treasureIter1 = m_mapIdTreasure.find(servantTreasureTemp->mCombineId1);
		treasureIter1->mValue->count -= servantTreasureTemp->mCombineCount1;

		PersistUpdateServantTreasureNotify updateServantTreasureMsg1;
		updateServantTreasureMsg1.m_nPlayerUid = m_nPlayerUid;
		updateServantTreasureMsg1.m_servantTreasure = *(treasureIter1->mValue);

		PersistSystem::getSingleton().postThreadMsg(updateServantTreasureMsg1, m_nPlayerUid);
	}
	
	if(servantTreasureTemp->mCombineId2)
	{
		Map<UInt64, ServantTreasure *>::Iter * treasureIter2 = m_mapIdTreasure.find(servantTreasureTemp->mCombineId2);
		treasureIter2->mValue->count -= servantTreasureTemp->mCombineCount2;

		PersistUpdateServantTreasureNotify updateServantTreasureMsg2;
		updateServantTreasureMsg2.m_nPlayerUid = m_nPlayerUid;
		updateServantTreasureMsg2.m_servantTreasure = *(treasureIter2->mValue);

		PersistSystem::getSingleton().postThreadMsg(updateServantTreasureMsg2, m_nPlayerUid);
	}
	
	if(servantTreasureTemp->mCombineId3)
	{
		Map<UInt64, ServantTreasure *>::Iter * treasureIter3 = m_mapIdTreasure.find(servantTreasureTemp->mCombineId3);
		treasureIter3->mValue->count -= servantTreasureTemp->mCombineCount3;

		PersistUpdateServantTreasureNotify updateServantTreasureMsg3;
		updateServantTreasureMsg3.m_nPlayerUid = m_nPlayerUid;
		updateServantTreasureMsg3.m_servantTreasure = *(treasureIter3->mValue);

		PersistSystem::getSingleton().postThreadMsg(updateServantTreasureMsg3, m_nPlayerUid);
	}

	

	ServantTreasure* newTreasure =  addTreasure(treasureId, 1);

	
	return true;

}



bool ServantManager::treasureEquip(UInt64 servantId, const Vector<UInt64>& equipVec)
{
	const ConnId& connId = m_pPlayer->getConnId();
	
	if(equipVec.size() != 4)
	{
		//提示客户端参数不对
		GCTreasureEquipResp equipResp;
		equipResp.mPacketID = BOC_STREASURE_EQUIP_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;
		Json::StyledWriter writer;
		
		equipResp.mRespJsonStr = writer.write(root);

		cout <<equipResp.mRespJsonStr;

		NetworkSystem::getSingleton().sendMsg(equipResp,connId);
		return false;
	}

	Map<UInt64 ,ServantData*>::Iter * servantIter = m_mapIdServant.find(servantId);
	
	if(!servantIter)
	{
		//提示玩家参数不对
		GCTreasureEquipResp equipResp;
		equipResp.mPacketID = BOC_STREASURE_EQUIP_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;
		Json::StyledWriter writer;

		equipResp.mRespJsonStr = writer.write(root);

		cout <<equipResp.mRespJsonStr;

		NetworkSystem::getSingleton().sendMsg(equipResp,connId);

		return false;
	}

	ServantFloorTemplate * servantFloorTmp = SERVANTFLOOR_TABLE().reverseGet(servantIter->mValue->servantId, servantIter->mValue->floor);
	UInt32 index = 0;
    
	//遍历当前佣兵穿戴的装备
	for(List<UInt64>::Iter  * treasureIter = servantIter->mValue->equipTreasures.begin(); treasureIter != NULL; 
		treasureIter = servantIter->mValue->equipTreasures.next(treasureIter) )
	{
		//如果客户端发过来该位置为空，那么跳过，判断下移槽位
		if(!equipVec[index])
		{
			index ++;
			continue;
		}
		
		//佣兵穿戴的和客户端发送过来的一样，那么跳过，说明没有更改
		if(equipVec[index] == treasureIter->mValue)
		{
			index ++;
			continue;
		}

		//如果佣兵现在有穿戴，而客户端发来消息该位置没有穿戴说明参数不对，当前穿戴的和客户端发过来的数据不一致
		if(treasureIter -> mValue)
		{
			//提示玩家参数不对	

			GCTreasureEquipResp equipResp;
			equipResp.mPacketID = BOC_STREASURE_EQUIP_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::InvalidParameter;
			Json::StyledWriter writer;

			equipResp.mRespJsonStr = writer.write(root);

			cout <<equipResp.mRespJsonStr;

			NetworkSystem::getSingleton().sendMsg(equipResp,connId);

			return false;
		}
	
		//当该位置穿戴的为空，并且客户端发过来的装备id在玩家所有佣兵装备中查找
		Map<UInt64, ServantTreasure *>::Iter *  equipTreasureIter = m_mapIdTreasure.find(equipVec[index]);
		
		if(!equipTreasureIter || !(equipTreasureIter->mValue->count) )
		{
			//提示玩家宝物不存在

			GCTreasureEquipResp equipResp;
			equipResp.mPacketID = BOC_STREASURE_EQUIP_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::STreasureNotExit;
			Json::StyledWriter writer;

			equipResp.mRespJsonStr = writer.write(root);

			cout <<equipResp.mRespJsonStr;

			NetworkSystem::getSingleton().sendMsg(equipResp,connId);


			return false;
		}

		//审核玩家发送过来的id和表格里配置的是否一样
		if(index == 0)
		{
			if(servantFloorTmp->mSlot1 != equipVec[index])
			{
				//提示玩家参数不正确
				GCTreasureEquipResp equipResp;
				equipResp.mPacketID = BOC_STREASURE_EQUIP_RESP;

				Json::Value root;
				root["errorId"] = LynxErrno::InvalidParameter;
				Json::StyledWriter writer;

				equipResp.mRespJsonStr = writer.write(root);

				cout <<equipResp.mRespJsonStr;

				NetworkSystem::getSingleton().sendMsg(equipResp,connId);

				return false;
			}
		}

		if(index == 1)
		{
			if(servantFloorTmp->mSlot2 != equipVec[index])
			{
				//提示玩家参数不正确

				GCTreasureEquipResp equipResp;
				equipResp.mPacketID = BOC_STREASURE_EQUIP_RESP;

				Json::Value root;
				root["errorId"] = LynxErrno::InvalidParameter;
				Json::StyledWriter writer;

				equipResp.mRespJsonStr = writer.write(root);

				cout <<equipResp.mRespJsonStr;

				NetworkSystem::getSingleton().sendMsg(equipResp,connId);

				return false;
				
			}
		}

		if(index == 2)
		{
			if(servantFloorTmp->mSlot3 != equipVec[index])
			{
				//提示玩家参数不正确
				GCTreasureEquipResp equipResp;
				equipResp.mPacketID = BOC_STREASURE_EQUIP_RESP;

				Json::Value root;
				root["errorId"] = LynxErrno::InvalidParameter;
				Json::StyledWriter writer;

				equipResp.mRespJsonStr = writer.write(root);

				cout <<equipResp.mRespJsonStr;

				NetworkSystem::getSingleton().sendMsg(equipResp,connId);

				return false;
			}
		}

		if(index == 3)
		{
			if(servantFloorTmp->mSlot4 != equipVec[index])
			{
				//提示玩家参数不正确
				GCTreasureEquipResp equipResp;
				equipResp.mPacketID = BOC_STREASURE_EQUIP_RESP;

				Json::Value root;
				root["errorId"] = LynxErrno::InvalidParameter;
				Json::StyledWriter writer;

				equipResp.mRespJsonStr = writer.write(root);

				cout <<equipResp.mRespJsonStr;

				NetworkSystem::getSingleton().sendMsg(equipResp,connId);

				return false;
			}
		}

		index++;

	}

	GCTreasureEquipResp equipResp;
	equipResp.mPacketID = BOC_STREASURE_EQUIP_RESP;
	Json::Value root;

	
	index = 0;
	for(List<UInt64>::Iter  * treasureIter = servantIter->mValue->equipTreasures.begin(); treasureIter != NULL; 
		treasureIter = servantIter->mValue->equipTreasures.next(treasureIter) )
	{
		if(!equipVec[index])
		{
			index ++;
			root["equipTreasures"].append(treasureIter->mValue);
			continue;
		}


		if(equipVec[index] == treasureIter->mValue)
		{
			index ++;
			root["equipTreasures"].append(treasureIter->mValue);
			continue;
		}

		
		Map<UInt64, ServantTreasure *>::Iter *  equipTreasureIter = m_mapIdTreasure.find(equipVec[index]);

		equipTreasureIter->mValue->count--;

		PersistUpdateServantTreasureNotify updateTreasure ;
		updateTreasure.m_nPlayerUid = m_nPlayerUid;
		updateTreasure.m_servantTreasure = *(equipTreasureIter->mValue);

		PersistSystem::getSingleton().postThreadMsg(updateTreasure, updateTreasure.m_nPlayerUid);




		//发现有新穿戴的宝物，当前槽位为空
		treasureIter->mValue = equipVec[index];

		root["equipTreasures"].append(treasureIter->mValue);

		Json::Value changeTreasure;
		changeTreasure["treasureId"] = equipTreasureIter->mValue->treasureId;
		changeTreasure["treasureCount"] = equipTreasureIter->mValue->count;

		root["changeTreasures"].append(changeTreasure);

		index++;

	}
	
	root["errorId"] = LynxErrno::None;
	root["servantId"] = servantId;
	Json::StyledWriter writer;

	equipResp.mRespJsonStr = writer.write(root);

	cout <<equipResp.mRespJsonStr;

	NetworkSystem::getSingleton().sendMsg(equipResp,connId);

	
	PersistUpdateServantNotify updateServant;
	updateServant.m_nPlayerUid = m_nPlayerUid;
	updateServant.m_servantData = *(servantIter->mValue);

	PersistSystem::getSingleton().postThreadMsg(updateServant, updateServant.m_nPlayerUid);

	

	return true;
}

bool ServantManager::treasureEquipOnce(UInt64 servantId)
{
	const ConnId& connId = m_pPlayer->getConnId();
	
	List<UInt64> changeList;
	changeList.clear();
	Map<UInt64 ,ServantData*>::Iter * servantIter = m_mapIdServant.find(servantId);
	
	if(!servantIter)
	{
		//提示玩家参数不对
		GCServantEquipOnceResp equipResp;
		equipResp.mPacketID = BOC_STREASURE_EQUIP_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;
		Json::StyledWriter writer;

		equipResp.mRespJsonStr = writer.write(root);

		cout <<equipResp.mRespJsonStr;

		NetworkSystem::getSingleton().sendMsg(equipResp,connId);

		return false;
	}

	ServantFloorTemplate * servantFloorTmp = SERVANTFLOOR_TABLE().reverseGet(servantIter->mValue->servantId, servantIter->mValue->floor);
	UInt32 index = 0;
    
	//遍历当前佣兵穿戴的装备
	for(List<UInt64>::Iter  * treasureIter = servantIter->mValue->equipTreasures.begin(); treasureIter != NULL; 
		treasureIter = servantIter->mValue->equipTreasures.next(treasureIter) )
	{
		if(treasureIter->mValue)
		{
			index ++;
			continue;
		}
	
		//审核玩家发送过来的id和表格里配置的是否一样
		if(index == 0)
		{
			//当该位置穿戴的为空,根据配置表查找需要的装备是否存在，不存在进行合成操作
			Map<UInt64, ServantTreasure *>::Iter *  equipTreasureIter = m_mapIdTreasure.find(servantFloorTmp->mSlot1);
			//如果装备不存在或者数量为0，考虑是否可以合成
			if(!equipTreasureIter || !(equipTreasureIter->mValue->count) )
			{
				bool combineRes = autoCombineTreasure(servantFloorTmp->mSlot1,changeList);
				if(!combineRes)
				{
					index++;
					continue;
				}
			}
			
			equipTreasureIter = m_mapIdTreasure.find(servantFloorTmp->mSlot1);
			equipTreasureIter->mValue->count --;
			treasureIter->mValue = servantFloorTmp->mSlot1;
				PersistUpdateServantTreasureNotify updateServantTreasureMsg;
				updateServantTreasureMsg.m_nPlayerUid = m_nPlayerUid;
				updateServantTreasureMsg.m_servantTreasure = *(equipTreasureIter->mValue);
				changeList.insertTail(servantFloorTmp->mSlot1);
		}

		if(index == 1)
		{
			//当该位置穿戴的为空,根据配置表查找需要的装备是否存在，不存在进行合成操作
			Map<UInt64, ServantTreasure *>::Iter *  equipTreasureIter = m_mapIdTreasure.find(servantFloorTmp->mSlot2);
			//如果装备不存在或者数量为0，考虑是否可以合成
			if(!equipTreasureIter || !(equipTreasureIter->mValue->count) )
			{
				bool combineRes = autoCombineTreasure(servantFloorTmp->mSlot2,changeList);
				if(!combineRes)
				{
					index++;
					continue;
				}
			}
			
			equipTreasureIter = m_mapIdTreasure.find(servantFloorTmp->mSlot2);
			equipTreasureIter->mValue->count --;
			
			treasureIter->mValue = servantFloorTmp->mSlot2;

			PersistUpdateServantTreasureNotify updateServantTreasureMsg;
				updateServantTreasureMsg.m_nPlayerUid = m_nPlayerUid;
				updateServantTreasureMsg.m_servantTreasure = *(equipTreasureIter->mValue);
				changeList.insertTail(servantFloorTmp->mSlot2);
			
		}

		if(index == 2)
		{
			//当该位置穿戴的为空,根据配置表查找需要的装备是否存在，不存在进行合成操作
			Map<UInt64, ServantTreasure *>::Iter *  equipTreasureIter = m_mapIdTreasure.find(servantFloorTmp->mSlot3);

			//如果装备不存在或者数量为0，考虑是否可以合成
			if(!equipTreasureIter || !(equipTreasureIter->mValue->count) )
			{
				bool combineRes = autoCombineTreasure(servantFloorTmp->mSlot3,changeList);
				if(!combineRes)
				{
					index++;
					continue;
				}
			}
			
			equipTreasureIter = m_mapIdTreasure.find(servantFloorTmp->mSlot3);
			equipTreasureIter->mValue->count --;

			treasureIter->mValue = servantFloorTmp->mSlot3;

			PersistUpdateServantTreasureNotify updateServantTreasureMsg;
				updateServantTreasureMsg.m_nPlayerUid = m_nPlayerUid;
				updateServantTreasureMsg.m_servantTreasure = *(equipTreasureIter->mValue);

					changeList.insertTail(servantFloorTmp->mSlot3);

		}

		if(index == 3)
		{
			//当该位置穿戴的为空,根据配置表查找需要的装备是否存在，不存在进行合成操作
			Map<UInt64, ServantTreasure *>::Iter *  equipTreasureIter = m_mapIdTreasure.find(servantFloorTmp->mSlot4);

			//如果装备不存在或者数量为0，考虑是否可以合成
			if(!equipTreasureIter || !(equipTreasureIter->mValue->count) )
			{
				bool combineRes = autoCombineTreasure(servantFloorTmp->mSlot4,changeList);
				if(!combineRes)
				{
					index++;
					continue;
				}
			}

			equipTreasureIter = m_mapIdTreasure.find(servantFloorTmp->mSlot4);
			equipTreasureIter->mValue->count --;

			treasureIter->mValue = servantFloorTmp->mSlot4;

			PersistUpdateServantTreasureNotify updateServantTreasureMsg;
				updateServantTreasureMsg.m_nPlayerUid = m_nPlayerUid;
				updateServantTreasureMsg.m_servantTreasure = *(equipTreasureIter->mValue);

				changeList.insertTail(servantFloorTmp->mSlot4);
		}

			index ++;
	}

	Json::Value root;

	for(List<UInt64>::Iter  * treasureIter = servantIter->mValue->equipTreasures.begin(); treasureIter != NULL; 
		treasureIter = servantIter->mValue->equipTreasures.next(treasureIter) )
	{
			root["equipTreasures"].append(treasureIter->mValue);
	}

	root["servantId"] = servantId;

	for(List<UInt64>::Iter  * changeIter = changeList.begin(); changeIter != NULL; 
		changeIter = changeList.next(changeIter) )
	{
		Json::Value changeTreasure;
		Map<UInt64, ServantTreasure *>::Iter *  treasureIter = m_mapIdTreasure.find(changeIter->mValue);
		changeTreasure["treasureId"] = treasureIter->mValue->treasureId;
		changeTreasure["treasureCount"] = treasureIter->mValue->count;
		root["changeTreasures"].append(changeTreasure);
	}

	GCServantEquipOnceResp equipOnceResp;
	equipOnceResp.mPacketID = BOC_SERVANT_EQUIPONCE_RESP;
	
	Json::StyledWriter writer;
	
	equipOnceResp.mRespJsonStr = writer.write(root);


	NetworkSystem::getSingleton().sendMsg(equipOnceResp,connId);

	
	PersistUpdateServantNotify updateServant;
	updateServant.m_nPlayerUid = m_nPlayerUid;
	updateServant.m_servantData = *(servantIter->mValue);

	PersistSystem::getSingleton().postThreadMsg(updateServant, updateServant.m_nPlayerUid);

	return true;
}

//助阵位索引，从0到6
bool ServantManager::assistbattleOpen(UInt32 battleIndex)
{
	const ConnId& connId = m_pPlayer->getConnId();

	bool openState = getOpenState(battleIndex + 6);

	if(openState)
	{
		Json::Value root;
		root["errorId"] = LynxErrno::ServantBattleOpen;
		
		Json::StyledWriter writer;

		//提示玩家助阵位已经开启
		GCServantBattleOpenResp battleOpenResp;
		battleOpenResp.mRespJsonStr = writer.write(root);
		battleOpenResp.mPacketID = BOC_SERVANTBATTLE_OPEN_RESP;

		cout << battleOpenResp.mRespJsonStr;
		NetworkSystem::getSingleton().sendMsg(battleOpenResp,connId);

		return false;
	}

	assert(battleIndex + 6 <= 12);

	ServantBattleOpenTemplate * servantBattleOpen = SERVANTBATTLEOPEN_TABLE().get(battleIndex + 6);
	if (servantBattleOpen == NULL)
	{
		Json::Value root;
		root["errorId"] = LynxErrno::ClienServerDataNotMatch;

		Json::StyledWriter writer;
		GCServantBattleOpenResp battleOpenResp;
		battleOpenResp.mRespJsonStr = writer.write(root);
		battleOpenResp.mPacketID = BOC_SERVANTBATTLE_OPEN_RESP;
		NetworkSystem::getSingleton().sendMsg(battleOpenResp,connId);
		LOG_WARN("servantBattleOpen not found!!");
		return false;
	}

	UInt32 playerlv = m_pPlayer->getPlayerLeval();

	if(servantBattleOpen->mLevel > playerlv)
	{
		//提示玩家等级不足

		Json::Value root;
		root["errorId"] = LynxErrno::LevelNotEnough;
		
		Json::StyledWriter writer;

		//提示玩家助阵位已经开启
		GCServantBattleOpenResp battleOpenResp;
		battleOpenResp.mRespJsonStr = writer.write(root);
		battleOpenResp.mPacketID = BOC_SERVANTBATTLE_OPEN_RESP;

		cout << battleOpenResp.mRespJsonStr;
		NetworkSystem::getSingleton().sendMsg(battleOpenResp,connId);

		return false;
	}

	UInt64 gold = m_pPlayer->getPlayerGold();
	
	//消耗元宝，判断元宝是否数量不足
	if(gold < servantBattleOpen->mCost)
	{
		//提示玩家元宝不足

		Json::Value root;
		root["errorId"] = LynxErrno::RmbNotEnough;

		Json::StyledWriter writer;

		//提示玩家助阵位已经开启
		GCServantBattleOpenResp battleOpenResp;
		battleOpenResp.mRespJsonStr = writer.write(root);
		battleOpenResp.mPacketID = BOC_SERVANTBATTLE_OPEN_RESP;

		cout << battleOpenResp.mRespJsonStr;
		NetworkSystem::getSingleton().sendMsg(battleOpenResp,connId);

		return false;
	}

	m_pServantBattleData->assistBattleBit |= m_bitVec[battleIndex + 6];


	m_pPlayer->getPersistManager().setDirtyBit(SERVANTBATTLEBIT);

	Goods goods;
	List<Goods> itemList;

	goods.resourcestype =AWARD_BASE;
	goods.subtype = AWARD_BASE_GOLD;
	goods.num = 0 -servantBattleOpen->mCost;
	itemList.insertTail(goods);
	GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog21);


	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["assistIndex"] = battleIndex;
    root["rmb"] = m_pPlayer->getPlayerGold();
    root["quality"] = 1;
	root["slot"] = 0;

	Json::StyledWriter writer;

	m_pPlayer->getAchieveManager().updateAchieveData(UNLOCKSERVANTASSIS,1);
	//提示玩家助阵位已经开启
	GCServantBattleOpenResp battleOpenResp;
	battleOpenResp.mRespJsonStr = writer.write(root);
	battleOpenResp.mPacketID = BOC_SERVANTBATTLE_OPEN_RESP;

	cout << battleOpenResp.mRespJsonStr;
	NetworkSystem::getSingleton().sendMsg(battleOpenResp,connId);
	
	return true;
}

//battleindex 从0到6
bool ServantManager::assistbattleEnhance(UInt32 battleIndex)
{
	const ConnId& connId = m_pPlayer->getConnId();

	
	//从0到6
	assert(battleIndex <= 6);
	//SERVANT_TABLE
	bool openState = getOpenState(battleIndex + 6);
	
	if(!openState)
	{
		//提示玩家阵容未开启
		GCAssistBattleEnhanceResp enhanceResp;
		enhanceResp.mPacketID = BOC_ASSISTBATTLE_ENHANCE_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ServantBattleLock;
		Json::StyledWriter writer;
		
		enhanceResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(enhanceResp, connId);

		cout << enhanceResp.mRespJsonStr;

		return false;
	}
	
	List<AssistBattleData>::Iter * servantBattleIter = m_pServantBattleData->assistServantBattle.getn(battleIndex);
	
	if(!servantBattleIter)
	{
		//提示玩家参数不正确

		GCAssistBattleEnhanceResp enhanceResp;
		enhanceResp.mPacketID = BOC_ASSISTBATTLE_ENHANCE_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;
		Json::StyledWriter writer;

		enhanceResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(enhanceResp, connId);

		cout << enhanceResp.mRespJsonStr;


		return false;
	}

	

	//反向查找,位置从1到7
	ServantAssistBattleTemplate* assistBattleTemp = SERVANTASSISTBATTLE_TABLE().reverseGet(battleIndex + 1, servantBattleIter->mValue.quality);

	if(!assistBattleTemp)
	{
		//提示玩家参数错误
		GCAssistBattleEnhanceResp enhanceResp;
		enhanceResp.mPacketID = BOC_ASSISTBATTLE_ENHANCE_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;
		Json::StyledWriter writer;

		enhanceResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(enhanceResp, connId);

		cout << enhanceResp.mRespJsonStr;


		return false;
	}

	if(!assistBattleTemp->mCost)
	{
		//提示玩家达到最高强化等级
		GCAssistBattleEnhanceResp enhanceResp;
		enhanceResp.mPacketID = BOC_ASSISTBATTLE_ENHANCE_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::BattleQualityLvLimit;
		Json::StyledWriter writer;

		enhanceResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(enhanceResp, connId);

		cout << enhanceResp.mRespJsonStr;


		return false;
	}

	//强化石不足
	if(m_pServantData->battleStone < assistBattleTemp->mCost)
	{
		//提示玩家强化石头不足
		GCAssistBattleEnhanceResp enhanceResp;
		enhanceResp.mPacketID = BOC_ASSISTBATTLE_ENHANCE_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::MaterialNotEnough;
		Json::StyledWriter writer;

		enhanceResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(enhanceResp, connId);

		cout << enhanceResp.mRespJsonStr;


		return false;
	}
	
	//消耗石头，并且阵容品质等级增加一级
	m_pServantData->battleStone -= assistBattleTemp->mCost;

	servantBattleIter->mValue.quality ++;

	if(servantBattleIter->mValue.quality == 4)
	{
		m_pPlayer->getAchieveManager().updateAchieveData(OWNPURPLEASSIST,1);
	}

	m_pPlayer->getPersistManager().setDirtyBit(SERVANTBATTLEBIT|SERVANTFOODBIT);

	GCAssistBattleEnhanceResp enhanceResp;
	enhanceResp.mPacketID = BOC_ASSISTBATTLE_ENHANCE_RESP;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["assistIndex"] = battleIndex;
	root["stone"] = m_pServantData->battleStone;
	root["quality"] = servantBattleIter->mValue.quality;

	Json::StyledWriter writer;

	enhanceResp.mRespJsonStr = writer.write(root);

	NetworkSystem::getSingleton().sendMsg(enhanceResp, connId);

	cout << enhanceResp.mRespJsonStr;


	return true;
}

bool ServantManager::servantBattleSet(const Vector<UInt64> &totalBattle)
{
	const ConnId& connId = m_pPlayer->getConnId();

	assert(totalBattle.size() == 12);

	UInt32 battleIndex = 0;
	for(List<UInt64>::Iter * mainBattleIter = m_pServantBattleData->mainServantBattle.begin(); mainBattleIter != NULL;
		mainBattleIter = m_pServantBattleData->mainServantBattle.next(mainBattleIter) )
	{
		if(mainBattleIter->mValue == totalBattle[battleIndex])
		{
			battleIndex ++;
			continue;
		}

		if(totalBattle[battleIndex])
		{
			bool openstate = getOpenState(battleIndex + 1);
			if(!openstate)
			{
				//提示玩家未解锁
				Json::Value root;
				root["errorId"] = LynxErrno::ServantBattleLock;
				Json::StyledWriter writer;
				GCServantBattleSetResp setResp;
                setResp.mPacketID = BOC_SERVANTBATTLE_SET_RESP;
				setResp.mRespJsonStr = writer.write(root);
				cout << setResp.mRespJsonStr;
				NetworkSystem::getSingleton().sendMsg(setResp, connId);

				return false;
			}

			Map<UInt64 ,ServantData*>::Iter * servantDataIter = m_mapIdServant.find(totalBattle[battleIndex]);
			if(!servantDataIter)
			{
				//提示玩家不存在这个佣兵
				Json::Value root;
				root["errorId"] = LynxErrno::ServantNotExit;
				Json::StyledWriter writer;
				GCServantBattleSetResp setResp;
				setResp.mPacketID = BOC_SERVANTBATTLE_SET_RESP;
				setResp.mRespJsonStr = writer.write(root);
				cout << setResp.mRespJsonStr;
				NetworkSystem::getSingleton().sendMsg(setResp, connId);

				return false;
			}
			char dest[1024]={0};
			snprintf(dest,sizeof(dest),"%d,%d",battleIndex,totalBattle[battleIndex]);
			LogicSystem::getSingleton().write_log(LogType91,m_pPlayer->getPlayerGuid(), dest,LogInfo);
		}
		
		battleIndex ++;
		
	}

	for(List<AssistBattleData>::Iter * assistBattleIter = m_pServantBattleData->assistServantBattle.begin(); assistBattleIter != NULL;
		assistBattleIter = m_pServantBattleData->assistServantBattle.next(assistBattleIter))
	{
		if(assistBattleIter->mValue.equipAssist == totalBattle[battleIndex])
		{
			battleIndex ++;
			continue;
		}
		
		if(totalBattle[battleIndex])
		{
			bool openstate = getOpenState(battleIndex + 1);
			if(!openstate)
			{
				//提示玩家未解锁
				Json::Value root;
				root["errorId"] = LynxErrno::ServantBattleLock;
				Json::StyledWriter writer;
				GCServantBattleSetResp setResp;
				setResp.mPacketID = BOC_SERVANTBATTLE_SET_RESP;
				setResp.mRespJsonStr = writer.write(root);
				cout << setResp.mRespJsonStr;
				NetworkSystem::getSingleton().sendMsg(setResp, connId);

				return false;
			}

			Map<UInt64 ,ServantData*>::Iter * servantDataIter = m_mapIdServant.find(totalBattle[battleIndex]);
			if(!servantDataIter)
			{
				//提示玩家不存在这个佣兵

				//提示玩家未解锁
				Json::Value root;
				root["errorId"] = LynxErrno::ServantNotExit;
				Json::StyledWriter writer;
				GCServantBattleSetResp setResp;
				setResp.mPacketID = BOC_SERVANTBATTLE_SET_RESP;
				setResp.mRespJsonStr = writer.write(root);
				cout << setResp.mRespJsonStr;
				NetworkSystem::getSingleton().sendMsg(setResp, connId);

				return false;
			}

			char dest[1024]={0};
			snprintf(dest,sizeof(dest),"%d,%d",battleIndex,totalBattle[battleIndex]);
			LogicSystem::getSingleton().write_log(LogType94,m_pPlayer->getPlayerGuid(), dest,LogInfo);
		}

		battleIndex ++;
	}

	battleIndex = 0;

	Json::Value root;
	root["errorId"] = LynxErrno::None;

	for(List<UInt64>::Iter * mainBattleIter = m_pServantBattleData->mainServantBattle.begin(); mainBattleIter != NULL;
		mainBattleIter = m_pServantBattleData->mainServantBattle.next(mainBattleIter) )
	{
		mainBattleIter->mValue = totalBattle[battleIndex];
        root["totalBattle"].append(mainBattleIter->mValue);
		battleIndex ++;

	}

	for(List<AssistBattleData>::Iter * assistBattleIter = m_pServantBattleData->assistServantBattle.begin(); assistBattleIter != NULL;
		assistBattleIter = m_pServantBattleData->assistServantBattle.next(assistBattleIter))
	{
		assistBattleIter->mValue.equipAssist = totalBattle[battleIndex];
		root["totalBattle"].append(totalBattle[battleIndex]);
		battleIndex ++;

	}


	Json::StyledWriter writer;

	GCServantBattleSetResp setResp;
	setResp.mPacketID = BOC_SERVANTBATTLE_SET_RESP;
	setResp.mRespJsonStr = writer.write(root);
	cout << setResp.mRespJsonStr;
	NetworkSystem::getSingleton().sendMsg(setResp, connId);

	m_pPlayer->getPersistManager().setDirtyBit(SERVANTBATTLEBIT);
	//计算缘分
	//calculateLuck();
	return true;
}


bool ServantManager::assistBattleOneSet(const Vector<UInt64> &totalBattle)
{
	const ConnId& connId = m_pPlayer->getConnId();

	assert(totalBattle.size() == 12);

	UInt32 battleIndex = 0;
	for(List<UInt64>::Iter * mainBattleIter = m_pServantBattleData->mainServantBattle.begin(); mainBattleIter != NULL;
		mainBattleIter = m_pServantBattleData->mainServantBattle.next(mainBattleIter) )
	{
		//上阵位的id和请求id一样，跳过这个循环
		if(mainBattleIter->mValue == totalBattle[battleIndex])
		{
			battleIndex ++;
			continue;
		}

		if(totalBattle[battleIndex])
		{
			bool openstate = getOpenState(battleIndex + 1);
			if(!openstate)
			{
				//提示玩家未解锁
				Json::Value root;
				root["errorId"] = LynxErrno::ServantBattleLock;
				Json::StyledWriter writer;
				GCServantBattleSetResp setResp;
				setResp.mPacketID = BOC_SERVANTBATTLE_SET_RESP;
				setResp.mRespJsonStr = writer.write(root);
				cout << setResp.mRespJsonStr;
				NetworkSystem::getSingleton().sendMsg(setResp, connId);

				return false;
			}

			Map<UInt64 ,ServantData*>::Iter * servantDataIter = m_mapIdServant.find(totalBattle[battleIndex]);
			if(!servantDataIter)
			{
				//提示玩家不存在这个佣兵
				Json::Value root;
				root["errorId"] = LynxErrno::ServantNotExit;
				Json::StyledWriter writer;
				GCServantBattleSetResp setResp;
				setResp.mPacketID = BOC_SERVANTBATTLE_SET_RESP;
				setResp.mRespJsonStr = writer.write(root);
				cout << setResp.mRespJsonStr;
				NetworkSystem::getSingleton().sendMsg(setResp, connId);

				return false;
			}

			char dest[1024]={0};
			snprintf(dest,sizeof(dest),"%d,%d",battleIndex,totalBattle[battleIndex]);
			LogicSystem::getSingleton().write_log(LogType91,m_pPlayer->getPlayerGuid(), dest,LogInfo);
		}

		battleIndex ++;

	}

	for(List<AssistBattleData>::Iter * assistBattleIter = m_pServantBattleData->assistServantBattle.begin(); assistBattleIter != NULL;
		assistBattleIter = m_pServantBattleData->assistServantBattle.next(assistBattleIter))
	{
		//助阵位id和请求的一样，跳过这层循环
		if(assistBattleIter->mValue.equipAssist == totalBattle[battleIndex])
		{
			battleIndex ++;
			continue;
		}

		if(totalBattle[battleIndex])
		{
			bool openstate = getOpenState(battleIndex + 1);
			if(!openstate)
			{
				//提示玩家未解锁
				Json::Value root;
				root["errorId"] = LynxErrno::ServantBattleLock;
				Json::StyledWriter writer;
				GCServantBattleSetResp setResp;
				setResp.mPacketID = BOC_SERVANTBATTLE_SET_RESP;
				setResp.mRespJsonStr = writer.write(root);
				cout << setResp.mRespJsonStr;
				NetworkSystem::getSingleton().sendMsg(setResp, connId);

				return false;
			}

			Map<UInt64 ,ServantData*>::Iter * servantDataIter = m_mapIdServant.find(totalBattle[battleIndex]);
			if(!servantDataIter)
			{
				//提示玩家不存在这个佣兵

				//提示玩家未解锁
				Json::Value root;
				root["errorId"] = LynxErrno::ServantNotExit;
				Json::StyledWriter writer;
				GCServantBattleSetResp setResp;
				setResp.mPacketID = BOC_SERVANTBATTLE_SET_RESP;
				setResp.mRespJsonStr = writer.write(root);
				cout << setResp.mRespJsonStr;
				NetworkSystem::getSingleton().sendMsg(setResp, connId);

				return false;
			}
			char dest[1024]={0};
			snprintf(dest,sizeof(dest),"%d,%d",battleIndex,totalBattle[battleIndex]);
			LogicSystem::getSingleton().write_log(LogType94,m_pPlayer->getPlayerGuid(), dest,LogInfo);
		}

		battleIndex ++;
	}

	battleIndex = 0;

	Json::Value root;
	root["errorId"] = LynxErrno::None;

	for(List<UInt64>::Iter * mainBattleIter = m_pServantBattleData->mainServantBattle.begin(); mainBattleIter != NULL;
		mainBattleIter = m_pServantBattleData->mainServantBattle.next(mainBattleIter) )
	{
		mainBattleIter->mValue = totalBattle[battleIndex];
		root["totalBattle"].append(mainBattleIter->mValue);
		battleIndex ++;

	}

	for(List<AssistBattleData>::Iter * assistBattleIter = m_pServantBattleData->assistServantBattle.begin(); assistBattleIter != NULL;
		assistBattleIter = m_pServantBattleData->assistServantBattle.next(assistBattleIter))
	{
		assistBattleIter->mValue.equipAssist = totalBattle[battleIndex];
		root["totalBattle"].append(totalBattle[battleIndex]);
		battleIndex ++;

	}


	Json::StyledWriter writer;

	GCServantBattleSetResp setResp;
	setResp.mPacketID = BOC_SERVANTBATTLE_SET_RESP;
	setResp.mRespJsonStr = writer.write(root);
	cout << setResp.mRespJsonStr;
	NetworkSystem::getSingleton().sendMsg(setResp, connId);

	m_pPlayer->getPersistManager().setDirtyBit(SERVANTBATTLEBIT);
	//计算缘分
	//calculateLuck();
	return true;	
}

//判断一条缘分对应所有佣兵是否都在阵位上
bool ServantManager::luckServant(ServantLuckTemplate * servantLuckTemp, UInt64 baseServant)
{
	bool isActive = true;
	
	//判断佣兵，首先缘分需要的佣兵不为0，而且该佣兵不是当前佣兵
	if(servantLuckTemp->mServant1 && servantLuckTemp->mServant1 != baseServant)
	{
		isActive = isActiveLuck(servantLuckTemp->mServant1);
	}

	if(!isActive)
	{
		return isActive;
	}

	if(servantLuckTemp->mServant2 && servantLuckTemp->mServant2 != baseServant)
	{
		isActive = isActiveLuck(servantLuckTemp->mServant2);
	}

	if(!isActive)
	{
		return isActive;
	}

	if(servantLuckTemp->mServant3 && servantLuckTemp->mServant3 != baseServant)
	{
		isActive = isActiveLuck(servantLuckTemp->mServant3);
	}

	if(!isActive)
	{
		return isActive;
	}

	if(servantLuckTemp->mServant4 && servantLuckTemp->mServant4 != baseServant)
	{
		isActive = isActiveLuck(servantLuckTemp->mServant4);
	}

	if(!isActive)
	{
		return isActive;
	}

	if(servantLuckTemp->mServant5 && servantLuckTemp->mServant5 != baseServant)
	{
		isActive = isActiveLuck(servantLuckTemp->mServant5);
	}

	if(!isActive)
	{
		return isActive;
	}

	return true;
}


void ServantManager::judgeMainBattleLuck()
{
	for(List<UInt64>::Iter * mainBattleIter = m_pServantBattleData->mainServantBattle.begin(); mainBattleIter != NULL;
		mainBattleIter = m_pServantBattleData->mainServantBattle.next(mainBattleIter) )
	{
		//未开启或者未上阵，那么跳过这层循环
		if(!mainBattleIter->mValue)
		{
			continue;
		}

		ServantTemplate * servantTemp = SERVANT_TABLE().get(mainBattleIter->mValue);

		//遍历该京剧猫可能的所有缘分
		for(List<UInt64>::Iter * luckyIter = servantTemp->luckyList.begin(); luckyIter != NULL; 
			luckyIter = servantTemp->luckyList.next(luckyIter))
		{
			Vector<UInt64>::iterator saveLuckIter = find(m_luckVec.begin(), m_luckVec.end(), luckyIter->mValue);

			//该缘分已经存在，那么跳过这层循环
			if(saveLuckIter != m_luckVec.end())
			{
				continue;
			}

			//不存在这个缘分，判断这个缘分需要的佣兵是否都上阵
			ServantLuckTemplate * servantLuckTemp = SERVANTLUCK_TABLE().get(luckyIter->mValue);

			//bool变量初始为真，表示该缘分激活，实际根据判断，设置
			bool isActive = true;

			isActive = luckServant(servantLuckTemp,mainBattleIter->mValue);

			if(isActive)
			{
				//该缘分需要的佣兵都已经上阵，该缘分激活,放入vec中
				m_luckVec.push_back(luckyIter->mValue);
			}


		}

	}
}


void ServantManager::judgeAssistBattleLuck()
{
	for(List<AssistBattleData>::Iter * assistBattleIter = m_pServantBattleData->assistServantBattle.begin(); assistBattleIter != NULL;
		assistBattleIter = m_pServantBattleData->assistServantBattle.next(assistBattleIter) )
	{
		//未开启或者未上阵，那么跳过这层循环
		if(!(assistBattleIter->mValue.equipAssist))
		{
			continue;
		}

		ServantTemplate * servantTemp = SERVANT_TABLE().get(assistBattleIter->mValue.equipAssist);

		//遍历该京剧猫可能的所有缘分
		for(List<UInt64>::Iter * luckyIter = servantTemp->luckyList.begin(); luckyIter != NULL; 
			luckyIter = servantTemp->luckyList.next(luckyIter))
		{
			Vector<UInt64>::iterator saveLuckIter = find(m_luckVec.begin(), m_luckVec.end(), luckyIter->mValue);

			//该缘分已经存在，那么跳过这层循环
			if(saveLuckIter != m_luckVec.end())
			{
				continue;
			}

			//不存在这个缘分，判断这个缘分需要的佣兵是否都上阵
			ServantLuckTemplate * servantLuckTemp = SERVANTLUCK_TABLE().get(luckyIter->mValue);

			//bool变量初始为真，表示该缘分激活，实际根据判断，设置
			bool isActive = true;

			isActive = luckServant(servantLuckTemp,assistBattleIter->mValue.equipAssist);

			if(isActive)
			{
				//该缘分需要的佣兵都已经上阵，该缘分激活,放入vec中
				m_luckVec.push_back(luckyIter->mValue);
			}


		}

	}
}

void ServantManager::calculateLuck()
{
	m_luckVec.clear();
	
	judgeMainBattleLuck();
	judgeAssistBattleLuck();

}

bool ServantManager::isActiveLuck(UInt64 servantId)
{
	List<UInt64> ::Iter * findServantIter = m_pServantBattleData->mainServantBattle.find(servantId,m_pServantBattleData->mainServantBattle.begin(),m_pServantBattleData->mainServantBattle.end());
	
	if(findServantIter)
	{
		return true;
	}


	for(List<AssistBattleData>::Iter * assistBattleIter = m_pServantBattleData->assistServantBattle.begin(); assistBattleIter != NULL;
		assistBattleIter = m_pServantBattleData->assistServantBattle.next(assistBattleIter) )
	{
		if(assistBattleIter->mValue.equipAssist == servantId)
		{
			return true;
		}
	}


	//需要的佣兵没上阵，说明该缘分未激活，剩下的也不需要判断了
	return false;
		
}

void ServantManager::calculateFinalRate()
{
	//下面计算A位置概率加成
	UInt32 cdeRates = m_rateVec[2] + m_rateVec[3] + m_rateVec[4];
	UInt32 baseRate = (m_rateVec[0] > cdeRates )?  cdeRates : m_rateVec[0];
	//A位置概率加成
	UInt32 aAddRate = baseRate * 0.8;

	//加成后A的概率变为
	m_rateVec[0] += aAddRate;

	//CDE进行衰减
	//C衰减后概率变为
	m_rateVec[2] -= (aAddRate * m_rateVec[2]/(cdeRates));
	//D衰减后概率变为
	m_rateVec[3] -= (aAddRate * m_rateVec[3]/(cdeRates));

	//E衰减后概率变为
	m_rateVec[4] -= (aAddRate * m_rateVec[4]/(cdeRates));


	//计算B位置概率加成
	cdeRates = m_rateVec[2] + m_rateVec[3] + m_rateVec[4];
	baseRate = (m_rateVec[1]  > cdeRates) ? cdeRates:m_rateVec[1];
	//B位置概率加成
	UInt32 bAddRate = baseRate * 0.4;
	//加成后B的概率变为
	m_rateVec[1] += bAddRate;

	//CDE进行概率进行衰减
	//C衰减后概率变为
	m_rateVec[2] -= (bAddRate * m_rateVec[2]/(cdeRates));
	//D衰减后概率变为
	m_rateVec[3] -= (bAddRate * m_rateVec[3]/(cdeRates));
	//E衰减后概率变为
	m_rateVec[4] -= (bAddRate * m_rateVec[4]/(cdeRates));
}

void ServantManager::calculateServantRate()
{
	UInt32 index = 0;
	
	for(List<UInt64>::Iter * mainBattleIter = m_pServantBattleData->mainServantBattle.begin(); mainBattleIter != NULL;
		mainBattleIter = m_pServantBattleData->mainServantBattle.next(mainBattleIter) )
	{
		//该阵位上英雄id非0
		if(mainBattleIter->mValue)
		{
			ServantTemplate * servantTemp = SERVANT_TABLE().get(mainBattleIter->mValue);
			//找到英雄对应的rate
			if(servantTemp)
			{
				//放入对应的vector里
				m_rateVec[index] = servantTemp->mRate/100;
			}
		}
		

		index ++;
	}

	UInt32 totalRate = 0;
	for(UInt32 i = 0; i < m_rateVec.size(); i++)
	{
		totalRate += m_rateVec[i];
	}

	if(totalRate > 100)
	{
		//第一次衰减，将总概率缩小到100%
		//多出来的概率
		UInt32 moreRate = totalRate - 100;
		if(moreRate)
		{
			UInt32 cdeRate = m_rateVec[2] + m_rateVec[3] + m_rateVec[4];
			//衰减c,d,e三个位置的概率
			if(cdeRate)
			{
				//C位置衰减的概率
				UInt32 CRateless = (m_rateVec[2]/cdeRate) * moreRate;

				//D位置衰减概率
				UInt32 DRateless = (m_rateVec[3]/cdeRate) * moreRate;

				//E位置衰减概率
				UInt32 ERateless = (m_rateVec[4]/cdeRate) * moreRate;

				//C位置概率变为
				m_rateVec[2] -= CRateless;
				//D位置概率变为
				m_rateVec[3] -= DRateless;
				//E位置概率变为
				m_rateVec[4] -= ERateless;

			}
		
			calculateFinalRate();
			
		}

	}
	else
	{
		calculateFinalRate();
	}
}

UInt32 ServantManager::calculateFightCapacity()
{
	return calculateAssistCapacity() + calculateLuckCapacity() + calculateServantsCapacity();
}


UInt32 ServantManager::calculateAssistCapacity()
{
	//读表获得单个助阵位战力加成

	UInt32 index = 1;
	UInt32 fightCapacity = 0;
	for(List<AssistBattleData>::Iter * assistIter = m_pServantBattleData->assistServantBattle.begin(); assistIter != NULL;
		assistIter = m_pServantBattleData->assistServantBattle.next(assistIter) )
	{
		ServantAssistBattleTemplate * assistBattleTemp = SERVANTASSISTBATTLE_TABLE().reverseGet(index,assistIter->mValue.quality);
		//读表获得单个阵位的战力

		Map<UInt64 ,ServantData*>::Iter * servantDataIter = m_mapIdServant.find(assistIter->mValue.equipAssist);
		if(servantDataIter)
		{
			fightCapacity += ((assistBattleTemp->mPower) * (servantDataIter->mValue->star)/10.0 + assistBattleTemp->mPower);
		}
		else
		{
			fightCapacity += assistBattleTemp->mPower;
		}

		index++;
	}

	return fightCapacity;
}



void ServantManager::attrConverToCapacity(UInt32 & fightCapacity,UInt32 attrType, UInt32 attrValue)
{
	Map<UInt32, double>::Iter * attrIter = m_attrPercentMap.find(attrType);
	if(attrIter)
	{
		fightCapacity += attrValue * attrIter->mValue;
	}
}

UInt32 ServantManager::calculateLuckCapacity()
{

	UInt32 fightCapacity = 0;
	for(UInt32 i = 0; i < m_luckVec.size(); i++)
	{
		ServantLuckTemplate * servantLuckTemp = SERVANTLUCK_TABLE().get(m_luckVec[i]);
		
		if(!servantLuckTemp)
		{
			continue;
		}

		if(servantLuckTemp->mAttrType1)
		{
			attrConverToCapacity(fightCapacity,servantLuckTemp->mAttrType1,servantLuckTemp->mAttrValue1);
		}

		if(servantLuckTemp->mAttrType2)
		{
			attrConverToCapacity(fightCapacity,servantLuckTemp->mAttrType2,servantLuckTemp->mAttrValue2);
		}

		if(servantLuckTemp->mAttrType3)
		{
			attrConverToCapacity(fightCapacity,servantLuckTemp->mAttrType3,servantLuckTemp->mAttrValue3);
		}
	}

	return fightCapacity;
}

UInt32 ServantManager::calculateServantsCapacity()
{
	UInt32 fightCapacity = 0;

	for(Map<UInt64 ,ServantData*>::Iter* servantDataIter = m_mapIdServant.begin(); servantDataIter != NULL; 
		servantDataIter = m_mapIdServant.next(servantDataIter) )
	{
		if(servantDataIter->mValue->star && servantDataIter->mValue->level)
		{
			fightCapacity += calculateServantCapacity(servantDataIter->mValue);
		}
		
	}

	return fightCapacity;
}


void ServantManager::calculateServantsAddAttr(PlayerAttrData & playerAttrData)
{
	for(Map<UInt64 ,ServantData*>::Iter* servantDataIter = m_mapIdServant.begin(); servantDataIter != NULL; 
		servantDataIter = m_mapIdServant.next(servantDataIter) )
	{
		if(servantDataIter->mValue->star && servantDataIter->mValue->level)
		{
			calculateServantAddAttr(playerAttrData,servantDataIter->mValue);
		}

	}

}

void ServantManager::calculateServantAddAttr(PlayerAttrData & playerAttrData,ServantData * servantData)
{
	ServantStarTemplate * servantStarTemp = SERVANTSTAR_TABLE().reverseGet(servantData->servantId,servantData->star);
	
	if(servantStarTemp)
	{
		playerAttrData.addAttrTypeValue(servantStarTemp->mAttrType, servantStarTemp->mAttrValue);
	}
	
	

	ServantFloorTemplate * servantFloorTemp = SERVANTFLOOR_TABLE().reverseGet(servantData->servantId,servantData->floor);

	if(servantFloorTemp)
	{
		if(servantFloorTemp->mAttrType1)
		{
			playerAttrData.addAttrTypeValue(servantFloorTemp->mAttrType1, servantFloorTemp->mAttrValue1);
		}

		if(servantFloorTemp->mAttrType2)
		{
			playerAttrData.addAttrTypeValue(servantFloorTemp->mAttrType2, servantFloorTemp->mAttrValue2);
		}


		if(servantFloorTemp->mAttrType3)
		{
			playerAttrData.addAttrTypeValue(servantFloorTemp->mAttrType3, servantFloorTemp->mAttrValue3);
		}


		if(servantFloorTemp->mAttrType4)
		{
			playerAttrData.addAttrTypeValue(servantFloorTemp->mAttrType4, servantFloorTemp->mAttrValue4);
		}


		if(servantFloorTemp->mAttrType5)
		{
			playerAttrData.addAttrTypeValue(servantFloorTemp->mAttrType5, servantFloorTemp->mAttrValue5);
		}
	}

	



	for(List<UInt64>::Iter * treasureIter = servantData->equipTreasures.begin(); treasureIter != NULL; 
		treasureIter =  servantData->equipTreasures.next(treasureIter))
	{
		if(treasureIter->mValue)
		{
			ServantTreasureTemplate * servantTreasureTemp = SERVANTTREASURE_TABLE().get(treasureIter->mValue);

			if(!servantTreasureTemp)
			{
				continue;
			}

			if(servantTreasureTemp->mAttrType1)
			{
				playerAttrData.addAttrTypeValue( servantTreasureTemp->mAttrType1, servantTreasureTemp->mAttrValue1);
			}

			if(servantTreasureTemp->mAttrType2)
			{
				playerAttrData.addAttrTypeValue( servantTreasureTemp->mAttrType2, servantTreasureTemp->mAttrValue2);
			}

			if(servantTreasureTemp->mAttrType3)
			{
				playerAttrData.addAttrTypeValue( servantTreasureTemp->mAttrType3, servantTreasureTemp->mAttrValue3);
			}
		}
	}
}


UInt32 ServantManager::calculateServantCapacity(ServantData * servantData)
{
	UInt32 fightCapacity = 0;

	ServantStarTemplate * servantStarTemp = SERVANTSTAR_TABLE().reverseGet(servantData->servantId,servantData->star);

	if(servantStarTemp)
	{
		fightCapacity += servantStarTemp->mPower;
	}
	

	ServantFloorTemplate * servantFloorTemp = SERVANTFLOOR_TABLE().reverseGet(servantData->servantId,servantData->floor);

	if(servantFloorTemp)
	{
		fightCapacity += servantFloorTemp->mPower;
	}
	

	for(List<UInt64>::Iter * treasureIter = servantData->equipTreasures.begin(); treasureIter != NULL; 
		treasureIter =  servantData->equipTreasures.next(treasureIter))
	{
		if(treasureIter->mValue)
		{
			ServantTreasureTemplate * servantTreasureTemp = SERVANTTREASURE_TABLE().get(treasureIter->mValue);
			if(servantTreasureTemp)
			{
				fightCapacity += servantTreasureTemp->mPower;
			}
			
		}
	}


	return fightCapacity;
}


void ServantManager::calculteAttrAdd(PlayerAttrData & playerAttrData)
{
	//阵位提供属性加成
	UInt32 index = 1;
	for(List<AssistBattleData>::Iter * assistIter = m_pServantBattleData->assistServantBattle.begin(); assistIter != NULL;
		assistIter = m_pServantBattleData->assistServantBattle.next(assistIter) )
	{
		ServantAssistBattleTemplate * assistBattleTemp = SERVANTASSISTBATTLE_TABLE().reverseGet(index,assistIter->mValue.quality);
		if(assistBattleTemp)
		{
			//读表获得单个阵位的战力
			playerAttrData.addAttrTypeValue(assistBattleTemp->mAttrType, assistBattleTemp->mAttrValue);
		}

		index++;
	}

	//情谊属性加成
	for(UInt32 i = 0; i < m_luckVec.size(); i++)
	{
		ServantLuckTemplate * servantLuckTemp = SERVANTLUCK_TABLE().get(m_luckVec[i]);
		if (servantLuckTemp == NULL)
		{
			LOG_WARN("servantLuckTemp not found!!");
			return;
		}

		if(servantLuckTemp->mAttrType1)
		{
			playerAttrData.addAttrTypeValue(servantLuckTemp->mAttrType1, servantLuckTemp->mAttrValue1);
		}

		if(servantLuckTemp->mAttrType2)
		{
			playerAttrData.addAttrTypeValue(servantLuckTemp->mAttrType2, servantLuckTemp->mAttrValue2);
		}

		if(servantLuckTemp->mAttrType3)
		{
			playerAttrData.addAttrTypeValue(servantLuckTemp->mAttrType3, servantLuckTemp->mAttrValue3);
		}
	}

	//计算所有佣兵提供的属性
	calculateServantsAddAttr(playerAttrData);
}


void ServantManager::getServantListJson(List<ServantData> * servantList,Json::Value &root)
{


	for(List<ServantData>::Iter * servantIter = servantList->begin(); servantIter != NULL; 
		servantIter = servantList->next(servantIter))
	{
		Json::Value servantRoot;
		servantRoot["servantUid"] = servantIter->mValue.servantUid;
		servantRoot["servantId"] = servantIter->mValue.servantId;
		servantRoot["pieceCount"] = servantIter->mValue.pieceCount;
		servantRoot["level"] = servantIter->mValue.level;
		servantRoot["lvexp"] = servantIter->mValue.lvexp;
		servantRoot["star"] = servantIter->mValue.star;
		servantRoot["floor"] = servantIter->mValue.floor;

		for(List<UInt64>::Iter * treasureIter = servantIter->mValue.equipTreasures.begin(); treasureIter != NULL;
			treasureIter = servantIter->mValue.equipTreasures.next(treasureIter))
		{
			servantRoot["equipTreasures"].append(treasureIter->mValue);
		}



		root["servantData"].append(servantRoot);
	}
}


void ServantManager::GMsetServantFood(UInt64 foodType, UInt64 foodCount)
{
		switch(foodType)
		{
		case 1:
			{
				m_pServantData->food1count = foodCount;
				m_pPlayer->getPersistManager().setDirtyBit(SERVANTFOODBIT);
			}
			break;
		case 2:
			{
				m_pServantData->food2count = foodCount;
				m_pPlayer->getPersistManager().setDirtyBit(SERVANTFOODBIT);
			}
			break;
		case 3:
			{
				m_pServantData->food3count = foodCount;
				m_pPlayer->getPersistManager().setDirtyBit(SERVANTFOODBIT);
			}
			break;
		case 4:
			{
				m_pServantData->food4count = foodCount;
				m_pPlayer->getPersistManager().setDirtyBit(SERVANTFOODBIT);
			}
			break;
		case 5:
			{
				m_pServantData->food5count = foodCount;
				m_pPlayer->getPersistManager().setDirtyBit(SERVANTFOODBIT);
			}
			break;
		default:
			break;
		}
}