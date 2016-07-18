#include "GMMessageHandler.h"
#include "LogicSystem.h"


using namespace Lynx;

void GMMessageHandler::onMsgGMReq(const ConnId& connId, CGGMReq& msg)
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

		Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
		if (!player)
		{
			LOG_WARN("Failed to get Player by ConnId: %llu", connId);
			return;
		}

		UInt64 etype = root["etype"].asUInt64();
		UInt64 param1 = root["param1"].asUInt64();
		UInt64 param2 = root["param2"].asUInt64();
		std::string param3 = root["param3"].asString();
		std::string param4 = root["param4"].asString();

		player->getGMManager().dealGMRequest(etype,param1,param2, param3, param4);
	}
	else
	{
		LOG_INFO("empty message GMMessage!");
	}
}
