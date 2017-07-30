#include "DetailInfoManager.h"
#include "Player.h"
#include "LogicSystem.h"
#include "RedisManager.h"

using namespace Lynx;


DetailInfoManager::DetailInfoManager()
{
	m_pPlayer = NULL;
	m_playerIdStr = "";
}

DetailInfoManager:: ~DetailInfoManager()
{

}



bool DetailInfoManager::initial(Player* player)
{
	if (player == NULL)
	{
		LOG_WARN("Player pointer is NULL.");
		return false;
	}
	m_playerIdStr.clear();
	m_pPlayer = player;
	Guid playrUid = m_pPlayer->getPlayerGuid();
	std::stringstream mystream;
	mystream << playrUid;
	mystream >> m_playerIdStr;

	return true;
}

void DetailInfoManager::release()
{
	m_pPlayer = NULL;
	m_playerIdStr = "";
}


void DetailInfoManager::setDetailInfo(std::string detailInfo)
{
    UInt64 playerUid = m_pPlayer->getPlayerGuid(); 
	
     #ifdef __linux__
	
	 RedisManager::getSingleton().set(m_playerIdStr, detailInfo);
	
      #endif
}

std::string DetailInfoManager::getDetailInfoSelf()
{
	UInt32 isRobot = 0;
	const ConnId& connId = m_pPlayer->getConnId();

	if (lynxAtoi<UInt64>(m_playerIdStr.c_str())  > RobotMinRoleID && lynxAtoi<UInt64>(m_playerIdStr.c_str()) <RobotMaxRoleID)
	{
		isRobot = 1;
	}
	std::string detailInfo = "";	
     #ifdef __linux__
         detailInfo =  RedisManager::getSingleton().get(m_playerIdStr);
	#endif

	GCClientDetailInfoResp clientDetailInfoResp;
	clientDetailInfoResp.mPacketID = BOC_CLIENTDETAILINFO_RESP;
	Json::Value root;
	root["detailData"] = detailInfo;
	root["isRobot"] = isRobot;
	Json::StyledWriter writer;
	
	clientDetailInfoResp.mRespJsonStr = writer.write(root);
	cout << clientDetailInfoResp.mRespJsonStr;

	NetworkSystem::getSingleton().sendMsg(clientDetailInfoResp, connId);
	
    return detailInfo;
}

std::string DetailInfoManager::getDetailInfo(std::string playerUidStr)
{
	UInt32 isRobot = 0;
	const ConnId& connId = m_pPlayer->getConnId();

	std::string detailInfo = "";	

	if (playerUidStr.c_str()!= ""&&lynxAtoi<UInt64>(playerUidStr.c_str())!= 0)//add by cheng
	{	
		#ifdef __linux__

			detailInfo =  RedisManager::getSingleton().get(playerUidStr);
			
		#endif		
	}

	if (lynxAtoi<UInt64>(playerUidStr.c_str())  > RobotMinRoleID && lynxAtoi<UInt64>(playerUidStr.c_str()) <RobotMaxRoleID)
	{
		isRobot = 1;
	}

	GCClientDetailInfoResp clientDetailInfoResp;
	clientDetailInfoResp.mPacketID = BOC_CLIENTDETAILINFO_RESP;
	Json::Value root;
	root["detailData"] = detailInfo;
	root["isRobot"] = isRobot;
	Json::StyledWriter writer;

	clientDetailInfoResp.mRespJsonStr = writer.write(root);
	cout << clientDetailInfoResp.mRespJsonStr;


	NetworkSystem::getSingleton().sendMsg(clientDetailInfoResp, connId);

	return detailInfo;
}
