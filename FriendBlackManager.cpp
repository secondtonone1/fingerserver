#include "FriendBlackManager.h"
#include "Player.h"
#include "ConfigParser.h"
#include "LogicSystem.h"
#include "PersistSystem.h"
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

	String ip = ConfigParser::getSingleton().getMainDbIp();
	UInt16 port = ConfigParser::getSingleton().getMainDbPort();
	String username = ConfigParser::getSingleton().getMainDbUsername();
	String password = ConfigParser::getSingleton().getMainDbPassword();
	String dbname = ConfigParser::getSingleton().getMainDbName();

	if (!mDBInterface.initial(ip.c_str(), port, username.c_str(),
		password.c_str(), dbname.c_str()))
	{
		LOG_WARN("Failed to connect mysql server. [%s %s %s %s]",
			ip.c_str(), username.c_str(), password.c_str(), dbname.c_str());
		return false;
	}

	return true;

}

void FriendBlackManager::release()
{
	m_pPlayer = NULL;
	m_pFriendData = NULL;
	m_pFriendList = NULL;
	m_pBlackList = NULL;
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
			
			bool dbRes = getBaseInfoFromDb(friendIter->mValue, baseInfoData);
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
		friendEle["leaveTime"] = baseInfoData.leaveTime;
		
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

			bool dbRes = getBaseInfoFromDb(blackIter->mValue, baseInfoData);
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
		blackEle["leaveTime"] = baseInfoData.leaveTime;

		root["blacks"].append(blackEle);
	}
	
	std::string jsonStr = writer.write(root);
	//cout << jsonStr;
	return jsonStr;

}

bool FriendBlackManager::getBaseInfoFromDb(UInt64 playerUid, BaseInfoData & baseInfoData)
{
	char basedata[2048] = {0};

	snprintf(basedata, sizeof(basedata),  "call basedata_load(%llu)", playerUid);

	LOG_DEBUG("Sql:%s", basedata);
	bool result = mDBInterface.execSql(basedata);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败
		mDBInterface.freeResult(&rs);
		LOG_INFO("sql exec failed");
		return false;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		// 角色不存在
		mDBInterface.freeResult(&rs);
		return false;

	}

	baseInfoData.modelId  = lynxAtoi<UInt32>(row[0]);
	baseInfoData.name = row[1];
	baseInfoData.power = lynxAtoi<UInt64>(row[2]);
	baseInfoData.vipLv = lynxAtoi<UInt32>(row[3]);
	baseInfoData.level = lynxAtoi<UInt32>(row[4]);
	baseInfoData.leaveTime = lynxAtoi<UInt64>(row[5]);

	mDBInterface.freeResult(&rs); 

	return true;
}


bool FriendBlackManager::getOtherFriendInfo(UInt64 otherUid, FriendBlackInfo & friendblackInfo)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call friendandblack_load(%llu)", otherUid);
	bool res = mDBInterface.execSql(sql);

	if(!res)
	{
		LOG_INFO("call friendandblack_load failed!! player: %llu",  otherUid);

		return false;

	}

	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// 执行失败

		
		mDBInterface.freeResult(&rs);

		return false ;		
	}


	MYSQL_ROW row = mDBInterface.fetchRow(rs);
	if(!row)
	{
		mDBInterface.freeResult(&rs);
		return false;
	}

	friendblackInfo.friendstr = row[1];
	friendblackInfo.blackstr = row[2];
	friendblackInfo.friendCount= lynxAtoi<UInt32>(row[3]);
	friendblackInfo.playerUid = otherUid;
	

	mDBInterface.freeResult(&rs); 

	return true;
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