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

	LOG_INFO("player %llu setDetailInfo", playerUid);

	LOG_INFO("playerIdStr is %s",m_playerIdStr.c_str());
	LOG_INFO("detailInfo is %s", detailInfo.c_str());

     #ifdef __linux__
	
	 RedisManager::getSingleton().set(m_playerIdStr, detailInfo);
	
      #endif
	 LOG_INFO("player %llu setDetailInfo success", playerUid);	
}

std::string DetailInfoManager::getDetailInfoSelf()
{
	
	const ConnId& connId = m_pPlayer->getConnId();

	std::string detailInfo = "";	
     #ifdef __linux__
         detailInfo =  RedisManager::getSingleton().get(m_playerIdStr);
	#endif

	GCClientDetailInfoResp clientDetailInfoResp;
	clientDetailInfoResp.mPacketID = BOC_CLIENTDETAILINFO_RESP;
	Json::Value root;
	root["detailData"] = detailInfo;
	Json::StyledWriter writer;
	
	clientDetailInfoResp.mRespJsonStr = writer.write(root);
	cout << clientDetailInfoResp.mRespJsonStr;

	NetworkSystem::getSingleton().sendMsg(clientDetailInfoResp, connId);
	
    return detailInfo;
}

std::string DetailInfoManager::getDetailInfo(std::string playerUidStr)
{
	const ConnId& connId = m_pPlayer->getConnId();

	std::string detailInfo = "";	

	if (playerUidStr.c_str()!= ""&&lynxAtoi<UInt64>(playerUidStr.c_str())!= 0)//add by cheng
	{	
		#ifdef __linux__

			detailInfo =  RedisManager::getSingleton().get(playerUidStr);
			
		#endif		
	}

	GCClientDetailInfoResp clientDetailInfoResp;
	clientDetailInfoResp.mPacketID = BOC_CLIENTDETAILINFO_RESP;
	Json::Value root;
	root["detailData"] = detailInfo;
	Json::StyledWriter writer;

	clientDetailInfoResp.mRespJsonStr = writer.write(root);
	cout << clientDetailInfoResp.mRespJsonStr;


	NetworkSystem::getSingleton().sendMsg(clientDetailInfoResp, connId);

	return detailInfo;
}
