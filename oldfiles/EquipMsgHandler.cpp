#include "EquipMsgHandler.h"
#include "LogicSystem.h"
#include "GameServer.h"

using namespace Lynx;

void 
EquipMsgHandler::onUseEquipReq(const ConnId& connId, UseEquipReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    UseEquipResp resp;
    resp.mErrno = player->getEquipManager().useEquip(msg.mEquipGuid, msg.mPresentPos);
    player->sendMsgWithAck(resp);
}

void 
EquipMsgHandler::onRefineEquipReq(const ConnId& connId, RefineEquipReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get player by connId %llu", connId);
        return;
    }

    RefineEquipResp resp;
    resp.mEquipId = msg.mEquipId;

    if (msg.mAutoRefine)
    {
        resp.mErrno = player->getEquipManager().autoRefineEquip(msg.mEquipId, resp.mRefineLevel);
    }
    else
    {
        resp.mErrno = player->getEquipManager().refineEquip(msg.mEquipId, resp.mRefineLevel);
    }

    player->sendMsgWithAck(resp);
}

void 
EquipMsgHandler::onEnchantEquipReq(const ConnId& connId, EnchantEquipReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get player by connId %llu", connId);
        return;
    }

    EnchantEquipResp resp;
    resp.mEquipId = msg.mEquipId;
    resp.mErrno = player->getEquipManager().enchantEquip(msg.mEquipId, resp.mEnchantLevel);
    player->sendMsgWithAck(resp);
}

void 
EquipMsgHandler::onCombineEquipReq(const ConnId& connId, CombineEquipReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("failed to get Player by connId %llu", connId);
        return;
    }
    
    CombineEquipResp resp;
    resp.mErrno = player->getItemManager().combineEquip(msg.mEquipDebrisId, resp.mEquipId);
    player->sendMsgWithAck(resp);
}

void 
EquipMsgHandler::onSellEquipReq(const ConnId& connId, SellEquipReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("failed to get Player by connId %llu", connId);
        return;
    }
    SellEquipResp resp;
    resp.mErrorId = player->getEquipManager().sellEquip(msg.mSellList, resp.mGetGold);
    player->sendMsgWithAck(resp);
}