#include "ServantMsgHandler.h"
#include "LogicSystem.h"
#include "GameServer.h"

using namespace Lynx;

void ServantMsgHandler::onServantCombine(const ConnId& connId, CGServantCombineReq& msg)
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
		UInt64 servantId = root["servantId"].asUInt64();
		
		player->getServantManager().combineServant(servantId);

	}
	else
	{
		LOG_INFO("empty message servantCombine!");
	}


}


void ServantMsgHandler::onServantLvUp(const ConnId&connId, CGServantLvUpReq&msg)
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
		UInt64 servantId = root["servantId"].asUInt64();
		
		List<UInt32 > foodList;
		for(UInt32 i = 0; i < root["foods"].size(); i++)
		{
			foodList.insertTail(root["foods"][i].asUInt());	
		}
		

		player->getServantManager().servantLvUp(servantId, foodList);

	}
	else
	{
		LOG_INFO("empty message servantLvUp!");
	}
}


void ServantMsgHandler::onServantStarUp(const ConnId&connId, CGServantStarUpReq & msg)
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
		UInt64 servantId = root["servantId"].asUInt64();

		
		
		player->getServantManager().servantStarUp(servantId);

	}
	else
	{
		LOG_INFO("empty message servantStarUp!");
	}
}

void ServantMsgHandler::onServantFloorUp(const ConnId&connId, CGServantFloorUpReq & msg)
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
		UInt64 servantId = root["servantId"].asUInt64();

		player->getServantManager().servantFloorUp(servantId);

	}
	else
	{
		LOG_INFO("empty message servantFloorUp!");
	}
}

void ServantMsgHandler::onServantTreasureCombine(const ConnId&connId, CGTreasureCombineReq & msg)
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
		UInt64 treasureId = root["treasureId"].asUInt64();
		UInt32 treasureCount = root["count"].asUInt();

		player->getServantManager().combineTreasure(treasureId, treasureCount);

	}
	else
	{
		LOG_INFO("empty message servantTreasureCombine!");
	}
}

void ServantMsgHandler::onServantTreasureEquip(const ConnId&connId, CGTreasureEquipReq & msg)
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
		UInt64 treasureId = root["servantId"].asUInt64();
		
		Vector<UInt64> equipTreaures;

		for(UInt32 i = 0; i < root["equipTreasures"].size(); i++)
		{
			UInt64 treasureId = root["equipTreasures"][i].asUInt64();
			equipTreaures.push_back(treasureId);
		}
		
		

		player->getServantManager().treasureEquip(treasureId, equipTreaures);

	}
	else
	{
		LOG_INFO("empty message servantTreasureEquip!");
	}
}

void ServantMsgHandler::onServantBattleOpen(const ConnId& connId, CGServantBattleOpenReq & msg)
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
		
		UInt64 assistBattleIndex = root["assistIndex"].asUInt();
		
		player->getServantManager().assistbattleOpen(assistBattleIndex);	

	}
	else
	{
		LOG_INFO("empty message battleopen!");
	}
}

void ServantMsgHandler::onAssistBattleEnhance(const ConnId& connId, CGAssistBattleEnhanceReq & msg)
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

		UInt64 assistBattleIndex = root["assistIndex"].asUInt();

		player->getServantManager().assistbattleEnhance(assistBattleIndex);
			

	}
	else
	{
		LOG_INFO("empty message battleenhance!");
	}
}


void ServantMsgHandler::onServantBattleSet(const ConnId& connId, CGServantBattleSetReq & msg)
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

		Vector<UInt64> battleVec;
		for(UInt32 i = 0; i < root["totalBattle"].size(); i++)
		{
			battleVec.push_back(root["totalBattle"][i].asUInt64());
		}
		
		player->getServantManager().servantBattleSet(battleVec);

		


	}
	else
	{
		LOG_INFO("empty message battleenhance!");
	}
}


void ServantMsgHandler::onAssistBattleOneset(const ConnId& connId,   CGAssistBattleOnSetReq & msg)
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
		Vector<UInt64> battleVec;
		for(UInt32 i = 0; i < root["totalBattle"].size(); i++)
		{
			battleVec.push_back(root["totalBattle"][i].asUInt64());
		}

		player->getServantManager().assistBattleOneSet(battleVec);
		
	}
	else
	{
		LOG_INFO("empty message battleenhance!");
	}
}

void ServantMsgHandler::onServantSwitch(const ConnId & connId, CGServantSwitchReq & msg)
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
		UInt64 servantid = root["servantid"].asUInt64();
		UInt32 count = root["piececount"].asUInt();
		player->getServantManager().servantSwitch(servantid, count);
	}
	else
	{
		LOG_INFO("empty message servantswitch!");
	}
}