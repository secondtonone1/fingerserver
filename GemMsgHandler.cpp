#include "GemMsgHandler.h"
#include "LogicSystem.h"
#include "GameServer.h"

using namespace Lynx;

void GemMsgHandler::onGemCombineReq(const ConnId& connId, CGGemCombineReq& msg)
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
		UInt64 gemItemId = root["gemItemId"].asUInt64();
		UInt32 count = root["count"].asUInt();
	
		player->getGemManager().combineOneTypeGems(gemItemId, count);

	}
	else
	{
		LOG_INFO("empty message gemCombineReq!");
	}
}

void GemMsgHandler::onGemCombineOnce(const ConnId& connId, CGGemCombineOnceReq& msg)
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
		UInt64 gemEquipPos = root["equipPos"].asUInt();
		player->getGemManager().gemCombineOnce(gemEquipPos);
		
	}
	else
	{
		LOG_INFO("empty message gemCombineOnceReq!");
	}
}