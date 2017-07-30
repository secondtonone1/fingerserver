#include "ChatManager.h"
#include "Player.h"
#include "ConfigParser.h"
#include "LogicSystem.h"
#include "ConsortSystem/ConsortSystem.h"
#include "RedisManager.h"
using namespace Lynx;

ChatManager::ChatManager()
{
	m_pPlayer = NULL;
}


ChatManager:: ~ChatManager()
{

}

bool ChatManager::initial(Player* player)
{
	if (player == NULL)
	{
		LOG_WARN("Player pointer is NULL.");
		return false;
	}

	m_pPlayer = player;

	
	m_pChatList = &player->mPlayerData.mChatData.chatLists;


	return true;

}


void ChatManager::release()
{
	m_pPlayer = NULL;
}

void ChatManager::insertPlayerToChatList(UInt64 playerUid)
{
	List<ChatData>::Iter * findChatIter = NULL;
	
	//查找当前玩家聊天列表里是否有playeruid玩家
	for(List<ChatData>::Iter * chatIter = m_pChatList->begin(); chatIter != NULL; chatIter = m_pChatList->next(chatIter))
	{
		if(chatIter->mValue.playerUid == playerUid)
		{
			findChatIter = chatIter;
			break;
		}
			
	}
	//存在该玩家，则移除之前的聊天记录	
	if(findChatIter)
	{
		m_pChatList->erase(findChatIter);
	}
		
	ChatData chatData;
	chatData.chatTime = TimeUtil::getTimeSec();
	chatData.playerUid = playerUid;

	m_pChatList->insertTail(chatData);

	checkChatListFull();

	m_pPlayer->getPersistManager().setDirtyBit(CHATDATABIT);
	
}

void ChatManager::checkChatListFull()
{
	//循环移除超过10个的聊天玩家
	UInt32 listSize = m_pChatList->size() ;
	if(listSize > 10)
	{
		List<ChatData> ::Iter * chatIter = m_pChatList->begin();
		for(UInt32 i = 0; i < listSize - 10; i++)
		{
			if(chatIter)
			{
				chatIter = m_pChatList->erase(chatIter);
			}
			
		}
	}
}



void ChatManager::setBlackList(UInt64 playeruid)
{
	//添加黑名单功能


	const ConnId& connId = m_pPlayer->getConnId();

	GCSetBlackListResp blackListResp;
	blackListResp.mPacketID = BOC_SETBLACKLIST_RESP;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["playeruid"] = playeruid;
	Json::StyledWriter writer;
	blackListResp.mRespJsonStr = writer.write(root);

		
	NetworkSystem::getSingleton().sendMsg(blackListResp, connId);
}


void ChatManager::addFriendList(UInt64 playeruid)
{
	//添加好友功能

	//以下是回包

	const ConnId& connId = m_pPlayer->getConnId();

	GCAddFriendResp addFriendResp;
	addFriendResp.mPacketID = BOC_ADDFRIEND_RESP;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["playeruid"] = playeruid;
	Json::StyledWriter writer;
	addFriendResp.mRespJsonStr = writer.write(root);


	NetworkSystem::getSingleton().sendMsg(addFriendResp, connId);

}

void ChatManager::showDetailInfo(UInt64 playeruid)
{
	//查看详情功能

	const ConnId& connId = m_pPlayer->getConnId();

	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playeruid);
	
	//玩家不在线
	if(!player)
	{
		
		BaseInfoData baseInfo;
		bool flag = LogicSystem::getSingleton().getBaseInfo(playeruid,baseInfo);

			if(!flag  )
			{
					GCShowDetailResp showDetailResp;
					showDetailResp.mPacketID = BOC_SHOWDETAIL_RESP;

					Json::Value root;
					root["errorId"] = LynxErrno::InvalidParameter;

					Json::StyledWriter writer;
					showDetailResp.mRespJsonStr = writer.write(root);

					NetworkSystem::getSingleton().sendMsg(showDetailResp, connId);

					return;
				

			}



		
		Json::Value root;
		root["errorId"] = LynxErrno::None;
		root["modelid"] = baseInfo.modelId;
		root["name"] = baseInfo.name.c_str();
		root["level"] = baseInfo.level;
		root["vipLv"] = baseInfo.vipLv;
		root["playeruid"] = playeruid;
		root["attack"] = baseInfo.power;
	
		

	

		GCShowDetailResp showDetailResp;
		showDetailResp.mPacketID = BOC_SHOWDETAIL_RESP;

		Json::StyledWriter writer;
		showDetailResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(showDetailResp, connId);

		return;


	}

	
		
		
		GCShowDetailResp showDetailResp;
		showDetailResp.mPacketID = BOC_SHOWDETAIL_RESP;

		Json::Value root;
		root["errorId"] = LynxErrno::None;
		root["modelid"] = player->getPlayerModelID();
		root["name"] = player->getPlayerName();
		root["level"] = player->getPlayerLeval();
		root["vipLv"] = player->getVipLevel();
		root["playeruid"] = playeruid;
		root["attack"] = player->getPlayerPower();

		Json::StyledWriter writer;
		showDetailResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(showDetailResp, connId);

		return;
	
}

std::string ChatManager::convertDataToJson()
{
	Json::Value root;

	root["disableflag"] = m_pPlayer->mPlayerData.mChatData.disableFlag;

	root["disablebegintime"] = m_pPlayer->mPlayerData.mChatData.disableBeginTime;

	root["disableTimeLong"] = m_pPlayer->mPlayerData.mChatData.disableTimeLong;

	root["nowtime"] = UInt64(time(0));

	for(List<ChatData>::Iter * chatIter = m_pChatList->begin(); chatIter != NULL; 
		chatIter = m_pChatList->next(chatIter) )
	{
		
		Json::Value chatDataRoot;

		Player * player = LogicSystem::getSingleton().getPlayerByGuid(chatIter->mValue.playerUid);
		if(player)
		{
			chatDataRoot["lastChatTime"] = chatIter->mValue.chatTime;
			chatDataRoot["playeruid"] = chatIter->mValue.playerUid;

			chatDataRoot["modelid"] = player->getPlayerModelID();
			chatDataRoot["name"] = player->getPlayerName();
			
			if(player->getConnId())
			{
				chatDataRoot["onLine"] = 1;
			}
			else
			{
				chatDataRoot["onLine"] = 0;
			}

			root["chatplayers"].append(chatDataRoot);
		}
		else
		{
			
			ConsortSystem::getSingleton().chatListJson(root["chatplayers"],chatIter->mValue.playerUid,
				chatIter->mValue.chatTime);

		}
		
	}


	Json::StyledWriter writer;
	std::string jsonStr = writer.write(root);

	//cout << jsonStr;
	//LOG_INFO("chat list is :%s",jsonStr.c_str() );
	return jsonStr;


}


void ChatManager::receiveMsg(UInt32 channelType, UInt64 fromPlayer, UInt64 toPlayer, std::string ChatMsg)
{
	const ConnId& connId = m_pPlayer->getConnId();
	
	if(channelType == CONSORTMASG)
	{
		const PlayerConsortData & playerconsortdata = m_pPlayer->getPlayerConsortData();
		LogicSystem::getSingleton().sendSystemMsg(10, ChatMsg, playerconsortdata.m_nConsortId);
	}

	if(channelType == WORLDCHAT)
	{
		//chang by cqy
		GCClientChatResp clientChatResp;
		clientChatResp.mPacketID = BOC_CLIENTCHAT_RESP;
		Json::Value root;
		root["channelType"] = channelType;
		root["fromPlayer"] = fromPlayer;
		root["chatMsg"] = ChatMsg;
		root["modelid"] = m_pPlayer->getPlayerModelID();
		root["name"] = m_pPlayer->getPlayerName();
		root["chattime"] = TimeUtil::getTimeSec();

		Json::StyledWriter writer;

		clientChatResp.mRespJsonStr = writer.write(root);

		std::string sdkstr = RedisManager().getSingleton().get("sdkchat");  
		sdkstr.append("!@#$");
		sdkstr.append(clientChatResp.mRespJsonStr);
		RedisManager().getSingleton().set("sdkchat",sdkstr );

		const Map<ConnId, Player*> connectionMap = LogicSystem::getSingleton().getPlayerConnectionMap();

		LOG_WARN("connectionMap size = %d",connectionMap.size());

		for( Map<ConnId, Player*>::Iter* mapIter = connectionMap.begin(); mapIter != NULL; 
			mapIter = connectionMap.next(mapIter))
		{
			//不是发送者
			if(mapIter->mKey != connId)
			{
				cout << clientChatResp.mRespJsonStr << endl;
				
				NetworkSystem::getSingleton().sendMsg(clientChatResp, mapIter->mKey);

				
			}
			
		}

	}

	if(channelType == CONSORTIACHAT)
	{
		
			const PlayerConsortData & consortData = m_pPlayer->getPlayerConsortData();
		
			ConsortSystem::getSingleton().sendConsortMsg(0, consortData.m_nConsortId, m_pPlayer->getPlayerGuid(), m_pPlayer->getPlayerName().c_str(), ChatMsg,"",
				TimeUtil::getTimeSec());
		

		
	}

	if(channelType == PRIVATECHAT)
	{
		Player*  player = LogicSystem::getSingleton().getPlayerByGuid(toPlayer);

		if(!player)
		{
			GCOffLineNotify offLineNotify;
			offLineNotify.mPacketID = BOC_OFFLINE_NOTIFY;
			Json::Value root;
			root["playeruid"] = toPlayer;

			Json::StyledWriter writer;

			offLineNotify.mRespJsonStr = writer.write(root);

			NetworkSystem::getSingleton().sendMsg(offLineNotify, connId);

			cout << offLineNotify.mRespJsonStr;

			return;
		}

		const ConnId& toConnId = player->getConnId();

		if(!toConnId)
		{
			GCOffLineNotify offLineNotify;
			offLineNotify.mPacketID = BOC_OFFLINE_NOTIFY;
			Json::Value root;
			root["playeruid"] = toPlayer;

			Json::StyledWriter writer;

			offLineNotify.mRespJsonStr = writer.write(root);

			NetworkSystem::getSingleton().sendMsg(offLineNotify, connId);

			cout << offLineNotify.mRespJsonStr;

			return; 
		}

		player->getChatManager().insertPlayerToChatList(fromPlayer);

		
		GCClientChatResp clientChatResp;
		clientChatResp.mPacketID = BOC_CLIENTCHAT_RESP;

		Json::Value root;
		root["channelType"] = channelType;
		root["fromPlayer"] = fromPlayer;
		root["chatMsg"] = ChatMsg;
		root["modelid"] = m_pPlayer->getPlayerModelID();
		root["name"] = m_pPlayer->getPlayerName();
		root["chattime"] = TimeUtil::getTimeSec();

		Json::StyledWriter writer;

		clientChatResp.mRespJsonStr = writer.write(root);

		cout << clientChatResp.mRespJsonStr;

		NetworkSystem::getSingleton().sendMsg(clientChatResp, toConnId);

	}

	
}


void ChatManager::clientForbidChat(UInt64 playerUid, UInt32 timelong, UInt64 timebegin)
{
	//const ConnId& connId = m_pPlayer->getConnId();


	m_pPlayer->mPlayerData.mChatData.disableFlag = true;

	m_pPlayer->mPlayerData.mChatData.disableBeginTime = timebegin;

	m_pPlayer->mPlayerData.mChatData.disableTimeLong = timelong;

	m_pPlayer->getPersistManager().setDirtyBit(CHATDATABIT);

	const ConnId& connId = m_pPlayer->getConnId();
	if(connId)
	{
			Json::StyledWriter writer;
	
			GCForbidChatNotify forbidNotify;
			forbidNotify.mPacketID = BOC_FORBIDCHAT_NOTIFY;
			Json::Value root;
			root["playeruid"] = playerUid;
			root["timelong"] = timelong;
			root["timebegin"] = timebegin;
			root["nowtime"] = UInt64(time(0));
	
			forbidNotify.mRespJsonStr = writer.write(root);
			LOG_INFO("forbid msg is = %s",forbidNotify.mRespJsonStr.c_str())  ;
			NetworkSystem::getSingleton().sendMsg(forbidNotify,connId);
	}

}

void ChatManager::clientResetChat(UInt64 playerUid)
{
	m_pPlayer->mPlayerData.mChatData.disableFlag = false;
	
	m_pPlayer->mPlayerData.mChatData.disableBeginTime = 0;

	m_pPlayer->mPlayerData.mChatData.disableTimeLong = 0;

	m_pPlayer->getPersistManager().setDirtyBit(CHATDATABIT);

}




//GM发送的公告信息
void ChatManager::sendGMNotifyMsg(std::string msg, UInt64 dealline,  UInt64 terminal)
{

}


void ChatManager::checkOnline(UInt64 playerUid)
{
	const ConnId& connId = m_pPlayer->getConnId();

	Json::Value root;
	for(List<ChatData>::Iter * chatDataIter =  m_pChatList->begin(); chatDataIter != NULL; 
		chatDataIter = m_pChatList->next(chatDataIter) )
	{
		Player*  chatPlayer = LogicSystem::getSingleton().getPlayerByGuid(chatDataIter->mValue.playerUid);
		//内存中没有该玩家说明已经离线
		if(!chatPlayer)
		{
			Json::Value chatRoot;
			chatRoot["playeruid"] = chatDataIter->mValue.playerUid;
			chatRoot["onLine"] = 0;

			root.append(chatRoot);

		}
		else
		{
			const ConnId & connId = chatPlayer->getConnId();
			//玩家connid为0表示离线，非0表示在线
			if(connId)
			{
				Json::Value chatRoot;
				chatRoot["playeruid"] = chatDataIter->mValue.playerUid;
				chatRoot["onLine"] = 1;

				root.append(chatRoot);
			}
			else
			{
				Json::Value chatRoot;
				chatRoot["playeruid"] = chatDataIter->mValue.playerUid;
				chatRoot["onLine"] = 0;

				root.append(chatRoot);
			}
			
			

		}
	}

	Json::StyledWriter writer;
	
	GCChatOnlineResp chatOnlineResp;
	chatOnlineResp.mPacketID = BOC_CLIENTONLINE_RESP;
	chatOnlineResp.mRespJsonStr = writer.write(root);
	cout << chatOnlineResp.mRespJsonStr;
	NetworkSystem::getSingleton().sendMsg(chatOnlineResp,connId);

}