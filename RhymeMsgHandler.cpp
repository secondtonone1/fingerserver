#include "RhymeMsgHandler.h"
#include "LogicSystem.h"
#include "GameServer.h"

using namespace Lynx;

void RhymeMsgHandler::onRhymeEnhanceReq(const ConnId& connId, CGRhymeEnhanceReq& msg)
{
	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!player)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	Json::Value root;

	Json::Reader reader;

	if(reader.parse(msg.mReqJsonStr,root))
	{
		UInt64 playerUid = root["uuid"].asUInt64();
		UInt32 enhanceTimes = root["enhanceTimes"].asUInt();
		
		if(enhanceTimes == 1)
		{
			player->getRhymeManager().onEnhanceOnce();
		}
		else if(enhanceTimes == 10)
		{
			player->getRhymeManager().onEnhanceTen();
		}
		
	}
	else
	{
		LOG_INFO("empty message polishequipreq!");
	}

	
}

void RhymeMsgHandler::onRhymeAcupointActiveReq(const ConnId& connId, CGRhymeAcupointActiveReq& msg)
{
	//cout << "begin" << time(0) << endl;

	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!player)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	Json::Value root;
	
	Json::Reader reader;

	if(reader.parse(msg.mReqJsonStr,root))
	{
		UInt32 step = root["step"].asUInt();
		UInt32 level = root["level"].asUInt();
		UInt64 acupointIndex = root["acupointId"].asUInt64();
		player->getRhymeManager().activeRhymeAcupoint(acupointIndex,step,level);
	}
	else
	{
		LOG_INFO("empty message polishequipreq!");
	}

	
}

void RhymeMsgHandler::onRhymeSkillSetReq(const ConnId& connId, CGRhymeSkillSetReq& msg)
{
	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!player)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	Json::Value root;

	Json::Reader reader;

	if(reader.parse(msg.mReqJsonStr,root))
	{
		UInt32 acupointIndex = root["equipIdx"].asUInt();
		player->getRhymeManager().activeSkillSet(acupointIndex);
	}
	else
	{
		LOG_INFO("empty message polishequipreq!");
	}
}


void RhymeMsgHandler::onRhymeStepReq(const ConnId& connId , CGRhymeStepReq& msg)
{
	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!player)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	Json::Value root;

	Json::Reader reader;

	if(reader.parse(msg.mReqJsonStr,root))
	{
		UInt64 playerUid = root["uuid"].asUInt64();
		player->getRhymeManager().rhymeStep();
		

	}
	else
	{
		LOG_INFO("empty message polishequipreq!");
	}
}