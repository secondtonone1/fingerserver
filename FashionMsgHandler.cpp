#include "FashionMsgHandler.h"
#include "LogicSystem.h"
#include "GameServer.h"

using namespace Lynx;


void FashionMsgHandler::onFashionBuy(const ConnId&connId, CGFashionBuyReq&msg)
{
	Player* selfplayer = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!selfplayer)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	Json::Value root;

	Json::Reader reader;

	if(reader.parse(msg.mReqJsonStr,root))
	{
		
		UInt64 tempId = root["fashionId"].asUInt64();
		
		selfplayer->getFashionManager().buyFashion(tempId);
		


	}
	else
	{
		LOG_INFO("empty message GM send Email!");
	}
}

