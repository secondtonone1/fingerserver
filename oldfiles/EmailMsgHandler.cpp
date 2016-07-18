#include "EmailMsgHandler.h"
#include "LogicSystem.h"
#include "GameServer.h"

using namespace Lynx;

void 
EmailMsgHandler::onDelEmailReq(const ConnId& connId, DelEmailReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get player by connId %llu", connId);
        return;
    }

    DelEmailResp resp;
    resp.mEmailId = msg.mEmailId;
    resp.mErrno = player->getEmailManager().destroyEmail(msg.mEmailId);

    player->sendMsgWithAck(resp);
}

void 
EmailMsgHandler::onTakeEmailItemReq(const ConnId& connId, TakeEmailItemReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get player by connId %llu", connId);
        return;
    }

    TakeEmailItemResp resp;
    resp.mEmailId = msg.mEmailId;
    resp.mErrno = player->getEmailManager().takeEmailItem(msg.mEmailId);
    player->sendMsgWithAck(resp);
}

void 
EmailMsgHandler::onOpenEmailNotify(const ConnId& connId, OpenEmailNotify& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get player by connId %llu", connId);
        return;
    }

    player->getEmailManager().openEmail(msg.mEmailId);
}

