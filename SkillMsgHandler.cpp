#include "SkillMsgHandler.h"
#include "LogicSystem.h"
#include "GameServer.h"

using namespace Lynx;

void SkillMsgHandler::onSkillLevelUpReq(const ConnId& connId, CGSkillLevelUpReq& msg)
{
	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!player)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	std::string reqJsonStr = msg.mReqJsonStr;
	Json::Reader reader;

	Json::Value root;
	if (reader.parse(reqJsonStr, root))
	{
		UInt64 playerUid = root["playerUid"].asUInt64();
		UInt32 skillUid = root["skillID"].asUInt64();
		
		Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
		if (!player)
		{
			LOG_WARN("Failed to get Player by ConnId: %llu", connId);
			return;
		}

		player->getSkillManager().skillLevelUp(skillUid);

	}
	else
	{
		LOG_INFO("empty message skillLevelUp!");
	}

}


void SkillMsgHandler::onSkillPositionSetReq(const ConnId& connId, CGSkillEquipSetReq&msg)
{
	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!player)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}


	std::string reqJsonStr = msg.mReqJsonStr;
	Json::Reader reader;

	Map<UInt64,UInt32> skillEquipMap;
	Json::Value root;
	if (reader.parse(reqJsonStr, root))
	{
		for(UInt32 i = 0; i < root["skillSetList"].size(); i++)
		{
			UInt64 skillId = root["skillSetList"][i]["skillId"].asUInt64();
			UInt32 equipPos = root["skillSetList"][i]["pos"].asUInt();
			
			skillEquipMap.insert(skillId,equipPos);

		}

		if(skillEquipMap.size())
		{
			player->getSkillManager().skillPositonSet(skillEquipMap);
		}
		else
		{
			LOG_INFO("empty message skillEquipSet!");
		}
	}
	else
	{
		LOG_INFO("empty message skillEquipSet!");
	}

}


void SkillMsgHandler::onSkillLvUpOnceReq(const ConnId& connId , CGSkillLvUpOnceReq& msg)
{
	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!player)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	std::string reqJsonStr = msg.mReqJsonStr;
	Json::Reader reader;

	Map<UInt64,UInt32> skillEquipMap;
	Json::Value root;
	if (reader.parse(reqJsonStr, root))
	{
			UInt64 playerUid = root["playerUid"].asUInt64();
		UInt32 skillUid = root["skillID"].asUInt64();
			player->getSkillManager().skillLvUpOnce(skillUid);
	}
	else
	{
		LOG_INFO("empty message onSkillLvUpOnceReq!");
	}

}