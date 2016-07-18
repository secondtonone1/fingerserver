#include "JewelryMsgHandler.h"
#include "LogicSystem.h"
#include "GameServer.h"

using namespace Lynx;


void JewelryMsgHandler::onJewelryPolish(const ConnId& connId, CGJewelryPolishReq& msg)
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
		UInt64 objuid = root["objuid"].asUInt64();
		UInt64 materialuid = root["materialuid"].asUInt64();
		UInt32 attrIndex = root["objAttrIndex"].asUInt();

		player->getJewelryManager().jewelryPolish(objuid, materialuid,attrIndex);

	}
	else
	{
		LOG_INFO("empty message jewelryPolish!");
	}
}


void JewelryMsgHandler::onJewelryReset(const ConnId& connId, CGJewelryResetReq & msg)
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
		UInt64 objuid = root["objJewelryUid"].asUInt64();
		UInt32 rdIndex = root["objRdAttrIndex"].asUInt();
		
		player->getJewelryManager().jewelryReset(objuid);

	}
	else
	{
		LOG_INFO("empty message jewelryPolish!");
	}
}

void JewelryMsgHandler::onJewelrySet(const ConnId& connId, CGJewelrySetReq & msg)
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
		UInt32 jewelryPos = root["jewelryPos"].asUInt();
		UInt64 jewelryUid = root["jewelryUid"].asUInt64();

		player->getJewelryManager().jewelrySet(jewelryPos,jewelryUid);

	}
	else
	{
		LOG_INFO("empty message jewelrySet!");
	}
}

void JewelryMsgHandler::onJewelryUnload(const ConnId & connId, CGJewelryUnloadReq& msg)
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
		UInt32 jewelryPos = root["jewelryPos"].asUInt();
		
		player->getJewelryManager().jewelryUnload(jewelryPos);

	}
	else
	{
		LOG_INFO("empty message jewelryUnload!");
	}
}


void JewelryMsgHandler::onJewelrySell(const ConnId & connId, CGJewelrySellReq& msg)
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
		UInt32 arraySize = root["jewelrySellList"].size();
		
		List<UInt64> sellList;
		for(UInt32 i = 0; i < arraySize; i++)
		{
			sellList.insertTail(root["jewelrySellList"][i].asUInt64());
		}

		player->getJewelryManager().jewelrySell(sellList);
        
	}
	else
	{
		LOG_INFO("empty message jewelryUnload!");
	}
	
	
}

void JewelryMsgHandler::onJewelryUpdate(const ConnId & connId, CGJewelryUpdateReq &msg)
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
		UInt32 arraySize = root["jewelryList"].size();

		List<UInt64> getList;
		for(UInt32 i = 0; i < arraySize; i++)
		{
			getList.insertTail(root["jewelryList"][i].asUInt64());
		}

		player->getJewelryManager().jewelrySell(getList);

	}
	else
	{
		LOG_INFO("empty message jewelryUpdate!");
	}


}


void JewelryMsgHandler::onJewelrySetOnce(const ConnId & connId, CGJewelrySetOnceReq &msg)
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
		UInt32 arraySize = root.size();

		Map<UInt32,UInt64> equipMap;
		for(UInt32 i = 0; i < arraySize; i++)
		{
			equipMap.insert(root[i]["pos"].asUInt(),root[i]["jewelryuid"].asUInt64());
		}

		
		player->getJewelryManager().jewelryEquipOnce(equipMap);

	}
	else
	{
		LOG_INFO("empty message jewelryequip oneset!");
	}
}