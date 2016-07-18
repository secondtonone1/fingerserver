#include "TowerMsgHandler.h"
#include "LogicSystem.h"
#include "GameServer.h"

using namespace Lynx;

void
TowerMsgHandler::onOpenTowerBufferReq(const ConnId& connId, OpenTowerBufferReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    player->getTowerManager().openTowerBuffer();
}

void
TowerMsgHandler::onSelectBufferReq(const ConnId& connId, SelectBufferReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    player->getTowerManager().selectTowerBuffer(msg.mSelectIndex);
}

void
TowerMsgHandler::onStartTowerBattleReq(const ConnId& connId, StartTowerBattleReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    player->getTowerManager().startTowerBattle(msg.mTollgateDifficulty);
}

void
TowerMsgHandler::onTowerBattleResultReq(const ConnId& connId, TowerBattleResultReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    player->getTowerManager().towerBattleResult(msg);
}

void
TowerMsgHandler::onResetTowerReq(const ConnId& connId, ResetTowerReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    player->getTowerManager().resetTower();
}

void
TowerMsgHandler::onSweepTowerReq(const ConnId& connId, SweepTowerReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    player->getTowerManager().sweepTower();
}

void
TowerMsgHandler::onOpenTowerRankReq(const ConnId& connId, OpenTowerRankReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_DEBUG("Failed to get player by connId %llu", connId);
        return;
    }

    LogicSystem::getSingleton().openTowerRank(player->getPlayerGuid());
}
