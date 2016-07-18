#include "CommonMsgHandler.h"
#include "LogicSystem.h"
#include "MsgfireInfoToJson.h"
#include "FireConfirm/FireConfirmManager.h"

using namespace Lynx;

void
CommonMsgHandler::onMsgAckNotify(const ConnId& connId, MsgAckNotify& msg)
{
	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!player)
	{
		LOG_DEBUG("Failed to get player by connId %llu", connId);
		return;
	}

	player->onMsgAckNotify(msg.mAckId);
}

void 
CommonMsgHandler::onSyncTimeReq(const ConnId& connId, SyncTimeReq& msg)
{
    MsgServerTimeRespToJson resp;

	msg.convertJsonToData(msg.strRec);
    resp.mClientTime = msg.mClientTime;
    resp.mServerTime = TimeUtil::getTimeSec();
	std::string jsonStr = resp.convertDataToJson();	

    NetworkSystem::getSingleton().sender( connId,SYNC_TIME_RESP,jsonStr);
}


void 
CommonMsgHandler::onKeepalivePingReq(const ConnId& connId, KeepalivePingReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!player)
	{
		LOG_DEBUG("Failed to get player by connId %llu", connId);
		return;
	}

    if (player->getPlayerGuid() != msg.mPlayerGuid)
    {
        LOG_DEBUG("Connection don't match with player.");
        return;
    }

    KeepalivePingResp resp;
    player->sendMsg(resp);
}

