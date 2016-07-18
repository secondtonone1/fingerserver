#include "SystemAwardMsgHandler.h"
#include "GameServer.h"

using namespace Lynx;

void 
SystemAwardMsgHandler::onGetAccumulateLoginAwardReq(const ConnId& connId, GetAccumulateLoginAwardReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get Player by connid: %llu", connId);
        return;
    }
    GetAccumulateLoginAwardResp resp;
    resp.mErrorId = player->getSystemAwardManager().getAccumulateLoginAward(msg.mLoginDay);
    player->sendMsgWithAck(resp);
}

void 
SystemAwardMsgHandler::onGetContinousLoginAwardReq(const ConnId& connId, GetContinousLoginAwardReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get Player by connid: %llu", connId);
        return;
    }
    GetContinousLoginAwardResp resp;
    resp.mErrorId = player->getSystemAwardManager().getContinousLoginAward(msg.mLoginDay);
    player->sendMsgWithAck(resp);
}

void 
SystemAwardMsgHandler::onGetLevelAwardReq(const ConnId& connId, GetLevelAwardReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get Player by connid: %llu", connId);
        return;
    }
    GetLevelAwardResp resp;
    resp.mErrorId = player->getSystemAwardManager().getLevelAward(msg.mLevel);
    player->sendMsgWithAck(resp);
}
