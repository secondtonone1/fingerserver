#include "ActivityMsgHandler.h"
#include "LogicSystem.h"
#include "GameServer.h"

using namespace Lynx;

void
ActivityMsgHandler::onActivityRuneBattleReq(const ConnId& connId, ActivityRuneBattleReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    player->getActivityManager().startRuneBattle(msg.mTollgateId);
}

void
ActivityMsgHandler::onActivityRuneResultReq(const ConnId& connId, ActivityRuneResultReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    player->getActivityManager().runeBattleResult(msg);
}

void
ActivityMsgHandler::onActivityGoldBattleReq(const ConnId& connId, ActivityGoldBattleReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    player->getActivityManager().startGoldBattle(msg.mTollgateId);
}

void
ActivityMsgHandler::onActivityGoldResultReq(const ConnId& connId, ActivityGoldResultReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    player->getActivityManager().goldBattleResult(msg);
}

void
ActivityMsgHandler::onActivityPetBattleReq(const ConnId& connId, ActivityPetBattleReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    player->getActivityManager().startPetBattle(msg.mTollgateId);
}

void
ActivityMsgHandler::onActivityPetResultReq(const ConnId& connId, ActivityPetResultReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    player->getActivityManager().petBattleResult(msg);
}

void
ActivityMsgHandler::onActivityEnchantBattleReq(const ConnId& connId, ActivityEnchantBattleReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    player->getActivityManager().startEnchantBattle(msg.mTollgateId);
}

void
ActivityMsgHandler::onActivityEnchantResultReq(const ConnId& connId, ActivityEnchantResultReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    player->getActivityManager().enchantBattleResult(msg);
}

void
ActivityMsgHandler::onActivityRuneNumberReq(const ConnId& connId, ActivityRuneNumberReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    player->getActivityManager().runeNumber(msg);    
}

void
ActivityMsgHandler::onActivityRefreshSettingReq(const ConnId& connId, ActivityRefreshSettingReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    player->getActivityManager().refreshSetting();

}