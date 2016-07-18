#include "HeroEquipMsgHandler.h"
#include "LogicSystem.h"
#include "GameServer.h"

using namespace Lynx;

void HeroEquipMsgHandler::onHeroEquipLvUp(const ConnId& connId, CGHeroEquipEnhanceReq& msg)
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
		UInt64 playerUid = root["playeruid"].asUInt64();
		UInt32 equipPos = root["equippos"].asUInt();
		UInt32 enhanceType = root["enhancetype"].asUInt();
		player->getHeroEquipManager().enhanceEquip(playerUid, equipPos, enhanceType);

	}
	else
	{
		LOG_INFO("empty message polishequipreq!");
	}

}

void HeroEquipMsgHandler::onHeroEquipGemSet(const ConnId& connId, CGHeroEquipGemSetReq& msg)
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
		UInt32 equipPos= root["equipPos"].asUInt();
		UInt32 gemSlot = root["equipGemSlot"].asUInt();
		UInt64 gemId = root["gemId"].asUInt64();
		player->getHeroEquipManager().equipSetGem(equipPos, gemSlot, gemId);

	}
	else
	{
		LOG_INFO("empty message equip set Gems!");
	}

}

void HeroEquipMsgHandler::onHeroEquipGemSetOnce(const ConnId& connId,CGHeroEquipGemSetOnceReq & msg)
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
		UInt32 equipPos= root["equipPos"].asUInt();

		player->getHeroEquipManager().equipSetGemOnce(equipPos);

	}
	else
	{
		LOG_INFO("empty message equip setOnce Gems!");
	}

}

void HeroEquipMsgHandler::onHeroEquipGemManualActive(const ConnId& connId, CGHeroEquipGemManualActiveReq & msg)
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
		UInt32 gemPos = root["gemPos"].asUInt();
		UInt32 equipPos = root["equipPos"].asUInt();
	
		player->getHeroEquipManager().manualActive(equipPos, gemPos);

	}
	else
	{
		LOG_INFO("empty message gemManualActive FAiled!");
	}

}

void HeroEquipMsgHandler::onHeroEquipGemLvUp(const ConnId& connId, CGHeroEquipGemLvUpReq & msg)
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
		UInt32 gemPos = root["gemPos"].asUInt();
		UInt32 equipPos = root["equipPos"].asUInt();

		player->getHeroEquipManager().equipGemLvUp(equipPos, gemPos);

	}
	else
	{
		LOG_INFO("empty message equipGemLvUp FAiled!");
	}
	
}


void HeroEquipMsgHandler::onHeroEquipGemUnload(const ConnId& connId, CGHeroEquipGemUnloadReq & msg)
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
		UInt32 gemPos = root["gemPos"].asUInt();
		UInt32 equipPos = root["equipPos"].asUInt();

		player->getHeroEquipManager().equipGemUnload(equipPos, gemPos);

	}
	else
	{
		LOG_INFO("empty message equipGemUnload FAiled!");
	}
}