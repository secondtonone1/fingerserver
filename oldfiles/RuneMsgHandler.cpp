#include "RuneMsgHandler.h"
#include "LogicSystem.h"
#include "GameServer.h"

using namespace Lynx;

void 
RuneMsgHandler::onUseRuneReq(const ConnId& connId, UseRuneReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }
    UseRuneResp resp;
    resp.mErrno = player->getRuneManager().useRune(msg.mRuneGuid, msg.mPresentPos);
    player->sendMsgWithAck(resp);
}

void 
RuneMsgHandler::onRefineRuneReq(const ConnId& connId, RefineRuneReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    RefineRuneResp resp;
    resp.mRuneId = msg.mRuneId;
    resp.mErrno = player->getRuneManager().refineRune(resp.mRuneId, msg.mItemList, resp.mRefineLevel);
    player->sendMsgWithAck(resp);
}

void 
RuneMsgHandler::onSellRuneReq(const ConnId& connId, SellRuneReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    SellRuneResp resp;
    resp.mErrorId = player->getRuneManager().sellRune(msg.mSellList, resp.mGetGold);
    player->sendMsgWithAck(resp);
}

void 
RuneMsgHandler::onCombineRuneReq(const ConnId& connId, CombineRuneReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get player by connId %llu", connId);
        return;
    }

    CombineRuneResp resp;
    resp.mErrorId = player->getItemManager().combineRune(msg.mItemId, resp.mRuneId);
    player->sendMsgWithAck(resp);
}