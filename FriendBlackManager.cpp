#include "FriendBlackManager.h"
#include "Player.h"
#include "ConfigParser.h"
#include "LogicSystem.h"
#include "PersistSystem.h"
#include "ConsortSystem/ConsortSystem.h"
using namespace Lynx;

FriendBlackManager::FriendBlackManager()
{
	m_pPlayer = NULL;
	m_pFriendData = NULL;
	m_pFriendList = NULL;
	m_pBlackList = NULL;
}


FriendBlackManager:: ~FriendBlackManager()
{

}

bool FriendBlackManager::initial(Player* player)
{
	if (player == NULL)
	{
		LOG_WARN("Player pointer is NULL.");
		return false;
	}

	m_pPlayer = player;
	m_nSelfUid = m_pPlayer->getPlayerGuid();
	m_pFriendData = &player->mPlayerData.mFriendData;
	m_pFriendList =  &m_pFriendData->mFriendList;
	m_pBlackList = &m_pFriendData->mBlackList;



	for(List<UInt64>::Iter *beApplyIter = m_pFriendData->mBeApplyList.begin(); beApplyIter != m_pFriendData->mBeApplyList.end(); 
		beApplyIter = m_pFriendData->mBeApplyList.next(beApplyIter))
	{
		if (beApplyIter->mValue == 0)
		{
			continue;
		}
		BaseInfoData baseInfoData;
		bool res = LogicSystem::getSingleton().getBaseInfo(beApplyIter->mValue, baseInfoData);
		//没找到 
		if(!res)
		{
			//没找到，从数据库里load数据，并且插入Logic管理的map

			bool dbRes = ConsortSystem::getSingleton().getBaseInfoFromDb(beApplyIter->mValue, baseInfoData);

			//没找到，则跳过此次循环
			if(!dbRes)
			{
				continue;
			}
			//sql找到对应条目，放入管理map里
			LogicSystem::getSingleton().insertBaseInfo(baseInfoData);

		}
			m_pFriendBeApplyList.insertTail(baseInfoData);
	}

	return true;

}

void FriendBlackManager::release()
{
	m_pPlayer = NULL;
	m_pFriendData = NULL;
	m_pFriendList = NULL;
	m_pBlackList = NULL;
	m_pFriendBeApplyList.clear();
}

List<UInt64> &FriendBlackManager::getFriendList()
{
	return m_pFriendData->mFriendList;
}

List<UInt64> &FriendBlackManager::getBlackList()
{
	return m_pFriendData->mBlackList;
}

UInt32 FriendBlackManager::getFriendCount()
{
	return m_pFriendList->size();
}

UInt32 FriendBlackManager::getBlackCount()
{
	return m_pBlackList->size();
}


bool FriendBlackManager::delFriend(UInt64 playerUid)
{

	List<UInt64>::Iter *  findIter = m_pFriendList->find(playerUid, m_pFriendList->begin(),m_pFriendList->end() );
	if(findIter)
	{
		m_pFriendData->mFriendList.erase(findIter);

		notifySaveDb(m_nSelfUid);

		return true;
	}

	return false;
	
}

bool FriendBlackManager::addFriend(UInt64 playerUid)
{
	List<UInt64>::Iter *  findIter = m_pFriendList->find(playerUid, m_pFriendList->begin(),m_pFriendList->end() );
	if(findIter)
	{
		return false;
	}

	m_pFriendList->insertTail(playerUid);

	notifySaveDb(m_nSelfUid);

	return true;
}


bool FriendBlackManager::delBlack(UInt64 playerUid)
{
	List<UInt64>::Iter *  findIter = m_pBlackList->find(playerUid,m_pBlackList->begin(), m_pBlackList->end());
	if(findIter)
	{
		m_pBlackList->erase(findIter);
		notifySaveDb(m_nSelfUid);
		return true;
	}
	
	

	return false;
}

bool FriendBlackManager::addBlack(UInt64 playerUid)
{
	List<UInt64>::Iter *  findIter = m_pBlackList->find(playerUid,m_pBlackList->begin(), m_pBlackList->end());
	if(findIter)
	{
		return false;
	}

	m_pBlackList->insertTail(playerUid);

	notifySaveDb(m_nSelfUid);

	return true;
}


void FriendBlackManager::notifySaveDb(UInt64 playerUid)
{
	PersistUpdateFriendNotify updateFriendNotify;
	updateFriendNotify.m_friendData = *m_pFriendData;
	updateFriendNotify.m_nPlayerUid = playerUid;
	PersistSystem::getSingleton().postThreadMsg(updateFriendNotify,  playerUid);
}

std::string FriendBlackManager::convertDataToJson()
{
		/*GCFriendBlackListResp backListResp;
		backListResp.mPacketID = BOC_FRIENDBLACKLIST_RESP;*/
	Json::Value root;
	Json::StyledWriter writer;
	
	for(List<UInt64>::Iter *friendIter = m_pFriendList->begin(); friendIter != m_pFriendList->end(); 
		friendIter = m_pFriendList->next(friendIter))
	{
		BaseInfoData baseInfoData;
		bool res = LogicSystem::getSingleton().getBaseInfo(friendIter->mValue, baseInfoData);
		//没找到
		if(!res)
		{
			//没找到，从数据库里load数据，并且插入Logic管理的map
			
			bool dbRes = ConsortSystem::getSingleton().getBaseInfoFromDb(friendIter->mValue, baseInfoData);
			
			//没找到，则跳过此次循环
			if(!dbRes)
			{
				continue;
			}
			//sql找到对应条目，放入管理map里
			LogicSystem::getSingleton().insertBaseInfo(baseInfoData);

		}

		
		Json::Value friendEle;
		friendEle["playeruid"]= friendIter->mValue;
		friendEle["modelid"] = baseInfoData.modelId;
		friendEle["name"] = baseInfoData.name.c_str();
		friendEle["power"] = baseInfoData.power;
		friendEle["viplv"] = baseInfoData.vipLv;
		friendEle["level"] = baseInfoData.level;
		friendEle["leaveTime"] = baseInfoData.leaveTime/1000;
		
		root["friends"].append(friendEle);
		
	}


	for(List<UInt64>::Iter *blackIter = m_pBlackList->begin(); blackIter != m_pBlackList->end(); 
		blackIter = m_pBlackList->next(blackIter))
	{
		
		BaseInfoData baseInfoData;
		bool res = LogicSystem::getSingleton().getBaseInfo(blackIter->mValue, baseInfoData);
		//没找到
		if(!res)
		{
			//没找到，从数据库里load数据，并且插入Logic管理的map

			
			bool dbRes = ConsortSystem::getSingleton().getBaseInfoFromDb(blackIter->mValue, baseInfoData);
			//没找到，则跳过此次循环
			if(!dbRes)
			{
					continue;
			}
			LogicSystem::getSingleton().insertBaseInfo(baseInfoData);
		}


		Json::Value blackEle;
		blackEle["playeruid"]= blackIter->mValue;
		blackEle["modelid"] = baseInfoData.modelId;
		blackEle["name"] = baseInfoData.name.c_str();
		blackEle["power"] = baseInfoData.power;
		blackEle["viplv"] = baseInfoData.vipLv;
		blackEle["level"] = baseInfoData.level;
		blackEle["leaveTime"] = baseInfoData.leaveTime/1000;

		root["blacks"].append(blackEle);
	}


		for(List<BaseInfoData>::Iter *applyIter = m_pFriendBeApplyList.begin(); applyIter !=NULL; applyIter = m_pFriendBeApplyList.next(applyIter))
		{

			Json::Value beApplyList;
			beApplyList["playeruid"]= applyIter->mValue.playerUid;
			beApplyList["modelid"] =  applyIter->mValue.modelId;
			beApplyList["name"] =  applyIter->mValue.name.c_str();
			beApplyList["power"] =  applyIter->mValue.power;
			beApplyList["viplv"] =  applyIter->mValue.vipLv;
			beApplyList["level"] =  applyIter->mValue.level;
// 			beApplyList["leaveTime"] =  applyIter->mValue.leaveTime;
			Player *player = LogicSystem::getSingleton().getPlayerByGuid(applyIter->mValue.playerUid);
			if (player ==  NULL)
			{
				beApplyList["leaveTime"] = 1;
			}
			else
			{
				beApplyList["leaveTime"] = 0;
			}

			root["friendBeApplyList"].append(beApplyList);
		}

	
	std::string jsonStr = writer.write(root);
	//cout << jsonStr;
	return jsonStr;

}





bool FriendBlackManager::judgeFriend(UInt64 playerUid)
{
	List<UInt64>::Iter *  findIter = m_pFriendList->find(playerUid, m_pFriendList->begin(),m_pFriendList->end() );
	if(findIter)
	{
		

		return true;
	}

	return false;
}




bool FriendBlackManager::judgeBlack(UInt64 playerUid)
{
	List<UInt64>::Iter *  findIter = m_pBlackList->find(playerUid, m_pBlackList->begin(),m_pBlackList->end() );
	if(findIter)
	{


		return true;
	}

	return false;
}




bool FriendBlackManager::addFriendBeApply(BaseInfoData baseInfoData)
{
	if (m_pFriendBeApplyList.size() >= 50)
	{
		return false;
	}

		for (List<BaseInfoData>::Iter * iter = m_pFriendBeApplyList.begin();iter!=NULL; iter = m_pFriendBeApplyList.next(iter))
		{
			if (iter->mValue.playerUid == baseInfoData.playerUid)
			{
				return false;
			}
		}

	
	m_pFriendBeApplyList.insertTail(baseInfoData);

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(baseInfoData.playerUid);

	player->getPersistManager().setDirtyBit(FRIENDBEAPPLYDATABIT);
	
	return true;
}




bool FriendBlackManager::delFriendBeApply(UInt64 playerUid)
{

		for (List<BaseInfoData>::Iter  * iter = m_pFriendBeApplyList.begin();iter!=NULL; iter = m_pFriendBeApplyList.next(iter))
		{
			if (iter->mValue.playerUid == playerUid)
			{
				m_pFriendBeApplyList.erase(iter);
				Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
				if (player== NULL)
				{
					return false;
				}

				player->getPersistManager().setDirtyBit(FRIENDBEAPPLYDATABIT);

				
				return true;
			}
		}




	return false;
}
bool FriendBlackManager::delFriendBeAllApply()
{

	m_pFriendBeApplyList.clear();
	
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(m_nSelfUid);

	player->getPersistManager().setDirtyBit(FRIENDBEAPPLYDATABIT);


	return true;
}

List<BaseInfoData>& FriendBlackManager::getFriendApplyList()
{
	return m_pFriendBeApplyList;
	
}

bool FriendBlackManager::judgeOtherBeApply(UInt64 playerUid)
{
	if (m_pFriendBeApplyList.size() >= 50)
	{
		return true;
	}

	for (List<BaseInfoData>::Iter * iter = m_pFriendBeApplyList.begin();iter!=NULL; iter = m_pFriendBeApplyList.next(iter))
	{
		if (iter->mValue.playerUid == playerUid)
		{
			return true;
		}
	}

	return false;
}

// 
// bool FriendBlackManager::onGetOtherBeApplyOffLineReq(UInt64 playerUid)
// {
// 	if (m_pFriendBeApplyList.size() >= 50)
// 	{
// 		return false;
// 	}
// 
// 	for (List<BaseInfoData>::Iter * iter = m_pFriendBeApplyList.begin();iter!=NULL; iter = m_pFriendBeApplyList.next(iter))
// 	{
// 		if (iter->mValue.playerUid == playerUid)
// 		{
// 			return false;
// 		}
// 	}
// 
// 	return true;
// }

// bool FriendBlackManager::onGetOtherBeApplyOffLineResp(UInt64 playerUid)
// {
// 	if (m_pFriendBeApplyList.size() >= 50)
// 	{
// 		return false;
// 	}
// 
// 	for (List<BaseInfoData>::Iter * iter = m_pFriendBeApplyList.begin();iter!=NULL; iter = m_pFriendBeApplyList.next(iter))
// 	{
// 		if (iter->mValue.playerUid == playerUid)
// 		{
// 			return false;
// 		}
// 	}
// 
// 	return true;
// }
// 
// 
