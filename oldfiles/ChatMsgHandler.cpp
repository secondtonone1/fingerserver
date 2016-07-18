#include "ChatMsgHandler.h"
#include "GameServer.h"

using namespace Lynx;

void
ChatMsgHandler::onSendChatMsgNotify(const ConnId& connId, SendChatMsgNotify& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!player)
	{
		LOG_DEBUG("Failed to get player by connId %llu", connId);
		return;
	}

    if (msg.mChatData.mReceiverGuid == player->getPlayerGuid())
    {
        LOG_WARN("Failed to send msg to myself %llu", player->getPlayerGuid());
        ForbiddenChatNotify notify;
        notify.mWhy = 2;
        player->sendMsg(notify);
        return;
    }

    UInt32 currTime = TimeUtil::getTimeSec();
    if (currTime - player->getLastChatTime() <= 3)
    {
        ForbiddenChatNotify notify;
        notify.mWhy = 1;
        player->sendMsg(notify);
        return;
    }
    player->updateLastChatTime();

    msg.mChatData.mSenderGuid = player->getPlayerGuid();
    msg.mChatData.mSenderName = player->getPlayerName().c_str();
    msg.mChatData.mSenderVipLevel = player->getPlayerData().mBaseData.m_nVipLevel;
    //msg.mChatData.mSenderIcon = player->getPlayerData().mBaseData.mIcon; sec
    msg.mChatData.mSenderLevel = player->getPlayerData().mBaseData.m_nLevel;
    //msg.mChatData.mSenderBattleValue = player->getPlayerData().mBaseData.mBattleValue; sec

    LogicSystem::getSingleton().insertChatData(msg.mChatData);
}


void
ChatMsgHandler::onGetChatMsgReq(const ConnId& connId, GetChatMsgReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!player)
	{
		LOG_DEBUG("Failed to get player by connId %llu", connId);
		return;
	}

    GetChatMsgResp resp;
    resp.mChannel = msg.mChannel;

    LogicSystem::getSingleton().getChatData(player, msg.mChannel, resp.mChatDataList);

    if (resp.mChatDataList.empty())
    {

        return ;
    }

    player->sendMsg(resp);
}
