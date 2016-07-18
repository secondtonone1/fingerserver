#include "TollgateMsgHandler.h"
#include "LogicSystem.h"
#include "GameServer.h"

using namespace Lynx;

void
TollgateMsgHandler::onStartTollgateBattleReq(const ConnId& connId, StartTollgateBattleReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    player->getTollgateManager().startTollgateBattle(msg.mTollgateGuid, msg.mTollgateDifficulty);
}

void
TollgateMsgHandler::onStartFiveTollgateSweepReq(const ConnId& connId, StartFiveTollgateSweepReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    player->getTollgateManager().startFiveTollgateSweep(msg.mTollgateGuid, msg.mTollgateDifficulty);
}

void 
TollgateMsgHandler::onTollgateBattleResultReq(const ConnId& connId, TollgateBattleResultReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    player->getTollgateManager().tollgateBattleResult(msg);
}

void
TollgateMsgHandler::onAgainTollgateBattleReq(const ConnId& connId, AgainTollgateBattleReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    player->getTollgateManager().againTollgateBattle(msg.mTollgateGuid, msg.mTollgateDifficulty);
}

void
TollgateMsgHandler::onGetScenePassAwardReq(const ConnId& connId, GetScenePassAwardReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    player->getTollgateManager().getScenePassAward(msg.mSceneId, msg.mChestsIndex);
}

void
TollgateMsgHandler::onStartStoreHouseBattleReq(const ConnId& connId, StartStoreHouseBattleReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    player->getTollgateManager().startStoreHouseBattle(msg.mTollgateId);
}

void 
TollgateMsgHandler::onStoreHouseBattleResultReq(const ConnId& connId, StoreHouseBattleResultReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    player->getTollgateManager().storeHouseBattleResult(msg);
}
