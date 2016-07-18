#include "HoarStoneMsgHandler.h"
#include "LogicSystem.h"
#include "GameServer.h"

using namespace Lynx;

void HoarStoneMsgHandler::onHoarStoneActiveReq(const ConnId& connId, CGHoarStoneActiveReq& msg)
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
		UInt64 hoarStoneIndex = root["hoarstoneIdx"].asUInt64();
	
		player->getHoarStoneManager().hoarStoneActive(playerUid,hoarStoneIndex);
	}
	else
	{
		
		LOG_INFO("empty message hoarStoneActiveReq!");
	
	}
	
}

void HoarStoneMsgHandler::onHoarStoneLvUpReq(const ConnId& connId, CGHoarStoneLvUpReq& msg)
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
		UInt64 hoarStoneID = root["hoarstoneIdx"].asUInt64();
		
		player->getHoarStoneManager().hoarStoneLvUp(hoarStoneID);
	}
	else
	{
		LOG_INFO("empty message hoarStoneLvUpReq!");
	}
}

void HoarStoneMsgHandler::onHoarStoneStarUpReq(const ConnId& connId, CGHoarStoneStarUpReq& msg)
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
		UInt64 hoarStoneID = root["hoarstoneIdx"].asUInt64();
		
		player->getHoarStoneManager().hoarStoneStarUp(hoarStoneID);
	}
	else
	{
		LOG_INFO("empty message hoarStoneLvUpReq!");
	}
}

void HoarStoneMsgHandler::onHoarStoneAddPieceReq(const ConnId& connId, CGHoarStoneAddPieceReq& msg)
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
		UInt64 hoarStoneID = root["hoarStoneID"].asUInt64();
		UInt32 pieceCount = root["pieceCount"].asUInt();

		
		player->getHoarStoneManager().addHoarStonePiece(hoarStoneID,pieceCount);
	}
	else
	{
		LOG_INFO("empty message hoarStoneAddPieceReq!");
	}
}

void HoarStoneMsgHandler::onHoarStoneAddRuneReq(const ConnId& connId, CGHoarStoneAddRuneReq& msg)
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
		UInt64 hoarStoneRuneID = root["hoarStoneRuneID"].asUInt64();
		UInt32 hoarStoneRuneCount = root["hoarStoneRuneCount"].asUInt();
		
		player->getHoarStoneManager().addHoarStoneRune(hoarStoneRuneID,hoarStoneRuneCount);
		
	}
	else
	{
		LOG_INFO("empty message hoarStoneAddRuneReq");
	}
}

void HoarStoneMsgHandler::onHoarStoneRuneEquipReq(const ConnId& connId, CGHoarStoneRuneEquipReq& msg)
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
		UInt64 hoarStoneID = root["hoarstoneIdx"].asUInt64();
		Json::Value::Members member = root["equipList"].getMemberNames();
		
		List<runePosData> runePosList;

		std::stringstream mystream;
		for(Json::Value::Members::iterator iter = member.begin(); iter != member.end(); ++iter)
		{
			UInt64 hoarStoneRuneID = root["equipList"][(*iter)].asUInt64();
			mystream.clear();
			mystream.str("");
			mystream << *iter;
			UInt32 hoarStonePos;
			mystream >> hoarStonePos;

			runePosData runeData;
			runeData.runePos = hoarStonePos;
			runeData.runeID = hoarStoneRuneID;
			
			runePosList.insertTail(runeData);
			
		}
		
	
		player->getHoarStoneManager().equipHoarStoneRune(runePosList,hoarStoneID);
		
	}
	else
	{
		LOG_INFO("empty message hoarStoneAddRuneReq");
	}
}


void HoarStoneMsgHandler::onHoarStoneRuneCombineReq(const ConnId& connId, CGHoarStoneRuneCombReq& msg)
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
		UInt64 dstRuneId = root["dstEquipId"].asUInt64();
		 	
        UInt32 dstRuneCount = root["dstEquipCount"].asUInt();

		UInt32 transferId = root["transferId"].asUInt();
		
		player->getHoarStoneManager().combinHoarStoneRune(dstRuneId,dstRuneCount,transferId);
	}
	else
	{
		LOG_INFO("empty message hoarStoneRuneCombinReq");
	}

}