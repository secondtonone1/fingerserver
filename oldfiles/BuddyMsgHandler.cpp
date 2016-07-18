#include "BuddyMsgHandler.h"
#include "GameServer.h"

using namespace Lynx;

void
BuddyMsgHandler::onInviteBuddyReq(const ConnId& connId, InviteBuddyReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    InviteBuddyResp resp;
    resp.mBuddyGuid = msg.mBuddyGuid;
    resp.mErrno = player->getBuddyManager().inviteBuddy(msg.mBuddyGuid);
    player->sendMsg(resp);
}

void
BuddyMsgHandler::onAddBuddyReq(const ConnId& connId, AddBuddyReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    AddBuddyResp resp;
    resp.mBuddyGuid = msg.mBuddyGuid;
    resp.mErrno = player->getBuddyManager().addBuddy(msg.mBuddyGuid);
    player->sendMsg(resp);
}


void
BuddyMsgHandler::onDelBuddyReq(const ConnId& connId, DelBuddyReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    DelBuddyResp resp;
    resp.mBuddyGuid = msg.mBuddyGuid;
    resp.mErrno = player->getBuddyManager().delBuddy(msg.mBuddyGuid);
    player->sendMsg(resp);
}

void 
BuddyMsgHandler::onSyncBuddyListReq(const ConnId& connId, SyncBuddyListReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    SyncBuddyListResp resp;
    player->getBuddyManager().syncBuddyList(resp.mAddBuddyDataList, 
        resp.mChangeBuddyDataList, resp.mDelBuddyDataList);

    player->sendMsg(resp);
}

void 
BuddyMsgHandler::onSyncInviteBuddyListReq(const ConnId& connId, SyncInviteBuddyListReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    SyncInviteBuddyListResp resp;
    player->getBuddyManager().syncInviteBuddyList(resp.mAddBuddyDataList, resp.mChangeBuddyDataList,
        resp.mDelBuddyDataList);
    player->sendMsg(resp);
}

void
BuddyMsgHandler::onGetRecommendBuddyListReq(const ConnId& connId, GetRecommendBuddyListReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    GetRecommendBuddyListResp resp;
    player->getBuddyManager().getRecommendBuddyList(resp.mBuddyDataList);
    player->sendMsg(resp);
}

void 
BuddyMsgHandler::onRejectBuddyNotify(const ConnId& connId, RejectBuddyNotify& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    player->getBuddyManager().rejectBuddy(msg.mBuddyGuid);
}

