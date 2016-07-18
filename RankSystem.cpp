#include "RankSystem.h"
#include "GameServer.h"
#include "LogicSystem.h"

using namespace Lynx;

RankSystem::RankSystem(){}

RankSystem::~RankSystem()
{}

bool RankSystem::initial()
{
	m_nLastSyncTime = 0;
	LOG_INFO("Initial RankSystem success!!!!");
	clearPowerRank();
	return true;
}

void RankSystem::release()
{
	m_onLineLvMap.clear();
}

void 
RankSystem::update(const UInt64& accTime)
{

	//600000ms 测试
	if(accTime - m_nLastSyncTime >= 600000)
	{
		//上次同步时间更新
		m_nLastSyncTime = accTime;
		
		PerisistUpdateOnlineLvRank updateOnlineRank;
		updateOnlineRank.m_nTime = m_nLastSyncTime;
		PersistSystem::getSingleton().postThreadMsg(updateOnlineRank, m_nLastSyncTime);

		PersistGetPowerRank getPowerRank;
		getPowerRank.m_nTime = m_nLastSyncTime;
		PersistSystem::getSingleton().postThreadMsg(getPowerRank, m_nLastSyncTime);
		
	}

}

void RankSystem::clearPowerRank()
{
	m_first50List.clear();
	m_powerRankMap.clear();
}

void RankSystem::creatPowerRank(UInt64 playerUid, UInt32 rank)
{
	m_powerRankMap.insert(playerUid, rank);
	m_first50List.insertTail(playerUid);
}


void RankSystem::createOnlineLvs(const List<BaseInfoData> &playerInfoList)
{
	m_onLineLvMap.clear();

	
	for(List<BaseInfoData>::Iter * baseInfoIter = playerInfoList.begin();  baseInfoIter != NULL;  
		baseInfoIter = playerInfoList.next(baseInfoIter))
	{
		Map<UInt32, Set<UInt64> >::Iter * lvFind = m_onLineLvMap.find(baseInfoIter->mValue.level);
		
		if(lvFind)
		{
			lvFind->mValue.insert(baseInfoIter->mValue.playerUid);
			
		}
		else
		{
			Set<UInt64> uuidSet;
			uuidSet.insert(baseInfoIter->mValue.playerUid);
			m_onLineLvMap.insert(baseInfoIter->mValue.level, uuidSet);
		}
	}

}

void RankSystem::getOnlinePlayers(UInt32 srcLv, Player * selfPlayer, List<BaseInfoData>&getList)
{
	Map<UInt32, Set<UInt64> >::Iter * mapIterFind = m_onLineLvMap.find(srcLv);
	if(!mapIterFind)
	{
		return;
	}
	UInt32 count = 30;
	getPlayerFromSet(mapIterFind->mValue, count, selfPlayer, getList);

	UInt32 order = rand()%2;
	//order 顺序
	if(getList.size() >= count)
	{
		return;
	}

	UInt32 needCount = count - getList.size();

	//正向取
	if(!order)
	{
		for(Map<UInt32, Set<UInt64> >::Iter * mapIterLoop = m_onLineLvMap.next(mapIterFind); 
			mapIterLoop != NULL;  mapIterLoop =  m_onLineLvMap.next(mapIterLoop) )
		{
			//找到了指定数量的玩家
			if(needCount <= 0)
			{
				return;
			}

			getPlayerFromSet(mapIterLoop->mValue, needCount, selfPlayer, getList);

			needCount = count - getList.size();
		}

		for(Map<UInt32, Set<UInt64> >::Iter * mapIterLoop = m_onLineLvMap.prev(mapIterFind); 
			mapIterLoop != NULL;  mapIterLoop =  m_onLineLvMap.prev(mapIterLoop) )
		{
			if(needCount <= 0)
			{
				return;
			}

			getPlayerFromSet(mapIterLoop->mValue, needCount, selfPlayer, getList);

			needCount = count - getList.size();
		}


	}
	else
	{
		//反向取

		for(Map<UInt32, Set<UInt64> >::Iter * mapIterLoop = m_onLineLvMap.prev(mapIterFind); 
			mapIterLoop != NULL;  mapIterLoop =  m_onLineLvMap.prev(mapIterLoop) )
		{
			if(needCount <= 0)
			{
				return;
			}

			getPlayerFromSet(mapIterLoop->mValue, needCount, selfPlayer, getList);

			needCount = count - getList.size();
		}


		for(Map<UInt32, Set<UInt64> >::Iter * mapIterLoop = m_onLineLvMap.next(mapIterFind); 
			mapIterLoop != NULL;  mapIterLoop =  m_onLineLvMap.next(mapIterLoop) )
		{
			//找到了指定数量的玩家
			if(needCount <= 0)
			{
				return;
			}

			getPlayerFromSet(mapIterLoop->mValue, needCount, selfPlayer, getList);

			needCount = count - getList.size();
		}

	}

	
}

void RankSystem::getPlayerFromSet(const Set<UInt64> &playerSet, UInt32 count, Player * selfPlayer,  List<BaseInfoData> &getList)
{
	UInt32 setSize = playerSet.size();
	UInt64 selfUid = selfPlayer->getPlayerGuid();
	//set大小小于要查找的count
	if(setSize <= count)
	{
		for(Set<UInt64>::Iter * setIter = playerSet.begin();  setIter != NULL;  
			setIter = playerSet.next(setIter))
		{
			bool isFriend = selfPlayer->getFriendBlackManager().judgeFriend(setIter->mValue);
			if(isFriend)
			{
				continue;
			}
		
			bool isBlack = selfPlayer->getFriendBlackManager().judgeBlack(setIter->mValue);
			if(isBlack)
			{
				continue;
			}

			if(selfUid == setIter->mValue)
			{
				continue;
			}

			BaseInfoData baseInfoData;
			LogicSystem::getSingleton().getBaseInfo(setIter->mValue, baseInfoData);
			getList.insertTail(baseInfoData);
		}

		return ;
	}
	
	//set大小大于要查找的count
	if(setSize > count )
	{
		UInt32 diffSize = setSize - count + 1;
		UInt32 offset = rand()%diffSize;
		//正向取还是反向取
		UInt32 order = rand()%2;
		if(order)
		{
				//正向取
				UInt32 index = 0;
				UInt32 insertCount = 0;
				for(Set<UInt64>::Iter * setIter = playerSet.begin();  setIter != NULL;  
					setIter = playerSet.next(setIter))
				{
					if(index != offset)
					{
						index++;
						continue;
					}

					bool isFriend = selfPlayer->getFriendBlackManager().judgeFriend(setIter->mValue);
					if(isFriend)
					{
						continue;
					}

					bool isBlack = selfPlayer->getFriendBlackManager().judgeBlack(setIter->mValue);
					if(isBlack)
					{
						continue;
					}

					if(selfUid == setIter->mValue)
					{
						continue;
					}

					BaseInfoData baseInfoData;
					LogicSystem::getSingleton().getBaseInfo(setIter->mValue, baseInfoData);
					getList.insertTail(baseInfoData);
					insertCount++;
					if(insertCount == count)
					{
						return ;
					}
				}
		}
		else
		{
				//反向取

				UInt32 index = 0;
				UInt32 insertCount = 0;
				for(Set<UInt64>::Iter * setIter = playerSet.r_begin();  setIter != NULL;  
					setIter = playerSet.prev(setIter))
				{
					if(index != offset)
					{
						index++;
						continue;
					}

					bool isFriend = selfPlayer->getFriendBlackManager().judgeFriend(setIter->mValue);
					if(isFriend)
					{
						continue;
					}

					bool isBlack = selfPlayer->getFriendBlackManager().judgeBlack(setIter->mValue);
					if(isBlack)
					{
						continue;
					}

					if(selfUid == setIter->mValue)
					{
						continue;
					}

					BaseInfoData baseInfoData;
					LogicSystem::getSingleton().getBaseInfo(setIter->mValue, baseInfoData);
					getList.insertTail(baseInfoData);
					insertCount++;
					if(insertCount == count)
					{
						return ;
					}
				}

		}
	}
	return ;
}


std::string  RankSystem::sendPowerRankJson(UInt64 playerUid)
{
	
	
	Map<UInt64, UInt32>::Iter * powerMapIter = m_powerRankMap.find(playerUid);

	if(!powerMapIter)
	{
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;
		Json::StyledWriter writer;
		return writer.write(root);
		
	}

	BaseInfoData selfInfoData;
	LogicSystem::getSingleton().getBaseInfo(playerUid, selfInfoData);

	Json::Value root;
	root["selfdata"]["playeruid"] = selfInfoData.playerUid;
	root["selfdata"]["modelid"] = selfInfoData.modelId;
	root["selfdata"]["name"] = selfInfoData.name.c_str();
	root["selfdata"]["lvl"] = selfInfoData.level;
    root["selfdata"]["power"] = selfInfoData.power;
    root["selfdata"]["rank"] = powerMapIter->mValue;
    root["selfdata"]["online"] = selfInfoData.leaveTime;


	UInt32 playerCount = 0;
	for(List<UInt64>::Iter * uidIter = m_first50List.begin(); uidIter != NULL; uidIter = m_first50List.next(uidIter))
	{
		playerCount++;
	
		BaseInfoData baseInfoData;
		LogicSystem::getSingleton().getBaseInfo(uidIter->mValue, baseInfoData);

		Json::Value  dataRoot;
		dataRoot["playeruid"] = baseInfoData.playerUid;
		dataRoot["modelid"] = baseInfoData.modelId;
		dataRoot["name"] = baseInfoData.name.c_str();
		dataRoot["lvl"] = baseInfoData.level;
		dataRoot["power"] = baseInfoData.power;
		dataRoot["rank"] = playerCount;
		dataRoot["online"] = baseInfoData.leaveTime;

		root["ranklist"].append(dataRoot);

		if(playerCount == 50)
		{
			break;
		}
	}

	

	Json::StyledWriter writer;
	return writer.write(root);

	

}

