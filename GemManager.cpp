#include "GemManager.h"
#include "Player.h"
#include "LogicSystem.h"
using namespace Lynx;


GemManager::GemManager()
{
	m_pPlayer = NULL;
	m_pGemDataList = NULL;
}

GemManager:: ~GemManager()
{

}



bool GemManager::initial(Player* player)
{
	if (player == NULL)
	{
		LOG_WARN("Player pointer is NULL.");
		return false;
	}

	m_pPlayer = player;

	m_pGemDataList = &player->mPlayerData.mGemData.mGemLists;

	for(List<GemData>::Iter * gemDataIter = m_pGemDataList->begin(); gemDataIter != NULL; 
		gemDataIter = m_pGemDataList->next(gemDataIter))
	{
		Map<UInt64, GemData*>::Iter* gemMapIter = m_gemDataMap.insert(gemDataIter->mValue.m_nId, &gemDataIter->mValue);
		
		addGemTypeBaseCount(gemDataIter->mValue.m_nId, gemDataIter->mValue.m_nCount);
		createGemTypeLvToDataMap(gemDataIter->mValue.m_nId,gemMapIter);
	}

	

	return true;
}

void GemManager::release()
{
	m_pPlayer = NULL;
	m_pGemDataList = NULL;
	m_gemDataMap.clear();
	m_gemTypeLvMap.clear();
	m_gemTypeToBaseCount.clear();
	
}

void GemManager::createGemTypeLvToDataMap(UInt64 gemID, Map<UInt64, GemData*>::Iter* gemMapIter)
{
	NewGemTemplate * newGemTemp = NEWGEM_TABLE().get(gemID);

	if(!newGemTemp)
	{
		LOG_WARN("Invalid gemId!");
		return;
	}

	Map<UInt32, Map<UInt32, GemData*> >::Iter * typeLvMapIter = m_gemTypeLvMap.find(newGemTemp->mEquipType);
	
	if(typeLvMapIter)
	{
		Map<UInt32, GemData*>::Iter* gemDataIter = typeLvMapIter->mValue.find(newGemTemp->mLevel);
		
		if(gemDataIter)
		{
			LOG_WARN("CreateGemTypeLvMap error! gem has exist in this map!!");
			//assert(false);
		}
		else
		{
			typeLvMapIter->mValue.insert(newGemTemp->mLevel, gemMapIter->mValue);
		}
	}
	else
	{
		Map<UInt32, GemData*> lvGemDataMap;
		lvGemDataMap.insert(newGemTemp->mLevel, gemMapIter->mValue);
	
		m_gemTypeLvMap.insert(newGemTemp->mEquipType, lvGemDataMap);
	}
	
	
}

void GemManager::playerGetGems(UInt64 gemID, UInt32 count)
{
	addGems(gemID,count);
	addGemTypeBaseCount(gemID, count);
	
}

bool GemManager::playerConsumeGems(UInt64 gemID, UInt32 count)
{
	bool delRes = delGems(gemID,count);
	
	if(delRes)
	{
		delGemTypeBaseCount(gemID, count);
		return true;
	}
	else
	{
		return false;
	}
	
}

UInt32 GemManager::getGems(UInt64 gemID)
{
	return getGem(gemID);
}


UInt32 GemManager::getGem(UInt64 gemID)
{

	Map<UInt64, GemData*>::Iter * gemDataMapIter = m_gemDataMap.find(gemID);

	if(gemDataMapIter)
	{
		return gemDataMapIter->mValue->m_nCount;
	}
	else
	{
		return 0;
	}
}


GemData* GemManager::addGems(UInt64 gemID, UInt32 count)
{
	//更新成就
	NewGemTemplate * newGemTemp = NEWGEM_TABLE().get(gemID);
	assert(newGemTemp);
	m_pPlayer->getAchieveManager().updateAchieveData(MAXGEMLV,newGemTemp->mLevel);
	
	Map<UInt64, GemData*>::Iter * gemDataMapIter = m_gemDataMap.find(gemID);
	
	if(gemDataMapIter)
	{
		gemDataMapIter->mValue->m_nCount += count;
		gemDataMapIter->mValue->m_nDirtyType = UPDATEDIRTY;
		m_pPlayer->getPersistManager().setDirtyBit(GEMDATABIT);

		return gemDataMapIter->mValue;
	
	}
	else
	{
		
		
		if(!newGemTemp)
		{
			LOG_WARN("Invalid gemid %llu", gemID);
			 assert(false);
		}

		GemData gemData;
		gemData.m_nId = gemID;
		gemData.m_nCount = count;

		Guid gemGuid = LogicSystem::getSingleton().generateMaterialGuid();
		gemData.m_nUid = gemGuid;
		gemData.m_nDirtyType = UPDATEDIRTY;

		List<GemData>::Iter * insertIter = m_pGemDataList->insertTail(gemData);
		Map<UInt64, GemData*>::Iter * gemDataMapIter = m_gemDataMap.insert(gemID, &insertIter->mValue);
		createGemTypeLvToDataMap(gemID,gemDataMapIter);
		m_pPlayer->getPersistManager().setDirtyBit(GEMDATABIT);

		return &insertIter->mValue;
	}

	
}

bool GemManager::delGems(UInt64 gemID, UInt32 count)
{
	Map<UInt64, GemData *>::Iter * gemDataIter = m_gemDataMap.find(gemID);
	if(gemDataIter)
	{
		if(gemDataIter->mValue->m_nCount >= count)
		{
			gemDataIter->mValue->m_nCount -= count;
			gemDataIter->mValue->m_nDirtyType = UPDATEDIRTY;

			m_pPlayer->getPersistManager().setDirtyBit(GEMDATABIT);
			
			return true;
		}
		else
		{
			return false;
		}

	}
	else
	{	
		return false;
	}
}

Map<UInt32, GemData*> *GemManager::getSameTypeGems(UInt32 gemType)
{
	Map<UInt32, Map<UInt32, GemData*> >::Iter * typeLvGemIter = m_gemTypeLvMap.find(gemType);
	
	if(typeLvGemIter)
	{
		return &typeLvGemIter->mValue;
	}
	else
	{
		return  NULL;
	}
}

GemData* GemManager::getGemData(UInt64 gemItemID)
{
	Map<UInt64, GemData*>::Iter * gemDataIter = m_gemDataMap.find(gemItemID);
	
	if(gemDataIter)
	{
		return  gemDataIter->mValue;
	}
	else
	{
		return NULL;
	}
	
}

std::string GemManager::convertDataToJson()
{
	Json::Value root;
	for(Map<UInt64, GemData*>::Iter * gemDataIter = m_gemDataMap.begin(); gemDataIter != NULL; 
		gemDataIter = m_gemDataMap.next(gemDataIter))
	{
		Json::Value gemDataRoot;
		gemDataRoot["itemId"] = gemDataIter->mValue->m_nId;
		gemDataRoot["stackAmount"] = gemDataIter->mValue->m_nCount;

		root.append(gemDataRoot);
	}
	
	Json::StyledWriter writer;
	
	return writer.write(root);
}

List<GemData> * GemManager:: getGemDataList(void)
{
	return m_pGemDataList;
}

void GemManager::addGemTypeBaseCount(UInt64 gemID, UInt32 gemCount)
{
	GemCombineOnceTemplate * gemCombineOnceTemp = GEMCOMBINEONCE_TABLE().get(gemID);

	if(!gemCombineOnceTemp)
	{
		LOG_WARN("Invalid gemId");
		return;
	}

	Map<UInt32, UInt32>::Iter * typeBaseCountIter = m_gemTypeToBaseCount.find(gemCombineOnceTemp->mType);

	if(typeBaseCountIter)
	{
		typeBaseCountIter->mValue += gemCombineOnceTemp->mCount * gemCount;
	}
	else
	{	 
		m_gemTypeToBaseCount.insert(gemCombineOnceTemp->mType,gemCombineOnceTemp->mCount * gemCount);
	}
}

void GemManager::delGemTypeBaseCount(UInt64 gemID, UInt32 gemCount)
{
	GemCombineOnceTemplate * gemCombineOnceTemp = GEMCOMBINEONCE_TABLE().get(gemID);

	if(!gemCombineOnceTemp)
	{
		LOG_WARN("Invalid gemId");
		return;
	}

	Map<UInt32, UInt32>::Iter * typeBaseCountIter = m_gemTypeToBaseCount.find(gemCombineOnceTemp->mType);

	if(typeBaseCountIter)
	{
		typeBaseCountIter->mValue -= gemCombineOnceTemp->mCount * gemCount;
	}
	else
	{	 
		return ;
	}
}

void GemManager::sendRespToClient(List<UInt64> &newGemList,  errorId  errorId)
{
	Json::Value tmpValue;
	if(errorId != LynxErrno::None)
	{
		GCGemCombineResp gemCombineResp;
		gemCombineResp.mPacketID = BOC_GEM_COMBINE_RESP;

		Json::Value root;
		root["errorId"] = errorId;
		Json::StyledWriter writer;
		
		gemCombineResp.mRespJsonStr = writer.write(root);
		
		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(gemCombineResp,connId);

		//cout << gemCombineResp.mRespJsonStr ;
		
	}
	else
	{
		GCGemCombineResp gemCombineResp;
		gemCombineResp.mPacketID = BOC_GEM_COMBINE_RESP;

		Json::Value root;
		root["errorId"] = LynxErrno::None;
		
		for(List<UInt64>::Iter * gemIdIter = newGemList.begin(); gemIdIter != NULL; gemIdIter = newGemList.next(gemIdIter))
		{
			Json::Value gemRoot;
			Map<UInt64, GemData*>::Iter *gemDataIter = m_gemDataMap.find(gemIdIter->mValue);

			if(gemDataIter)
			{
				gemRoot["itemId"] = gemDataIter->mValue->m_nId;
				gemRoot["stackAmount"] = gemDataIter->mValue->m_nCount;

				root["gems"].append(gemRoot);
			}

			tmpValue = gemRoot;
			
		}

		Json::StyledWriter writer;

		gemCombineResp.mRespJsonStr = writer.write(root);

		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(gemCombineResp,connId);
		

		cout << gemCombineResp.mRespJsonStr;
	}
}

void GemManager::combineGemsList(UInt64 gemItemID,UInt32 count)
{
	//表示有改动的宝石列表，新生成或者减少的
	List<UInt64> newGemList;
	
	GemCombineTemplate * gemCombineTemplate = GEMCOMBINE_TABLE().get(gemItemID);
	if(!gemCombineTemplate)
	{
		//提示玩家参数有误
		sendRespToClient(newGemList,LynxErrno::InvalidParameter);
		return;
	}

	UInt32 newGemCount = count/gemCombineTemplate->mAmount;

	if(gemCombineTemplate->mNextGem)
	{
		bool delRes = delGems(gemItemID,count);
		if(!delRes)
		{
			//提示玩家参数有误
			sendRespToClient(newGemList,LynxErrno::InvalidParameter);
			return ;
		}
		
		addGems(gemCombineTemplate->mNextGem, newGemCount);

		newGemList.insertTail(gemCombineTemplate->mNextGem);
		newGemList.insertTail(gemItemID);

		//发送成功的数据包给客户端
		sendRespToClient(newGemList,LynxErrno::None);
		return ;
	}
	else
	{
		//提示玩家宝石达到最高等级
		sendRespToClient(newGemList,LynxErrno::GemLevelLimit);
		return ;
	}
}

//目的宝石gemitemid，要合成目的宝石的数量
void GemManager::combineOneTypeGems(UInt64 gemItemID, UInt32 count)
{
	//表示有改动的宝石列表，新生成或者减少的
	List<UInt64> newGemList;

	//目的宝石表元素
	GemCombineTemplate * gemCombineTemplate = GEMCOMBINE_TABLE().get(gemItemID);

	if(!gemCombineTemplate)
	{
		//提示玩家宝石参数有错误，此宝石不可合成即为1级
		sendRespToClient(newGemList,LynxErrno::InvalidParameter);
		return ;
	}

	//合成目的宝石所需前一种宝石的总数量 = 目的宝石数量*前一种宝石数量；
	UInt32 materialGemCount = count * gemCombineTemplate->mPreAmount;

	if(gemCombineTemplate->mPreGem)
	{
		bool delRes = delGems(gemCombineTemplate->mPreGem,materialGemCount);
		if(!delRes)
		{
			//提示玩家参数有误
			sendRespToClient(newGemList,LynxErrno::InvalidParameter);
			return ;
		}

		addGems(gemItemID, count);

		newGemList.insertTail(gemCombineTemplate->mPreGem);
		newGemList.insertTail(gemItemID);

		//更新宝石等级成就
		NewGemTemplate * newGemTemp = NEWGEM_TABLE().get(gemItemID);
		m_pPlayer->getAchieveManager().updateAchieveData(MAXGEMLV,newGemTemp->mLevel);

		//更新七日训
		LogicSystem::getSingleton().updateSevenDayTask(m_pPlayer->getPlayerGuid(),SDT03,1);

		//发送成功的数据包给客户端
		sendRespToClient(newGemList,LynxErrno::None);
		return ;
	}
	else
	{
		//提示玩家宝石参数有错误，此宝石不可合成即为1级
		sendRespToClient(newGemList,LynxErrno::InvalidParameter);
		return ;
	}
}

void GemManager::gemCombineOnce(void)
{
	removeAllGems();

	for(Map<UInt32, UInt32>::Iter * typeBaseCountIter = m_gemTypeToBaseCount.begin(); typeBaseCountIter != NULL; 
		typeBaseCountIter = m_gemTypeToBaseCount.next(typeBaseCountIter))
	{
		UInt32 gemType = typeBaseCountIter->mKey;
		UInt32 gemCount = typeBaseCountIter->mValue;
		
		if(!gemCount)
		{
			continue;
		}

		for(UInt32 level = 15; level > 0; level--)
		{
			//该类型和等级对应的数量和id结构体
			GemIdAndCount gemIdAndCount  = GEMCOMBINEONCE_TABLE().getIdAndCount(gemType,level);

			if(gemIdAndCount.mID == 0 && gemIdAndCount.mCount == 0)
			{
				continue;
			}

			NewGemTemplate * newGemTemp = NEWGEM_TABLE().get(gemIdAndCount.mID);

			assert(newGemTemp);
			if(!newGemTemp)
			{
				LOG_WARN("Invalid gemId %llu",gemIdAndCount.mID);
				continue;
			}


			int newGemCounts = 0;
			while(gemCount >= gemIdAndCount.mCount)
			{
				newGemCounts++;
				gemCount -= gemIdAndCount.mCount;

			}

			if(newGemCounts)
			{
				Map<UInt64, GemData*>::Iter * gemDataMapIter = m_gemDataMap.find(gemIdAndCount.mID);

				if(gemDataMapIter)
				{
					gemDataMapIter->mValue->m_nCount += newGemCounts;
					gemDataMapIter->mValue->m_nDirtyType = UPDATEDIRTY;
					m_pPlayer->getPersistManager().setDirtyBit(GEMDATABIT);

				}
				else
				{
									
					GemData gemData;
					gemData.m_nId = gemIdAndCount.mID;
					gemData.m_nCount = newGemCounts;

					Guid gemGuid = LogicSystem::getSingleton().generateMaterialGuid();
					gemData.m_nUid = gemGuid;
					gemData.m_nDirtyType = UPDATEDIRTY;

					List<GemData>::Iter * insertIter = m_pGemDataList->insertTail(gemData);
					Map<UInt64, GemData*>::Iter * gemDataMapIter = m_gemDataMap.insert(gemIdAndCount.mID, &insertIter->mValue);
					
					createGemTypeLvToDataMap(gemIdAndCount.mID,gemDataMapIter);
					m_pPlayer->getPersistManager().setDirtyBit(GEMDATABIT);
				}
			}

			
		}
	}


	GCGemListResp gemListResp;
	gemListResp.mPacketID = BOC_GEM_LIST_RESP;
	gemListResp.mRespJsonStr = convertDataToJson();
	cout << gemListResp.mRespJsonStr;
	const ConnId& connId = m_pPlayer->getConnId();

	NetworkSystem::getSingleton().sendMsg(gemListResp,connId);

}

//原来合成，可重用
//void GemManager::gemCombineOnce(UInt32 equipPos)
//{
//	const ConnId& connId = m_pPlayer->getConnId();
//
//	UInt32  playerLv = m_pPlayer->getPlayerLeval();
//
//	Map<UInt32, GemData*> * sameGemMap = getSameTypeGems(equipPos);
//
//	for(Map<UInt32, GemData*>::Iter*  gemDataIter = sameGemMap->begin();gemDataIter != NULL; 
//		gemDataIter = sameGemMap->next(gemDataIter))
//	{
//		gemDataIter->mValue->m_nCount = 0;
//		gemDataIter->mValue->m_nDirtyType = UPDATEDIRTY;
//	}
//
//	
//	Map<UInt32, UInt32>::Iter * typeBaseCountIter = m_gemTypeToBaseCount.find(equipPos);
//
//	if(!typeBaseCountIter)
//	{
//		GCGemCombineOnceResp combineOnceResp;
//		combineOnceResp.mPacketID = BOC_GEM_COMBINEONCE_RESP;
//		Json::Value root;
//		root["errorId"] = LynxErrno::InvalidParameter;
//		Json::FastWriter writer;
//		
//		combineOnceResp.mRespJsonStr = writer.write(root);
//
//		NetworkSystem::getSingleton().sendMsg(combineOnceResp,connId);
//
//		return;
//
//	}
//	
//	UInt32 gemCount = typeBaseCountIter->mValue;
//
//	if(!gemCount)
//	{
//		GCGemCombineOnceResp combineOnceResp;
//		combineOnceResp.mPacketID = BOC_GEM_COMBINEONCE_RESP;
//		Json::Value root;
//		root["errorId"] = LynxErrno::GemNotExist;
//		Json::FastWriter writer;
//
//		combineOnceResp.mRespJsonStr = writer.write(root);
//
//		NetworkSystem::getSingleton().sendMsg(combineOnceResp,connId);
//		return;
//	}
//
//		for(UInt32 level = 15; level > 0; level--)
//		{
//			//该类型和等级对应的数量和id结构体
//			GemIdAndCount gemIdAndCount  = GEMCOMBINEONCE_TABLE().getIdAndCount(equipPos,level);
//
//			if(gemIdAndCount.mID == 0 && gemIdAndCount.mCount == 0)
//			{
//				continue;
//			}
//
//			NewGemTemplate * newGemTemp = NEWGEM_TABLE().get(gemIdAndCount.mID);
//
//			assert(newGemTemp);
//			if(!newGemTemp)
//			{
//				LOG_WARN("Invalid gemId %llu",gemIdAndCount.mID);
//				continue;
//			}
//
//			//玩家等级不足宝石合成等级
//			if(playerLv < newGemTemp->mLvRequire)
//			{
//				continue;
//			}
//
//			int newGemCounts = 0;
//			while(gemCount >= gemIdAndCount.mCount)
//			{
//				newGemCounts++;
//				gemCount -= gemIdAndCount.mCount;
//
//			}
//
//			if(newGemCounts)
//			{
//				Map<UInt64, GemData*>::Iter * gemDataMapIter = m_gemDataMap.find(gemIdAndCount.mID);
//
//				if(gemDataMapIter)
//				{
//					gemDataMapIter->mValue->m_nCount += newGemCounts;
//					gemDataMapIter->mValue->m_nDirtyType = UPDATEDIRTY;
//					m_pPlayer->getPersistManager().setDirtyBit(GEMDATABIT);
//
//				}
//				else
//				{
//					GemData gemData;
//					gemData.m_nId = gemIdAndCount.mID;
//					gemData.m_nCount = newGemCounts;
//
//					Guid gemGuid = LogicSystem::getSingleton().generateMaterialGuid();
//					gemData.m_nUid = gemGuid;
//					gemData.m_nDirtyType = UPDATEDIRTY;
//
//					List<GemData>::Iter * insertIter = m_pGemDataList->insertTail(gemData);
//				
//					Map<UInt64, GemData*>::Iter * gemDataMapIter = m_gemDataMap.insert(gemIdAndCount.mID, &insertIter->mValue);
//
//					createGemTypeLvToDataMap(gemIdAndCount.mID,gemDataMapIter);
//					m_pPlayer->getPersistManager().setDirtyBit(GEMDATABIT);
//				}
//			}
//
//
//		}
//
//		GCGemCombineOnceResp combineOnceResp;
//		combineOnceResp.mPacketID = BOC_GEM_COMBINEONCE_RESP;
//		Json::Value root;
//		root["errorId"] = LynxErrno::None;
//
//		for(Map<UInt32, GemData*>::Iter*  gemDataIter = sameGemMap->begin();gemDataIter != NULL; 
//			gemDataIter = sameGemMap->next(gemDataIter))
//		{
//			Json::Value gemRoot;
//			gemRoot["itemId"] = gemDataIter->mValue->m_nId ;
//			gemRoot["stackAmount"] = gemDataIter->mValue->m_nCount;
//
//			root["gems"].append(gemRoot);
//		}
//
//
//		root["equipPos"] = equipPos;
//
//		Json::StyledWriter writer;
//		
//		combineOnceResp.mRespJsonStr = writer.write(root);
//		//cout << combineOnceResp.mRespJsonStr;
//		NetworkSystem::getSingleton().sendMsg(combineOnceResp,connId);
//	
//
//	
//
//}



void GemManager::gemCombineOnce(UInt32 equipPos)
{
	const ConnId& connId = m_pPlayer->getConnId();

	UInt32  playerLv = m_pPlayer->getPlayerLeval();

	Map<UInt32, GemData*> * sameGemMap = getSameTypeGems(equipPos);
	
	if(!sameGemMap)
	{
		GCGemCombineOnceResp combineOnceResp;
		combineOnceResp.mPacketID = BOC_GEM_COMBINEONCE_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;
		Json::FastWriter writer;

		combineOnceResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(combineOnceResp,connId);

		return;
	}

	//可合成宝石的基数
	UInt32 combineCount = 0;

	for(Map<UInt32, GemData*>::Iter*  gemDataIter = sameGemMap->begin();gemDataIter != NULL; 
		gemDataIter = sameGemMap->next(gemDataIter))
	{
		NewGemTemplate * newGemTemp = NEWGEM_TABLE().get(gemDataIter->mValue->m_nId);

		if(!newGemTemp)
		{
			continue;
		}
		
		//大于玩家等级的宝石保存起来
		if(newGemTemp->mLvRequire > playerLv)
		{
			
		}
		else
		{
			//统计该类型该等级宝石的个数
			UInt32 typeLvCount = gemDataIter->mValue->m_nCount;
			
			//小于等于玩家的宝石打碎，并且计数

			gemDataIter->mValue->m_nCount = 0;
			//根据位置和等级找到该宝石对应的id，和转化为1级宝石的数量
			GemIdAndCount gemIdAndCount  = GEMCOMBINEONCE_TABLE().getIdAndCount(equipPos,gemDataIter->mKey);
			
			if(gemIdAndCount.mID && gemIdAndCount.mCount)
			{
				combineCount += gemIdAndCount.mCount * typeLvCount;
			}

			
			gemDataIter->mValue->m_nDirtyType = UPDATEDIRTY;

		}

		
	}


	Map<UInt32, UInt32>::Iter * typeBaseCountIter = m_gemTypeToBaseCount.find(equipPos);

	if(!typeBaseCountIter)
	{
		GCGemCombineOnceResp combineOnceResp;
		combineOnceResp.mPacketID = BOC_GEM_COMBINEONCE_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;
		Json::FastWriter writer;

		combineOnceResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(combineOnceResp,connId);

		return;

	}

	

	if(!combineCount)
	{
		GCGemCombineOnceResp combineOnceResp;
		combineOnceResp.mPacketID = BOC_GEM_COMBINEONCE_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::GemNotExist;
		Json::FastWriter writer;

		combineOnceResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(combineOnceResp,connId);
		return;
	}

	for(UInt32 level = 15; level > 0; level--)
	{
		//该类型和等级对应的数量和id结构体
		GemIdAndCount gemIdAndCount  = GEMCOMBINEONCE_TABLE().getIdAndCount(equipPos,level);

		if(gemIdAndCount.mID == 0 && gemIdAndCount.mCount == 0)
		{
			continue;
		}

		NewGemTemplate * newGemTemp = NEWGEM_TABLE().get(gemIdAndCount.mID);

		assert(newGemTemp);
		if(!newGemTemp)
		{
			LOG_WARN("Invalid gemId %llu",gemIdAndCount.mID);
			continue;
		}

		//玩家等级不足宝石合成等级
		if(playerLv < newGemTemp->mLvRequire)
		{
			continue;
		}

		int newGemCounts = 0;
		while(combineCount >= gemIdAndCount.mCount)
		{
			newGemCounts++;
			combineCount -= gemIdAndCount.mCount;

		}

		if(newGemCounts)
		{
			Map<UInt64, GemData*>::Iter * gemDataMapIter = m_gemDataMap.find(gemIdAndCount.mID);

			if(gemDataMapIter)
			{
				gemDataMapIter->mValue->m_nCount += newGemCounts;
				gemDataMapIter->mValue->m_nDirtyType = UPDATEDIRTY;
				m_pPlayer->getPersistManager().setDirtyBit(GEMDATABIT);

			}
			else
			{
				GemData gemData;
				gemData.m_nId = gemIdAndCount.mID;
				gemData.m_nCount = newGemCounts;

				Guid gemGuid = LogicSystem::getSingleton().generateMaterialGuid();
				gemData.m_nUid = gemGuid;
				gemData.m_nDirtyType = UPDATEDIRTY;

				List<GemData>::Iter * insertIter = m_pGemDataList->insertTail(gemData);

				Map<UInt64, GemData*>::Iter * gemDataMapIter = m_gemDataMap.insert(gemIdAndCount.mID, &insertIter->mValue);

				createGemTypeLvToDataMap(gemIdAndCount.mID,gemDataMapIter);
				m_pPlayer->getPersistManager().setDirtyBit(GEMDATABIT);
			}
		}


	}

	GCGemCombineOnceResp combineOnceResp;
	combineOnceResp.mPacketID = BOC_GEM_COMBINEONCE_RESP;
	Json::Value root;
	root["errorId"] = LynxErrno::None;

	for(Map<UInt32, GemData*>::Iter*  gemDataIter = sameGemMap->begin();gemDataIter != NULL; 
		gemDataIter = sameGemMap->next(gemDataIter))
	{
		Json::Value gemRoot;
		gemRoot["itemId"] = gemDataIter->mValue->m_nId ;
		gemRoot["stackAmount"] = gemDataIter->mValue->m_nCount;

		root["gems"].append(gemRoot);

		//更新宝石等级成就
		NewGemTemplate * newGemTemp = NEWGEM_TABLE().get(gemDataIter->mValue->m_nId);
		m_pPlayer->getAchieveManager().updateAchieveData(MAXGEMLV,newGemTemp->mLevel);
	}


	root["equipPos"] = equipPos;
	root["enhancetype"] = 1;

	Json::StyledWriter writer;

	combineOnceResp.mRespJsonStr = writer.write(root);
	//cout << combineOnceResp.mRespJsonStr;
	NetworkSystem::getSingleton().sendMsg(combineOnceResp,connId);


	//更新七日训
	LogicSystem::getSingleton().updateSevenDayTask(m_pPlayer->getPlayerGuid(),SDT03,1);


}


void GemManager::removeAllGems()
{
	for(Map<UInt64, GemData*>::Iter * gemDataIter = m_gemDataMap.begin(); gemDataIter != NULL; 
		gemDataIter = m_gemDataMap.next(gemDataIter))
	{
		gemDataIter->mValue->m_nCount = 0;
		gemDataIter->mValue->m_nDirtyType = UPDATEDIRTY;
	}

}

