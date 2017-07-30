#include "DetailInfoMsgHandler.h"
#include "LogicSystem.h"
#include "GameServer.h"
#include "CommonMsgHandler.h"

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
		UInt32 oldPower = player->getPlayerPower();
		player->getDetailInfoManager().setDetailInfo(detailInfo);
		
		

		//将战力改变同步到戏班
		if(oldPower != playerPower)
		{
			player->setPlayerPower(playerPower);
			player->getConsortManager().updateConsortPower(oldPower, playerPower);
			player->getPersistManager().setDirtyBit(BASEDATABIT);
		}
	}
	else
	{
		LOG_INFO("empty message setDetailInfo!");
	}

	SyncTimeReq syncResp;
	CommonMsgHandler::onSyncTimeReq(connId, syncResp);
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
	
		//之前改过名字则返回值为空
		if(oldStr.empty() )
		{
			GCPlayerRenameResp renameResp;
			renameResp.mPacketID = BOC_PLAYERRENAME_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::NameChangeOnce;
			

			Json::StyledWriter writer;	
			renameResp.mRespJsonStr = writer.write(root);
			cout << renameResp.mRespJsonStr;
			NetworkSystem::getSingleton().sendMsg(renameResp, connId);


			return;
		}


		Goods goods;
		List<Goods> itemList;

		goods.resourcestype =AWARD_BASE;
		goods.subtype = AWARD_BASE_GOLD;
		goods.num = 0 - 2500;
		itemList.insertTail(goods);
		GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog5);

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
