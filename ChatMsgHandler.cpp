#include "ChatMsgHandler.h"
#include "LogicSystem.h"


using namespace Lynx;

void ChatMsgHandler::onMsgSetBlack(const ConnId& connId, CGSetBlackListReq& msg)
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

		player->getChatManager().setBlackList(playerUid);

	}
	else
	{
		LOG_INFO("empty message MsgSetBlack!");
	}
}


void ChatMsgHandler::onMsgAddFriend(const ConnId& connId, CGAddFriendReq& msg)
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

		player->getChatManager().addFriendList(playerUid);

	}
	else
	{
		LOG_INFO("empty message MsgAddFriend!");
	}
}


void ChatMsgHandler::onMsgShowDetail(const ConnId& connId, CGShowDetailReq& msg)
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

		player->getChatManager().showDetailInfo(playerUid);

	}
	else
	{
		LOG_INFO("empty message MsgShowDetail!");
	}
}

void ChatMsgHandler::onMsgClientChat(const ConnId& connId, CGClientChatReq& msg)
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
		UInt32 channelType = root["channelType"].asUInt();
		UInt64 fromPlayer = root["fromPlayer"].asUInt64(); 
		UInt64 toPlayer = root["toPlayer"].asUInt64();
		std::string chatMsg = root["chatMsg"].asString();
		player ->getChatManager().receiveMsg(channelType, fromPlayer, toPlayer, chatMsg);
	}
	else
	{
		LOG_INFO("empty message clientChatMsg!");
	}
}


void ChatMsgHandler ::onMsgClientForbid(const ConnId& connId, CGForbidChatReq& msg)
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
		UInt64 playerUid = root["playeruid"].asUInt64();
		UInt64 timebegin = root["timebegin"].asUInt64();
		UInt32 timelong = root["timelong"].asUInt();

		player->getChatManager().clientForbidChat( playerUid, timelong, timebegin);
	}
	else
	{
		LOG_INFO("empty message clientChatMsg!");
	}
}


void ChatMsgHandler::onMsgClientReset(const ConnId& connId, CGResetChatReq& msg)
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
		UInt64 playerUid = root["playeruid"].asUInt64();
		player->getChatManager().clientResetChat(playerUid);
	}
	else
	{
		LOG_INFO("empty message clientReset!");
	}

	

}


void ChatMsgHandler::onMsgClientOnline(const ConnId& connId, CGChatOnlineReq &msg)
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
		UInt64 playerUid = root["playeruid"].asUInt64();
		player->getChatManager().checkOnline(playerUid);
	}
	else
	{
		LOG_INFO("empty message clientOnline!");
	}

	
}