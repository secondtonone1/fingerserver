#include "PVPSystemWC.h"


#include "../GameServer.h"
#include "../LogicSystem.h"

using namespace Lynx;

PVPSystemWC::PVPSystemWC(){}

PVPSystemWC::~PVPSystemWC()
{}


bool PVPSystemWC::initial()
{
	
	return true;

	
}

void PVPSystemWC::release()
{
	m_mapPVPIng.clear();
	m_mapWaitForPVP.clear();
	
}

void PVPSystemWC::addWaitForPVP(UInt64 playerUid, std::string data)
{
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
	if(!player)
	{
		LOG_INFO("player not find");
		return ;
	}
	const ConnId& connId = player->getConnId();
	Map<UInt64, PVPDataWC>::Iter * findPVPIng = m_mapPVPIng.find(playerUid);
	Map<UInt64, PVPDataWC>:: Iter * findWait = m_mapWaitForPVP.find(playerUid);

	if(findPVPIng != NULL || findWait != NULL)
	{
		Json::Value root;
		root["packetType"] = 2;
		root["errorId"] = LynxErrno::PVPStateError;

		GCPVPResp pvpResp;
		Json::StyledWriter writer;
		pvpResp.mPacketID = BOC_PVP_RESP;
		pvpResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(pvpResp,connId);
		LOG_INFO("PVP REQ JSONRESP: %s", pvpResp.mRespJsonStr.c_str());
		return ;	
	}
	
	//是否有其他玩家处于等待PVP状态
	if(m_mapWaitForPVP.size())
	{
		Map<UInt64, PVPDataWC >::Iter * waitIter = m_mapWaitForPVP.begin();
		for( ; 
			waitIter != m_mapWaitForPVP.end();  waitIter = m_mapWaitForPVP.next(waitIter))
		{
			if(waitIter->mKey != playerUid)
			{
				break;
			}
		}

		if(waitIter == m_mapWaitForPVP.end())
		{
				LOG_INFO("PVP Logic Error!!!");
				return ;
		}

		PVPDataWC pvpDataWC;
		pvpDataWC.mOtherUid = waitIter->mKey;
		pvpDataWC.mSelfUid = playerUid;
		pvpDataWC.mData = data.c_str();
		pvpDataWC.mReady = 0;

		m_mapPVPIng.insert(playerUid, pvpDataWC);
	
		PVPDataWC pvpDataWCOther;
		pvpDataWCOther.mOtherUid = playerUid;
		pvpDataWCOther.mSelfUid = waitIter->mKey;
		pvpDataWCOther.mData = waitIter->mValue.mData;
		pvpDataWCOther.mReady = 0;

		m_mapPVPIng.insert( waitIter->mKey, pvpDataWCOther);

		m_mapWaitForPVP.erase(waitIter);

		//回包，并发送PVP通知
		Json::Value root;
		root["packetType"] = 2;
		root["errorId"] = LynxErrno::None;

		GCPVPResp pvpResp;
		Json::StyledWriter writer;
		pvpResp.mPacketID = BOC_PVP_RESP;
		pvpResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(pvpResp,connId);
		LOG_INFO("PVP REQ JSONRESP: %s", pvpResp.mRespJsonStr.c_str());

		//发送PVP通知
		Json::Value pvpRoot;
		pvpRoot["packetType"] = 5;
		pvpRoot["errorId"] = LynxErrno::None;
		pvpRoot["resultString"] = (pvpDataWC.mData + ';' + pvpDataWCOther.mData).c_str();
		
		GCPVPResp pvpRespNotify;
		pvpRespNotify.mPacketID = BOC_PVP_RESP;
		pvpRespNotify.mRespJsonStr = writer.write(pvpRoot);
		NetworkSystem::getSingleton().sendMsg(pvpRespNotify,connId);

		Player  * otherPlayer = LogicSystem::getSingleton().getPlayerByGuid(pvpDataWCOther.mSelfUid);
		if(otherPlayer )
		{
			const ConnId & otherconnId = otherPlayer->getConnId();
			if(otherconnId)
			{
				NetworkSystem::getSingleton().sendMsg(pvpRespNotify,otherconnId);
			}
			
		}

	}
	else
	{
		PVPDataWC pvpDataWC;
		pvpDataWC.mSelfUid = playerUid;
		pvpDataWC.mOtherUid = 0;
		pvpDataWC.mData = data.c_str();
		pvpDataWC.mReady = 0;
		m_mapWaitForPVP.insert(playerUid, pvpDataWC);

		Json::Value root;
		root["packetType"] = 2;
		root["errorId"] = LynxErrno::None;

		GCPVPResp pvpResp;
		Json::StyledWriter writer;
		pvpResp.mPacketID = BOC_PVP_RESP;
		pvpResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(pvpResp,connId);
		LOG_INFO("PVP REQ JSONRESP: %s", pvpResp.mRespJsonStr.c_str());
	}

}

void PVPSystemWC::dealConClose(UInt64 playerUid)
{
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
	if(!player)
	{
		LOG_INFO("player not find");
		return ;
	}


	Map<UInt64, PVPDataWC>::Iter * findPVPIng = m_mapPVPIng.find(playerUid);
	Map<UInt64, PVPDataWC>:: Iter * findWait = m_mapWaitForPVP.find(playerUid);

	//从等待队列中移除
	if(findWait)
	{
		m_mapWaitForPVP.erase(findWait);
		return;
	}


	if(findPVPIng)
	{
		//发现正在pvp的一方掉线，通知另一方
		Player * other = LogicSystem::getSingleton().getPlayerByGuid(findPVPIng->mValue.mOtherUid);
		if(!other)
		{
			m_mapPVPIng.erase(findPVPIng);
			return;
		}

		const ConnId & connId = other->getConnId();
		if(!connId)
		{
			m_mapPVPIng.erase(findPVPIng);
			return;
		}

		Json::Value root;
		root["packetType"] = 9;
		root["errorId"] = LynxErrno::None;
		std::stringstream mystream;
		mystream.clear();
		mystream.str("");
		mystream << playerUid;
		std::string result;
		mystream >> result;

		root["resultString"] = result;

		GCPVPResp pvpResp;
		Json::StyledWriter writer;
		pvpResp.mPacketID = BOC_PVP_RESP;
		pvpResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(pvpResp,connId);
		LOG_INFO("offline JSONRESP: %s", pvpResp.mRespJsonStr.c_str());

		m_mapPVPIng.erase(findPVPIng);
		
	}

}

void PVPSystemWC::cancelWaitForPVP(UInt64 playerUid)
{
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
	if(!player)
	{
		LOG_INFO("player not find");
		return ;
	}
	
	const ConnId& connId = player->getConnId();
	
	Map<UInt64, PVPDataWC>:: Iter * findWait = m_mapWaitForPVP.find(playerUid);

	if(findWait == NULL)
	{
		Json::Value root;
		root["packetType"] = 4;
		root["errorId"] = LynxErrno::PlayerNotWait;
		

		GCPVPResp pvpResp;
		Json::StyledWriter writer;
		pvpResp.mPacketID = BOC_PVP_RESP;
		pvpResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(pvpResp,connId);
		LOG_INFO("cancel wait for pvp JSONRESP: %s", pvpResp.mRespJsonStr.c_str());


		return;
	}


	m_mapWaitForPVP.erase(findWait);

	Json::Value root;
	root["packetType"] = 4;
	root["errorId"] = LynxErrno::None;
		

	GCPVPResp pvpResp;
	Json::StyledWriter writer;
	pvpResp.mPacketID = BOC_PVP_RESP;
	pvpResp.mRespJsonStr = writer.write(root);

	NetworkSystem::getSingleton().sendMsg(pvpResp,connId);
	LOG_INFO("cancel wait for pvp JSONRESP: %s", pvpResp.mRespJsonStr.c_str());
}

void PVPSystemWC::readyForPVP(UInt64 playerUid)
{
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
	if(!player)
	{
		LOG_INFO("player not find");
		return ;
	}
	
	const ConnId& connId = player->getConnId();
	Map<UInt64, PVPDataWC>::Iter * findPVPIng = m_mapPVPIng.find(playerUid);

	if(!findPVPIng)
	{
		Json::Value root;
		root["errorId"] = LynxErrno::ClienServerDataNotMatch;
		GCPVPResp pvpResp;
		Json::StyledWriter writer;
		pvpResp.mPacketID = BOC_PVP_RESP;
		pvpResp.mRespJsonStr = writer.write(root);
		NetworkSystem::getSingleton().sendMsg(pvpResp,connId);
		LOG_INFO("player not find");
		return;
	}

	findPVPIng->mValue.mReady = 1;

	Player * otherPlayer = LogicSystem::getSingleton().getPlayerByGuid(findPVPIng->mValue.mOtherUid);
	if(!otherPlayer)
	{
		Json::Value root;
		root["errorId"] = LynxErrno::ClienServerDataNotMatch;
		GCPVPResp pvpResp;
		Json::StyledWriter writer;
		pvpResp.mPacketID = BOC_PVP_RESP;
		pvpResp.mRespJsonStr = writer.write(root);
		NetworkSystem::getSingleton().sendMsg(pvpResp,connId);
		LOG_INFO("player not find");
		return ;
	}

	Map<UInt64, PVPDataWC>::Iter * findOtherPVPIng = m_mapPVPIng.find(findPVPIng->mValue.mOtherUid);
	if(!findOtherPVPIng)
	{
		Json::Value root;
		root["errorId"] = LynxErrno::ClienServerDataNotMatch;
		GCPVPResp pvpResp;
		Json::StyledWriter writer;
		pvpResp.mPacketID = BOC_PVP_RESP;
		pvpResp.mRespJsonStr = writer.write(root);
		NetworkSystem::getSingleton().sendMsg(pvpResp,connId);
		LOG_INFO("player not find");
		return;
	}
	
	if(findOtherPVPIng->mValue.mReady)
	{
		//发送通知
		const ConnId & connId = player->getConnId();
		const ConnId & otherConnId = otherPlayer->getConnId();

		Json::Value root;
		root["packetType"] = 7;
		root["errorId"] = LynxErrno::None;
		
		std::stringstream mystream;
		mystream.clear();
		mystream.str("");
		mystream << time(0);
		std::string result;
		mystream >> result;

		root["resultString"] = result;

		GCPVPResp pvpResp;
		Json::StyledWriter writer;
		pvpResp.mPacketID = BOC_PVP_RESP;
		pvpResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(pvpResp,connId);
		LOG_INFO("cancel wait for pvp JSONRESP: %s", pvpResp.mRespJsonStr.c_str());

		NetworkSystem::getSingleton().sendMsg(pvpResp,otherConnId);
		LOG_INFO("cancel wait for pvp JSONRESP: %s", pvpResp.mRespJsonStr.c_str());
	
	}

}

void PVPSystemWC::cancelPVPIng(UInt64 playerUid)
{
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
	if(!player)
	{
		LOG_INFO("player not find");
		return ;
	}
	
	const ConnId& connId = player->getConnId();
	Map<UInt64, PVPDataWC>::Iter * findPVPIng = m_mapPVPIng.find(playerUid);

	if(!findPVPIng || !connId)
	{
		LOG_INFO("player not find");
		return;
	}

	Json::Value root;
	root["packetType"] = 9;
	root["errorId"] = LynxErrno::None;
	

		std::stringstream mystream;
		mystream.clear();
		mystream.str("");
		mystream << playerUid;
		std::string result;
		mystream >> result;

		root["resultString"] = result;


	GCPVPResp pvpResp;
	Json::StyledWriter writer;
	pvpResp.mPacketID = BOC_PVP_RESP;
	pvpResp.mRespJsonStr = writer.write(root);

	NetworkSystem::getSingleton().sendMsg(pvpResp,connId);
	LOG_INFO("cancel pvping for pvp JSONRESP: %s", pvpResp.mRespJsonStr.c_str());

	UInt64 otherUid = findPVPIng->mValue.mOtherUid;

	Map<UInt64, PVPDataWC>::Iter * findOtherPVPIng = m_mapPVPIng.find(otherUid);
	if(!findOtherPVPIng)
	{
		LOG_INFO("player not find");
		return;
	}

	Player * otherPlayer = LogicSystem::getSingleton().getPlayerByGuid(otherUid);
	const ConnId& otherConnId = otherPlayer->getConnId();
	if(!otherConnId)
	{
		LOG_INFO("player not find");
		return ;
	}
		NetworkSystem::getSingleton().sendMsg(pvpResp,otherConnId);
		LOG_INFO("cancel pvping for pvp JSONRESP: %s", pvpResp.mRespJsonStr.c_str());

}

void PVPSystemWC::sendMsgToOther(UInt64 playerUid, std::string res)
{
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
	if(!player)
	{
		LOG_INFO("player not find");
		return ;
	}
	
	const ConnId& connId = player->getConnId();
	Map<UInt64, PVPDataWC>::Iter * findPVPIng = m_mapPVPIng.find(playerUid);

	if(!findPVPIng || !connId)
	{
		LOG_INFO("player not find");
		return;
	}

	Player * otherPlayer = LogicSystem::getSingleton().getPlayerByGuid(findPVPIng->mValue.mOtherUid);
	const ConnId & otherConnId = otherPlayer->getConnId();

	Json::Value root;
	root["packetType"] = 102;
	root["errorId"] = LynxErrno::None;
	root["resultString"] = res;

	GCPVPResp pvpResp;
	Json::StyledWriter writer;
	pvpResp.mPacketID = BOC_PVP_RESP;
	pvpResp.mRespJsonStr = writer.write(root);

	NetworkSystem::getSingleton().sendMsg(pvpResp,otherConnId);
	LOG_INFO("send PVP MSG to Other for pvp JSONRESP: %s", pvpResp.mRespJsonStr.c_str());


	//新增加需求，该包也要发送给自己

	NetworkSystem::getSingleton().sendMsg(pvpResp,connId);
	LOG_INFO("send PVP MSG to self for pvp JSONRESP: %s", pvpResp.mRespJsonStr.c_str());

}