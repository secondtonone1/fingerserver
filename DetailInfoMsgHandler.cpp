#include "DetailInfoMsgHandler.h"
#include "LogicSystem.h"
#include "GameServer.h"

using namespace Lynx;

void DetailInfoMsgHandler::onClientDetailInfoReq(const ConnId& connId, CGClientDetailInfoReq& msg)
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
			std::stringstream mystream;
			mystream << playerUid;
			std::string playerStr;
			mystream >> playerStr;

			player->getDetailInfoManager().getDetailInfo(playerStr);
			

		}
		else
		{
			LOG_INFO("empty message DetailInfoReq!");
		}
}


void DetailInfoMsgHandler::onClientUpdateInfoReq(const ConnId& connId, CGClientUpdateInfoReq& msg)
{
	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!player)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	Json::Reader reader;
    Json::Value root;

	if(reader.parse(msg.mReqJsonStr,root))
	{
		std::string detailInfo = root["detailData"].asString();
		UInt32 playerPower = root["power"].asUInt();

		player->getDetailInfoManager().setDetailInfo(detailInfo);
		player->setPlayerPower(playerPower);
		player->getPersistManager().setDirtyBit(BASEDATABIT);
	}
	else
	{
		LOG_INFO("empty message setDetailInfo!");
	}
}


void DetailInfoMsgHandler::onRenameReq(const ConnId& connId, CGPlayerRenameReq&msg)
{
	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!player)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	Json::Reader reader;
	Json::Value root;

	if(reader.parse(msg.mReqJsonStr,root))
	{
		UInt64 gold = player->getPlayerGold();
		if(gold < 2500)
		{
			GCPlayerRenameResp renameResp;
			renameResp.mPacketID = BOC_PLAYERRENAME_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::RmbNotEnough;
			Json::StyledWriter writer;	
			renameResp.mRespJsonStr = writer.write(root);
			cout << renameResp.mRespJsonStr;
			NetworkSystem::getSingleton().sendMsg(renameResp, connId);
			return;
		}

		std::string newname = root["name"].asString();
		std::string oldStr = player->getPlayerInfoManager().rename(newname);
	
		
		player->setPlayerGold(gold - 2500);

	  player->getPersistManager().setDirtyBit(BASEDATABIT);

		GCPlayerRenameResp renameResp;
		renameResp.mPacketID = BOC_PLAYERRENAME_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::None;
		root["gold"] = gold - 2500;
		root["newname"] = newname;
		root["oldname"] = oldStr;

		Json::StyledWriter writer;	
		renameResp.mRespJsonStr = writer.write(root);
		cout << renameResp.mRespJsonStr;
		NetworkSystem::getSingleton().sendMsg(renameResp, connId);
	}
	else
	{
		LOG_INFO("empty message onRenameReq!");
	}
}
